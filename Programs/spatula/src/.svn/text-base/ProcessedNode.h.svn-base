/*
 *  ProcessedNode.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 12/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef PROCESSEDNODE_H
#define PROCESSEDNODE_H

#include <QString>
#include <list>
#include <vector>

#include <dom/domNode.h>
#include <dom/domInstance_geometry.h>
#include <dom/domGeometry.h>

#include "CGALDefinitions.h"
#include "Particle.h"
#include "meshlab/meshmodel.h"

class CutOutInformation {
public:
	std::vector< std::vector< uint > > facets;
	std::vector< Point_3 > vertex;
	
	CutOutInformation & operator=(const CutOutInformation &cutout);
	
	
};

inline CutOutInformation & CutOutInformation::operator=(const CutOutInformation &cutout)
{
	facets = cutout.facets;
	vertex = cutout.vertex;	
	return *this;  // Return a reference to myself.
}


class ProcessedNode 
{
	
public:
	
	enum NodeType {
		SceneNode,
		GeometricObjectNode,
		CameraNode,
		LampNode,
		UnknownNode
	};
	
	enum TransformationType
	{
		ovUnknown,
		ovTranslate,
		ovRotate,
		ovScale,
		ovLookAt,
		ovSkew,
		ovMatrix
	};
	
	QString							name;
	QString							id;
	QString							sid;
	QString							workName;

	std::vector<ProcessedNode>		children;
	NodeType						nodeType;
	Aff_transformation_3			affTransformation3;
	
	//std::vector<InstanceGeometry> instanceGeometries;
	
	std::vector<Particle> particles;
	std::vector<Particle> bubbles;
	
	 
	std::vector< MeshModel * > newSurfaces;
	
	
	ProcessedNode();
	ProcessedNode(domNode&);
	
	~ProcessedNode();
	
	void clear();
	static Aff_transformation_3 readTransformationsFromCollada(domNode& nodeElement);
	static double** readSimpleTransformationsFromCollada(domNode& nodeElement);
private:
	
	void readNamesFromCollada(domNode &nodeElement);
	
	
	void processTechniqueArray(domTechnique_Array & techs);
	void processElementsArray(daeElementRefArray &techContents);	
	
	// get the values and store them in the arrays
	
	void processParticlePosition(domFloat_array* data);
	void processParticleTemperature(domFloat_array* data);
	void processParticleMass(domFloat_array* data);
	void processParticleDensity(domFloat_array* data);
	void processParticleOriginalDensity(domFloat_array* data);
	void processParticleMassDensityRatio(domFloat_array* data);
	void processParticleVisible(domFloat_array* data);
	void processParticleTrueSurface(domFloat_array* data);
	void processParticleNormal(domFloat_array* data);
	void processParticleSmoothingLength(domFloat_array* data);
	
	void processBubblePosition(domFloat_array* data);
	void processBubbleTemperature(domFloat_array* data);
	void processBubbleMass(domFloat_array* data);
	void processBubbleDensity(domFloat_array* data);
	void processBubbleOriginalDensity(domFloat_array* data);
	void processBubbleBubbleDensity(domFloat_array* data);
	void processBubbleSize(domFloat_array* data);
	void processBubbleMassDensityRatio(domFloat_array* data);
	void processBubbleVisible(domFloat_array* data);
	void processBubbleTrueSurface(domFloat_array* data);
	void processBubbleNormal(domFloat_array* data);
	void processBubbleSmoothingLength(domFloat_array* data);
	// get affine transformation matrixes
	
	static Aff_transformation_3 getScalingTransformation(domScaleRef scaleArray);
	static Aff_transformation_3 getTranslationTransformation(domTranslateRef translateArray);
	static Aff_transformation_3 getRotationTransformation(domRotateRef rotateArray);	
	static TransformationType getTransformationType(char *nameType);

};


#endif