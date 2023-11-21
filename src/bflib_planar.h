/******************************************************************************/
// Bullfrog Engine Emulation Library - for use to remake classic games like
// Syndicate Wars, Magic Carpet or Dungeon Keeper.
/******************************************************************************/
/** @file bflib_planar.h
 *     Header file for bflib_planar.c.
 * @par Purpose:
 *     Basic planar integer geometry.
 * @par Comment:
 *     Just a header file - #defines, typedefs, function prototypes etc.
 * @author   Tomasz Lis
 * @date     24 Jan 2009 - 08 Mar 2009
 * @par  Copying and copyrights:
 *     This program is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 2 of the License, or
 *     (at your option) any later version.
 */
/******************************************************************************/
#ifndef BFLIB_PLANAR_H
#define BFLIB_PLANAR_H

#include "bflib_basics.h"

#ifdef __cplusplus
extern "C" {
#endif
/******************************************************************************/
struct TbRect {
  long left;
  long top;
  long right;
  long bottom;
};

struct TbPoint {
  long x;
  long y;
};
/******************************************************************************/

/**
 * Calculate the Chebyshev distance^ between two 2-dimensional points (x1,y1) and (x2,y2).
 * 
 * ^"Chebyshev distance is a metric defined on a vector space where the distance between two vectors is the greatest of their differences along any coordinate dimension." - https://en.wikipedia.org/wiki/Chebyshev_distance
 * 
 * @param x1 2-dimensional point #1; x coordinate
 * @param y1 2-dimensional point #1; y coordinate
 * @param x2 2-dimensional point #2; x coordinate
 * @param y2 2-dimensional point #2; y coordinate
 * @return the Chebyshev distance between the 2 given 2d points.
 */
#define chebyshev_distance(x1,y1,x2,y2) (max(abs(x1 - x2), abs(y1 - y2)))

/**
 * Calculate the manhattan distance* between two 2-dimensional points (x1,y1) and (x2,y2).
 * 
 * "This distance can be imagined as the length needed to move between two points in a grid where you can only move up, down, left or right." - https://simple.wikipedia.org/wiki/Manhattan_distance
 * 
 * @param x1 2-dimensional point #1; x coordinate
 * @param y1 2-dimensional point #1; y coordinate
 * @param x2 2-dimensional point #2; x coordinate
 * @param y2 2-dimensional point #2; y coordinate
 * @return the manhattan distance between the 2 given 2d points.
 */
#define manhattan_distance(x1,y1,x2,y2) (abs(x1 - x2) + abs(y1 - y2))
/******************************************************************************/
void LbSetRect(struct TbRect *rect, long xLeft, long yTop, long xRight, long yBottom);

long get_angle_symmetric_difference(long angle_a, long angle_b);
long get_angle_difference(long angle_a, long angle_b);
long get_angle_sign(long angle_a, long angle_b);

long distance_with_angle_to_coord_x(long distance, long angle);
long distance_with_angle_to_coord_y(long distance, long angle);

long get_distance_xy(long x1, long x2, long y1, long y2);

long distance3d_with_angles_to_coord_x(long distance, long angle_a, long angle_b);
long distance3d_with_angles_to_coord_y(long distance, long angle_a, long angle_b);
#define distance_with_angle_to_coord_z(distance, angle) distance_with_angle_to_coord_x(distance, angle)
long move_coord_with_angle_x(long pos_x, long distance, long angle);
long move_coord_with_angle_y(long pos_y, long distance, long angle);
/******************************************************************************/
#ifdef __cplusplus
}
#endif
#endif
