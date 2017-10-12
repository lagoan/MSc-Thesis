/*
 *  Geometry.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 26/09/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <dae.h>
#include <dom/domMesh.h>
#include <dom/domTriangles.h>
#include <dom/domPolygons.h>
#include <dom/domPolylist.h>
#include <integration/integrationHelper.h>
#include "Geometry.h"


Geometry::Geometry(domGeometry & geometryElement_)
{
	*this = Geometry();

	domMesh* _meshElement = geometryElement_.getMesh();
	
	// polygons
	for (ovUInt i=0; i < _meshElement->getPolygons_array().getCount(); i++) {
		domPolygons* polygons = daeSafeCast<domPolygons>( _meshElement->getPolygons_array()[i]);
		objects.push_back(lookup<Polygons, domPolygons>(*polygons));
	}
	
	// polylyst
	for (ovUInt i=0; i < _meshElement->getPolylist_array().getCount(); i++) {
		domPolylist* polylist = daeSafeCast<domPolylist>( _meshElement->getPolylist_array()[i]);
		objects.push_back(lookup<Polygons, domPolylist>(*polylist));
	}
	
	// triangles
	for (ovUInt i=0; i < _meshElement->getTriangles_array().getCount(); i++) {
		domTriangles* triangles = daeSafeCast<domTriangles>( _meshElement->getTriangles_array()[i]);
		objects.push_back(lookup<Polygons, domTriangles>(*triangles));
	}

}