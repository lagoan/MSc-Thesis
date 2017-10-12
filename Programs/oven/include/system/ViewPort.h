/*
 *  ViewPort.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 02/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef VIEWPORT_H
#define VIEWPORT_H

#include <QtGui>

#ifdef Q_OS_WIN
#include <GL/glew.h> 
#include <GL/wglew.h> 
#endif

#ifdef Q_OS_LINUX
//#include <GL/glew.h> 

#endif


#include <QGLWidget>
#include <QMutex>
#include <QHash>
#include <QImage>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
//void setupExtensions()
//{ shading_enabled = true; }; // OS X already has these extensions
#endif

#include "LoggingSystem.h"
#include "AnimationGenerator.h"
#include "primitives/Matrix.h"
#include "primitives/ovVector3D.h"
#include "scene/Scene.h"
#include "ArcBall.h"
#include "types.h"

class Camera 
{
public:
	ovTuple3f position;
	Matrix	  rotation;
	QPoint    lastPos; // 2d position on screen
};


enum ParticleRenderingType {
	particlePosition = 0, 
	particleSize,
	particleTemperature,
	particleCO2,
	particleOnSurface,
	particleInCollision,
	particleTrueSurface
	
};

class ViewPort : public QGLWidget
{
    Q_OBJECT

public:

	

    ViewPort(QWidget* parent = 0);
	~ViewPort(){};
	void setScene(Scene *scene){ _scene = *scene; updateGL();};
	void setFrame(AnimationFrame& frame);
	void setSplatRadius(double newRadius);
        Camera getCamera() {return _camera; };
        void setCamera(Camera newCamera);
        QHash<QString, QImage> getImages();

public slots:
	void updateViewPort();
	void setSelectedObject( QTreeWidgetItem* , QTreeWidgetItem* );
	void setViewParticles(bool show);
	void setViewSurface(bool show);
	void setViewCutOuts(bool show);
	void setViewBubbleParticles(bool show);
	void setViewGradient(bool show);
	void setViewVelocity(bool show);
	void setParticleRenderingType( ParticleRenderingType type );

protected:
    void initializeGL();
    void resizeGL(ovInt width, ovInt height);
    void paintGL();

	void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

signals:
	void selectOnObjectList(ovInt);
        void cameraChanged();
private:
    
	Scene _scene;
	
	ovUInt _listNames;
	ovUInt _currentList;

	ArcBall _arcBall;
	Camera  _camera;

	ovInt _width;
	ovInt _height;

	ovBool _showSurface;
	ovBool _showParticles;
	ovBool _showBubbleParticles;
	ovBool _showGradient;
	ovBool _showVelocity; 

	ParticleRenderingType _particleRenderingType;
	
	GLfloat _splatRadius;
	GLfloat	_near;
	GLfloat _top;
	GLfloat _bottom;
	
	QMutex _animationFrameLock;

	QTimer _qFrameTimer;

	//std::vector<AnimationFrame> animationFrame;
	AnimationFrame animationFrame;

	// vertes shaders
	GLuint _glVertexShaderTemperature;
	GLuint _glFragmentShaderTemperature;
	GLuint _glProgramTemperature;

	GLuint _glVertexShaderOnSurface;
	GLuint _glProgramOnSurface;

	GLuint _glVertexShaderCO2;
	GLuint _glProgramCO2;

	GLuint _glVertexShaderSize;
	GLuint _glProgramSize;

	GLuint _glVertexShaderTrueSurface;
	GLuint _glProgramTrueSurface;

	GLuint _glVertexShaderDensity;
	GLuint _glProgramDensity;

	GLuint _glVertexShaderTrueSurfaceTime;
	GLuint _glProgramTrueSurfaceTime;
	
	GLuint _glVertexShaderCollision;
	GLuint _glProgramCollision;
	
	void draw();
	void drawGrid();
	void buildGrid();
	ovInt pickObject(const QPoint &pos);
	ovInt processHit(ovInt hits, GLuint buffer[]);
	void setParticleRendering(std::vector< ParticleInstance* > &particleList, 
							  std::vector< ovInt > &particleCount);
	void setBubbleParticleRendering(std::vector< ParticleInstance* > &particleList, 
									std::vector< ovInt > &particleCount);
	void renderFluidParticles(const ParticleInstance *start, ovInt size, ObjectType type);
	void renderBubbleParticles(const GLvoid *start, ovInt size);
	void setupGLSL();
	void setParticleRenderingOptions(const ParticleInstance *start);
	void unsetParticleRenderingOptions();
	
	
	
	
   
};



#endif
