
/*
 *  BaseProperties.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 13/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef BASEPROPERTIES_H
#define BASEPROPERTIES_H

#include <utility>
#include <QString>
#include <map>
#include <vector>
#include <QReadWriteLock>
#include "types.h"

template <typename T>
class BaseProperties
{
public:
	BaseProperties() {
		_customWidgetCount = 0;
		reset();
	}
	virtual void reset() {
		_valuesInt.clear();
		_valuesFloat.clear();
		_valuesBool.clear();
	}

	ovInt	geti(T propertyType);
	ovFloat getf(T propertyType);
	ovBool  getb(T propertyType);

	void seti(T propertyType, ovInt value);
	void setf(T propertyType, ovFloat value);
	void setb(T propertyType, ovBool value);

	static QString getName(T propertyType);

	BaseProperties& operator=(const BaseProperties& prop);
	BaseProperties(const BaseProperties &prop);

	virtual QString getTextValue( T propertyType) { return "Value";};
	


	std::map<T, ovInt> getValuesInt();
	std::map<T, ovFloat> getValuesFloat();
	std::map<T, ovBool> getValuesBool();
	
protected:
	
	std::map<T, ovInt>   _valuesInt;
	std::map<T, ovFloat> _valuesFloat;
	std::map<T, ovBool>	 _valuesBool;

	QReadWriteLock _intLock;
	QReadWriteLock _floatLock;
	QReadWriteLock _boolLock;

	ovInt	_customWidgetCount;
	
};

template <typename T>
inline BaseProperties<T>& BaseProperties<T>::operator=(const BaseProperties<T>& prop)
{
	_valuesInt		= prop._valuesInt;
	_valuesFloat	= prop._valuesFloat;

	return (*this);
}



template <typename T>
BaseProperties<T>::BaseProperties(const BaseProperties<T> &prop)
{
	
	_intLock.lockForWrite();
	_valuesInt		= prop._valuesInt;
	_intLock.unlock();

	_floatLock.lockForWrite();
	_valuesFloat	= prop._valuesFloat;
	_floatLock.unlock();

}

template <typename T>
ovInt BaseProperties<T>::geti(T propertyType)
{	
	ovInt _temp;
	_intLock.lockForRead();
	_temp = _valuesInt[propertyType];
	_intLock.unlock();
	return _temp;
}

template <typename T>
ovFloat BaseProperties<T>::getf(T propertyType)
{	
	ovFloat _temp;
	_floatLock.lockForRead();
	_temp = _valuesFloat[propertyType];
	_floatLock.unlock();
	return _temp;
}


template <typename T>
ovBool BaseProperties<T>::getb(T propertyType)
{	
	ovBool _temp;
	_boolLock.lockForRead();
	_temp = _valuesBool[propertyType];
	_boolLock.unlock();
	return _temp;
}

template <typename T>
void BaseProperties<T>::seti(T propertyType, ovInt value)
{
	_intLock.lockForWrite();
	_valuesInt[propertyType] = value;
	_intLock.unlock();
}

template <typename T>
void BaseProperties<T>::setf(T propertyType, ovFloat value)
{
	_floatLock.lockForWrite();
	_valuesFloat[propertyType] = value;
	_floatLock.unlock();
}

template <typename T>
void BaseProperties<T>::setb(T propertyType, ovBool value)
{
	_boolLock.lockForWrite();
	_valuesBool[propertyType] = value;
	_boolLock.unlock();
}

/*
template <typename T>
ovInt BaseProperties<T>::getPropertyCount() 
{
	
	ovInt _temp;

	_intLock.lockForRead();
	_floatLock.lockForRead();
	_boolLock.lockForRead();
	
	_temp = _valuesInt.size()   + 
			_valuesFloat.size() + 
			_valuesBool.size()  +
			_customWidgetCount  ;

	_boolLock.unlock();
	_floatLock.unlock();
	_intLock.unlock();
	
	return _temp;

}
*/

template <typename T>
std::map<T, ovInt> BaseProperties<T>::getValuesInt()
{

	std::map<T, ovInt> _temp;
	_intLock.lockForRead();
	_temp = _valuesInt;
	_intLock.unlock();
	return _temp;
}

template <typename T>
std::map<T, ovFloat>  BaseProperties<T>::getValuesFloat()
{
	std::map<T, ovFloat> _temp;
	_floatLock.lockForRead();
	_temp = _valuesFloat;
	_floatLock.unlock();
	return _temp;
}


template <typename T>
std::map<T, ovBool> BaseProperties<T>::getValuesBool()
{
	std::map<T, ovBool> _temp;
	_boolLock.lockForRead();
	_temp = _valuesBool;
	_boolLock.unlock();
	return _temp;
}

template <typename T>
QString BaseProperties<T>::getName(T propertyType)
{
	return "Name";

}



#endif