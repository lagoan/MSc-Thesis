/*
 *  Particle.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 01/02/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARTICLE_H
#define PARTICLE_H

#include <vector>
#include "CGALDefinitions.h"


class Particle {
	
public:
	
	Point_3 position;
	Point_3 normal;
	double temperature;
	double mass;
	double density;
	double originalDensity;
	double bubbleDensity;
	double size;
	double massDensityRatio;
	double smoothingLength;
	bool   visible;
	bool   trueSurface;
	bool   isBubble;
	std::vector<Particle *> neighborParticles;

	Particle();
	//Particle( Point_3 position_, double temperature_, double size_, double massDensityRatio_, bool visible_ );
	~Particle();
	
};




#endif