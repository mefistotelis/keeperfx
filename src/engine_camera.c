/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file engine_camera.c
 *     Camera move, maintain and support functions.
 * @par Purpose:
 *     Defines and maintains cameras.
 * @par Comment:
 *     None.
 * @author   Tomasz Lis
 * @date     20 Mar 2009 - 30 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#include "engine_camera.h"

#include "globals.h"
#include "bflib_basics.h"
#include "bflib_math.h"
#include "bflib_memory.h"
#include "bflib_video.h"
#include "bflib_sprite.h"
#include "bflib_vidraw.h"
#include "bflib_planar.h"

#include "engine_lenses.h"
#include "engine_render.h"
#include "vidmode.h"
#include "map_blocks.h"
#include "dungeon_data.h"
#include "config_settings.h"
#include "player_instances.h"


#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
/******************************************************************************/
long camera_zoom;
float zoomed_range;
/******************************************************************************/
#ifdef __cplusplus
}
#endif
/******************************************************************************/

void calculate_zoomed_range(struct Camera *cam) {
    // zoomed_range is the current camera zoom converted to a percentage that ranges between base level zoom and fully zoomed out.

    float range_input = cam->zoom;
    float range_min;
    float range_max;
    switch (cam->view_mode) {
        case PVM_IsometricView:
            range_min = CAMERA_ZOOM_MIN; // Fully zoomed out
            range_max = 8192; // Base zoom level
            break;
        case PVM_FrontView:
            range_min = FRONTVIEW_CAMERA_ZOOM_MIN; // Fully zoomed out
            range_max = 45000; // Base zoom level
            break;
        default:
            zoomed_range = 0;
            return;
    }
    if (range_input < range_min) {
        range_input = range_min;
    } else if (range_input > range_max) {
        range_input = range_max;
    }
    zoomed_range = ((range_input - range_min)) / (range_max - range_min);
}

MapCoordDelta get_3d_box_distance(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    long dist_y = abs(pos2->y.val - (long)pos1->y.val);
    long dist_x = abs(pos2->x.val - (long)pos1->x.val);
    if (dist_y <= dist_x)
        dist_y = dist_x;
    long dist_z = abs(pos2->z.val - (long)pos1->z.val);
    if (dist_y <= dist_z)
        dist_y = dist_z;
    return dist_y;
}

MapCoordDelta get_2d_box_distance(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    long dist_y = abs((long)pos1->y.val - (long)pos2->y.val);
    long dist_x = abs((long)pos1->x.val - (long)pos2->x.val);
    if (dist_y <= dist_x)
        return dist_x;
    return dist_y;
}

MapCoordDelta get_2d_box_distance_xy(long pos1_x, long pos1_y, long pos2_x, long pos2_y)
{
    long dist_x = abs((long)pos1_x - (long)pos2_x);
    long dist_y = abs((long)pos1_y - (long)pos2_y);
    if (dist_y <= dist_x)
      return dist_x;
    return dist_y;
}

void angles_to_vector(short angle_xy, short angle_yz, long dist, struct ComponentVector *cvect)
{
    long long cos_yz = LbCosL(angle_yz) >> 2;
    long long sin_yz = LbSinL(angle_yz) >> 2;
    long long cos_xy = LbCosL(angle_xy) >> 2;
    long long sin_xy = LbSinL(angle_xy) >> 2;
    long long lldist = dist;
    long long mag = (lldist << 14) - cos_yz;
    long long factor = sin_xy * mag;
    cvect->x = (factor >> 14) >> 14;
    factor = cos_xy * mag;
    cvect->y = -(factor >> 14) >> 14;
    factor = lldist * sin_yz;
    cvect->z = (factor >> 14);
}

long get_angle_xy_to_vec(const struct CoordDelta3d *vec)
{
    return LbArcTanAngle(vec->x.val, vec->y.val) & LbFPMath_AngleMask;
}

long get_angle_yz_to_vec(const struct CoordDelta3d *vec)
{
    long dist = LbDiagonalLength(abs(vec->x.val), abs(vec->y.val));
    return LbArcTanAngle(vec->z.val, dist) & LbFPMath_AngleMask;
}

long get_angle_xy_to(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    return LbArcTanAngle((long)pos2->x.val - (long)pos1->x.val, (long)pos2->y.val - (long)pos1->y.val) & LbFPMath_AngleMask;
}

long get_angle_yz_to(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    long dist = get_2d_distance(pos1, pos2);
    return LbArcTanAngle(pos2->z.val - pos1->z.val, dist) & LbFPMath_AngleMask;
}

// TODO these are actually Coord2d and Coord3d just inherits from it
MapCoordDelta get_2d_distance(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    long dist_x = (long)pos1->x.val - (long)pos2->x.val;
    long dist_y = (long)pos1->y.val - (long)pos2->y.val;
    return LbDiagonalLength(abs(dist_x), abs(dist_y));
}

MapCoordDelta get_2d_distance_squared(const struct Coord3d *pos1, const struct Coord3d *pos2)
{
    long dist_x = (long)pos1->x.val - (long)pos2->x.val;
    long dist_y = (long)pos1->y.val - (long)pos2->y.val;
    return dist_x * dist_x + dist_y * dist_y;
}

void project_point_to_wall_on_angle(const struct Coord3d *pos1, struct Coord3d *pos2, long angle_xy, long angle_z, long distance, long num_steps)
{
    long dx = distance_with_angle_to_coord_x(distance, angle_xy);
    long dy = distance_with_angle_to_coord_y(distance, angle_xy);
    long dz = distance_with_angle_to_coord_z(distance, angle_z);
    struct Coord3d pos;
    pos.x.val = pos1->x.val;
    pos.y.val = pos1->y.val;
    pos.z.val = pos1->z.val;
    // Do num_steps until a solid wall is reached
    for (long n = num_steps; n > 0; n--)
    {
        if (point_in_map_is_solid(&pos))
            break;
        pos.x.val += dx;
        pos.y.val += dy;
        pos.z.val += dz;
    }
    pos2->x.val = pos.x.val;
    pos2->y.val = pos.y.val;
    pos2->z.val = pos.z.val;
}

void view_zoom_camera_in(struct Camera *cam, long limit_max, long limit_min)
{
    long new_zoom;
    long old_zoom = get_camera_zoom(cam);
    switch (cam->view_mode)
    {
    case PVM_IsometricView:
        new_zoom = (100 * old_zoom) / 85;
        if (new_zoom == old_zoom)
            new_zoom++;
        if (new_zoom < limit_min) {
            new_zoom = limit_min;
        } else
        if (new_zoom > limit_max) {
            new_zoom = limit_max;
        }
        break;
    case PVM_ParchmentView:
        new_zoom = (5 * old_zoom) / 4;
        if (new_zoom == old_zoom)
            new_zoom++;
        if (new_zoom < 16) {
            new_zoom = 16;
        } else
        if (new_zoom > 1024) {
            new_zoom = 1024;
        }
        break;
    case PVM_FrontView:
        new_zoom = (100 * old_zoom) / 85;
        if (new_zoom == old_zoom)
            new_zoom++;
        if (new_zoom < FRONTVIEW_CAMERA_ZOOM_MIN) { //Originally 16384, adjusted for view distance
            new_zoom = FRONTVIEW_CAMERA_ZOOM_MIN;
        } else
        if (new_zoom > FRONTVIEW_CAMERA_ZOOM_MAX) {
            new_zoom = FRONTVIEW_CAMERA_ZOOM_MAX;
        }
        break;
    default:
        new_zoom = old_zoom;
    }
    set_camera_zoom(cam, new_zoom);
}

void set_camera_zoom(struct Camera *cam, long new_zoom)
{
    if (cam == NULL)
      return;
    switch (cam->view_mode)
    {
    case PVM_IsometricView:
    case PVM_FrontView:
        cam->zoom = new_zoom;
        break;
    case PVM_ParchmentView:
        cam->mappos.z.val = new_zoom;
        break;
    }
}

void view_zoom_camera_out(struct Camera *cam, long limit_max, long limit_min)
{
    long new_zoom;
    long old_zoom = get_camera_zoom(cam);
    switch (cam->view_mode)
    {
    case PVM_IsometricView:
        new_zoom = (85 * old_zoom) / 100;
        if (new_zoom == old_zoom)
            new_zoom--;
        if (new_zoom < limit_min) {
            new_zoom = limit_min;
        } else
        if (new_zoom > limit_max) {
            new_zoom = limit_max;
        }
        break;
    case PVM_ParchmentView:
        new_zoom = (4 * old_zoom) / 5;
        if (new_zoom == old_zoom)
            new_zoom--;
        if (new_zoom < 16) {
            new_zoom = 16;
        } else
        if (new_zoom > 1024) {
            new_zoom = 1024;
        }
        break;
    case PVM_FrontView:
        new_zoom = (85 * old_zoom) / 100;
        if (new_zoom == old_zoom)
            new_zoom--;
        if (new_zoom < FRONTVIEW_CAMERA_ZOOM_MIN) {
            new_zoom = FRONTVIEW_CAMERA_ZOOM_MIN;
        } else
        if (new_zoom > FRONTVIEW_CAMERA_ZOOM_MAX) {
            new_zoom = FRONTVIEW_CAMERA_ZOOM_MAX;
        }
        break;
    default:
        new_zoom = old_zoom;
    }
    set_camera_zoom(cam, new_zoom);
}

/**
 * Conducts clipping to zoom level of given camera, based on current screen mode.
 */
void update_camera_zoom_bounds(struct Camera *cam,unsigned long zoom_max,unsigned long zoom_min)
{
    SYNCDBG(7,"Starting");
    long zoom_val = get_camera_zoom(cam);
    if (zoom_val < zoom_min)
    {
      zoom_val = zoom_min;
    } else
    if (zoom_val > zoom_max)
    {
      zoom_val = zoom_max;
    }
    set_camera_zoom(cam, zoom_val);
}

long get_camera_zoom(struct Camera *cam)
{
    if (cam == NULL)
      return 0;
    switch (cam->view_mode)
    {
    case PVM_IsometricView:
    case PVM_FrontView:
        return cam->zoom;
    case PVM_ParchmentView:
        return cam->mappos.z.val;
    default:
        return 0;
    }
}

/** When the menu is hidden in Isometric view, show less of the map (at max zoom out)
    because the increased view exceeds the render array, and we want to hide the graphical glitches it causes)
    otherwise this function just sets zoom_min = CAMERA_ZOOM_MIN
 *
 * @param cam The current player's camera.\
 * @param showgui Whether the side-menu is visible or not (you should pass "game.operation_flags & GOF_ShowGui".\
 */
unsigned long adjust_min_camera_zoom(struct Camera *cam, int showgui)
{
  unsigned long zoom_min = CAMERA_ZOOM_MIN;
  if (showgui == 0 && cam->view_mode == PVM_IsometricView)
    zoom_min += 300; // a higher value is a nearer zoom
  return zoom_min;
}

/** Scales camera zoom for current screen resolution.
 *
 * @param zoom_lvl Unscaled zoom level.
 * @return Zoom level scaled with use of current units_per_pixel value.
 */
unsigned long scale_camera_zoom_to_screen(unsigned long zoom_lvl)
{
    unsigned long size_narr = ((pixel_size * units_per_pixel_min) << 7) / 10;
    unsigned long size_wide = (pixel_size * units_per_pixel) << 3;
    return  ((zoom_lvl*size_wide) >> 8) + ((zoom_lvl*size_narr) >> 8);
}

void view_set_camera_y_inertia(struct Camera *cam, long delta, long ilimit)
{
    long abslimit = abs(ilimit);
    cam->inertia_y += delta;
    if (cam->inertia_y < -abslimit) {
        cam->inertia_y = -abslimit;
    } else
    if (cam->inertia_y > abslimit) {
        cam->inertia_y = abslimit;
    }
    cam->in_active_movement_y = true;
}

void view_set_camera_x_inertia(struct Camera *cam, long delta, long ilimit)
{
    long abslimit = abs(ilimit);
    cam->inertia_x += delta;
    if (cam->inertia_x < -abslimit) {
        cam->inertia_x = -abslimit;
    } else
    if (cam->inertia_x > abslimit) {
        cam->inertia_x = abslimit;
    }
    cam->in_active_movement_x = true;
}

void view_set_camera_rotation_inertia(struct Camera *cam, long delta, long ilimit)
{
    int limit_val = abs(ilimit);
    int new_val = delta + cam->inertia_rotation;
    cam->inertia_rotation = new_val;
    if (new_val < -limit_val)
    {
        cam->inertia_rotation = -limit_val;
        cam->in_active_movement_rotation = true;
    } else
    if (new_val > limit_val)
    {
        cam->inertia_rotation = limit_val;
        cam->in_active_movement_rotation = true;
    } else
    {
        cam->in_active_movement_rotation = true;
    }
}

void init_player_cameras(struct PlayerInfo *player)
{

    struct Thing* heartng = get_player_soul_container(player->id_number);
    struct Camera* cam = &player->cameras[CamIV_FirstPerson];
    cam->mappos.x.val = 0;
    cam->mappos.y.val = 0;
    cam->mappos.z.val = 256;
    cam->orient_b = 0;
    cam->orient_c = 0;
    cam->field_13 = 188;
    cam->orient_a = LbFPMath_PI/2;
    cam->view_mode = PVM_CreatureView;

    cam = &player->cameras[CamIV_Isometric];
    cam->mappos.x.val = heartng->mappos.x.val;
    cam->mappos.y.val = heartng->mappos.y.val;
    cam->mappos.z.val = 0;
    cam->orient_c = 0;
    cam->field_13 = 188;
    cam->orient_b = -266;
    cam->orient_a = LbFPMath_PI/4;
    cam->view_mode = PVM_IsometricView;
    cam->zoom = settings.isometric_view_zoom_level;

    cam = &player->cameras[CamIV_Parchment];
    cam->mappos.x.val = 0;
    cam->mappos.y.val = 0;
    cam->mappos.z.val = 32;
    cam->field_13 = 188;
    cam->view_mode = PVM_ParchmentView;

    cam = &player->cameras[CamIV_FrontView];
    cam->mappos.x.val = heartng->mappos.x.val;
    cam->mappos.y.val = heartng->mappos.y.val;
    cam->mappos.z.val = 32;
    cam->field_13 = 188;
    cam->view_mode = PVM_FrontView;
    cam->zoom = settings.frontview_zoom_level;
}

static int get_walking_bob_direction(struct Thing *thing)
{
  int anim_time;
  int result;
  TbBool biggernThen1408;

  anim_time = thing->anim_time;
  if ( anim_time >= 0 && anim_time >= 256 )
  {
    if ( anim_time < 640 )
    {
      result = 1;
      goto LABEL_10;
    }
    if ( anim_time >= 1024 )
    {
      biggernThen1408 = anim_time < 1408;
      result = 1;
      if ( biggernThen1408 )
        goto LABEL_10;
    }
  }
  result = -1;
LABEL_10:
  if ( thing->anim_speed < 0 )
    return -result;
  return result;
  
}

void update_player_camera_fp(struct Camera *cam, struct Thing *thing)
{

    int pos_x;
    int pos_y;
    short camHeight;
    short v13;
    short v15;
    int avg_filled_stl;
    unsigned short move_angle_z;
    int byte16;
    short cam_z_val;
    int thing_z_val;
    short v27;
    short v28;
    short v29;
    short v30;
    int thing_height;

    struct CreatureStats* crstat = creature_stats_get_from_thing(thing);
    struct CreatureControl *cctrl = creature_control_get_from_thing(thing);
    static const int chicken_height = 100;
    int eye_height;
    TbBool chicken = (creature_affected_by_spell(thing, SplK_Chicken));
    if (!chicken)
    {
        eye_height = crstat->eye_height + (crstat->eye_height * gameadd.crtr_conf.exp.size_increase_on_exp * cctrl->explevel) / 100;
    }
    else
    {
        eye_height = chicken_height + (chicken_height * gameadd.crtr_conf.exp.size_increase_on_exp * cctrl->explevel) / 100;
    }

    char direction = get_walking_bob_direction(thing);
    if ( thing_is_creature(thing) )
    {
        if ( cctrl->move_speed && thing->floor_height >= thing->mappos.z.val )
            cctrl->bob_direction =  16 * direction;
        else
            cctrl->bob_direction = 0;

        pos_x = move_coord_with_angle_x(thing->mappos.x.val,-90,thing->move_angle_xy);
        pos_y = move_coord_with_angle_y(thing->mappos.y.val,-90,thing->move_angle_xy);

        if ( pos_x >= 0 )
        {
            if ( pos_x > 0xFFFF )
                pos_x = -1 * abs(pos_x);
        }
        else
        {
            pos_x = 0;
        }
        if ( pos_y >= 0 )
        {
            if ( pos_y > 0xFFFF )
                pos_y = -1 * abs(pos_y);
        }
        else
        {
            pos_y = 0;
        }
        
        cam->mappos.x.val = pos_x;
        cam->mappos.y.val = pos_y;

        
        if ( (thing->movement_flags & TMvF_Flying) != 0 )
        {
            cam->mappos.z.val = thing->mappos.z.val + eye_height;
            cam->orient_a = thing->move_angle_xy;
            cam->orient_b = thing->move_angle_z;
            cam->orient_c = cctrl->field_CC;
        }
        else
        {
            cam->orient_a = thing->move_angle_xy;
            camHeight = cam->mappos.z.val;
            cam->orient_b = thing->move_angle_z;
            cam->orient_c = 0;
            thing_height = thing->mappos.z.val;
            if ( eye_height + thing_height <= camHeight )
            {
                v15 = camHeight + (thing_height + cctrl->bob_direction - camHeight + eye_height) / 2;
                cam->mappos.z.val = v15;
                if ( eye_height + thing->mappos.z.val + cctrl->bob_direction > v15 )
                    cam->mappos.z.val = eye_height + thing->mappos.z.val + cctrl->bob_direction;
            }
            else
            {
                v13 = camHeight + (thing_height + cctrl->bob_direction - camHeight + eye_height) / 2;
                cam->mappos.z.val = v13;
                if ( eye_height + thing->mappos.z.val + cctrl->bob_direction < v13 )
                    cam->mappos.z.val = eye_height + thing->mappos.z.val + cctrl->bob_direction;
            }
        }
/*
        SubtlCodedCoords v18 = thing->mappos.x.stl.num + (thing->mappos.y.stl.num << 8);
        v19 = (int)(((*(int *)((char *)dword_6CC96D           + 5 * v18) & 0xF000000u) >> 24 << 8)
                  + ((*(int *)((char *)&dword_6CC968          + 5 * v18) & 0xF000000u) >> 24 << 8)
                  + ((*(int *)((char *)&dword_6CC46D          + 5 * v18) & 0xF000000u) >> 24 << 8)
                  + ((*(int *)((char *)get_mapwho_thing_index + 5 * v18) & 0xF000000u) >> 24 << 8))
            / 4;
        */
    
   
        struct Map* mapblk1 = get_map_block_at(thing->mappos.x.stl.num,     thing->mappos.y.stl.num);
        struct Map* mapblk2 = get_map_block_at(thing->mappos.x.stl.num + 1, thing->mappos.y.stl.num);
        struct Map* mapblk3 = get_map_block_at(thing->mappos.x.stl.num,     thing->mappos.y.stl.num + 1);
        struct Map* mapblk4 = get_map_block_at(thing->mappos.x.stl.num + 1, thing->mappos.y.stl.num + 1);


        avg_filled_stl = ((get_mapblk_filled_subtiles(mapblk1) * COORD_PER_STL) +
                          (get_mapblk_filled_subtiles(mapblk2) * COORD_PER_STL) +
                          (get_mapblk_filled_subtiles(mapblk3) * COORD_PER_STL) +
                          (get_mapblk_filled_subtiles(mapblk4) * COORD_PER_STL) )/4;
 
        if ( cam->mappos.z.val > avg_filled_stl - 64 )
            cam->mappos.z.val = avg_filled_stl - 64;

    }
    else
    {
        cam->mappos.x.val = thing->mappos.x.val;
        cam->mappos.y.val = thing->mappos.y.val;
        if ( thing_is_mature_food(thing) )
        {
            cam->mappos.z.val = thing->mappos.z.val + 240;
            cam->orient_a = thing->move_angle_xy;
            move_angle_z = thing->move_angle_z;
            cam->orient_c = 0;
            cam->orient_b = move_angle_z;
            byte16 = thing->food.byte_16;
            thing->move_angle_z = 0;
            if ( byte16 )
            {
                if ( byte16 <= 3 )
                    thing->move_angle_z = -116 * byte16 + 2048;
                else
                    thing->move_angle_z = 116 * byte16 + 1352;
            }
        }
        else
        {
            cam_z_val = cam->mappos.z.val;
            cam->orient_a = thing->move_angle_xy;
            cam->orient_b = thing->move_angle_z;
            cam->orient_c = 0;
            thing_z_val = thing->mappos.z.val;
            if ( thing_z_val + 32 <= cam_z_val )
            {
                v29 = cam_z_val + (thing_z_val - cam_z_val + 64) / 2;
                cam->mappos.z.val = v29;
                v30 = thing->mappos.z.val;
                if ( v30 + 64 > v29 )
                    cam->mappos.z.val = v30 + 64;
            }
            else
            {
                v27 = cam_z_val + (thing_z_val - cam_z_val + 64) / 2;
                cam->mappos.z.val = v27;
                v28 = thing->mappos.z.val;
                if ( v28 + 64 < v27 )
                    cam->mappos.z.val = v28 + 64;
            }
        }
    }
  
}

void view_move_camera_left(struct Camera *cam, long distance)
{
  
    int pos_x;
    int pos_y;
    int parchment_pos_x;
  
    if ( cam->view_mode == PVM_IsometricView || cam->view_mode == PVM_FrontView)
    {

        pos_x = cam->mappos.x.val - FIXED_POLAR_TO_X(cam->orient_a + 512,distance);
        pos_y = cam->mappos.y.val + FIXED_POLAR_TO_Y(cam->orient_a + 512,distance);

        if ( pos_x < 0 )
            pos_x = 0;
        if ( pos_x > 0xFFFF )
            pos_x = -1;

        if ( pos_y < 0 )
            pos_y = 0;
        if ( pos_y > 0xFFFF )
            pos_y = -1;

        cam->mappos.x.val = pos_x;
        cam->mappos.y.val = pos_y;
        return;
    }
  
    else if ( cam->view_mode == PVM_ParchmentView )
    {
        parchment_pos_x = cam->mappos.x.val - distance;

        if ( parchment_pos_x < 0 )
            parchment_pos_x = 0;
        if ( parchment_pos_x > 0xFFFF )
            parchment_pos_x = -1;

        cam->mappos.x.stl.pos = parchment_pos_x;

    }
    
}

void view_move_camera_right(struct Camera *cam, long distance)
{
    int pos_x;
    int pos_y;
    int parchment_pos_x;
  
    if ( cam->view_mode == PVM_IsometricView || cam->view_mode == PVM_FrontView)
    {

        pos_x = cam->mappos.x.val + FIXED_POLAR_TO_X(cam->orient_a + 512,distance);
        pos_y = cam->mappos.y.val - FIXED_POLAR_TO_Y(cam->orient_a + 512,distance);

        if ( pos_x < 0 )
            pos_x = 0;
        if ( pos_x > 0xFFFF )
            pos_x = -1;

        if ( pos_y < 0 )
            pos_y = 0;
        if ( pos_y > 0xFFFF )
            pos_y = -1;

        cam->mappos.x.val = pos_x;
        cam->mappos.y.val = pos_y;
        return;
    }
  
    else if ( cam->view_mode == PVM_ParchmentView )
    {
        parchment_pos_x = cam->mappos.x.val + distance;

        if ( parchment_pos_x < 0 )
            parchment_pos_x = 0;
        if ( parchment_pos_x > 0xFFFF )
            parchment_pos_x = -1;

        cam->mappos.x.stl.pos = parchment_pos_x;

    }
    
}

void view_move_camera_up(struct Camera *cam, long distance)
{
    int pos_x;
    int pos_y;
    int parchment_pos_y;
  
    if ( cam->view_mode == PVM_IsometricView || cam->view_mode == PVM_FrontView)
    {

        pos_x = cam->mappos.x.val + FIXED_POLAR_TO_X(cam->orient_a,distance);
        pos_y = cam->mappos.y.val - FIXED_POLAR_TO_Y(cam->orient_a,distance);

        if ( pos_x < 0 )
            pos_x = 0;
        if ( pos_x > 0xFFFF )
            pos_x = -1;

        if ( pos_y < 0 )
            pos_y = 0;
        if ( pos_y > 0xFFFF )
            pos_y = -1;

        cam->mappos.x.val = pos_x;
        cam->mappos.y.val = pos_y;
        return;
    }
    else if ( cam->view_mode == PVM_ParchmentView )
    {
        parchment_pos_y = cam->mappos.y.val - distance;

        if ( parchment_pos_y < 0 )
            parchment_pos_y = 0;
        if ( parchment_pos_y > 0xFFFF )
            parchment_pos_y = -1;

        cam->mappos.y.stl.pos = parchment_pos_y;

    }
}

void view_move_camera_down(struct Camera *cam, long distance)
{
    int pos_x;
    int pos_y;
    int parchment_pos_y;
  
    if ( cam->view_mode == PVM_IsometricView || cam->view_mode == PVM_FrontView)
    {

        pos_x = cam->mappos.x.val - FIXED_POLAR_TO_X(cam->orient_a,distance);
        pos_y = cam->mappos.y.val + FIXED_POLAR_TO_Y(cam->orient_a,distance);

        if ( pos_x < 0 )
            pos_x = 0;
        if ( pos_x > 0xFFFF )
            pos_x = -1;

        if ( pos_y < 0 )
            pos_y = 0;
        if ( pos_y > 0xFFFF )
            pos_y = -1;

        cam->mappos.x.val = pos_x;
        cam->mappos.y.val = pos_y;
        return;
    }
  
    else if ( cam->view_mode == PVM_ParchmentView )
    {
        parchment_pos_y = cam->mappos.y.val - distance;

        if ( parchment_pos_y < 0 )
            parchment_pos_y = 0;
        if ( parchment_pos_y > 0xFFFF )
            parchment_pos_y = -1;

        cam->mappos.y.stl.pos = parchment_pos_y;

    }
    
}

void view_process_camera_inertia(struct Camera *cam)
{
    int i;
    i = cam->inertia_x;
    if (i > 0) {
        view_move_camera_right(cam, abs(i));
    } else
    if (i < 0) {
        view_move_camera_left(cam, abs(i));
    }
    if ( cam->in_active_movement_x ) {
        cam->in_active_movement_x = false;
    } else {
        cam->inertia_x /= 2;
    }
    i = cam->inertia_y;
    if (i > 0) {
        view_move_camera_down(cam, abs(i));
    } else
    if (i < 0) {
        view_move_camera_up(cam, abs(i));
    }
    if (cam->in_active_movement_y) {
        cam->in_active_movement_y = false;
    } else {
        cam->inertia_y /= 2;
    }
    if (cam->inertia_rotation) {
        cam->orient_a = (cam->inertia_rotation + cam->orient_a) & LbFPMath_AngleMask;
    }
    if (cam->in_active_movement_rotation) {
        cam->in_active_movement_rotation = false;
    } else {
        cam->inertia_rotation /= 2;
    }
}

void update_player_camera(struct PlayerInfo *player)
{
    struct Dungeon *dungeon;
    dungeon = get_players_dungeon(player);
    struct Camera *cam;
    cam = player->acamera;
    view_process_camera_inertia(cam);
    switch (cam->view_mode)
    {
    case PVM_CreatureView:
        if (player->controlled_thing_idx > 0) {
            struct Thing *ctrltng;
            ctrltng = thing_get(player->controlled_thing_idx);
            update_player_camera_fp(cam, ctrltng);
        } else
        if (player->instance_num != PI_HeartZoom) {
            ERRORLOG("Cannot go first person without controlling creature");
        }
        break;
    case PVM_IsometricView:
        player->cameras[CamIV_FrontView].mappos.x.val = cam->mappos.x.val;
        player->cameras[CamIV_FrontView].mappos.y.val = cam->mappos.y.val;
        break;
    case PVM_FrontView:
        player->cameras[CamIV_Isometric].mappos.x.val = cam->mappos.x.val;
        player->cameras[CamIV_Isometric].mappos.y.val = cam->mappos.y.val;
        break;
    }
    if (dungeon->camera_deviate_quake) {
        dungeon->camera_deviate_quake--;
    }
    if (dungeon->camera_deviate_jump > 0) {
        dungeon->camera_deviate_jump -= 32;
    }
}

void update_all_players_cameras(void)
{
  int i;
  struct PlayerInfo *player;
  SYNCDBG(6,"Starting");
  for (i=0; i<PLAYERS_COUNT; i++)
  {
    player = get_player(i);
    if (player_exists(player) && ((player->allocflags & PlaF_CompCtrl) == 0))
    {
          update_player_camera(player);
    }
  }
}
/******************************************************************************/
