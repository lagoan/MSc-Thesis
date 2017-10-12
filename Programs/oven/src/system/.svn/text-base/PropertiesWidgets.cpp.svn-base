/*
 *  PropertiesWidgets.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 19/09/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#include "PropertiesWidgets.h"
#include "SimulationProperties.h"
#include "LoggingSystem.h"


PropertiesDelegate::PropertiesDelegate(QObject *parent)
     : QItemDelegate(parent) {}

QWidget* PropertiesDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &, 
					  const QModelIndex &index) const
{
	
	if (PROPERTIES->selectedNodeType == SceneNode) {
		
		ScenePropertyType propertyType = SceneProperties::getPropertiesType(index.row());

		switch( propertyType ) {
			case startFrame:
			case endFrame:
			case currentFrame:
			case liquidBubbleInteractionFrequency:
			case framesPerSecond:
			case stepsPerFrame:
				{
				QSpinBox* editor = new QSpinBox(parent);
				editor->setMaximum(1e6);
				return editor;
			}
			case endTime:
			case isoDensityRatio:
			case meshGridSizeOverParticleDistance:
			case isoDensityRadiusRatio:
			case particleDistance:
			case minTemp:
			case maxTemp:
			case startCookTime:
				{
				QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
				editor->setMaximum(1e8);
				editor->setDecimals(7);
				return editor;
			}
			case useParticleNumberDensity:
			case renderSurface:
			case performBaking:
			case ambientHeatTransfer:
				{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "True");
				editor->insertItem(1, "False");
				
				if (propertyType == renderSurface) {
					editor->setCurrentIndex(0); // set to false
				}
				return editor;

			}

		}
	}
	else if (PROPERTIES->selectedNodeType == GeometricObjectNode) {

		ObjectPropertyType propertyType = ObjectProperties::getPropertiesType(index.row());

		switch( propertyType ) {
		
			
			case surfaceTensionValue:
			case massValue:
			case densityValue:
			case localParticleDistance:
			case particlePositionJitter:
			case shearModulus:
			case relaxationTime:
			case frictionRate:
			case distanceDumpingRate:
			case energyDumpingRate:
			case soundSpeed:
			case velocityTuner:
			case courantFactor:
			case smoothLengthRatio:
			case surfaceDensityThreshold:
			case cgTolerance:
			case positionJitter:
			case maxSizeScale:
			case minSizeScale:
			case bubbleDistance:
			case bubbleJitter:
			case co2EnoughForBubble:
			case co2PeakGeneration:
			case co2TemperatureMean:
			case co2TemperatureRange:
			case heatConductivity:
			{
				QDoubleSpinBox* editor = new QDoubleSpinBox(parent);
				editor->setMaximum(1e10);
				editor->setDecimals(7);
				return editor;
			}

			case co2BubblesPerParticle:
			{
				QSpinBox* editor = new QSpinBox(parent);
				editor->setMaximum(1e6);
				return editor;
			}

			case objectType:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Passive");
				editor->insertItem(1, "Solid");
				editor->insertItem(2, "Fluid");
				editor->insertItem(3, "Cut out");
				return editor;
				
			}
			case initializeParticle:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Density");
				editor->insertItem(1, "Mass");
				return editor;
			}
			case particleFluidType:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Upsample");
				editor->insertItem(1, "SPH");
				editor->setCurrentIndex(0);
				return editor;
			}
			case fluidCompressibility:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Poisson equation");
				editor->insertItem(1, "Gas state equation");
				editor->insertItem(2, "No compressibility");
				return editor;
			}
			case fluidType:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Non Newtonian");
				editor->insertItem(1, "Newtonian");
				return editor;
			}
			case tensorIntegration:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Explicit");
				editor->insertItem(1, "Implicit");
				return editor;
			}
			case addBubblesByTotalOrRate:
			{	
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "Number of bubbles");
				editor->insertItem(1, "CO2 per bubble");
				return editor;
			}
			case fixedParticles:
			case applyGravity:
			case fixedMotion:
			case applySurfaceTension:
			case bubblyFluid:
			case addBubblesFromCO2:
			case generateCO2:
			{
				QComboBox* editor = new QComboBox(parent);
				editor->insertItem(0, "True");
				editor->insertItem(1, "False");
				
				if (propertyType == applySurfaceTension ||
					propertyType == fixedMotion         ||
					propertyType == fixedParticles      ||
					propertyType == bubblyFluid         ){
						editor->setCurrentIndex(0);
				}

				return editor;
			}

			

		}

	}

	// should never execute this code
	QLineEdit* editor = new QLineEdit(parent);
	return editor;

}

void PropertiesDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

	QSpinBox *spinBoxEdit = qobject_cast<QSpinBox*>(editor);
	if (spinBoxEdit) {
		spinBoxEdit->setValue(index.model()->data(index, Qt::EditRole).toInt());
	}
	else {
		QDoubleSpinBox *doubleSpinBoxEdit = qobject_cast<QDoubleSpinBox*>(editor);
		if (doubleSpinBoxEdit) {
			doubleSpinBoxEdit->setValue(index.model()->data(index, Qt::EditRole).toDouble());
		}
		else {
			QComboBox *comboBoxEdit = qobject_cast<QComboBox*>(editor);
			if (comboBoxEdit) {
				comboBoxEdit->setCurrentIndex( comboBoxEdit->findText( index.model()->data(index, Qt::EditRole).toString() ) );
			}
		}
	}
	
	

}

/* setModelData will write the change to the property value in the singleton
 * PROPERTIES
 */

void PropertiesDelegate::setModelData(QWidget *editor, QAbstractItemModel *model,
									  const QModelIndex &index) const
{


	QSpinBox *edit = qobject_cast<QSpinBox *>(editor);
	if (edit) {
		model->setData(index, edit->value());
		PROPERTIES->updateValuesInt(index.row() , (int)edit->value() );
	}
	else {
		QDoubleSpinBox *edit = qobject_cast<QDoubleSpinBox *>(editor);
		if (edit) {
			model->setData(index, edit->value());
			PROPERTIES->updateValuesFloat(index.row() , (float)edit->value() );
		}
		else {
			QComboBox *edit = qobject_cast<QComboBox *>(editor);
			if (edit) {
				//model->setData(index, edit->currentIndex());

				PROPERTIES->updateValuesCombo(index.row() , (int)edit->currentIndex() );

				if (PROPERTIES->selectedNodeType == SceneNode) {
					model->setData(index, 
						PROPERTIES->getSceneProperties().getTextValue( (ScenePropertyType) index.row() )
					);

				}
				else if (PROPERTIES->selectedNodeType == GeometricObjectNode) { 
					model->setData(index, 
						PROPERTIES->getObjectProperties(PROPERTIES->selectedObject)->getTextValue( (ObjectPropertyType) index.row() )
					);
				}
				
			}
		}
	}

	
	
	
}



void PropertiesWidgets::generate()
{

	QTableWidgetItem* _newTableWidgetItem;

	// clear previous properties, if any
	clear();

	// generate scene widgets


	// scene spin boxes
	
	std::map<ScenePropertyType, ovInt> _intSceneProperties; 
	_intSceneProperties = PROPERTIES->getSceneProperties().getValuesInt();

	if ( _intSceneProperties.size() ) {
		std::map<ScenePropertyType, ovInt>::iterator iterIntScene;
		iterIntScene = _intSceneProperties.begin() ;

		while (iterIntScene != _intSceneProperties.end() ) {

			_newTableWidgetItem = new QTableWidgetItem;
			sceneValues.insert( 
				std::pair<ScenePropertyType, QTableWidgetItem*>( iterIntScene->first, _newTableWidgetItem ) );
			
			iterIntScene++;
		}
	}

	// scene double spinboxes

	std::map<ScenePropertyType, ovFloat> _floatSceneProperties; 
	_floatSceneProperties = PROPERTIES->getSceneProperties().getValuesFloat();

	if ( _floatSceneProperties.size() ) {
		std::map<ScenePropertyType, ovFloat>::iterator iterFloatScene;
		iterFloatScene = _floatSceneProperties.begin() ;

		while (iterFloatScene != _floatSceneProperties.end() ) {

			_newTableWidgetItem = new QTableWidgetItem;
			sceneValues.insert( 
				std::pair<ScenePropertyType, QTableWidgetItem*>( iterFloatScene->first, _newTableWidgetItem ) );
			
			iterFloatScene++;
		}
	}

	// scene combo boxes (boolean)

	std::map<ScenePropertyType, ovBool> _boolSceneProperties; 
	_boolSceneProperties = PROPERTIES->getSceneProperties().getValuesBool();

	if ( _boolSceneProperties.size() ) {

	std::map<ScenePropertyType, ovBool>::iterator iterBoolScene;
	iterBoolScene = _boolSceneProperties.begin() ;

		while (iterBoolScene != _boolSceneProperties.end() ) {

			_newTableWidgetItem = new QTableWidgetItem;
			
			sceneValues.insert( 
				std::pair<ScenePropertyType, QTableWidgetItem*>( iterBoolScene->first, _newTableWidgetItem ) );
			
			iterBoolScene++;
		}
	}



	// scene tree widget items (tags)

	for (ovInt i = 0; i < PROPERTIES->getSceneProperties().getPropertiesTypeCount(); i++ ) {

		_newTableWidgetItem = new QTableWidgetItem( SceneProperties::getName( PROPERTIES->getSceneProperties().getPropertiesType(i) ) );
		_newTableWidgetItem->setFlags(Qt::ItemIsEnabled);
		sceneQTableWidgetItem.insert(
			std::pair<ScenePropertyType, QTableWidgetItem*>( PROPERTIES->getSceneProperties().getPropertiesType(i), _newTableWidgetItem ) ); 
	
	}	

	// scene custom widgets
	

	// generate object widgets
	// we assume all objects have the same properties

	if ( PROPERTIES->containsObjects() ) {

		ovInt glName;
		glName = PROPERTIES->getGLName();

	
		// object spin boxes

		std::map<ObjectPropertyType, ovInt> _intObjectProperties; 
		_intObjectProperties = PROPERTIES->getObjectProperties(glName)->getValuesInt();
		
		if ( _intObjectProperties.size() ) {
			std::map<ObjectPropertyType, ovInt>::iterator iterIntObject;
			
			iterIntObject = _intObjectProperties.begin() ;

			while (iterIntObject != _intObjectProperties.end() ) {
				objectValues.insert( 
					std::pair<ObjectPropertyType, QTableWidgetItem*>( iterIntObject->first, new QTableWidgetItem ) );
				
				++iterIntObject;
			}
		}
		// object double spinboxes

		std::map<ObjectPropertyType, ovFloat> _floatObjectProperties; 
		_floatObjectProperties = PROPERTIES->getObjectProperties(glName)->getValuesFloat();


		if ( _floatObjectProperties.size() ) {
			std::map<ObjectPropertyType, ovFloat>::iterator iterFloatObject;
			
			
			iterFloatObject = _floatObjectProperties.begin() ;
			
			
			while ( iterFloatObject != _floatObjectProperties.end() ) {
				objectValues.insert( 
					std::pair<ObjectPropertyType, QTableWidgetItem*>( iterFloatObject->first, new QTableWidgetItem ) 
				);
						
				++iterFloatObject;
			}
		}

		// object combo boxes (boolean)

		std::map<ObjectPropertyType, ovBool> _boolObjectProperties; 
		_boolObjectProperties = PROPERTIES->getObjectProperties(glName)->getValuesBool();

		if ( _boolObjectProperties.size() ) {
			std::map<ObjectPropertyType, ovBool>::iterator iterBoolObject;

			
			iterBoolObject = _boolObjectProperties.begin() ;
			

			while (iterBoolObject != _boolObjectProperties.end() ) {
				objectValues.insert( 
					std::pair<ObjectPropertyType, QTableWidgetItem*>( iterBoolObject->first, new QTableWidgetItem ) );
				
				++iterBoolObject;
			}
		}


		// object tree widget items (tags)

		for (ovInt i = 0; i < ObjectProperties::getPropertiesTypeCount(); i++ ) {

			_newTableWidgetItem = new QTableWidgetItem( ObjectProperties::getName( ObjectProperties::getPropertiesType(i) ) );
			_newTableWidgetItem->setFlags(Qt::ItemIsEnabled);
			objectQTableWidgetItem.insert(
				std::pair<ObjectPropertyType, QTableWidgetItem*>( ObjectProperties::getPropertiesType(i), _newTableWidgetItem ) ); 
		
		}	

		// object custom widgets
		
		// objectType property

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( objectType, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( particleFluidType, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( fluidType, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( tensorIntegration, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( initializeParticle, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( fluidCompressibility, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( fixedParticles, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( applyGravity, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( fixedMotion, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( applySurfaceTension, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( bubblyFluid, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( addBubblesFromCO2, new QTableWidgetItem ) );

		objectValues.insert( 
			std::pair<ObjectPropertyType, QTableWidgetItem*>( addBubblesByTotalOrRate, new QTableWidgetItem ) );




	}
}

void PropertiesWidgets::clear()
{
	

	// scene values
	std::map<ScenePropertyType, QTableWidgetItem*>::iterator iterTableWidget;
	iterTableWidget = sceneValues.begin() ;

	while (iterTableWidget != sceneValues.end() ) {
		delete iterTableWidget->second;
		iterTableWidget++;
	}
	sceneValues.clear();

	// scene 
	
	iterTableWidget = sceneQTableWidgetItem.begin() ;

	while (iterTableWidget != sceneQTableWidgetItem.end() ) {
		delete iterTableWidget->second;		
		iterTableWidget++;
	}
	sceneQTableWidgetItem.clear();


	// object values 
	std::map<ObjectPropertyType, QTableWidgetItem*>::iterator iterTableWidgetObject;
	iterTableWidgetObject = objectValues.begin() ;

	while (iterTableWidgetObject != objectValues.end() ) {
		delete iterTableWidgetObject->second;
		iterTableWidgetObject++;
	}
	objectValues.clear();

	// objects

	
	iterTableWidgetObject = objectQTableWidgetItem.begin() ;

	while (iterTableWidgetObject != objectQTableWidgetItem.end() ) {
		delete iterTableWidgetObject->second;		
		iterTableWidgetObject++;
	}
	objectQTableWidgetItem.clear();


}
