// PrtlInBox.cpp: implementation of the CPrtlInBox class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlInBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlInBox::CPrtlInBox(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
					   SimuValue angleAboutZ, CVector3D* topCtrPos, CVector3D* direction)
: CBoxOnXYPlane(heightOnZ, widthOnX, widthOnY, angleAboutZ, topCtrPos, direction)
{

}

CPrtlInBox::~CPrtlInBox()
{

}

void CPrtlInBox::CreatePrtlsInBox(CPrtlFluid* pPrtlFluid)
{
	SimuValue prtlDistance = pPrtlFluid->m_pfPrtlDist;
	int prtlLayerX = 1+(int)ceil(m_boxWidthOnX/prtlDistance);
	int prtlLayerY = 1+(int)ceil(m_boxWidthOnY/prtlDistance);
	int prtlLayerZ = 1+(int)ceil(m_boxHeightOnZ/prtlDistance);
	SimuValue minX = -prtlDistance*(prtlLayerX-1)/2;
	SimuValue minY = -prtlDistance*(prtlLayerY-1)/2;
	SimuValue minZ = -prtlDistance*(prtlLayerZ-1)/2;
	CVector3D tmpPos, ctrPos;
	ctrPos.SetValueAsAddition(&m_boxTopCtrPos, &m_boxBtmCtrPos);
	ctrPos.ScaledBy(0.5);
	CFluidPrtl* newPrtl;
	for (int i=0; i < prtlLayerX; i++)
	{
		for (int j=0; j < prtlLayerY; j++)
		{
			for (int k=0; k < prtlLayerZ; k++)
			{
				tmpPos[X] = minX + i*prtlDistance;
				tmpPos[Y] = minY + j*prtlDistance;
				tmpPos[Z] = minZ + k*prtlDistance;
				newPrtl = pPrtlFluid->CreateOneFluidPrtl();
				newPrtl->m_fpMass = pPrtlFluid->m_pfPrtlMass;
				RotateStdVectToOrgVect(tmpPos, *newPrtl->m_vpPos);
				newPrtl->m_vpPos->AddedBy(&ctrPos);
				newPrtl->m_vpVel->SetValue((SimuValue)0);
				newPrtl->m_fpFixedMotion = false;
				pPrtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
				if (i == 0 || i == prtlLayerX-1
				 || j == 0 || j == prtlLayerY-1
				 || k == 0 || k == prtlLayerZ-1)
					newPrtl->m_fpOnSurface = true;
				else
					newPrtl->m_fpOnSurface = false;
			}
		}
	}
}
