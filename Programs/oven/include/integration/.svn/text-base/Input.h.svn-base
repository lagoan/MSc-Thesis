/*
 *  Input.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 21/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef INPUT_H
#define INPUT_H

#include <vector>
#include <QString>

#include <dom/domInputLocalOffset.h>
#include <dae/daeURI.h>

#include "system/types.h"

class Input 
{
public:
	Input();
	Input(const Input& in);
	~Input(){};

	// COLLADA attributes
	QString semantic;
	ovInt	offset;
	ovInt	set;
	daeURI *source; // XXX check there is no memory leak

	ovInt	sourceIndex;

	static std::vector<Input> getInputs(domInputLocalOffset_Array inputArray);
};


#endif