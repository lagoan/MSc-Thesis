/*
 *  MainWindow.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 02/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <map>
#include <vector>
#include <list>
#include <utility>

#include "ViewPort.h"
//#include "integrationSample.h"
#include "ui_MainWindow.h"
#include "SimulationProperties.h"
#include "PropertiesWidgets.h"
#include "AnimationGenerator.h"
#include "integration/integrationHelper.h"

#include <QtGui>
#include <QThread>
#include <QImage>
#include <dae.h>
#include <dom/domCOLLADA.h>
#include <dom/domProfile_COMMON.h>
#include <dom/domConstants.h>


#define MAX_FRAMES		9999
#define INIT_END_FRAMES	255

class Scene;


class LoadThread : public QThread
{
	Q_OBJECT

public:
	LoadThread(){_semaphore = new QSemaphore(0); 
				 _stop = false;
				 
	};
	~LoadThread(){ delete _semaphore;};
	void setDAEObject(DAE* daeObject){ _daeObject = daeObject; };
	void setFileName(QString daeFileName){_daeFileName = daeFileName;};
	void stop(){_stop = true; _semaphore->release();};
	Scene getScene();
	

signals:
	void finished();
	void finishedError(int);

public slots:
	void get();

	
protected:
	void run();

private:
	DAE*	_daeObject;
	QString	_daeFileName;
	Scene*  _scene;					// for handling signals
	QMutex  _sceneMutex;			// for locking the use of _scene
	QSemaphore* _semaphore;
	volatile ovBool _stop;
	
	Scene convertModel(domCOLLADA& root);
		

};

/* SaveThread will wait for a scene, file name and animation frame to be set up
 * and the will save the file on the destination folder
 */

class SaveThread : public QThread
{
	Q_OBJECT

public:
	SaveThread();
	~SaveThread();
	void setScene(Scene );
        void saveFrame(AnimationFrame, QHash<QString, QImage> , QString );
	void stop();

	void setDAEObject(DAE* daeObject){ _daeObject = daeObject; };
	void setDAEFileName(QString name){ _daeFileName = name; };

signals:
	void finished();

public slots:
	//void save();
	


protected:
	void run();

private:
	
        Scene                               _scene;
        std::list<QString>                  _fileNames;
        std::list<AnimationFrame>           _animationFrames;
        std::list< QHash<QString, QImage> > _images;
	
	QMutex		_sceneMutex, _frameMutex;
	QSemaphore	_semaphore;
	DAE*		_daeObject;
	DAE*		_currentDae;
	volatile ovBool _stop;

        AnimationFrame          _workingAnimationFrame;
        QString                 _workingFileName;
        QString                 _daeFileName;
        QHash<QString, QImage>	_currentImageHash;

	daeElement* particlePositionElement;
	daeElement* particleTemperatureElement;
	daeElement* particleMassElement;
	daeElement* particleDensityElement;
	daeElement* particleOriginalDensityElement;
	daeElement* particleSmoothingLengthElement;
	daeElement* particleMassDensityRatio;
	daeElement* particleVisible;
	daeElement* particleTrueSurfaceArray;	
	daeElement* particleTrueSurfaceTimeArray;

	daeElement* bubblePositionElement;
	daeElement* bubbleTemperatureElement;
	daeElement* bubbleMassElement;
	daeElement* bubbleDensityElement;
	daeElement* bubbleOriginalDensityElement;
	daeElement* bubbleDensityAddedElement;
	daeElement* bubbleSizeElement;
	daeElement* bubbleMassDensityRatio;
	daeElement* bubbleVisible;
	daeElement* bubbleTrueSurfaceArray;
	daeElement* bubbleTrueSurfaceTimeArray;
	daeElement* particleNormalElement;
	daeElement* bubbleNormalElement;
	daeElement* bubbleSmoothingLengthElement;
	
	void updateFluidParticlesOnCollada();
	void updateFluidGeometryOnCollada();
	void saveFluidInformationToFile();
	void releaseElements();


};

/* The PropertiesWidgets class will work as a container for the property 
 * widgets that will be used in the program. The scene widgets will always be 
 * "connected" to the scene variables; the object widgets will have to change 
 * from object to object when selection changes. Connections need to be done 
 * by hand. The generation of the widgets should be done after the PROPERTIES
 * have been initialized.
 */




class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT
 
public:
    MainWindow();
	~MainWindow();

public slots:
	void updateQSliderFrames();
	void updateQSpinBoxFrames();
	void updateObjectInspector(ovInt);
	void updatePropertyEditor(ovInt);
	void setSelectedProperties( QTreeWidgetItem* , QTreeWidgetItem* );
	void loadFile();
	void saveFrames();
	void setScene();
	void printSceneProperties();
	void printAnimationFrameProperties(AnimationFrame &);
	void printAnimationFrameProperties(ovUInt frameNumber);
	void getError(int);
	void startBaking();
	void getFrame();
	void setAnimation(int milliseconds);
	void setNextFrame();
	void playAnimation();
	void showLastFrame();
	void showFirstFrame();
	void displayErrorMessage(QString message);
	void setLiveSave();
	
	void setParticleRenderingTypePosition(bool);
	void setParticleRenderingTypeTemperature(bool);
	void setParticleRenderingTypeSize(bool);
	void setParticleRenderingTypeCO2(bool);
	void setParticleRenderingTypeOnSurface(bool);
	void setParticleRenderingTypeInCollision(bool);
	void setParticleRenderingTypeTrueSurface(bool);
	void updateSplatRadius(double);
	void updateSplatRadius(int);
        void updateCameraValueWidgets();
        void updateCameraValues(double);

private:
	
	// tool box widgets
	
	QSlider*		  _qSliderFrames;
	QSpinBox*		  _qSpinBoxFrames;
	QTreeWidgetItem*  _qTreeWidgetItem;
	QTimer*			  _qFrameTimer;
	
	QString		_daeFileName;
	Scene*		_scene;		
	DAE*		_daeObject;

	PropertiesWidgets propertiesWidgets;
	
	LoadThread		   _loadThread;
	SaveThread		   _saveThread;
	AnimationGenerator _animationGenerator;
	ovBool			   _animating;
	ovBool			   _playingAnimation;
	ovBool			   _liveSave;

        Camera _currentCamera;

	// for saving files
	//int currentWorkingFrame;
	QString path;
	QString identificator;
	QString length;

	
	std::vector<AnimationFrame> _animationFrames;
	ovDouble _lastFrameTime;

	QString  getFileName(QString label);
	QString  getDirectoryPath();
	QString  getDAEPath();
	QString  getSystemPath();

	QAction *_lastParticleActionType;

	void finishSettingGUI();
	void resetObjectInspector();
	void generateObjectList(Node* node, QTreeWidgetItem *treeItem);
	void clearData();

	void addBubbleToCollada();
	void updateFluidGeometryOnCollada(ovInt frameNumber);
	void setParticleRenderingTypeGeneral(QAction*, ParticleRenderingType, bool toggled);
	

	//HACK
	std::vector<bool> stillInside;
	
 
};
 
 
#endif
