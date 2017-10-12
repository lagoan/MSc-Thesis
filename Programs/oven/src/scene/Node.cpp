/*
 *  Node.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <QGLWidget>

#include "system/types.h"
#include "system/LoggingSystem.h"
#include "system/ColorPalette.h"
#include "system/SimulationProperties.h"
#include "integration/integrationHelper.h"
#include "primitives/rayTriangleIntersect.h"

#include "Node.h"


#if 0
bool pointIsInside(ovTuple3f& newPos, Node& node)
{
	
	
	ExactPoint_3 exactPoint(newPos.x, newPos.y, newPos.z );
	
	Vertex_const_handle v;
	Halfedge_const_handle e;
	Halffacet_const_handle f;
	Volume_const_handle c;
	Object_handle o;
	
	for (ovUInt i=0; i < node.instanceGeometries.size(); i++) {
		for(ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			//		for(ovInt k=0; k < node.instanceGeometries[i].geometry->objects[j].
			
			o = node.instanceGeometries[i].geometry->objects[j].nefPolyhedron.locate(exactPoint);
			
			// if its on a vertex edge or facet 
			if(CGAL::assign(v, o) ||
			   CGAL::assign(e, o) ||
			   CGAL::assign(f, o) ) {
				return true;
				
			} else if(CGAL::assign(c,o)) {
				// if it is inside the volume
				if (c->mark()) {
					return true;
					
				}
				
			}
			
		}
		
	}
	return false;
}
#endif

Node::Node() 
{		
		parent = NULL;
		glName = 0;
}

Node::Node(domNode& nodeElement) 
{
	*this = Node();
	
	// do something with domNode_
	readNamesFromCollada(nodeElement);
	readTransformationsFromCollada(nodeElement);
						 
	for (size_t i = 0; i < nodeElement.getNode_array().getCount(); i++) {
		children.push_back(&lookup<Node, domNode>(*nodeElement.getNode_array()[i]));
	}

	for (size_t i = 0; i < nodeElement.getInstance_node_array().getCount(); i++) {
		domNode* child = daeSafeCast<domNode>( nodeElement.getInstance_node_array()[i]->getUrl().getElement() );
		children.push_back(&lookup<Node, domNode>(*child));
	}

	
	for (size_t i = 0; i < nodeElement.getInstance_geometry_array().getCount(); i++) {
		domInstance_geometry* instanceGeom = nodeElement.getInstance_geometry_array()[i];
		InstanceGeometry instance;
		instanceGeometries.push_back( instance ); 		
		domGeometry* geom = daeSafeCast<domGeometry>(instanceGeom->getUrl().getElement());
		instanceGeometries[ instanceGeometries.size() - 1 ].geometry = &lookup<Geometry, domGeometry>(*geom);
	}
	
	if ( nodeElement.getInstance_geometry_array().getCount() ) {
		nodeType = GeometricObjectNode;
	}
	else if ( nodeElement.getInstance_light_array().getCount() ) {
		nodeType = LampNode;
	}
	else if ( nodeElement.getInstance_camera_array().getCount() ) {
		nodeType = CameraNode;
	}
	else {
		nodeType = UnknownNode;
	}		

}

Node::~Node()
{

	for ( ovUInt i=0; i< instanceGeometries.size(); i++) {
		delete instanceGeometries[i].geometry; 
	}

	for ( ovUInt i=0; i< children.size(); i++) {
		delete children[i];
	}

}

Node::Node(const Node &node)
{
	name = node.name;
	id = node.id;
	sid = node.sid;
	workName = node.workName;
	glName = node.glName;
	nodeType = node.nodeType;
	transformationMatrix = node.transformationMatrix;
	
	Node* temp;
	for (ovUInt i=0; i < node.children.size(); i++) {

		temp = new Node;
		*temp = *(node.children[i]);
		children.push_back(temp);
	}
	
	instanceGeometries = node.instanceGeometries;


	//parent = new Node;
	//*parent = *(node.parent);
}

void Node::readNamesFromCollada(domNode& nodeElement) 
{
	if ( nodeElement.getSid() ) {
		sid	= nodeElement.getSid();
		workName = sid;
	}
	if ( nodeElement.getId() ) {	
		id	= nodeElement.getId();
		workName = id;
	}
	if ( nodeElement.getName() ) {	
		name = nodeElement.getName();
	}
	
}

void Node::readTransformationsFromCollada(domNode& nodeElement)
{
	ovChar* typeName;
	TransformationType transformationType;
	
	domTranslateRef translateArray;
	domRotateRef rotateArray;
	domScaleRef scaleArray;
	
	for ( ovUInt i = 0; i < nodeElement.getContents().getCount(); i++) {
		
		typeName = (ovChar *)nodeElement.getContents()[i]->getTypeName(); 
		
		transformationType = getTransformationType(typeName);
		
		switch (transformationType) {
				
			case ovTranslate:
				translateArray = (domTranslate * )(domElement * )nodeElement.getContents()[i];
				assert( translateArray->getValue().getCount() == 3 ); 
				
				
				// new translation
				transformationMatrix.translate(translateArray->getValue()[0],
													  translateArray->getValue()[1],
													  translateArray->getValue()[2]);
				break;
			case ovRotate:
				rotateArray = (domRotate * )(domElement * )nodeElement.getContents()[i];
				assert( rotateArray->getValue().getCount() == 4 );
				
	
				
				// new rotation , angle first
				transformationMatrix.rotate(rotateArray->getValue()[3],
												   rotateArray->getValue()[0],
											       rotateArray->getValue()[1],
												   rotateArray->getValue()[2]);
				
				break;
			case ovScale:
				scaleArray = (domScale * )(domElement * )nodeElement.getContents()[i];
				assert( scaleArray->getValue().getCount() == 3 ); 
				
				
				// new scaling
				transformationMatrix.scale(scaleArray->getValue()[0],
												  scaleArray->getValue()[1],
												  scaleArray->getValue()[2]);
				break;
			case ovLookAt:
			case ovSkew:
			case ovMatrix:
			case ovUnknown:
				break;
		}
		
	}	
	
}

TransformationType Node::getTransformationType(ovChar* nameType)
{
	
	if ( strcmp( nameType, "translate" ) == 0)
		return (ovTranslate);
	if ( strcmp( nameType, "rotate" ) == 0)
		return (ovRotate); 
	if ( strcmp( nameType, "scale" ) == 0)
		return (ovScale);
	if ( strcmp( nameType, "lookat" ) == 0)
		return (ovLookAt);
	if ( strcmp( nameType, "skew" ) == 0)
		return (ovSkew);
	if ( strcmp( nameType, "matrix" ) == 0)
		return (ovMatrix);
	
	return ovUnknown;	
	
}

void Node::render()
{
	
	ovInt _instanceGeomCount;
	ovInt _childrenCount;
	ObjectType objectType = Passive;

	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glPushName(glName);

	glMultMatrixd( transformationMatrix.getM() );
	
	
	// draw geometry
	if (nodeType == GeometricObjectNode) {

		objectType = PROPERTIES->getObjectProperties(glName)->getObjectType();
		glColor3dv( PROPERTIES->getObjectProperties(glName)->getColor().T );
		
	}
	
	if ( objectType != Fluid || !(PROPERTIES->processingAnimation)) {
			_instanceGeomCount = instanceGeometries.size();
			for (ovInt i=0; i < _instanceGeomCount; i++) {
				if (objectType != CutOut || SceneProperties::renderCutOuts == true) {
					instanceGeometries[i].render();
				}
			}

			/* http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=37 */
			
			if (PROPERTIES->selectedObject == glName) {
				glEnable(GL_CULL_FACE);
				glEnable (GL_BLEND);				
				glBlendFunc (GL_SRC_ALPHA ,GL_ONE_MINUS_SRC_ALPHA);
				glPolygonMode (GL_BACK, GL_LINE);				// Draw Backfacing Polygons As Wireframes
				glLineWidth (1.9f);								// Set The Line Width
				glCullFace (GL_FRONT);							// Don't Draw Any Front-Facing Polygons
				glDepthFunc (GL_LEQUAL);						// Change The Depth Mode
				glColor3dv(ColorPalette::outline.T);			// Set The Outline Color
			

				for (ovInt i=0; i < _instanceGeomCount; i++) {
					instanceGeometries[i].render();
				}
				glLineWidth (1.0);
				glDepthFunc (GL_LESS);							// Reset The Depth-Testing Mode
				glCullFace (GL_BACK);							// Reset The Face To Be Culled
				glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);		// Reset Back-Facing Polygon Drawing Mode
				glDisable (GL_BLEND);							// Disable Blending
				glDisable(GL_CULL_FACE);
				
			}
	} 

	// now the children
	
	
	_childrenCount = children.size();
	for (ovInt i=0; i < _childrenCount; i++) {
		children[i]->render();
	}
	
	glPopName();
	glPopMatrix();

}


/* applyTransformationToPos will apply the transformation matrix to the point 
 * stored on instance instanceIndex, on object objectIndex and finally, the
 * location of the position is given by positionIndex. This function does not
 * read well outside of context but it beats repeating the same lines of code
 */

ovVector3D Node::applyTransformationToPos(ovInt instanceIndex, ovInt objectIndex, ovInt positionIndex)
{
	ovVector3D currentPoint;
	
	currentPoint.set( instanceGeometries[instanceIndex].geometry->objects[objectIndex].sourcePosition[positionIndex].T );
	currentPoint = transformationMatrix * currentPoint;

	return currentPoint;

}

ovVector3D Node::applyTransformationToPosToVector(ovVector3D vector)
{
	return transformationMatrix * vector;
}


bool Node::pointIsInside(ovDouble x, ovDouble y, ovDouble z)
{
	ovTuple3f proxy;
	
	proxy.x = x;
	proxy.y = y;
	proxy.z = z;
	
	return pointIsInside(proxy);
	
}


bool Node::pointIsInside(ovTuple4f& newPos)
{
	ovTuple3f proxy;
	
	proxy.x = newPos.x;
	proxy.y = newPos.y;
	proxy.z = newPos.z;
	
	return pointIsInside(proxy);
	
}

bool Node::pointIsInside(ovTuple3f& newPos)
{
	ovTuple3f direction;
	ovVector3D v0, v1, v2;
	direction.x = 0;
	direction.y = 1;
	direction.z = 0;
	ovInt result;
	ovFloat t = 0, u, v;
	ovInt intersectionCount = 0;
	ovBool hitEdge;
	std::vector< std::pair<ovVector3D, ovVector3D> > visitedEdges;	
	std::vector< std::pair<ovVector3D, ovVector3D> >::iterator searchResult;
	std::pair<ovVector3D, ovVector3D> currentEdge;
	
	
	//*surfaceParticle = false;
	
	for (ovUInt i=0; i < instanceGeometries.size(); i++) {
		for(ovUInt j=0; j < instanceGeometries[i].geometry->objects.size(); j++) {
			for(ovInt k=0; k < instanceGeometries[i].geometry->objects[j].triangleCount; k++) {
				
				// v0, v1, v2 give us the triangle
				
				v0 = applyTransformationToPos(i, j, k*3);
				v1 = applyTransformationToPos(i, j, k*3 + 1);
				v2 = applyTransformationToPos(i, j, k*3 + 2);
				
				result = intersectsRayTriangle(newPos.T, direction.T,
											   v0.getT(), v1.getT(), v2.getT(), &t, &u, &v);
				
				
				hitEdge = false;
				
				if (result != 0 && t > 0) {
					
					/* this commented code could provide more particles inside
					 * a geometric volume. right now it has a problem that it
					 * identifies particles outside as inside, causing errors 
					 * in the resulting volume
					 */
					
					
#if 1
					if (u+v-1 == 0) {
						currentEdge = getEdge(v1, v2);
						hitEdge = true;
						
					}else if (u == 0) {
						currentEdge = getEdge(v0, v2);
						hitEdge = true;
						
						
					}
					else if (v == 0) {
						currentEdge = getEdge(v0, v1);
						hitEdge = true;
						
					}
					
					searchResult = find( visitedEdges.begin(), visitedEdges.end(), currentEdge );
					
					if (!hitEdge) {
						intersectionCount++;
						
					}
					else if ( searchResult == visitedEdges.end() ) {
						intersectionCount++;
						visitedEdges.push_back(currentEdge);
					}
#else 
					intersectionCount++;
#endif
					
					if (t < EPSILON) {
						//*surfaceParticle = true;
						return true;
					}
				}
			}
		}
	}
	
	if (intersectionCount == 0 || intersectionCount % 2 != 1) {
		return false;
	}
	
	return true;
}


std::pair<ovVector3D, ovVector3D> Node::getEdge(ovVector3D a, ovVector3D b)
{
	std::pair<ovVector3D, ovVector3D> edge;
	ovBool firstA = true;
	
	
	if (a.getX() > b.getX()) {
		firstA = false;
	}
	else if (a.getY() > b.getY()) {
		firstA = false;
	}
	else if (a.getZ() > b.getZ()) {
		firstA = false;
	}
	
	if (firstA) {
		edge.first = a;
		edge.second = b;
	}
	
	else {
		edge.first = b;
		edge.second = a;
	}
	
	return edge;
}



/*
void Node::applyTransfromationToNefPolyhedrons()
{
	
	
	std::cout << "transforming \n";
	Nef_polyhedron::Aff_transformation_3 trans(transformationMatrix(0, 0),
											   transformationMatrix(0, 1),
											   transformationMatrix(0, 2),
											   transformationMatrix(0, 3),
											   transformationMatrix(1, 0),
											   transformationMatrix(1, 1),
											   transformationMatrix(1, 2),
											   transformationMatrix(1, 3),
											   transformationMatrix(2, 0),
											   transformationMatrix(2, 1),
											   transformationMatrix(2, 2),
											   transformationMatrix(2, 3), 1);
	
	
	
	for ( ovUInt i=0; i< instanceGeometries.size(); i++) {
		for( ovUInt j=0; j< instanceGeometries[i].geometry->objects.size(); j++ ) {
						 
				instanceGeometries[i].geometry->objects[j].nefPolyhedron.transform( trans );
		}
	}
	
}
 */



