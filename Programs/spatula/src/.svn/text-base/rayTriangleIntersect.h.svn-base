/*
 *  rayTriangleIntersect.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 21/06/08.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

extern "C" {

/* ray triangle intersection code from :
 * Fast, Minimum Storage Ray-Triangle Intersection
 * Tomas Möller and Ben Trumbore
 * Journal of Graphics Tools
 * 1997
 */

#ifndef RAYTRIANGLEINTERSECT_H
#define RAYTRIANGLEINTERSECT_H

#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 


int intersectsRayTriangle(double orig[3], double dir[3],
                   double vert0[3], double vert1[3], double vert2[3],
                   double *t, double *u, double *v);
                
                
int intersectsRayTriangleTestCull(double orig[3], double dir[3],
                   double vert0[3], double vert1[3], double vert2[3],
                   double *t, double *u, double *v);



}

#endif

