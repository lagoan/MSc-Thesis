/*
 *  Node.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef	NODE_H
#define	NODE_H

#include <QString>
#include <vector>
#include <dom/domNode.h>
#include <dom/domInstance_geometry.h>
#include <dom/domGeometry.h>

#include "system/types.h"
#include "InstanceGeometry.h"
//#include "Transformation.h"
#include "primitives/TransformationMatrix.h"


enum NodeType {
	SceneNode,
	GeometricObjectNode,
	CameraNode,
	LampNode,
	UnknownNode
};


class Node 
{

public:

	QString					name;
	QString					id;
	QString					sid;
	QString					workName;
	ovInt					glName;
	Node*					parent;
	std::vector<Node*>		children;
	NodeType				nodeType;
	TransformationMatrix	transformationMatrix;

	std::vector<InstanceGeometry> instanceGeometries;
	// TODO: add more instances (camera, controller, light, node)
	// std::vector<InstanceCamera>
	// std::vector<InstanceController>
	// std::vector<InstanceLight>
	// std::vector<InstanceNode>
	
	Node();
	~Node();
	Node(domNode&);
	Node(const Node &node);
     

	void render();
	ovVector3D applyTransformationToPos(ovInt instanceIndex, ovInt objectIndex, ovInt positionIndex);
	ovVector3D applyTransformationToPosToVector(ovVector3D vector);
	
 	
	bool pointIsInside(ovTuple4f& newPos);
	bool pointIsInside(ovTuple3f& newPos);
	bool pointIsInside(ovDouble x, ovDouble y, ovDouble z);
	
private:
	void readNamesFromCollada(domNode&);
	void readTransformationsFromCollada(domNode&);
	TransformationType getTransformationType(ovChar* nameType);
	std::pair<ovVector3D, ovVector3D> getEdge(ovVector3D a, ovVector3D b);

	
};



#endif