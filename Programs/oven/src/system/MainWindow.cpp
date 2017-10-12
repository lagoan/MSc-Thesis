/*
 *  MainWindow.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 02/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "MainWindow.h"
#include "LoggingSystem.h"
#include "SimulationProperties.h"


#include <cstdlib>
#include <QtGui> 
#include <QFile>
#include <QPixmap>

#include <dom/domRotate.h>
#include <dom/domScale.h>
#include <dom/domSkew.h>
#include <dom/domTranslate.h>

#include <iostream>
#include <cfloat>
#include <limits>

Scene LoadThread::convertModel(domCOLLADA& root)
{
	Scene scene;
	domVisual_scene* _domScene;
	
	_domScene = daeSafeCast<domVisual_scene>(root.getDescendant("visual_scene"));
	
	
	scene = lookup<Scene, domVisual_scene>(*_domScene);
	
	/*
	 domNode_Array& nodes = _domScene->getNode_array();
	 
	 for (size_t i = 0; i < nodes.getCount(); i++) {
	 lookup<Node, domNode>(*nodes[i]);
	 }
	 */
	
	return scene;
	//_scene = lookup<Scene, domVisual_scene>(_domScene);
	
}

void LoadThread::run()
{
	
	
	domCOLLADA* root;
	
	_scene = NULL;
	
	while (true) {
		_semaphore->acquire();
		if (_stop) {
			return;
		}
		
		
		root = _daeObject->open( _daeFileName.toStdString() );
		
		try {
			if (_scene != NULL) {
				delete _scene;
			}
			_scene = new Scene();
			*_scene = convertModel(*root);
			
			emit finished();
			
		}
		catch (...) {
			emit finishedError(0); // 0  is irrelevant
		}
		
		
	}
	
}

Scene LoadThread::getScene()
{
	Scene result;
	_sceneMutex.lock();
	result = *_scene;
	_sceneMutex.unlock();
	return result;
}

void LoadThread::get()
{
	_semaphore->release();
}



SaveThread::SaveThread() : _semaphore(0)
{
	_stop = false;
}

SaveThread::~SaveThread()
{
	if (_currentDae) {
		delete _currentDae;
	}
	
}


void SaveThread::run()
{
	
	//_currentDae = new DAE();
	
	
	while (true) {
		
		_semaphore.acquire();
		if (_stop) {
			return;
		}
		
			
		_frameMutex.lock();
		_currentDae = new DAE();
		
		_currentDae->load(_daeFileName.toAscii());
		_workingAnimationFrame = _animationFrames.front();
		_workingFileName = _fileNames.front();
        _currentImageHash = _images.front();

        _animationFrames.pop_front();
		_fileNames.pop_front();
        _images.pop_front();
		
                QString fileName = _workingFileName;
		fileName.remove("file:///");
		fileName.remove(".dae");


                //test.save(fileName, "PNG", 100);


                QHashIterator<QString, QImage> i(_currentImageHash);
                while (i.hasNext()) {
                    i.next();
                    i.value().save(fileName +
                                   QString("_") +
                                   i.key() +
                                   QString(".png") , "PNG", 100);
                }



                //_currentImage.save(fileName, "PNG", 100);


                _frameMutex.unlock();
		
		updateFluidParticlesOnCollada();
		saveFluidInformationToFile();
		//updateFluidGeometryOnCollada();
		
		
		_currentDae->saveAs(_workingFileName.toStdString().c_str(), _daeFileName.toStdString().c_str() );
		
		releaseElements();
		
		_currentDae->unload(_daeFileName.toAscii());
		delete _currentDae;
		
		//particlePositionElement->release();
		DAE::cleanup();	
	
	}
}

void SaveThread::releaseElements()
{
	
	daeElement::removeFromParent(particlePositionElement);
	daeElement::removeFromParent(particleTemperatureElement);
	daeElement::removeFromParent(particleMassElement);
	daeElement::removeFromParent(particleDensityElement);
	daeElement::removeFromParent(particleOriginalDensityElement);
	daeElement::removeFromParent(particleMassDensityRatio);
	daeElement::removeFromParent(particleVisible);
	daeElement::removeFromParent(particleTrueSurfaceArray);
	daeElement::removeFromParent(particleTrueSurfaceTimeArray);
	daeElement::removeFromParent(bubblePositionElement);
	daeElement::removeFromParent(bubbleTemperatureElement);
	daeElement::removeFromParent(bubbleMassElement);
	daeElement::removeFromParent(bubbleDensityElement);
	daeElement::removeFromParent(bubbleOriginalDensityElement);
	daeElement::removeFromParent(bubbleSizeElement);
	daeElement::removeFromParent(bubbleMassDensityRatio);
	daeElement::removeFromParent(bubbleDensityAddedElement);
	daeElement::removeFromParent(bubbleVisible);
	daeElement::removeFromParent(bubbleTrueSurfaceArray);
	daeElement::removeFromParent(bubbleTrueSurfaceTimeArray);
	daeElement::removeFromParent(particleNormalElement);
	daeElement::removeFromParent(bubbleNormalElement);
	daeElement::removeFromParent(particleSmoothingLengthElement);
	daeElement::removeFromParent(bubbleSmoothingLengthElement);
	
}

void SaveThread::saveFluidInformationToFile()
{
	QString data = "";
	
	//
	
	std::map<QString, double> fluidProperties;
	std::map<QString, double>::iterator fpIterator;
	
	fluidProperties["averageTemperature"] 		= 0;
	fluidProperties["averageNeighbourCount"] 	= 0;
	fluidProperties["averageShearModulus"] 		= 0;
	fluidProperties["averageDecayRate"] 		= 0;
	fluidProperties["averagePressure"] 			= 0;
	fluidProperties["averageBubblePressure"] 	= 0;
	fluidProperties["averageDensity"] 			= 0;
	fluidProperties["averageVelocityTuner"] 	= 0;
	
	fluidProperties["minTemperature"] 			= DBL_MAX;
	fluidProperties["minNeighbourCount"] 		= DBL_MAX;
	fluidProperties["minShearModulus"] 			= DBL_MAX;
	fluidProperties["minDecayRate"] 			= DBL_MAX;
	fluidProperties["minPressure"] 				= DBL_MAX;
	fluidProperties["minBubblePressure"] 		= DBL_MAX;
	fluidProperties["minDensity"] 				= DBL_MAX;
	fluidProperties["minVelocityTuner"] 		= DBL_MAX;
	fluidProperties["minPosX"] 					= DBL_MAX;                               
	fluidProperties["minPosY"] 					= DBL_MAX;                                      
	fluidProperties["minPosZ"] 					= DBL_MAX;  
	
	fluidProperties["maxTemperature"] 			= DBL_MIN;	
	fluidProperties["maxNeighbourCount"]		= DBL_MIN;
	fluidProperties["maxShearModulus"] 			= DBL_MIN;
	fluidProperties["maxDecayRate"] 			= DBL_MIN;
	fluidProperties["maxPressure"] 				= DBL_MIN;
	fluidProperties["maxBubblePressure"] 		= DBL_MIN;
	fluidProperties["maxDensity"] 				= DBL_MIN;
	fluidProperties["maxVelocityTuner"] 		= DBL_MIN;                                
	fluidProperties["maxPosX"] 					= DBL_MIN;                               
	fluidProperties["maxPosY"] 					= DBL_MIN;                                      
	fluidProperties["maxPosZ"] 					= DBL_MIN;                                      

	//
	for (ovInt i=0; i < _scene.directory.getSize(); i++) {
		if (_scene.directory[i].nodeType == GeometricObjectNode &&
			PROPERTIES->getObjectProperties(_scene.directory[i].glName)->getObjectType() == Fluid &&
			_workingAnimationFrame.listFluids.size() ) {
					
			ovInt currentIndex = _workingAnimationFrame.getIndexByGLName(_scene.directory[i].glName);
		
			if (currentIndex != OUT_OF_RANGE_INDEX) { 
			
				ovInt particleCount = _workingAnimationFrame.listParticleCount[currentIndex];
				ovInt bubbleCount = _workingAnimationFrame.listBubblesCount[currentIndex];
			
				// particles
				for (ovInt j=0; j < particleCount; j++) {
					
					// average 
					
					fluidProperties["averageTemperature"]		+= _workingAnimationFrame.listParticles[currentIndex][j].temp; 	
					fluidProperties["averageNeighbourCount"] 	+= _workingAnimationFrame.listParticles[currentIndex][j].neighbourCount;
					fluidProperties["averageShearModulus"] 		+= _workingAnimationFrame.listParticles[currentIndex][j].shearModulus;
					fluidProperties["averageDecayRate"] 		+= _workingAnimationFrame.listParticles[currentIndex][j].decayRate;
					fluidProperties["averagePressure"] 			+= _workingAnimationFrame.listParticles[currentIndex][j].pressure;
					fluidProperties["averageBubblePressure"] 	+= _workingAnimationFrame.listParticles[currentIndex][j].bubblePressure;
					fluidProperties["averageDensity"] 			+= _workingAnimationFrame.listParticles[currentIndex][j].density;
					fluidProperties["averageVelocityTuner"] 	+= _workingAnimationFrame.listParticles[currentIndex][j].velocityTuner;
					
					// min
					
					if (fluidProperties["minTemperature"] > _workingAnimationFrame.listParticles[currentIndex][j].temp) {
						fluidProperties["minTemperature"] = _workingAnimationFrame.listParticles[currentIndex][j].temp;
					}
					if (fluidProperties["minNeighbourCount"] > _workingAnimationFrame.listParticles[currentIndex][j].neighbourCount) {
						fluidProperties["minNeighbourCount"] = _workingAnimationFrame.listParticles[currentIndex][j].neighbourCount;
					}
					if (fluidProperties["minShearModulus"] > _workingAnimationFrame.listParticles[currentIndex][j].shearModulus) {
						fluidProperties["minShearModulus"] = _workingAnimationFrame.listParticles[currentIndex][j].shearModulus;
					}
					if (fluidProperties["minDecayRate"] > _workingAnimationFrame.listParticles[currentIndex][j].decayRate) {
						fluidProperties["minDecayRate"] = _workingAnimationFrame.listParticles[currentIndex][j].decayRate;
					}	
					if (fluidProperties["minPressure"] > _workingAnimationFrame.listParticles[currentIndex][j].pressure) {
						fluidProperties["minPressure"] = _workingAnimationFrame.listParticles[currentIndex][j].pressure;
					}
					if (fluidProperties["minBubblePressure"] > _workingAnimationFrame.listParticles[currentIndex][j].bubblePressure) {
						fluidProperties["minBubblePressure"] = _workingAnimationFrame.listParticles[currentIndex][j].bubblePressure;
					}
					if (fluidProperties["minDensity"] > _workingAnimationFrame.listParticles[currentIndex][j].density) {
						fluidProperties["minDensity"] = _workingAnimationFrame.listParticles[currentIndex][j].density;
					}
					if (fluidProperties["minVelocityTuner"] > _workingAnimationFrame.listParticles[currentIndex][j].velocityTuner) {
						fluidProperties["minVelocityTuner"] = _workingAnimationFrame.listParticles[currentIndex][j].velocityTuner;
					}
					if (fluidProperties["minPosX"] > _workingAnimationFrame.listParticles[currentIndex][j].pos.x) {
						fluidProperties["minPosX"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.x;
					}
					if (fluidProperties["minPosY"] > _workingAnimationFrame.listParticles[currentIndex][j].pos.y) {
						fluidProperties["minPosY"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.y;
					}
					if (fluidProperties["minPosZ"] > _workingAnimationFrame.listParticles[currentIndex][j].pos.z) {
						fluidProperties["minPosZ"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.z;
					}	
					
					// max
					
					if (fluidProperties["maxTemperature"] < _workingAnimationFrame.listParticles[currentIndex][j].temp) {
						fluidProperties["maxTemperature"] = _workingAnimationFrame.listParticles[currentIndex][j].temp;
					}
					if (fluidProperties["maxNeighbourCount"] < _workingAnimationFrame.listParticles[currentIndex][j].neighbourCount) {
						fluidProperties["maxNeighbourCount"] = _workingAnimationFrame.listParticles[currentIndex][j].neighbourCount;
					}
					if (fluidProperties["maxShearModulus"] < _workingAnimationFrame.listParticles[currentIndex][j].shearModulus) {
						fluidProperties["maxShearModulus"] = _workingAnimationFrame.listParticles[currentIndex][j].shearModulus;
					}
					if (fluidProperties["maxDecayRate"] < _workingAnimationFrame.listParticles[currentIndex][j].decayRate) {
						fluidProperties["maxDecayRate"] = _workingAnimationFrame.listParticles[currentIndex][j].decayRate;
					}	
					if (fluidProperties["maxPressure"] < _workingAnimationFrame.listParticles[currentIndex][j].pressure) {
						fluidProperties["maxPressure"] = _workingAnimationFrame.listParticles[currentIndex][j].pressure;
					}
					if (fluidProperties["maxBubblePressure"] < _workingAnimationFrame.listParticles[currentIndex][j].bubblePressure) {
						fluidProperties["maxBubblePressure"] = _workingAnimationFrame.listParticles[currentIndex][j].bubblePressure;
					}
					if (fluidProperties["maxDensity"] < _workingAnimationFrame.listParticles[currentIndex][j].density) {
						fluidProperties["maxDensity"] = _workingAnimationFrame.listParticles[currentIndex][j].density;
					}
					if (fluidProperties["maxVelocityTuner"] < _workingAnimationFrame.listParticles[currentIndex][j].velocityTuner) {
						fluidProperties["maxVelocityTuner"] = _workingAnimationFrame.listParticles[currentIndex][j].velocityTuner;
					}
					if (fluidProperties["maxPosX"] < _workingAnimationFrame.listParticles[currentIndex][j].pos.x) {
						fluidProperties["maxPosX"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.x;
					}
					if (fluidProperties["maxPosY"] < _workingAnimationFrame.listParticles[currentIndex][j].pos.y) {
						fluidProperties["maxPosY"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.y;
					}
					if (fluidProperties["maxPosZ"] < _workingAnimationFrame.listParticles[currentIndex][j].pos.z) {
						fluidProperties["maxPosZ"] = _workingAnimationFrame.listParticles[currentIndex][j].pos.z;
					}		
					
				}
			
				// bubbles
				for (ovInt j=0; j < bubbleCount; j++) {
					// average
					
					fluidProperties["averageTemperature"]		+= _workingAnimationFrame.listBubbles[currentIndex][j].temp; 	
					fluidProperties["averageNeighbourCount"] 	+= _workingAnimationFrame.listBubbles[currentIndex][j].neighbourCount;
					fluidProperties["averageShearModulus"] 		+= _workingAnimationFrame.listBubbles[currentIndex][j].shearModulus;
					fluidProperties["averageDecayRate"] 		+= _workingAnimationFrame.listBubbles[currentIndex][j].decayRate;
					fluidProperties["averagePressure"] 			+= _workingAnimationFrame.listBubbles[currentIndex][j].pressure;
					fluidProperties["averageBubblePressure"] 	+= _workingAnimationFrame.listBubbles[currentIndex][j].bubblePressure;
					fluidProperties["averageDensity"] 			+= _workingAnimationFrame.listBubbles[currentIndex][j].density;
					fluidProperties["averageVelocityTuner"] 	+= _workingAnimationFrame.listBubbles[currentIndex][j].velocityTuner;
					
					
					// min
					
					if (fluidProperties["minTemperature"] > _workingAnimationFrame.listBubbles[currentIndex][j].temp) {
						fluidProperties["minTemperature"] = _workingAnimationFrame.listBubbles[currentIndex][j].temp;
					}
					if (fluidProperties["minNeighbourCount"] > _workingAnimationFrame.listBubbles[currentIndex][j].neighbourCount) {
						fluidProperties["minNeighbourCount"] = _workingAnimationFrame.listBubbles[currentIndex][j].neighbourCount;
					}
					if (fluidProperties["minShearModulus"] > _workingAnimationFrame.listBubbles[currentIndex][j].shearModulus) {
						fluidProperties["minShearModulus"] = _workingAnimationFrame.listBubbles[currentIndex][j].shearModulus;
					}
					if (fluidProperties["minDecayRate"] > _workingAnimationFrame.listBubbles[currentIndex][j].decayRate) {
						fluidProperties["minDecayRate"] = _workingAnimationFrame.listBubbles[currentIndex][j].decayRate;
					}	
					if (fluidProperties["minPressure"] > _workingAnimationFrame.listBubbles[currentIndex][j].pressure) {
						fluidProperties["minPressure"] = _workingAnimationFrame.listBubbles[currentIndex][j].pressure;
					}
					if (fluidProperties["minBubblePressure"] > _workingAnimationFrame.listBubbles[currentIndex][j].bubblePressure) {
						fluidProperties["minBubblePressure"] = _workingAnimationFrame.listBubbles[currentIndex][j].bubblePressure;
					}
					if (fluidProperties["minDensity"] > _workingAnimationFrame.listBubbles[currentIndex][j].density) {
						fluidProperties["minDensity"] = _workingAnimationFrame.listBubbles[currentIndex][j].density;
					}
					if (fluidProperties["minVelocityTuner"] > _workingAnimationFrame.listBubbles[currentIndex][j].velocityTuner) {
						fluidProperties["minVelocityTuner"] = _workingAnimationFrame.listBubbles[currentIndex][j].velocityTuner;
					}
					if (fluidProperties["minPosX"] > _workingAnimationFrame.listBubbles[currentIndex][j].pos.x) {
						fluidProperties["minPosX"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.x;
					}
					if (fluidProperties["minPosY"] > _workingAnimationFrame.listBubbles[currentIndex][j].pos.y) {
						fluidProperties["minPosY"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.y;
					}
					if (fluidProperties["minPosZ"] > _workingAnimationFrame.listBubbles[currentIndex][j].pos.z) {
						fluidProperties["minPosZ"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.z;
					}
					
					
					// max
					
					if (fluidProperties["maxTemperature"] < _workingAnimationFrame.listBubbles[currentIndex][j].temp) {
						fluidProperties["maxTemperature"] = _workingAnimationFrame.listBubbles[currentIndex][j].temp;
					}
					if (fluidProperties["maxNeighbourCount"] < _workingAnimationFrame.listBubbles[currentIndex][j].neighbourCount) {
						fluidProperties["maxNeighbourCount"] = _workingAnimationFrame.listBubbles[currentIndex][j].neighbourCount;
					}
					if (fluidProperties["maxShearModulus"] < _workingAnimationFrame.listBubbles[currentIndex][j].shearModulus) {
						fluidProperties["maxShearModulus"] = _workingAnimationFrame.listBubbles[currentIndex][j].shearModulus;
					}
					if (fluidProperties["maxDecayRate"] < _workingAnimationFrame.listBubbles[currentIndex][j].decayRate) {
						fluidProperties["maxDecayRate"] = _workingAnimationFrame.listBubbles[currentIndex][j].decayRate;
					}	
					if (fluidProperties["maxPressure"] < _workingAnimationFrame.listBubbles[currentIndex][j].pressure) {
						fluidProperties["maxPressure"] = _workingAnimationFrame.listBubbles[currentIndex][j].pressure;
					}
					if (fluidProperties["maxBubblePressure"] < _workingAnimationFrame.listBubbles[currentIndex][j].bubblePressure) {
						fluidProperties["maxBubblePressure"] = _workingAnimationFrame.listBubbles[currentIndex][j].bubblePressure;
					}
					if (fluidProperties["maxDensity"] < _workingAnimationFrame.listBubbles[currentIndex][j].density) {
						fluidProperties["maxDensity"] = _workingAnimationFrame.listBubbles[currentIndex][j].density;
					}
					if (fluidProperties["maxVelocityTuner"] < _workingAnimationFrame.listBubbles[currentIndex][j].velocityTuner) {
						fluidProperties["maxVelocityTuner"] = _workingAnimationFrame.listBubbles[currentIndex][j].velocityTuner;
					}
					if (fluidProperties["maxPosX"] < _workingAnimationFrame.listBubbles[currentIndex][j].pos.x) {
						fluidProperties["maxPosX"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.x;
					}
					if (fluidProperties["maxPosY"] < _workingAnimationFrame.listBubbles[currentIndex][j].pos.y) {
						fluidProperties["maxPosY"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.y;
					}
					if (fluidProperties["maxPosZ"] < _workingAnimationFrame.listBubbles[currentIndex][j].pos.z) {
						fluidProperties["maxPosZ"] = _workingAnimationFrame.listBubbles[currentIndex][j].pos.z;
					}	
					

				}
				
				fluidProperties["averageTemperature"]		/= particleCount + bubbleCount;
				fluidProperties["averageNeighbourCount"] 	/= particleCount + bubbleCount;
				fluidProperties["averageShearModulus"] 		/= particleCount + bubbleCount;
				fluidProperties["averageDecayRate"] 		/= particleCount + bubbleCount;
				fluidProperties["averagePressure"] 			/= particleCount + bubbleCount;
				fluidProperties["averageBubblePressure"] 	/= particleCount + bubbleCount;
				fluidProperties["averageDensity"] 			/= particleCount + bubbleCount;
				fluidProperties["averageVelocityTuner"] 	/= particleCount + bubbleCount;
				
			
			}
					
		}
	}
	
	for (fpIterator = fluidProperties.begin();
		 fpIterator != fluidProperties.end();
		 ++fpIterator) {
			
		data += fpIterator->first + " = " + QString::number(fpIterator->second, 'f', 6) + "\n";	
	}
	
	QString fileName = _workingFileName;
	fileName += ".txt";
	fileName.remove("file:///");
	fileName.remove(".dae");
	
	QFile file(fileName);

	if (! file.open(QIODevice::WriteOnly | QIODevice::Text) ) { // QIODevice::IO_Raw | QIODevice::IO_ReadWrite
		std::cout << "could not open file\n";
	}
	file.write(data.toStdString().c_str(), data.size());
	file.close();
	
}

void SaveThread::updateFluidParticlesOnCollada()
{
	
	domNode *node;
	
	daeElement* extra;
	daeElement* technique;
	
	daeElement *visualScene;
	
	domIDREF_array *idRefArray;
	xsIDREFS idRefsValues;
	
	// create extra for cutouts
	
	_currentDae->getDatabase()->getElement((daeElement**)&visualScene, 
										   0, 0, COLLADA_ELEMENT_VISUAL_SCENE);
	
	
	
	
	
	visualScene->removeChildElement( visualScene->getChild("extra") );
	
	extra = visualScene->add("extra");
	technique = extra->add("technique");
	technique->setAttribute("profile", "oven");
	
	idRefArray = daeSafeCast<domIDREF_array>(technique->createAndPlace(COLLADA_ELEMENT_IDREF_ARRAY));
	
	// for all objects
	for (ovInt i=0; i < _scene.directory.getSize(); i++) {
		
		// add cutout information
		
		
		
		if (_scene.directory[i].nodeType == GeometricObjectNode &&
			PROPERTIES->getObjectProperties(_scene.directory[i].glName)->getObjectType() == CutOut) {
			
			//_currentDae->getDatabase()->getElement((daeElement**)&node, 
			//									   0, _scene.directory[i].id.toAscii(), COLLADA_ELEMENT_NODE);
			
			// add id to idrefarray
			
			
			idRefsValues.append(_scene.directory[i].id.toStdString().c_str());
			
		}
		
		// for fluids
		else if (_scene.directory[i].nodeType == GeometricObjectNode &&
				 PROPERTIES->getObjectProperties(_scene.directory[i].glName)->getObjectType() == Fluid &&
				 _workingAnimationFrame.listFluids.size() ) {
			
			
			_currentDae->getDatabase()->getElement((daeElement**)&node, 
												   0, _scene.directory[i].id.toAscii(), COLLADA_ELEMENT_NODE);
			
			// add extra
			extra = node->add("extra");
			technique = extra->add("technique");
			technique->setAttribute("profile", "oven");
			
			
			ovInt currentIndex = _workingAnimationFrame.getIndexByGLName(_scene.directory[i].glName);
			
			
			
			if (currentIndex != OUT_OF_RANGE_INDEX) { 
				
				
				// batter particles
				QString positions;
				QString temperature;
				QString mass;
				QString density;
				QString originalDensity;
				QString massDensityRatio;
				QString visible;
				QString trueSurface;
				QString trueSurfaceTime;
				QString particleNormals;
				QString smoothingLength;
				
				ovInt particleCount = _workingAnimationFrame.listParticleCount[currentIndex];
				
				particlePositionElement = technique->add("float_array");
				particlePositionElement->setAttribute("id", "particlePosition");
				particlePositionElement->setAttribute("count", QString::number(particleCount * 3).toStdString().c_str() );
				
				particleTemperatureElement = technique->add("float_array");
				particleTemperatureElement->setAttribute("id", "particleTemperature");
				particleTemperatureElement->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleMassElement = technique->add("float_array");
				particleMassElement->setAttribute("id", "particleMass");
				particleMassElement->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleDensityElement = technique->add("float_array");
				particleDensityElement->setAttribute("id", "particleDensity");
				particleDensityElement->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleOriginalDensityElement = technique->add("float_array");
				particleOriginalDensityElement->setAttribute("id", "particleOriginalDensity");
				particleOriginalDensityElement->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleMassDensityRatio = technique->add("float_array");
				particleMassDensityRatio->setAttribute("id", "particleMassDensityRatio");
				particleMassDensityRatio->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleVisible = technique->add("float_array");
				particleVisible->setAttribute("id", "particleVisible");
				particleVisible->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleTrueSurfaceArray = technique->add("float_array");
				particleTrueSurfaceArray->setAttribute("id", "particleTrueSurface");
				particleTrueSurfaceArray->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleTrueSurfaceTimeArray = technique->add("float_array");
				particleTrueSurfaceTimeArray->setAttribute("id", "particleTrueSurfaceTime");
				particleTrueSurfaceTimeArray->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				particleNormalElement = technique->add("float_array");
				particleNormalElement->setAttribute("id", "particleNormal");
				particleNormalElement->setAttribute("count", QString::number(particleCount * 3).toStdString().c_str() );
				
				particleSmoothingLengthElement = technique->add("float_array");
				particleSmoothingLengthElement->setAttribute("id", "particleSmoothingLength");
				particleSmoothingLengthElement->setAttribute("count", QString::number(particleCount).toStdString().c_str() );
				
				for (ovInt j=0; j < particleCount; j++) {
					
					// add position
					
					positions += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].pos.x, 'f', 12 ) + ' ';
					positions += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].pos.y, 'f', 12 ) + ' ';
					positions += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].pos.z, 'f', 12 ) + ' ';
					
					// add temperature 
					temperature +=  QString::number( _workingAnimationFrame.listParticles[currentIndex][j].temp ) + ' ';
					
					// add mass // should be constant
					mass += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].mass ) + ' ';
					
					// add density
					density += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].density ) + ' ';
					
					// add originalDensity
					originalDensity += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].originalDensity ) + ' ';
					
					// add mass / density ratio
					
					massDensityRatio += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].massDensityRatio ) + ' ';
					
					// add visible
					
					visible += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].visible ) + ' ';
					
					// add trueSurface
					
					trueSurface += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].trueSurface ) + ' ';
					
					// add trueSurfaceTime
					
					trueSurfaceTime += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].trueSurfaceTime ) + ' ';
					
					// add normal
					
					particleNormals += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].normal.x, 'f', 12 ) + ' ';
					particleNormals += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].normal.y, 'f', 12 ) + ' ';
					particleNormals += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].normal.z, 'f', 12 ) + ' ';
					
					// add smoothing lenght
					
					smoothingLength += QString::number( _workingAnimationFrame.listParticles[currentIndex][j].smoothingLength ) + ' ';
					
				}
				particlePositionElement->setCharData( positions.toStdString() ); // set to positions
				particleTemperatureElement->setCharData( temperature.toStdString() ); // set to temperature
				particleMassElement->setCharData( mass.toStdString() ); // set to mass
				particleDensityElement->setCharData( density.toStdString() ); // set to density
				particleOriginalDensityElement->setCharData( originalDensity.toStdString() ); // set to originalDensity
				particleMassDensityRatio->setCharData( massDensityRatio.toStdString() );  // set mass / density ratio
				particleVisible->setCharData( visible.toStdString() );  // set visible
				particleTrueSurfaceArray->setCharData(trueSurface.toStdString()); // set true surface
				particleTrueSurfaceTimeArray->setCharData(trueSurfaceTime.toStdString()); // set true surface time
				particleNormalElement->setCharData(particleNormals.toStdString()); // set normals
				particleSmoothingLengthElement->setCharData(smoothingLength.toStdString()); // set smoothing length

				
				// bubble particles
				positions = "";
				temperature = "";
				mass = "";
				density = "";
				originalDensity = "";
				massDensityRatio = "";
				visible = "";
				trueSurface = "";
				trueSurfaceTime = "";
				particleNormals = "";
				smoothingLength = "";
				
				QString size;
				QString densityAdded;
				
				ovInt bubbleCount = _workingAnimationFrame.listBubblesCount[currentIndex];
				
				bubblePositionElement = technique->add("float_array");
				bubblePositionElement->setAttribute("id", "bubblePosition");
				bubblePositionElement->setAttribute("count", QString::number(bubbleCount * 3).toStdString().c_str() );
				
				bubbleTemperatureElement = technique->add("float_array");
				bubbleTemperatureElement->setAttribute("id", "bubbleTemperature");
				bubbleTemperatureElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
		
				bubbleMassElement = technique->add("float_array");
				bubbleMassElement->setAttribute("id", "bubbleMass");
				bubbleMassElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleDensityElement = technique->add("float_array");
				bubbleDensityElement->setAttribute("id", "bubbleDensity");
				bubbleDensityElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleOriginalDensityElement = technique->add("float_array");
				bubbleOriginalDensityElement->setAttribute("id", "bubbleOriginalDensity");
				bubbleOriginalDensityElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleDensityAddedElement = technique->add("float_array");
				bubbleDensityAddedElement->setAttribute("id", "bubbleDensityAdded");
				bubbleDensityAddedElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleSizeElement = technique->add("float_array");
				bubbleSizeElement->setAttribute("id", "bubbleSize");
				bubbleSizeElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				
				bubbleMassDensityRatio = technique->add("float_array");
				bubbleMassDensityRatio->setAttribute("id", "bubbleMassDensityRatio");
				bubbleMassDensityRatio->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleVisible = technique->add("float_array");
				bubbleVisible->setAttribute("id", "bubbleVisible");
				bubbleVisible->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleTrueSurfaceArray = technique->add("float_array");
				bubbleTrueSurfaceArray->setAttribute("id", "bubbleTrueSurface");
				bubbleTrueSurfaceArray->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleTrueSurfaceTimeArray = technique->add("float_array");
				bubbleTrueSurfaceTimeArray->setAttribute("id", "bubbleTrueSurfaceTime");
				bubbleTrueSurfaceTimeArray->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				bubbleNormalElement = technique->add("float_array");
				bubbleNormalElement->setAttribute("id", "bubbleNormal");
				bubbleNormalElement->setAttribute("count", QString::number(bubbleCount * 3).toStdString().c_str() );
				
				bubbleSmoothingLengthElement = technique->add("float_array");
				bubbleSmoothingLengthElement->setAttribute("id", "bubbleSmoothingLength");
				bubbleSmoothingLengthElement->setAttribute("count", QString::number(bubbleCount).toStdString().c_str() );
				
				for (ovInt j=0; j < bubbleCount; j++) {
					
					// add position
					positions += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].pos.x, 'f', 12 ) + ' ';
					positions += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].pos.y, 'f', 12 ) + ' ';
					positions += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].pos.z, 'f', 12 ) + ' ';
					
					// add temperature 
					temperature +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].temp ) + ' ';
								
					// add mass
					mass +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].mass ) + ' ';
					
					// add density
					density +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].density, 'f', 64 ) + ' ';					
					
					// add density
					originalDensity +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].originalDensity, 'f', 64 ) + ' ';					
					
					
					// add density added
					densityAdded +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].densityAdded ) + ' ';		
								 
					// add size 
					size +=  QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].size ) + ' ';
					
					// add mass / density ratio
					massDensityRatio += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].massDensityRatio, 'f', 12 ) + ' ';
					
					// add visible
					
					visible += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].visible ) + ' ';
					
					// add true surface
					
					trueSurface += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].trueSurface ) + ' ';
					
					// add true surface time
					
					trueSurfaceTime += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].trueSurfaceTime ) + ' ';
					
					
					// add normals
					
					particleNormals += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].normal.x, 'f', 12 ) + ' ';
					particleNormals += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].normal.y, 'f', 12 ) + ' ';
					particleNormals += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].normal.z, 'f', 12 ) + ' ';
					
					// add smoothing lenght
					
					smoothingLength += QString::number( _workingAnimationFrame.listBubbles[currentIndex][j].smoothingLength ) + ' ';
					
				}
				
				bubblePositionElement->setCharData( positions.toStdString() ); // set to positions
				bubbleTemperatureElement->setCharData( temperature.toStdString() ); // set to temperature
				
				bubbleMassElement->setCharData( mass.toStdString() ); // set to temperature
				bubbleDensityElement->setCharData( density.toStdString() ); // set to temperature								
				bubbleOriginalDensityElement->setCharData( originalDensity.toStdString() ); // set to temperature								
				bubbleDensityAddedElement->setCharData( densityAdded.toStdString() ); // set to temperature
								
				bubbleSizeElement->setCharData( size.toStdString() ); // set to size
				bubbleMassDensityRatio->setCharData( massDensityRatio.toStdString() );  // set mass / density ratio
				bubbleVisible->setCharData( visible.toStdString() );  // set visible
				bubbleTrueSurfaceArray->setCharData(trueSurface.toStdString()); // set true surface
				bubbleTrueSurfaceTimeArray->setCharData(trueSurfaceTime.toStdString()); // set true surface time
				bubbleNormalElement->setCharData(particleNormals.toStdString()); // set normal
				bubbleSmoothingLengthElement->setCharData(smoothingLength.toStdString()); // set smoothing length

				
			}
		}
	}
	
	idRefArray->setName("cutouts");
	idRefArray->setValue(idRefsValues);
	idRefArray->setCount(idRefsValues.getCount());
}



inline void SaveThread::stop()
{
	// finish cleanly
	_stop = true; 
	_semaphore.release();
	
}



inline void SaveThread::setScene(Scene scene)
{
	_sceneMutex.lock();
	_scene = scene;
	_sceneMutex.unlock();
	
}

inline void SaveThread::saveFrame(AnimationFrame frame, QHash<QString, QImage> images, QString name)
{
	_frameMutex.lock();
	// a more apt way would be using pairs, but this is good enough
	_animationFrames.push_back(frame);
	_fileNames.push_back(name);
    _images.push_back(images);
	_frameMutex.unlock();
	_semaphore.release();
}





// MainWindow

MainWindow::MainWindow()
{
	
    setupUi(this); // this sets up GUI
	
	LOGS->setLogTextEdit(textEditConsole);
	LOGS->logEvent("Starting oven system", Important);
	
	finishSettingGUI();
	
	connect( _qSliderFrames, SIGNAL( valueChanged(int) ), this, SLOT( updateQSpinBoxFrames() )); 
	connect( _qSpinBoxFrames, SIGNAL( valueChanged(int) ), this, SLOT( updateQSliderFrames() ));
	
	connect( actionOpen, SIGNAL( triggered() ), this, SLOT( loadFile() ));
	connect( actionSave, SIGNAL( triggered() ), this, SLOT( saveFrames() ));
	connect( actionQuit, SIGNAL( triggered() ), qApp, SLOT( quit() ));
	
	connect( actionParticles, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewParticles(bool) ));
	connect( actionSurface, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewSurface(bool) ));
	connect( actionBubbleParticles, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewBubbleParticles(bool) ));
	connect( actionCut_outs, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewCutOuts(bool) ));
	connect( actionVelocity, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewVelocity(bool) ));
	connect( actionGradient, SIGNAL( toggled(bool) ), viewPortWidget, SLOT( setViewGradient(bool) ));

	connect( &_loadThread, SIGNAL( finished() ), this, SLOT( setScene() ));
	connect( &_loadThread, SIGNAL( finishedError(int) ), this, SLOT( getError(int) ));
	
	connect( treeWidgetObjectInspector, SIGNAL( currentItemChanged ( QTreeWidgetItem* , QTreeWidgetItem* )   ), 
			viewPortWidget, SLOT(setSelectedObject( QTreeWidgetItem* , QTreeWidgetItem* ) ));
	
	connect( treeWidgetObjectInspector, SIGNAL( currentItemChanged ( QTreeWidgetItem* , QTreeWidgetItem* )   ), 
			this, SLOT(setSelectedProperties( QTreeWidgetItem* , QTreeWidgetItem* ) ));
	
	connect( viewPortWidget, SIGNAL(selectOnObjectList(ovInt)),
			this, SLOT( updateObjectInspector(ovInt) ) );
	connect( viewPortWidget, SIGNAL(selectOnObjectList(ovInt)),
			this, SLOT( updatePropertyEditor(ovInt) ) );
	
	connect( actionRecord, SIGNAL( triggered() ), this, SLOT( startBaking() ) );
	connect( actionPlay, SIGNAL( triggered() ), this, SLOT( playAnimation() ) );
	connect( actionLast, SIGNAL( triggered() ), this, SLOT( showLastFrame() ) );
	connect( actionFirst, SIGNAL( triggered() ), this, SLOT( showFirstFrame() ) );
	connect( actionLiveSave, SIGNAL( triggered() ), this, SLOT( setLiveSave() ) );
	
	connect( PROPERTIES, SIGNAL( updatedValues() ), viewPortWidget, SLOT( updateViewPort() ) );
	
	
	//connect(&_animationGenerator, SIGNAL( newFrame(int) ), this, SLOT( getFrame(int) ) );
	//qRegisterMetaType< AnimationFrame >("AnimationFrame");
	connect(&_animationGenerator, SIGNAL( newFrame() ), this, SLOT( getFrame() ) );
	//^^^
	
	connect(&_animationGenerator, SIGNAL( newAnimation(int) ), this, SLOT( setAnimation(int) ) ); 
	
	_qFrameTimer = new QTimer(this);
	connect(_qFrameTimer, SIGNAL(timeout()), this, SLOT( setNextFrame() ));
	
	connect(&_animationGenerator, SIGNAL( errorMessage(QString) ), this, SLOT( displayErrorMessage(QString) ) ); 
	
	
	connect(actionPosition,		SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypePosition(bool) ) );
	connect(actionTemperature,	SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeTemperature(bool) ) );
	connect(actionSize,			SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeSize(bool) ) );
	connect(actionCO2,			SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeCO2(bool) ) );
	connect(actionOn_surface,	SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeOnSurface(bool) ) );
	connect(actionIn_collision, SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeInCollision(bool) ) );
	connect(actionTrue_surface, SIGNAL( triggered (bool) ), this, SLOT( setParticleRenderingTypeTrueSurface(bool) ) );
	
	connect(doubleSpinBoxSplatRadius, SIGNAL( valueChanged (double) ), this, SLOT(updateSplatRadius(double)));
	connect(horizontalSliderSplatRadius, SIGNAL( valueChanged (int) ), this, SLOT(updateSplatRadius(int)));

        connect(viewPortWidget, SIGNAL( cameraChanged() ), this, SLOT( updateCameraValueWidgets() ) );

        connect(doubleSpinBoxPosX, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxPosY, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxPosZ, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );

        connect(doubleSpinBoxRot_0_0, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_0_1, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_0_2, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );

        connect(doubleSpinBoxRot_1_0, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_1_1, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_1_2, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );

        connect(doubleSpinBoxRot_2_0, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_2_1, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );
        connect(doubleSpinBoxRot_2_2, SIGNAL( valueChanged(double) ), this, SLOT( updateCameraValues(double) ) );



	_qTreeWidgetItem  = NULL;
	_scene			  = NULL;
	_animating		  = false;
	_playingAnimation = false;
	_liveSave		  = false;
	
	_lastParticleActionType = actionPosition;
	
	
	_daeObject = new DAE;
	//_daeObject->setIntegrationLibrary(&intRegisterElements);
	
	_loadThread.start();
	_loadThread.setDAEObject(_daeObject);
	
	_saveThread.start();
	_saveThread.setDAEObject(_daeObject);
	
	_animationGenerator.start();
	
	
	
	srand(42); // start randomization
	
}

MainWindow::~MainWindow()
{
	_loadThread.stop();
	_loadThread.wait();
	_animationGenerator.stop();
	_animationGenerator.wait();
	
	delete _daeObject;
	delete _qSliderFrames;
	delete _qSpinBoxFrames;
	
}

void MainWindow::finishSettingGUI()
{
	// set size and position
#if 0	
	resize( QApplication::desktop()->QDesktopWidget::availableGeometry().width() * 0.95,
		   QApplication::desktop()->QDesktopWidget::availableGeometry().height() * 0.95 );
	
#ifdef Q_OS_WIN
	move( QApplication::desktop()->QDesktopWidget::availableGeometry().width() * 0.0225, 
		 QApplication::desktop()->QDesktopWidget::availableGeometry().height() * 0.01 );
#endif
	#endif 
	resize(950, 500);
	// window menu
	
	menuTools->addAction(dockWidgetObjectInspector->toggleViewAction());
	menuTools->addAction(dockWidgetPropertyEditor->toggleViewAction());
	menuTools->addAction(dockWidgetConsole->toggleViewAction());
	menuTools->addAction(dockWidgetRenderingOptions->toggleViewAction());
	menuTools->addSeparator();
	menuTools->addAction(toolBarAnimationControls->toggleViewAction());
	
	// frame controls
	
	_qSliderFrames = new QSlider(Qt::Horizontal);
	_qSpinBoxFrames = new QSpinBox();
	_qSliderFrames->setTickPosition(QSlider::TicksBothSides);
	
	_qSpinBoxFrames->setMaximum(0);
	_qSliderFrames->setMaximum(0);
	_qSpinBoxFrames->setFixedWidth(64);
	_qSliderFrames->setMinimumWidth(200);
	
	toolBarAnimationControls->addSeparator();
	toolBarAnimationControls->addWidget(_qSpinBoxFrames);
	toolBarAnimationControls->addWidget(_qSliderFrames);
	toolBarAnimationControls->addSeparator();
	
	// console
	
	dockWidgetConsole->hide();
	dockWidgetConsole->setFloating(true);
	dockWidgetConsole->resize(640, 480);
	
	// object inspector
	
	treeWidgetObjectInspector->header()->hide();
	
	tableWidgetPropertyEditor->setItemDelegate( new PropertiesDelegate() );
	
	tableWidgetPropertyEditor->verticalHeader()->hide();
	
    tableWidgetPropertyEditor->horizontalHeader()->hide();
	tableWidgetPropertyEditor->horizontalHeader()->setResizeMode(QHeaderView::Stretch); 
	
	tableWidgetPropertyEditor->verticalHeader()->setResizeMode(QHeaderView::Fixed);
	tableWidgetPropertyEditor->verticalHeader()->setDefaultSectionSize(20);
	
	
	tableWidgetPropertyEditor->setColumnCount(2);
	
}

void MainWindow::updateQSliderFrames()
{
	_qSliderFrames->setValue( _qSpinBoxFrames->value() );
	if ( !	_animationFrames.empty() ) {
		viewPortWidget->setFrame(_animationFrames[_qSpinBoxFrames->value()]);
	}
}

void MainWindow::updateQSpinBoxFrames()
{
	_qSpinBoxFrames->setValue( _qSliderFrames->value() );
	if ( !	_animationFrames.empty() ) {
		viewPortWidget->setFrame(_animationFrames[_qSliderFrames->value()]);
	}
}

void MainWindow::updateObjectInspector(ovInt glName)
{
	QString name;
	QTreeWidgetItemIterator it(treeWidgetObjectInspector);
	
	if ( glName < _scene->directory.getSize()  &&
		glName >= 0) {
		name = _scene->directory[glName].workName;
		
		while (*it) {
			if ( name == (*it)->text(0) ){
				treeWidgetObjectInspector->setCurrentItem(*it);
				break;
			}
			++it;
		}
	}
}

void MainWindow::updatePropertyEditor(ovInt glName)
{
	ovInt _rowCount;
	
	if ( glName < _scene->directory.getSize() && glName >= 0) {
		
		tableWidgetPropertyEditor->horizontalHeader()->setUpdatesEnabled(FALSE);
		
		PROPERTIES->selectedNodeType = _scene->directory[glName].nodeType;
		PROPERTIES->selectedObject = glName;
		
		for (ovInt i=0; i < tableWidgetPropertyEditor->rowCount(); i++) {
			tableWidgetPropertyEditor->takeItem(i,0);
			tableWidgetPropertyEditor->takeItem(i,1);
			
		}
		
		if ( _scene->directory[glName].nodeType == SceneNode ) {				// show scene properties
			_rowCount = PROPERTIES->getSceneProperties().getPropertiesTypeCount();
			tableWidgetPropertyEditor->setRowCount(_rowCount);
			
			ScenePropertyType sceneType;
			
			// show scene widgets
			for (ovInt i=0; i < _rowCount; i++) {
				sceneType = SceneProperties::getPropertiesType(i);
				propertiesWidgets.sceneValues[ sceneType ]->setText( PROPERTIES->getSceneProperties().getTextValue( sceneType ));
				
				tableWidgetPropertyEditor->setItem(i, 0, propertiesWidgets.sceneQTableWidgetItem[ sceneType ]);
				tableWidgetPropertyEditor->setItem(i, 1, propertiesWidgets.sceneValues[ sceneType ] );
				
			}
			
		}
		else if ( _scene->directory[glName].nodeType == GeometricObjectNode ) {	// show individual object properties
			
			_rowCount = PROPERTIES->getObjectProperties(glName)->getPropertiesTypeCount();
			tableWidgetPropertyEditor->setRowCount(_rowCount);
			
			ObjectPropertyType objectPropertyType;
			
			// show object widgets
			for (ovInt i=0; i < _rowCount; i++) {
				
				objectPropertyType = ObjectProperties::getPropertiesType(i);
				
				if (propertiesWidgets.objectValues[ objectPropertyType ]){
					propertiesWidgets.objectValues[ objectPropertyType ]->setText( 
																				  PROPERTIES->getObjectProperties(PROPERTIES->selectedObject)->getTextValue( objectPropertyType )
																				  );
				}
				
				tableWidgetPropertyEditor->setItem(i, 0, propertiesWidgets.objectQTableWidgetItem[ objectPropertyType ]);
				tableWidgetPropertyEditor->setItem(i, 1, propertiesWidgets.objectValues[ objectPropertyType ]);
			}
		}
		else {
			tableWidgetPropertyEditor->setRowCount(0);
		}
		
		
		
		tableWidgetPropertyEditor->horizontalHeader()->setUpdatesEnabled(TRUE); 
	}
}

void MainWindow::setSelectedProperties( QTreeWidgetItem* current, QTreeWidgetItem* previous)
{
	ovInt i;	
	QString _name;
	
	if (current != NULL){
		_name = current->text(0);
		
		for (i = 0; i < _scene->directory.getSize(); i++) {
			
			if (_name == _scene->directory[i].workName) {
				// if an object is found, show its properties
				updatePropertyEditor( i );
				break;
			}
		}
	}
}

void MainWindow::loadFile()
{
	
	if (_daeFileName != "") {
		_daeObject->unload( _daeFileName.toAscii() );
	}
	
	_daeFileName = getFileName("Open file");
	clearData();
	
	if (! _daeFileName.isEmpty() ) {
		_daeFileName.replace("\\", "/");
		_daeFileName = "file:///" + _daeFileName;
		
		_daeObject->clear(); //XXX needed ?
		_loadThread.setFileName( _daeFileName );
		_saveThread.setDAEFileName( _daeFileName );
		_loadThread.get();
	}
}

void MainWindow::saveFrames()
{
	//QString _destination;
	
	//_destination = getDirectoryPath();
	
	//LOGS->logEvent(QString("Saving on") + _destination);
	
	
	QString fileName = _daeFileName.section('/', -1);
	QString path = getDAEPath();
	QString identificator = fileName;
	
	identificator.remove(".dae");
	
	
	QString systemPath = getSystemPath();
	
	QDir mydir("");
	mydir.mkpath(systemPath);
	
	QString length = QString::number( ( _animationFrames.size() ));
	////
	addBubbleToCollada();
	
	///
	for (ovUInt i=0; i< _animationFrames.size(); i++) {
		
		QString destination = path + identificator +  QString("_%1").arg(i, length.size(), 10, QChar('0')) + QString(".dae");
		updateFluidGeometryOnCollada(i);
		//_daeObject->saveAs(destination.toAscii(), _daeFileName.toAscii() );
	}
	
	// clean fluids from lib
	// for all frames
	// add fluids
	// save frame
	// remove fluids
	
	// generate rendering file (?) bat sh et al
	
}

void MainWindow::addBubbleToCollada()
{
	
	// add bubble geometry if not there already
#if 1
	domGeometry *bubbleShape;
	
	QString bubbleFile = getFileName("Open bubble description");
	
	if (! bubbleFile.isEmpty() ) {
		bubbleFile.replace("\\", "/");
		bubbleFile = "file:///" + bubbleFile;
		
		
		
		_daeObject->load( bubbleFile.toAscii() );
		_daeObject->getDatabase()->getElement((daeElement**) &bubbleShape, 0,
											  NULL, COLLADA_ELEMENT_GEOMETRY,
											  bubbleFile.toAscii() );
		
		bubbleShape->setId("OvensUniqueBubbleShape");
		bubbleShape->setName("OvensUniqueBubbleShape");
		
		// XXX should check for other name collisions
		
		
		domLibrary_geometries_Array	 libGeos = _daeObject->getDom(_daeFileName.toAscii())->getLibrary_geometries_array();
		libGeos[0]->placeElement(bubbleShape);
		
		
		_daeObject->unload( bubbleFile.toAscii() );
	}
	
#endif
}


QString MainWindow::getFileName(QString info)
{
	QString _fileName; 
	
	_fileName = QFileDialog::getOpenFileName(this,
											 info,
											 QString(),
											 tr("DAE Files (*.dae);;All Files (*)"));
	
	
	return _fileName;
	
}

QString MainWindow::getDirectoryPath()
{
	QString _pathName;
	
	_pathName = QFileDialog::getExistingDirectory(this,
												  tr("Choose a working folder"),
												  QString(),
												  QFileDialog::DontResolveSymlinks
												  | QFileDialog::ShowDirsOnly);
	
    return _pathName;
	
}

void MainWindow::resetObjectInspector()
{
	
	// remove previous listing if exists	
	treeWidgetObjectInspector->takeTopLevelItem(0);
	
	if (_qTreeWidgetItem != NULL) {
		delete _qTreeWidgetItem;
	}
	
	_qTreeWidgetItem = new QTreeWidgetItem( (QTreeWidget*)0, 
										   QStringList( _scene->name ) );
	
	generateObjectList(_scene->sceneRoot, _qTreeWidgetItem);
	
	// add items
	treeWidgetObjectInspector->insertTopLevelItem(0, _qTreeWidgetItem);
	
	// expand all items
 	QTreeWidgetItemIterator it(treeWidgetObjectInspector);
	while (*it) {
		treeWidgetObjectInspector->expandItem(*it);
		++it;
	}
	
	
}

void MainWindow::generateObjectList(Node* node, QTreeWidgetItem *treeItem)
{
	QList<QTreeWidgetItem *> _items;
	QList<QTreeWidgetItem *> _itemsRemoved;
	int _numChildren, _numItems;
	
	
	// remove old items
	_itemsRemoved = treeItem->takeChildren();
	
	_numItems = _itemsRemoved.size();
	for (ovInt i=0; i<_numItems; i++) {
		delete _itemsRemoved[i];
	}
	
	// add new items
	_numChildren = node->children.size();	
	for (ovInt i=0; i<_numChildren; i++) {		
		_items.append(new QTreeWidgetItem((QTreeWidget*)0, 
										  QStringList(node->children[i]->workName) ));
		
		generateObjectList(node->children[i], _items[i]);	
	}
	
	
	treeItem->addChildren(_items);
	
	
}

void MainWindow::setScene()
{
	if (_scene != NULL) {
		delete _scene;
	}
	
	_scene = new Scene();
	
	*_scene = _loadThread.getScene();
	
	if (_scene) {
		
		PROPERTIES->initialize(_scene); // initializing PROPERTIES
		viewPortWidget->setScene(_scene);
		propertiesWidgets.generate();
		resetObjectInspector();
		printSceneProperties();
		_saveThread.setScene(*_scene);
		
	} 
	
}

void MainWindow::printSceneProperties()
{
	LOGS->logEvent(QString("Opened file %1").arg( _daeFileName ), Important);
	LOGS->logEvent(QString("Number of nodes in scene %1").arg( _scene->getNodeCount() ), Important);
	
}

void MainWindow::printAnimationFrameProperties(AnimationFrame &animationFrame)
{
	
	for (ovUInt i=0; i< animationFrame.listGlNames.size(); i++) {
		Node *currentNode = _scene->directory.getNodeByName(animationFrame.listGlNames[i]);
		
		QString type;
		
		if (PROPERTIES->getObjectProperties(currentNode->glName)->getObjectType() == Solid) {
			type = "Solid";
		}
		else if (PROPERTIES->getObjectProperties(currentNode->glName)->getObjectType() == Fluid) {
			type = "Fluid";
			
		}
		
		LOGS->logEvent(QString("Node %1 [%2]").arg(currentNode->id).arg(type) , Important);
		
		if ( animationFrame.listParticleCount[i] != 0 ) {
			LOGS->logEvent(QString("%1 particles").arg( animationFrame.listParticleCount[i] ) , Important);
		}
		
		if ( animationFrame.listBubblesCount[i] != 0 ) {
			LOGS->logEvent(QString("%1 bubbles").arg( animationFrame.listBubblesCount[i] ) , Important);
		}
		
		
	}
	
}

void MainWindow::printAnimationFrameProperties(ovUInt frameNumber)
{
	
	if (frameNumber < _animationFrames.size() ) {
		printAnimationFrameProperties( _animationFrames[frameNumber] );
	}
	
}



void MainWindow::getError(int res)
{
	LOGS->logEvent(QString("Could not open file %1").arg(_daeFileName), Error);
	//LOGS->logEvent(QString("Error: %1").arg(daeErrorString(res)), Error); 
}

void MainWindow::startBaking()
{
	if (! _animating ) {
		
		//currentWorkingFrame = 0;
                 // por live save
		if ( _liveSave ) {
			QDir mydir("");
			mydir.mkpath( getSystemPath() );
			
			path = getDAEPath();
			identificator = _daeFileName.section('/', -1);
			identificator.remove(".dae");
			length = QString::number(( PROPERTIES->getSceneProperties().getf(endTime) * 24 ));

			
		}

		_animating = true;
		actionRecord->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/stop.png")));
		_lastFrameTime = 0.0f;
		_animationFrames.clear();
		LOGS->logEvent("Generating fluid animation", Important);
		tableWidgetPropertyEditor->setEnabled(false);
		//treeWidgetObjectInspector->setEnabled(false);
		
		_qSliderFrames->setMaximum(0);
		_qSpinBoxFrames->setMaximum(0);
		_qSliderFrames->setValue(0);
		
		_animationGenerator.setScene(*_scene);
		_animationGenerator.animate();
		
		
		
	}
	else {
		_animating = false;
		actionRecord->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/record.png")));
		LOGS->logEvent("Stopping fluid animation", Important);
		tableWidgetPropertyEditor->setEnabled(true);
		_animationGenerator.stopGenerating();
		
	}
	
}

void MainWindow::getFrame()
{
	AnimationFrame latestFrame;
	
	latestFrame = _animationGenerator.getFrame();
	
	//if ( fabs( _lastFrameTime - latestFrame.animationTime ) <= EPSILON ) {
	
	
	
	// print info for first frame
	if (latestFrame.frame == 0) {
		printAnimationFrameProperties( latestFrame );
		
	}
	
	if ( _liveSave ) {
		
		

             QHash<QString, QImage> images = viewPortWidget->getImages();

             //QHash<QString, QImage> images ;
		


		
		// XXX create directory if it does not exist
		QString destination = path + identificator +   QString("_%1").arg(latestFrame.frame, length.size(), 10, QChar('0'))  + QString(".dae");
		
        _saveThread.saveFrame(latestFrame, images, destination);
		//updateFluidGeometryOnCollada(i);
		
	}
	else {
		_animationFrames.push_back( latestFrame );
		
	}
	
	//++currentWorkingFrame;
	
	LOGS->logEvent( QString("Input time %1").arg( _lastFrameTime ) );
	//std::cout << "input time " << _lastFrameTime << std::endl;
	_lastFrameTime += ( 1.0f / PROPERTIES->getSceneProperties().geti(framesPerSecond) );
	
	
	//}
	
	//viewPortWidget->setFrame(_animationFrames[ _animationFrames.size()-1 ]);
	viewPortWidget->setFrame(latestFrame);
	
	PROPERTIES->processingAnimation = true;
	_animationGenerator.continueWithNewFrame();
	
}

void MainWindow::setAnimation(int milliseconds)
{
	ovInt lastFrame;
	QTime totalTime(0, 0, 0);
	
	totalTime = totalTime.addMSecs( milliseconds );
	
	lastFrame = _animationFrames.size()-1;
	
	LOGS->logEvent(QString("Genrated animation in %1 ").arg( totalTime.toString("hh:mm:ss.zzz") ), Important);
	
	actionRecord->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/record.png")));
	_qSliderFrames->setMaximum(lastFrame);
	_qSpinBoxFrames->setMaximum(lastFrame);
	
	// set the animation to the first frame
	_qSliderFrames->setValue(0);
	updateQSpinBoxFrames();
	
	
}

void MainWindow::setNextFrame()
{
	ovInt currentFrame = _qSliderFrames->value();
	currentFrame = (currentFrame++) % (_animationFrames.size()-1);
	_qSliderFrames->setValue(currentFrame);
	updateQSpinBoxFrames();
	viewPortWidget->setFrame(_animationFrames[_qSliderFrames->value()]);
	
}

void MainWindow::playAnimation()
{
	if ( ! _animationFrames.empty() ) {
		if ( _playingAnimation == false ) {
			_qFrameTimer->start(1000.0f / PROPERTIES->getSceneProperties().geti(framesPerSecond) );	
			actionPlay->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/pause.png")));
		}
		else {
			_qFrameTimer->stop();
			actionPlay->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/play.png")));
		}
		_playingAnimation = !_playingAnimation;
	}
}

void MainWindow::clearData()
{
	_animating = false;
	_playingAnimation = false;
	actionPlay->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/play.png")));
	actionRecord->setIcon(QIcon(QString::fromUtf8(":/img/icons/icons/record.png")));
	_animationFrames.clear();
	_qSliderFrames->setMaximum(0);
	_qSpinBoxFrames->setMaximum(0);
	_qSliderFrames->setValue(0);
	updateQSpinBoxFrames();
	
}

void MainWindow::showLastFrame()
{
	if ( ! _animationFrames.empty() ) {
		_qSliderFrames->setValue(_animationFrames.size()-1);
		updateQSpinBoxFrames();
	}
}

void MainWindow::showFirstFrame()
{
	_qSliderFrames->setValue(0);
	updateQSpinBoxFrames();
}

void MainWindow::displayErrorMessage(QString message)
{
	QMessageBox msgBox(QMessageBox::Warning, "Warning",
					   message, 0, this);
	msgBox.addButton(tr("OK"), QMessageBox::AcceptRole);
	msgBox.exec();
	
	
}

void MainWindow::updateFluidGeometryOnCollada(ovInt frameNumber)
{
#if 0
	domNode *node;
	//domInstance_geometry *instanceGeometry;
	//domLibrary_visual_scenes_Array libVisScenes;
	domCOLLADA *domRoot;
	
	
	
	// for all objects
	for (int i=0; i < _scene->directory.getSize(); i++) {
		
		// check if fluid
		
		
		if (_scene->directory[i].nodeType == GeometricObjectNode &&
			PROPERTIES->getObjectProperties(_scene->directory[i].glName)->getObjectType() == Fluid &&
			_animationFrames[frameNumber].listFluids.size() ) {
			
			
			// get fluid geometry
			_daeObject->getDatabase()->getElement((daeElement**)&node, 
												  0, _scene->directory[i].id.toAscii(), COLLADA_ELEMENT_NODE);
			
			// get visual scene array
			
			// remove node transformations if they exist since they are already
			// taken into account
			domTranslate_Array tranAry = node->getTranslate_array();
			for(ovUInt j = 0; j < tranAry.getCount(); j++) {
				daeElement::removeFromParent( tranAry[j] );
				//tranAry.setCount( tranAry.getCount() - 1 );
			}
			
			domRotate_Array rotAry = node->getRotate_array();
			for(ovUInt j = 0; j < rotAry.getCount(); j++) {
				daeElement::removeFromParent( rotAry[j] );
				//rotAry.setCount( rotAry.getCount() - 1 );
			}
			
			domSkew_Array skeAry = node->getSkew_array();
			for(ovUInt j = 0; j < skeAry.getCount(); j++) {
				daeElement::removeFromParent( skeAry[j] );
				//skeAry.setCount( skeAry.getCount() - 1 );
			}
			
			// get root
			domRoot = _daeObject->getDom(_daeFileName.toAscii());
			
			
			// delete current geometry and add current instance
			domInstance_geometry_Array instanceGeometryArray = node->getInstance_geometry_array();
			
			//get the first ID 
			QString newID = instanceGeometryArray[0]->getUrl().getID();
			
			// get material
			domGeometry *tempGeometry;
			
			
			_daeObject->getDatabase()->getElement((daeElement**)&tempGeometry, 
												  0, newID.toAscii(), COLLADA_ELEMENT_GEOMETRY);
			
			// for now we use the first usable material we encounter
			
			//XXX this thing breaks sometimes
			//LOGS->logEvent(QString("this newid %1").arg( newID ));
			//LOGS->logEvent(QString("instance geom array count %1").arg( instanceGeometryArray.getCount() ));
			QString materialEasy;
			
			
			if (tempGeometry->getMesh()->getPolylist_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolylist_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getPolygons_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolygons_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getTriangles_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getTriangles_array()[0]->getMaterial();
			}
			
			
			// remove old geometries of current node
			
			
			for (ovUInt j=0 ; j < instanceGeometryArray.getCount(); j++) {
				_daeObject->getDatabase()->getElement((daeElement**)&tempGeometry, 
													  0, instanceGeometryArray[j]->getUrl().getID(), COLLADA_ELEMENT_GEOMETRY);
				
				daeBool removed = daeElement::removeFromParent( tempGeometry );
				
				
				//LOGS->logEvent(QString("erasing %1").arg( instanceGeometryArray[j]->getUrl().getID() ));
			}
			
			
			
			domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
			
			// add new geometry  for the node
			
			//domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
			
			domGeometry *geo = daeSafeCast<domGeometry>( libGeos[0]->createAndPlace( COLLADA_ELEMENT_GEOMETRY ) );
			
			geo->setId( newID.toAscii() );
			domMesh *mesh = daeSafeCast<domMesh>( geo->createAndPlace( COLLADA_ELEMENT_MESH ) );
			
			////////////////////////////
			
			// the following code is based / taken from the COLLADA export sample program
			
			//we will need 3 sources for this mesh. positions, normals, and UVs
			domSource *positionSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *normalSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *uvSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			
			//create the positions source.
			std::string srcName = _scene->directory[i].id.toStdString () + std::string("-positions");
			positionSrc->setId( srcName.c_str() );
			domFloat_array *fa = daeSafeCast<domFloat_array>( positionSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			std::string arrayName = srcName + std::string("-array");
			fa->setId( arrayName.c_str() );
			
			
			
			
			ovInt workIndex = 0;
			for (ovUInt p=0; p<_animationFrames[frameNumber].listGlNames.size(); p++) {
				if (_animationFrames[frameNumber].listGlNames[p] == _scene->directory[i].glName) {
					// we found the geometry corresponding to the node
					workIndex = p;
					break;
				}
			}
			
			//if (_animationFrames[frameNumber].listFluids.size()) {
			fa->setCount( _animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount * 3 );
			
			domListOfFloats &posSrcArray = fa->getValue();
			
			for (ovInt p=0; p < _animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount; p++) {
				posSrcArray.append3( _animationFrames[frameNumber].listFluids[workIndex].sourcePosition[p].x ,
									_animationFrames[frameNumber].listFluids[workIndex].sourcePosition[p].y ,
									_animationFrames[frameNumber].listFluids[workIndex].sourcePosition[p].z 
									
									);
			}
			
			//create the accessor
			domSource::domTechnique_common *srcTeqC = daeSafeCast<domSource::domTechnique_common>( positionSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			acc->setCount( _animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount );
			acc->setStride( 3 );
			
			
			daeURI uri;
			uri.setElement( fa );
			uri.resolveURI();
			acc->setSource( uri );
			// vvv
			domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "X" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Y" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Z" );
			param->setType( "float" );
			
			//create the normals source.
			srcName = _scene->directory[i].id.toStdString() + std::string("-normals");
			normalSrc->setId( srcName.c_str() );
			fa = daeSafeCast<domFloat_array>( normalSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			arrayName = srcName + std::string("-array");
			fa->setId( arrayName.c_str() );
			fa->setCount( _animationFrames[frameNumber].listFluids[workIndex].sourceNormalCount * 3 );
			
			domListOfFloats &normalSrcArray = fa->getValue();
			
			for (ovInt p=0; p < _animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount; p++) {
				
				normalSrcArray.append3( _animationFrames[frameNumber].listFluids[workIndex].sourceNormal[p].x , 
									   _animationFrames[frameNumber].listFluids[workIndex].sourceNormal[p].y ,
									   _animationFrames[frameNumber].listFluids[workIndex].sourceNormal[p].z 
									   );
			}
			
			//create the accessor
			srcTeqC = daeSafeCast<domSource::domTechnique_common>( normalSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			acc->setCount( _animationFrames[frameNumber].listFluids[workIndex].sourceNormalCount );
			acc->setStride( 3 );
			uri.setElement( fa );
			uri.resolveURI();
			acc->setSource( uri );
			
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "X" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Y" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Z" );
			param->setType( "float" );
			
			
			/*
			 //create the uvs source.
			 srcName = _scene->directory[i].id.toStdString() + std::string("-uvs");
			 uvSrc->setId( srcName.c_str() );
			 fa = daeSafeCast<domFloat_array>( uvSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			 arrayName = srcName + std::string("-array");
			 fa->setId( arrayName.c_str() );
			 fa->setCount( 8 );
			 
			 domListOfFloats &uvSrcArray = fa->getValue();
			 uvSrcArray.append2( 0, 0 ); //uv 0,0
			 uvSrcArray.append2( 0, 1 ); //uv 0,1
			 uvSrcArray.append2( 1, 0 ); //uv 1,0
			 uvSrcArray.append2( 1, 1 ); //uv 1,1
			 
			 //create the accessor
			 srcTeqC = daeSafeCast<domSource::domTechnique_common>( uvSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			 acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			 acc->setCount( 4 );
			 acc->setStride( 2 );
			 uri.setElement( fa );
			 uri.resolveURI();
			 acc->setSource( uri );
			 
			 param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			 param->setName( "S" );
			 param->setType( "float" );
			 param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			 param->setName( "T" );
			 param->setType( "float" );
			 */
			//create the vertices 
			domVertices *verts = daeSafeCast<domVertices>( mesh->createAndPlace( COLLADA_ELEMENT_VERTICES ) );
			srcName = _scene->directory[i].id.toStdString() + std::string("-vertices");
			
			verts->setId( srcName.c_str() );
			
			domInputLocal *inputLocal = daeSafeCast<domInputLocal>( verts->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			
			inputLocal->setSemantic( COMMON_PROFILE_INPUT_POSITION );
			uri.setElement( positionSrc );
			uri.resolveURI();
			inputLocal->setSource( uri );
			
			
			
			
			domPolylist *plist = daeSafeCast<domPolylist>( mesh->createAndPlace( COLLADA_ELEMENT_POLYLIST ) );
			plist->setCount( _animationFrames[frameNumber].listFluids[workIndex].triangleCount );
			plist->setMaterial( materialEasy.toAscii() );
			
			domInputLocalOffset *ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			ilo->setSemantic( COMMON_PROFILE_INPUT_VERTEX );
			ilo->setOffset( 0 );
			uri.setElement( verts );
			uri.resolveURI();
			ilo->setSource( uri );
			ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			ilo->setSemantic( COMMON_PROFILE_INPUT_NORMAL );
			ilo->setOffset( 1 );
			uri.setElement( normalSrc );
			uri.resolveURI();
			ilo->setSource( uri );
			/*
			 ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			 ilo->setSemantic( COMMON_PROFILE_INPUT_TEXCOORD );
			 ilo->setOffset( 2 );
			 ilo->setSet( 0 );
			 uri.setElement( uvSrc );
			 uri.resolveURI();
			 ilo->setSource( uri );
			 */
			domPolylist::domVcount *vcount = daeSafeCast<domPolylist::domVcount>( plist->createAndPlace( COLLADA_ELEMENT_VCOUNT ) );
			//each number in the vcount element represents how many vertices a polygon has. Here we deal only with triangles
			
			for (ovInt p=0; p < _animationFrames[frameNumber].listFluids[workIndex].triangleCount; p++) {
				vcount->getValue().append(3);
			}
			
			
			//each set of three is one number per input-offset. for this example it's vert, normal, uv.
			domP *q = daeSafeCast<domP>( plist->createAndPlace( COLLADA_ELEMENT_P ) );
			domListOfUInts &indices = q->getValue();
			
			//_animationFrames[frameNumber].listFluids[workIndex].
			
			for (ovInt p=_animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount-1; p >=0 ; p--) {
				//for (ovInt p=0; p < _animationFrames[frameNumber].listFluids[workIndex].sourcePositionCount ; p++) {
				indices.append2( p, p );
			}
			
			////////////////////////////
			// this marks the end of the code taken from the COLLADA export sample program
			
			// add bubbles if needed
			
			
			if ( PROPERTIES->getObjectProperties(_scene->directory[i].glName)->getb(bubblyFluid) ) {
				
				
				// check for bubbles //XXX not finished !!!
				ovUInt bubbleCount;
				QString bubbleID;
				QString nodeID;
				domNode *newNode;
				
				
				
				
				
				// this sort of works
				//domVisual_scene *visScene = daeSafeCast<domVisual_scene>( libVisScenes->createAndPlace( COLLADA_ELEMENT_VISUAL_SCENE ) );
				domVisual_scene *visScene;
				
				_daeObject->getDatabase()->getElement((daeElement**)&visScene, 
													  0, NULL, COLLADA_ELEMENT_VISUAL_SCENE);
				
				
				// remove past bubbles if such exist
				
				ovInt currentNodeCount = _daeObject->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
				
				// this is ok
				
				for (ovInt p=0; p < currentNodeCount; p++) {
					_daeObject->getDatabase()->getElement((daeElement**)&newNode, 
														  p, NULL, COLLADA_ELEMENT_NODE);
					
					nodeID = newNode->getId();
					
					if ( nodeID.contains("bubbleInstance_") ) {
						daeElement::removeFromParent(newNode);
						currentNodeCount = _daeObject->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
						p=0;
						
					}
					
				}
				
				// ugly code, will fix later. I promise
				// get the new bubble shape
				
				QString bubbleFile;
				domGeometry* bubbleShape;
				
				_daeObject->getDatabase()->getElement((daeElement**) &bubbleShape, 0,
													  "OvensUniqueBubbleShape", COLLADA_ELEMENT_GEOMETRY );
				
				
				/*		
				 //bubbleFile = getFileName("Open bubble description");
				 bubbleFile = "C:\Documents and Settings\orodrigu\My Documents\dae\bubble.dae";
				 
				 bubbleFile.replace("\\", "/");
				 bubbleFile = "file:///" + bubbleFile;
				 
				 
				 _daeObject->load( bubbleFile.toAscii() );
				 
				 
				 _daeObject->getDatabase()->getElement((daeElement**) &bubbleShape, 0,
				 NULL, COLLADA_ELEMENT_GEOMETRY,
				 bubbleFile.toAscii() );
				 
				 //bubbleShape->setId("OvensUniqueBubbleShape"); //XXX later
				 
				 _daeObject->saveAs( saveDestination.toAscii(), bubbleFile.toAscii() );
				 
				 
				 
				 //domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
				 //libGeos[0]->placeElement(bubbleShape);
				 
				 _daeObject->unload( bubbleFile.toAscii() );
				 
				 */
				
				////
				
				bubbleCount = _animationFrames[frameNumber].listBubblesCount[workIndex];
				domTranslate *trans;
				domScale     *scale;
				domInstance_geometry *instanceGeom;
				QString geoPath;
				daeURI bubbleURI;
				domBind_material *bindMat;
				domBind_material::domTechnique_common *bmtc;
				domInstance_material *instMat;
				
				//HACK
				if (stillInside.size() != bubbleCount) {
					LOGS->logEvent("stillInside just once");
					for (ovUInt pp=stillInside.size(); pp< bubbleCount; pp++) {
						stillInside.push_back(true);
					}
				}
				
				for (ovUInt p=0; p<bubbleCount; p++) {
					
					
					//bool pointIsInside(ovTuple3f& newPos, Node& node)
					//Node test;
					//test = _scene->directory[i];
					//test.instanceGeometries[j].geometry->objects[0].
					//_animationFrames[i].listFluids[j];
					
					
					if (stillInside[p] && 
						pointIsInside(_animationFrames[frameNumber].listBubbles[workIndex][p].pos, 
										 &(_animationFrames[i].listFluids[workIndex])) ) {
						
						bubbleID = QString("bubbleInstance_%1").arg(p);
						
						
						
						//newNode = daeSafeCast<domNode>( visScene->createAndPlace( COLLADA_ELEMENT_NODE ) );
						newNode = daeSafeCast<domNode>( node->createAndPlace( COLLADA_ELEMENT_NODE ) );
						newNode->setId( bubbleID.toAscii() );
						
						trans = daeSafeCast<domTranslate>( newNode->createAndPlace( COLLADA_ELEMENT_TRANSLATE ) );
						trans->setSid( "translate" );
						
						trans->getValue().set3( _animationFrames[frameNumber].listBubbles[workIndex][p].pos.x, 
											   _animationFrames[frameNumber].listBubbles[workIndex][p].pos.y, 
											   _animationFrames[frameNumber].listBubbles[workIndex][p].pos.z);
						
						// set bubble size adjustment
						scale = daeSafeCast<domScale>( newNode->createAndPlace( COLLADA_ELEMENT_SCALE ) );
						scale->setSid( "scale" );
						
						// XXX W CHANGE ovTuple4f
						ovFloat scaleVar = _animationFrames[frameNumber].listBubbles[workIndex][p].size;
						
						
						scale->getValue().set3( scaleVar, scaleVar, scaleVar);
						
						geoPath = /*saveDestination +*/ QString("#OvensUniqueBubbleShape");
						
						instanceGeom = daeSafeCast<domInstance_geometry>( newNode->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
						bubbleURI.setURI(geoPath.toAscii());
						instanceGeom->setUrl( bubbleURI );
						
						// instance geom
						/*
						 domInstance_geometry *instGeo = daeSafeCast<domInstance_geometry>( node->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
						 uri.setElement( geo );
						 uri.resolveURI();
						 instGeo->setUrl( uri );
						 */
						
						// material
						daeURI uri;
						instanceGeometryArray = node->getInstance_geometry_array();
						
						
						
						//domLibrary_materials	 *libMats	   = daeSafeCast<domLibrary_materials>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_MATERIALS ) );
						//domLibrary_effects		 *libEffects   = daeSafeCast<domLibrary_effects>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_EFFECTS ) );
						
						//domEffect *effect = daeSafeCast<domEffect>( libEffects->createAndPlace( COLLADA_ELEMENT_EFFECT ) );
						//effect->setId( "simple_texture-fx" );
						
						//domMaterial *material = daeSafeCast<domMaterial>( libMats->createAndPlace( COLLADA_ELEMENT_MATERIAL ) );
						//material->setId( "texture_material" );  // ESTE
						
						//domInstance_effect *instEf = daeSafeCast<domInstance_effect>( material->createAndPlace( COLLADA_ELEMENT_INSTANCE_EFFECT ) );
						//uri.setElement( effect );
						//uri.resolveURI();
						//instEf->setUrl( uri );
						
						
						bindMat = daeSafeCast<domBind_material>( instanceGeom->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
						bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
						instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
						//uri.setElement( material ); //este
						//uri.resolveURI();
						
						// XXX tempo, currently only sets one bubble
						bubbleShape->getMesh()->getPolylist_array()[0]->setMaterial(instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol());
						
						instMat->setSymbol( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol() ); // ESTE
						instMat->setTarget( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getTarget() );
						
						/*
						 
						 domBind_material *bindMat = daeSafeCast<domBind_material>( instanceGeom->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
						 domBind_material::domTechnique_common *bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
						 domInstance_material *instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
						 uri.setElement( material ); //este
						 uri.resolveURI();
						 instMat->setSymbol( "material" ); // ESTE
						 instMat->setTarget( uri );
						 */
						
						/*
						 domBind_material *bindMat = daeSafeCast<domBind_material>( instGeo->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
						 domBind_material::domTechnique_common *bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
						 domInstance_material *instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
						 uri.setElement( material );
						 uri.resolveURI();
						 instMat->setSymbol( materialSymbol );
						 instMat->setTarget( uri );
						 */
						
						/*
						 domInstance_material::domBind_vertex_input *bvi = daeSafeCast<domInstance_material::domBind_vertex_input>( instMat->createAndPlace( COLLADA_ELEMENT_BIND_VERTEX_INPUT ) );
						 bvi->setSemantic( texCoordSymbol );
						 bvi->setInput_semantic( COMMON_PROFILE_INPUT_TEXCOORD );
						 bvi->setInput_set( 0 );
						 */
					}
					else {
						// HACK
						if (p < stillInside.size()) {
							stillInside[p] = false;
						}
					}
					
					
				}
				
			}
		}
		
	}
#endif
}


QString MainWindow::getDAEPath()
{
	QString fileName = _daeFileName.section('/', -1);
	QString path = _daeFileName;
	path.remove( fileName );
	path += "/";
	
	QString identificator = fileName;
	identificator.remove(".dae");
	path += identificator + QString("_animation") + QString("/"); 
	
	return path;
	
}

QString MainWindow::getSystemPath()
{
	QString systemPath = getDAEPath();
	systemPath.remove("file:///");
#ifdef Q_OS_WIN
	systemPath.replace('/', '\\');
#endif
	return systemPath;
	
}

void MainWindow::setLiveSave()
{
	_liveSave = !_liveSave;
	
}



/////////////



void SaveThread::updateFluidGeometryOnCollada()
{
#if 1
	domNode *node;
	//domInstance_geometry *instanceGeometry;
	//domLibrary_visual_scenes_Array libVisScenes;
	domCOLLADA *domRoot;
	
	
	
	// for all objects
	for (int i=0; i < _scene.directory.getSize(); i++) {
		
		// check if fluid
		
		
		if (_scene.directory[i].nodeType == GeometricObjectNode &&
			PROPERTIES->getObjectProperties(_scene.directory[i].glName)->getObjectType() == Fluid &&
			_workingAnimationFrame.listFluids.size() ) {
			
			
			// get fluid geometry
			_daeObject->getDatabase()->getElement((daeElement**)&node, 
												  0, _scene.directory[i].id.toAscii(), COLLADA_ELEMENT_NODE);
			
			// get visual scene array
			
			// remove node transformations if they exist since they are already
			// taken into account
			domTranslate_Array tranAry = node->getTranslate_array();
			for(ovUInt j = 0; j < tranAry.getCount(); j++) {
				daeElement::removeFromParent( tranAry[j] );
				//tranAry.setCount( tranAry.getCount() - 1 );
			}
			
			domRotate_Array rotAry = node->getRotate_array();
			for(ovUInt j = 0; j < rotAry.getCount(); j++) {
				daeElement::removeFromParent( rotAry[j] );
				//rotAry.setCount( rotAry.getCount() - 1 );
			}
			
			domSkew_Array skeAry = node->getSkew_array();
			for(ovUInt j = 0; j < skeAry.getCount(); j++) {
				daeElement::removeFromParent( skeAry[j] );
				//skeAry.setCount( skeAry.getCount() - 1 );
			}
			
			// get root
			domRoot = _daeObject->getDom(_daeFileName.toAscii());
			
			
			// delete current geometry and add current instance
			domInstance_geometry_Array instanceGeometryArray = node->getInstance_geometry_array();
			
			//get the first ID 
			QString newID = instanceGeometryArray[0]->getUrl().getID();
			
			// get material
			domGeometry *tempGeometry;
			
			
			_daeObject->getDatabase()->getElement((daeElement**)&tempGeometry, 
												  0, newID.toAscii(), COLLADA_ELEMENT_GEOMETRY);
			
			// for now we use the first usable material we encounter
			
			//XXX this thing breaks sometimes
			//LOGS->logEvent(QString("this newid %1").arg( newID ));
			//LOGS->logEvent(QString("instance geom array count %1").arg( instanceGeometryArray.getCount() ));
			QString materialEasy;
			
			
			if (tempGeometry->getMesh()->getPolylist_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolylist_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getPolygons_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolygons_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getTriangles_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getTriangles_array()[0]->getMaterial();
			}
			
			
			// remove old geometries of current node
			
			
			for (ovUInt j=0 ; j < instanceGeometryArray.getCount(); j++) {
				_daeObject->getDatabase()->getElement((daeElement**)&tempGeometry, 
													  0, instanceGeometryArray[j]->getUrl().getID(), COLLADA_ELEMENT_GEOMETRY);
				
				daeBool removed = daeElement::removeFromParent( tempGeometry );
				
				
				//LOGS->logEvent(QString("erasing %1").arg( instanceGeometryArray[j]->getUrl().getID() ));
			}
			
			
			
			domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
			
			// add new geometry  for the node
			
			//domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
			
			domGeometry *geo = daeSafeCast<domGeometry>( libGeos[0]->createAndPlace( COLLADA_ELEMENT_GEOMETRY ) );
			
			geo->setId( newID.toAscii() );
			domMesh *mesh = daeSafeCast<domMesh>( geo->createAndPlace( COLLADA_ELEMENT_MESH ) );
			
			////////////////////////////
			
			// the following code is based / taken from the COLLADA export sample program
			
			//we will need 3 sources for this mesh. positions, normals, and UVs
			domSource *positionSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *normalSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *uvSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			
			//create the positions source.
			std::string srcName = _scene.directory[i].id.toStdString () + std::string("-positions");
			positionSrc->setId( srcName.c_str() );
			domFloat_array *fa = daeSafeCast<domFloat_array>( positionSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			std::string arrayName = srcName + std::string("-array");
			fa->setId( arrayName.c_str() );
			
			
			
			
			ovInt workIndex = 0;
			for (ovUInt p=0; p<_workingAnimationFrame.listGlNames.size(); p++) {
				if (_workingAnimationFrame.listGlNames[p] == _scene.directory[i].glName) {
					// we found the geometry corresponding to the node
					workIndex = p;
					break;
				}
			}
			
			//if (_workingAnimationFrame.listFluids.size()) {
			fa->setCount( _workingAnimationFrame.listFluids[workIndex].sourcePositionCount * 3 );
			
			domListOfFloats &posSrcArray = fa->getValue();
			
			for (ovInt p=0; p < _workingAnimationFrame.listFluids[workIndex].sourcePositionCount; p++) {
				posSrcArray.append3( _workingAnimationFrame.listFluids[workIndex].sourcePosition[p].x ,
									_workingAnimationFrame.listFluids[workIndex].sourcePosition[p].y ,
									_workingAnimationFrame.listFluids[workIndex].sourcePosition[p].z 
									
									);
			}
			
			//create the accessor
			domSource::domTechnique_common *srcTeqC = daeSafeCast<domSource::domTechnique_common>( positionSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			acc->setCount( _workingAnimationFrame.listFluids[workIndex].sourcePositionCount );
			acc->setStride( 3 );
			daeURI uri(*fa, "");
			//uri.setElement( fa );
			//uri.resolveURI();
			acc->setSource( uri );
			
			domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "X" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Y" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Z" );
			param->setType( "float" );
			
			//create the normals source.
			srcName = _scene.directory[i].id.toStdString() + std::string("-normals");
			normalSrc->setId( srcName.c_str() );
			fa = daeSafeCast<domFloat_array>( normalSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			arrayName = srcName + std::string("-array");
			fa->setId( arrayName.c_str() );
			fa->setCount( _workingAnimationFrame.listFluids[workIndex].sourceNormalCount * 3 );
			
			domListOfFloats &normalSrcArray = fa->getValue();
			
			for (ovInt p=0; p < _workingAnimationFrame.listFluids[workIndex].sourcePositionCount; p++) {
				
				normalSrcArray.append3( _workingAnimationFrame.listFluids[workIndex].sourceNormal[p].x , 
									   _workingAnimationFrame.listFluids[workIndex].sourceNormal[p].y ,
									   _workingAnimationFrame.listFluids[workIndex].sourceNormal[p].z 
									   );
			}
			
			//create the accessor
			srcTeqC = daeSafeCast<domSource::domTechnique_common>( normalSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			acc->setCount( _workingAnimationFrame.listFluids[workIndex].sourceNormalCount );
			acc->setStride( 3 );
			uri = daeURI(*fa, "");
			//uri.setElement( fa );
			//uri.resolveURI();
			acc->setSource( uri );
			
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "X" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Y" );
			param->setType( "float" );
			param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			param->setName( "Z" );
			param->setType( "float" );
			
			
			/*
			 //create the uvs source.
			 srcName = _scene.directory[i].id.toStdString() + std::string("-uvs");
			 uvSrc->setId( srcName.c_str() );
			 fa = daeSafeCast<domFloat_array>( uvSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
			 arrayName = srcName + std::string("-array");
			 fa->setId( arrayName.c_str() );
			 fa->setCount( 8 );
			 
			 domListOfFloats &uvSrcArray = fa->getValue();
			 uvSrcArray.append2( 0, 0 ); //uv 0,0
			 uvSrcArray.append2( 0, 1 ); //uv 0,1
			 uvSrcArray.append2( 1, 0 ); //uv 1,0
			 uvSrcArray.append2( 1, 1 ); //uv 1,1
			 
			 //create the accessor
			 srcTeqC = daeSafeCast<domSource::domTechnique_common>( uvSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			 acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
			 acc->setCount( 4 );
			 acc->setStride( 2 );
			 uri.setElement( fa );
			 uri.resolveURI();
			 acc->setSource( uri );
			 
			 param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			 param->setName( "S" );
			 param->setType( "float" );
			 param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
			 param->setName( "T" );
			 param->setType( "float" );
			 */
			//create the vertices 
			domVertices *verts = daeSafeCast<domVertices>( mesh->createAndPlace( COLLADA_ELEMENT_VERTICES ) );
			srcName = _scene.directory[i].id.toStdString() + std::string("-vertices");
			
			verts->setId( srcName.c_str() );
			
			domInputLocal *inputLocal = daeSafeCast<domInputLocal>( verts->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			
			inputLocal->setSemantic( COMMON_PROFILE_INPUT_POSITION );
			uri = daeURI(*positionSrc, "");
			//uri.setElement( positionSrc );
			//uri.resolveURI();
			inputLocal->setSource( uri );
			
			
			
			
			domPolylist *plist = daeSafeCast<domPolylist>( mesh->createAndPlace( COLLADA_ELEMENT_POLYLIST ) );
			plist->setCount( _workingAnimationFrame.listFluids[workIndex].triangleCount );
			plist->setMaterial( materialEasy.toAscii() );
			
			domInputLocalOffset *ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			ilo->setSemantic( COMMON_PROFILE_INPUT_VERTEX );
			ilo->setOffset( 0 );
			uri = daeURI(*verts, "");
			//uri.setElement( verts );
			//uri.resolveURI();
			ilo->setSource( uri );
			ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			ilo->setSemantic( COMMON_PROFILE_INPUT_NORMAL );
			ilo->setOffset( 1 );
			uri = daeURI(*normalSrc, "");
			//uri.setElement( normalSrc );
			//uri.resolveURI();
			ilo->setSource( uri );
			/*
			 ilo = daeSafeCast<domInputLocalOffset>( plist->createAndPlace( COLLADA_ELEMENT_INPUT ) );
			 ilo->setSemantic( COMMON_PROFILE_INPUT_TEXCOORD );
			 ilo->setOffset( 2 );
			 ilo->setSet( 0 );
			 uri.setElement( uvSrc );
			 uri.resolveURI();
			 ilo->setSource( uri );
			 */
			domPolylist::domVcount *vcount = daeSafeCast<domPolylist::domVcount>( plist->createAndPlace( COLLADA_ELEMENT_VCOUNT ) );
			//each number in the vcount element represents how many vertices a polygon has. Here we deal only with triangles
			
			for (ovInt p=0; p < _workingAnimationFrame.listFluids[workIndex].triangleCount; p++) {
				vcount->getValue().append(3);
			}
			
			
			//each set of three is one number per input-offset. for this example it's vert, normal, uv.
			domP *q = daeSafeCast<domP>( plist->createAndPlace( COLLADA_ELEMENT_P ) );
			domListOfUInts &indices = q->getValue();
			
			//_workingAnimationFrame.listFluids[workIndex].
			
			for (ovInt p=_workingAnimationFrame.listFluids[workIndex].sourcePositionCount-1; p >=0 ; p--) {
				//for (ovInt p=0; p < _workingAnimationFrame.listFluids[workIndex].sourcePositionCount ; p++) {
				indices.append2( p, p );
			}
			
			////////////////////////////
			// this marks the end of the code taken from the COLLADA export sample program
			
			// add bubbles if needed
			
			
			if ( PROPERTIES->getObjectProperties(_scene.directory[i].glName)->getb(bubblyFluid) ) {
				
				
				// check for bubbles //XXX not finished !!!
				ovInt bubbleCount;
				QString bubbleID;
				QString nodeID;
				domNode *newNode;
				
				
				
				
				
				// this sort of works
				//domVisual_scene *visScene = daeSafeCast<domVisual_scene>( libVisScenes->createAndPlace( COLLADA_ELEMENT_VISUAL_SCENE ) );
				domVisual_scene *visScene;
				
				_daeObject->getDatabase()->getElement((daeElement**)&visScene, 
													  0, NULL, COLLADA_ELEMENT_VISUAL_SCENE);
				
				
				// remove past bubbles if such exist
				
				ovInt currentNodeCount = _daeObject->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
				
				// this is ok
				
				for (ovInt p=0; p < currentNodeCount; p++) {
					_daeObject->getDatabase()->getElement((daeElement**)&newNode, 
														  p, NULL, COLLADA_ELEMENT_NODE);
					
					nodeID = newNode->getId();
					
					if ( nodeID.contains("bubbleInstance_") ) {
						daeElement::removeFromParent(newNode);
						currentNodeCount = _daeObject->getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
						p=0;
						
					}
					
				}
				
				// ugly code, will fix later. I promise
				// get the new bubble shape
				
				QString bubbleFile;
				domGeometry* bubbleShape;
				
				_daeObject->getDatabase()->getElement((daeElement**) &bubbleShape, 0,
													  "OvensUniqueBubbleShape", COLLADA_ELEMENT_GEOMETRY );
				
				
				////
				
				bubbleCount = _workingAnimationFrame.listBubblesCount[workIndex];
				domTranslate *trans;
				domScale     *scale;
				domInstance_geometry *instanceGeom;
				QString geoPath;
				//daeURI bubbleURI;
				domBind_material *bindMat;
				domBind_material::domTechnique_common *bmtc;
				domInstance_material *instMat;
				
				
				
				for (ovInt p=0; p<bubbleCount; p++) {
					
					
					
					bubbleID = QString("bubbleInstance_%1").arg(p);
					
					
					
					//newNode = daeSafeCast<domNode>( visScene->createAndPlace( COLLADA_ELEMENT_NODE ) );
					newNode = daeSafeCast<domNode>( node->createAndPlace( COLLADA_ELEMENT_NODE ) );
					newNode->setId( bubbleID.toAscii() );
					
					trans = daeSafeCast<domTranslate>( newNode->createAndPlace( COLLADA_ELEMENT_TRANSLATE ) );
					trans->setSid( "translate" );
					
					trans->getValue().set3( _workingAnimationFrame.listBubbles[workIndex][p].pos.x, 
										   _workingAnimationFrame.listBubbles[workIndex][p].pos.y, 
										   _workingAnimationFrame.listBubbles[workIndex][p].pos.z);
					
					// set bubble size adjustment
					scale = daeSafeCast<domScale>( newNode->createAndPlace( COLLADA_ELEMENT_SCALE ) );
					scale->setSid( "scale" );
					
					// XXX W CHANGE ovTuple4f
					ovFloat scaleVar = _workingAnimationFrame.listBubbles[workIndex][p].size;
					
					scale->getValue().set3( scaleVar, scaleVar, scaleVar);
					
					geoPath = /*saveDestination +*/ QString("#OvensUniqueBubbleShape");
					
					instanceGeom = daeSafeCast<domInstance_geometry>( newNode->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
					daeURI bubbleURI(*instanceGeom, geoPath.toStdString());
					//daeURI bubbleURI(geoPath.toStdString());
					//bubbleURI.setURI(geoPath.toAscii());
					instanceGeom->setUrl( bubbleURI );
					
					// instance geom
					/*
					 domInstance_geometry *instGeo = daeSafeCast<domInstance_geometry>( node->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
					 uri.setElement( geo );
					 uri.resolveURI();
					 instGeo->setUrl( uri );
					 */
					
					// material
					//daeURI uri;
					instanceGeometryArray = node->getInstance_geometry_array();
					
					
					
					//domLibrary_materials	 *libMats	   = daeSafeCast<domLibrary_materials>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_MATERIALS ) );
					//domLibrary_effects		 *libEffects   = daeSafeCast<domLibrary_effects>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_EFFECTS ) );
					
					//domEffect *effect = daeSafeCast<domEffect>( libEffects->createAndPlace( COLLADA_ELEMENT_EFFECT ) );
					//effect->setId( "simple_texture-fx" );
					
					//domMaterial *material = daeSafeCast<domMaterial>( libMats->createAndPlace( COLLADA_ELEMENT_MATERIAL ) );
					//material->setId( "texture_material" );  // ESTE
					
					//domInstance_effect *instEf = daeSafeCast<domInstance_effect>( material->createAndPlace( COLLADA_ELEMENT_INSTANCE_EFFECT ) );
					//uri.setElement( effect );
					//uri.resolveURI();
					//instEf->setUrl( uri );
					
					
					bindMat = daeSafeCast<domBind_material>( instanceGeom->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
					bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
					instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
					//uri.setElement( material ); //este
					//uri.resolveURI();
					
					// XXX tempo, currently only sets one bubble
					bubbleShape->getMesh()->getPolylist_array()[0]->setMaterial(instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol());
					
					instMat->setSymbol( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol() ); // ESTE
					instMat->setTarget( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getTarget() );
					
					/*
					 
					 domBind_material *bindMat = daeSafeCast<domBind_material>( instanceGeom->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
					 domBind_material::domTechnique_common *bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
					 domInstance_material *instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
					 uri.setElement( material ); //este
					 uri.resolveURI();
					 instMat->setSymbol( "material" ); // ESTE
					 instMat->setTarget( uri );
					 */
					
					/*
					 domBind_material *bindMat = daeSafeCast<domBind_material>( instGeo->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
					 domBind_material::domTechnique_common *bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
					 domInstance_material *instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
					 uri.setElement( material );
					 uri.resolveURI();
					 instMat->setSymbol( materialSymbol );
					 instMat->setTarget( uri );
					 */
					
					/*
					 domInstance_material::domBind_vertex_input *bvi = daeSafeCast<domInstance_material::domBind_vertex_input>( instMat->createAndPlace( COLLADA_ELEMENT_BIND_VERTEX_INPUT ) );
					 bvi->setSemantic( texCoordSymbol );
					 bvi->setInput_semantic( COMMON_PROFILE_INPUT_TEXCOORD );
					 bvi->setInput_set( 0 );
					 */
				}		
			}
		}
	}
#endif
}


void MainWindow::setParticleRenderingTypeGeneral(QAction *action, ParticleRenderingType type, bool toggled)
{
	if (_lastParticleActionType == action) {
		action->setChecked(true);
	}
	else if (toggled) {
		_lastParticleActionType->setChecked(false);
		_lastParticleActionType = action;
		
		viewPortWidget->setParticleRenderingType(type);
	}
	
	
}

void MainWindow::setParticleRenderingTypePosition(bool toggled)
{
	setParticleRenderingTypeGeneral(actionPosition, particlePosition, toggled);
}

void MainWindow::setParticleRenderingTypeTemperature(bool toggled)
{
	setParticleRenderingTypeGeneral(actionTemperature, particleTemperature, toggled);
}

void MainWindow::setParticleRenderingTypeSize(bool toggled)
{
	setParticleRenderingTypeGeneral(actionSize, particleSize, toggled);
}

void MainWindow::setParticleRenderingTypeCO2(bool toggled) 
{
	setParticleRenderingTypeGeneral(actionCO2, particleCO2, toggled);
}

void MainWindow::setParticleRenderingTypeOnSurface(bool toggled)
{
	setParticleRenderingTypeGeneral(actionOn_surface, particleOnSurface, toggled);
}

void MainWindow::setParticleRenderingTypeInCollision(bool toggled)
{
	setParticleRenderingTypeGeneral(actionIn_collision, particleInCollision, toggled);
}

void MainWindow::setParticleRenderingTypeTrueSurface(bool toggled)
{
	
	setParticleRenderingTypeGeneral(actionTrue_surface, particleTrueSurface, toggled);
}

void MainWindow::updateSplatRadius(double newRadius) {
	horizontalSliderSplatRadius->setValue((int)newRadius * 100);	
	viewPortWidget->setSplatRadius(newRadius);
}

void MainWindow::updateSplatRadius(int partialNewRadius) {
	
	double newRadius = (double) partialNewRadius / 100.0f;
	doubleSpinBoxSplatRadius->setValue(newRadius);
	viewPortWidget->setSplatRadius(newRadius);
	
	
}

void MainWindow::updateCameraValueWidgets()
{
    _currentCamera = viewPortWidget->getCamera();
    doubleSpinBoxPosX->setValue(_currentCamera.position.x);
    doubleSpinBoxPosY->setValue(_currentCamera.position.y);
    doubleSpinBoxPosZ->setValue(_currentCamera.position.z);
/*
    doubleSpinBoxRot_0_0->setValue(_currentCamera.rotation(0,0));
    doubleSpinBoxRot_0_1->setValue(_currentCamera.rotation(0,1));
    doubleSpinBoxRot_0_2->setValue(_currentCamera.rotation(0,2));

    doubleSpinBoxRot_1_0->setValue(_currentCamera.rotation(1,0));
    doubleSpinBoxRot_1_1->setValue(_currentCamera.rotation(1,1));
    doubleSpinBoxRot_1_2->setValue(_currentCamera.rotation(1,2));

    doubleSpinBoxRot_2_0->setValue(_currentCamera.rotation(2,0));
    doubleSpinBoxRot_2_1->setValue(_currentCamera.rotation(2,1));
    doubleSpinBoxRot_2_2->setValue(_currentCamera.rotation(2,2));
*/
}

void MainWindow::updateCameraValues(double dummyValue)
{
    _currentCamera.position.x = doubleSpinBoxPosX->value();
    _currentCamera.position.y = doubleSpinBoxPosY->value();
    _currentCamera.position.z = doubleSpinBoxPosZ->value();

    _currentCamera.rotation(0, 0) = doubleSpinBoxRot_0_0->value();
    _currentCamera.rotation(0, 1) = doubleSpinBoxRot_0_1->value();
    _currentCamera.rotation(0, 2) = doubleSpinBoxRot_0_2->value();

    _currentCamera.rotation(1, 0) = doubleSpinBoxRot_1_0->value();
    _currentCamera.rotation(1, 1) = doubleSpinBoxRot_1_1->value();
    _currentCamera.rotation(1, 2) = doubleSpinBoxRot_1_2->value();

    _currentCamera.rotation(2, 0) = doubleSpinBoxRot_2_0->value();
    _currentCamera.rotation(2, 1) = doubleSpinBoxRot_2_1->value();
    _currentCamera.rotation(2, 2) = doubleSpinBoxRot_2_2->value();

    viewPortWidget->setCamera(_currentCamera);

}

