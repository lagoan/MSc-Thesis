/*
 *  ArcBall.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 28/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <cmath>
#include <cassert>

#include "system/constants.h"
#include "system/LoggingSystem.h"

#include "ArcBall.h"

ArcBall::ArcBall()
{
	_startVector.set(0.0f, 0.0f, 0.0f);
	_endVector.set(0.0f, 0.0f, 0.0f);

	_thisRotation.loadIdentity();
	_lastRotation.loadIdentity();
}

ovVector3D ArcBall::mapToSphere(ovInt x, ovInt y)
{
	ovVector3D  _result;
	ovTuple2f _newPoint;
	ovFloat   _length;
	ovFloat   _norm;

	// Adjust point coords and scale down to range of [-1 ... 1]
	_newPoint.x = (x * _adjustWidth) - 1.0f;
	_newPoint.y = 1.0f - (y * _adjustHeight);
	
	_length = (_newPoint.x * _newPoint.x) + (_newPoint.y * _newPoint.y);	
   
	if (_length > 1.0f) {
		//Compute a normalizing factor (radius / sqrt(length))
        _norm    = 1.0f / sqrt( _length );
		
		// TODO FIXME if click is done outside of the sphere the changes are erratic
        
		//Return the "normalized" vector, a point on the sphere
		_result.set( _newPoint.x * _norm,
					 _newPoint.y * _norm,
					 0.0f );
		
		
	}
	else {
        _result.set( _newPoint.x,
					 _newPoint.y,
					 sqrt( 1.0f - _length ) );
	}

	return _result;

}

void ArcBall::startRotation(ovInt x, ovInt y)
{
	_startVector = mapToSphere(x, y);
	_lastRotation = _thisRotation;
}

void ArcBall::dragRotation(ovInt x, ovInt y)
{
	ovTuple4f  _quaternion;
	ovVector3D _perpendicular;
	
	_endVector = mapToSphere(x, y);

	_perpendicular = _startVector.crossP( _endVector );

	if ( _perpendicular.getLength() > EPSILON ) {	// if its non-zero
		// We're ok, so return the perpendicular vector as the transform after all
		_quaternion.x = _perpendicular.getX();
		_quaternion.y = _perpendicular.getY();
		_quaternion.z = _perpendicular.getZ();
		// In the quaternion values, w is cosine (theta / 2), where theta is rotation angle
		_quaternion.w = _startVector.dotP( _endVector );


	}
	else {											//if its -zero
		_quaternion.x = 0.0f;
		_quaternion.y = 0.0f;
		_quaternion.z = 0.0f;
		_quaternion.w = 0.0f;
		
	}

	_thisRotation = computeRotation( _quaternion );
	_thisRotation *= _lastRotation;
}

void ArcBall::setBounds(ovInt width, ovInt height)
{
	assert((width > 1.0f) && (height > 1.0f));

    //Set adjustment factor for width/height
    _adjustWidth  = 1.0f / (((ovFloat)width  - 1.0f) * 0.5f);
    _adjustHeight = 1.0f / (((ovFloat)height - 1.0f) * 0.5f);

}

Matrix ArcBall::computeRotation(ovTuple4f quaternion){ // REWRITTEN

	Matrix _result;

	ovFloat n, s;

	ovFloat xx, yy, zz, ww;
	ovFloat xy, xz, xw;
	ovFloat yz, yw;
	ovFloat zw;

	xx = quaternion.x * quaternion.x;
	yy = quaternion.y * quaternion.y;
	zz = quaternion.z * quaternion.z;
	ww = quaternion.w * quaternion.w;
	
	xy = quaternion.x * quaternion.y;
	xz = quaternion.x * quaternion.z;
	xw = quaternion.x * quaternion.w;
	
	yz = quaternion.y * quaternion.z;
	yw = quaternion.y * quaternion.w;
	
	zw = quaternion.z * quaternion.w;


	n = xx + yy + zz + ww;
   
	s = (n > 0.0f) ? (2.0f / n) : 0.0f;

	_result.loadIdentity();
	
	_result(0,0) = 1.0f - s*(yy + zz) ;
	_result(0,1) = s*(xy - zw);
	_result(0,2) = s*(xz + yw);

	_result(1,0) = s*(xy + zw);
	_result(1,1) = 1.0f - s*(xx + zz);
	_result(1,2) = s*(yz - xw);

	_result(2,0) = s*(xz - yw);
	_result(2,1) = s*(yz + xw);
	_result(2,2) = 1.0f - s*(xx+yy);

	return _result;

	
}
