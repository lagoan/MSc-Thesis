/*
 *  InstanceGeometry.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 20/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INSTANCEGEOMETRY_H
#define INSTANCEGEOMETRY_H

#include <vector>


#include "primitives/Geometry.h"
#include <dom/domInstance_geometry.h>

class Node;

class InstanceGeometry
{
public:
	Node* parent;	
	Geometry* geometry;
	// TODO: add materials
	
	InstanceGeometry();
	InstanceGeometry(const InstanceGeometry& instanceGeometry);
	void render();

};

#endif