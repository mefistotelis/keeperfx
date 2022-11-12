/******************************************************************************/
// Free implementation of Bullfrog's Dungeon Keeper strategy game.
/******************************************************************************/
/** @file map_ceiling.c
 *     Map ceiling support functions.
 * @par Purpose:
 *     Functions to calculate the ceiling.
 * @par Comment:
 *     None.
 * @author   keeperFx Team
 * @date     12 Nov 2022
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/

#include "pre_inc.h"
#include "globals.h"
#include "map_columns.h"
#include "front_simple.h"
#include "globals.h"
#include "game_legacy.h"
#include "post_inc.h"

#ifdef __cplusplus
extern "C" {
#endif

DLLIMPORT long _DK_ceiling_init(unsigned long a1, unsigned long a2);


signed char *ceiling_cache;

int find_column_height_including_lintels(struct Column *col)
{
  unsigned __int8 i; // dl

  if ( !col->solidmask )
    return 0;
  for ( i = 7; !col->cubes[i]; --i )
    ;
  return i + 1;
}

int __fastcall ceiling_block_is_solid_including_corners_return_height(int a1, int a2, int a3)
{
  struct Column *col; // eax
  unsigned __int8 v6; // dl
  int v8; // edx
  unsigned __int8 v10; // ch
  int v11; // edx
  unsigned __int8 v13; // cl
  int v14; // edx
  unsigned __int8 v16; // bh
  int v17; // edx
  unsigned __int8 bitfields; // bh

  if ( (game.map[a1].flags & 0x10) == 0 && (game.columns.lookup[game.map[a1].data & 0x7FF]->bitfields & 0xE) == 0 )
  {
    if ( a2 <= 0 )
    {
      if ( a3 > 0 )
      {
        v17 = a1 - 256;
        if ( (game.map[v17].flags & 0x10) != 0 || (game.columns.lookup[game.map[v17].data & 0x7FF]->bitfields & 0xE) != 0 )
        {
          col = game.columns.lookup[game.map[a1 - 256].data & 0x7FF];
          bitfields = col->bitfields;
          if ( (bitfields & 0xE) != 0 )
            return find_column_height_including_lintels(col);
          return bitfields >> 4;
        }
      }
    }
    else
    {
      v8 = a1 - 1;
      if ( (game.map[v8].flags & 0x10) != 0 || (game.columns.lookup[game.map[v8].data & 0x7FF]->bitfields & 0xE) != 0 )
      {
        col = game.columns.lookup[game.map[a1 - 1].data & 0x7FF];
        v10 = col->bitfields;
        if ( (v10 & 0xE) != 0 )
          return find_column_height_including_lintels(col);
        return v10 >> 4;
      }
      if ( a3 > 0 )
      {
        v11 = a1 - 256;
        if ( (game.map[v11].flags & 0x10) != 0 || (game.columns.lookup[game.map[v11].data & 0x7FF]->bitfields & 0xE) != 0 )
        {
          col = game.columns.lookup[game.map[a1 - 256].data & 0x7FF];
          v13 = col->bitfields;
          if ( (v13 & 0xE) != 0 )
            return find_column_height_including_lintels(col);
          return v13 >> 4;
        }
        v14 = a1 - 257;
        if ( (game.map[v14].flags & 0x10) != 0 || (game.columns.lookup[game.map[v14].data & 0x7FF]->bitfields & 0xE) != 0 )
        {
          col = game.columns.lookup[game.map[a1 - 257].data & 0x7FF];
          v16 = col->bitfields;
          if ( (v16 & 0xE) != 0 )
            return find_column_height_including_lintels(col);
          return v16 >> 4;
        }
      }
    }
    return -1;
  }
  col = game.columns.lookup[game.map[a1].data & 0x7FF];
  v6 = col->bitfields;
  if ( (v6 & 0xE) != 0 )
    return find_column_height_including_lintels(col);
  return v6 >> 4;
}

int ceiling_calculate_height_from_nearest_walls(int result, int a2)
{
  int v2; // edx

  v2 = game.ceiling_step * a2;
  if ( result >= game.ceiling_height_max )
  {
    if ( result > game.ceiling_height_max )
    {
      result -= v2;
      if ( result <= game.ceiling_height_min )
        return game.ceiling_height_min;
    }
  }
  else
  {
    result += v2;
    if ( result >= game.ceiling_height_max )
      return game.ceiling_height_max;
  }
  return result;
}


long ceiling_partially_recompute_heights(long sx, long sy, long ex, long ey)
{
  int v5; // edx
  int v6; // edi
  int v7; // eax
  int v8; // edx
  int v9; // edx
  int v10; // edx
  int v11; // eax
  int v12; // ebx
  int v13; // eax
  int v14; // eax
  int v15; // ebp
  int v16; // edx
  int v17; // edi
  int v18; // esi
  int i; // ecx
  signed __int8 is_solid_including_corners_return_height; // al
  unsigned long *p_data; // ebp
  int v22; // eax
  int v23; // edx
  struct MapOffset *v24; // eax
  int v25; // ecx
  int v26; // ebx
  int v27; // esi
  int v28; // ecx
  int v29; // ebx
  int v30; // eax
  int v31; // eax
  int v32; // edi
  char v33; // bl
  int v34; // ebx
  int v35; // edx
  unsigned int v37; // [esp+4h] [ebp-54h]
  int v38; // [esp+8h] [ebp-50h] BYREF
  int v39; // [esp+Ch] [ebp-4Ch]
  int v40; // [esp+10h] [ebp-48h]
  int v41; // [esp+14h] [ebp-44h]
  int v42; // [esp+18h] [ebp-40h]
  int v43; // [esp+1Ch] [ebp-3Ch]
  int v44; // [esp+20h] [ebp-38h]
  int v45; // [esp+24h] [ebp-34h]
  int v46; // [esp+28h] [ebp-30h]
  int v47; // [esp+2Ch] [ebp-2Ch]
  int *v48; // [esp+30h] [ebp-28h]
  int v49; // [esp+34h] [ebp-24h]
  unsigned int v50; // [esp+38h] [ebp-20h]
  int v51; // [esp+3Ch] [ebp-1Ch]
  int v52; // [esp+40h] [ebp-18h]
  int v53; // [esp+44h] [ebp-14h]
  int v54; // [esp+48h] [ebp-10h]

  v5 = game.ceiling_dist;
  if ( game.ceiling_dist > 4 )
    v5 = 4;
  v6 = sx - v5;
  v7 = v5;
  v8 = v6;
  if ( v6 <= 0 )
    v8 = 0;
  v45 = v8;
  v9 = sy - v7;
  if ( sy - v7 <= 0 )
    v9 = 0;
  v40 = v9;
  v10 = ex + v7;
  if ( ex + v7 >= 256 )
    v10 = 256;
  v11 = ey + v7;
  v47 = v10;
  if ( v11 >= 256 )
    v11 = 256;
  v39 = v11;
  v12 = v11;
  ceiling_cache = scratch;
  v13 = v45 - game.ceiling_dist;
  if ( v45 - game.ceiling_dist <= 0 )
    v13 = 0;
  v44 = v13;
  v14 = v40 - game.ceiling_dist;
  if ( v40 - game.ceiling_dist <= 0 )
    v14 = 0;
  v15 = game.ceiling_dist + v47;
  if ( game.ceiling_dist + v47 >= 256 )
    v15 = 256;
  v16 = v12 + game.ceiling_dist;
  if ( v12 + game.ceiling_dist >= 256 )
    v16 = 256;
  v41 = v16;
  v17 = v14;
  if ( v14 < v16 )
  {
    v43 = v14 << 8;
    do
    {
      v18 = v44;
      for ( i = v44 + v43; v18 < v15; ceiling_cache[i - 1] = is_solid_including_corners_return_height )
        is_solid_including_corners_return_height = ceiling_block_is_solid_including_corners_return_height(
                                                     i++,
                                                     v18++,
                                                     v17);
      ++v17;
      v43 += 256;
    }
    while ( v17 < v41 );
  }
  if ( v40 < v39 )
  {
    v46 = v40 << 8;
    v42 = v39 << 8;
    do
    {
      v49 = v45;
      v51 = v46 + v45;
      if ( v47 > v45 )
      {
        v50 = 5 * v51;
        do
        {
          p_data = &game.map[v50 / 5].data;
          v22 = ceiling_cache[v51];
          v38 = v22;
          if ( v22 <= -1 )
          {
            v52 = v51;
            v48 = &v38;
            v54 = v51 % 256;
            v53 = v51 / 256;
            v23 = 0;
            v24 = spiral_step;
            if ( game.ceiling_search_dist > 0 )
            {
              while ( 1 )
              {
                v25 = v54 + v24->h;
                v26 = v53 + v24->v;
                if ( v25 >= 0 && v25 < 256 && v26 >= 0 && v26 < 256 )
                {
                  v27 = ceiling_cache[v52 + (*(long *)v24 >> 16)];
                  if ( v27 > -1 )
                    break;
                }
                ++v23;
                ++v24;
                if ( v23 >= game.ceiling_search_dist )
                  goto LABEL_43;
              }
              *v48 = v27;
              v28 = v54 - v25;
              v29 = v53 - v26;
              if ( (int)abs(v28) <= (int)abs(v29) )
                v30 = v29;
              else
                v30 = v28;
              v37 = abs(v30);
              v31 = 1;
            }
            else
            {
LABEL_43:
              v31 = 0;
            }
            if ( v31 )
              v22 = ceiling_calculate_height_from_nearest_walls(v38, v37);
            else
              v22 = game.ceiling_height_max;
          }
          v32 = v47;
          v33 = *((char *)p_data + 3) & 0xF0;
          v50 += 5;
          ++v51;
          *((char *)p_data + 3) = v33;
          v35 = ((v22 & 0xF) << 24) | *p_data;
          v34 = ++v49;
          *p_data = v35;
        }
        while ( v34 < v32 );
      }
      v46 += 256;
    }
    while ( v46 < v42 );
  }
  return 1;


}

/******************************************************************************/
#ifdef __cplusplus
}
#endif
