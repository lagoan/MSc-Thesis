
/*
 *  ParticleInstance.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 23/08/08.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PARTICLEINSTANCE_H
#define PARTICLEINSTANCE_H

#include "system/types.h"

class ParticleInstance {

public:


	ovDouble onSurface;
	ovDouble inCollision;
	ovDouble trueSurface;
	ovDouble visible;
	ovDouble density;
	ovDouble originalDensity;
	ovDouble mass;
	ovDouble densityAdded;
	ovDouble trueSurfaceTime;
	
	ovTuple3f pos;
	ovTuple3f normal;
	ovTuple3f gradient;
	ovTuple3f veloctiy;

	ovDouble temp;
	ovDouble co2;
	ovDouble size;

	ovDouble massDensityRatio;
	
	ovDouble neighbourCount;
	ovDouble shearModulus;
	ovDouble decayRate;
	ovDouble pressure;
	ovDouble bubblePressure;
	ovDouble velocityTuner;
	ovDouble smoothingLength;

	
	ParticleInstance operator= (const ParticleInstance &p);
	
};

inline ParticleInstance ParticleInstance::operator= (const ParticleInstance &p)
{
	pos  = p.pos;
	normal = p.normal;
	gradient = p.gradient;
	veloctiy = p.veloctiy;
	temp = p.temp;
	co2  = p.co2;
	size = p.size;
	density = p.density;
	originalDensity = p.originalDensity;
	trueSurfaceTime = p.trueSurfaceTime;
	onSurface	= p.onSurface;
	inCollision = p.inCollision;
	trueSurface = p.trueSurface;
	massDensityRatio = p.massDensityRatio;
	visible = p.visible;
	
	neighbourCount = p.neighbourCount;
	shearModulus = p.shearModulus;
	decayRate = p.decayRate;
	pressure = p.pressure;
	bubblePressure = p.bubblePressure;
	velocityTuner = p.velocityTuner;
	smoothingLength = p.smoothingLength;
	
	mass = p.mass;
	densityAdded - p.densityAdded;
	
	return *this;

}

#endif
