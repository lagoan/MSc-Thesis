/*
 *  TransformationMatrix.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 25/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TRANSFORMATIONMATRIX_H	
#define TRANSFORMATIONMATRIX_H

#include "system/types.h"
#include "Matrix.h"

enum TransformationType
{
	ovUnknown,
	ovTranslate,
	ovRotate,
	ovScale,
	ovLookAt,
	ovSkew,
	ovMatrix
};

class TransformationMatrix : public Matrix
{
public:
	TransformationMatrix();
	~TransformationMatrix(){};
	
	void translate(ovFloat x, ovFloat y, ovFloat z);
	void rotate(ovFloat angle, ovFloat x, ovFloat y, ovFloat z);
	void scale(ovFloat x, ovFloat y, ovFloat z);
	void skew();	// TODO skew
	void matrix();	// TODO matrix
	// TODO lookAt ?


};

#endif