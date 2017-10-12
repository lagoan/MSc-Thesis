// Point3D.cpp: implementation of the CPoint3D class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Point3D.h"


int CPoint3D::m_p3dMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPoint3D::CPoint3D()
{
	m_p3dMaxId ++;

	SetPoint3DColor(CSimuManager::COLOR_WHITE);
}

CPoint3D::~CPoint3D()
{

}

void CPoint3D::RenderGeometry()
{
	glPushAttrib(GL_CURRENT_BIT);
#ifdef	SIMU_PARA_ENABLE_POINT_3D_COLOR
	SimuColor3v(m_p3dColor.v);
#endif

	GLUquadricObj* qObj = gluNewQuadric();
	glPushMatrix();
	glTranslated(m_p3dPos[X], m_p3dPos[Y], m_p3dPos[Z]);
	gluSphere(qObj, CSimuManager::m_sphereRadius,
					CSimuManager::m_sphereSlices,
					CSimuManager::m_sphereStacks);
	glPopMatrix();
	gluDeleteQuadric(qObj);

	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CPoint3D::SetPoint3DColor(CVector3D* color)
{
#ifdef	SIMU_PARA_ENABLE_POINT_3D_COLOR
	m_p3dColor.SetValue(color);
#endif
}
