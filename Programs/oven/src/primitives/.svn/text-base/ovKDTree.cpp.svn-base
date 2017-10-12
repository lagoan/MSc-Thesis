/*
 *  ovKDTree.cpp
 *  ovKDTree
 *
 *  Created by Omar Rodriguez-Arenas on 23/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#include <vector>
#include <algorithm>
#include <climits>

#include "ovKDTree.h"

//unsigned long int ovKDTree::lastUpdate = 0;

ovKDTree::ovKDTree()
{
	_child[0] = NULL;
	_child[1] = NULL;
	_data = NULL;
	nodeHeight = 0;
	splitAxis = 0;
	nodeUpdate = 0;
	//minPrtlPos = NULL;
	//maxPrtlPos = NULL;
}

ovKDTree::~ovKDTree()
{
	clear();
}

/* addNodes will set the value to the root node and will continue
 * to create the rest of the tree by recursively calling generateNode()
 */

void ovKDTree::addNodes(TGeomElemArray<CFluidPrtl> &dataList, bool fullSort)
{
	std::vector< CFluidPrtl* > newData, leftVector, rightVector;
	//std::sort(dataList.begin(), dataList.end(), ovKDTreeDataTypeComp(0));
	
	nodeHeight = 0;
	splitAxis = 0;
	nodeUpdate = 0;
	//ovKDTree::lastUpdate = 0;
	clear();

	//if (minPrtlPos)
	//	delete minPrtlPos;
	//if	(maxPrtlPos)
	//	delete maxPrtlPos;

	//minPrtlPos = new int[3];
	//maxPrtlPos = new int[3];

	for (int i=0; i<3; i++) {
		minPrtlPos[i] = INT_MAX;
		maxPrtlPos[i] = INT_MIN;
	}

	srand(42);
	
	for (int i=0; i< dataList.m_paNum; i++) {
		newData.push_back(dataList[i]);
	}

	_data = this->splitData(newData, leftVector, rightVector, splitAxis);
	
	setMinMaxPos(_data, minPrtlPos, maxPrtlPos);

	_child[0] = generateNode( leftVector , 1, minPrtlPos, maxPrtlPos );
	_child[1] = generateNode( rightVector, 1, minPrtlPos, maxPrtlPos );

} 

/*
void ovKDTree::addNodes(TGeomElemArray<CFluidPrtl> &m_pfPrtls)
{
	std::vector<CFluidPrtl*> input;
	for (int i=0; i < m_pfPrtls.m_paNum; ++i) {
		input.AppendOnePointer(m_pfPrtls[i]);
	}

	addNodes(input);
	

}
*/

/* clear will free the memory reserved for the kdTree leaving
 * _data (the particle information) intact
 */

void ovKDTree::clear()
{
	if (_child[0]) {
		delete _child[0];
	}
	
	if (_child[1]) {
		delete _child[1];
	}
	/*
	if (minPrtlPos) {
		delete minPrtlPos;
		minPrtlPos = NULL
	}
	if	(maxPrtlPos){
		delete maxPrtlPos;
	}
	 */
}

ovKDTree* ovKDTree::generateNode(std::vector< CFluidPrtl* >& dataList, const int height, int *minPrtlPos, int *maxPrtlPos, bool fullSort)
{
	
	if (dataList.size() == 0) {
		return NULL;
	}
	
	ovKDTree *result = new ovKDTree;
	std::vector< CFluidPrtl* > leftVector, rightVector;
	
	result->nodeHeight = height;
	result->splitAxis =  height % OVKDTREEDIMENTION;
	result->nodeUpdate = 0;		
	//std::sort(dataList.begin(), dataList.end(), ovKDTreeDataTypeComp(result->splitAxis));

	result->_data = result->splitData(dataList, leftVector, rightVector, result->splitAxis, fullSort);

	if ( result->_data == NULL ) {
		return NULL;
	}
	setMinMaxPos(result->_data , minPrtlPos, maxPrtlPos);

	result->_child[0] = generateNode( leftVector, (height + 1), minPrtlPos, maxPrtlPos, fullSort );
	result->_child[1] = generateNode( rightVector, (height + 1), minPrtlPos, maxPrtlPos, fullSort );
	
	return result;

}



/* splitData takes three vectors as arguments, it will split int the middle 
 * the first vector leving returning the value at the midle it empty and store 
 * the result on the last two vectors
 *
 */ 

CFluidPrtl* ovKDTree::splitData(std::vector< CFluidPrtl* > data, std::vector< CFluidPrtl* >& left, std::vector< CFluidPrtl* >& right, unsigned int splitAxis, bool fullSort)
{

	//int i;
	//std::vector<CFluidPrtl*> sortingVector;

	// get the las instance of the split value
	
	
	if (data.empty() ) {
		return NULL;
	}
	
	int index = data.size()/2;
	if (index >= data.size()) {
		index = 0;
	}
		//	sort all
	CFluidPrtl *result;
	if(fullSort && false ) {
		
		std::sort(data.begin(), data.end(), ovKDTreeDataTypeComp(splitAxis));
		
		result = data[index];
	} else {
	 
	
#if 0

	// sort partial

	int maxNodes = data.size() * 0.10f + 1;
	if (maxNodes > 10) maxNodes = 10;
	
	

	//srand(42);
	

	for (int i=0; i< maxNodes; i++) {
		sortingVector.push_back(data[ rand() % data.size() ]);
	}

	std::sort(sortingVector.begin(), sortingVector.end(), ovKDTreeDataTypeComp(splitAxis));

	CFluidPrtl *result = sortingVector[sortingVector.size()/2];

	
#else
	
	// take middle
	
	result = data[index];
#endif

	}

	//CFluidPrtl *result = data[ data.m_paNum /2 ];
	//for ( ; middle < data.m_paNum-1 && data[middle]->m_vpPos->v[splitAxis] == data[middle+1]->m_vpPos->v[splitAxis]; ++middle )
	//	;
	
	//middle = data.m_paNum/2;
	//CFluidPrtl *result = data[middle];

	for (int i = 0; i< data.size() ; i++ ) {
		if (data[i] == result) continue;
		
		if ( data[i]->m_vpPos->v[splitAxis] <= result->m_vpPos->v[splitAxis] ) {
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
	for ( middle = data.m_paNum/2; middle < data.m_paNum-1 && data[middle]->m_vpPos->v[splitAxis] == data[middle+1]->m_vpPos->v[splitAxis]; ++middle )
		;
	
	//middle = data.m_paNum/2;
	CFluidPrtl *result = data[middle];

	for (i = 0; i< data.m_paNum ; i++ ) {
		if (data[i] == result) continue;
		
		if ( data[i]->m_vpPos->v[splitAxis] <= result->m_vpPos->v[splitAxis] ) {
			left.AppendOnePointer(data[i]);
		
		}
		else {
			right.AppendOnePointer(data[i]);
		}

	}
	return result;
#endif 
}



inline ovKDTree* ovKDTree::findNearestNeighbor(CVector3D* target,  SimuValue maxDistSqd)
{
	
	return findNearestNeighbor(this, target, maxDistSqd);

}

ovKDTree* ovKDTree::findNearestNeighbor(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd)
{
	int first;
	ovKDTree *nearest = NULL;
	
	if (pivot == NULL) {
		return this;
	}
	unsigned int currentSplitAxis = pivot->splitAxis;
	first = target->v[currentSplitAxis] > pivot->_data->m_vpPos->v[currentSplitAxis];
	
	
	nearest = pivot->findNearestNeighbor(pivot->_child[first], target, maxDistSqd);
	
	SimuValue distSqd = nearest->getDistSqd(target);

	if (distSqd < maxDistSqd) {
		maxDistSqd = distSqd;
	}
	
	
	
	SimuValue temp = pivot->_data->m_vpPos->v[currentSplitAxis] - target->v[currentSplitAxis];
	
	temp = temp*temp;

	
	if (temp <=	maxDistSqd) {
		
		SimuValue pivotSqdDist = pivot->getDistSqd(target);
		if (pivotSqdDist < distSqd) {
			nearest = pivot;
			distSqd = pivotSqdDist;
			maxDistSqd = distSqd;
		}


		ovKDTree *tempNearest = pivot->findNearestNeighbor(pivot->_child[first ^ 1], target, maxDistSqd);
		SimuValue tempDistSqd = tempNearest->getDistSqd(target);
		if ( tempDistSqd <= distSqd ) {
			nearest = tempNearest;
			distSqd = tempDistSqd;
			maxDistSqd = distSqd;
		}

	}
	
		
	return nearest;
}


void ovKDTree::rangeSearch(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result)
{
        maxDist = maxDist*maxDist;
	//ovKDTree::lastUpdate++;
	CVector3D volNearPt;
	volNearPt.v[0] = target->v[0];
	volNearPt.v[1] = target->v[1];
	volNearPt.v[2] = target->v[2];
        ovKDTree* root = this;
        rangeSearch(root, target, maxDist, volNearPt, result);
}

ovKDTree* ovKDTree::rangeSearch(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd, CVector3D &volNearPt , TPointerArray<CFluidPrtl> &result)
{
	
	ovKDTree *nearest = NULL;
	
	
    if (pivot == NULL) {
		return nearest;
	}
	
	if (pivot->getDistSqd(target) <= maxDistSqd) {
		result.AppendOnePointer( pivot->_data);
	}
	
	int first = target->v[pivot->splitAxis] > pivot->_data->m_vpPos->v[pivot->splitAxis];
	
	rangeSearch(pivot->_child[first], target, maxDistSqd, volNearPt, result);
	
    double oldValue = volNearPt.v[pivot->splitAxis];
    volNearPt.v[pivot->splitAxis] = pivot->_data->m_vpPos->v[pivot->splitAxis];
	

	
	if(getDistSqd(&volNearPt , target) <= maxDistSqd) {
		rangeSearch(pivot->_child[first ^ 1], target, maxDistSqd, volNearPt ,result);
	}
	
    volNearPt.v[pivot->splitAxis] = oldValue;
	
	return nearest;
	

}


void ovKDTree::rangeSearchOne(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result)
{
        maxDist = maxDist*maxDist;
	//ovKDTree::lastUpdate++;
	CVector3D volNearPt;
	volNearPt.v[0] = target->v[0];
	volNearPt.v[1] = target->v[1];
	volNearPt.v[2] = target->v[2];
        ovKDTree* root = this;
        rangeSearch(root, target, maxDist, volNearPt, result);
}

ovKDTree* ovKDTree::rangeSearchOne(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd, CVector3D &volNearPt , TPointerArray<CFluidPrtl> &result)
{
	
	ovKDTree *nearest = NULL;
	
	
    if (pivot == NULL) {
		return nearest;
	}
	
	if (pivot->getDistSqd(target) <= maxDistSqd) {
		result.AppendOnePointer( pivot->_data);
		return;
	}
	if (result.m_paNum > 0) return;
	
	int first = target->v[pivot->splitAxis] > pivot->_data->m_vpPos->v[pivot->splitAxis];
	
	rangeSearch(pivot->_child[first], target, maxDistSqd, volNearPt, result);
	
    double oldValue = volNearPt.v[pivot->splitAxis];
    volNearPt.v[pivot->splitAxis] = pivot->_data->m_vpPos->v[pivot->splitAxis];
	

	
	if(getDistSqd(&volNearPt , target) <= maxDistSqd) {
		rangeSearch(pivot->_child[first ^ 1], target, maxDistSqd, volNearPt ,result);
	}
	
    volNearPt.v[pivot->splitAxis] = oldValue;
	
	return nearest;
	

}


/*
inline SimuValue ovKDTree::getDistSqd(CFluidPrtl &b)
{
		
	return getDistSqd(b->m_vpPos->v);
		
}	
*/

SimuValue ovKDTree::getDistSqd(CVector3D* a, CVector3D* b)
{
		
	SimuValue dx = a->v[0] - b->v[0];
	SimuValue dy = a->v[1] - b->v[1];
	SimuValue dz = a->v[2] - b->v[2];
	
	return dx*dx + dy*dy + dz*dz;
		
}

SimuValue ovKDTree::getDistSqd(CVector3D* b)
{
		
	SimuValue dx = _data->m_vpPos->v[0] - b->v[0];
	SimuValue dy = _data->m_vpPos->v[1] - b->v[1];
	SimuValue dz = _data->m_vpPos->v[2] - b->v[2];
	
	return dx*dx + dy*dy + dz*dz;
		
}	

void ovKDTree::insertInstance(CFluidPrtl *input)
{
	
	int newPos = input->m_vpPos->v[splitAxis] <= _data->m_vpPos->v[splitAxis] ? 0 : 1;
		
	
	if ( _child[newPos] != NULL ) {
		// continue looking for a place
		_child[newPos]->insertInstance(input);
	} else {
		// found the place for the new node
		ovKDTree *newNode = new ovKDTree;
		newNode->setValues(input, nodeHeight + 1);
		_child[newPos] = newNode;
		
	}

}

void ovKDTree::setValues( CFluidPrtl *data, unsigned int height)
{
	_data = data;
	nodeHeight = height;
	splitAxis = height % 3;

}

void ovKDTree::setMinMaxPos(CFluidPrtl* data, int *minPrtlPos, int *maxPrtlPos)
{

	if (data == NULL)
		return;

	for (int i=0; i<3; i++) {
		if (data->m_vpPos->v[i] < minPrtlPos[i]){
			minPrtlPos[i] = data->m_vpPos->v[i];
		}
		if (data->m_vpPos->v[i] > minPrtlPos[i]){
			maxPrtlPos[i] = data->m_vpPos->v[i];
		}
	}


}

#if 0
void ovKDTree::rangeSearchFluidParticles(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result)
{
	maxDist = maxDist*maxDist;
	//ovKDTree::lastUpdate++;
	rangeSearchFluidParticles(this, target, maxDist, result);
}

ovKDTree* ovKDTree::rangeSearchFluidParticles(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd, TPointerArray<CFluidPrtl> &result)
{


	int first;
	ovKDTree *nearest = NULL;
	
	
	if (pivot == NULL) {
		return this;
	}
	unsigned int currentSplitAxis = pivot->splitAxis;
	first = target->v[currentSplitAxis] > pivot->_data->m_vpPos->v[currentSplitAxis];
	
	nearest = pivot->rangeSearch(pivot->_child[first], target, maxDistSqd, result);
	
	SimuValue distSqd = nearest->getDistSqd(target);

	if (distSqd <= maxDistSqd) {
		if (nearest->nodeUpdate < ovKDTree::lastUpdate) {
			nearest->nodeUpdate = ovKDTree::lastUpdate;
			if (!nearest->_data->m_bpIsBubble)
				result.AppendOnePointer( nearest->_data );
		}
	}
		
	SimuValue temp = pivot->_data->m_vpPos->v[currentSplitAxis] - target->v[currentSplitAxis];
	
	temp = temp*temp;

	
	if (temp <=	maxDistSqd) {
		
		SimuValue pivotSqdDist = pivot->getDistSqd(target);

		
		if (pivotSqdDist <= maxDistSqd && pivot->nodeUpdate < ovKDTree::lastUpdate){
			pivot->nodeUpdate = ovKDTree::lastUpdate;
			if (pivotSqdDist < distSqd){
				nearest = pivot;
			}
			if (!pivot->_data->m_bpIsBubble)
				result.AppendOnePointer(pivot->_data);

		}

		if (temp < maxDistSqd) {

			ovKDTree *tempNearest = pivot->rangeSearch(pivot->_child[first ^ 1], target, maxDistSqd, result);
			SimuValue tempDistSqd = tempNearest->getDistSqd(target);

			if ( tempDistSqd < maxDistSqd && tempNearest->nodeUpdate < ovKDTree::lastUpdate) {
				tempNearest->nodeUpdate = ovKDTree::lastUpdate;
				if (tempDistSqd < distSqd && tempDistSqd < pivotSqdDist){
					nearest = tempNearest;
				}
				if (!tempNearest->_data->m_bpIsBubble)
					result.AppendOnePointer( tempNearest->_data );
				
				
			}
		}
		

	}
		
	return nearest;

}
#endif
