/*
 *  Scene.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SCENE_H
#define	SCENE_H

#include <QString>
#include <dom/domVisual_scene.h>

#include "system/types.h"

#include "Node.h"

class Directory
{
public:
	Directory(){ reset(); };
	void reset(){ nextName = 0; _nodes.clear(); };
	void pushNode(Node* n){ _nodes.push_back( n ); };
	Node* getNode(int i){ return _nodes[i]; };
	Node* getNodeByName(ovInt glName);
	ovInt getNextName(){ return nextName++; };
	ovInt getSize(){ return _nodes.size(); };
	void fillDirectory(Node* sceneRoot);
	const Node& operator[](ovUInt i);

	

private:
	ovInt nextName;
	std::vector<Node*> _nodes;

};


inline const Node& Directory::operator[] (ovUInt i)
{
	return *(_nodes[i]);
}


class Scene
{
public:
	Scene();
	~Scene();
	Scene(domVisual_scene&);
	Scene(const Scene& scene);
	QString name;
	Node* sceneRoot;
	Directory directory;


	
	
	void render();
	ovInt getNodeCount(){ return directory.getSize(); };
	
	Scene& operator=(const Scene& scene);


private:

	Node* getNewTree(Node* node);	// for use in our deep copy constructor
	void fillDirectory(Node* node);


};

inline Scene& Scene::operator=(const Scene& scene) {
	name = scene.name;
	
	sceneRoot = getNewTree(scene.sceneRoot);

	directory.reset();
	directory.fillDirectory(sceneRoot);

	return *this;
}

#endif