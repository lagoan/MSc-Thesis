/*
 *  Transformation.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef	TRANSFORMATION_H
#define TRANSFORMATION_H

#include "system/types.h"
#include "primitives/TransformationMatrix.h"


class Transformation
{

public:

	Transformation() {
		values[0] = 0;
		values[1] = 0;
		values[2] = 0;
		values[3] = 0;
	};
	
	Transformation(TransformationType, ovFloat a, ovFloat b, ovFloat c, ovFloat d) {
		values[0] = a;
		values[1] = b;
		values[2] = c;
		values[3] = d;
	};
		
	Transformation(TransformationType, ovFloat a, ovFloat b, ovFloat c) {
		values[0] = a;
		values[1] = b;
		values[2] = c;
		values[3] = 0;
	};
	
	void setValues(ovFloat a, ovFloat b, ovFloat c, ovFloat d);
	void setValues(ovFloat a, ovFloat b, ovFloat c);
	
	TransformationType	type;
	ovFloat values[4];
	

};

#endif