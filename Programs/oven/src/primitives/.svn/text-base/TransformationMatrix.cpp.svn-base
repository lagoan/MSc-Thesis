/*
 *  TransformationMatrix.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 25/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <cmath>

#include "TransformationMatrix.h"

#include "system/types.h"

TransformationMatrix::TransformationMatrix()
{
	loadIdentity();
}

void TransformationMatrix::translate(ovFloat x, ovFloat y, ovFloat z)
{
	Matrix _m;
	_m.loadIdentity();
	_m(0,3) = x;
	_m(1,3) = y;
	_m(2,3) = z;
	

	(*this) *= _m;
}

void TransformationMatrix::rotate(ovFloat angle, ovFloat x, ovFloat y, ovFloat z)
{
	ovFloat _norm;
	ovFloat _cos, _sin;
	
	Matrix _m;

	_norm = sqrt(x*x + y*y + z*z);
	_cos = cos(angle * ovPI / 180);
	_sin = sin(angle * ovPI / 180);


	x /= _norm;
	y /= _norm;
	z /= _norm;

	_m(0, 0)=x*x*(1-_cos)+_cos;
	_m(0, 1)=x*y*(1-_cos)-z*_sin;
	_m(0, 2)=x*z*(1-_cos)+y*_sin;
	_m(0, 3)=0;
	_m(1, 0)=y*x*(1-_cos)+z*_sin;
	_m(1, 1)=y*y*(1-_cos)+_cos;
	_m(1, 2)=y*z*(1-_cos)-x*_sin;
	_m(1, 3)=0;
	_m(2, 0)=x*z*(1-_cos)-y*_sin;
	_m(2, 1)=y*z*(1-_cos)+x*_sin;
	_m(2, 2)=z*z*(1-_cos)+_cos;
	_m(2, 3)=0;
	_m(3, 0)=0;
	_m(3, 1)=0;
	_m(3, 2)=0;
	_m(3, 3)=1;

	(*this) *= _m;

}

void TransformationMatrix::scale(ovFloat x, ovFloat y, ovFloat z)
{
	Matrix _m;
	
	_m(0,0) = x;
	_m(1,1) = y;
	_m(2,2) = z;
	_m(3,3) = 1.0;

	(*this) *= _m;

}