
#ifndef PROCESSEDSCENE_H
#define PROCESSEDSCENE_H

#include <list>
#include <QString>
#include <dom/domVisual_scene.h>

#include "ProcessedNode.h"
#include "CGALDefinitions.h"
#include "CMesh.h"
//#include <vcg/space/index/grid_static_ptr.h>
//#include <vcg/space/index/grid_static_obj.h>

//typedef vcg::GridStaticPtr< CMeshO::FaceType, CMeshO::ScalarType> StaticPtrGrid;

class ProcessedScene {
	
public:
	
	QString sceneName;
	QString fileName;
	ProcessedNode sceneRoot;
	
	 
	
	ProcessedScene();
	ProcessedScene(domVisual_scene& visual_scene);
	~ProcessedScene();

	ProcessedScene & operator=(const ProcessedScene &scene);

	
	void clearNodes();
	std::list<ProcessedNode*>   getNodes();
	
	std::vector< CutOutInformation > cutOuts;
	std::vector< CMesh * > voids;
	std::vector< StaticPtrGrid > grids;

private:
	
	std::list<ProcessedNode*> getNodes(ProcessedNode &node);
	void getCutouts(domVisual_scene& visual_scene);
	
	
	
};


inline ProcessedScene & ProcessedScene::operator=(const ProcessedScene &scene)
{
	sceneName = scene.sceneName;
	
	fileName = scene.fileName;
	sceneRoot = scene.sceneRoot;
	
	sceneRoot.clear();
	sceneRoot = scene.sceneRoot;
	
	cutOuts.clear();
	cutOuts = scene.cutOuts;
	voids = scene.voids;
	grids = scene.grids;	
	return *this;  // Return a reference to myself.
}


#endif