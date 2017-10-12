/*
 *  AnimationGenerator.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 11/10/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
#include <omp.h>
#include <cfloat>
#include <cmath>
#include <utility>
#include <algorithm>
#include <QTime>
#include <CGAL/bounding_box.h>


#include "SimulationProperties.h"
#include "AnimationGenerator.h"
#include "LoggingSystem.h"
#include "primitives/ovVector3D.h"

#include "simulation/PrtlFluid.h"
#include "simulation/FluidPrtl.h"
#include "simulation/SimuTriangle.h"
#include "simulation/SimuSrfcPoint3D.h"

#include <iostream>
using namespace std;

//FluidInformation AnimationFrame::information = FluidInformation();
/*
bool pointIsInside(ovTuple4f& newPos,  Polygons& polys)
{
	ovTuple3f proxy;

	proxy.x = newPos.x;
	proxy.y = newPos.y;
	proxy.z = newPos.z;

	return pointIsInside(proxy, polys);
}
*/
/*
bool pointIsInside(ovTuple3f& newPos, Polygons& polys)
{
	Node proxy;

	InstanceGeometry instanceG;
	
	instanceG.geometry = new Geometry;
	
	proxy.instanceGeometries.push_back(instanceG);
	proxy.instanceGeometries[0].geometry->objects.push_back(polys);

	//proxy.instanceGeometries
	return pointIsInside(newPos, proxy);

}
*/

 
 
AnimationFrame::AnimationFrame()
{
	frame = 0;
}

AnimationFrame::AnimationFrame(const AnimationFrame &aFrame)
{
	ParticleInstance *particles;
	ParticleInstance *bubbles;
	
	listFluids.clear();
	listParticleCount.clear();
	listGlNames.clear();
	
	frame = aFrame.frame;
	
	animationTime = aFrame.animationTime;
	listFluids = aFrame.listFluids;
	listParticleCount = aFrame.listParticleCount;
	listBubblesCount.clear();
	listBubblesCount = aFrame.listBubblesCount;
	listGlNames = aFrame.listGlNames;
	
	this->listParticles.clear();
	this->listBubbles.clear();
	
	for (ovUInt i=0; i<aFrame.listParticles.size(); i++) {
		
		particles = new ParticleInstance[ aFrame.listParticleCount[i] ];
		for (ovInt j=0; j < aFrame.listParticleCount[i]; j++) {
			particles[j] = aFrame.listParticles[i][j];
		}
		this->listParticles.push_back(particles);
		
		bubbles = new ParticleInstance[ aFrame.listBubblesCount[i] ];
		for (ovInt j=0; j < aFrame.listBubblesCount[i]; j++) {
			bubbles[j] = aFrame.listBubbles[i][j];
		}
		this->listBubbles.push_back(bubbles);
	}
	
	
}

AnimationFrame::~AnimationFrame()
{
	
	
	
	// particles
	
	for (ovUInt i=0; i < listParticles.size(); i++) {
		if (listParticles[i]){
			delete listParticles[i];
		}
	}

	listParticles.clear();
	listParticleCount.clear();
	
	// bubbles
	
	for (ovUInt i=0; i < listBubbles.size(); i++) {
		if (listBubbles[i]){
			delete listBubbles[i];
		}
	}

	listBubbles.clear();
	listBubblesCount.clear();
	
	
	// other
	
	listGlNames.clear();
	listFluids.clear();
	
	
}

ovUInt AnimationFrame::getIndexByGLName(GLint glName_) 
{
	for (ovUInt i=0; i<listGlNames.size(); i++) {
		if (listGlNames[i] == glName_) {
			return i;
		}
	}
	
	return OUT_OF_RANGE_INDEX;
	
}

/////


AnimationGenerator::AnimationGenerator()
{
	_semaphore = new QSemaphore(0); 
	_stop = false;
	_scene = NULL;
	_simuManager = NULL;
	_simuFlexApp.InitInstance();
	_animationFrameLock = new QSemaphore(0);
	
	
}

AnimationGenerator::~AnimationGenerator()
{
	if (_semaphore)
		delete _semaphore;
	
	if (_scene)
		delete _scene;
	
	if(_simuManager)
		delete _simuManager;
	
	if (_animationFrameLock)
		delete _animationFrameLock;

	_simuFlexApp.ExitInstance();
}


void AnimationGenerator::setScene(Scene scene)
{
	if (_scene != NULL) {
		//delete _scene;
	}
	_scene = new Scene;

	*_scene = scene;
	
}

void AnimationGenerator::setFrame(AnimationFrame& frame)
{
	_animationFrame = frame;
}


void AnimationGenerator::animateFluids()
{
	ovInt objectName;
	ObjectType objectType;
	CPrtlFluid* newFluid;

	ovInt currentFrame = 0;
	
	AnimationFrame af, af2, af3;

	if (_stop) {
		return;
	}
	_stopAnimating = false;
	if (_simuManager) {
		delete _simuManager;
	}

	//cutOuts.clear();
	
	_simuManager = new CSimuManager;

	// mesher options
	//CSimuManager::CREATE_SURFACE_MESH    = PROPERTIES->getSceneProperties().getb( renderSurface );
	CSimuManager::USE_PRTL_NUM_DENSITY   = PROPERTIES->getSceneProperties().getb( useParticleNumberDensity );
	CSimuManager::m_isoDensityRatio	     = PROPERTIES->getSceneProperties().getf( isoDensityRatio );
	CSimuManager::m_isoDenRadiusRatio    = PROPERTIES->getSceneProperties().getf( isoDensityRadiusRatio );
	CSimuManager::m_meshSizeOverPrtlDist = PROPERTIES->getSceneProperties().getf( meshGridSizeOverParticleDistance );
	CSimuManager::m_numFramesPerSecond   = PROPERTIES->getSceneProperties().geti( framesPerSecond );
	CSimuManager::m_numStepDivPerFrame   = PROPERTIES->getSceneProperties().geti( stepsPerFrame );
	CSimuManager::m_prtlIniTemperature   = PROPERTIES->getSceneProperties().getf( minTemp );
	CSimuManager::m_minTemperature		 = PROPERTIES->getSceneProperties().getf( minTemp );
	CSimuManager::m_maxTemperature		 = PROPERTIES->getSceneProperties().getf( maxTemp );
	CSimuManager::m_startCookTime		 = PROPERTIES->getSceneProperties().getf( startCookTime );
	//CSimuManager::m_subThermalSteps;   // add to gui
	//CSimuManager::m_heatConductivity;  // add to gui
	//CSimuManager::m_heatConductivity = 500; // change per fluid
	CSimuManager::BAKE_FLUIDS			= PROPERTIES->getSceneProperties().getb( performBaking );
	CSimuManager::m_numFluidBubbleInteractionFrequency = PROPERTIES->getSceneProperties().geti( liquidBubbleInteractionFrequency );
	//CSimuManager::GENERATE_CO2 = true;
	CSimuManager::AMBIENT_HEAT_TRANSFER = PROPERTIES->getSceneProperties().getb( ambientHeatTransfer );
	CSimuManager::HEAT_TRANSFER = false;

	CSimuManager::m_bStickyInterFluids = false; //true;
	CSimuManager::m_onlyGravityTime = 0;

	//CSimuManager::m_prtlPosJitter = 0.01;
	//CSimuManager::m_numStepDivPerFrame = 3;
	//CSimuManager::m_heatConductivity = 1000;
	
	//CSimuManager::m_framesPerResample = 2;
	//CSimuManager::m_numStepDivPerFrame = 3;
	//CSimuManager::m_maxShearModulus = pow(10.0f, 6);
	//CSimuManager::m_minShearModulus = pow(10.0f, 3);
	
	//CSimuManager::m_prtlMass = 100;
	//CSimuManager::m_prtlDensity = 0.1;

    CSimuManager::m_interFluidDamping = 0.1;
	CSimuManager::m_rotationFactor	= 1;
	_simuManager->m_dbAnimationClock = 0;
	_simuManager->m_dbTimeStep = CSimuManager::ComputeTimeStep();
	//_simuManager->m_dbTimeStep = PROPERTIES->getSceneProperties().getf( timeStep );
	
	//cout << "time step " << _simuManager->m_dbTimeStep << endl;
	
	
	//srand(time(NULL));

	// for each object

	for (ovInt i=0; i < _scene->directory.getSize(); i++) {
		
		if (_scene->directory[i].nodeType == GeometricObjectNode) {
			objectName = _scene->directory[i].glName;
 			objectType = PROPERTIES->getObjectProperties(objectName)->getObjectType();
			
			setSimuValues(objectName);
  			
			//CSimuManager::SetPrtlDistance(PROPERTIES->getObjectProperties(objectName)->getf(localParticleDistance)* SIZE_MODIFIER);
			//CSimuManager::SetPrtlDistance(PROPERTIES->getSceneProperties().getf(particleDistance) * SIZE_MODIFIER);
			
			_simuManager->m_prtlDistance =	PROPERTIES->getObjectProperties(objectName)->getf(localParticleDistance) * SIZE_MODIFIER;
			
			switch (objectType) {
			
		
			case Fluid:
					// even solids are fluids
					newFluid = generateFluidParticles(_scene->directory[i]);
					newFluid->objectType = objectType;
					
					// set visible on each particle
					
					setVisibleParticles(newFluid);

					
					// feed to Simu
					_simuManager->m_aryPrtlFluid.AppendOnePointer(newFluid);
				break;
			case Solid:
					CSimuManager::m_eFluidModel = CSimuManager::SPH;
					CSimuManager::m_bFixedParticles = true;
					PROPERTIES->getObjectProperties(objectName)->setb(fixedParticles, true);
					// even solids are fluids
					newFluid = generateFluidParticles(_scene->directory[i]);
					newFluid->objectType = objectType;
					
					// feed to Simu
					_simuManager->m_aryPrtlFluid.AppendOnePointer(newFluid);
					
					
			case CutOut:
					// generateCutOutNef
					//cutOuts.push_back( generateCutOut( _scene->directory[i] ) );
					//std::cout << "got a cutout \n";
					
					// apply transformation to cutouts
					
				
					
				break;

			}
			
		}
	}

	
	
	
	// start simulation returning frames
	QTime animationTimer;

	animationTimer.start();
	static float currentTime = 0;
	ovBool emitNewFrame = false;




	while ( _simuManager->m_dbAnimationClock < PROPERTIES->getSceneProperties().getf(endTime) &&
			_stopAnimating == false) {
	
		_animationFrame.animationTime = _simuManager->m_dbAnimationClock;

		
		
		try {
			
			// get surface only if we are realy using the frame and if it is 
			// enabled

			if (currentTime >= ( 1.0f / PROPERTIES->getSceneProperties().geti(framesPerSecond) ) ){
				
					if ( PROPERTIES->getSceneProperties().getb( renderSurface ) ) {
						CSimuManager::CREATE_SURFACE_MESH = true;
					}

					_animationFrame.frame = currentFrame++;
					currentTime = 0;
					emitNewFrame = true;
			}

			_simuManager->m_pSimuDriver->RunAnimationOneTimeStep();
			_simuManager->m_dbAnimationClock += _simuManager->m_dbTimeStep;

			currentTime += _simuManager->m_dbTimeStep;
			CSimuManager::CREATE_SURFACE_MESH = false;

		}
		catch (const string &message) {
			emit errorMessage( message.c_str() );
			_stopAnimating = true;
		}
		catch(const std::bad_alloc& e)
		{
			emit errorMessage( "Error allocating memory" );
			_stopAnimating = true;
	
		}
		catch (...) {
			emit errorMessage( "Unkown error ocurred" );
			_stopAnimating = true;
		}


		if (emitNewFrame) {
			
			for (int k=0; k < _simuManager->m_aryPrtlFluid.m_paNum; k++) {
				CPrtlFluid* pFluid = _simuManager->m_aryPrtlFluid[k];
				setVisibleParticles(pFluid);
			}
			
			emitNewFrame = false;
			fillFrame();
			emit newFrame();
			
			_animationFrameLock->acquire();
			
		}
		
	}
	emit newAnimation(animationTimer.elapsed());
}

AnimationFrame AnimationGenerator::getFrame()
{
	return _animationFrame;
	
}

void AnimationGenerator::continueWithNewFrame()
{
	_animationFrameLock->release();
	
}

void AnimationGenerator::setVisibleParticles(CPrtlFluid* newFluid)
{
	
	ovInt objectName;
	
	// for each particle

	for (ovInt i=0; i< newFluid->m_pfPrtls.m_paNum; i++) {
		for (ovInt j=0; j < _scene->directory.getSize(); j++) {
			
			if (_scene->directory[j].nodeType != GeometricObjectNode ) continue;
			
 			objectName = _scene->directory[j].glName;
			
 			ObjectType objectType = PROPERTIES->getObjectProperties(objectName)->getObjectType();

			
			if (objectType == CutOut) {
			
				Node node = _scene->directory[j];
				newFluid->m_pfPrtls[i]->visible = node.pointIsInside((newFluid->m_pfPrtls[i]->m_vpPos->v[0] / SIZE_MODIFIER),
														(newFluid->m_pfPrtls[i]->m_vpPos->v[1] / SIZE_MODIFIER),
														(newFluid->m_pfPrtls[i]->m_vpPos->v[2] / SIZE_MODIFIER));
				/*
				newFluid->m_pfPrtls[i]->visible = 
					! _scene->directory[j].pointIsInside((newFluid->m_pfPrtls[i]->m_vpPos->v[0] / SIZE_MODIFIER),
														(newFluid->m_pfPrtls[i]->m_vpPos->v[1] / SIZE_MODIFIER),
														(newFluid->m_pfPrtls[i]->m_vpPos->v[2] / SIZE_MODIFIER)));	
				*/
				if ( ! newFluid->m_pfPrtls[i]->visible ) {
					break;
				}

				 
			}
		}
	}
	
	// check for each cutout
	
}


#if 0
void AnimationGenerator::addBubbles()
{
	// scene has surface
	// frame has particles
	// result should be a list of points for every set of particles

	
		
	for (ovInt i=0; i<_animationFrame.listParticles.size(); i++) {

		// lock this motha
		_bubbleSemaphore->acquire();
		_bubblePositions.clear();

		// get the surface where its going
		Node* node = _scene->directory.getNodeByName(_animationFrame.listGlNames[i]);

		if (node) {
			
			for (ovInt j=0; j< _animationFrame.listParticleCount.size(); j++) {
				// for each particle
				for ( ovInt k=0; k< _animationFrame.listParticleCount[j]; k++) {
					
					// pointIsInside is not working 100% correctly
					if ( pointIsInside( _animationFrame.listParticles[j][k], node) ) {
					
						_bubblePositions.push_back( _animationFrame.listParticles[j][k] );

					}		
				}
			}

			// emit finished one frame of bubbles
			emit newBubbles();
		}

		// for every partcile check inclusion in surface
		// add if needed

	}
	// emit something
}
#endif 

void AnimationGenerator::run()
{

	

	while (true) {
		
		_semaphore->acquire();
		animateFluids();

	}

}

void AnimationGenerator::getStartEndPoints(Node node, ovTuple3f* start, ovTuple3f* end)
{
	ovVector3D transformedPoint;
	
	start->x = DBL_MAX;
	start->y = DBL_MAX;
	start->z = DBL_MAX;

	end->x = DBL_MIN;
	end->y = DBL_MIN;
	end->z = DBL_MIN;

	// XXX FIXME should use back the original non repeating sources (!)

	for (ovUInt i=0; i < node.instanceGeometries.size(); i++) {
		for(ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			for(ovInt k=0; k < node.instanceGeometries[i].geometry->objects[j].sourcePositionCount; k++) {

				// starting point
				
				transformedPoint = node.applyTransformationToPos(i, j, k);

				if (start->x > transformedPoint.getX()) {
					start->x = transformedPoint.getX();
				}
				if (start->y > transformedPoint.getY()) {
					start->y = transformedPoint.getY();
				}
				if (start->z > transformedPoint.getZ()) {
					start->z = transformedPoint.getZ();
				}

				// ending point
				
				if (end->x < transformedPoint.getX()) {
					end->x = transformedPoint.getX();
				}
				if (end->y < transformedPoint.getY()) {
					end->y = transformedPoint.getY();
				}
				if (end->z < transformedPoint.getZ()) {
					end->z = transformedPoint.getZ();
				}
			}
		}
	}

	//end->x += PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
	//end->y += PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
	//end->z += PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
}


CPrtlFluid* AnimationGenerator::generateFluidParticles(const Node &node)
{
		
	ovVector3D tempPos;
	
	CPrtlFluid* newFluid;
	CVector3D position;
	
	CSimuManager::m_airPressure = -100; //XXX airPressure
	newFluid = CSimuManager::CreatePrtlFluid();
	newFluid->glName = node.glName;



	// XXX set correct values !!!

	
	newFluid->m_pfSimuManager = _simuManager;
	

	//newFluid->m_pfAirPressure = 100; //-100;
	
	newFluid->m_pfConstSummation = 4;
	newFluid->m_pfRandomSeed = 42;


	newFluid->objectType				= PROPERTIES->getObjectProperties(newFluid->glName)->getObjectType();
	newFluid->addBubblesFromCO2			= PROPERTIES->getObjectProperties(newFluid->glName)->getb(addBubblesFromCO2);
	newFluid->addBubblesByTotalOrRate	= PROPERTIES->getObjectProperties(newFluid->glName)->getBubbleGenerationType();
	newFluid->co2BubblesPerParticle		= PROPERTIES->getObjectProperties(newFluid->glName)->geti(co2BubblesPerParticle);
	newFluid->co2EnoughForBubble		= PROPERTIES->getObjectProperties(newFluid->glName)->getf(co2EnoughForBubble);
	newFluid->m_CO2PeakGeneration		= PROPERTIES->getObjectProperties(newFluid->glName)->getf(co2PeakGeneration);
	newFluid->m_CO2TemperatureMean		= PROPERTIES->getObjectProperties(newFluid->glName)->getf(co2TemperatureMean);
	newFluid->m_CO2TemperatureRange		= PROPERTIES->getObjectProperties(newFluid->glName)->getf(co2TemperatureRange);
	newFluid->generateCO2				= PROPERTIES->getObjectProperties(newFluid->glName)->getb(generateCO2);
	newFluid->m_pfHeatConductivity		= PROPERTIES->getObjectProperties(newFluid->glName)->getf(heatConductivity);

	newFluid->m_CO2AreaUnderRateCurve	= newFluid->CalulateAreaUnderCO2GenerationCurve(CSimuManager::m_minTemperature, 
																						CSimuManager::m_maxTemperature);
	//newFluid->m_pfMinShearModulus  = pow(10.0f, 2);
	//newFluid->m_pfMaxShearModulus  = pow(10.0f, 4);
	//newFluid->m_pfShearModulus = pow(10.0f, 4);
	
	//newFluid->m_pfRelaxationTime = 0.1;
	//newFluid->m_pfPrtlMassRatio = 100;
	//newFluid->m_pfOnlyGravityTime = 0;
	//newFluid->m_pfPrtlMassRatio = 100;
	

	//newFluid->initialVelocity[0] = -1000.0f;
	//newFluid->initialVelocity[0] = 3000.0f;
	//newFluid->initialVelocity[1] = 2500.0f;
	//newFluid->initialVelocity[2] = 0.0f;

	//newFluid->m_pfPrtlMass = 5;

	// add fluid particles on each vertex
	
		
	if ( PROPERTIES->getObjectProperties(node.glName)->getObjectType() == Fluid ) {
		//addParticlesOnVertices(node, newFluid, false);
		//addParticlesOnEdges(node, newFluid, false);
		//addParticlesOnFacets(node, newFluid, false);

		
		
		addParticlesInVolume(node, newFluid, false);

		// need to add nodes to kdTree before doing any searches
		
		
		if ( PROPERTIES->getObjectProperties(node.glName)->getb(bubblyFluid) ) {
			//addParticlesOnVertices(node, newFluid, true);
			//addParticlesOnEdges(node, newFluid, true);
			//addParticlesOnFacets(node, newFluid, true);
			newFluid->kdTree.addNodes(newFluid->m_pfPrtls);
			addParticlesInVolume(node, newFluid, true);

			// reassign smooth length

			SimuValue fluidLocalParticleDistance = PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
			SimuValue fluidBubbleDistance		 = PROPERTIES->getObjectProperties(node.glName)->getf(bubbleDistance);

		

			//if ( fluidBubbleDistance < fluidLocalParticleDistance ) {
				//reassignSmoothLength(newFluid, SIZE_MODIFIER * fluidBubbleDistance * PROPERTIES->getObjectProperties(node.glName)->getf(smoothLengthRatio) );
				//reassignSmoothLength(newFluid, fluidLocalParticleDistance * PROPERTIES->getObjectProperties(node.glName)->getf(smoothLengthRatio) );

			//}

		}
		
		// XXX check cutouts to change cutout values on each particle
		newFluid->PreparePrtlEvolution();
		newFluid->DetectSrfcPrtls();

	} else if ( PROPERTIES->getObjectProperties(node.glName)->getObjectType() == Solid) {
		
		addRegisteredParticlesOnSurface(node, newFluid);
		newFluid->PreparePrtlEvolution();
	}

	
	

	return newFluid;
}


CPrtlFluid* AnimationGenerator::generateSolidParticles(Node &node)
{

	CPrtlFluid* newFluid;

	newFluid = generateFluidParticles(node);

	// make it a solid
	
	newFluid->objectType = Solid;
	
	
	
	newFluid->m_pbFixedParticles = true;
		
	//newFluid->PreparePrtlEvolution();
	return newFluid;

}

#if 0

Nef_polyhedron AnimationGenerator::generateCutOut(const Node &node)
{
	
	Nef_polyhedron constructedNefPolyhedron;
	
	
	for (ovUInt i=0; i < node.instanceGeometries.size(); i++) {
		for(ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			Nef_polyhedron nef( node.instanceGeometries[i].geometry->objects[j].polyhedron );
			constructedNefPolyhedron = constructedNefPolyhedron + nef;
		}
	}
	
	Nef_polyhedron::Aff_transformation_3 trans(node.transformationMatrix(0, 0),
											   node.transformationMatrix(0, 1),
											   node.transformationMatrix(0, 2),
											   node.transformationMatrix(0, 3),
											   node.transformationMatrix(1, 0),
											   node.transformationMatrix(1, 1),
											   node.transformationMatrix(1, 2),
											   node.transformationMatrix(1, 3),
											   node.transformationMatrix(2, 0),
											   node.transformationMatrix(2, 1),
											   node.transformationMatrix(2, 2),
											   node.transformationMatrix(2, 3), 1);
	
	constructedNefPolyhedron.transform( trans );
	
	return constructedNefPolyhedron;
}
#endif

void AnimationGenerator::setSimuValues(ovInt glName)
{

	if (PROPERTIES->getObjectProperties(glName)->getParticleFluidType() == SPH /*|| 
		PROPERTIES->getObjectProperties(glName)->getObjectType() == Solid*/){ // solids need to be SPH
		CSimuManager::m_eFluidModel = CSimuManager::SPH;
	}

	else if (PROPERTIES->getObjectProperties(glName)->getParticleFluidType() == Upsample) {
		CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE;
	}
	
	CSimuManager::ADAPTIVE_TIME_STEP = false;

	if (PROPERTIES->getObjectProperties(glName)->getFluidCompressibility() == PoissonEquation) {
		_simuManager->m_eCompressibility = CSimuManager::POISSON;
	}
	else if(PROPERTIES->getObjectProperties(glName)->getFluidCompressibility() == GasStateEquation) {
		_simuManager->m_eCompressibility = CSimuManager::GAS_STATE;
	} 
	else { // NoCompressibility
		_simuManager->m_eCompressibility = CSimuManager::NO_PRESSURE;
	}

	if (PROPERTIES->getObjectProperties(glName)->getTensorIntegration() == Explicit) {
		CSimuManager::IMPLICIT_STRESS_INTEGRATION = false;
	} else { // Implicit
		CSimuManager::IMPLICIT_STRESS_INTEGRATION = true;
	}

	if (PROPERTIES->getObjectProperties(glName)->getInitializeParticle() == DensityValue ) {
		CSimuManager::UNIFORM_FIXED_PARTICLE_MASS = false;
	}
	else { // MassValue
		CSimuManager::UNIFORM_FIXED_PARTICLE_MASS = true;
	}
	
	//CSimuManager::UNIFORM_FIXED_PARTICLE_MASS = true;
	
	//_simuManager->m_airPressure = 0;

	_simuManager->m_courantFactor = PROPERTIES->getObjectProperties(glName)->getf(courantFactor);

	_simuManager->APPLY_GRAVITY =			PROPERTIES->getObjectProperties(glName)->getb(applyGravity);
        _simuManager->APPLY_SURFACE_TENSION =           PROPERTIES->getObjectProperties(glName)->getb(applySurfaceTension);;
	_simuManager->m_surfaceTension =		PROPERTIES->getObjectProperties(glName)->getf(surfaceTensionValue);
        _simuManager->m_prtlMass =			PROPERTIES->getObjectProperties(glName)->getf(massValue);
	_simuManager->m_prtlDensity =			PROPERTIES->getObjectProperties(glName)->getf(densityValue);
	_simuManager->m_prtlDistance =			PROPERTIES->getSceneProperties().getf(particleDistance) * SIZE_MODIFIER; //PROPERTIES->getObjectProperties(glName)->getf(localParticleDistance) * SIZE_MODIFIER;
	_simuManager->m_prtlPosJitter =			PROPERTIES->getObjectProperties(glName)->getf(particlePositionJitter);
	_simuManager->m_relaxationTime =		PROPERTIES->getObjectProperties(glName)->getf(relaxationTime);
	_simuManager->m_shearModulus =			PROPERTIES->getObjectProperties(glName)->getf(shearModulus);
	_simuManager->m_frictionRate =			PROPERTIES->getObjectProperties(glName)->getf(frictionRate);
	_simuManager->m_distDumpingRate =		PROPERTIES->getObjectProperties(glName)->getf(distanceDumpingRate);
	_simuManager->m_energyDumpingRate =		PROPERTIES->getObjectProperties(glName)->getf(energyDumpingRate);
	_simuManager->m_soundSpeed =			PROPERTIES->getObjectProperties(glName)->getf(soundSpeed);
	_simuManager->m_velocityTuner =			PROPERTIES->getObjectProperties(glName)->getf(velocityTuner);
	_simuManager->m_courantFactor =			PROPERTIES->getObjectProperties(glName)->getf(courantFactor);
	_simuManager->m_smoothLengthRatio =		PROPERTIES->getObjectProperties(glName)->getf(smoothLengthRatio);
	_simuManager->m_srfcDenThreshold =		PROPERTIES->getObjectProperties(glName)->getf(surfaceDensityThreshold);
	_simuManager->m_CGSolverTolerance =		PROPERTIES->getObjectProperties(glName)->getf(cgTolerance);
	//_simuManager->m_bFixedParticles =		PROPERTIES->getObjectProperties(glName)->getb(fixedParticles);
	CSimuManager::m_bFixedParticles =		PROPERTIES->getObjectProperties(glName)->getb(fixedParticles);
	CSimuManager::m_relaxationTime =		PROPERTIES->getObjectProperties(glName)->getf(relaxationTime);
	CSimuManager::m_shearModulus =			PROPERTIES->getObjectProperties(glName)->getf(shearModulus);
	
	_simuManager->m_dbTimeStep = _simuManager->ComputeTimeStep();
}


void AnimationGenerator::copyPrtlValues(ParticleInstance &particleInstance, CFluidPrtl *fluidPrtl)
{
	particleInstance.pos.x = fluidPrtl->m_vpPos->v[X] / SIZE_MODIFIER;
	particleInstance.pos.y = fluidPrtl->m_vpPos->v[Y] / SIZE_MODIFIER;
	particleInstance.pos.z = fluidPrtl->m_vpPos->v[Z] / SIZE_MODIFIER;
	
	particleInstance.normal.x = fluidPrtl->m_vpNormal->v[X];
	particleInstance.normal.y = fluidPrtl->m_vpNormal->v[Y];
	particleInstance.normal.z = fluidPrtl->m_vpNormal->v[Z];
	
	particleInstance.gradient.x = fluidPrtl->m_vpGrad->v[X];
	particleInstance.gradient.y = fluidPrtl->m_vpGrad->v[Y];
	particleInstance.gradient.z = fluidPrtl->m_vpGrad->v[Z];

	particleInstance.veloctiy.x = fluidPrtl->m_vpVel->v[X];


	particleInstance.veloctiy.y = fluidPrtl->m_vpVel->v[Y];
	particleInstance.veloctiy.z = fluidPrtl->m_vpVel->v[Z];

	particleInstance.size  = fluidPrtl->m_pfSize;
	particleInstance.temp  = fluidPrtl->m_fpTemperature;
	particleInstance.co2   = fluidPrtl->m_pfCO2;
	particleInstance.density = fluidPrtl->m_fpDensity;
	particleInstance.originalDensity = fluidPrtl->m_fpOrgDensity;
	particleInstance.mass = fluidPrtl->m_fpMass;
    particleInstance.densityAdded = fluidPrtl->m_bubbleDensity;
	particleInstance.trueSurfaceTime = fluidPrtl->m_pbTrueCurrentSurfaceTime;
	
	particleInstance.onSurface   = fluidPrtl->m_fpOnSurface ? 1 : 0;
	particleInstance.inCollision = fluidPrtl->m_fpInCollision ? 1 : 0;
	particleInstance.trueSurface = fluidPrtl->m_pbTrueCurrentSurface ? 1.0f : 0.0f;
	
	particleInstance.massDensityRatio = fluidPrtl->m_fpMass / fluidPrtl->m_fpDensity;
	particleInstance.visible = fluidPrtl->visible ? 1 : 0;
	
	particleInstance.neighbourCount = fluidPrtl->m_fpNgbrs.m_paNum;
	particleInstance.shearModulus = fluidPrtl->m_fpShearModulus;
	particleInstance.decayRate = fluidPrtl->m_decayRate;
	particleInstance.pressure = fluidPrtl->m_fpPressure;
	particleInstance.bubblePressure = fluidPrtl->m_bubblePressure;

	particleInstance.velocityTuner = fluidPrtl->velocityTuner;
	particleInstance.neighbourCount = fluidPrtl->m_fpNgbrs.m_paNum;
	particleInstance.smoothingLength =  fluidPrtl->m_pfSmoothLen / SIZE_MODIFIER;
	//std::cout << " particleInstance.density " << particleInstance.density << "\n";
	
}

void AnimationGenerator::fillFrame()
{
	Polygons thisFluid;
	ParticleInstance *thisParticles;
	ParticleInstance *thisBubbles;
	std::vector<ParticleInstance*> surfaceParticles;
	//ovInt numberParticles;
	
	//CSimuTriangle* triangle;	
	//CSimuSrfcPoint3D *v0, *v1, *v2;
	
	
	_animationFrame.listFluids.clear();
	// take all particles
	
	for (ovUInt i = 0; i < _animationFrame.listParticles.size(); i++) {
		delete _animationFrame.listParticles[i];
	}
	
	for (ovUInt i = 0; i < _animationFrame.listBubbles.size(); i++) {
		delete _animationFrame.listBubbles[i];
	}
	
	
	_animationFrame.listParticles.clear();
	_animationFrame.listBubbles.clear();
	_animationFrame.listParticleCount.clear();
	_animationFrame.listBubblesCount.clear();
	_animationFrame.listGlNames.clear();
	
	
	
	// for each fluid
	for(ovInt i=0; i < _simuManager->m_aryPrtlFluid.m_paNum; i++) {
		
		
		if(_simuManager->m_aryPrtlFluid[i]->objectType == Solid) continue;
		
		// now deal with particles / bubbles
		
		ovInt particleCount =0, bubbleCount =0;
		
		for (ovInt j=0; j<_simuManager->m_aryPrtlFluid[i]->m_pfPrtls.m_paNum; j++) {
			if ( _simuManager->m_aryPrtlFluid[i]->m_pfPrtls[j]->m_bpIsBubble ) {
				bubbleCount++;
			}
			else {
				particleCount++;
			}
		}
		
		_animationFrame.listParticleCount.push_back( particleCount );
		_animationFrame.listBubblesCount.push_back( bubbleCount );
		thisParticles = new ParticleInstance[ particleCount ];
		thisBubbles = new ParticleInstance[ bubbleCount ];
		
		for (ovInt j=0, p=0, q=0; j<_simuManager->m_aryPrtlFluid[i]->m_pfPrtls.m_paNum; j++) {
			
			if (_simuManager->m_aryPrtlFluid[i]->m_pfPrtls[j]->m_bpIsBubble) {
				copyPrtlValues(thisBubbles[p], _simuManager->m_aryPrtlFluid[i]->m_pfPrtls[j]);
		
				
				surfaceParticles.push_back(&thisBubbles[q]);
				p++;
			}
			else {
				copyPrtlValues(thisParticles[q], _simuManager->m_aryPrtlFluid[i]->m_pfPrtls[j]);
				
				surfaceParticles.push_back(&thisParticles[q]);
	
				q++;
				
			}
			
		}
		
		
		
		
		_animationFrame.listGlNames.push_back( _simuManager->m_aryPrtlFluid[i]->glName );
		
		if ( PROPERTIES->getSceneProperties().getb(renderSurface) && _simuManager->m_aryPrtlFluid[i]->objectType == Fluid ) {
			//std::cout << "size " << surfaceParticles.size() << "\n";
		//	_animationFrame.listFluids.push_back( SurfaceMesher::getMesh(surfaceParticles) );
			//_animationFrame.listFluids.push_back( thisFluid );
		} else {
			
			_animationFrame.listFluids.push_back( thisFluid );
		}
		
		_animationFrame.listParticles.push_back(thisParticles);
		_animationFrame.listBubbles.push_back(thisBubbles);
	}
	
	// perform the nef polyhedron boolean cut outs
#if 0	
	if ( PROPERTIES->getSceneProperties().getb(renderSurface) && !cutOuts.empty() ) {
		Polyhedron newPoly;
		// for each cutout
		for (ovUInt i=0; i < _animationFrame.listFluids.size(); i++) {
			// construct nef
			Nef_polyhedron fluidNef(_animationFrame.listFluids[i].polyhedron);
			
			for (ovUInt j=0; j < cutOuts.size(); j++) {
				fluidNef = fluidNef - cutOuts[j];
			}
			fluidNef.convert_to_Polyhedron(newPoly);
			_animationFrame.listFluids[i] = newPoly;
						
		}
		
	}
#endif
}
	




void AnimationGenerator::printFluidInfo()
{

	cout << "Density " << _simuManager->m_aryPrtlFluid[0]->m_pfDensity<< endl;
	cout << "prtl dist " << _simuManager->m_aryPrtlFluid[0]->m_pfPrtlDist<< endl;
	cout << "prtl mass " << _simuManager->m_aryPrtlFluid[0]->m_pfPrtlMass<< endl;
	cout << "total mass " << _simuManager->m_aryPrtlFluid[0]->m_pfTotalMass<< endl;
	cout << "total free prtl mass " << _simuManager->m_aryPrtlFluid[0]->m_pfTotalFreePrtlMass<< endl;
	//cout << "smooth len " << _simuManager->m_aryPrtlFluid[0]->m_pfSmoothLen<< endl;
	//cout << "smooth len sqr " << _simuManager->m_aryPrtlFluid[0]->m_pfSmoothLenSqr<< endl;
	cout << "time step " << _simuManager->m_aryPrtlFluid[0]->m_pfTimeStep<< endl;
	cout << "max time step " << _simuManager->m_aryPrtlFluid[0]->m_pfMaxTimeStep<< endl;
	cout << "random seed " << _simuManager->m_aryPrtlFluid[0]->m_pfRandomSeed<< endl;
	cout << "avg ve ratio " << _simuManager->m_aryPrtlFluid[0]->m_pfAvgVERatio<< endl;

	cout << "unweighted sum " << _simuManager->m_aryPrtlFluid[0]->m_pfUniWeightSum<< endl;
	cout << "uni prtl volume " << _simuManager->m_aryPrtlFluid[0]->m_pfUniPrtlVolume<< endl;
	cout << "uni prtl num density " << _simuManager->m_aryPrtlFluid[0]->m_pfUniPrtlNumDensity<< endl;

	// for fluid-fluid interaction
	cout << "prtl mass ratio " << _simuManager->m_aryPrtlFluid[0]->m_pfPrtlMassRatio<< endl;
	cout << "inter fluid damoing " << _simuManager->m_aryPrtlFluid[0]->m_pfInterFluidDamping<< endl;
	cout << "inter fluid radius " << _simuManager->m_aryPrtlFluid[0]->m_pfInterFluidRadius<< endl; // for repulsive force
	cout << "kevariation " << _simuManager->m_aryPrtlFluid[0]->m_pfKEVariation<< endl;	// for debug use only

	cout << "air pressure " << _simuManager->m_aryPrtlFluid[0]->m_pfAirPressure<< endl;
	cout << "shear modulis" << _simuManager->m_aryPrtlFluid[0]->m_pfShearModulus<< endl;
	cout << "relaxation time " << _simuManager->m_aryPrtlFluid[0]->m_pfRelaxationTime<< endl;
	

	cout << "subthermal steps " <<  _simuManager->m_aryPrtlFluid[0]->m_pfSubThermalSteps<< endl;
	cout << "heat conductivity " << _simuManager->m_aryPrtlFluid[0]->m_pfHeatConductivity<< endl;
	cout << "init temperature " << _simuManager->m_aryPrtlFluid[0]->m_pfIniTemperature<< endl;
	cout << "min shear modulus " << _simuManager->m_aryPrtlFluid[0]->m_pfMinShearModulus<< endl;
	cout << "max shear modulus " << _simuManager->m_aryPrtlFluid[0]->m_pfMaxShearModulus<< endl;
	// linear melting 0
	cout << "coeff temp" << _simuManager->m_aryPrtlFluid[0]->m_pfCoeffTemperature<< endl;
	cout << "coeff shear modulus" << _simuManager->m_aryPrtlFluid[0]->m_pfCoeffShearModulus<< endl;
	cout << "const summation " << _simuManager->m_aryPrtlFluid[0]->m_pfConstSummation<< endl;
	

	// variables for animation control
	cout << "only boundary" << _simuManager->m_aryPrtlFluid[0]->m_pfOnlyBdryMovingTime<< endl;
	cout << "only gravity" << _simuManager->m_aryPrtlFluid[0]->m_pfOnlyGravityTime<< endl;


	cout << "avg num neighbors " << _simuManager->m_aryPrtlFluid[0]->m_pfAvgNumNgbrs<< endl;

	cout << "poisson pause steps " <<  _simuManager->m_aryPrtlFluid[0]->m_pfPoissonPauseSteps<< endl;
	cout << "srfc detect pause steps " <<  _simuManager->m_aryPrtlFluid[0]->m_pfSrfcDetectPauseSteps<< endl;

	// variables for iso-surface
	cout << "iso density radius " << _simuManager->m_aryPrtlFluid[0]->m_pfIsoDensituRadius<< endl;
	cout << "iso density ratio " << _simuManager->m_aryPrtlFluid[0]->m_pfIsoDensityRatio<< endl;
	
			

	cout << "interact time " << _simuManager->m_aryPrtlFluid[0]->m_pfInteractTime<< endl;
	cout << "motion time " << _simuManager->m_aryPrtlFluid[0]->m_pfMotionTime<< endl;
	cout << "surface time " << _simuManager->m_aryPrtlFluid[0]->m_pfSurfaceTime<< endl;

}

void AnimationGenerator::addParticlesOnVertices(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets)
{
	ovVector3D transformedPoint;
	std::vector<ovVector3D> addedList;
	ovBool foundFlag;
	Vertex_iterator vertexIterator;


	for (ovUInt i =0; i < node.instanceGeometries.size(); i++) {
		for (ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			
			vertexIterator = node.instanceGeometries[i].geometry->objects[j].polyhedron.vertices_begin();

			while (vertexIterator != node.instanceGeometries[i].geometry->objects[j].polyhedron.vertices_end()) {
				
				transformedPoint.set( CGAL::to_double(vertexIterator->point().x()),
									  CGAL::to_double(vertexIterator->point().y()),
									  CGAL::to_double(vertexIterator->point().z()) );

				transformedPoint = node.applyTransformationToPosToVector( transformedPoint );
				
				foundFlag = checkPastInsertions(addedList, transformedPoint);
						
				if ( foundFlag == false ) {
					addedList.push_back(transformedPoint);
					
					addParticleToFluid(newFluid, 
									   transformedPoint,
									   newFluid->initialVelocity, 
									   true, 
									   PROPERTIES->getObjectProperties(node.glName)->getb(fixedParticles),
									   isBubble);

				}
				
				++vertexIterator;

			}
		}
	}
}

void AnimationGenerator::addParticlesOnEdges(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets)
{
	ovVector3D transformedPoint;
	Halfedge_iterator hdsIterator;
	Vertex_handle vhBegin, vhEnd;
	Vertex currentPosition, step;
	Vector_3 midVector, stepVector;
	//ovVector3D midPoint, begin, end;
	Point temp, temp2;
	ovFloat midVectorLength, stepVectorLength;
	ovBool foundFlag;
	vector<ovVector3D> addedList;
	ObjectPropertyType distanceType;
	
	distanceType = getDistanceType( isBubble );


	for (ovUInt i =0; i < node.instanceGeometries.size(); i++) {
		for (ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			
			hdsIterator = node.instanceGeometries[i].geometry->objects[j].polyhedron.halfedges_begin();
			
			while( hdsIterator != node.instanceGeometries[i].geometry->objects[j].polyhedron.halfedges_end() ) {
				
				vhBegin = hdsIterator->vertex();
				vhEnd = hdsIterator->prev()->vertex();
			
				midVector = vhEnd->point() - vhBegin->point();
				midVectorLength = CGAL::sqrt(CGAL::to_double(midVector.squared_length()));	

				// normalize midVector and set it to apropiate moving distance

				midVector = (midVector / midVectorLength); 
				midVector = midVector * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);
				
				if (CGAL::sqrt(CGAL::to_double(midVector.squared_length())) < midVectorLength) {
					
					currentPosition = *vhBegin;
					currentPosition.point() = currentPosition.point() + midVector;

				
					stepVector = currentPosition.point() - vhBegin->point();
					stepVectorLength = CGAL::sqrt(CGAL::to_double(stepVector.squared_length()));
					

					while ( stepVectorLength <= midVectorLength  ) {
							
						
						transformedPoint.set( CGAL::to_double(currentPosition.point().x()),
											  CGAL::to_double(currentPosition.point().y()),
											  CGAL::to_double(currentPosition.point().z()) );

						transformedPoint = node.applyTransformationToPosToVector( transformedPoint );

						foundFlag = checkPastInsertions(addedList, transformedPoint);
						
						if ( foundFlag == false ) {
							addedList.push_back(transformedPoint);
							
							addParticleToFluid(newFluid, 
											   transformedPoint,
											   newFluid->initialVelocity, 
											   true, 
											   PROPERTIES->getObjectProperties(node.glName)->getb(fixedParticles),
											   isBubble);

						}

						currentPosition.point() = currentPosition.point() + midVector;
						stepVector = currentPosition.point() - vhBegin->point();
						stepVectorLength = CGAL::sqrt(CGAL::to_double(stepVector.squared_length()));

						
					}
				}
				++hdsIterator;
			
			}
		}
	}
}

/* this function finally adds the particle to the list, should change the parameter
 * values to flags
 */

void AnimationGenerator::addParticleToFluid(CPrtlFluid* newFluid, 
											ovVector3D position,
											CVector3D &velocity, 
											ovBool onSurface, 
											ovBool isFixed, 
											ovBool isBubble,
											ovInt belongsToFacet)
{
	CFluidPrtl* newPrtl;
	ovFloat jitter;

	newPrtl = newFluid->CreateOneFluidPrtl();						
	newPrtl->m_fpMass = newFluid->m_pfPrtlMass; 
	newPrtl->m_vpVel->SetValue(&velocity);
	newPrtl->m_pointBelongsToFacet = belongsToFacet;
//isBubble = true;

	

#if 1
	if ( isBubble ) {
		jitter = PROPERTIES->getObjectProperties(newFluid->glName)->getf(bubbleJitter);
	}
	else {
		jitter = PROPERTIES->getObjectProperties(newFluid->glName)->getf(positionJitter);
	}
#else
	// XXX HARDCODED for now
	jitter = PROPERTIES->getObjectProperties(newFluid->glName)->getf(positionJitter);
	
	if (PROPERTIES->getObjectProperties(newFluid->glName)->getObjectType() == Fluid) {
		isBubble = randFloat(0.0f, 9.0f) <= 8.0;
	}
#endif
	
	// accomodate for jittering
	position += ovVector3D(randFloat(-1.0f * jitter, jitter),
						   randFloat(-1.0f * jitter, jitter),
						   randFloat(-1.0f * jitter, jitter) );

	newPrtl->m_vpPos->SetElements((float)position.getX() * SIZE_MODIFIER, 
								  (float)position.getY() * SIZE_MODIFIER, 
								  (float)position.getZ() * SIZE_MODIFIER);

	// set particle size (only used for object behaviour ie bubbles
	/*
	newPrtl->m_pfSize = randFloat( PROPERTIES->getObjectProperties(newFluid->glName)->getf( minSizeScale ) ,
								   PROPERTIES->getObjectProperties(newFluid->glName)->getf( maxSizeScale ) );	
	*/

	
	newPrtl->m_pfSize					= 1; //INITIAL_BUBBLE_VOLUME; //0
	newPrtl->m_initBubbleSize			= newPrtl->m_pfSize;
	newPrtl->m_initBubbleTemperature	= PROPERTIES->getSceneProperties().getf( minTemp );
	
	newPrtl->m_pfSmoothLen				= newFluid->m_pfPrtlDist*CSimuManager::m_smoothLengthRatio;
	newPrtl->m_fpInitialSmoothingLength = newPrtl->m_pfSmoothLen;
	newPrtl->m_fpOnSurface				= onSurface;
	newPrtl->m_pbFixedParticle			= isFixed;
	newPrtl->m_bpIsBubble				= isBubble;
	newFluid->m_pfPrtls.AppendOnePointer(newPrtl);

}






ovBool AnimationGenerator::checkPastInsertions(std::vector<ovVector3D> &addedList, ovVector3D newPoint) 
{
	ovFloat distance;

	for (ovUInt p=0; p <addedList.size(); p++) {
		distance = newPoint.getDistanceTo(addedList[p]);
		
		if ( distance < MIN_SRFC_PRTL_DIST ) { // min distance between vertex particles
			return true;

		}

	}

	return false;
}


void AnimationGenerator::addRegisteredParticlesOnSurface(Node node, CPrtlFluid* newFluid)
{
	std::vector<Point_3> pointsInPlane;
	std::vector<ovInt> coordsToUse;
	ovVector3D transformedPoint;
	ovInt facetName = 0;
	
	// for edges
	//Vector_3 edges[3];
	ovVector3D vertices[3];
	CVector3D noSpeed;
	noSpeed.SetValue(0.0f);

	for (ovUInt i =0; i < node.instanceGeometries.size(); i++) {
		for (ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			
			//newFluid->nefPolyhedronList.push_back(node.instanceGeometries[i].geometry->objects[j].nefPolyhedron);

			
			Polygons *poly = &(node.instanceGeometries[i].geometry->objects[j]);
			ovUInt triangleCount = poly->triangleCount; 
			ovUInt currentVertex = 0;
			for (ovUInt triangleIterator = 0; triangleIterator < triangleCount; triangleIterator++ ) {
				//node.instanceGeometries[i].geometry->objects[j].				
				// add 3 vertices points				
				
				CPrtlFluid::FacetComposedOf facetComposedOf;
				
				for (ovInt k = 0; k < 3; k++, currentVertex++) {
					
					vertices[k] = poly->sourcePosition[currentVertex];
					
					transformedPoint.set( vertices[k].getX(),
										  vertices[k].getY(),
										  vertices[k].getZ() );
					
					transformedPoint = node.applyTransformationToPosToVector( transformedPoint );
					
					addParticleToFluid(newFluid, 
									   transformedPoint,
									   noSpeed,
									   true, 
									   true,
									   false,
									   facetName);

					facetComposedOf.vertices[k] = newFluid->m_pfPrtls.m_paNum - 1;
					newFluid->vertexBelongstoFacet.push_back(facetName);

				}
				newFluid->facets.push_back(facetComposedOf);
				
				
				// add edge points

				SimuValue particleDistance = PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
#if 1
				for (ovInt k=0; k<3; k++) {
					ovVector3D currentEdge = vertices[(k+1) % 3] - vertices[k];
					SimuValue currentEdgeSquaredLength = currentEdge.getSquaredLength();
					
					// normalize step length and set to particle distance
					ovVector3D stepEdge = (currentEdge / currentEdge.getLength() ) * particleDistance;
					ovVector3D currentStepEdge = stepEdge;

					ovVector3D currentPosition;
					int stepScale = 1;
					while ( currentStepEdge.getSquaredLength() < currentEdgeSquaredLength ) {
						currentPosition = vertices[k] + currentStepEdge;
						// add the next point
						
						transformedPoint.set( currentPosition.getX(),
											  currentPosition.getY(),
											  currentPosition.getZ() );
						
						transformedPoint = node.applyTransformationToPosToVector( transformedPoint );
						
						addParticleToFluid(newFluid, 
										   transformedPoint,
										   noSpeed, 
										   true, 
										   true,
										   false,
										   facetName);

						newFluid->vertexBelongstoFacet.push_back(facetName);

						// move to the next step
						currentStepEdge = stepEdge * ++stepScale;

						
					}
				}
#endif
				// add facet points

				///////////

				Plane_3 plane( Point_3(vertices[0].getX(), vertices[0].getY(), vertices[0].getZ() ), 
							   Point_3(vertices[1].getX(), vertices[1].getY(), vertices[1].getZ() ), 
							   Point_3(vertices[2].getX(), vertices[2].getY(), vertices[2].getZ() ) );

				getFourBoxPlaneIntersections(getFacetBoundingBox( vertices ), plane, pointsInPlane, coordsToUse);
											

				// get step vectors

				Vector_3 iMidVector = pointsInPlane[1] - pointsInPlane[0];
				Vector_3 jMidVector = pointsInPlane[2] - pointsInPlane[0];

				// normalize the vectors and set them to the step size

				//Vector_3 iStepVector = iMidVector/sqrt(iMidVector*iMidVector);
				//Vector_3 jStepVector = jMidVector/sqrt(jMidVector*jMidVector);

				Vector_3 iStepVector = iMidVector/CGAL::sqrt(CGAL::to_double(iMidVector.squared_length()));
				Vector_3 jStepVector = jMidVector/CGAL::sqrt(CGAL::to_double(jMidVector.squared_length()));

				iStepVector = iStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);
				jStepVector = jStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(localParticleDistance);

				// compute the points

				for (Vector_3 iCurrentPos = iStepVector; 
					 iCurrentPos.squared_length() < iMidVector.squared_length();
					 iCurrentPos = iCurrentPos + iStepVector) {

						 for (Vector_3 jCurrentPos = jStepVector;
							  jCurrentPos.squared_length() < jMidVector.squared_length();
							  jCurrentPos = jCurrentPos + jStepVector) {
								
								Point_3 currentPosition = pointsInPlane[0] + iCurrentPos + jCurrentPos;

								// need to check if its actually on the facet 
								ovInt intersectionCount = 0;

								Kernel::Ray_3 intersectionRay(currentPosition, iStepVector);
								//facetCirculator = facetIterator->facet_begin();

								

								for (ovInt k=0; k<3; k++) {
									Kernel::Segment_2 proyectedEdge(Kernel::Point_2(vertices[(k+1)%3].getT()[coordsToUse[0]] ,
																					vertices[(k+1)%3].getT()[coordsToUse[1]]) ,
																	Kernel::Point_2(vertices[k].getT()[coordsToUse[0]] ,
																					vertices[k].getT()[coordsToUse[1]]));


									Kernel::Ray_2     proyectedRay(Kernel::Point_2(currentPosition[coordsToUse[0]],
																				   currentPosition[coordsToUse[1]]),
																   Kernel::Direction_2(Kernel::Vector_2(iStepVector[coordsToUse[0]],
																										iStepVector[coordsToUse[1]])));

									
										
									if ( CGAL::do_intersect(proyectedEdge, proyectedRay) ) {
										intersectionCount++;
									}
								}
								/*

								
								do {
									// check for intersections in 2D
									
									Kernel::Segment_2 proyectedEdge(Kernel::Point_2(facetCirculator->vertex()->point()[coordsToUse[0]] ,
																					facetCirculator->vertex()->point()[coordsToUse[1]]),
																	Kernel::Point_2(facetCirculator->prev()->vertex()->point()[coordsToUse[0]] ,
																					facetCirculator->prev()->vertex()->point()[coordsToUse[1]]));


									Kernel::Ray_2     proyectedRay(Kernel::Point_2(currentPosition[coordsToUse[0]],
																				   currentPosition[coordsToUse[1]]),
																   Kernel::Direction_2(Kernel::Vector_2(iStepVector[coordsToUse[0]],
																										iStepVector[coordsToUse[1]])));

									
										
									if ( CGAL::do_intersect(proyectedEdge, proyectedRay) ) {
										intersectionCount++;
									}
									
									++facetCirculator;
									
								}while ( facetCirculator != facetIterator->facet_begin());
								    
								*/
								  
								if (intersectionCount % 2 != 0) {
									  transformedPoint.set( CGAL::to_double(currentPosition.x()),
															CGAL::to_double(currentPosition.y()),
															CGAL::to_double(currentPosition.z()) );

										transformedPoint = node.applyTransformationToPosToVector( transformedPoint );

									

										addParticleToFluid(newFluid, 
														   transformedPoint,
														   noSpeed, 
														   true, 
														   true,
														   false,
														   facetName);

										newFluid->vertexBelongstoFacet.push_back(facetName);
									
								  }

						 }


				}

				// finished computing the points 

				///////////

				// clean up for this facet and move to next
				pointsInPlane.clear();
				++facetName;

			}
		}
	}
	//std::cout << newFluid->m_pfPrtls.m_paNum << " second\n";
}



void AnimationGenerator::addParticlesOnFacets(Node node, CPrtlFluid* newFluid, ovBool isBubble, ovBool registerFacets)
{

	Facet_iterator facetIterator;
	std::vector<Point_3> pointsInPlane;
	std::vector<ovInt> coordsToUse;
	Polyhedron::Halfedge_around_facet_circulator facetCirculator;
	vector<ovVector3D> addedList;
	ovBool foundFlag;
	ovVector3D transformedPoint;
	ObjectPropertyType distanceType;
	
	distanceType = getDistanceType( isBubble );
	

	for (ovUInt i =0; i < node.instanceGeometries.size(); i++) {
		for (ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {

			facetIterator = node.instanceGeometries[i].geometry->objects[j].polyhedron.facets_begin();

			while ( facetIterator != node.instanceGeometries[i].geometry->objects[j].polyhedron.facets_end() ) {
				
				facetCirculator = facetIterator->facet_begin();

				Plane_3 plane( (*facetCirculator++).vertex()->point(),
							   (*facetCirculator++).vertex()->point(),
							   (*facetCirculator++).vertex()->point() );

				getFourBoxPlaneIntersections(getFacetBoundingBox( *facetIterator ), plane, pointsInPlane, coordsToUse);
											

				// get step vectors

				Vector_3 iMidVector = pointsInPlane[1] - pointsInPlane[0];
				Vector_3 jMidVector = pointsInPlane[2] - pointsInPlane[0];

				// normalize the vectors and set them to the step size

				Vector_3 iStepVector = iMidVector/CGAL::sqrt(CGAL::to_double(iMidVector.squared_length()));
				Vector_3 jStepVector = jMidVector/CGAL::sqrt(CGAL::to_double(jMidVector.squared_length()));
				

			
				// iStepVector = iStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType) * 0.5;
				// jStepVector = jStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType) * 0.5;
				iStepVector = iStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);
				jStepVector = jStepVector * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);

				// compute the points

				
				

				for (Vector_3 iCurrentPos = iStepVector; 
					 iCurrentPos.squared_length() < iMidVector.squared_length();
					 iCurrentPos = iCurrentPos + iStepVector) {

						 for (Vector_3 jCurrentPos = jStepVector;
							  jCurrentPos.squared_length() < jMidVector.squared_length();
							  jCurrentPos = jCurrentPos + jStepVector) {
								
								Point_3 currentPosition = pointsInPlane[0] + iCurrentPos + jCurrentPos;

								// need to check if its actually on the facet 
								ovInt intersectionCount = 0;

								Kernel::Ray_3 intersectionRay(currentPosition, iStepVector);
								facetCirculator = facetIterator->facet_begin();


								
								do {
									// check for intersections in 2D
									
									Kernel::Segment_2 proyectedEdge(Kernel::Point_2(facetCirculator->vertex()->point()[coordsToUse[0]] ,
																					facetCirculator->vertex()->point()[coordsToUse[1]]),
																	Kernel::Point_2(facetCirculator->prev()->vertex()->point()[coordsToUse[0]] ,
																					facetCirculator->prev()->vertex()->point()[coordsToUse[1]]));


									Kernel::Ray_2     proyectedRay(Kernel::Point_2(currentPosition[coordsToUse[0]],
																				   currentPosition[coordsToUse[1]]),
																   Kernel::Direction_2(Kernel::Vector_2(iStepVector[coordsToUse[0]],
																										iStepVector[coordsToUse[1]])));

									
										
									if ( CGAL::do_intersect(proyectedEdge, proyectedRay) ) {
										intersectionCount++;
									}
									
									++facetCirculator;

								  }while ( facetCirculator != facetIterator->facet_begin());
									   
								  if (intersectionCount %2 != 0) {
									  transformedPoint.set( CGAL::to_double(currentPosition.x()),
															CGAL::to_double(currentPosition.y()),
															CGAL::to_double(currentPosition.z()) );

									  transformedPoint = node.applyTransformationToPosToVector( transformedPoint );

									  foundFlag = checkPastInsertions(addedList, transformedPoint);

									  if ( foundFlag == false ) {
								  		addedList.push_back(transformedPoint);
									 
										addParticleToFluid(newFluid, 
														   transformedPoint,
														   newFluid->initialVelocity, 
														   true, 
														   PROPERTIES->getObjectProperties(node.glName)->getb(fixedParticles),
														   isBubble);

									  }
								  }

						 }


				}

				pointsInPlane.clear();
				++facetIterator;
			}

		}
	}
}


void AnimationGenerator::addParticlesInVolume(Node node, CPrtlFluid* newFluid, ovBool isBubble)
{
	//Point_3 startPoint(start.x, start.y, start.z);
	//Point_3 endPoint(end.x, end.y, end.z);
	Point_3 newPos;
	Vector_3 stepX(1,0,0), 
			 stepY(0,1,0),
			 stepZ(0,0,1);
	Vector_3 start(0,0,0);
	Vector_3 xSide, ySide, zSide;
	ObjectPropertyType distanceType;
	
	distanceType = getDistanceType( isBubble );

	// set correct particle distance

	stepX = stepX * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);
	stepY = stepY * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);
	stepZ = stepZ * PROPERTIES->getObjectProperties(node.glName)->getf(distanceType);

	Kernel::Iso_cuboid_3 boundingBox = getNodeBoundingBox(node);
	
	// get the vectors representing the x, y & z lengths of the cube
	xSide = boundingBox[1] - boundingBox[0];
	ySide = boundingBox[3] - boundingBox[0];
	zSide = boundingBox[5] - boundingBox[0];

	Vector_3 xPos;
	Vector_3 yPos;
	Vector_3 zPos;

	bool bubbleStrips = false;
	int stripCount = 5;
	double stripWidth = sqrt(zSide.squared_length()) / stripCount; // on Z
	double currentWidth =0;

	for(xPos = start; 
		xPos.squared_length() <= xSide.squared_length(); 
		xPos = xPos + stepX ) {
		for(yPos = start; 
			yPos.squared_length() <= ySide.squared_length(); 
			yPos = yPos + stepY ) {
			currentWidth = 0;
			for(zPos = start; 
				zPos.squared_length() <= zSide.squared_length(); 
				zPos = zPos + stepZ ) {
				
				newPos = boundingBox[0];
				newPos = newPos + (xPos + yPos + zPos);

				// is the new particle inside the volume ?
			

				//Kernel::Ray_3x( newPos, Kernel::Direction_3(0,1,0));
				ovTuple3f newValue;
				newValue.x = CGAL::to_double(newPos.x());
				newValue.y = CGAL::to_double(newPos.y());
				newValue.z = CGAL::to_double(newPos.z());
				//ovBool surfaceParticle;
				
				//if ( (isBubble == false && pointIsInside(newValue, node)) ||
				//	 (isBubble == true  && positionIsInsideFluidByLocalConvexHull(newFluid, newValue)) ) {
				if ( node.pointIsInside(newValue) ) {
					

					
					
					//isBubble = rand() % 100 <= 80;
                                   //    isBubble = rand() % 100 <= 50;
                     
                     
                     			//currentWidth += sqrt(stepZ.squared_length());
					//if (currentWidth > stripWidth) {
					//	bubbleStrips = ! bubbleStrips;	
					//	currentWidth = 0;
					//}
					//isBubble = bubbleStrips;
                    			
					//isBubble = rand() % 100 <= 70;
					isBubble = rand() % 100 <= 40;

					addParticleToFluid(newFluid, 
									   ovVector3D(CGAL::to_double(newPos.x()), 
												  CGAL::to_double(newPos.y()), 
												  CGAL::to_double(newPos.z())), 
									   newFluid->initialVelocity,
									   false, 
									   PROPERTIES->getObjectProperties(node.glName)->getb(fixedParticles),
									   isBubble);

				}

			}
		}
	}

	

}

Kernel::Iso_cuboid_3 AnimationGenerator::getFacetBoundingBox(ovVector3D vertices[3])
{
	std::vector<Point_3> points;
		
	for (ovInt i = 0; i < 3; i++ ) {
		points.push_back( Point_3( vertices[i].getX(), vertices[i].getY(), vertices[i].getZ() ) );
	} 

	return  bounding_box(points.begin(), points.end());

}

Kernel::Iso_cuboid_3 AnimationGenerator::getFacetBoundingBox(Facet facet)
{
	std::vector<Point_3> points;
	Polyhedron::Halfedge_around_facet_circulator facetCirculator;
	 
	
	facetCirculator = facet.facet_begin();

	do {
		points.push_back(facetCirculator->vertex()->point());
	} while ( ++facetCirculator != facet.facet_begin() );

	return  bounding_box(points.begin(), points.end());

}

void AnimationGenerator::getFourBoxPlaneIntersections(Kernel::Iso_cuboid_3 cuboid, 
														  Plane_3 plane, 
														  std::vector<Point_3> &result, 
														  std::vector<ovInt> &coords)
{
	Kernel::FT toX, toY, toZ;
	Vector_3 orthogonalVector;
//	std::vector<Point_3> result;
	Point_3 cutDirections[8];
	Vector_3 shiftPosition;
	CGAL::Object intersectionResult;
	Point_3 intersectionPoint;

	result.clear();
	coords.clear();

	orthogonalVector = plane.orthogonal_vector();

	// decide which plane should we proyect to
	
	toX = orthogonalVector.x() * orthogonalVector.x();
	toY = orthogonalVector.y() * orthogonalVector.y();
	toZ = orthogonalVector.z() * orthogonalVector.z();

	if (toX >= toY && toX >= toZ) {
		coords.push_back( 1 );
		coords.push_back( 2 );
		shiftPosition = Vector_3(1,0,0);
		cutDirections[0] = cuboid[0];
		cutDirections[1] = cuboid[1];
		cutDirections[2] = cuboid[3];
		cutDirections[3] = cuboid[2];
		cutDirections[4] = cuboid[5];
		cutDirections[5] = cuboid[6];
		cutDirections[6] = cuboid[4];
		cutDirections[7] = cuboid[7];
	}
	else if (toY >= toX && toY >= toZ) {
		coords.push_back( 0 );
		coords.push_back( 2 );
		shiftPosition = Vector_3(0,1,0);		
		cutDirections[0] = cuboid[1];
		cutDirections[1] = cuboid[2];
		cutDirections[2] = cuboid[0];
		cutDirections[3] = cuboid[3];
		cutDirections[4] = cuboid[6];
		cutDirections[5] = cuboid[7];
		cutDirections[6] = cuboid[5];
		cutDirections[7] = cuboid[4];
	}
	else {
		coords.push_back( 0 );
		coords.push_back( 1 );
		shiftPosition = Vector_3(0,0,1);
		cutDirections[0] = cuboid[1];
		cutDirections[1] = cuboid[6];
		cutDirections[2] = cuboid[0];
		cutDirections[3] = cuboid[5];
		cutDirections[4] = cuboid[2];
		cutDirections[5] = cuboid[7];
		cutDirections[6] = cuboid[3];
		cutDirections[7] = cuboid[4];
	}

	for (ovInt i=0; i < 8; i+=2) {

		
		if ( cutDirections[i] == cutDirections[i+1] ) {

			cutDirections[i+1] = cutDirections[i+1] + shiftPosition;

		}

		CGAL::Segment_3<Kernel> cubeEdge(cutDirections[i], cutDirections[i+1]);
		intersectionResult = CGAL::intersection(plane, cubeEdge.supporting_line());

		if ( CGAL::assign(intersectionPoint, intersectionResult) ) { // there should always be an intersection
			result.push_back(intersectionPoint);
		}	
	}
	//return result;
}


void AnimationGenerator::stopGenerating()
{
	_stopAnimating = true;

}

Kernel::Iso_cuboid_3 AnimationGenerator::getNodeBoundingBox(Node node)
{
	std::vector<Point_3> points;
	ovVector3D transformedPoint;

	for (ovUInt i=0; i < node.instanceGeometries.size(); i++) {
		for(ovUInt j=0; j < node.instanceGeometries[i].geometry->objects.size(); j++) {
			for(ovInt k=0; k < node.instanceGeometries[i].geometry->objects[j].sourcePositionCount; k++) {

				// starting point
				
				transformedPoint = node.applyTransformationToPos(i, j, k);
				points.push_back( Point_3( transformedPoint.getX(), 
										   transformedPoint.getY(), 
										   transformedPoint.getZ() ) );
			}
		}
	}
	
	return  bounding_box(points.begin(), points.end());

}

ovFloat AnimationGenerator::randFloat(ovFloat min, ovFloat max)
{

	ovFloat randNumber;

	randNumber = rand()/(float(RAND_MAX)+1); 

	if ( min > max ) {
        return randNumber*(min-max)+max;    
    }
    else {
        return randNumber*(max-min)+min;
    }    

}

void AnimationGenerator::animate()
{
	_semaphore->release();
}


#if 0
void AnimationGenerator::startBubbles()
{
	_workOn = AnimateBubbles;
	_semaphore->release();
}

std::vector<ovTuple3f> AnimationGenerator::getBubbles()
{
	std::vector<ovTuple3f> bubbles;
	bubbles = _bubblePositions;
	_bubbleSemaphore->release();
	return bubbles;

}

#endif

ObjectPropertyType AnimationGenerator::getDistanceType(ovBool isBubble)
{
	if ( isBubble ) {
		return bubbleDistance;
	}
	else {
		return localParticleDistance;
	}

}

void AnimationGenerator::reassignSmoothLength(CPrtlFluid* fluid, SimuValue newSmoothLength)
{
	for (int i = 0; i < fluid->m_pfPrtls.m_paNum; i++) {
		
		CFluidPrtl *pPrtl = fluid->m_pfPrtls[i];
		pPrtl->m_pfSmoothLen = newSmoothLength;
	}

}
