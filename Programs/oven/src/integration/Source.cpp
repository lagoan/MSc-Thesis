/*
 *  Source.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 21/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <dom/domSource.h>
#include "system/LoggingSystem.h"
#include "Source.h"

std::vector<Source> Source::getSources(std::vector<Input>& inputs)				
{
	std::vector<Source> _sources;
	ovInt _inputCount;
	
	_inputCount = inputs.size();

	for (ovInt i = 0; i < _inputCount; i++) {
		ovInt j, _count;

		inputs[i].sourceIndex = i;												// set id for internal use 
		
		Source _workingSource;
		daeURI _uri = *(inputs[i].source);
		daeElement* _tempElement = _uri.getElement();
		domSource* _currentDOMSource = (domSource*)(domElement*)_tempElement;

		
		if ( _currentDOMSource->getFloat_array() != NULL ) {					// get the float array

			_count = _currentDOMSource->getFloat_array()->getCount();

			for (j = 0; j < _count; j++) {
				_workingSource.floatArray.push_back( 
					_currentDOMSource->getFloat_array()->getValue()[j] 
				);
			}	
		}

		
		if ( _currentDOMSource->getInt_array() != NULL ) {						// get the int array

			_count = _currentDOMSource->getInt_array()->getCount();

			for (j = 0; j < _count; j++) {
				_workingSource.intArray.push_back( 
					_currentDOMSource->getInt_array()->getValue()[j] 
				);
			}
		}


		if ( _currentDOMSource->getBool_array() != NULL ) {						// get the bool array

			_count = _currentDOMSource->getBool_array()->getCount();

			for (j = 0; j < _count; j++) {
				_workingSource.boolArray.push_back( 
					_currentDOMSource->getBool_array()->getValue()[j] 
				);
			}
		}

		
		if ( _currentDOMSource->getName_array() != NULL ) {						// get the name array

			_count = _currentDOMSource->getName_array()->getCount();

			for (j = 0; j < _count; j++) {
				_workingSource.nameArray.push_back( 
					_currentDOMSource->getName_array()->getValue()[j] 
				);
			}
		}

		_sources.push_back( _workingSource );
	}

	return _sources;
}

void Source::setPolygonSources(Polygons* _polygons, std::vector<Source>& _sources, std::vector<Input>& _inputs)
{


	for (ovUInt i = 0; i < _inputs.size(); i++) {
		if (_inputs[i].semantic == QString("POSITION")) {
			for (ovUInt j = 0 ; j < _sources[i].floatArray.size(); j += 3) {		// 3 is the stride
				ovTuple3f _position;
				_position.x = _sources[ _inputs[i].sourceIndex ].floatArray[ j ];
				_position.y = _sources[ _inputs[i].sourceIndex ].floatArray[ j + 1 ];
				_position.z = _sources[ _inputs[i].sourceIndex ].floatArray[ j + 2 ];
		
				_polygons->sourcePositionPushBack( _position );

			}

		}
		else if (_inputs[i].semantic == QString("NORMAL")) {
			for (ovUInt j = 0 ; j < _sources[i].floatArray.size(); j += 3) {		// 3 is the stride
				ovTuple3f _normal;
				_normal.x = _sources[ _inputs[i].sourceIndex ].floatArray[ j ];
				_normal.y = _sources[ _inputs[i].sourceIndex ].floatArray[ j + 1 ];
				_normal.z = _sources[ _inputs[i].sourceIndex ].floatArray[ j + 2 ];

				_polygons->sourceNormalPushBack( _normal );
			}

		}
		else if (_inputs[i].semantic == "TEXCOORD") {
		// TODO add texcoords

		}
		else if (_inputs[i].semantic == "COLOR") {
		// TODO add color
		}	
	}
	
}