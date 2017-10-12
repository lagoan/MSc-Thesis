/*
 *  ovVector3D.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 24/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <cmath>

#include "ovVector3D.h"

ovVector3D::ovVector3D(ovFloat X, ovFloat Y, ovFloat Z)
{
	set(X, Y, Z);
}

ovVector3D::ovVector3D(ovFloat v[3])
{
	set(v[0], v[1], v[2]);
}

void ovVector3D::set(ovFloat X, ovFloat Y, ovFloat Z)
{
	_data.x = X;
	_data.y = Y;
	_data.z = Z;
}

void ovVector3D::set(ovFloat* data)
{
	set(data[0], data[1], data[2]);
}

ovFloat ovVector3D::getLength()
{
	return sqrt(getSquaredLength());
}

ovFloat ovVector3D::getSquaredLength()
{
	ovFloat _result = 0.0f;
	
	_result += _data.x * _data.x;
	_result += _data.y * _data.y;
	_result += _data.z * _data.z;

	return _result;

}

ovVector3D ovVector3D::normalize()
{
	ovVector3D _result;
	ovFloat _length;

	_length = getLength();

	_result._data.x = _data.x / _length;
	_result._data.y = _data.y / _length;
	_result._data.z = _data.z / _length;

	return _result;
}

ovVector3D ovVector3D::crossP(ovVector3D v)
{
	ovVector3D _result;

	_result._data.x =  ( _data.y * v.getZ() ) - ( _data.z * v.getY() );
	_result._data.y =  ( _data.z * v.getX() ) - ( _data.x * v.getZ() );
	_result._data.z =  ( _data.x * v.getY() ) - ( _data.y * v.getX() );

	return _result;
}

ovFloat ovVector3D::dotP(ovVector3D v)
{
	ovFloat _result = 0.0;

	_result += _data.x * v.getX();
	_result += _data.y * v.getY();
	_result += _data.z * v.getZ();

	return _result;
}

ovFloat ovVector3D::getDistanceTo(ovVector3D v)
{
	ovVector3D result;

	result = *this - v;

	return result.getLength();
}

ovFloat ovVector3D::getDistanceTo(ovFloat x, ovFloat y, ovFloat z)
{
	ovVector3D result;
	
	result.setX( this->getX() - x );
	result.setY( this->getY() - y );
	result.setZ( this->getZ() - z );
	
	return result.getLength();

}

