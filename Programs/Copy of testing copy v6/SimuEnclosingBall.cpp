// SimuEnclosingBall.cpp: implementation of the CSimuEnclosingBall class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuEnclosingBall.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuEnclosingBall::CSimuEnclosingBall()
{
	m_ebShow = false;
	m_ebMovable = false;
	m_ebRadius = 0.5*CSimuManager::m_prtlDistance;
	if (m_ebRadius < 0.1)
		m_ebRadius = 0.1;

	m_ebColor.SetValue((SimuValue)1);
}

CSimuEnclosingBall::~CSimuEnclosingBall()
{
}

void CSimuEnclosingBall::RenderEnclosingBall()
{
	GLUquadricObj* qObj = gluNewQuadric();
	glPushAttrib(GL_CURRENT_BIT);
	glPushMatrix();
	SimuTranslate(m_ebPos[X], m_ebPos[Y], m_ebPos[Z]);
	gluSphere(qObj, m_ebRadius, 20, 20);
	glPopMatrix();
	glPopAttrib();
	gluDeleteQuadric(qObj);
}
