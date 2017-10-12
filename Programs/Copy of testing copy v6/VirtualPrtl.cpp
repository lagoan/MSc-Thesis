// VirtualPrtl.cpp: implementation of the CVirtualPrtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "VirtualPrtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CVirtualPrtl::m_vpMaxId = 0;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVirtualPrtl::CVirtualPrtl()
{
	m_vpMaxId ++;

	m_vpPos = new CVector3D();
	m_vpVel = new CVector3D();

	SetVirtualPrtlColor(CSimuManager::COLOR_WHITE);
}

CVirtualPrtl::~CVirtualPrtl()
{
	if (m_vpPos != NULL)
	{
		delete m_vpPos;
		m_vpPos = NULL;
	}
	if (m_vpVel!= NULL)
	{
		delete m_vpVel;
		m_vpVel = NULL;
	}
}

void CVirtualPrtl::RenderGeometry()
{
	RenderPrtlByOpenGL();
}

void CVirtualPrtl::RenderPrtlByOpenGL()
{
	if (CSimuManager::SHOW_SELECTED_PRTL_ONLY 
	 && !m_geSelected && !m_geHighlit)
		return;

	if (CSimuManager::DRAW_HIGHLIT_ONLY && !m_geHighlit)
		return;

	glPushAttrib(GL_CURRENT_BIT);
	if (m_geHighlit)
		SimuColor3v(CSimuManager::COLOR_RED->v);
#ifdef	SIMU_PARA_ENABLE_PARTICLE_COLOR
	else
		SimuColor3v(m_vpColor.v);
#endif

	SimuValue radius = CSimuManager::m_sphereRadius;
	if (CSimuManager::DRAW_PARTICLES_BY_SPHERE)
	{
		GLUquadricObj* qObj = gluNewQuadric();
		glPushMatrix();
		glTranslated(m_vpPos->v[X], m_vpPos->v[Y], m_vpPos->v[Z]);
		gluSphere(qObj, radius, CSimuManager::m_sphereSlices, CSimuManager::m_sphereStacks);
		glPopMatrix();
		gluDeleteQuadric(qObj);
	}
	else
	{
		glBegin(GL_LINES);
		SimuVertex3d(m_vpPos->v[X]-radius, m_vpPos->v[Y], m_vpPos->v[Z]);
		SimuVertex3d(m_vpPos->v[X]+radius, m_vpPos->v[Y], m_vpPos->v[Z]);
		SimuVertex3d(m_vpPos->v[X], m_vpPos->v[Y]-radius, m_vpPos->v[Z]);
		SimuVertex3d(m_vpPos->v[X], m_vpPos->v[Y]+radius, m_vpPos->v[Z]);
		SimuVertex3d(m_vpPos->v[X], m_vpPos->v[Y], m_vpPos->v[Z]-radius);
		SimuVertex3d(m_vpPos->v[X], m_vpPos->v[Y], m_vpPos->v[Z]+radius);
		glEnd();
	}

	if (CSimuManager::DRAW_PARTICLE_ID)
	{
		char tmpStr[LEN_FLUID_PARTICLE_ID];

		SimuColor3v(CSimuManager::COLOR_RED->v);
		sprintf(tmpStr, " %d", GetGeometryElementId());
		glPushMatrix();
		glTranslated(m_vpPos->v[X], m_vpPos->v[Y], m_vpPos->v[Z]);
		glScaled(CSimuManager::m_particleIdSize,
				 CSimuManager::m_particleIdSize,
				 CSimuManager::m_particleIdSize);
		glCallLists(strlen(tmpStr), GL_UNSIGNED_BYTE, tmpStr);
		glPopMatrix();
	}
	glPopAttrib();
	CSimuUtility::RestoreOpenGLPreviousColor();
}

void CVirtualPrtl::SetVirtualPrtlColor(CVector3D* color)
{
#ifdef	SIMU_PARA_ENABLE_PARTICLE_COLOR
	m_vpColor.SetValue(color);
#endif
}
