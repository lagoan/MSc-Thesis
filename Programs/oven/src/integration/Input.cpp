/*
 *  Input.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 21/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include <dom/domVertices.h>

#include "Input.h"
#include "system/LoggingSystem.h"
Input::Input()
{
	semantic = "";
	offset	 = 0;
	set		 = -1;
}

Input::Input(const Input& in)
{
	semantic = in.semantic;
	offset	 = in.offset;
	set		 = in.set;
	source	 = in.source;

}

std::vector<Input> Input::getInputs(domInputLocalOffset_Array inputArray)
{
	ovInt inputCount;
	std::vector<Input> _inputs;
	
	inputCount = inputArray.getCount();
	
	for (ovInt i = 0; i < inputCount; i++) {
		Input currentInput;
		
		currentInput.semantic = inputArray[i]->getSemantic();
		currentInput.offset   = inputArray[i]->getOffset();
		currentInput.source   = new daeURI(inputArray[i]->getSource());
		//currentInput.source   = inputArray[i]->getSource();
		currentInput.set	  = inputArray[i]->getSet();
			
		if (QString(inputArray[i]->getSemantic()) == QString("VERTEX")) {		// get the positions for the geometry
		

			daeURI _uri = inputArray[i]->getSource();							
			daeElement* _tempElement = _uri.getElement();
			domVertices* _vertices = (domVertices*)(domElement*)_tempElement;

			for (ovUInt j = 0; j < _vertices->getInput_array().getCount(); j++) {
				
				currentInput.semantic = _vertices->getInput_array()[j]->getSemantic();
				currentInput.source   = new daeURI(_vertices->getInput_array()[j]->getSource());
				//currentInput.source   = _vertices->getInput_array()[j]->getSource();
				_inputs.push_back( currentInput );
			}

		} else {
			_inputs.push_back( currentInput );
		}
	}
	
	return _inputs;
}