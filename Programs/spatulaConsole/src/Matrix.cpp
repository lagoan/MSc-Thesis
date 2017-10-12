/*
 *  Matrix.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 24/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#include "Matrix.h"

#include "system/types.h"

Matrix::Matrix()
{
	loadZero();
}


void Matrix::loadIdentity()
{
	for (ovInt i = 0; i < 4; i++) {
		(*this)(i,i) = 1;
	}
}

void Matrix::loadZero()
{
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++ ) {
			(*this)(i,j) = 0;
		}
	}
}
