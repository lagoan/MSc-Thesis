/*
 *  ExporterXYZ.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 09-10-05.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef EXPORTERXYZ_H
#define EXPORTERXYZ_H

#include <QObject>
#include <QHash>
#include <QString>

#include <map>
#include <list>


#include "ProcessedScene.h"
#include "ProcessedNode.h"

#include "Task.h"
#include "CGALDefinitions.h"

class ExporterXYZ : public Task {

	Q_OBJECT
	
public:
	ExporterXYZ();
	~ExporterXYZ();

	QString getData(ProcessedScene &scene);

	
private:
	/*
	bool IsPointOnTriangle(Point_2 p, 
						   Point_2 a, 
						   Point_2 b, 
						   Point_2 c);
	bool IsPosOnSrfcByItsCGALConvexHull(Point_3 & target,
										std::vector<Point_3> &points,
										Vector_3 & normal);
 	*/
};



#endif