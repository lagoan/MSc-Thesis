/*
 *  integrationHelper.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 11/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef INTEGRATIONHELPER_H
#define INTEGRATIONHELPER_H

#include <iostream>

#include <dae/daeElement.h>
#include <dae.h>

// This function checks to see if a user data object has already been attached to
// the DOM object. If so, that object is casted from void* to the appropriate type
// and returned, otherwise the object is created and attached to the DOM object
// via the setUserData method.
template<typename MyType, typename DomType> MyType& lookup(DomType& domObject) {
	if (!domObject.getUserData()) {
		domObject.setUserData(new MyType(domObject));
	}
	return *(MyType*)(domObject.getUserData());
}


// This function traverses all the DOM objects of a particular type and frees
// destroys the associated user data object.
template<typename MyType, typename DomType> void freeConversionObjects(DAE& dae) {
	std::vector<daeElement*> elts = dae.getDatabase()->typeLookup(DomType::ID());
	for (size_t i = 0; i < elts.size(); i++) {
		delete (MyType*)elts[i]->getUserData();
	}
}


#endif