/*
 *  ovKDTree.h
 *  ovKDTree
 *
 *  Created by Omar Rodriguez-Arenas on 23/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef OVKDTREE_H
#define OVKDTREE_H

#include <vector>

#include "simulation/TPointerArray.h"
#include "simulation/TGeomElemArray.h"
#include "simulation/FluidPrtl.h"
#include "simulation/Vector3D.h"


#define OVKDTREEDIMENTION 3

class ovKDTreeDataTypeComp 
{
	private:
		int comparissonDimention;
	public:
		ovKDTreeDataTypeComp(int n) : comparissonDimention(n) {}
		int operator()(CFluidPrtl* a, CFluidPrtl* b) {
			return (a->m_vpPos->v[comparissonDimention] < b->m_vpPos->v[comparissonDimention]);
		};
};


/* 
 * Important: ovKDTree could have some memory leak problems since
 * we did not write the operator=, regardless, they should be small
 * enough to be irrelevant at the moment
 */

class ovKDTree 
{

public:
	
	int minPrtlPos[3];
	int maxPrtlPos[3];

	ovKDTree();
	~ovKDTree();
	void clear();
	CFluidPrtl* getData(){return _data;}
	void addNodes(TGeomElemArray<CFluidPrtl> &m_pfPrtls, bool fullSort = false); 
	 
	ovKDTree* findNearestNeighbor(CVector3D* target,  SimuValue maxDistSqd);
	void rangeSearch(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result);
	void rangeSearchOne(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result);
	//void rangeSearchFluidParticles(CVector3D* target, SimuValue maxDist, TPointerArray<CFluidPrtl> &result);
	
	void setValues( CFluidPrtl* data, unsigned int height);
	void insertInstance(CFluidPrtl* input);
	


private:

	
	CFluidPrtl* _data;
	unsigned int nodeHeight;
	unsigned int splitAxis; 
	ovKDTree *_child[2]; // 0 near, 1 far

	//static unsigned long int lastUpdate;
	unsigned long int nodeUpdate; 


	ovKDTree* findNearestNeighbor(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd);
	ovKDTree* rangeSearch(ovKDTree* const pivot, CVector3D* target, SimuValue &maxDistSqd, CVector3D &volNearPt , TPointerArray<CFluidPrtl> &result);
	ovKDTree* rangeSearchOne(ovKDTree* const pivot, CVector3D* target, SimuValue &maxDistSqd, CVector3D &volNearPt , TPointerArray<CFluidPrtl> &result);
	//ovKDTree* rangeSearchFluidParticles(ovKDTree* pivot, CVector3D* target, SimuValue &maxDistSqd, TPointerArray<CFluidPrtl> &result);
	
	ovKDTree* generateNode(std::vector< CFluidPrtl* >&, const int height, int *minPrtlPos, int *maxPrtlPos, bool fullSort = false);
	CFluidPrtl* splitData(std::vector< CFluidPrtl* > data, std::vector< CFluidPrtl* >& left, std::vector< CFluidPrtl* >& right, unsigned int splitAxis, bool fullSort = false);
	//CFluidPrtl* splitData(std::vector< CFluidPrtl* >&, std::vector< CFluidPrtl* >&, std::vector< CFluidPrtl* >&, unsigned int splitAxis);
	
	//SimuValue getDistSqd(CFluidPrtl &b);
	SimuValue getDistSqd(CVector3D *b);
	void setMinMaxPos(CFluidPrtl* data, int *minPrtlPos, int *maxPrtlPos);
	SimuValue getDistSqd(CVector3D* a, CVector3D* b);

};

#endif
