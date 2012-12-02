/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file map_blocks.h
 *     Header file for map_blocks.c.
 * @par Purpose:
 *     Map blocks support functions.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     11 Mar 2010 - 12 May 2010
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef DK_MAP_BLOCKS_H
#define DK_MAP_BLOCKS_H

#include "bflib_basics.h"
#include "globals.h"

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************************/
#pragma pack(1)

struct Thing;
struct Map;

#pragma pack()
/******************************************************************************/
TbBool block_has_diggable_side(long plyr_idx, long slb_x, long slb_y);

void place_slab_type_on_map(SlabKind nslab, MapSubtlCoord stl_x, MapSubtlCoord stl_y, PlayerNumber owner, unsigned char a5);
void mine_out_block(MapSubtlCoord stl_x, MapSubtlCoord stl_y, PlayerNumber plyr_idx);
unsigned char dig_has_revealed_area(long a1, long a2, unsigned char a3);
void dig_out_block(MapSubtlCoord stl_x, MapSubtlCoord stl_y, PlayerNumber plyr_idx);
void neutralise_enemy_block(MapSubtlCoord stl_x, MapSubtlCoord stl_y, PlayerNumber domn_plyr_idx);
void check_map_explored(struct Thing *thing, long a2, long a3);
long ceiling_partially_recompute_heights(long sx, long sy, long ex, long ey);
TbBool set_slab_explored(PlayerNumber plyr_idx, MapSlabCoord slb_x, MapSlabCoord slb_y);
void update_floor_and_ceiling_heights_at(MapSubtlCoord stl_x, MapSubtlCoord stl_y,
    MapSubtlCoord *floor_height, MapSubtlCoord *ceiling_height);
TbBool point_in_map_is_solid(const struct Coord3d *pos);
unsigned char alter_rock_style(unsigned char a1, signed char a2, signed char a3, unsigned char a4);

long element_top_face_texture(struct Map *map);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
