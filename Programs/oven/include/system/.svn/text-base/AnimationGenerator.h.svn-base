/*
 *  AnimationGenerator.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 11/10/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef ANIMATIONGENERATOR_H
#define ANIMATIONGENERATOR_H

#include "types.h"
#include "constants.h"
#include "SurfaceMesher.h"
#include "scene/Scene.h"
#include "primitives/ovVector3D.h"

#include "simulation/SimuManager.h"
#include "simulation/SimuFlexApp.h"
#include "simulation/SimuConstant.h"

#include <QThread>
#include <QSemaphore>
#include <QMutex>

#include <utility>
#include <vector>

#include "primitives/ParticleInstance.h"
#include "primitives/Polygon.h"


#define MIN_SRFC_PRTL_DIST 0.1f
#define OUT_OF_RANGE_INDEX -1



class AnimationFrame 
{

public:
	ovInt frame;
	ovFloat animationTime;
	std::vector< Polygons > listFluids;
	std::vector< ParticleInstance* > listParticles;   
	std::vector< ParticleInstance* > listBubbles;     
	std::vector< ovInt > listParticleCount;
	std::vector< ovInt > listBubblesCount;
	std::vector< GLint > listGlNames;
	
	
	AnimationFrame operator= (const AnimationFrame &aFrame);
	
	AnimationFrame(const AnimationFrame &aFrame);
	AnimationFrame();
	~AnimationFrame();
	ovUInt getIndexByGLName(GLint);

};



inline AnimationFrame AnimationFrame::operator= (const AnimationFrame &aFrame)
{
	
	ParticleInstance *particles;
	ParticleInstance *bubbles;
	
	frame = aFrame.frame;
	animationTime = aFrame.animationTime;
	
	this->listFluids.clear(); 
	
	this->listFluids = aFrame.listFluids;
	this->listParticleCount.clear();
	this->listParticleCount = aFrame.listParticleCount;
	this->listBubblesCount.clear();
	this->listBubblesCount = aFrame.listBubblesCount;
	
	this->listGlNames.clear();
	this->listGlNames = aFrame.listGlNames;
	
	
	for (ovUInt i = 0; i < this->listParticles.size(); i++ ) {
		delete this->listParticles[i];
	}
	
	for (ovUInt i = 0; i < this->listBubbles.size(); i++ ) {
		delete this->listBubbles[i];
	}
	
	
	this->listParticles.clear();
	this->listBubbles.clear();
	
	for (ovUInt i=0; i<aFrame.listParticles.size(); i++) {
		
		particles = new ParticleInstance[ aFrame.listParticleCount[i] ];
		for (ovInt j=0; j < aFrame.listParticleCount[i]; j++) {
			particles[j] = aFrame.listParticles[i][j];
		}
		this->listParticles.push_back(particles);
		
		bubbles = new ParticleInstance[ aFrame.listBubblesCount[i] ];
		for (ovInt j=0; j < aFrame.listBubblesCount[i]; j++) {
			bubbles[j] = aFrame.listBubbles[i][j];
		}
		this->listBubbles.push_back(bubbles);
		
	}
	
	
	return *this;
	
}



class AnimationGenerator : public QThread
{
	Q_OBJECT

public:

	AnimationGenerator();
	~AnimationGenerator();
	void setScene(Scene scene);
	void setFrame(AnimationFrame& frame);
	void stop(){_stop = true; _semaphore->release();};
	void animate();
	void stopGenerating();
	void continueWithNewFrame();
	AnimationFrame getFrame();
	
signals:
	void newFrame();
	void newAnimation(int);
	void errorMessage(QString);
	
protected:
	void run();

private:
	Scene* _scene;
	QSemaphore* _semaphore;
	QSemaphore* _animationFrameLock;
	AnimationFrame _animationFrame;
	

	volatile ovBool _stop;
	volatile ovBool _stopAnimating;

	CSimuManager* _simuManager;
	CSimuFlexApp _simuFlexApp;
	
	//std::vector<Nef_polyhedron> cutOuts;

	
	void getStartEndPoints(Node node, ovTuple3f* start, ovTuple3f* end); // XXX remove
	Kernel::Iso_cuboid_3 getNodeBoundingBox(Node node);
	CPrtlFluid* generateFluidParticles(const Node &node);
	CPrtlFluid* generateSolidParticles(Node &node);
	//Nef_polyhedron generateCutOut(const Node &node);

	//bool pointIsInside(ovTuple3f& newPos, Node& node);
	//std::pair<ovVector3D, ovVector3D> getEdge(ovVector3D a, ovVector3D b);
	
	void setSimuValues(ovInt glName);
	void fillFrame();

	void printFluidInfo();

	void addParticleToFluid(CPrtlFluid* newFluid, 
							ovVector3D position, 
							CVector3D &velocity, 
							ovBool onSurface, 
							ovBool isFixed,
							ovBool isBubble,
							ovInt belongsToFacet = 0);



	/* All of the addParticles-On- functions will be able to register prticles to 
	 * their corresponding facets to be used with solid fluid collisions
	 */

	void addRegisteredParticlesOnSurface(Node node, CPrtlFluid* newFluid);

	void addParticlesOnVertices(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets = false);
	void addParticlesOnEdges(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets = false);
	void addParticlesOnFacets(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets = false);

	void addParticlesInVolume(Node node, CPrtlFluid* newFluid, ovBool isBubble);

	Kernel::Iso_cuboid_3 getFacetBoundingBox(Facet facet);
	Kernel::Iso_cuboid_3 getFacetBoundingBox(ovVector3D vertices[3]);
	void getFourBoxPlaneIntersections(Kernel::Iso_cuboid_3 cuboid, 
									  Plane_3 plane, 
									  std::vector<Point_3> &result, 
									  std::vector<ovInt> &coords);

	ovBool checkPastInsertions(std::vector<ovVector3D> &addedList, ovVector3D newPoint);
	ovFloat randFloat(ovFloat min, ovFloat max);

	void animateFluids();
	ObjectPropertyType getDistanceType(ovBool isBubble);

	void reassignSmoothLength(CPrtlFluid* fluid, SimuValue newSmoothLength);
	void copyPrtlValues(ParticleInstance &particleInstance, CFluidPrtl *fluidPrtl);

	void setVisibleParticles(CPrtlFluid*);
	
};

#endif 
