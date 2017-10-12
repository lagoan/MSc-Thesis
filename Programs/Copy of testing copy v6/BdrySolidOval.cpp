// BdrySolidOval.cpp: implementation of the CBdrySolidOval class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BdrySolidOval.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CBdrySolidOval::m_bsoMaxId = 0;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBdrySolidOval::CBdrySolidOval()
{
	InitializeSolidOval();
}

CBdrySolidOval::CBdrySolidOval(SimuValue radiusX, SimuValue radiusY, SimuValue radiusZ,
							   SimuValue angleAboutZ, CVector3D* ctrPos, CVector3D* direction)
: COvalAlong3DAxes (radiusX, radiusY, radiusZ, angleAboutZ, ctrPos, direction)
{
	InitializeSolidOval();
}

CBdrySolidOval::~CBdrySolidOval()
{

}

void CBdrySolidOval::InitializeSolidOval()
{
	m_bdryType = SOLID_OVAL;

	m_bsoMaxId++;
	char tmpName[LEN_BOUNDARY_NAME];
	sprintf(tmpName, "solid_oval_%d", m_bsoMaxId);
	m_deiName = tmpName;
}
