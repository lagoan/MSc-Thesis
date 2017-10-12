/*
 *  Source.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 21/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SOURCE_H
#define SOURCE_H

#include <vector>
#include <QString>
 
#include "system/types.h"
#include "primitives/Polygons.h"

#include "Input.h"

class Source {
public:
	
	QString id;
	QString name;

	std::vector<ovFloat> floatArray;
	std::vector<ovInt>	 intArray;
	std::vector<ovBool>	 boolArray;
	std::vector<QString> nameArray;

	static void setPolygonSources(Polygons* _polygons, std::vector<Source>& _sources, std::vector<Input>& _inputs);
	static std::vector<Source> getSources(std::vector<Input>& inputs);
	

};


#endif