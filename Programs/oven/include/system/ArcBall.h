/*
 *  ArcBall.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 28/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

/*
 * The ArcBall class will give us an intuitive rotation for the camera.
 * This implementation follows that of Terence J. Grant (tjgrant@tatewake.com) 
 * which as of 28/08/07 can be found in the url:
 * http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=48
 *
 */

#ifndef ARCBALL_H
#define ARCBALL_H


#include "primitives/ovVector3D.h"
#include "primitives/Matrix.h"
#include "types.h"

class ArcBall
{
public:
	ArcBall();
	~ArcBall(){};
	Matrix getM(){ return _thisRotation; };

	void startRotation(ovInt x, ovInt y);
	void dragRotation(ovInt x, ovInt y);
	void setBounds(ovInt width, ovInt height);

private:
	Matrix	   _thisRotation;
	Matrix	   _lastRotation;

	ovVector3D   _startVector;
	ovVector3D   _endVector;

	ovFloat    _adjustWidth;    
    ovFloat    _adjustHeight;

	ovVector3D mapToSphere(ovInt x, ovInt y);
	Matrix computeRotation(ovTuple4f quaternion);
	

};


#endif