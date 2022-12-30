/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_sndlib.c
 *     Low-level sound and music related routines.
 * @par Purpose:
 *     Hardware wrapper to play music and sound in games.
 * @par Comment:
 *     Windows version os Bullfrog Engine uses Miles Sound System, wrapped
 *     with WSND7R.DLL. This library contains definitions of the exported functions.
 * @author   KeeperFX Team
 * @date     16 Nov 2008 - 30 Dec 2008
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#ifdef _WIN32

#include "pre_inc.h"
#include "bflib_sndlib.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef _WIN32
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include "bflib_basics.h"
#include "post_inc.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
// Global variables

typedef int (WINAPI *FARPROCI)(int);
typedef int (WINAPI *FARPROCII)(int,int);
typedef int (WINAPI *FARPROCIII)(int,int,int);
typedef int (WINAPI *FARPROCS)(const char *);
typedef int (WINAPI *FARPROCP)(const void *);
typedef int (WINAPI *FARPROCSIII)(const char *,int,int,int);
typedef int (WINAPI *FARPROCIIII)(int,int,int,int);
typedef struct SampleInfo * (WINAPI *FARPROC_PLAY1)(int,int,int,int,int,unsigned char,unsigned char, void *, int);

/******************************************************************************/
// Functions

// GetModuleHandleExA(0,"WSND7R",&hModule); seems not to work

int FreeAudio(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_FreeAudio@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of FreeAudio function; skipped."); return 0; }
    return proc();
}

int SetRedbookVolume(int volume)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetRedbookVolume@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetRedbookVolume function; skipped."); return 0; }
    return ((FARPROCI)proc)(volume);
}

int SetSoundMasterVolume(int volume)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetSoundMasterVolume@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetSoundMasterVolume function; skipped."); return 0; }
    return ((FARPROCI)proc)(volume);
}

int SetMusicMasterVolume(int volume)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetMusicMasterVolume@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetMusicMasterVolume function; skipped."); return 0; }
    return ((FARPROCI)proc)(volume);
}

int GetSoundInstalled(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_GetSoundInstalled@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of GetSoundInstalled function; skipped."); return 0; }
    return proc();
}

int PlayRedbookTrack(int track)
{
    SYNCDBG(18,"Starting");
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_PlayRedbookTrack@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of PlayRedbookTrack function; skipped."); return 0; }
    return ((FARPROCI)proc)(track);
}

int PauseRedbookTrack(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_PauseRedbookTrack@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of PauseRedbookTrack function; skipped."); return 0; }
    return proc();
}

int ResumeRedbookTrack(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_ResumeRedbookTrack@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of ResumeRedbookTrack function; skipped."); return 0; }
    return proc();
}

int MonitorStreamedSoundTrack(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_MonitorStreamedSoundTrack@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of MonitorStreamedSoundTrack function; skipped."); return 0; }
    return proc();
}

int StopRedbookTrack(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_StopRedbookTrack@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of StopRedbookTrack function; skipped."); return 0; }
    return proc();
}

void * GetSoundDriver(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_GetSoundDriver@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of GetSoundDriver function; skipped."); return 0; }
    return (void *)proc();
}

int StopAllSamples(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_StopAllSamples@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of StopAllSamples function; skipped."); return 0; }
    return proc();
}

struct SampleInfo * GetFirstSampleInfoStructure(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_GetFirstSampleInfoStructure@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of GetFirstSampleInfoStructure function; skipped."); return 0; }
    return (struct SampleInfo *)proc();
}

int InitAudio(void *i)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_InitAudio@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of InitAudio function; skipped."); return 0; }
    return ((FARPROCP)proc)(i);
}

int SetupAudioOptionDefaults(void *i)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetupAudioOptionDefaults@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetupAudioOptionDefaults function; skipped."); return 0; }
    return ((FARPROCP)proc)(i);
}

int PlayStreamedSample(char *fname, int a2, int a3, int a4)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_PlayStreamedSample@16");
    if (proc==NULL)
    { ERRORLOG("Can't get address of PlayStreamedSample function; skipped."); return 0; }
    return ((FARPROCSIII)proc)(fname, a2, a3, a4);
}

int IsSamplePlaying(int a1, int a2, int a3)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_IsSamplePlaying@12");
    if (proc==NULL)
    { ERRORLOG("Can't get address of IsSamplePlaying function; skipped."); return 0; }
    return (unsigned char)((FARPROCIII)proc)(a1, a2, a3);
}

int StopStreamedSample(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_StopStreamedSample@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of StopStreamedSample function; skipped."); return 0; }
    return proc();
}

int StreamedSampleFinished(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_StreamedSampleFinished@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of StreamedSampleFinished function; skipped."); return 0; }
    return proc();
}

int SetStreamedSampleVolume(int volume)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetStreamedSampleVolume@4");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetStreamedSampleVolume function; skipped."); return 0; }
    return ((FARPROCI)proc)(volume);
}

struct SampleInfo * GetLastSampleInfoStructure(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_GetLastSampleInfoStructure@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of GetLastSampleInfoStructure function; skipped."); return 0; }
    return (struct SampleInfo *)proc();
}

int GetCurrentSoundMasterVolume(void)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_GetCurrentSoundMasterVolume@0");
    if (proc==NULL)
    { ERRORLOG("Can't get address of GetCurrentSoundMasterVolume function; skipped."); return 0; }
    return proc();
}

int StopSample(int a,int b)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_StopSample@8");
    if (proc==NULL)
    { ERRORLOG("Can't get address of StopSample function; skipped."); return 0; }
    return ((FARPROCII)proc)(a,b);
}

int SetSampleVolume(int a,int b,int c,int d)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetSampleVolume@16");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetSampleVolume function; skipped."); return 0; }
    return ((FARPROCIIII)proc)(a,b,c,d);
}

int SetSamplePan(int a,int b,int c,int d)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetSamplePan@16");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetSamplePan function; skipped."); return 0; }
    return ((FARPROCIIII)proc)(a,b,c,d);
}

int SetSamplePitch(int a,int b,int c,int d)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_SetSamplePitch@16");
    if (proc==NULL)
    { ERRORLOG("Can't get address of SetSamplePitch function; skipped."); return 0; }
    return ((FARPROCIIII)proc)(a,b,c,d);
}

struct SampleInfo * PlaySampleFromAddress(int a1, int smpl_idx, int a3, int a4, int a5, unsigned char a6, unsigned char a7, void * buf, int sfxid)
{
    HMODULE hModule=LoadLibrary("WSND7R");
    FARPROC proc = GetProcAddress(hModule, "_PlaySampleFromAddress@36");
    if (proc==NULL)
    { ERRORLOG("Can't get address of PlaySampleFromAddress function; skipped."); return 0; }
    return ((FARPROC_PLAY1)(void *)proc)(a1, smpl_idx, a3, a4, a5, a6, a7, buf, sfxid);
}

/******************************************************************************/
#ifdef __cplusplus
}
#endif

#else

int FreeAudio() { return 0; }
int SetRedbookVolume(int volume) { return 0; }
int SetSoundMasterVolume(int volume) { return 0; }
int SetMusicMasterVolume(int volume) { return 0; }
int GetSoundInstalled() { return 0; }
int PlayRedbookTrack(int v) { return 0; }
int PauseRedbookTrack() { return 0; }
int ResumeRedbookTrack() { return 0; }
int MonitorStreamedSoundTrack() { return 0; }
int StopRedbookTrack() { return 0; }
void * GetSoundDriver() { return 0; }
int StopAllSamples() { return 0; }
struct SampleInfo * GetFirstSampleInfoStructure() { return 0; }
int InitAudio(void *p) { return 0; }
int SetupAudioOptionDefaults(void *p) { return 0; }
int PlayStreamedSample(char *fname, int a2, int a3, int a4) { return 0; }
int IsSamplePlaying(int a1, int a2, int a3) { return 0; }
int StopStreamedSample() { return 0; }
int StreamedSampleFinished() { return 0; }
int SetStreamedSampleVolume(int v) { return 0; }
struct SampleInfo * GetLastSampleInfoStructure() { return 0; }
int GetCurrentSoundMasterVolume() { return 0; }
int StopSample(int a,int b) { return 0; }
int SetSampleVolume(int a,int b,int c,int d) { return 0; }
int SetSamplePan(int a,int b,int c,int d) { return 0; }
int SetSamplePitch(int a,int b,int c,int d) { return 0; }
struct SampleInfo * PlaySampleFromAddress(int a1, int smpl_idx, int a3, int a4, int a5, unsigned char a6, unsigned char a7, void * buf, int a9) { return 0; }

#endif
