/*
 *  Scene.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "Scene.h"
#include "InstanceGeometry.h"
#include "primitives/Geometry.h"
#include "system/LoggingSystem.h"
#include "integration/integrationHelper.h"

void Directory::fillDirectory(Node* node)
{
	
	ovUInt _count;
	_count = node->children.size();
	
	pushNode(node);

	for (ovUInt i = 0; i < _count; i++) {
		fillDirectory( node->children[i] );

	}
}

Node* Directory::getNodeByName(ovInt glName) 
{
	for (ovUInt i=0; i<_nodes.size(); i++) {
		if (_nodes[i]->glName == glName) {
			return _nodes[i];
		}
	}

	return NULL;

}


Scene::Scene(){
	sceneRoot = NULL;
}

Scene::~Scene(){
	if (sceneRoot != NULL) {
		delete sceneRoot;
	}
}

Scene::Scene(const Scene& scene)
{
	name = scene.name;
	
	sceneRoot = getNewTree(scene.sceneRoot);

	directory.reset();
	directory.fillDirectory(sceneRoot);

		
}

Scene::Scene(domVisual_scene& visual_scene)
{
	*this = Scene();
	
	// do something with visual_scene_ 
	
	if ( visual_scene.getName() ) {
		name = visual_scene.getName(); 
	}
	else {
		name = "Scene";
	}
	
	sceneRoot = new Node();
	sceneRoot->nodeType = SceneNode;
	
	sceneRoot->workName = name;
	
	domNode_Array& nodes = visual_scene.getNode_array();
	
	
	for ( ovUInt i = 0; i < nodes.getCount(); i++) {
					
		sceneRoot->children.push_back(& lookup<Node, domNode>(*nodes[i]) );
		sceneRoot->children[sceneRoot->children.size() - 1]->parent = sceneRoot;
		
	}
	
	if (sceneRoot != NULL) {
		fillDirectory( sceneRoot );
	}
}

void Scene::fillDirectory(Node* node)
{
	
	ovUInt _count;
	_count = node->children.size();
	
	node->glName = directory.getNextName();
	directory.pushNode(node);
	
	for (ovUInt i = 0; i < _count; i++) {
		fillDirectory( node->children[i] );
		
	}
}

Node* Scene::getNewTree(Node* node)
{
	Node* newNode = new Node;
	if (node != NULL) {
		newNode->name					= node->name;
		newNode->id						= node->id;
		newNode->sid					= node->sid;
		newNode->workName				= node->workName;
		newNode->glName					= node->glName;
		newNode->transformationMatrix	= node->transformationMatrix; 
		newNode->nodeType				= node->nodeType;
		
		ovInt size;

		size = node->instanceGeometries.size();
		for (ovInt i = 0; i < size; i++) {
			
			InstanceGeometry instanceGeometry;
			instanceGeometry.parent = newNode;
			
			instanceGeometry.geometry = new Geometry;
			*(instanceGeometry.geometry) = *(node->instanceGeometries[i].geometry);
			
			newNode->instanceGeometries.push_back( instanceGeometry );
		}

		size = node->children.size();
		for (ovInt i=0; i < size; i++) {
			newNode->children.push_back( getNewTree(node->children[i]) );
		}
	}
	return newNode;

}

void Scene::render()
{
	if (sceneRoot != NULL){
		sceneRoot->render();
	}

}

