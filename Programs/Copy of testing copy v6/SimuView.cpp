// SimuView.cpp : implementation of the CSimuView class
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMsg.h"
#include "SimuView.h"

//#include <gl/gl.h>
#include <gl/glu.h>
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuView

CSimuView::CSimuView(CSimuManager* pSimuManager)
{
	if (pSimuManager == NULL)
	{
		CSimuMsg::ExitWithMessage("CSimuView::CSimuView", "pSimuManager == NULL");
	}

	m_pSimuManager		= pSimuManager;
	m_hrc				= NULL;

	// initialize light
	m_uNumOfLights = 2;
	m_pLight[0] = new CLightAttribute;
	m_pLight[1] = new CLightAttribute;
	m_pLight[0]->position->SetName("light0 position");
	m_pLight[1]->position->SetName("light1 position");
	m_pLight[0]->ambient->SetName("light0 ambient");
	m_pLight[1]->ambient->SetName("light1 ambient");
	m_pLight[0]->diffuse->SetName("light0 diffuse");
	m_pLight[1]->diffuse->SetName("light1 diffuse");
	m_pLight[0]->specular->SetName("light0 specular");
	m_pLight[1]->specular->SetName("light1 specular");

	m_pEnvironMaterial = new CLightAttribute;
	m_pEnvironMaterial->SetSpecular(0.1, 0.1, 0.1, 1.0);
	m_pEnvironMaterial->SetDiffuse(0.1, 0.1, 0.1, 1.0);
	m_pEnvironMaterial->SetAmbient(0.1, 0.1, 0.1, 1.0);

	m_pBackGroundDimension		= new CVector(BACKGROUND_ATT_NUM, "background dimension");
	m_pBackGroundColor			= new CVector(VECTOR_DIMENSION, "background color");
	m_pBackGroundTopLeft		= new CVector3D();
	m_pBackGroundBottomRight	= new CVector3D();
	m_pGroundDimension			= new CVector(GROUND_ATT_NUM, "ground dimension");
	m_pGroundColor				= new CVector(VECTOR_DIMENSION, "ground color");
	m_pEyePosition				= new CVector(VECTOR_DIMENSION, "eye position");
	m_pEyeDistance				= new CVector(SCALAR_DIMENSION);
	m_pRotatePosition			= new CVector(ROTATE_DIM);
	m_pFocusPosition			= new CVector(VECTOR_DIMENSION, "focus position");
	m_pUpDirection				= new CVector(VECTOR_DIMENSION, "up direction");
	m_pStepSize					= new CVector(STEP_SIZE_ITEMS, "step size");

	m_bZoom = false;
	for(UINT i=0; i<BOUNDARY_DIM; i++)
		m_bKeyDownForBoundary[i] = false;
	m_bF1DownForLight0	= false;
	m_bF2DownForLight1	= false;
	m_bControlKeyDown	= false;
	m_bShiftKeyDown		= false;
	m_controlAmplifier	= 1;
}

CSimuView::~CSimuView()
{
	delete m_pLight[0];
	delete m_pLight[1];
	delete m_pEnvironMaterial;

	delete m_pBackGroundDimension;
	delete m_pBackGroundColor;
	delete m_pBackGroundTopLeft;
	delete m_pBackGroundBottomRight;
	delete m_pGroundDimension;
	delete m_pGroundColor;
	delete m_pEyePosition;
	delete m_pEyeDistance;
	delete m_pRotatePosition;
	delete m_pFocusPosition;
	delete m_pUpDirection;
	delete m_pStepSize;
}

/////////////////////////////////////////////////////////////////////////////
// CSimuView public methods

/*
This function sets new focus position as well as relevant positions.
*/
void CSimuView::SetNewFocusPosition(CVector3D* newFocusPos)
{
	if (newFocusPos->v[Y] - m_pFocusPosition->v[Y] > 0) return;
	SimuValue focusMovingDistance = 0;
	for (int i=0; i < VECTOR_DIMENSION; i++)
	{
		SimuValue tmpDeltaPos = newFocusPos->v[i] - m_pFocusPosition->v[i];
		if (i == Y)
		{
			m_pGroundDimension->v[GROUND_LEVEL] += tmpDeltaPos;
			focusMovingDistance = tmpDeltaPos;
		}
		m_pFocusPosition->v[i] += tmpDeltaPos;
		m_pLight[0]->position->v[i] += tmpDeltaPos;
		m_pLight[1]->position->v[i] += tmpDeltaPos;
		m_pBackGroundTopLeft->v[i] += tmpDeltaPos;
		m_pBackGroundBottomRight->v[i] += tmpDeltaPos;
	}
	UpdateLightsInGLFloat();
/*
	UpdateEyeDistanceAndRotatePositionWithEyeAndFocusPositions();

	// pitch camera up and down and keep focus on body
#define		SIMU_PARA_MAX_OPEN_ANGLE		10.0*PI/180.0
#define		SIMU_PARA_MIN_CLOSE_ANGLE		0.5*PI/180.0
	static bool bNeedMovingCamera = false;
	if (fabs(m_pRotatePosition->v[PITCH]) > SIMU_PARA_MAX_OPEN_ANGLE
		|| bNeedMovingCamera)
	{
		bNeedMovingCamera = true;
		SimuValue cameraMovingDistance = 0;
		focusMovingDistance = fabs(focusMovingDistance);
		while (cameraMovingDistance < focusMovingDistance)
			cameraMovingDistance += m_pStepSize->v[STEP_SIZE_MOVE];
		if (m_pRotatePosition->v[PITCH] > 0)
			m_pEyePosition->v[Y] -= cameraMovingDistance;
		else
			m_pEyePosition->v[Y] += cameraMovingDistance;
		UpdateEyeDistanceAndRotatePositionWithEyeAndFocusPositions();
		if (fabs(m_pRotatePosition->v[PITCH]) < SIMU_PARA_MIN_CLOSE_ANGLE)
			bNeedMovingCamera = false;
	}
*/
	UpdateEyePosition();
}

CString CSimuView::GetFocusPositionValueString()
{
	char	tmpBuf[LEN_MESSAGE];
	sprintf(tmpBuf, "focus=(%.4f, %.4f, %.4f)",
							m_pFocusPosition->v[X],
							m_pFocusPosition->v[Y],
							m_pFocusPosition->v[Z]);
	CString strFocusPos = tmpBuf;
	return strFocusPos;
}

void CSimuView::ResetFocusPosition()
{
	InitViewParameters();
}

void CSimuView::RefreshView()
{
	Setup3DMapping(m_view_width, m_view_height);
	Invalidate(false);
}

/////////////////////////////////////////////////////////////////////////////
// CSimuView private methods

void CSimuView::InitViewParameters()
{
	m_pSimuManager->InitNamedVector(m_pLight[0]->position);
	m_pSimuManager->InitNamedVector(m_pLight[1]->position);
	m_pSimuManager->InitNamedVector(m_pLight[0]->ambient);
	m_pSimuManager->InitNamedVector(m_pLight[1]->ambient);
	m_pSimuManager->InitNamedVector(m_pLight[0]->diffuse);
	m_pSimuManager->InitNamedVector(m_pLight[1]->diffuse);
	m_pSimuManager->InitNamedVector(m_pLight[0]->specular);
	m_pSimuManager->InitNamedVector(m_pLight[1]->specular);

	m_pSimuManager->InitNamedVector(m_pEyePosition);
	m_pSimuManager->InitNamedVector(m_pFocusPosition);
	m_pSimuManager->InitNamedVector(m_pUpDirection);
	m_pSimuManager->InitNamedVector(m_pBackGroundDimension);
	m_pSimuManager->InitNamedVector(m_pBackGroundColor);
	m_pSimuManager->InitNamedVector(m_pGroundDimension);
	m_pSimuManager->InitNamedVector(m_pGroundColor);
	m_pSimuManager->InitNamedVector(CSimuManager::m_viewBoundary);
	m_pSimuManager->InitNamedVector(m_pStepSize);

	m_pLight[0]->position->ToGLFloat(m_pLight[0]->positionInGLFloat, m_pLight[0]->position->dim);
	m_pLight[0]->specular->ToGLFloat(m_pLight[0]->specularInGLFloat, m_pLight[0]->specular->dim);
	m_pLight[0]->diffuse->ToGLFloat(m_pLight[0]->diffuseInGLFloat, m_pLight[0]->diffuse->dim);
	m_pLight[0]->ambient->ToGLFloat(m_pLight[0]->ambientInGLFloat, m_pLight[0]->ambient->dim);
	m_pLight[1]->position->ToGLFloat(m_pLight[1]->positionInGLFloat, m_pLight[1]->position->dim);
	m_pLight[1]->specular->ToGLFloat(m_pLight[1]->specularInGLFloat, m_pLight[1]->specular->dim);
	m_pLight[1]->diffuse->ToGLFloat(m_pLight[1]->diffuseInGLFloat, m_pLight[1]->diffuse->dim);
	m_pLight[1]->ambient->ToGLFloat(m_pLight[1]->ambientInGLFloat, m_pLight[1]->ambient->dim);

	UpdateEyeDistanceAndRotatePositionWithEyeAndFocusPositions();

	m_pBackGroundTopLeft->v[X] = -m_pBackGroundDimension->v[BACKGROUND_WIDTH]/2;
	m_pBackGroundTopLeft->v[Y] = m_pBackGroundDimension->v[BACKGROUND_HEIGHT];
	m_pBackGroundTopLeft->v[Z] = m_pFocusPosition->v[Z]-m_pBackGroundDimension->v[BACKGROUND_DISTANCE];
	m_pBackGroundBottomRight->v[X] = m_pBackGroundDimension->v[BACKGROUND_WIDTH]/2;
	m_pBackGroundBottomRight->v[Y] = m_pGroundDimension->v[GROUND_LEVEL];
	m_pBackGroundBottomRight->v[Z] = m_pBackGroundTopLeft->v[Z];
}

void CSimuView::UpdateEyePosition()
{
	if (m_pRotatePosition->v[YAW] >= PI*2)
		m_pRotatePosition->v[YAW] -= PI*2;
	else if (m_pRotatePosition->v[YAW] <= -PI*2)
		m_pRotatePosition->v[YAW] += PI*2;
	if (m_pRotatePosition->v[PITCH] >= PI*2)
		m_pRotatePosition->v[PITCH] -= PI*2;
	else if (m_pRotatePosition->v[PITCH] <= -PI/2)
		m_pRotatePosition->v[PITCH] += PI*2;

	// the eye moves along with the focus, keeping certain distance and angle
	m_pEyePosition->SetValue(m_pFocusPosition);
	m_pEyePosition->v[Y] += m_pEyeDistance->v[EYE_DISTANCE]*sin(m_pRotatePosition->v[PITCH]);
	m_pEyePosition->v[X] += m_pEyeDistance->v[EYE_DISTANCE]*cos(m_pRotatePosition->v[PITCH])*sin(m_pRotatePosition->v[YAW]);
	m_pEyePosition->v[Z] += m_pEyeDistance->v[EYE_DISTANCE]*cos(m_pRotatePosition->v[PITCH])*cos(m_pRotatePosition->v[YAW]);
}

void CSimuView::UpdateEyeDistanceAndRotatePositionWithEyeAndFocusPositions()
{
	m_pEyeDistance->v[EYE_DISTANCE] = m_pEyePosition->GetDistanceToVector(m_pFocusPosition);

	SimuValue D_in_X, D_in_Z, D_in_X_Z;
	D_in_X = m_pEyePosition->v[X]-m_pFocusPosition->v[X];
	D_in_Z = m_pEyePosition->v[Z]-m_pFocusPosition->v[Z];
	D_in_X_Z = sqrt(pow(D_in_X, 2.0)+pow(D_in_Z, 2.0));

	m_pRotatePosition->v[YAW] = acos(D_in_Z/D_in_X_Z);
	if (D_in_X < 0)	// sin < 0
		m_pRotatePosition->v[YAW] += PI;

	m_pRotatePosition->v[PITCH] = acos(D_in_X_Z/m_pEyeDistance->v[EYE_DISTANCE]);
	if (m_pFocusPosition->v[Y] > m_pEyePosition->v[Y])
		m_pRotatePosition->v[PITCH] *= -1;
}

void CSimuView::UpdateLightsInGLFloat()
{
	if (m_uNumOfLights >= 1)
	{
		m_pLight[0]->position->ToGLFloat(m_pLight[0]->positionInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[0]->specular->ToGLFloat(m_pLight[0]->specularInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[0]->diffuse->ToGLFloat(m_pLight[0]->diffuseInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[0]->ambient->ToGLFloat(m_pLight[0]->ambientInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}
	if (m_uNumOfLights >= 2)
	{
		m_pLight[1]->position->ToGLFloat(m_pLight[1]->positionInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[1]->specular->ToGLFloat(m_pLight[1]->specularInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[1]->diffuse->ToGLFloat(m_pLight[1]->diffuseInGLFloat, LIGHT_ATTRIBUTE_DIM);
		m_pLight[1]->ambient->ToGLFloat(m_pLight[1]->ambientInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}
}

void CSimuView::Setup3DMapping(int view_width, int view_height)
{
	// Get hDC
	CDC* pDC;
	HDC hDC;

	int retryTimes = 18;
	BOOL bResult=false;
	while ((retryTimes > 0)&&(!bResult))
	{
		// Get hDC
		pDC = this->GetDC();
		hDC = pDC->GetSafeHdc();

		//
		// Make the rendering context m_hrc current
		//
		bResult = wglMakeCurrent(hDC, m_hrc);
		retryTimes--;
	}
	if (!bResult)
	{
		int err_code = GetLastError();
		char str[100];
		_itoa(err_code, str, 10);
		TRACE("wglMakeCurrent Failed %x in OnSize.\r\n", GetLastError() ) ;
		CSimuMsg::ExitWithMessage("CSimuView::OnSize", "Error code is %s.", str);
	}

#ifndef RENDERING_CONTEXT_TEST

    //
    // Set up the mapping of 3-space to screen space
    //
    SimuValue gldAspect = (SimuValue) view_width/ (SimuValue) view_height;
    glMatrixMode(GL_PROJECTION);
   	glLoadIdentity();
	glFrustum(CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT],
			  CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT],
			  CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM],
			  CSimuManager::m_viewBoundary->v[BOUNDARY_TOP],
			  CSimuManager::m_viewBoundary->v[BOUNDARY_NEAR],
			  CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]);

	glViewport(0, 0, view_width, view_height);

#endif

	// No rendering context will be current.
    wglMakeCurrent(NULL, NULL);

	// Release DC to save memory
	this->ReleaseDC(pDC);
}

void CSimuView::DrawVirtualEnvironmentByOpenGL()
{
//	TRACE("Draw Virtual Environment\r\n");
	
	// Clear the color and depth buffers
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f) ;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set the material color to follow the current color
	glEnable(GL_COLOR_MATERIAL) ;
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE) ;
	glMaterialfv(GL_FRONT, GL_SPECULAR, m_pEnvironMaterial->specularInGLFloat);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, m_pEnvironMaterial->diffuseInGLFloat);
	glMaterialfv(GL_FRONT, GL_AMBIENT, m_pEnvironMaterial->ambientInGLFloat);

	// Enable lighting calculations
	#ifdef	SIMU_PARA_SHADING_SMOOTH
	glShadeModel(GL_SMOOTH);
	#else
	glShadeModel(GL_FLAT);
	#endif
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(m_pEyePosition->v[X], m_pEyePosition->v[Y], m_pEyePosition->v[Z],
				m_pFocusPosition->v[X], m_pFocusPosition->v[Y], m_pFocusPosition->v[Z],
				m_pUpDirection->v[X], m_pUpDirection->v[Y], m_pUpDirection->v[Z]);

	m_pSimuManager->DrawSimulationBodies();

	// Draw x and z coordinates
	if (CSimuManager::DRAW_X_Z_COORDINATES)
	{
		SimuColor3v(CSimuManager::COLOR_WHITE->v);
		glBegin(GL_LINES);
		SimuVertex3d(0,
					m_pGroundDimension->v[GROUND_LEVEL],
					m_pGroundDimension->v[GROUND_RADIUS]);
		SimuVertex3d(0,
					m_pGroundDimension->v[GROUND_LEVEL],
					-m_pGroundDimension->v[GROUND_RADIUS]);
		SimuVertex3d(m_pGroundDimension->v[GROUND_RADIUS],
					m_pGroundDimension->v[GROUND_LEVEL],
					0);
		SimuVertex3d(-m_pGroundDimension->v[GROUND_RADIUS],
					m_pGroundDimension->v[GROUND_LEVEL],
					0);
		glEnd();
	}

	glPushMatrix();
	// With this transfermation, the lights won't rotate around y axis, ie yaw, with eye position
	glRotated(m_pRotatePosition->v[YAW]*180/PI, 0.0, 1.0, 0.0);
	#define		LIGHT_SIZE	0.05
	if (CSimuManager::ENABLE_LIGHT0)
	{
		// Set light0 attributes
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_POSITION, m_pLight[0]->positionInGLFloat);
		glLightfv(GL_LIGHT0, GL_SPECULAR, m_pLight[0]->specularInGLFloat);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, m_pLight[0]->diffuseInGLFloat);
		glLightfv(GL_LIGHT0, GL_AMBIENT, m_pLight[0]->ambientInGLFloat);
		if (CSimuManager::DRAW_LIGHT0)
		{
			// Draw light0 position
			if (CSimuManager::DRAW_PARTICLES_BY_SPHERE)
			{
				GLUquadricObj* qObj = gluNewQuadric();
				glPushMatrix();
				glTranslated(m_pLight[0]->position->v[X], m_pLight[0]->position->v[Y], m_pLight[0]->position->v[Z]);
				gluSphere(qObj, CSimuManager::m_sphereRadius, CSimuManager::m_sphereSlices, CSimuManager::m_sphereStacks);
				glPopMatrix();
				gluDeleteQuadric(qObj);
			}
			else
			{
				glBegin(GL_LINES);
				SimuColor3d(1.0, 1.0, 1.0);
				SimuVertex3d(m_pLight[0]->position->v[X]-LIGHT_SIZE, m_pLight[0]->position->v[Y], m_pLight[0]->position->v[Z]);
				SimuVertex3d(m_pLight[0]->position->v[X]+LIGHT_SIZE, m_pLight[0]->position->v[Y], m_pLight[0]->position->v[Z]);
				SimuVertex3d(m_pLight[0]->position->v[X], m_pLight[0]->position->v[Y]-LIGHT_SIZE, m_pLight[0]->position->v[Z]);
				SimuVertex3d(m_pLight[0]->position->v[X], m_pLight[0]->position->v[Y]+LIGHT_SIZE, m_pLight[0]->position->v[Z]);
				SimuVertex3d(m_pLight[0]->position->v[X], m_pLight[0]->position->v[Y], m_pLight[0]->position->v[Z]-LIGHT_SIZE);
				SimuVertex3d(m_pLight[0]->position->v[X], m_pLight[0]->position->v[Y], m_pLight[0]->position->v[Z]+LIGHT_SIZE);
				glEnd();
			}
		}
	}
	else
		glDisable(GL_LIGHT0);

	if (CSimuManager::ENABLE_LIGHT1)
	{
		// Set light1 attributes
		glEnable(GL_LIGHT1);
		glLightfv(GL_LIGHT1, GL_POSITION, m_pLight[1]->positionInGLFloat);
		glLightfv(GL_LIGHT1, GL_SPECULAR, m_pLight[1]->specularInGLFloat);
		glLightfv(GL_LIGHT1, GL_DIFFUSE, m_pLight[1]->diffuseInGLFloat);
		glLightfv(GL_LIGHT1, GL_AMBIENT, m_pLight[1]->ambientInGLFloat);
		if (CSimuManager::DRAW_LIGHT1)
		{
			// Draw light1 position
			if (CSimuManager::DRAW_PARTICLES_BY_SPHERE)
			{
				GLUquadricObj* qObj = gluNewQuadric();
				glPushMatrix();
				glTranslated(m_pLight[1]->position->v[X], m_pLight[1]->position->v[Y], m_pLight[1]->position->v[Z]);
				gluSphere(qObj, CSimuManager::m_sphereRadius, CSimuManager::m_sphereSlices, CSimuManager::m_sphereStacks);
				glPopMatrix();
				gluDeleteQuadric(qObj);
			}
			else
			{
				glBegin(GL_LINES);
				SimuColor3d(1.0, 1.0, 1.0);
				SimuVertex3d(m_pLight[1]->position->v[X]-LIGHT_SIZE, m_pLight[1]->position->v[Y], m_pLight[1]->position->v[Z]);
				SimuVertex3d(m_pLight[1]->position->v[X]+LIGHT_SIZE, m_pLight[1]->position->v[Y], m_pLight[1]->position->v[Z]);
				SimuVertex3d(m_pLight[1]->position->v[X], m_pLight[1]->position->v[Y]-LIGHT_SIZE, m_pLight[1]->position->v[Z]);
				SimuVertex3d(m_pLight[1]->position->v[X], m_pLight[1]->position->v[Y]+LIGHT_SIZE, m_pLight[1]->position->v[Z]);
				SimuVertex3d(m_pLight[1]->position->v[X], m_pLight[1]->position->v[Y], m_pLight[1]->position->v[Z]-LIGHT_SIZE);
				SimuVertex3d(m_pLight[1]->position->v[X], m_pLight[1]->position->v[Y], m_pLight[1]->position->v[Z]+LIGHT_SIZE);
				glEnd();
			}
		}
	}
	else
		glDisable(GL_LIGHT1);

	// Draw the background
/*	SimuColor3v(m_pBackGroundColor->v);
	glBegin(GL_POLYGON);
	glNormal3d(0.0, 0.0, 1.0);
	SimuVertex3d(m_pBackGroundTopLeft->v[X], m_pBackGroundTopLeft->v[Y], m_pBackGroundTopLeft->v[Z]);
	SimuVertex3d(m_pBackGroundBottomRight->v[X], m_pBackGroundTopLeft->v[Y], m_pBackGroundBottomRight->v[Z]);
	SimuVertex3d(m_pBackGroundBottomRight->v[X], m_pBackGroundBottomRight->v[Y], m_pBackGroundBottomRight->v[Z]);
	SimuVertex3d(m_pBackGroundTopLeft->v[X], m_pBackGroundBottomRight->v[Y], m_pBackGroundTopLeft->v[Z]);
	glEnd();
*/
	glPopMatrix();
}

/////////////////////////////////////////////////////////////////////////////
// CSimuView message map

BEGIN_MESSAGE_MAP(CSimuView,CWnd )
	//{{AFX_MSG_MAP(CSimuView)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_KEYUP()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CSimuView message handlers

BOOL CSimuView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

int CSimuView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	char location[] = "CSimuView::OnCreate(lpCreateStruct) ";

	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	InitViewParameters();
	
	// Get hDC
	CDC* pDC = this->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	//
	// Fill in the Pixel Format Descriptor
	//
    PIXELFORMATDESCRIPTOR pfd ;
	memset(&pfd,0, sizeof(PIXELFORMATDESCRIPTOR)) ;

    pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);   
    pfd.nVersion = 1 ;                           // Version number
	pfd.dwFlags =  PFD_DOUBLEBUFFER |            // Use double buffer
	               PFD_SUPPORT_OPENGL |          // Use OpenGL
	               PFD_DRAW_TO_WINDOW ;          // Pixel format is for a window.
	pfd.iPixelType = PFD_TYPE_RGBA ;
    pfd.cColorBits = 24;                         // 24-bit color
	pfd.cDepthBits = 32 ;					   	 // 32-bit depth buffer
    pfd.iLayerType = PFD_MAIN_PLANE ;            // Layer type

    int nPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if (nPixelFormat == 0)
	{
		TRACE("ChoosePixelFormat Failed %d\r\n",GetLastError()) ;
		return -1 ;
	}
	TRACE("Pixel Format %d\r\n", nPixelFormat) ;

    BOOL bResult = SetPixelFormat(hDC, nPixelFormat, &pfd);
	if (!bResult)
	{
		TRACE("SetPixelFormat Failed %d\r\n",GetLastError()) ;
		return -1 ;
	}
	
	//
    // Create a rendering context.
    //
    m_hrc = wglCreateContext(hDC);
	if (!m_hrc)
	{
		TRACE("wglCreateContext Failed %x in OnCreate.\r\n", GetLastError()) ;
		return -1;
	}
    wglMakeCurrent( hDC, m_hrc);

    LOGFONT     lf;
    HFONT       hFont, hOldFont;
	GLYPHMETRICSFLOAT agmf[256];
	int errorCode;

    // Let's create a default TrueType font to display.
    memset(&lf,0,sizeof(LOGFONT));
    lf.lfHeight               =   -28 ;
    lf.lfWeight               =   FW_NORMAL ;
    lf.lfCharSet              =   ANSI_CHARSET ;
    lf.lfOutPrecision         =   OUT_DEFAULT_PRECIS ;
    lf.lfClipPrecision        =   CLIP_DEFAULT_PRECIS ;
    lf.lfQuality              =   DEFAULT_QUALITY ;
    lf.lfPitchAndFamily       =   FF_DONTCARE|DEFAULT_PITCH;
    lstrcpy (lf.lfFaceName, "Arial") ;

    hFont = CreateFontIndirect(&lf);
    hOldFont = (HFONT)SelectObject(hDC, hFont);	

    if (!(wglUseFontOutlines(hDC, 0, 255, 1000, 0.0f, 0.15f, 
       WGL_FONT_POLYGONS, agmf)))
	{
		errorCode = GetLastError();
	}
	glListBase(1000); // indicate the start of display lists for the glyphs.

	DeleteObject(SelectObject(hDC,hOldFont));	

	// Release DC to save memory
	ReleaseDC(pDC);

	return 0;
}

void CSimuView::OnDestroy() 
{
	CWnd ::OnDestroy();
	
	// TODO: Add your message handler code here

	wglDeleteContext( m_hrc );
}

//#define RENDERING_CONTEXT_TEST

void CSimuView::OnPaint() 
{
	// Don't use this to get hDC.
	// Even release with *dc, the memory is sucked out
	// But, don't take this line out even looking useless
	// Otherwise, there will be problem with modal dialog box, like MessageBox(...)
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
//	TRACE("OnPaint\r\n");

	// Get hDC
	CDC* pDC = this->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	//
	// Make the HGLRC current
	//
	BOOL bResult = wglMakeCurrent(hDC, m_hrc);
	if (!bResult)
	{
		int err_code = GetLastError();
		char str[100];
		_itoa(err_code, str, 10);
		TRACE("wglMakeCurrent Failed %x in OnPaint.\r\n", GetLastError() ) ;
		CSimuMsg::ExitWithMessage("CSimuView::OnPaint", "Error code is %s.", str);
	}

#ifdef RENDERING_CONTEXT_TEST

			// OpenGL animation code goes here
			static float theta = 0.0f;
			
			glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
			glClear( GL_COLOR_BUFFER_BIT );
			
			glPushMatrix();
			glRotatef( theta, 0.0f, 0.0f, 1.0f );
			glBegin( GL_TRIANGLES );
			glColor3f( 1.0f, 0.0f, 0.0f ); glVertex2f( 0.0f, 1.0f );
			glColor3f( 0.0f, 1.0f, 0.0f ); glVertex2f( 0.87f, -0.5f );
			glColor3f( 0.0f, 0.0f, 1.0f ); glVertex2f( -0.87f, -0.5f );
			glEnd();
			glPopMatrix();
			
			theta += 0.1f;
//			theta += 1.0f;

#else
	DrawVirtualEnvironmentByOpenGL();
#endif

    // Flush the drawing pipeline.
    glFlush();

	SwapBuffers(hDC);

	wglMakeCurrent(NULL, NULL) ;

	// Release DC to save memory
	this->ReleaseDC(pDC);

#ifdef RENDERING_CONTEXT_TEST
	Invalidate(false);
#endif

	// Do not call CWnd::OnPaint() for painting messages
}

void CSimuView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	TRACE("OnSize\r\n");
	
	if ( (cx <= 0) || (cy <= 0) ) return ;

	m_view_width = cx;
	m_view_height = cy;
	
/*
	// Get hDC
	CDC* pDC = this->GetDC();
	HDC hDC = pDC->GetSafeHdc();

	//
	// Make the rendering context m_hrc current
	//
    BOOL bResult = wglMakeCurrent(hDC, m_hrc);
*/
	Setup3DMapping(m_view_width, m_view_height);
}

void CSimuView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch (nChar)
	{
		case VK_CONTROL:
			m_bControlKeyDown = false;
			break;
		case VK_SHIFT:
			m_bShiftKeyDown = false;
			break;
		case VK_F1:
			m_bF1DownForLight0 = false;
			break;
		case VK_F2:
			m_bF2DownForLight1 = false;
			break;
		case 'C':
			CSimuManager::m_enclosingBall->m_ebMovable = false;
			break;
		case 'L':
			m_bKeyDownForBoundary[BOUNDARY_LEFT] = false;
			break;
		case 'R':
			m_bKeyDownForBoundary[BOUNDARY_RIGHT] = false;
			break;
		case 'B':
			m_bKeyDownForBoundary[BOUNDARY_BOTTOM] = false;
			break;
		case 'T':
			m_bKeyDownForBoundary[BOUNDARY_TOP] = false;
			break;
		case 'N':
			m_bKeyDownForBoundary[BOUNDARY_NEAR] = false;
			break;
		case 'F':
			m_bKeyDownForBoundary[BOUNDARY_FAR] = false;
			break;
		case 'Z':
			m_bZoom = false;
			break;
		default:
			break;
	}

	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CSimuView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch (nChar)
	{
		case VK_ESCAPE:
			PostQuitMessage(0);
		case VK_CONTROL:
			m_bControlKeyDown = true;
			break;
		case VK_SHIFT:
			m_bShiftKeyDown = true;
			break;
		case VK_F1:
			m_bF1DownForLight0 = true;
			break;
		case VK_F2:
			m_bF2DownForLight1 = true;
			break;
		case 'C':
			CSimuManager::m_enclosingBall->m_ebMovable = true;
			break;
		case 'L':
			m_bKeyDownForBoundary[BOUNDARY_LEFT] = true;
			break;
		case 'R':
			m_bKeyDownForBoundary[BOUNDARY_RIGHT] = true;
			break;
		case 'B':
			m_bKeyDownForBoundary[BOUNDARY_BOTTOM] = true;
			break;
		case 'T':
			m_bKeyDownForBoundary[BOUNDARY_TOP] = true;
			break;
		case 'N':
			m_bKeyDownForBoundary[BOUNDARY_NEAR] = true;
			break;
		case 'F':
			m_bKeyDownForBoundary[BOUNDARY_FAR] = true;
			break;
		case 'Z':
			m_bZoom = true;
			break;
		case VK_LEFT:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_LEFT])
				CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_RIGHT])
				CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[0]->position->v[X] += m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[1]->position->v[X] += m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pFocusPosition->v[X] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[0]->position->v[X] += m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[1]->position->v[X] += m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_LEFT]
			  &&!m_bKeyDownForBoundary[BOUNDARY_RIGHT]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pRotatePosition->v[YAW] -= m_pStepSize->v[STEP_SIZE_ROTATE];
				if (m_bControlKeyDown)
					m_pRotatePosition->v[YAW] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_ROTATE];
			}
			break; 
		case VK_RIGHT:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[X] += m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_LEFT])
				CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_RIGHT])
				CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[X] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[X] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[X] += m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[0]->position->v[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[1]->position->v[X] -= m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pEyePosition->v[X] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[0]->position->v[X] -= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[1]->position->v[X] -= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_LEFT]
			  &&!m_bKeyDownForBoundary[BOUNDARY_RIGHT]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pRotatePosition->v[YAW] += m_pStepSize->v[STEP_SIZE_ROTATE];
				if (m_bControlKeyDown)
					m_pRotatePosition->v[X] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_ROTATE];
			}
			break; 
		case VK_UP:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_BOTTOM])
				CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_TOP])
				CSimuManager::m_viewBoundary->v[BOUNDARY_TOP]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				m_pLight[0]->position->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				m_pLight[1]->position->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				m_pGroundDimension->v[GROUND_LEVEL] += m_pStepSize->v[STEP_SIZE_MOVE];
				m_pBackGroundTopLeft->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				m_pBackGroundBottomRight->v[Y] += m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pFocusPosition->v[Y] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pLight[0]->position->v[Y] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pLight[1]->position->v[Y] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pGroundDimension->v[GROUND_LEVEL] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pBackGroundTopLeft->v[Y] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pBackGroundBottomRight->v[Y] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_BOTTOM]
			  &&!m_bKeyDownForBoundary[BOUNDARY_TOP]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pRotatePosition->v[PITCH] += m_pStepSize->v[STEP_SIZE_ROTATE];
				if (m_bControlKeyDown)
					m_pRotatePosition->v[PITCH] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_ROTATE];
			}
			break; 
		case VK_DOWN:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_BOTTOM])
				CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_TOP])
				CSimuManager::m_viewBoundary->v[BOUNDARY_TOP]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				m_pLight[0]->position->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				m_pLight[1]->position->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				m_pGroundDimension->v[GROUND_LEVEL] -= m_pStepSize->v[STEP_SIZE_MOVE];
				m_pBackGroundTopLeft->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				m_pBackGroundBottomRight->v[Y] -= m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pFocusPosition->v[Y] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pLight[0]->position->v[Y] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pLight[1]->position->v[Y] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pGroundDimension->v[GROUND_LEVEL] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pBackGroundTopLeft->v[Y] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
					m_pBackGroundBottomRight->v[Y] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_BOTTOM]
			  &&!m_bKeyDownForBoundary[BOUNDARY_TOP]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pRotatePosition->v[PITCH] -= m_pStepSize->v[STEP_SIZE_ROTATE];
				if (m_bControlKeyDown)
					m_pRotatePosition->v[PITCH] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_ROTATE];
			}
			break;
		case VK_PRIOR:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bZoom) // zoom in
			{
				CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_TOP]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_NEAR])
				CSimuManager::m_viewBoundary->v[BOUNDARY_NEAR]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_FAR])
				CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[0]->position->v[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[1]->position->v[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pFocusPosition->v[Z] -= m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[0]->position->v[Z] -= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[1]->position->v[Z] -= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_NEAR]
			  &&!m_bKeyDownForBoundary[BOUNDARY_FAR]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pEyeDistance->v[EYE_DISTANCE] -= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
												-= m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pEyeDistance->v[EYE_DISTANCE]
								-= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
					CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
								-= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			break;
		case VK_NEXT:
			if (CSimuManager::m_enclosingBall->m_ebMovable)
			{
				CSimuManager::m_enclosingBall->m_ebPos[Z] -= m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bZoom) // zoom out
			{
				CSimuManager::m_viewBoundary->v[BOUNDARY_LEFT]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_RIGHT]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_TOP]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_BOTTOM]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
				break;
			}
			if (m_bKeyDownForBoundary[BOUNDARY_NEAR])
				CSimuManager::m_viewBoundary->v[BOUNDARY_NEAR]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bKeyDownForBoundary[BOUNDARY_FAR])
				CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
											-= m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF1DownForLight0)
				m_pLight[0]->position->v[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bF2DownForLight1)
				m_pLight[1]->position->v[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
			if (m_bShiftKeyDown)
			{
				m_pFocusPosition->v[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[0]->position->v[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
//				m_pLight[1]->position->v[Z] += m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pFocusPosition->v[Z] += m_controlAmplifier
													*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[0]->position->v[Z] += m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
//					m_pLight[1]->position->v[Z] += m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			if (!m_bKeyDownForBoundary[BOUNDARY_NEAR]
			  &&!m_bKeyDownForBoundary[BOUNDARY_FAR]
			  &&!m_bF1DownForLight0 && !m_bF2DownForLight1
			  &&!m_bShiftKeyDown
			  )
			{
				m_pEyeDistance->v[EYE_DISTANCE] += m_pStepSize->v[STEP_SIZE_MOVE];
				CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
											+= m_pStepSize->v[STEP_SIZE_MOVE];
				if (m_bControlKeyDown)
				{
					m_pEyeDistance->v[EYE_DISTANCE]
								+= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
					CSimuManager::m_viewBoundary->v[BOUNDARY_FAR]
								+= m_controlAmplifier*m_pStepSize->v[STEP_SIZE_MOVE];
				}
			}
			break;
		default:
			break;
	}

	switch (nChar)
	{
		case VK_LEFT:
		case VK_RIGHT:
		case VK_UP:
		case VK_DOWN:
		case VK_PRIOR:
		case VK_NEXT:
//			if (m_bF1DownForLight0 || m_bF2DownForLight1)
				UpdateLightsInGLFloat();
//			else
				UpdateEyePosition();
			Setup3DMapping(m_view_width, m_view_height);
			m_pSimuManager->InitializeOptions();
			Invalidate(false);
			break;
		default:
			break;
	}

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
