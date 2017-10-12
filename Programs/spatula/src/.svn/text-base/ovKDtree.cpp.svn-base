/*
 *  ovKDtree.cpp
 *  ovKDtree
 *
 *  Created by Omar Rodriguez-Arenas on 23/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <algorithm>
#include <climits>
#include <iostream>
#include "ovKDtree.h"

unsigned long int ovKDtree::lastUpdate = 0;

ovKDtree::ovKDtree()
{
	_child[0]  = NULL;
	_child[1]  = NULL;
	_data      = NULL;
	nodeHeight = 0;
	splitAxis  = 0;
	nodeUpdate = 0;

}

ovKDtree::~ovKDtree()
{
	clear();
}

/* addNodes will set the value to the root node and will continue
 * to create the rest of the tree by recursively calling generateNode()
 */

void ovKDtree::addNodes(std::vector<Particle> &dataList)
{
	std::vector< Particle* > newData, leftVector, rightVector;
	//std::sort(dataList.begin(), dataList.end(), ovKDtreeDataTypeComp(0));
	
	nodeHeight           = 0;
	splitAxis            = 0;
	nodeUpdate           = 0;
	ovKDtree::lastUpdate = 0;

	clear();

	srand(42);
	
	for (size_t i=0; i< dataList.size(); i++) {
		newData.push_back(&dataList[i]);
	}

	_data = this->splitData(newData, leftVector, rightVector, splitAxis);
	

	_child[0] = generateNode( leftVector , 1 );
	_child[1] = generateNode( rightVector, 1 );

} 



/* clear will free the memory reserved for the kdTree leaving
 * _data (the particle information) intact
 */

void ovKDtree::clear()
{
	if (_child[0]) {
		delete _child[0];
	}
	
	if (_child[1]) {
		delete _child[1];
	}
	
}

ovKDtree* ovKDtree::generateNode(std::vector< Particle* >& dataList, const int height)
{
	
	if (dataList.size() == 0) {
		return NULL;
	}
	
	
	ovKDtree *result = new ovKDtree;
	std::vector< Particle* > leftVector, rightVector;
	
	result->nodeHeight = height;
	result->splitAxis =  height % ovKDtreeDIMENTION;
	result->nodeUpdate = 0;		
	//std::sort(dataList.begin(), dataList.end(), ovKDtreeDataTypeComp(result->splitAxis));

	result->_data = result->splitData(dataList, leftVector, rightVector, result->splitAxis);

	if ( result->_data == NULL ) {
		return NULL;
	}

	result->_child[0] = generateNode( leftVector, (height + 1) );
	result->_child[1] = generateNode( rightVector, (height + 1) );
	
	return result;

}



/* splitData takes three vectors as arguments, it will split int the middle 
 * the first vector leving returning the value at the midle it empty and store 
 * the result on the last two vectors
 *
 */ 

Particle* ovKDtree::splitData(std::vector< Particle* > data, std::vector< Particle* >& left, std::vector< Particle* >& right, unsigned int splitAxis)
{

	//int i;
	std::vector<Particle*> sortingVector;

	// get the las instance of the split value
#if 0
	int maxNodes = data.size() * 0.10f + 1;
	if (maxNodes > 10) maxNodes = 10;
	
	

	//srand(42);
	

	for (int i=0; i< maxNodes; i++) {
		sortingVector.push_back(data[ rand() % data.size() ]);
	}

	std::sort(sortingVector.begin(), sortingVector.end(), ovKDtreeDataTypeComp(splitAxis));

	Particle *result = sortingVector[sortingVector.size()/2];

	
#else
	if (data.empty() ) {
		return NULL;
	}
	
	Particle *result = data[data.size()/2];
#endif

	//Particle *result = data[ data.m_paNum /2 ];
	//for ( ; middle < data.m_paNum-1 && data[middle]->position[splitAxis] == data[middle+1]->position[splitAxis]; ++middle )
	//	;
	
	//middle = data.m_paNum/2;
	//Particle *result = data[middle];

	for (int i = 0; i< data.size() ; i++ ) {
		if (data[i] == result) continue;
		
		if ( data[i]->position[splitAxis] <= result->position[splitAxis] ) {
			left.push_back(data[i]);
		
		}
		else {
			right.push_back(data[i]);
		}

	}
	return result;

#if 0
	int i;
	int middle;
	
	// get the las instance of the split value
	for ( middle = data.m_paNum/2; middle < data.m_paNum-1 && data[middle]->position[splitAxis] == data[middle+1]->position[splitAxis]; ++middle )
		;
	
	//middle = data.m_paNum/2;
	Particle *result = data[middle];

	for (i = 0; i< data.m_paNum ; i++ ) {
		if (data[i] == result) continue;
		
		if ( data[i]->position[splitAxis] <= result->position[splitAxis] ) {
			left.AppendOnePointer(data[i]);
		
		}
		else {
			right.AppendOnePointer(data[i]);
		}

	}
	return result;
#endif 
}



ovKDtree* ovKDtree::findNearestNeighbor(Point_3 & target,  double maxDistSqd)
{
	
	return findNearestNeighbor(this, target, maxDistSqd);

}

ovKDtree* ovKDtree::findNearestNeighbor(ovKDtree* pivot, Point_3 & target, double &maxDistSqd)
{
	int first;
	ovKDtree *nearest = NULL;
	
	if (pivot == NULL) {
		return this;
	}
	
	first = target[pivot->splitAxis] > pivot->_data->position[pivot->splitAxis];
	
	
	nearest = pivot->findNearestNeighbor(pivot->_child[first], target, maxDistSqd);
	
	double distSqd = nearest->getDistSqd(target);

	if (distSqd < maxDistSqd) {
		maxDistSqd = distSqd;
	}
		
	double temp = CGAL::to_double( pivot->_data->position[pivot->splitAxis] - target[pivot->splitAxis] );
	
	temp = temp*temp;

	
	if (temp <=	maxDistSqd) {
		
		double pivotSqdDist = pivot->getDistSqd(target);
		if (pivotSqdDist < distSqd) {
			nearest = pivot;
			distSqd = pivotSqdDist;
			maxDistSqd = distSqd;
		}


		ovKDtree *tempNearest = pivot->findNearestNeighbor(pivot->_child[first ^ 1], target, maxDistSqd);
		double tempDistSqd = tempNearest->getDistSqd(target);
		if ( tempDistSqd <= distSqd ) {
			nearest = tempNearest;
			distSqd = tempDistSqd;
			maxDistSqd = distSqd;
		}

	}
	
		
	return nearest;
}


void ovKDtree::rangeSearch(Point_3 & target, double maxDist, std::vector<Particle *> &result)
{
	maxDist = maxDist*maxDist;
	ovKDtree::lastUpdate++;
	rangeSearch(this, target, maxDist, result);
}

ovKDtree* ovKDtree::rangeSearch(ovKDtree* pivot, Point_3 & target, double &maxDistSqd, std::vector<Particle *> &result)
{
	int first;
	ovKDtree *nearest = NULL;
	
	
	if (pivot == NULL) {
		return this;
	}
	
	first = target[pivot->splitAxis] > pivot->_data->position[pivot->splitAxis];
	
	nearest = pivot->rangeSearch(pivot->_child[first], target, maxDistSqd, result);
	
	double distSqd = nearest->getDistSqd(target);

	if (distSqd <= maxDistSqd) {
		if (nearest->nodeUpdate < ovKDtree::lastUpdate) {
			nearest->nodeUpdate = ovKDtree::lastUpdate;
			result.push_back( nearest->_data );
		}
	}
		
	double temp = CGAL::to_double( pivot->_data->position[pivot->splitAxis] - target[pivot->splitAxis] );
	
	temp = temp*temp;

	
	if (temp <=	maxDistSqd) {
		
		double pivotSqdDist = pivot->getDistSqd(target);

		
		if (pivotSqdDist <= maxDistSqd && pivot->nodeUpdate < ovKDtree::lastUpdate){
			pivot->nodeUpdate = ovKDtree::lastUpdate;
			if (pivotSqdDist < distSqd){
				nearest = pivot;
			}
			result.push_back( pivot->_data );

		}

		if (temp < maxDistSqd) {

			ovKDtree *tempNearest = pivot->rangeSearch(pivot->_child[first ^ 1], target, maxDistSqd, result);
			double tempDistSqd = tempNearest->getDistSqd(target);

			if ( tempDistSqd < maxDistSqd && tempNearest->nodeUpdate < ovKDtree::lastUpdate) {
				tempNearest->nodeUpdate = ovKDtree::lastUpdate;
				if (tempDistSqd < distSqd && tempDistSqd < pivotSqdDist){
					nearest = tempNearest;
				}
				result.push_back( tempNearest->_data );
				
				
			}
		}
		

	}
		
	return nearest;
}

/*
inline double ovKDtree::getDistSqd(Particle &b)
{
		
	return getDistSqd(b->position);
		
}	
*/

double ovKDtree::getDistSqd(Point_3 & b)
{
		
	double dx = CGAL::to_double( _data->position[0] - b[0] );
	double dy = CGAL::to_double(_data->position[1] - b[1] );
	double dz = CGAL::to_double(_data->position[2] - b[2] );
	
	return dx*dx + dy*dy + dz*dz;
		
}	

void ovKDtree::insertInstance(Particle *input)
{
	
	int newPos = input->position[splitAxis] <= _data->position[splitAxis] ? 0 : 1;
		
	
	if ( _child[newPos] != NULL ) {
		// continue looking for a place
		_child[newPos]->insertInstance(input);
	} else {
		// found the place for the new node
		ovKDtree *newNode = new ovKDtree;
		newNode->setValues(input, nodeHeight + 1);
		_child[newPos] = newNode;
		
	}

}

void ovKDtree::setValues( Particle *data, unsigned int height)
{
	_data = data;
	nodeHeight = height;
	splitAxis = height % 3;

}


double ovKDtree::SplineWeightFunction(double distance, double radius)
{
	static const double pi = 4.0f * std::atan(1.0f);


	double h = radius / 2.0;
	
	double q = distance/h;
	double weight = 0;
	
	// cuadratic spline
#if 0	
	if (q >= 0 && q <= 2) {
		weight = 5.0/(4*pi*h*h*h) * ((3.0/16.0) *q*q - (3.0/4.0) * q + 3.0/4.0);
	} 
	return weight;
#endif
#if 0	
	// cubic spline
	double normalizing = 3.0 / (2 * pi * h*h*h);
	
	if (q >= 0 && q < 1) {
		weight = (2.0/3.0 - q*q) + (0.5* q*q*q);
	} else if (q >= 1 && q < 2) {
		weight = 1.0/6.0 * pow(2.0 - q, 3.0);
	} else { // q < 0
		return weight; // weight = 0
	}
	
	weight *= normalizing;
		
	return weight;
#endif
	
#if 1
	//std::cout << " q " << q << "\n";
	
	if (q >= 0 && q < 1) {
		weight = 1 - 1.5*pow(q, 2) + 0.75*pow(q, 3);
	} else if (q >= 1 && q < 2) {
		weight = 0.25*pow((2-q), 3);
	} else { // q < 0
		return weight; // weight = 0
	}
	
	weight /= pi * pow(h, 3);
	
	return weight;
	
#endif
}
