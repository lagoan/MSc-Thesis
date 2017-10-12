/*
 *  ovKDtree.h
 *  ovKDtree
 *
 *  Created by Omar Rodriguez-Arenas on 23/03/08.
 *  Copyright 2008 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ovKDtree_H
#define ovKDtree_H

#include <vector>

#include "Particle.h"


#define ovKDtreeDIMENTION 3

class ovKDtreeDataTypeComp 
{
	private:
		int comparissonDimention;
	public:
		ovKDtreeDataTypeComp(int n) : comparissonDimention(n) {}
		int operator()(Particle* a, Particle* b) {
			return (a->position[comparissonDimention] < b->position[comparissonDimention]);
		};
};


/* 
 * Important: ovKDtree could have some memory leak problems since
 * we did not write the operator=, regardless, they should be small
 * enough to be irrelevant at the moment
 */


class ovKDtree 
{

public:


	ovKDtree();
	~ovKDtree();
	void clear();
	Particle* getData(){return _data;}
	void addNodes(std::vector<Particle> &m_pfPrtls); 
	 
	ovKDtree* findNearestNeighbor(Point_3 &target,  double maxDistSqd);
	void rangeSearch(Point_3 & target, double maxDist, std::vector<Particle *> &result);
	
	void setValues( Particle* data, unsigned int height);
	void insertInstance(Particle* input);
	
	static double SplineWeightFunction(double distance, double radius);



private:

	
	Particle* _data;
	unsigned int nodeHeight;
	unsigned int splitAxis; 
	ovKDtree *_child[2]; // 0 near, 1 far

	static unsigned long int lastUpdate;
	unsigned long int nodeUpdate; 

	//void addNodes(std::vector<Particle*>);
	ovKDtree* findNearestNeighbor(ovKDtree* pivot, Point_3& target, double &maxDistSqd);
	ovKDtree* rangeSearch(ovKDtree* pivot, Point_3 &target, double &maxDistSqd, std::vector<Particle *> &result);
	
	ovKDtree* generateNode(std::vector< Particle* >&, const int height);
	Particle* splitData(std::vector< Particle* > data, std::vector< Particle* >& left, std::vector< Particle* >& right, unsigned int splitAxis);
	//Particle* splitData(std::vector< Particle* >&, std::vector< Particle* >&, std::vector< Particle* >&, unsigned int splitAxis);
	
	//double getDistSqd(Particle &b);
	double getDistSqd(Point_3 &b);
		

};

#endif
