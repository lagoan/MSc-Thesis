/*
 *  Matrix.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 24/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include "system/types.h"
#include "ovVector3D.h"

class Matrix{

public:
	Matrix();
	~Matrix(){};

	void loadIdentity();
	void loadZero();
	const ovFloat* getM(){ return _matrix; };


	// operators
	ovVector3D operator* (ovVector3D v);
	Matrix operator+ (Matrix m);
	Matrix operator- (Matrix m);
	Matrix operator* (Matrix m);
	Matrix operator= (Matrix m);
	Matrix operator*= (Matrix m);
	Matrix operator* (ovFloat f);
	Matrix operator/ (ovFloat f);
	ovFloat& operator() (ovUInt x, ovUInt y);  // matrix subscript
	const ovFloat& operator() (ovUInt x, ovUInt y) const;
	

protected:

	// _matrix is internally arranged in column major form, but we provide the
	// () operator to handle it in the row major form, which is more familiar
	// to C programmers
	ovFloat _matrix[16];
};


inline ovVector3D Matrix::operator* (ovVector3D v)
{
	ovVector3D _result;
	ovFloat _sum;
	// TODO XXX matrix vector operation
	
	for (ovInt i = 0; i < 4; i++) {
		_sum = 0;
		for (ovInt j = 0; j < 4; j++) {
			_sum += (*this)(i,j) * v(j);
		}
		_result(i) = _sum;
	}
	
	return _result;
}

inline Matrix Matrix::operator+ (Matrix m)
{
	Matrix _result; 
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			_result(i, j) = (*this)(i,j) + m(i,j); 
		}
	}
	
	return _result;

}

inline Matrix Matrix::operator- (Matrix m)
{
	Matrix _result; 
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			_result(i, j) = (*this)(i,j) - m(i,j); 
		}
	}
	
	return _result;

}

inline Matrix Matrix::operator* (Matrix m)										// TODO test to see if it is gl correct
{
	Matrix _result; 
	ovFloat _sum;
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			_sum = 0;

			for (ovInt k = 0; k < 4; k++) {
				_sum += (*this)(i,k) * m(k,j);
			}

			_result(i,j) = _sum;
		}
	}
	
	return _result;

}

inline Matrix Matrix::operator= (Matrix m)
{
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			(*this)(i, j) = m(i, j);
		}
	}
	
	return (*this);

}

inline Matrix Matrix::operator*= (Matrix m)	
{
	Matrix _mat;
	_mat = *this;
	*this = _mat * m;
	return *this;
}

inline Matrix Matrix::operator* (ovFloat f)
{
	Matrix _result; 
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			_result(i, j) = (*this)(i,j) * f; 
		}
	}
	
	return _result;

}

inline Matrix Matrix::operator/ (ovFloat f)
{
	Matrix _result; 
	
	for (ovInt i = 0; i < 4; i++) {
		for (ovInt j = 0; j < 4; j++) {
			_result(i, j) = (*this)(i,j) / f; 
		}
	}
	
	return _result;

}

inline ovFloat& Matrix::operator() (ovUInt x, ovUInt y)
{
	return _matrix[x + y * 4];
}

inline const ovFloat& Matrix::operator() (ovUInt x, ovUInt y) const
{
	return _matrix[x + y * 4];
}


#endif