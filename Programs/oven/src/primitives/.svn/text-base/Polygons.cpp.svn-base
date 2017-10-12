/*
 *  Polygons.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 22/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <QGLWidget>

#include "Polygons.h"
#include "system/types.h"
#include "system/LoggingSystem.h"
#include "integration/Input.h"
#include "integration/Source.h"




Polygons::Polygons()
{


	triangleCount       = 0;
	sourcePositionCount = 0;
	sourceNormalCount   = 0;
	indexCount          = 0;


	sourcePosition = NULL;
	sourceNormal   = NULL;
	//indexPosition  = NULL;
	//indexNormal    = NULL;
}

Polygons::~Polygons()
{

	//if (indexPosition) 
	//	delete indexPosition;

	//if (indexNormal)
	//	delete indexNormal;

	if (sourcePosition != NULL){
		delete sourcePosition;
	}

	if (sourceNormal != NULL) {
		delete sourceNormal;
	}

}

Polygons::Polygons(domTriangles& trianglesElement)
{
	// get inputs
	std::vector<Input> _inputs = Input::getInputs( trianglesElement.getInput_array() );
	ovUInt _inputCount = _inputs.size();
	ovUInt _largestStride = 0;
	BuildPolyhedron<HalfedgeDS> newPolyhedron;
	
	for (ovUInt i=0; i < _inputCount; i++) {
		if ( _inputs[i].offset > _largestStride ) {
			_largestStride = _inputs[i].offset;
		}
	}
	_largestStride++;
	ovInt _polygonCount = trianglesElement.getCount();
	
	ovInt _pIndex = 0;
	for (ovInt i = 0; i < _polygonCount; i++) {
		Polygons::Face _face;
		for (ovInt j = 0 ; j < 3; j++, _pIndex += _largestStride) {
			ovInt _tempIndex;
			for (ovUInt k = 0; k < _inputCount; k++) {
				
				if ( _inputs[k].semantic == "POSITION" ) {						// POSITION in collada
					
					// get the <p> value taking into account the offset
					_tempIndex = trianglesElement.getP()->getValue()[ _pIndex + _inputs[k].offset ];
					_face.indexPosition.push_back( _tempIndex );
					
				} 
				else if ( _inputs[k].semantic == "NORMAL" ) {
					
					// get the <p> value taking into account the offset
					_tempIndex = trianglesElement.getP()->getValue()[ _pIndex + _inputs[k].offset ];
					_face.indexNormal.push_back( _tempIndex );
					
				}
				else if ( _inputs[k].semantic == "TEXCOORD" ) {
					// TODO: add texcoord
					
				}
				else if ( _inputs[k].semantic == "COLOR" ) {
					// TODO: add color
					
				}
			}
			
		}
		newPolyhedron.addFacet(_face.indexPosition);
		facesPushBack( _face );
	}
	
	
	// get sources
	std::vector<Source> _sources = Source::getSources(_inputs);
	Source::setPolygonSources(this, _sources, _inputs);
	
	newPolyhedron.setVertices( getSourcePositionWorkingVector() );
	polyhedron.delegate(newPolyhedron);
	
	// get nef 
 	/*
	BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedron;
	
	ExactPolyhedron exactPoly;
	
	newExactPolyhedron.addPolyhedron(&polyhedron);
	exactPoly.delegate(newExactPolyhedron);
	
	nefPolyhedron = Nef_polyhedron(exactPoly);
 	*/
	
	triangulate();
	/*
	if ( polyhedron.is_closed() ) {
		std::cout << "closed triangles\n";
		try {
			Nef_polyhedron nef(polyhedron);
		}
		catch (...) {
			std::cout << "an error occured triangles\n";
		}
		//nefPolyhedron = polyhedron;
	}
	else {
		std::cout << "not closed triangles\n";
		
	}
	*/
	
}

Polygons::Polygons(domPolygons& polygonsElement)
{
	
	// get inputs
	std::vector<Input> _inputs = Input::getInputs( polygonsElement.getInput_array() );
	ovUInt _inputCount = _inputs.size();	
	ovUInt _largestStride = 0;
	BuildPolyhedron<HalfedgeDS> newPolyhedron;
	
	for (ovUInt i=0; i < _inputCount; i++) {
		if ( _inputs[i].offset > _largestStride ) {
			_largestStride = _inputs[i].offset;
		}
	}
	_largestStride++;
	// get information for faces
	// polygons treated as coplanar
	for (ovUInt i=0; i < polygonsElement.getP_array().getCount(); i++) {
		Polygons::Face _face;
		// for each vertex add to face
		for (ovUInt j=0; j < polygonsElement.getP_array()[i]->getValue().getCount(); j += _largestStride ) {
			// add to face
			for (ovUInt k = 0; k < _inputCount; k++) {
				ovInt _tempIndex;
				
				if ( _inputs[k].semantic == "POSITION" ) {						// POSITION in collada
					
					// get the <p> value taking into account the offset
					_tempIndex = polygonsElement.getP_array()[i]->getValue()[ j + _inputs[k].offset ];
					_face.indexPosition.push_back( _tempIndex );
					
				} 
				else if ( _inputs[k].semantic == "NORMAL" ) {
					
					// get the <p> value taking into account the offset
					_tempIndex = polygonsElement.getP_array()[i]->getValue()[ j + _inputs[k].offset ];
					_face.indexNormal.push_back( _tempIndex );
					
					
				}
				else if ( _inputs[k].semantic == "TEXCOORD" ) {
					// TODO: add texcoord
					
				}
				else if ( _inputs[k].semantic == "COLOR" ) {
					// TODO: add color
					
				}
			}
		}
		//add to _polygons
		newPolyhedron.addFacet(_face.indexPosition);
		facesPushBack( _face );
	}
	
	// get sources
	std::vector<Source> _sources = Source::getSources(_inputs);
	Source::setPolygonSources(this, _sources, _inputs);
	
	newPolyhedron.setVertices( getSourcePositionWorkingVector() );
	
	polyhedron.delegate(newPolyhedron);
	
	// get nef 
 	
 	/*
	 BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedron;
	 
	 ExactPolyhedron exactPoly;
	 
	 newExactPolyhedron.addPolyhedron(&polyhedron);
	 exactPoly.delegate(newExactPolyhedron);
	 
	 nefPolyhedron = Nef_polyhedron(exactPoly);
	 */
	
	/*	
	
	std::cout << "OFF" << std::endl << polyhedron.size_of_vertices() << ' '
	<< polyhedron.size_of_facets() <<' ' <<polyhedron.size_of_halfedges() << std::endl;
	triangulate();
	
	if ( polyhedron.is_closed() ) {
		std::cout << "closed polygons\n";
		try {
			Nef_polyhedron nef(polyhedron);
		}
		catch (...) {
			std::cout << "an error occured intpolygons\n";
		}
		//nefPolyhedron = polyhedron;
	}
	else {
		std::cout << "not closed polygons\n";
		
	}
	*/
}


Polygons::Polygons(domPolylist& polylistElement)
{
	// get inputs
	std::vector<Input> _inputs = Input::getInputs( polylistElement.getInput_array() );
	ovUInt _inputCount = _inputs.size();
	ovUInt _largestStride = 0;
	BuildPolyhedron<HalfedgeDS> newPolyhedron;
	
	for (ovUInt i=0; i < _inputCount; i++) {
		if ( _inputs[i].offset > _largestStride ) {
			_largestStride = _inputs[i].offset;
		}
	}
	_largestStride++;
	ovInt _polygonCount = polylistElement.getCount();
	
	ovInt _pIndex = 0;
	for (ovInt i = 0; i < _polygonCount; i++) {
		Polygons::Face _face;
		for (ovInt j = 0 ; j < polylistElement.getVcount()->getValue()[i]; j++, _pIndex += _largestStride) {
			ovInt _tempIndex;
			for (ovUInt k = 0; k < _inputCount; k++) {
				
				if ( _inputs[k].semantic == "POSITION" ) {						// POSITION in collada
					
					// get the <p> value taking into account the offset
					_tempIndex = polylistElement.getP()->getValue()[ _pIndex + _inputs[k].offset ];
					_face.indexPosition.push_back( _tempIndex );
					
				} 
				else if ( _inputs[k].semantic == "NORMAL" ) {
					
					// get the <p> value taking into account the offset
					_tempIndex = polylistElement.getP()->getValue()[ _pIndex + _inputs[k].offset ];
					_face.indexNormal.push_back( _tempIndex );
					
				}
				else if ( _inputs[k].semantic == "TEXCOORD" ) {
					// TODO: add texcoord
					
				}
				else if ( _inputs[k].semantic == "COLOR" ) {
					// TODO: add color
					
				}
			}
			
		}
		newPolyhedron.addFacet(_face.indexPosition);
		facesPushBack( _face );
	}
	
	
	// get sources
	std::vector<Source> _sources = Source::getSources(_inputs);
	Source::setPolygonSources(this, _sources, _inputs);
	
	newPolyhedron.setVertices( getSourcePositionWorkingVector() );
	
	polyhedron.delegate(newPolyhedron);
	//Nef_polyhedron polyhedron);
	
	
	// get nef 
 	
 	/*
	 BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedron;
	 
	 ExactPolyhedron exactPoly;
	 
	 newExactPolyhedron.addPolyhedron(&polyhedron);
	 exactPoly.delegate(newExactPolyhedron);
	 
	 nefPolyhedron = Nef_polyhedron(exactPoly);
	 */
	
	
	/*
	std::cout << "OFF" << std::endl << polyhedron.size_of_vertices() << ' '
	<< polyhedron.size_of_facets() <<' ' << polyhedron.size_of_halfedges() << std::endl;
	
	triangulate();
	
	if ( polyhedron.is_closed() ) {
		std::cout << "closed polylist\n";
		try {
			Nef_polyhedron nef(polyhedron);
			//nefPolyhedron = polyhedron;
		}
		catch (...) {
			std::cout << "an error occured polylist\n";
		}
	}
	else {
		std::cout << "not closed polylist\n";
		
	}
	*/
	
}


Polygons::Polygons(const Polygons& polygons)
{


    // public variables
	triangleCount       = polygons.triangleCount;
	sourcePositionCount = polygons.sourcePositionCount;
	sourceNormalCount   = polygons.sourceNormalCount;
	indexCount          = polygons.indexCount;
	polyhedron			= polygons.polyhedron;
	//nefPolyhedron		= polygons.nefPolyhedron;

//	indexPosition   = new GLuint[indexCount];
//	indexNormal     = new GLuint[indexCount];

	sourcePosition  = new ovTuple3f[sourcePositionCount];
	sourceNormal	= new ovTuple3f[sourceNormalCount];

//	for (ovInt i = 0; i < indexCount; i++) {
//		indexPosition[i] = (GLuint)polygons.indexPosition[i];
//		indexNormal[i]   = (GLuint)polygons.indexNormal[i];
//	}

	
	for (ovInt i = 0; i < sourcePositionCount; i++) {
		sourcePosition[i] = polygons.sourcePosition[i];
	}	
	for (ovInt i = 0; i < sourceNormalCount; i++) {
		sourceNormal[i]   = polygons.sourceNormal[i];
	}	
	

	// private variables (should be empty after triangulation)
	/*
	_faces			= polygons._faces;

	_sourcePosition = polygons._sourcePosition;
	_sourceNormal	= polygons._sourceNormal;
	_sourceColor	= polygons._sourceColor;
	_sourceTexCoord = polygons._sourceTexCoord;
	*/
}


void Polygons::render()
{
	//LOGS->logEvent(QString("index count %1").arg(indexCount));
	//LOGS->logEvent(QString("position count %1").arg(sourcePositionCount));


	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	
	glVertexPointer(3, GL_DOUBLE, sizeof(ovTuple3f), &sourcePosition[0]);
	glNormalPointer(GL_DOUBLE, sizeof(ovTuple3f), &sourceNormal[0]);

	//glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, indexPosition);
	glDrawArrays(GL_TRIANGLES, 0, indexCount);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);


	
	
	
	
#if 0
	ovInt _faceCount = faces.size();
	ovInt _vertexCount;
	ovTuple3f _position;
	ovTuple3f _normal;
	ovTuple3f _color;	

	for (ovInt i = 0; i < _faceCount; i++) {
		_vertexCount = faces[i].indexPosition.size();
		
		glBegin(GL_POLYGON);
		
		//for (ovInt j = _vertexCount-1 ; j >=0; j--) {
		for (ovInt j = 0; j < _vertexCount; j++) {
			
			if ( faces[i].indexPosition[j] < sourcePosition.size() )
				_position = sourcePosition[ faces[i].indexPosition[j] ];

			
			if (sourceNormal.size() != 0) {
				if (faces[i].indexNormal[j] < sourceNormal.size()){
					_normal = sourceNormal[ faces[i].indexNormal[j] ];
					glNormal3dv( (const GLdouble *) _normal.T );
				}
				
			}
			
			if (sourceColor.size() != 0) {
				_color = sourceColor[ faces[i].indexColor[j] ];
				glColor3fv( (const GLfloat *) _color.T );
			} 
			// TODO add texcoords

			glVertex3dv( (const GLdouble *) _position.T );
			
		}
		glEnd();
	}
#endif
}

void Polygons::facesPushBack(Face face)
{
	_faces.push_back(face);	
}

void Polygons::sourcePositionPushBack(ovTuple3f pos)
{
	_sourcePosition.push_back(pos);	
}

void Polygons::sourceNormalPushBack(ovTuple3f norm)
{
	_sourceNormal.push_back(norm);	
}



void Polygons::triangulate()
{

	// naive triangulation assuming polygons dont have holes and are convex


	int i, j;
	ovInt _initialPoint;
	ovInt _initialNormal;
	ovInt _indexCount, _newTriangleCount = 0;

	std::vector<int> _tempIndexPosition;
	std::vector<int> _tempIndexNormal;

	for (i = 0; i < _faces.size(); i++) {
		
		_indexCount = _faces[i].indexPosition.size();
		_newTriangleCount = _indexCount - 2;

		_initialPoint = _faces[i].indexPosition[0];
		_initialNormal = _faces[i].indexNormal[0];

		for (j=1; j <= _newTriangleCount; j++) {
			
			// add a
			_tempIndexPosition.push_back( _initialPoint );
			_tempIndexNormal.push_back( _initialNormal );

			// add b
			_tempIndexPosition.push_back( _faces[i].indexPosition[ j ] );
			_tempIndexNormal.push_back(   _faces[i].indexNormal[ j ] );

			// add c
			_tempIndexPosition.push_back( _faces[i].indexPosition[ j + 1 ] );
			_tempIndexNormal.push_back(   _faces[i].indexNormal[ j + 1 ] );

		}
	}
	
	

	triangleCount		= _tempIndexPosition.size() / 3;
	indexCount			= _tempIndexPosition.size();
	sourcePositionCount = indexCount;
	sourceNormalCount	= indexCount;
	
	

	
	sourcePosition  = new ovTuple3f[sourcePositionCount];
	sourceNormal	= new ovTuple3f[sourceNormalCount];
	


	for (i = 0; i < indexCount; i++) {
		sourcePosition[i] = _sourcePosition[ _tempIndexPosition[i] ];
		sourceNormal[i] = _sourceNormal[ _tempIndexNormal[i] ];
	}

	

	_faces.clear();
	_sourcePosition.clear();
	_sourceNormal.clear();

	


}

