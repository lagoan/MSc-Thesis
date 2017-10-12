/*
 *  ovVector3D.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 24/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef ovVector3D_H
#define ovVector3D_H

#include <cmath>

#include "system/types.h"
#include "system/constants.h"


//#define VECTOR_SIMILARITY_THRESHOLD 0.00001

class ovVector3D {
public:
	ovVector3D(){
		_data.x = 0;
		_data.y = 0;
		_data.z = 0;
		_data.w = 1;

	};
	ovVector3D(ovFloat X, ovFloat Y, ovFloat Z);
	ovVector3D(ovFloat v[3]);
	~ovVector3D(){};

	inline ovFloat getX(){ return _data.x; };
	inline ovFloat getY(){ return _data.y; };
	inline ovFloat getZ(){ return _data.z; };
	inline ovFloat* getT(){ return _data.T; };



	void set(ovFloat X, ovFloat Y, ovFloat Z);
	void set(ovFloat* data);
	
	inline void setX(ovFloat X){ _data.x = X; };
	inline void setY(ovFloat Y){ _data.y = Y; };
	inline void setZ(ovFloat Z){ _data.z = Z; };
	
	// vector operations

	ovFloat getLength();
	ovFloat getSquaredLength();
	ovVector3D normalize();
	ovVector3D crossP(ovVector3D);
	ovFloat dotP(ovVector3D);
	ovFloat getDistanceTo(ovVector3D v);
	ovFloat getDistanceTo(ovFloat x, ovFloat y, ovFloat z);


	// operators
	ovVector3D operator+ (ovVector3D b);
	ovVector3D operator- (ovVector3D b);
	ovVector3D operator* (ovFloat b);
	ovVector3D operator*= (ovFloat b);
	ovVector3D operator/ (ovFloat b);
	ovVector3D operator/= (ovFloat b);
	ovVector3D operator+= (ovVector3D b);
	ovVector3D operator= (const ovVector3D b);
	ovVector3D operator= (const ovTuple3f b);
	ovBool operator== (const ovVector3D& b) const;
	//friend ovBool operator== (const ovVector3D& a, const ovVector3D& b) const;
	ovBool operator!= (ovVector3D b);
	ovBool operator< (ovVector3D b);
	ovFloat& operator() (ovInt i);  // subscript
	const ovFloat& operator() (ovInt i) const;

private:
	// 4D tuple to deal with the transformation matrixes
	// The 4th element (w) is ignored for everything else
	ovTuple4f _data;	

};

ovVector3D inline ovVector3D::operator+ (ovVector3D b)
{
	ovVector3D _result;
	ovTuple3f _temp;

	_temp.x = _data.x + b.getX();
	_temp.y = _data.y + b.getY();
	_temp.z = _data.z + b.getZ();

	_result.set(_temp.T);

	return _result;

}


ovVector3D inline ovVector3D::operator- (ovVector3D b)
{
	ovVector3D _result;
	ovTuple3f _temp;

	_temp.x = _data.x - b.getX();
	_temp.y = _data.y - b.getY();
	_temp.z = _data.z - b.getZ();

	_result.set(_temp.T);

	return _result;
}

ovVector3D inline ovVector3D::operator* (ovFloat b)
{
	ovVector3D _result;
	ovTuple3f _temp;

	_temp.x = _data.x * b;
	_temp.y = _data.y * b;
	_temp.z = _data.z * b;

	_result = _temp;

	return _result;
}

ovVector3D inline ovVector3D::operator*= (ovFloat b)
{
	_data.x = _data.x * b;
	_data.y = _data.y * b;
	_data.z = _data.z * b;

	return *this;

}

ovVector3D inline ovVector3D::operator/= (ovFloat b)
{
	_data.x = _data.x / b;
	_data.y = _data.y / b;
	_data.z = _data.z / b;

	return *this;

}


ovVector3D inline ovVector3D::operator/ (ovFloat b)
{
	ovVector3D _result;
	ovTuple3f _temp;

	_temp.x = _data.x / b;
	_temp.y = _data.y / b;
	_temp.z = _data.z / b;

	_result.set(_temp.T);

	return _result;
}

ovVector3D inline ovVector3D::operator= (const ovVector3D b)
{
	_data.x = b._data.x;
	_data.y = b._data.y;
	_data.z = b._data.z;

	return *this;
}

ovVector3D inline ovVector3D::operator= (const ovTuple3f b)
{
	_data.x = b.x;
	_data.y = b.y;
	_data.z = b.z;

	return *this;
}


ovVector3D inline ovVector3D::operator+= (ovVector3D b)
{
	*this = *this + b;
	return *this;

}

inline ovBool ovVector3D::operator== (const ovVector3D& b) const
{
	if ( std::abs( _data.x - b._data.x ) > EPSILON ) 
		return false;
	if ( std::abs( _data.y - b._data.y  ) > EPSILON ) 
		return false;
	if ( std::abs( _data.z - b._data.z  ) > EPSILON ) 
		return false;

	return true;
}


ovBool inline ovVector3D::operator!= (ovVector3D b)
{
	return (! ( *this == b )) ;
}

ovBool inline ovVector3D::operator< (ovVector3D b)
{
	return getLength() < b.getLength();
}

inline ovFloat& ovVector3D::operator() (ovInt i)
{
	return _data.T[i];
}

inline const ovFloat& ovVector3D::operator() (ovInt i) const
{
	return _data.T[i];
}

#endif