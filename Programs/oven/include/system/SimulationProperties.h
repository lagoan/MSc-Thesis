/*
 *  SimulationProperties.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 10/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SIMULATIONPROPERTIES_H
#define SIMULATIONPROPERTIES_H

#include "types.h"
#include "scene/Scene.h"
#include "BaseProperties.h"
#include "LoggingSystem.h"
#include "ColorPalette.h"

#include <map>
#include <QObject>
#include <QString>
#include <QWidget>
#include <QReadWriteLock>





enum ScenePropertyType {
	startFrame = 0,
	endFrame,
	endTime,
	currentFrame,
	framesPerSecond,
	stepsPerFrame,
	renderSurface,
	particleDistance,
	useParticleNumberDensity,
	isoDensityRatio,
	meshGridSizeOverParticleDistance,
	isoDensityRadiusRatio,
	liquidBubbleInteractionFrequency,
	ambientHeatTransfer,
	performBaking,
	minTemp,
	maxTemp,
	startCookTime,

	
};

class SceneProperties : public BaseProperties<ScenePropertyType>
{
public:
	
	static ovBool renderCutOuts;
	
	SceneProperties() {
		_customWidgetCount = 0;
		reset();
	}
	
	void reset();
	static QString getName(ScenePropertyType scenePropertyType);
	
	
	SceneProperties& operator=(const SceneProperties& prop);
	SceneProperties(const SceneProperties &prop);

	QString getTextValue( ScenePropertyType scenePropertyType );

	static ovInt getPropertiesTypeCount(){ return _properties.size(); };
	static ScenePropertyType getPropertiesType(ovInt i){ return _properties[i]; };
	void setc(ScenePropertyType type, ovInt index);


	

protected:
	static std::vector<ScenePropertyType> initializeProperties();

private:
	static std::vector<ScenePropertyType> _properties;



};

inline SceneProperties& SceneProperties::operator=(const SceneProperties& prop)
{
	_valuesInt		= prop._valuesInt;
	_valuesFloat	= prop._valuesFloat;
	_valuesBool		= prop._valuesBool;

	return (*this);
}



enum ObjectType {
	Passive = 0,
	Solid,
	Fluid,
	CutOut
};

enum ParticleFluidType {
	Upsample = 0,
	SPH
};

enum FluidCompressibility {
	PoissonEquation = 0,
	GasStateEquation,
	NoCompressibility
};

enum FluidType {
	NonNewtonian = 0,
	Newtonian
};

enum TensorIntegration {
	Explicit = 0,
	Implicit
};

enum InitializeParticle {
	DensityValue = 0,
	MassValue
};

enum AddBubblesByTotalOrRate {
	ByTotalNumber = 0,
	ByRate
};

enum ObjectPropertyType {
	objectType = 0,
	particleFluidType,
	fluidCompressibility,
	fluidType,
	tensorIntegration,
	fixedParticles,
	applyGravity,
	fixedMotion,
	applySurfaceTension,
	surfaceTensionValue,
	initializeParticle,
	massValue,
	densityValue,
	localParticleDistance,
	particlePositionJitter,
	shearModulus,
	relaxationTime,
	frictionRate,
	distanceDumpingRate,
	energyDumpingRate,
	soundSpeed,
	velocityTuner,
	courantFactor,
	smoothLengthRatio,
	surfaceDensityThreshold,
	cgTolerance,
	positionJitter,
	maxSizeScale,
	minSizeScale,
	bubblyFluid,
	bubbleDistance,
	bubbleJitter,
	addBubblesFromCO2,
	addBubblesByTotalOrRate,
	co2BubblesPerParticle,
	co2EnoughForBubble,
	co2PeakGeneration,
	co2TemperatureMean,
	co2TemperatureRange,
	heatConductivity,
	generateCO2

};


class ObjectProperties : public BaseProperties<ObjectPropertyType>
{
public:
	ObjectProperties() { 
		_customWidgetCount = 1;
		_color = ColorPalette::objectPassive;
		reset();
	}

	void reset();
	static QString getName(ObjectPropertyType objectPropertyType);

	ObjectType				getObjectType();
	ParticleFluidType		getParticleFluidType();
	FluidCompressibility	getFluidCompressibility();
	FluidType				getFluidType();
	TensorIntegration		getTensorIntegration();
	InitializeParticle		getInitializeParticle();
	ovTuple3f				getColor();
	AddBubblesByTotalOrRate getBubbleGenerationType();

	void setObjectType(ObjectType);
	void setColor(ovTuple3f);
	void setColor(ovFloat r, ovFloat g, ovFloat b);

	QString getTextValue( ObjectPropertyType objectPropertyType );

	ObjectProperties operator=(const ObjectProperties& prop);
	ObjectProperties(const ObjectProperties &prop);

	static ovInt getPropertiesTypeCount(){ return _properties.size(); };
	static ObjectPropertyType getPropertiesType(ovInt i){ return _properties[i]; };
	
	// set custom variable (enum)
	void setc(ObjectPropertyType type, ovInt index);

protected:
	static std::vector<ObjectPropertyType> initializeProperties();

private:
	static std::vector<ObjectPropertyType> _properties;
	QMutex				_objectTypeLock;
	
	ObjectType					_objectType;
	ParticleFluidType			_particleFluidType;
	FluidCompressibility		_fluidCompressibility;
	FluidType					_fluidType;
	TensorIntegration			_tensorIntegration;
	InitializeParticle			_initializeParticle;
	ovTuple3f					_color;
	AddBubblesByTotalOrRate		_addBubblesByTotalOrRate;
	
};

inline ObjectProperties ObjectProperties::operator=(const ObjectProperties& prop)
{
	_valuesInt		= prop._valuesInt;
	_valuesFloat	= prop._valuesFloat;
	_valuesBool		= prop._valuesBool;

	return (*this);
}





/*
 * The SimulationProperties class will hold the properties of the fluid 
 * simulation for all the objects in the scene, a map will store the values
 * for the geometric objects. This map have as keys the glName of each object
 *
 */

#define PROPERTIES SimulationProperties::getInstance()

class SimulationProperties : public QObject
{
Q_OBJECT

public:

	// temp
	NodeType selectedNodeType;
	ovInt selectedObject;
	ovBool processingAnimation;

	static SimulationProperties* getInstance();
	void reset();
	void initialize(Scene*);
	
	void setSceneProperties(SceneProperties);
	void addObjectProperties(ovInt glName, ObjectProperties*);


	SceneProperties& getSceneProperties();
	ObjectProperties* getObjectProperties(ovInt glName);
	ovInt getGLName(){ return _objectsProperties.begin()->first; };
	ovBool containsObjects(){ return _objectsProperties.size() ? true : false;};
	

	void updateValuesInt(ovInt row , ovInt value );
	void updateValuesFloat(ovInt row , ovFloat value );
	void updateValuesCombo(ovInt row, ovInt index );

signals:
	void updatedValues();

protected:
	SimulationProperties();
	~SimulationProperties();
	
private:

	SceneProperties _sceneProperties;
	std::map<ovInt, ObjectProperties*> _objectsProperties;
	

	SimulationProperties(const SimulationProperties &s);
	SimulationProperties &operator = (const SimulationProperties &s);
	 
};


#endif