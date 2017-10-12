/*
 *  Geometry.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 20/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <dom/domGeometry.h>

#include "Polygons.h"

class Geometry
{
public:
	std::vector<Polygons> objects;
	
	Geometry(){};
	Geometry(domGeometry &);

};

#endif