/*
 *  ColladaReconstructor.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 15/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef COLLADARECONSTRUCTOR_H
#define COLLADARECONSTRUCTOR_H

#include <QObject>
#include <QString>

#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domConstants.h>

#include "ProcessedScene.h"
#include "Task.h"


#define __MAX_TEMP__ 136.0
#define __MIN_TEMP__ 26.0


class ColladaReconstructor : public Task
{
	
	Q_OBJECT	
		
public:
	
	ColladaReconstructor(){};
	~ColladaReconstructor(){};
	
	void reconstruct( ProcessedScene &scene, DAE &daeObject, MeshDocument & meshDocument);
	void releaseMemory();
	
private:
	
	domAsset *asset;
	domAsset::domUp_axis    *upAxis;
	domLibrary_visual_scenes *libVisualScenes; 
	domVisual_scene *visualScene;
	domCOLLADA::domScene *currentScene;
	domInstanceWithExtra *instanceVisualScene;
	std::vector<domNode *> nodes;
	std::vector<domInstance_geometry *> instanceGeometries;
	domLibrary_geometries *libGeos;
	domLibrary_materials *libMaterials;
	domLibrary_images *libImages;
	domLibrary_effects *libEffects;
	
	void removeTransformations(domNode *node);
	
	void createPositionSource(domSource *positionSrc, ProcessedNode* processedNode, std::map<QString, int> & vertexIndexes );
	void createPositionAccessor(domSource *positionSrc,  ProcessedNode* processedNode);
	void createVertices(domSource *positionSrc,  ProcessedNode* processedNode, domMesh *mesh, std::map<QString, int> &vertexIndexes, QString materialEasy);
	void createNormalSource(domSource *normalSrc, ProcessedNode* processedNode);
	void createNormalAccessor(domSource *normalSrc, ProcessedNode* processedNode);
	void createColorSource(domSource *colorSrc, ProcessedNode* processedNode);
	void createColorAccessor(domSource *colorSrc, ProcessedNode* processedNode);
	void createMapSource(domSource *mapSrc, ProcessedNode* processedNode);
	void createMapAccessor(domSource *mapSrc, ProcessedNode* processedNode);

	void createTextureEffect(domCOLLADA *);
	
    void addBubbleToCollada( DAE &daeObject, ProcessedScene &scene);
	void addBubbleInstancesToCollada(DAE &daeObject, ProcessedScene &scene);
	
	void fillTextureSampler(domCommon_newparam_type *);
	void fillTextureSurface(domCommon_newparam_type *);
	void fillTextureShading(domProfile_COMMON::domTechnique::domBlinn *);
	
	QString getPointKey(CVertexO &vertex);
	
	Vector_3 computeSmoothNormal(Vertex &vertex);
	Vector_3 ColladaReconstructor::getFacetsNormal(Facet &facet);
	
};


#endif
