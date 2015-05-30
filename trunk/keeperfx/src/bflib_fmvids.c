/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_fmvids.c
 *     Full Motion Videos (Smacker,FLIC) decode & play library.
 * @par Purpose:
 *     Routines to create and decode videos.
 * @par Comment:
 *     None.
 * @author   KeeperFX Team
 * @date     27 Nov 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "bflib_fmvids.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <windef.h>
#include <winbase.h>

#include "bflib_basics.h"
#include "bflib_memory.h"
#include "bflib_sndlib.h"
#include "bflib_video.h"
#include "bflib_keybrd.h"
#include "bflib_inputctrl.h"
#include "bflib_fileio.h"
#include "bflib_vidsurface.h"

/** Informs if the application window is active (focused on screen). */
extern volatile TbBool lbAppActive;

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
// Constants and defines
#define FLI_PREFIX 0xF100u
#define FLI_COLOR256 4
#define FLI_SS2      7
#define FLI_COLOR   11
#define FLI_LC      12
#define FLI_BLACK   13
#define FLI_BRUN    15
#define FLI_COPY    16
#define FLI_PSTAMP  18

VideoState *global_video_state;
SDL_mutex       *screen_mutex;

extern size_t av_strlcpy(char *dst, const char *src, size_t size);

#pragma region decoder helper

#pragma region audio
int _audio_decode_frame(AVCodecContext *aCodecCtx, uint8_t *audio_buf, int buf_size)
{
    static AVPacket pkt;
    static uint8_t *audio_pkt_data = NULL;
    static int audio_pkt_size = 0;
    static AVFrame frame;

    int len1, data_size = 0;

    while (!global_video_state->quit)
    {
        while (audio_pkt_size > 0)
        {
            int got_frame = 0;
            len1 = avcodec_decode_audio4(aCodecCtx, &frame, &got_frame, &pkt);
            if (len1 < 0)
            {
                /* if error, skip frame */
                audio_pkt_size = 0;
                break;
            }
            audio_pkt_data += len1;
            audio_pkt_size -= len1;
            data_size = 0;
            if (got_frame)
            {
                data_size = av_samples_get_buffer_size(NULL,
                    aCodecCtx->channels,
                    frame.nb_samples,
                    aCodecCtx->sample_fmt,
                    1);
                assert(data_size <= buf_size);
                memcpy(audio_buf, frame.data[0], data_size);
            }
            if (data_size <= 0)
            {
                /* No data yet, get more frames */
                continue;
            }
            /* We have data, return it and come back for more later */
            return data_size;
        }
        if (pkt.data)
            av_free_packet(&pkt);

        if (_packet_queue_get(&(global_video_state->audio_queue), &pkt, 1) < 0 || !lbAppActive)
        {
            return -1;
        }

        audio_pkt_data = pkt.data;
        audio_pkt_size = pkt.size;
    }

    return -1;
}

void _audio_callback(void * userdata, Uint8 *stream, int len)
{
    int len1, audio_size;

    static uint8_t audio_buf[(AVCODEC_MAX_AUDIO_FRAME_SIZE * 3) / 2];
    static unsigned int audio_buf_size = 0;
    static unsigned int audio_buf_index = 0;

    while (global_video_state &&
        !global_video_state->mute &&
        global_video_state->audio_ctx &&
        !global_video_state->quit &&
        (len > 0))
    {
        if (audio_buf_index >= audio_buf_size)
        {
            /* We have already sent all our data; get more */
            audio_size = _audio_decode_frame(global_video_state->audio_ctx, audio_buf, sizeof(audio_buf));
            if (audio_size < 0)
            {
                /* If error, output silence */
                audio_buf_size = 1024;
                memset(audio_buf, 0, audio_buf_size);
            }
            else
            {
                audio_buf_size = audio_size;
            }
            audio_buf_index = 0;
        }
        len1 = audio_buf_size - audio_buf_index;
        if (len1 > len)
            len1 = len;
        memcpy(stream, (uint8_t *)audio_buf + audio_buf_index, len1);
        len -= len1;
        stream += len1;
        audio_buf_index += len1;
    }
}

#pragma endregion

#pragma region packet queue
// Initialize the packet queue.
void _packet_queue_init(PacketQueue *queue)
{
    memset(queue, 0, sizeof(PacketQueue));
    queue->mutex = SDL_CreateMutex();
    queue->cond = SDL_CreateCond();
}

void _packet_queue_destroy(PacketQueue *queue)
{
    SDL_DestroyMutex(queue->mutex);
    SDL_DestroyCond(queue->cond );

    AVPacketNode *curNode;
    curNode = queue->first_pkt;
    while (curNode && queue->nb_packets > 0)
    {
        queue->size -= curNode->pkt.size;
        av_free_packet(&curNode->pkt);
        queue->nb_packets--;

        curNode = curNode->next;
    }

    queue->first_pkt = NULL;
    queue->last_pkt = NULL;
}

// Add packet to packet queue.
int _packet_queue_put(PacketQueue *queue, AVPacket *pPacket)
{
    AVPacketNode *pNode;

    pNode = (AVPacketNode*)av_malloc(sizeof(AVPacketNode));
    if (!pNode)
    {
        return -1;
    }
    pNode->pkt = *pPacket;
    pNode->next = NULL;

    SDL_LockMutex(queue->mutex);

    if (!queue->last_pkt)//queue videoState empty
    {
        queue->first_pkt = pNode;
    }
    else
    {
        queue->last_pkt->next = pNode;
    }

    queue->last_pkt = pNode;
    queue->nb_packets++;
    queue->size += pNode->pkt.size;
    SDL_CondSignal(queue->cond);

    SDL_UnlockMutex(queue->mutex);
    return 0;
}

// Get packet from packet queue.
int _packet_queue_get(PacketQueue *queue, AVPacket *pPacket, int block)
{
    AVPacketNode *pNode;
    int ret;

    SDL_LockMutex(queue->mutex);

    while (!global_video_state->quit)
    {
        pNode = queue->first_pkt;
        if (pNode)
        {
            queue->first_pkt = pNode->next;
            if (!queue->first_pkt) // Queue empty
            {
                queue->last_pkt = NULL;
            }
            queue->nb_packets--;
            queue->size -= pNode->pkt.size;
            *pPacket = pNode->pkt;
            av_free(pNode);
            ret = 1;
            break;
        }
        /*
        else if (!block) {
            ret = 0;
            break;
        }*/
        else
        {
            // wait until a condition variable is signaled.
            SDL_CondWait(queue->cond, queue->mutex);
        }
    }
    SDL_UnlockMutex(queue->mutex);
    return ret;
}
#pragma endregion

#pragma region timer helper
static Uint32 _sdl_refresh_timer_callback(Uint32 interval, void *arg)
{
    SDL_Event event;
    event.type = FF_REFRESH_EVENT;
    event.user.data1 = arg;
    SDL_PushEvent(&event);
    return 0; /* 0 means stop timer */
}

/* schedule a video refresh in 'delay' ms */
static void _schedule_refresh(VideoState *videoState, int delay)
{
    SDL_AddTimer(delay, _sdl_refresh_timer_callback, videoState);
}

double _synchronize_video(VideoState *videoState, AVFrame *src_frame, double pts)
{
    double frame_delay;

    if (pts != 0)
    {
        /* if we have pts, set video clock to it */
        videoState->video_clock = pts;
    }
    else
    {
        /* if we aren't given a pts, set it to the clock */
        pts = videoState->video_clock;
    }
    /* update the video clock */
    frame_delay = av_q2d(videoState->video_stream->codec->time_base);
    /* if we are repeating a frame, adjust clock accordingly */
    frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);
    videoState->video_clock += frame_delay;
    return pts;
}

#pragma endregion

#pragma region display
void _video_display(VideoPicture *videoPicture)
{
    assert(lbDrawTexture == NULL);

    if (videoPicture && videoPicture->texture)
    {
        lbDrawTexture = videoPicture->texture;

        SDL_LockMutex(screen_mutex);

        if (LbScreenLock() == Lb_SUCCESS)
        {
            LbScreenRender();
            LbScreenUnlock();
        }

        SDL_DestroyTexture(videoPicture->texture);
        videoPicture->texture = NULL;

        SDL_UnlockMutex(screen_mutex);
    }
}

// callback when timer videoState up
void video_refresh_timer_callback(void *userdata)
{        
    VideoState *videoState = (VideoState *)userdata;
    VideoPicture *videoPicture;

    if (videoState &&
        !videoState->quit)
    {
        if (videoState->video_stream)
        {
            if (videoState->pict_queue_size == 0)
            {
                if (videoState->no_more_packet)
                {
                    videoState->quit = 1;
                }
                else
                {
                    _schedule_refresh(videoState, 10);
                }
            }
            else
            {
                videoPicture = &videoState->pict_queue[videoState->pict_queue_read_idx];
                /* Now, normally here goes a ton of code
                about timing, etc. we're just going to
                guess at a delay for now. You can
                increase and decrease this value and hard code
                the timing - but I don't suggest that ;)
                We'll learn how to do it for real later.
                */
                _schedule_refresh(videoState, 40);

                /* show the picture! */
                _video_display(videoPicture);

                /* update queue for next picture! */
                if (++videoState->pict_queue_read_idx == VIDEO_PICTURE_QUEUE_SIZE)
                {
                    videoState->pict_queue_read_idx = 0;
                }

                if (videoState && videoState->pict_queue_cond && videoState->pict_queue_mutex)
                {
                    SDL_LockMutex(videoState->pict_queue_mutex);
                    videoState->pict_queue_size--;
                    SDL_CondSignal(videoState->pict_queue_cond);
                    SDL_UnlockMutex(videoState->pict_queue_mutex);
                }

                if (videoState->no_more_packet && (videoState->pict_queue_size == 0))
                {
                    videoState->quit = 1;
                }
            }
        }
        else
        {
            if (videoState->no_more_packet)
            {
                videoState->quit = 1;
            }
            else
            {
                _schedule_refresh(videoState, 10);
            }
        }
    }
}
#pragma endregion

#pragma region threads
// Open the smacker video file, parse its header, get the video and audio stream.
int _open_smacker_video(VideoState *videoState)
{
    int result = 0;
    // Open video file and read the header.
    if (avformat_open_input(&(videoState->p_format_ctx), videoState->filename, NULL, NULL) != 0)
    {
        ERRORLOG("Couldn't open file.");
        result = -1;
        goto ERROR;
    }

    // Retrieve stream information.
    if (avformat_find_stream_info(videoState->p_format_ctx, NULL) < 0)
    {
        ERRORLOG("Couldn't find stream information.");
        result = -1;
        goto ERROR;
    }

    int i;
    // Find the first video and audio stream.
    for (i = 0; i < videoState->p_format_ctx->nb_streams; i++)
    {
        if ((videoState->p_format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) &&
            (videoState->video_stream_idx< 0))
        {
            videoState->video_stream_idx = i;
        }
        if ((videoState->p_format_ctx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) &&
            (videoState->audio_stream_idx < 0))
        {
            videoState->audio_stream_idx = i;
        }
    }

    if (videoState->video_stream_idx == -1 || videoState->audio_stream_idx == -1)
    {
        ERRORLOG("Didn't find video or audio stream.");
        result = -1;
        goto ERROR;
    }

    if ((_open_stream_component(videoState, videoState->video_stream_idx) < 0) ||
        (_open_stream_component(videoState, videoState->audio_stream_idx) < 0))
    {
        result = -1;
        goto ERROR;
    }

ERROR:
    return result;
}

// Open codec for specified stream and fill the context structure.
int _open_stream_component(VideoState *videoState, int streamIdx)
{
    int result = 0;

    AVFormatContext* pFormatCtx = videoState->p_format_ctx;

    // Get a pointer to the codec context for the video stream.
    AVCodecContext* pCodecCtxOrig = pFormatCtx->streams[streamIdx]->codec;

    AVCodec* pCodec = NULL;

    if (streamIdx < 0 || streamIdx >= pFormatCtx->nb_streams)
    {
        result = -1; // Codec not found.
        goto ERROR;
    }

    // Find the decoder for stream.
    pCodec = avcodec_find_decoder((pCodecCtxOrig)->codec_id);
    if (pCodec == NULL)
    {
        ERRORLOG("Unsupported codec!");
        result = -1; // Codec not found.
        goto ERROR;
    }


    AVCodecContext* pCodecCtx = NULL;
    pCodecCtx = avcodec_alloc_context3(pCodec);
    if (avcodec_copy_context(pCodecCtx, pCodecCtxOrig) != 0)
    {
        ERRORLOG("Couldn't copy codec context");
        result = -1; // Error copying codec context
        goto ERROR;
    }

    // Open codec
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0)
    {
        result = -1; // Could not open codec
        goto ERROR;
    }


    SDL_AudioSpec desiredSpec, actualSpec;
    switch (pCodecCtx->codec_type)
    {
    case AVMEDIA_TYPE_AUDIO:

        _packet_queue_init(&videoState->audio_queue);

        if (!videoState->mute)
        {
            // Set audio settings from codec info
            desiredSpec.freq = pCodecCtx->sample_rate;
            desiredSpec.format = AUDIO_S16SYS;
            desiredSpec.channels = pCodecCtx->channels;
            desiredSpec.silence = 0;
            desiredSpec.samples = SDL_AUDIO_BUFFER_SIZE;
            desiredSpec.callback = _audio_callback;
            desiredSpec.userdata = NULL;

            if (SDL_OpenAudio(&desiredSpec, &actualSpec) < 0)
            {
                ERRORLOG("SDL_OpenAudio: %s\n", SDL_GetError());
                result = -1;
                goto ERROR;
            }

            videoState->audio_stream_idx = streamIdx;
            videoState->audio_stream = pFormatCtx->streams[streamIdx];
            videoState->audio_ctx = pCodecCtx;

            // Start audio.
            SDL_PauseAudio(0);
        }
        break;
    case AVMEDIA_TYPE_VIDEO:

        _packet_queue_init(&videoState->video_queue);

        videoState->video_stream_idx = streamIdx;
        videoState->video_stream = pFormatCtx->streams[streamIdx];
        videoState->video_ctx = pCodecCtx;

        videoState->video_thread_id = SDL_CreateThread(_video_thread, NULL, videoState);
        if (!videoState->video_thread_id)
        {
            ERRORLOG("failed creating video thread.");
            result = -1;
            goto ERROR;
        }

        // initialize SWS context for software scaling.
        videoState->sws_ctx = sws_getContext(pCodecCtx->width,
            pCodecCtx->height,
            pCodecCtx->pix_fmt,
            lbDisplay.PhysicalScreenWidth,
            lbDisplay.PhysicalScreenHeight,
            PIX_FMT_RGB24,
            SWS_LANCZOS,
            NULL,
            NULL,
            NULL
            );

        break;
    default:
        break;
    }

ERROR:
    avcodec_close(pCodecCtxOrig);
    return result;
}

int _decode_thread(void *arg)
{
    int result = 0;
    VideoState *videoState = (VideoState *)arg;

    AVPacket packet, *pPacket = &packet;

    videoState->video_stream_idx = -1;
    videoState->audio_stream_idx = -1;

    if (_open_smacker_video(videoState) < 0)
    {
        ERRORLOG("Failed opening video: %s.", videoState->filename);
        result = -1;
        goto ERROR;
    }

    // main decode loop
    while (!videoState->quit)
    {
        // seek stuff goes here
        if (videoState->audio_queue.size > MAX_AUDIOQ_SIZE ||
            videoState->video_queue.size > MAX_VIDEOQ_SIZE)
        {
            SDL_Delay(10);
            continue;
        }
        if (av_read_frame(videoState->p_format_ctx, pPacket) < 0)
        {
            if (videoState->p_format_ctx->pb->error == 0)
            {
                /* no error;*/
                result = 0;
                goto ERROR;
            }
            else
            {
                result = -1;
                goto ERROR;
            }
        }

        // videoState this a packet from the video stream?
        if (pPacket->stream_index == videoState->video_stream_idx)
        {
            _packet_queue_put(&videoState->video_queue, pPacket);
        }
        else if ((pPacket->stream_index == videoState->audio_stream_idx) &&
            !global_video_state->mute &&
            lbAppActive) // mute when not focused.
        {
            _packet_queue_put(&videoState->audio_queue, pPacket);
        }
        else
        {
            av_free_packet(pPacket);
        }
    }

ERROR:
    videoState->no_more_packet = 1;

    /* all done - wait for video thread to quit first */
    while (videoState->video_thread_id)
    {
        SDL_Delay(10);
    }

    av_free_packet(pPacket);

    // Close the codecs
    avcodec_close(videoState->video_ctx);
    videoState->video_ctx = NULL;
    videoState->video_stream = NULL;
    avcodec_close(videoState->audio_ctx);
    videoState->audio_ctx = NULL;
    videoState->audio_stream = NULL;

    sws_freeContext(videoState->sws_ctx);

    // Close the video file
    avformat_close_input(&(videoState->p_format_ctx));

    _packet_queue_destroy(&videoState->video_queue);
    _packet_queue_destroy(&videoState->audio_queue);

    int i = 0;
    for (i = 0; i < VIDEO_PICTURE_QUEUE_SIZE; i++)
    {
        SDL_DestroyTexture(videoState->pict_queue[i].texture);
    }

    videoState->decode_thread_id = NULL;

    ERRORLOG("quit decode thread");
    return result;
}

// This thread reads in packets from the video queue, decodes the video into frames,
// and then calls a queue_picture function to put the processed frame onto a picture queue.
int _video_thread(void *arg)
{
    VideoState *videoState = (VideoState *)arg;
    AVPacket packet, *pPacket = &packet;
    int frameFinished;
    AVFrame *pFrame;
    double pts;

    pFrame = av_frame_alloc();
    while (!videoState->quit &&
        (_packet_queue_get(&videoState->video_queue, pPacket, 1) >= 0))
    {
        pts = 0;

        // Decode video frame
        avcodec_decode_video2(videoState->video_ctx, pFrame, &frameFinished, pPacket);
        
        if (pPacket->dts != AV_NOPTS_VALUE)
        {
            pts = pFrame->pts;
        }
        else
        {
            pts = 0;
        }
        pts *= av_q2d(videoState->video_stream->time_base);

        // Did we get a video frame?
        if (frameFinished)
        {
            pts = _synchronize_video(videoState, pFrame, pts);
            if (_queue_picture(videoState, pFrame, pts) < 0)
            {
                goto ERROR;
            }
        }
        av_free_packet(pPacket);
    }

ERROR:
    av_free_packet(pPacket);
    av_frame_free(&pFrame);

    videoState->quit = 1;

    videoState->video_thread_id = NULL;
    ERRORLOG("quit video thread");
    return 0;
}

#pragma endregion

#pragma region picture
void _alloc_picture(void *userdata)
{
    VideoState *videoState = (VideoState *)userdata;
    VideoPicture *videoPicture;

    videoPicture = &(videoState->pict_queue[videoState->pict_queue_write_idx]);
    if (videoPicture->texture)
    {
        // we already have one, make one with different size.
        SDL_DestroyTexture(videoPicture->texture);
    }
    // Allocate a place to put our YUV image on that screen
    SDL_LockMutex(screen_mutex);

    videoPicture->texture = SDL_CreateTexture(lbGameRenderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
        lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenHeight);

    videoPicture->width = lbDisplay.PhysicalScreenWidth;
    videoPicture->height = lbDisplay.PhysicalScreenHeight;
    videoPicture->allocated = 1;

    SDL_UnlockMutex(screen_mutex);
}

// Allocate adequate buffer for frame containers.
int _prepare_scaled_frame(AVFrame** ppScaledFrame, uint8_t** pBuffer)
{
    // Allocate an AVFrame structure
    *ppScaledFrame = av_frame_alloc();

    if (*ppScaledFrame == NULL)
    {
        return -1;
    }

    // Determine required buffer size and allocate buffer
    int numBytes = avpicture_get_size(
        PIX_FMT_RGB24,
        lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenHeight);
    *pBuffer = (uint8_t *)av_malloc(numBytes*sizeof(uint8_t));

    // Assign appropriate parts of buffer to image planes in pScaledFrame
    // Note that pScaledFrame videoState an AVFrame, but AVFrame videoState a superset
    // of AVPicture
    avpicture_fill((AVPicture *)(*ppScaledFrame),
        *pBuffer,
        PIX_FMT_RGB24,
        lbDisplay.PhysicalScreenWidth,
        lbDisplay.PhysicalScreenHeight
        );

    return 0;
}

// Add decoded picture to picture queue.
int _queue_picture(VideoState *videoState, AVFrame *pFrame, double pts)
{
    int result = 0;
    VideoPicture *videoPicture;
    AVFrame *pScaledFrame = NULL;
    uint8_t *buffer = NULL;

    /* wait until we have space for a new pic */
    SDL_LockMutex(videoState->pict_queue_mutex);

    while (videoState->pict_queue_size >= VIDEO_PICTURE_QUEUE_SIZE &&
        !videoState->quit)
    {
        SDL_CondWait(videoState->pict_queue_cond, videoState->pict_queue_mutex);
    }
    SDL_UnlockMutex(videoState->pict_queue_mutex);

    if (videoState->quit)
    {
        result = -1;
        goto ERROR;
    }

    // write index videoState set to 0 initially
    videoPicture = &videoState->pict_queue[videoState->pict_queue_write_idx];

    /* allocate or resize the buffer! */
    if (!videoPicture->texture ||
        (videoPicture->width != lbDisplay.PhysicalScreenWidth) ||
        (videoPicture->height != lbDisplay.PhysicalScreenHeight))
    {
        videoPicture->allocated = 0;
        _alloc_picture(videoState);
        if (videoState->quit)
        {
            goto ERROR;
        }
    }

    /* We have a place to put our picture on the queue */
    if (videoPicture->texture)
    {
        //videoPicture->pts = pts;
        if (_prepare_scaled_frame(&pScaledFrame, &buffer) < 0)
        {
            ERRORLOG("Failed creating frames.");
            result = -1;
            goto ERROR;
        }

        sws_scale(videoState->sws_ctx, (uint8_t const * const *)pFrame->data, pFrame->linesize, 0, videoState->video_ctx->height/*srcSliceH*/, pScaledFrame->data, pScaledFrame->linesize);

        uint8_t * pixels = NULL;
        int pitch = 0;
        SDL_LockTexture(videoPicture->texture, NULL, (void **)(&pixels), &pitch);
        memcpy(pixels, pScaledFrame->data[0], lbDisplay.PhysicalScreenWidth*lbDisplay.PhysicalScreenHeight * 3);
        SDL_UnlockTexture(videoPicture->texture);

        /* now we inform our display thread that we have a pic ready */
        if (++videoState->pict_queue_write_idx == VIDEO_PICTURE_QUEUE_SIZE)
        {
            videoState->pict_queue_write_idx = 0;
        }
        SDL_LockMutex(videoState->pict_queue_mutex);
        videoState->pict_queue_size++;
        SDL_UnlockMutex(videoState->pict_queue_mutex);
    }
    
ERROR:
    // Free the RGB image
    if (buffer != NULL)
    {
        av_free(buffer);
    }
    if (pScaledFrame != NULL)
    {
        av_free(pScaledFrame);
    }

    return result;
}
#pragma endregion

#pragma endregion
/**
 * Plays Smacker file.
 * @return Returns 0 on error, 1 if file was played, 2 if the play was interrupted.
 */
short play_smk_direct(char *fname, int smkflags, int plyflags)
{
    SYNCDBG(7, "Starting");
    int result = 1;

   // lbDisplay.LeftButton = 0;

    global_video_state = av_mallocz(sizeof(VideoState));

    // Mute when set parameter or cannot get driver.
    global_video_state->mute = (plyflags & SMK_NoSound) || !GetSoundDriver();

    av_strlcpy(global_video_state->filename, fname, sizeof(global_video_state->filename));

    global_video_state->pict_queue_mutex = SDL_CreateMutex();
    global_video_state->pict_queue_cond = SDL_CreateCond();

    _schedule_refresh(global_video_state, 40);

    global_video_state->decode_thread_id = SDL_CreateThread(_decode_thread, NULL, global_video_state);
    if (!global_video_state->decode_thread_id)
    {
        ERRORLOG("Cannot create decode thread.");
        result = -1;
        goto ERROR;
    }

    while (!global_video_state->quit)
    {
        // Exit playing at user input.
        if (!LbWindowsControl())
        {
            global_video_state->quit = 1;
            goto ERROR;
        }
        if ((lbKeyOn[KC_ESCAPE] || lbKeyOn[KC_RETURN] || lbKeyOn[KC_SPACE] || lbDisplay.LeftButton))
        {
            global_video_state->quit = 1;
            goto ERROR;
        }
    }

ERROR:
    SDL_CloseAudio();

    // Wait for working thread to quit first.
    while (global_video_state->decode_thread_id || global_video_state->video_thread_id)
    {
        SDL_Delay(10);
    }

    SDL_DestroyMutex(global_video_state->pict_queue_mutex);
    SDL_DestroyCond(global_video_state->pict_queue_cond);

    av_free(global_video_state);
    global_video_state = NULL;
    ERRORLOG("exit smk play");
    return result;
}

/**
 * Writes the data into FLI animation.
 * @return Returns false on error, true on success.
 */
short anim_write_data(void *buf, long size)
{
    if (LbFileWrite(animation.outfhndl,buf,size) == size)
    {
      return true;
    }
    return false;
}

/**
 * Stores data into FLI buffer.
 * @return Returns false on error, true on success.
 */
short anim_store_data(void *buf, long size)
{
    memcpy(animation.field_C, buf, size);
    animation.field_C += size;
    return true;
}

/**
 * Reads the data from FLI animation.
 * @return Returns false on error, true on success.
 */
short anim_read_data(void *buf, long size)
{
    if (buf == NULL)
    {
        LbFileSeek(animation.inpfhndl,size,Lb_FILE_SEEK_CURRENT);
        return true;
    } else
    if (LbFileRead(animation.inpfhndl,buf,size) == size)
    {
        return true;
    }
    return false;
}

long anim_make_FLI_COPY(unsigned char *screenbuf)
{
    int scrpoints = animation.header.height * animation.header.width;
    memcpy(animation.field_C, screenbuf, scrpoints);
    animation.field_C += scrpoints;
    return scrpoints;
}

long anim_make_FLI_BLACK(unsigned char *screenbuf)
{
    return 0;
}

long anim_make_FLI_COLOUR256(unsigned char *palette)
{
    if (LbMemoryCompare(animation.palette, palette, 768) == 0) {
        return 0;
    }
    unsigned short *change_count;
    unsigned char *kept_count;
    short colridx;
    short change_chunk_len, kept_chunk_len;
    change_chunk_len = 0;
    kept_chunk_len = 0;
    change_count = (unsigned short *)animation.field_C;
    kept_count = NULL;
    animation.field_C += 2;
    for (colridx = 0; colridx < 256; colridx++)
    {
        unsigned char *anipal;
        unsigned char *srcpal;
        anipal = &animation.palette[3 * colridx];
        srcpal = &palette[3 * colridx];

        if (LbMemoryCompare(anipal, srcpal, 3) == 0)
        {
            change_chunk_len = 0;
            kept_chunk_len++;
        } else
        {
            if (!change_chunk_len)
            {
                *animation.field_C = kept_chunk_len;
                kept_chunk_len = 0;
                animation.field_C++;
                kept_count = (unsigned char *)animation.field_C;
                animation.field_C++;
            }
            ++change_chunk_len;
            *animation.field_C = 4 * srcpal[0];
            animation.field_C++;
            *animation.field_C = 4 * srcpal[1];
            animation.field_C++;
            *animation.field_C = 4 * srcpal[2];
            animation.field_C++;
            ++(*kept_count);
        }
        if (change_chunk_len == 1) {
            ++(*change_count);
        }
    }
    return 1;
}

/**
 * Compress data into FLI's BRUN block (8-bit Run-Length compression).
 * @return Returns unpacked size of the block which was compressed.
 */
long anim_make_FLI_BRUN(unsigned char *screenbuf)
{
    unsigned char *blk_begin = animation.field_C;
    short w,h,k,count;
    unsigned char *sbuf = screenbuf;
    for ( h = animation.header.height; h>0; h-- )
    {
      animation.field_C++;
      for (w=animation.header.width; w>0; )
      {
        count = 0;
        // Counting size of RLE block
        for ( k=1; w>1; k++ )
        {
          if (sbuf[k] != sbuf[0]) break;
          if (count == 127) break;
          w--;
          count++;
        }
        // If RLE block would be valid
        if ( count>0 )
        {
          if ( count < 127 )
            { count++; w--; }
          *animation.field_C = (char)count;
          animation.field_C++;
          *animation.field_C = sbuf[0];
          animation.field_C++;
          sbuf += count;
        } else
        {
          if ( w > 1 )
          {
            count=0;
            // Find the next block of at least 4 same pixels
            for ( k = 0; w>0; k++ )
            {
              if ( (sbuf[k+1]==sbuf[k]) && (sbuf[k+2]==sbuf[k]) && (sbuf[k+3]==sbuf[k]) )
                break;
              if ( count == -127 )
                break;
              count--;
              w--;
            }
          } else
          { count=-1; w--; }
          if ( count!=0 )
          {
            *animation.field_C = (char)count;
            animation.field_C++;
            memcpy(animation.field_C, sbuf, -count);
            sbuf -= count;
            animation.field_C -= count;
          }
        }
      }
    }
    // Make the block size even
    if ((int)animation.field_C & 1)
    {
      *animation.field_C='\0';
      animation.field_C++;
    }
    return (animation.field_C - blk_begin);
}

/**
 * Compress data into FLI's SS2 block.
 * @return Returns unpacked size of the block which was compressed.
 */
long anim_make_FLI_SS2(unsigned char *curdat, unsigned char *prvdat)
{
    unsigned char *blk_begin;
    blk_begin=animation.field_C;
    unsigned char *cbuf;
    unsigned char *pbuf;
    unsigned char *cbf;
    unsigned char *pbf;
    short h,w;
    short k,nsame,ndiff;
    short wend;
    short wendt;
    cbuf = curdat;
    pbuf = prvdat;
    unsigned short *lines_count;
    unsigned short *pckt_count;
    lines_count = (unsigned short *)animation.field_C;
    animation.field_C += 2;
    pckt_count = (unsigned short *)animation.field_C;

    wend = 0;
    for (h=animation.header.height; h>0; h--)
    {
      cbf = cbuf;
      pbf = pbuf;
      if (wend == 0)
      {
          pckt_count = (unsigned short *)animation.field_C;
          animation.field_C += 2;
          (*lines_count)++;
      }
      for (w=animation.header.width;w>0;)
      {
        for ( k=0; w>0; k++)
        {
          if ( *(unsigned short *)(pbf+2*(long)k) != *(unsigned short *)(cbf+2*(long)k) )
            break;
          w -= 2;
        }
        if (2*(long)k == animation.header.width)
        {
          wend--;
          cbf += LbGraphicsScreenWidth();
          pbf += LbGraphicsScreenWidth();
          continue;
        }
        if ( w > 0 )
        {
          if (wend != 0)
          {
            (*pckt_count) = wend;
            pckt_count = (unsigned short *)animation.field_C;
            animation.field_C += 2;
          }
          wendt = 2*k;
          wend = wendt;
          while (wend > 255)
          {
            *(unsigned char *)animation.field_C = 255;
            animation.field_C++;
            *(unsigned char *)animation.field_C = 0;
            animation.field_C++;
            wend -= 255;
            (*pckt_count)++;
          }
          cbf += wendt;
          pbf += wendt;

          for (nsame=0; nsame<127; nsame++)
          {
            if (w <= 2) break;
            if ((*(unsigned short *)(pbf+2*nsame+0) == *(unsigned short *)(cbf+2*nsame+0)) &&
                (*(unsigned short *)(pbf+2*nsame+2) == *(unsigned short *)(cbf+2*nsame+2)))
                break;
            if ( *(unsigned short *)(cbf+2*nsame+2) != *(unsigned short *)(cbf) )
              break;
            w -= 2;
          }
          if (nsame > 0)
          {
            if (nsame < 127)
            {
              nsame++;
              w -= 2;
            }
            *(unsigned char *)animation.field_C = wend;
            animation.field_C++;
            *(unsigned char *)animation.field_C = -nsame;
            animation.field_C++;
            *(unsigned short *)animation.field_C = *(unsigned short *)cbf;
            animation.field_C+=2;
            pbf += 2*nsame;
            cbf += 2*nsame;
            wend = 0;
            (*pckt_count)++;
          } else
          {
            if (w == 2)
            {
              ndiff = 1;
              w -= 2;
            } else
            {
              for (ndiff=0; ndiff<127; ndiff++)
              {
                if (w <= 0) break;
                if ( *(unsigned short *)(pbf+2*ndiff) == *(unsigned short *)(cbf+2*ndiff) )
                  break;
                if ((*(unsigned short *)(cbf+2*(ndiff+1)) == *(unsigned short *)(cbf+2*ndiff)) &&
                   (*(unsigned short *)(cbf+2*(ndiff+2)) == *(unsigned short *)(cbf+2*ndiff)) )
                  break;
                w -= 2;
              }
            }
            if (ndiff>0)
            {
              *(unsigned char *)animation.field_C = wend;
              animation.field_C++;
              *(unsigned char *)animation.field_C = ndiff;
              animation.field_C++;
              memcpy(animation.field_C, cbf, 2*(long)ndiff);
              animation.field_C += 2*(long)ndiff;
              pbf += 2*(long)ndiff;
              cbf += 2*(long)ndiff;
              wend = 0;
              (*pckt_count)++;
            }
          }
        }
      }
        cbuf += LbGraphicsScreenWidth();
        pbuf += LbGraphicsScreenWidth();
    }

    if (animation.header.height+wend == 0)
    {
      (*lines_count) = 1;
      (*pckt_count) = 1;
      *(unsigned char *)animation.field_C = 0;
      animation.field_C++;
      *(unsigned char *)animation.field_C = 0;
      animation.field_C++;
    } else
    if (wend != 0)
    {
        animation.field_C -= 2;
        (*lines_count)--;
    }
    // Make the data size even
    animation.field_C = (unsigned char *)(((unsigned int)animation.field_C + 1) & 0xFFFFFFFE);
    return animation.field_C - blk_begin;
}

/**
 * Compress data into FLI's LC block.
 * @return Returns unpacked size of the block which was compressed.
 */
long anim_make_FLI_LC(unsigned char *curdat, unsigned char *prvdat)
{
    unsigned char *blk_begin;
    blk_begin=animation.field_C;
    unsigned char *cbuf;
    unsigned char *pbuf;
    unsigned char *cbf;
    unsigned char *pbf;
    unsigned char *outptr;
    short h,w;
    short hend,wend;
    short hdim,wendt;
    short k,nsame,ndiff;
    int blksize;

    cbuf = curdat;
    pbuf = prvdat;
    for (hend = animation.header.height; hend>0;  hend--)
    {
      wend = 0;
      for (w = animation.header.width; w>0; w--)
      {
        if (cbuf[wend] != pbuf[wend]) break;
        ++wend;
      }
      if ( wend != animation.header.width )
        break;
      cbuf += LbGraphicsScreenWidth();
      pbuf += LbGraphicsScreenWidth();
    }

    if (hend != 0)
    {
      hend = animation.header.height - hend;
      blksize = animation.header.width * (long)(animation.header.height-1);
      cbuf = curdat+blksize;
      pbuf = prvdat+blksize;
      for (h=animation.header.height; h>0; h--)
      {
        wend = 0;
        for (w=animation.header.width; w>0; w--)
        {
          if (cbuf[wend] != pbuf[wend]) break;
          wend++;
        }
        if ( wend != animation.header.width )
          break;
        cbuf -= LbGraphicsScreenWidth();
        pbuf -= LbGraphicsScreenWidth();
      }
      hdim = h - hend;
      blksize = animation.header.width * (long)hend;
      cbuf = curdat+blksize;
      pbuf = prvdat+blksize;
      *(unsigned short *)animation.field_C = hend;
      animation.field_C += 2;
      *(unsigned short *)animation.field_C = hdim;
      animation.field_C += 2;

      for (h = hdim; h>0; h--)
      {
          cbf = cbuf;
          pbf = pbuf;
          outptr = animation.field_C++;
          for (w=animation.header.width; w>0; )
          {
            for ( wend=0; w>0; wend++)
            {
              if ( cbf[wend] != pbf[wend]) break;
              w--;
            }
            wendt = wend;
            if (animation.header.width == wend) continue;
            if ( w <= 0 ) break;
            while ( wend > 255 )
            {
              *(unsigned char *)animation.field_C = 255;
              animation.field_C++;
              *(unsigned char *)animation.field_C = 0;
              animation.field_C++;
              wend -= 255;
              (*(unsigned char *)outptr)++;
            }
            cbf += wendt;
            pbf += wendt;
            k = 0;
            nsame = 0;
            while ( w > 1 )
            {
              if ( nsame == -127 ) break;
              if ((cbf[k+0] == pbf[k+0]) && (cbf[k+1] == pbf[k+1]) && (cbf[k+2] == pbf[k+2]))
                  break;
              if (cbf[k+1] != cbf[0]) break;
              w--;
              k++;
              nsame--;
            }
            if ( nsame )
            {
              if ( nsame != -127 )
              { nsame--; w--; }
              *(unsigned char *)animation.field_C = wend;
              animation.field_C++;
              *(unsigned char *)animation.field_C = nsame;
              animation.field_C++;
              *(unsigned char *)animation.field_C = cbf[0];
              cbf -= nsame;
              pbf -= nsame;
              animation.field_C++;
              (*(unsigned char *)outptr)++;
            } else
            {
              if ( w == 1 )
              {
                ndiff = nsame + 1;
                w--;
              } else
              {
                k = 0;
                ndiff = 0;
                while (w != 0)
                {
                  if ( ndiff == 127 ) break;
                  if ((cbf[k+0] == pbf[k+0]) && (cbf[k+1] == pbf[k+1]) && (cbf[k+2] == pbf[k+2]))
                      break;
                  if ((cbf[k+1] == cbf[k+0]) && (cbf[k+2] == cbf[k+0]) && (cbf[k+3] == cbf[k+0]))
                      break;
                  w--;
                  k++;
                  ndiff++;
                }
              }
              if (ndiff != 0)
              {
                *(unsigned char *)animation.field_C = wend;
                animation.field_C++;
                *(unsigned char *)animation.field_C = ndiff;
                animation.field_C++;
                memcpy(animation.field_C, cbf, ndiff);
                animation.field_C += ndiff;
                cbf += ndiff;
                pbf += ndiff;
                (*(unsigned char *)outptr)++;
              }
            }
          }
          cbuf += LbGraphicsScreenWidth();
          pbuf += LbGraphicsScreenWidth();
      }
    } else
    {
      *(short *)animation.field_C = 0;
      animation.field_C += 2;
      *(short *)animation.field_C = 1;
      animation.field_C += 2;
      *(char *)animation.field_C = 0;
      animation.field_C++;
    }
    // Make the data size even
    animation.field_C = (unsigned char *)(((unsigned int)animation.field_C + 1) & 0xFFFFFFFE);
    return animation.field_C - blk_begin;
}

/*
 * Returns size of the FLI movie frame buffer, for given width
 * and height of animation. The buffer of returned size videoState big enough
 * to store one frame of any kind (any compression).
 */
long anim_buffer_size(int width,int height,int bpp)
{
    int n;
    n=(bpp>>3);
    if (bpp%8) n++;
    return abs(width)*abs(height)*n + 32767;
}

/*
 * Returns size of the FLI movie frame buffer, for given width
 * and height of animation. The buffer of returned size videoState big enough
 * to store one frame of any kind (any compression).
 */
short anim_format_matches(int width,int height,int bpp)
{
    if (width != animation.header.width)
      return false;
    if (height != animation.header.height)
      return false;
    if (bpp != animation.header.depth)
      return false;
    return true;
}

short anim_stop(void)
{
    SYNCLOG("Finishing movie recording.");
    if ( ((animation.field_0 & 0x01)==0) || (animation.outfhndl==0))
    {
      ERRORLOG("Can't stop recording movie");
      return false;
    }
    LbFileSeek(animation.outfhndl, 0, Lb_FILE_SEEK_BEGINNING);
    animation.header.frames--;
    LbFileWrite(animation.outfhndl, &animation.header, sizeof(struct AnimFLIHeader));
    if ( LbFileClose(animation.outfhndl) == -1 )
    {
        ERRORLOG("Can't close movie file");
        return false;
    }
    animation.outfhndl = 0;
    LbMemoryFree(animation.chunkdata);
    animation.chunkdata=NULL;
    animation.field_0 = 0;
    return true;
}

short anim_open(char *fname, int arg1, short arg2, int width, int height, int bpp, unsigned int flags)
{
  if ( flags & animation.field_0 )
  {
      ERRORLOG("Cannot record movie");
    return false;
  }
  if (flags & 0x01)
  {
      SYNCLOG("Starting to record new movie, \"%s\".",fname);
      LbMemorySet(&animation, 0, sizeof(struct Animation));
      animation.field_0 |= flags;
      animation.videobuf = LbMemoryAlloc(2 * height*width);
      if (animation.videobuf==NULL)
      {
          ERRORLOG("Cannot allocate video buffer.");
        return false;
      }
      long max_chunk_size = anim_buffer_size(width,height,bpp);
      animation.chunkdata = LbMemoryAlloc(max_chunk_size);
      if (animation.chunkdata==NULL)
      {
          ERRORLOG("Cannot allocate chunk buffer.");
        return false;
      }
      animation.outfhndl = LbFileOpen(fname, Lb_FILE_MODE_NEW);
      if (animation.outfhndl == -1)
      {
          ERRORLOG("Can't open movie file.");
        return false;
      }
      animation.header.dsize = 128;
      animation.header.magic = 0xAF12;
      animation.header.depth = bpp;
      animation.header.flags = 3;
      animation.header.speed = 57;
      animation.header.created = 0;
      animation.header.frames = 0;
      animation.header.width = width;
      animation.header.updated = 0;
      animation.header.aspectx = 6;
      animation.header.height = height;
      animation.header.reserved2 = 0;
      animation.header.creator = 0x464C4942;//'BILF'
      animation.header.aspecty = 5;
      animation.header.updater = 0x464C4942;
      LbMemorySet(animation.header.reserved3, 0, sizeof(animation.header.reserved3));
      animation.header.oframe1 = 0;
      animation.header.oframe2 = 0;
      LbMemorySet(animation.header.reserved4, 0, sizeof(animation.header.reserved4));
      animation.field_18 = arg2;
      if ( !anim_write_data(&animation.header, sizeof(struct AnimFLIHeader)) )
      {
          ERRORLOG("Movie write error.");
        LbFileClose(animation.outfhndl);
        return false;
      }
      animation.field_31C = 0;
      animation.field_320 = height*width + 1024;
      LbMemorySet(animation.palette, -1, sizeof(animation.palette));
  }
  if (flags & 0x02)
  {
      SYNCLOG("Resuming movie recording, \"%s\".",fname);
      animation.field_0 |= flags;
      animation.inpfhndl = LbFileOpen(fname, 2);
      if ( animation.inpfhndl == -1 )
        return false;
      // Reading header
      if (!anim_read_data(&animation.header, sizeof(struct AnimFLIHeader)))
      {
          ERRORLOG("Movie header read error.");
        LbFileClose(animation.inpfhndl);
        return false;
      }
      // Now we can allocate chunk buffer
      long max_chunk_size = anim_buffer_size(animation.header.width,animation.header.height,animation.header.depth);
      animation.chunkdata = LbMemoryAlloc(max_chunk_size);
      if (animation.chunkdata==NULL)
        return false;
      if (!anim_read_data(&animation.chunk, sizeof(struct AnimFLIChunk)))
      {
          ERRORLOG("Movie chunk read error.");
        LbFileClose(animation.inpfhndl);
        return false;
      }
      if (animation.chunk.ctype == FLI_PREFIX)
      {
        if (!anim_read_data(animation.chunkdata, animation.chunk.csize-sizeof(struct AnimFLIChunk)))
        {
            ERRORLOG("Movie data read error.");
            LbFileClose(animation.inpfhndl);
            return false;
        }
      } else
      {
        LbFileSeek(animation.inpfhndl, -sizeof(struct AnimFLIChunk), Lb_FILE_SEEK_CURRENT);
      }
      animation.field_31C = 0;
  }
  return true;
}

TbBool anim_make_next_frame(unsigned char *screenbuf, unsigned char *palette)
{
    SYNCDBG(7,"Starting");
    unsigned long max_chunk_size;
    unsigned char *dataptr;
    long brun_size,lc_size,ss2_size;
    int width = animation.header.width;
    int height = animation.header.height;
    animation.field_C = animation.chunkdata;
    max_chunk_size = anim_buffer_size(width,height,animation.header.depth);
    LbMemorySet(animation.chunkdata, 0, max_chunk_size);
    animation.prefix.ctype = 0xF1FAu;
    animation.prefix.nchunks = 0;
    animation.prefix.csize = 0;
    LbMemorySet(animation.prefix.reserved, 0, sizeof(animation.prefix.reserved));
    struct AnimFLIPrefix *prefx = (struct AnimFLIPrefix *)animation.field_C;
    anim_store_data(&animation.prefix, sizeof(struct AnimFLIPrefix));
    animation.subchunk.ctype = 0;
    animation.subchunk.csize = 0;
    struct AnimFLIChunk *subchnk = (struct AnimFLIChunk *)animation.field_C;
    anim_store_data(&animation.subchunk, sizeof(struct AnimFLIChunk));
    if ( animation.field_31C == 0 )
    {
        animation.header.oframe1 = animation.header.dsize;
    } else
    if ( animation.field_31C == 1 )
    {
        animation.header.oframe2 = animation.header.dsize;
    }
    if ( anim_make_FLI_COLOUR256(palette) )
    {
        prefx->nchunks++;
        subchnk->ctype = 4;
        subchnk->csize = animation.field_C-(unsigned char *)subchnk;
        animation.subchunk.ctype = 0;
        animation.subchunk.csize = 0;
        subchnk = (struct AnimFLIChunk *)animation.field_C;
        anim_store_data(&animation.subchunk, sizeof(struct AnimFLIChunk));
    }
    int scrpoints = animation.header.height * (long)animation.header.width;
    if (animation.field_31C == 0)
    {
        if ( anim_make_FLI_BRUN(screenbuf) )
        {
          prefx->nchunks++;
          subchnk->ctype = FLI_BRUN;
        } else
        {
          anim_make_FLI_COPY(screenbuf);
          prefx->nchunks++;
          subchnk->ctype = FLI_COPY;
        }
    } else
    {
      // Determining the best compression method
      dataptr = animation.field_C;
      brun_size = anim_make_FLI_BRUN(screenbuf);
      LbMemorySet(dataptr, 0, brun_size);
      animation.field_C = dataptr;
      ss2_size = anim_make_FLI_SS2(screenbuf, animation.videobuf);
      LbMemorySet(dataptr, 0, ss2_size);
      animation.field_C = dataptr;
      lc_size = anim_make_FLI_LC(screenbuf, animation.videobuf);
      if ((lc_size < ss2_size) && (lc_size < brun_size))
      {
          // Store the LC compressed data
          prefx->nchunks++;
          subchnk->ctype = FLI_LC;
      } else
      if (ss2_size < brun_size)
      {
          // Clear the LC compressed data
          LbMemorySet(dataptr, 0, lc_size);
          animation.field_C = dataptr;
          // Compress with SS2 method
          anim_make_FLI_SS2(screenbuf, animation.videobuf);
          prefx->nchunks++;
          subchnk->ctype = FLI_SS2;
      } else
      if ( brun_size < scrpoints+16 )
      {
          // Clear the LC compressed data
            LbMemorySet(dataptr, 0, lc_size);
          animation.field_C = dataptr;
          // Compress with BRUN method
          anim_make_FLI_BRUN(screenbuf);
          prefx->nchunks++;
          subchnk->ctype = FLI_BRUN;
      } else
      {
          // Clear the LC compressed data
            LbMemorySet(dataptr, 0, lc_size);
          animation.field_C = dataptr;
          // Store uncompressed frame data
          anim_make_FLI_COPY(screenbuf);
          prefx->nchunks++;
          subchnk->ctype = FLI_COPY;
      }
    }
    subchnk->csize = animation.field_C-(unsigned char *)subchnk;
    prefx->csize = animation.field_C - animation.chunkdata;
    if ( !anim_write_data(animation.chunkdata, animation.field_C-animation.chunkdata) )
    {
    //LbSyncLog("Finished frame w/error.\n");
      return false;
    }
    memcpy(animation.videobuf, screenbuf, height*width);
    memcpy(animation.palette, palette, sizeof(animation.palette));
    animation.header.frames++;
    animation.field_31C++;
    animation.header.dsize += animation.field_C-animation.chunkdata;
    //LbSyncLog("Finished frame ok.\n");
    return true;
}

TbBool anim_record_frame(unsigned char *screenbuf, unsigned char *palette)
{
    if ((animation.field_0 & 0x01)==0)
      return false;
    if (!anim_format_matches(MyScreenWidth/pixel_size,MyScreenHeight/pixel_size,LbGraphicsScreenBPP()))
      return false;
    return anim_make_next_frame(screenbuf, palette);
}

short anim_record(void)
{
    SYNCDBG(7,"Starting");
    static char finalname[255];
    if (LbGraphicsScreenBPP() != 8)
    {
      ERRORLOG("Cannot record movie in non-8bit screen mode");
      return 0;
    }
    int idx;
    for (idx=0; idx < 10000; idx++)
    {
        sprintf(finalname, "%s/game%04d.flc","scrshots",idx);
        if (LbFileExists(finalname))
          continue;
        return anim_open(finalname, 0, 0, MyScreenWidth/pixel_size,MyScreenHeight/pixel_size,8, 1);
    }
    ERRORLOG("No free file name for recorded movie");
    return 0;
}


/******************************************************************************/
#ifdef __cplusplus
}
#endif
