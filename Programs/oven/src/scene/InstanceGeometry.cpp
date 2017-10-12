/*
 *  InstanceGeometry.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 22/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <dae.h>
#include "integration/integrationHelper.h"

#include "InstanceGeometry.h"
#include "system/types.h"
#include "system/LoggingSystem.h"


InstanceGeometry::InstanceGeometry()
{
	parent = NULL;
	geometry = NULL;
}



InstanceGeometry::InstanceGeometry(const InstanceGeometry& instanceGeometry)
{
	//parent = new Node;
	//*parent = *(instanceGeometry.parent);
	
	if (instanceGeometry.geometry != NULL) {
		geometry = new Geometry;
		*geometry = *(instanceGeometry.geometry);
	}
	else {
		geometry = NULL;
	}


}

void InstanceGeometry::render()
{
	//Geometry* geometry;
	
	if (geometry != NULL) {
		ovInt _objectCount = geometry->objects.size();
		for (ovInt i=0; i<_objectCount; i++) {
			geometry->objects[i].render();

		}
	}
}