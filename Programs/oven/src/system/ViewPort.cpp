/*
 *  ViewPort.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 02/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */
 
#include "ViewPort.h"

#include <QtGui>
#include <QtOpenGL>
#include <cassert>

#include "system/LoggingSystem.h"
#include "system/SimulationProperties.h"
#include "system/types.h"
#include "system/constants.h"
#include "system/ColorPalette.h"


#include <iostream>


#define CAMERA_CHANGE_RATE 100.0
#define RENDERED_PARTICLES_COUNT 40000

ViewPort::ViewPort(QWidget* parent) : QGLWidget(parent)
{
    setFormat(QGLFormat(QGL::DoubleBuffer | 
                        QGL::DepthBuffer  |
                        QGL::SampleBuffers ));
	//_scene = NULL;
	
	// set initial camera position
	_camera.position.x = 0.0;
	_camera.position.y = 0.0;
	_camera.position.z = 0.0;
	_camera.rotation.loadIdentity();
	_arcBall.setBounds( this->width() ,
						this->height() );

	_splatRadius 		= 1.0f;

	_showSurface			= true;
	_showParticles			= true;
	_showBubbleParticles	= true;
	_showGradient			= true;
	_showVelocity			= true; 

	_particleRenderingType	= particlePosition;

	_qFrameTimer.start(10); 
	connect(&_qFrameTimer, SIGNAL(timeout()), this, SLOT( updateViewPort() ));

}

void ViewPort::initializeGL()
{
	// blender like background
	glClearColor(ColorPalette::background.r,
				 ColorPalette::background.g,
				 ColorPalette::background.b,
				 1.0);
	//qglClearColor(QColor(115,115,135));
    
	GLfloat light1Ambient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat light1Diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };	 
	GLfloat light1Position[]= { 20.0f, 20.0f, 20.0f, 0.0f};

	glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
	//glEnable(GL_ALPHA_TEST);
	glEnable(GL_NORMALIZE);
	//glEnable(GL_POINT_SMOOTH);
	
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	

	// light
	glEnable(GL_LIGHTING);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1Ambient);	
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1Diffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, light1Position);
	glEnable(GL_LIGHT1);
	
	glEnable(GL_COLOR_MATERIAL);
	_listNames = glGenLists(10);
	_currentList = _listNames;
	buildGrid();

	setupGLSL();


}

void ViewPort::resizeGL(ovInt width, ovInt height)
{ 
	glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    GLfloat aspect = GLfloat(width) / height;

	
	GLdouble fovY = 45.0;
	GLdouble fW, fH;
	GLdouble zNear = 0.1;
	GLdouble zFar = 10000.0;
	fH = tan( fovY / 360 * M_PI ) * zNear;	
	fW = fH * aspect;
	glFrustum( -fW, fW, -fH, fH, zNear, zFar );
	
	_width = width;
	_height = height;
	_near = zNear;
	_top = fH;
	_bottom = -fH;
	
    //glFrustum(-x, x, -1.0, 1.0, 0.1, 10000.0); 
   	//gluPerspective(45.0, x, 0.1, 10000.0 );
	glMatrixMode(GL_MODELVIEW);

	_arcBall.setBounds( this->width(),
						this->height() );
	updateViewPort();

}

void ViewPort::paintGL()
{ 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//move camera
	
	glTranslated( _camera.position.x, 
				  _camera.position.y,
				  _camera.position.z);

	// view change
	// glMultMatrixd( _camera.rotation.getM());

	// glRotatef( 45.0f, 1.0f, 0.0f, 0.0f);
	// glRotatef(-45.0f, 0.0f, 1.0f, 0.0f);
	// glTranslatef(-12.0f, -17.0f, -12.0f);

	// arcball
	glMultMatrixd( _camera.rotation.getM());

	drawGrid();
	draw();
}

void ViewPort::draw()
{ 
	
	AnimationFrame frame;

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//glColor3f(0.33f, 0.33f, 0.33f);
	_scene.render();

	// XXX temp render fluid surfaces
	
 
	if (PROPERTIES->processingAnimation) {
		
		_animationFrameLock.lock();

		// if ( /*PROPERTIES->getSceneProperties().getb( renderSurface ) && */
		// 			_showSurface == true) {
		// 			
		// 			//glColor3dv(ColorPalette::objectFluid.T);		
		// 			
		// 				//std::cout << "no of fluids " << animationFrame.listFluids.size() << "\n";
		// 			for (ovUInt i=0; i < animationFrame.listFluids.size(); i++) {
		// 					
		// 
		// 				glColor3dv( PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getColor().T );
		// 				animationFrame.listFluids[i].render();		
		// 				
		// 			}
		// 	
		// 	
		// 
		// }

		

		if ( _showParticles == true ) { 
			// lets render the particles

		
                        setParticleRendering(animationFrame.listParticles,
								 animationFrame.listParticleCount);
		}


		if ( _showBubbleParticles == true ) {
			setBubbleParticleRendering(animationFrame.listBubbles,
									   animationFrame.listBubblesCount);
		}
		

		_animationFrameLock.unlock();
		
	}
	
	glPopMatrix();
	
}

void ViewPort::drawGrid()
{ 
	glDisable(GL_LIGHTING);
	glCallList(_listNames);
	glEnable(GL_LIGHTING);
	
}



void ViewPort::buildGrid()
{
	glNewList(_currentList++ ,GL_COMPILE);
	ovInt i,j;
	ovFloat pos = 0;
	

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	glRotatef(90, 1.0, 0.0, 0.0);
	
	glScalef(4,4,1);
	glBegin(GL_LINES);

	glColor3dv(ColorPalette::gridLine.T);

	for (i=0; i<4; i++) {
		if (i==2){
			glColor3dv(ColorPalette::gridRedLine.T);
		}
		glVertex3f(i-2, -2, 0);
		glVertex3f(i-2, +2, 0);
		
		pos =0.20;
		glColor3dv(ColorPalette::gridSmallLine.T);
		for (j=0; j<4; j++) {
			glVertex3f(i-2+pos, -2, 0);
			glVertex3f(i-2+pos, +2, 0);
			glVertex3f(-2, i-2+pos, 0);
			glVertex3f(+2, i-2+pos, 0);
			
			pos += .20;
		}
		
		glColor3dv(ColorPalette::gridLine.T);
		
		if (i==2) {
			glColor3dv(ColorPalette::gridBlueLine.T);
		}
		glVertex3f(-2, i-2, 0);
		glVertex3f(+2, i-2, 0);
		
		glColor3dv(ColorPalette::gridLine.T);
	}
	
	glVertex3f(i-2, -2, 0);
	glVertex3f(i-2, +2, 0);
	glVertex3f(-2, i-2, 0);
	glVertex3f(+2, i-2, 0);
		
	glEnd();
	
	glPopMatrix();
	glEndList();
}

void ViewPort::setSelectedObject( QTreeWidgetItem* current, QTreeWidgetItem* previous)
{

	ovInt i;	
	QString _name;

	if (current != NULL){
		_name = current->text(0);

		for (i = 0; i < _scene.directory.getSize(); i++) {

			if (_name == _scene.directory[i].workName) {
				// if an object is found select it
				emit selectOnObjectList( i );
				//updateGL();
				break;
			}
		}
	}
}



void ViewPort::mousePressEvent(QMouseEvent *event)
{
	ovInt _selectedObject;

    _camera.lastPos = event->pos();
	_arcBall.startRotation( event->x(), event->y());
	
	if (event->buttons() & Qt::LeftButton) {
		if (! (event->modifiers() & Qt::ALT) ) {
			_selectedObject = pickObject( event->pos() );
			
			
			if (_selectedObject != NULL_OBJECT ) {
				//_scene.selectObject( _selectedObject );
				emit selectOnObjectList( _selectedObject );
				//updateGL();
			}
		}
	}
}

void ViewPort::mouseMoveEvent(QMouseEvent *event)
{
    ovInt dx = event->x() - _camera.lastPos.x();
    ovInt dy = event->y() - _camera.lastPos.y();
	
	// check to see if we are doing camera movements (ALT button)
	if ( event->modifiers() & Qt::ALT ) {
		
		if (event->buttons() & Qt::LeftButton) {
			// () arcball rotation

			_arcBall.dragRotation( event->x(), event->y() );
			_camera.rotation = _arcBall.getM();

		} else if (event->buttons() & Qt::MidButton) {
			// + xy translation
			_camera.position.x += dx / CAMERA_CHANGE_RATE;
			_camera.position.y -= dy / CAMERA_CHANGE_RATE;

		} else if (event->buttons() & Qt::RightButton) {
			// /\ z translation
			_camera.position.z += dx / CAMERA_CHANGE_RATE;
			_camera.position.z += dy / CAMERA_CHANGE_RATE;
		
		}
                emit cameraChanged();
	} 
	
	_camera.lastPos = event->pos();

	//updateGL();
}

 void ViewPort::wheelEvent(QWheelEvent *event)
 {
	ovInt numDegrees = event->delta() / 8;
	ovInt numSteps = numDegrees / 15;
	
	// /\ z translation
	_camera.position.z += numSteps ;
        emit cameraChanged();
	//updateGL();
 }

ovInt ViewPort::pickObject(const QPoint &pos)
{
	ovInt hits;
    const int MaxSize = 512;
    GLuint buffer[MaxSize];
    GLint viewport[4];

    glGetIntegerv(GL_VIEWPORT, viewport);
    glSelectBuffer(MaxSize, buffer);
    glRenderMode(GL_SELECT);

    glInitNames();
  
	glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluPickMatrix(GLdouble(pos.x()), GLdouble(viewport[3] - pos.y()),
                  5.0, 5.0, viewport);
    GLfloat x = (GLfloat)_width / _height;
    gluPerspective(45.0, x, 0.1, 10000.0 );

    draw();

	glMatrixMode(GL_PROJECTION);
    glPopMatrix();

	hits = glRenderMode(GL_RENDER);
    if (!hits)
        return NULL_OBJECT;
	
    return processHit(hits, buffer);
}

/* http://www.lighthouse3d.com/opengl/picking/index.php?openglway3 */

ovInt ViewPort::processHit(ovInt hits, GLuint buffer[])
{

	GLuint names, *ptr, minZ,*ptrNames, numberOfNames;

	ptr = (GLuint *) buffer;
	minZ = 0xffffffff;
	
	for (ovInt i = 0; i < hits; i++) {
		names = *ptr;
		ptr++;
		if (*ptr < minZ) {
			numberOfNames = names;
			minZ = *ptr;
			ptrNames = ptr+2;
		}

		ptr += names+2;
	}

	ptr = ptrNames;

	// return the name of the object that is nearest to the viewer
	return *(ptr+numberOfNames-1);

}

void ViewPort::updateViewPort()
{
	updateGL();
}

void ViewPort::setFrame(AnimationFrame& frame)
{ 
	_animationFrameLock.lock();
	animationFrame = frame;
	_animationFrameLock.unlock();
	//updateGL();
	
}


void ViewPort::renderFluidParticles(const ParticleInstance *start, ovInt size, ObjectType type)
{

	ParticleInstance *point = (ParticleInstance*)start;

	glEnableClientState(GL_VERTEX_ARRAY);	
	glVertexPointer(3, GL_DOUBLE, sizeof(ParticleInstance), &(start->pos));

	if (type == Fluid){
		setParticleRenderingOptions(start);
		glDrawArrays(GL_POINTS, 0, size);
		unsetParticleRenderingOptions();
	} 
	else {
		glDrawArrays(GL_POINTS, 0, size);
	}


	glDisableClientState(GL_VERTEX_ARRAY);

}

void ViewPort::renderBubbleParticles(const GLvoid *start, ovInt size)
{

	glEnableClientState(GL_VERTEX_ARRAY);

	glVertexPointer(3, GL_DOUBLE, sizeof(ParticleInstance), start);
	glDrawArrays(GL_POINTS, 0, size);

	glDisableClientState(GL_VERTEX_ARRAY);

}

void ViewPort::setViewParticles(bool show)
{
	_showParticles = show;
}

void ViewPort::setViewCutOuts(bool show)
{
	SceneProperties::renderCutOuts = show;
}

void ViewPort::setViewSurface(bool show)
{
	_showSurface = show;
}

void ViewPort::setViewBubbleParticles(bool show)
{
	_showBubbleParticles = show;
}

void ViewPort::setViewGradient(bool show)
{
	_showGradient = show;
}

void ViewPort::setViewVelocity(bool show)
{
	_showVelocity = show;
}

void ViewPort::setParticleRendering(std::vector< ParticleInstance* > &particleList, 
									std::vector< ovInt > &particleCountList)
{
	//glUseProgram(_glProgramTemperature);
	glPointSize(5.0);
	//glColor3dv(ColorPalette::fluidParticle.T);
	//glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFunc (GL_ONE, GL_ONE);
	ovInt particleCount;

	for (ovUInt i=0; i < particleList.size(); i++) {
		ObjectType type = PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getObjectType();
				
		if (type == Solid) continue;		
		glColor3dv( PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getColor().T );


		particleCount = particleCountList[i];
		
	
		
		ParticleInstance *t;

		t = particleList[i];

		while (particleCount > RENDERED_PARTICLES_COUNT) {
			renderFluidParticles(t, RENDERED_PARTICLES_COUNT, type);
			particleCount -= RENDERED_PARTICLES_COUNT;
			t += RENDERED_PARTICLES_COUNT;
		}

		renderFluidParticles(t, particleCount, type);
		
		// render velocity & gradient
		ovTuple3f tempVelocity;
		ovDouble tempVelocityLength;
		#if 0
		for (size_t j = 0; j < particleCountList[i]; j++) {
			// velocity
			if (_showVelocity && particleList[i][j].visible) {
				tempVelocity.x = particleList[i][j].veloctiy.x;
				tempVelocity.y = particleList[i][j].veloctiy.y;
				tempVelocity.x = particleList[i][j].veloctiy.z;
			
				tempVelocity.x /= 10000;
				tempVelocity.y /= 10000;
				tempVelocity.z /= 10000;

				glColor3dv(ColorPalette::velocity.T);
				glBegin(GL_LINES);
				glVertex3f(particleList[i][j].pos.x,
						   particleList[i][j].pos.y,
						   particleList[i][j].pos.z);
				glVertex3f(particleList[i][j].pos.x + 3.0 * tempVelocity.x,
						   particleList[i][j].pos.y + 3.0 * tempVelocity.y,
						   particleList[i][j].pos.z + 3.0 * tempVelocity.z);
				
				glEnd();
				
			}
			// gradient
			if (_showGradient && particleList[i][j].visible) {
				glColor3dv(ColorPalette::gradient.T);
				glBegin(GL_LINES);
				glVertex3f(particleList[i][j].pos.x,
						   particleList[i][j].pos.y,
						   particleList[i][j].pos.z);
				glVertex3f(particleList[i][j].pos.x + 5.0 * particleList[i][j].gradient.x,
						   particleList[i][j].pos.y + 5.0 * particleList[i][j].gradient.y,
						   particleList[i][j].pos.z + 5.0 * particleList[i][j].gradient.z);
				
				glEnd();
			}
			/*
			if (true) {
				glColor3dv(ColorPalette::normal.T);
				glBegin(GL_LINES);
				glVertex3f(particleList[i][j].pos.x,
						   particleList[i][j].pos.y,
						   particleList[i][j].pos.z);
				glVertex3f(particleList[i][j].pos.x + particleList[i][j].normal.x / 20.0,
						   particleList[i][j].pos.y + particleList[i][j].normal.y / 20.0,
						   particleList[i][j].pos.z + particleList[i][j].normal.z / 20.0);
				glEnd();
			}
			*/
		}
		 #endif
	}

	//glDisable(GL_BLEND);
	//glEnable(GL_LIGHTING);
	//glUseProgram(0);
}

void ViewPort::setBubbleParticleRendering(std::vector< ParticleInstance* > &particleList, 
										  std::vector< ovInt > &particleCountList)
{

	//glUseProgram(_glProgramTemperature);

	glPointSize(5.0);
	//glColor3dv(ColorPalette::fluidParticle.T);
	//glDisable(GL_LIGHTING);
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	ovInt particleCount;

	for (ovUInt i=0; i < particleList.size(); i++) {
		
		glColor3d( 1.0 - PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getColor().r,
				   1.0 - PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getColor().g,
				   1.0 - PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getColor().b);
		ObjectType type = PROPERTIES->getObjectProperties(animationFrame.listGlNames[i])->getObjectType();
		
		particleCount = particleCountList[i];
		ParticleInstance *t;

		// for (int j = 0; j < particleCount; j++) {
		// 			std::cout << particleList[i][j].normal.x << " " <<
		// 						 particleList[i][j].normal.y << " " <<
		// 						 particleList[i][j].normal.z << "\n"; 
		// 		}

		t = particleList[i];
	
		while (particleCount > RENDERED_PARTICLES_COUNT) {
			renderFluidParticles(t, RENDERED_PARTICLES_COUNT, type);
			particleCount -= RENDERED_PARTICLES_COUNT;
			t += RENDERED_PARTICLES_COUNT;
		}
		
		renderFluidParticles(t, particleCount, type);
		
		// render velocity & gradient
		ovTuple3f tempVelocity;
		ovDouble tempVelocityLength;
		#if 0
		for (size_t j = 0; j < particleCountList[i]; j++) {
			// velocity
			if (_showVelocity) {
				tempVelocity.x = particleList[i][j].veloctiy.x;
				tempVelocity.y = particleList[i][j].veloctiy.y;
				tempVelocity.x = particleList[i][j].veloctiy.z;
				/*
				tempVelocityLength = sqrt(tempVelocity.x * tempVelocity.x +
										  tempVelocity.y * tempVelocity.y +
										  tempVelocity.z * tempVelocity.z );
				*/
				tempVelocity.x /= 10000;
				tempVelocity.y /= 10000;
				tempVelocity.z /= 10000;

				glColor3dv(ColorPalette::velocity.T);
				glBegin(GL_LINES);
				glVertex3f(particleList[i][j].pos.x,
						   particleList[i][j].pos.y,
						   particleList[i][j].pos.z);
				glVertex3f(particleList[i][j].pos.x + 3.0 * tempVelocity.x,
						   particleList[i][j].pos.y + 3.0 * tempVelocity.y,
						   particleList[i][j].pos.z + 3.0 * tempVelocity.z);
				
				
				glEnd();
				
			}
			// gradient
			if (_showGradient) {
				glColor3dv(ColorPalette::gradient.T);
				glBegin(GL_LINES);
				glVertex3f(particleList[i][j].pos.x,
						   particleList[i][j].pos.y,
						   particleList[i][j].pos.z);
				glVertex3f(particleList[i][j].pos.x + 5.0 * particleList[i][j].gradient.x,
						   particleList[i][j].pos.y + 5.0 * particleList[i][j].gradient.y,
						   particleList[i][j].pos.z + 5.0 * particleList[i][j].gradient.z);
				
				glEnd();
			}
		}
#endif
	}

	//glDisable(GL_BLEND);
	//glEnable(GL_LIGHTING);

	//glUseProgram(0);
}

/* setupGLSL defines the shaders for viewing particles
 * right now there is a problem for GLSL 1.20 that it only
 * allows attributes of type float. We were using ints for bools on
 * surface and true surface.
 * it would be simple enough to change them to floats though they will occupy
 * more memory, will fix them - soon -
 */


#define NumberOfLines(x)  (sizeof(x)/sizeof(x[0]))



void ViewPort::setupGLSL()
{

	const GLchar* _glVertexShaderTemperatureSource[] = {
		"uniform float splatRadius;",
		"uniform float minTemp;",
		"uniform float maxTemp;",
		"uniform float near;",
		//"uniform float width;",
		"uniform float height;",
		"uniform float top;",
		"uniform float bottom;",
		
		"attribute float visible;",
		"attribute float temp;",
		"attribute float smoothingLength;",
		"attribute vec3  prtlPos;",
		"attribute vec3  normal;",

		//"varying vec3 pixelNormal;",
		//"varying float pixelVisible;"
		"void main()",
		"{",
		"	vec4 minTempColor, maxTempColor;",
		"	float a, b, zEye;",
		"	vec3 camPos = vec3(gl_ModelViewMatrixInverse * vec4(0,0,0,1.0));"
		"	zEye = distance(vec3(gl_Vertex), camPos);"
		"	float tColor = (temp - minTemp) / (maxTemp - minTemp);", // normalization
		"	if (tColor >= 0.0 && tColor <= 0.25) {",
		"		minTempColor = vec4( 0.0, 0.0, 1.0, 1.0);",  
		"		maxTempColor = vec4( 0.0, 1.0, 1.0, 1.0);",  
		"		a = 0.0; b = 0.25;", 
		"	} else if (tColor > 0.25 && tColor <= 0.5) {",
		"		minTempColor = vec4( 0.0, 1.0, 1.0, 1.0);",  
		"		maxTempColor = vec4( 0.0, 1.0, 0.0, 1.0);",  
		"		a = 0.25; b = 0.5;",
		"	} else if (tColor > 0.5 && tColor <= 0.75) {",
		"		minTempColor = vec4( 0.0, 1.0, 0.0, 1.0);",  
		"		maxTempColor = vec4( 1.0, 1.0, 0.0, 1.0);",  
		"		a = 0.5; b = 0.75;",
		"	} else if (tColor > 0.75 && tColor <= 1.0){",
		"		minTempColor = vec4( 1.0, 1.0, 0.0, 1.0);",  
		"		maxTempColor = vec4( 1.0, 0.0, 0.0, 1.0);",  
		"		a = 0.75; b = 1.0;",
		"	}",
		//"	gl_PointSize = 5.0;",
		//"	gl_PointSize = smoothingLength * splatRadius * (near / zEye) * (height / (top - bottom));"
		"	gl_PointSize = (splatRadius /100.0) * (near / zEye) * (height / (top - bottom));"
		"	tColor  = (tColor - a) / (b - a);",
		"   gl_FrontColor = ((maxTempColor - minTempColor) * tColor) + minTempColor;", //linear interpolation
		//"	gl_FrontColor.a = 0.8;",
		// "	if (visible < 0.5) {",
		// "		gl_FrontColor.a = 0.0;",
		// "		gl_PointSize = 0.01;",
		// "	}",
		//"   pixelNormal = gl_NormalMatrix * normal;",
		//"	pixelVisible = visible;",
		//"	vec3 toCamera = normalize(camPos - vec3(gl_ModelViewMatrix * vec4(prtlPos, 1.0)));",
		//"	float foreshoreteningRatio = dot(normal, toCamera);",
		//"	if (abs(foreshoreteningRatio) < 0.1) {pixelVisible = 0.0;}",
		"   gl_Position = ftransform();",
		"}"
	};
	
	const GLchar* _glFragmentShaderTemperatureSource[] = {
		"uniform float width;",
		"uniform float height;",
		"varying vec3 pixelNormal;",
		"varying float pixelVisible;"
		"void main()",
		"{",
			"vec3 pixelPos;",
			//"vec3 eyeNormal = gl_NormalMatrix * pixelNormal;",
			//"vec3 eyeNormal = pixelNormal;",
			"pixelPos.x = gl_TexCoord[0].s * 2.0 - 1.0;",
			"pixelPos.y = (1.0 - gl_TexCoord[0].t) * 2.0 - 1.0;",
			"pixelPos.z = -(pixelNormal.x/pixelNormal.z) * pixelPos.x - (pixelNormal.y/pixelNormal.z) * pixelPos.y;",
			"gl_FragColor = gl_Color;",
			"if (length(pixelPos) >= 1.0) {",
			"	discard;",
			"}",
			"if (pixelVisible < 0.5) {",
			"	discard;",
			"}",
		"}"
	};
	
	/*
	const GLchar* _glVertexShaderTemperatureSource[] = {
		"attribute float temp;",
		"attribute float minTemp;",
		"attribute float maxTemp;",
		"void main()",
		"{",
		"	float tColor = (temp - minTemp) / (maxTemp - minTemp);",
		"   vec4 minTempColor = vec4( 0.0, 0.0, 1.0, 1.0);",  
		"   vec4 maxTempColor = vec4( 1.0, 0.0, 0.0, 1.0);",  
		"   gl_FrontColor = ((maxTempColor - minTempColor) * tColor) + minTempColor;", //linear interpolation
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};
	*/
	const GLchar* _glVertexShaderOnSurfaceSource[] = {
		"attribute int onSurface;",
		"void main()",
		"{",
		"	vec4 surfaceColor = vec4(0.0, 1.0, 1.0, 1.0);",
		"	vec4 insideColor  = vec4(0.1, 0.1, 0.1, 1.0);",
		"	if (onSurface != 0) {",
		"		gl_FrontColor = surfaceColor;",
		"	} else {",
		"		gl_FrontColor = insideColor;",
		"	}",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};

	const GLchar* _glVertexShaderCO2Source[] = {
		"attribute float CO2;",
		"void main()",
		"{",
		"	gl_FrontColor = vec4(.5, .7, .7,  CO2/(4.0*2.0) + 0.5);",
		"	gl_PointSize = 5.0 + CO2;",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};

	const GLchar* _glVertexShaderSizeSource[] = {
			"attribute float Size;",
			"void main()",
			"{",
			"	gl_FrontColor = vec4(gl_Color.r, gl_Color.g, gl_Color.b, 1.0);",
			"	gl_PointSize = pow(1.0, (Size * 10000.0));",
			"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
			"}"
		};
	
	
	

	const GLchar* _glVertexShaderTrueSurfaceSource[] = {
		"attribute float trueSurface;",
		"void main()",
		"{",
		"	vec4 surfaceColor = vec4(1.0, 0.0, 1.0, 1.0);",
		"	vec4 insideColor  = vec4(0.1, 0.1, 0.1, 1.0);",
		"	if (trueSurface > 0.5) {",
		"		gl_FrontColor = surfaceColor;",
		"	} else {",
		"		gl_FrontColor = insideColor;",
		"	}",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};
	
	// this program will show the density values
	const GLchar* _glVertexShaderDensitySource[] = {
		"attribute float density;",
		"attribute float minDensity;",
		"attribute float maxDensity;",
		"void main()",
		"{",
		"	double nD = (density - minDensity) / (maxDensity - minDensity);",
		"	vec4 particleColor = vec4(0,0,0,1.0);",
		"	vec4 highDensityColor = vec4(0.0, 0.0, 1.0, 1.0);",
		"	vec4 lowDensityColor = vec4(0.0, 0.0, 1.0, 1.0);",
		"	particleColor = nD * highDensityColor + (1.0 - nD) * lowDensityColor;", 
		"	gl_FrontColor = particleColor;",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};

	// this program shows the changes in density

	/*
	const GLchar* _glVertexShaderDensitySource[] = {
		"attribute float density;",
		"attribute float minDensity;",
		"attribute float maxDensity;",
		"void main()",
		"{",
		"	vec4 particleColor = vec4(0.0, 0.0, 1.0, 1.0);",
		"	if (density - 1.0 > 0.0000001) {",
		"		particleColor.r = (density - 1.0) * 100.0;",
		"	} else if (density - 1.0 < 0.0000001) {",
		"		particleColor.g = (1.0 - density) * 100.0;",
		"	}",
		"	gl_FrontColor = particleColor;",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};
	*/
	const GLchar* _glVertexShaderTrueSurfaceTimeSource[] = {
		"attribute float trueSurfaceTime;",
		"void main()",
		"{",
		"	gl_FrontColor = vec4(trueSurfaceTime, trueSurfaceTime, trueSurfaceTime, 1.0);",
		"   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};
	
	
	const GLchar* _glVertexShaderCollisionSource[] = {
		"attribute float inCollision;",
		"void main()",
		"{",
		"	gl_FrontColor = vec4(inCollision, 0.0, inCollision, 1.0);",
		"	gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;",
		"}"
	};	
	

#ifdef Q_OS_WIN
	glewInit(); // should add some checking to see if it worked or not
#endif

#ifdef Q_OS_LINUX
//	glewInit(); // should add some checking to see if it worked or not
#endif

	// ugly code used to set up glsl :(

	// temperature shader
	_glVertexShaderTemperature = glCreateShader(GL_VERTEX_SHADER);
	_glFragmentShaderTemperature = glCreateShader(GL_FRAGMENT_SHADER);
	_glProgramTemperature = glCreateProgram();
	
	glShaderSource(_glVertexShaderTemperature, NumberOfLines(_glVertexShaderTemperatureSource), _glVertexShaderTemperatureSource, NULL);
	glCompileShader(_glVertexShaderTemperature);
	
	//glShaderSource(_glFragmentShaderTemperature, NumberOfLines(_glFragmentShaderTemperatureSource), _glFragmentShaderTemperatureSource, NULL);
	//glCompileShader(_glFragmentShaderTemperature);
	
	glAttachShader(_glProgramTemperature, _glVertexShaderTemperature);
	//glAttachShader(_glProgramTemperature, _glFragmentShaderTemperature);

	glLinkProgram(_glProgramTemperature);

	

	// onSurface shader
	_glVertexShaderOnSurface = glCreateShader(GL_VERTEX_SHADER);
	_glProgramOnSurface = glCreateProgram();

	glShaderSource(_glVertexShaderOnSurface, NumberOfLines(_glVertexShaderOnSurfaceSource), _glVertexShaderOnSurfaceSource, NULL);
	glCompileShader(_glVertexShaderOnSurface);
	glAttachShader(_glProgramOnSurface, _glVertexShaderOnSurface);
	glLinkProgram(_glProgramOnSurface);

	// CO2 shader

	_glVertexShaderCO2 = glCreateShader(GL_VERTEX_SHADER);
	_glProgramCO2 = glCreateProgram();

	glShaderSource(_glVertexShaderCO2, NumberOfLines(_glVertexShaderCO2Source), _glVertexShaderCO2Source, NULL);
	glCompileShader(_glVertexShaderCO2);
	glAttachShader(_glProgramCO2, _glVertexShaderCO2);
	glLinkProgram(_glProgramCO2);
	
	// Size shader

	_glVertexShaderSize = glCreateShader(GL_VERTEX_SHADER);
	_glProgramSize = glCreateProgram();

	glShaderSource(_glVertexShaderSize, NumberOfLines(_glVertexShaderSizeSource), _glVertexShaderSizeSource, NULL);
	glCompileShader(_glVertexShaderSize);
	glAttachShader(_glProgramSize, _glVertexShaderSize);
	glLinkProgram(_glProgramSize);

	// TrueSurface shader
	_glVertexShaderTrueSurface = glCreateShader(GL_VERTEX_SHADER);
	_glProgramTrueSurface = glCreateProgram();

	glShaderSource(_glVertexShaderTrueSurface, NumberOfLines(_glVertexShaderTrueSurfaceSource), _glVertexShaderTrueSurfaceSource, NULL);
	glCompileShader(_glVertexShaderTrueSurface);
	glAttachShader(_glProgramTrueSurface, _glVertexShaderTrueSurface);
	glLinkProgram(_glProgramTrueSurface);
	
	// Density shader
	_glVertexShaderDensity = glCreateShader(GL_VERTEX_SHADER);
	_glProgramDensity = glCreateProgram();
	
	glShaderSource(_glVertexShaderDensity, NumberOfLines(_glVertexShaderDensitySource), _glVertexShaderDensitySource, NULL);
	glCompileShader(_glVertexShaderDensity);
	glAttachShader(_glProgramDensity, _glVertexShaderDensity);
	glLinkProgram(_glProgramDensity);
	
	// TrueSurfaceTime shader
	_glVertexShaderTrueSurfaceTime = glCreateShader(GL_VERTEX_SHADER);
	_glProgramTrueSurfaceTime = glCreateProgram();
	
	glShaderSource(_glVertexShaderTrueSurfaceTime, NumberOfLines(_glVertexShaderTrueSurfaceTimeSource), _glVertexShaderTrueSurfaceTimeSource, NULL);
	glCompileShader(_glVertexShaderTrueSurfaceTime);
	glAttachShader(_glProgramTrueSurfaceTime, _glVertexShaderTrueSurfaceTime);
	glLinkProgram(_glProgramTrueSurfaceTime);
	
	// Collision shader
	_glVertexShaderCollision =  glCreateShader(GL_VERTEX_SHADER);
	_glProgramCollision = glCreateProgram();

	glShaderSource(_glVertexShaderCollision, NumberOfLines(_glVertexShaderCollisionSource), _glVertexShaderCollisionSource, NULL);
	glCompileShader(_glVertexShaderCollision);
	glAttachShader(_glProgramCollision, _glVertexShaderCollision);
	glLinkProgram(_glProgramCollision);
	
}


void ViewPort::setParticleRenderingType( ParticleRenderingType type ) {

	_particleRenderingType = type;

}

void ViewPort::setParticleRenderingOptions(const ParticleInstance *start)
{
	GLint tempLoc;    
	GLint visibleLoc; 
	GLint prtlPosLoc;
	GLint normalLoc;  
	GLint smoothLoc;  
	
	GLint radiusLoc;  
	GLint minTempLoc; 
	GLint maxTempLoc; 
	
	GLint nearLoc;    
	GLint widthLoc;   
	GLint heightLoc;  
	GLint topLoc;     
	GLint bottomLoc;  
	
	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
	//glTexEnvf(GL_POINT_SPRITE, GL_COORD_REPLACE, GL_TRUE);
	
	switch(_particleRenderingType) {
		case particlePosition:
			
			break;
		case particleTemperature:
			//glEnable(GL_POINT_SPRITE);	
			//glEnable( GL_TEXTURE_2D );
			//glClientActiveTexture(GL_TEXTURE0);
			
			glUseProgram(_glProgramTemperature);
			
			tempLoc    = glGetAttribLocation(_glProgramTemperature, "temp");
			visibleLoc = glGetAttribLocation(_glProgramTemperature, "visible");
			prtlPosLoc = glGetAttribLocation(_glProgramTemperature, "prtlPos");
			normalLoc  = glGetAttribLocation(_glProgramTemperature, "normal");
			smoothLoc  = glGetAttribLocation(_glProgramTemperature, "smoothingLength");
			
			radiusLoc  = glGetUniformLocation(_glProgramTemperature, "splatRadius");
			minTempLoc = glGetUniformLocation(_glProgramTemperature, "minTemp");
			maxTempLoc = glGetUniformLocation(_glProgramTemperature, "maxTemp");

			nearLoc    = glGetUniformLocation(_glProgramTemperature, "near");
			//widthLoc   = glGetUniformLocation(_glProgramTemperature, "width");
			heightLoc  = glGetUniformLocation(_glProgramTemperature, "height");
			topLoc     = glGetUniformLocation(_glProgramTemperature, "top");
			bottomLoc  = glGetUniformLocation(_glProgramTemperature, "bottom");
			
			
			glEnableVertexAttribArray(visibleLoc);
			glEnableVertexAttribArray(tempLoc);
			glEnableVertexAttribArray(prtlPosLoc);
			glEnableVertexAttribArray(smoothLoc);
			glEnableVertexAttribArray(normalLoc);
			
			glBindAttribLocation(_glProgramTemperature, visibleLoc, "visible");
			glVertexAttribPointer(visibleLoc, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].trueSurface) ); 
			
			glBindAttribLocation(_glProgramTemperature, tempLoc, "temp");
			glVertexAttribPointer(tempLoc, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].temp) ); 

			glBindAttribLocation(_glProgramTemperature, prtlPosLoc, "prtlPos");
			glVertexAttribPointer(prtlPosLoc, 3, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].pos) );

			glBindAttribLocation(_glProgramTemperature, normalLoc, "normal");
			glVertexAttribPointer(normalLoc, 3, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].normal) );

			glBindAttribLocation(_glProgramTemperature, smoothLoc, "smoothingLength");
			glVertexAttribPointer(smoothLoc, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].smoothingLength) );
				
			glUniform1f(radiusLoc, _splatRadius);
			glUniform1f(minTempLoc, PROPERTIES->getSceneProperties().getf(minTemp));
			glUniform1f(maxTempLoc, PROPERTIES->getSceneProperties().getf(maxTemp));
			glUniform1f(nearLoc, _near);
			//glUniform1f(widthLoc, _width);
			glUniform1f(heightLoc, _height);
			glUniform1f(topLoc, _top);
			glUniform1f(bottomLoc, _bottom);

			
			
			
		
			// std::cout << "_height should be " <<  _height << "\n";
			// float var;
			// glGetUniformfv(_glProgramTemperature, heightLoc, &var );
			// std::cout << "_height is " <<  var << "\n";
			// std::cout << "_height locations " <<  heightLoc << "\n";
			

			
			
			break;
		case particleCO2:
			
			
			//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramCO2, 1, "CO2");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].co2) ); 
			glUseProgram(_glProgramCO2);
			
			
			break;

		case particleOnSurface:
			
			
			
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramTrueSurfaceTime, 1, "trueSurfaceTime");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].trueSurfaceTime) ); 
			glUseProgram(_glProgramTrueSurfaceTime);
			
			
			break;

		case particleSize:
			
			//glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramSize, 1, "Size");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].size) ); 
			glUseProgram(_glProgramSize);
			
			break;

		case particleTrueSurface:
			
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramTrueSurface, 1, "trueSurface");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].trueSurface) ); 
			glUseProgram(_glProgramTrueSurface);
			
			break;

		case particleInCollision:
			
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramCollision, 1, "inCollision");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].inCollision) ); 
			glUseProgram(_glProgramCollision);

                        /*
                        // particle density
			glEnableVertexAttribArray(1);
			glBindAttribLocation(_glProgramDensity, 1, "density");
			glVertexAttribPointer(1, 1, GL_DOUBLE, GL_FALSE, sizeof(ParticleInstance), (GLvoid *)&(start[0].density) ); 

			glBindAttribLocation(_glProgramTemperature, 2, "minDensity");
			glVertexAttrib1f(2, 0);
			
			glBindAttribLocation(_glProgramTemperature, 3, "maxDensity");
			glVertexAttrib1f(3, 100);

			glUseProgram(_glProgramDensity);
			
			break;
			*/
		
	};



}

void ViewPort::unsetParticleRenderingOptions()
{
	switch(_particleRenderingType) {
		case particlePosition:
			break;
		
		case particleCO2:
		case particleSize:
			glDisableVertexAttribArray(1);
			
			break;

		case particleTemperature:
			glDisable(GL_POINT_SPRITE);	
			glDisableVertexAttribArray(4); // visible property
			//glDisable(GL_VERTEX_PROGRAM_POINT_SIZE);
		case particleOnSurface:
		case particleTrueSurface:
		case particleInCollision:
			glDisableVertexAttribArray(1);
			break;
			
		
		
	};

	glUseProgram(0);
}


void ViewPort::setSplatRadius(double newRadius)
{
	_splatRadius = newRadius;
}

void ViewPort::setCamera(Camera newCamera)
{
    for (int i=0; i<3; i++) {
        _camera.position.T[i] = newCamera.position.T[i];
        for(int j=0; j<3; j++) {
           // _camera.rotation(i, j) = newCamera.rotation(i, j);
        }
    }

}

QHash<QString, QImage> ViewPort::getImages()
{
#if 1
    QHash<QString, QImage> result;

    ParticleRenderingType origRenderingType = _particleRenderingType;
    _particleRenderingType = particleTemperature;
    _showVelocity = _showGradient = false;
    paintGL();
    result["temperature"] = grabFrameBuffer();

    _particleRenderingType = particlePosition;
    _showVelocity = _showGradient = true;
    paintGL();
    result["position"] = grabFrameBuffer();

    _particleRenderingType = origRenderingType;

    return result;
#endif
}
