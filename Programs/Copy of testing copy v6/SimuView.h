// SimuView.h : interface of the CSimuView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUVIEW_H__993A482E_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_SIMUVIEW_H__993A482E_CB30_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuManager.h"
#include "SimuModeler.h"

#define MAX_NUMBER_OF_LIGHTS	10
#define LIGHT_ATTRIBUTE_DIM		4

#include "Vector.h"

class CLightAttribute
{
public:
	CVector* position;
	CVector* specular;
	CVector* diffuse;
	CVector* ambient;
	CVector* shininess;
	GLfloat positionInGLFloat[LIGHT_ATTRIBUTE_DIM];
	GLfloat specularInGLFloat[LIGHT_ATTRIBUTE_DIM];
	GLfloat diffuseInGLFloat[LIGHT_ATTRIBUTE_DIM];
	GLfloat ambientInGLFloat[LIGHT_ATTRIBUTE_DIM];
	GLfloat shininessInGLFloat;

	CLightAttribute()
	{
		position	= new CVector(LIGHT_ATTRIBUTE_DIM);
		specular	= new CVector(LIGHT_ATTRIBUTE_DIM);
		diffuse		= new CVector(LIGHT_ATTRIBUTE_DIM);
		ambient		= new CVector(LIGHT_ATTRIBUTE_DIM);
		shininess	= new CVector(SCALAR_DIMENSION);
	}

	~CLightAttribute()
	{
		delete position;
		delete specular;
		delete diffuse;
		delete ambient;
		delete shininess;
	}

	void SetPosition(SimuValue x, SimuValue y, SimuValue z, SimuValue w)
	{
		position->SetElements(4, x, y, z, w);
		position->ToGLFloat(positionInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}

	void SetSpecular(SimuValue x, SimuValue y, SimuValue z, SimuValue w)
	{
		specular->SetElements(4, x, y, z, w);
		specular->ToGLFloat(specularInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}

	void SetDiffuse(SimuValue x, SimuValue y, SimuValue z, SimuValue w)
	{
		diffuse->SetElements(4, x, y, z, w);
		diffuse->ToGLFloat(diffuseInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}

	void SetAmbient(SimuValue x, SimuValue y, SimuValue z, SimuValue w)
	{
		ambient->SetElements(4, x, y, z, w);
		ambient->ToGLFloat(ambientInGLFloat, LIGHT_ATTRIBUTE_DIM);
	}

	void SetShininess(SimuValue s)
	{
		shininess->SetElements(1, s);
		shininessInGLFloat = (GLfloat)s;
	}
};

/////////////////////////////////////////////////////////////////////////////
// CSimuView window

class CSimuManager;

class CSimuView : public CWnd
{
// Construction
public:
	CSimuView(CSimuManager*);

// Attributes
public:

// Operations
public:
	void SetNewFocusPosition(CVector3D* newFocusPos);
	CString GetFocusPositionValueString();
	void ResetFocusPosition();
	void RefreshView();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuView)
	protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimuView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSimuView)
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	friend	CSimuManager;

	CSimuManager*		m_pSimuManager;
	HGLRC				m_hrc; 			//OpenGL Rendering Context

	UINT				m_uNumOfLights;
	CLightAttribute*	m_pLight[MAX_NUMBER_OF_LIGHTS];
	CLightAttribute*	m_pEnvironMaterial;

	enum {
		BACKGROUND_DISTANCE = 0,
		BACKGROUND_WIDTH = 1,
		BACKGROUND_HEIGHT = 2,
		BACKGROUND_ATT_NUM = 3,
		GROUND_LEVEL = 0,
		GROUND_GRID = 1,
		GROUND_RADIUS = 2,
		GROUND_ATT_NUM = 3,
		EYE_DISTANCE = 0,
		YAW = 0,
		PITCH = 1,
		ROTATE_DIM = 2,
		WINDOW_SIZE_DIM = 2,
		STEP_SIZE_MOVE = 0,
		STEP_SIZE_ROTATE = 1,
		STEP_SIZE_ITEMS = 2
	};
	CVector*	m_pBackGroundDimension;
	CVector*	m_pBackGroundColor;
	CVector3D*	m_pBackGroundTopLeft;
	CVector3D*	m_pBackGroundBottomRight;
	CVector*	m_pGroundDimension;
	CVector*	m_pGroundColor;
	CVector*	m_pEyePosition;
	CVector*	m_pEyeDistance;
	CVector*	m_pRotatePosition;
	CVector*	m_pFocusPosition;
	CVector*	m_pUpDirection;
	CVector*	m_pStepSize;

	bool	m_bZoom;
	bool	m_bKeyDownForBoundary[BOUNDARY_DIM];
	bool	m_bF1DownForLight0;
	bool	m_bF2DownForLight1;
	bool	m_bControlKeyDown;	// if true, make faster movement and rotation of eye or focus position
	bool	m_bShiftKeyDown;	// if true, move the focus. The eye moves along with the focus.
	SimuValue	m_controlAmplifier;
	int		m_view_width;
	int		m_view_height;

	void InitViewParameters();
	void UpdateEyePosition();
	void UpdateEyeDistanceAndRotatePositionWithEyeAndFocusPositions();
	void UpdateLightsInGLFloat();
	void Setup3DMapping(int view_width, int view_height);
	void DrawVirtualEnvironmentByOpenGL();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUVIEW_H__993A482E_CB30_11D5_9B85_0050BADE013B__INCLUDED_)
