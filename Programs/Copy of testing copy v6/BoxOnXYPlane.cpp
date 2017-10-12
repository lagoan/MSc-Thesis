// BoxOnXYPlane.cpp: implementation of the CBoxOnXYPlane class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "BoxOnXYPlane.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBoxOnXYPlane::CBoxOnXYPlane()
{
}

CBoxOnXYPlane::CBoxOnXYPlane(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
							 SimuValue angleAboutZ, CVector3D* btmCtrPos, CVector3D* direction)
: CRotationToZAxis(direction, angleAboutZ)
{
	m_boxHeightOnZ = heightOnZ;
	m_boxWidthOnX = widthOnX;
	m_boxWidthOnY = widthOnY;
	m_boxBtmCtrPos.SetValue(btmCtrPos);
	m_boxTopCtrPos.SetValue(btmCtrPos);
	m_boxTopCtrPos.AddedBy(&m_rtzaOrgDirection, m_boxHeightOnZ);

	ComputeBoxVariables();
}

CBoxOnXYPlane::~CBoxOnXYPlane()
{

}

void CBoxOnXYPlane::DrawBoxOnXYPlane()
{
	glPushMatrix();
	SimuTranslate(m_boxBtmCtrPos[X], m_boxBtmCtrPos[Y], m_boxBtmCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	SimuRotate(m_rtzaRotationAngleAboutZAxis, 0, 0, 1);
	glBegin(GL_LINES);
	// draw 4 edges along x direction
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	// draw 4 edges along y direction
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	// draw 4 edges along z direction
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMinPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMinPos[Y], m_boxMaxPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMinPos[Z]);
	SimuVertex3d(m_boxMaxPos[X], m_boxMaxPos[Y], m_boxMaxPos[Z]);
	glEnd();
	glPopMatrix();
}

void CBoxOnXYPlane::ExportBoxData(CString objName, FILE* fp)
{
	fprintf(fp, "#declare %s__m_boxHeightOnZ = %f;\n", objName, m_boxHeightOnZ);
	fprintf(fp, "#declare %s__m_boxWidthOnX = %f;\n", objName, m_boxWidthOnX);
	fprintf(fp, "#declare %s__m_boxWidthOnY = %f;\n", objName, m_boxWidthOnY);
	fprintf(fp, "#declare %s__m_boxTopCtrPos = <%f, %f, %f>;\n", objName,
							  m_boxTopCtrPos[X],
							  m_boxTopCtrPos[Y],
							  m_boxTopCtrPos[Z]);
	fprintf(fp, "#declare %s__m_boxBtmCtrPos = <%f, %f, %f>;\n", objName,
							  m_boxBtmCtrPos[X],
							  m_boxBtmCtrPos[Y],
							  m_boxBtmCtrPos[Z]);
	ExportRotationData(objName, fp);
}

void CBoxOnXYPlane::ImportBoxData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_boxHeightOnZ = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_boxWidthOnX = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_boxWidthOnY = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_boxTopCtrPos[X] = x;
							m_boxTopCtrPos[Y] = y;
							m_boxTopCtrPos[Z] = z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_boxBtmCtrPos[X] = x;
							m_boxBtmCtrPos[Y] = y;
							m_boxBtmCtrPos[Z] = z;
	ImportRotationData(fp);
	ComputeBoxVariables();
}

void CBoxOnXYPlane::ComputeBoxVariables()
{
	m_boxMinPos[X] = -m_boxWidthOnX/2;
	m_boxMinPos[Y] = -m_boxWidthOnY/2;
	m_boxMinPos[Z] = 0;
	m_boxMaxPos[X] = m_boxWidthOnX/2;
	m_boxMaxPos[Y] = m_boxWidthOnY/2;
	m_boxMaxPos[Z] = m_boxHeightOnZ;
}

void CBoxOnXYPlane::GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos)
{
	transformedPos.AddedBy(&m_boxBtmCtrPos, -1);
	RotateOrgVectToStdVect(transformedPos, standardPos);
	transformedPos.AddedBy(&m_boxBtmCtrPos, 1);
}

void CBoxOnXYPlane::GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos)
{
	RotateStdVectToOrgVect(standardPos, transformedPos);
	transformedPos.AddedBy(&m_boxBtmCtrPos, 1);
}

SimuValue CBoxOnXYPlane::GetDistanceToBox(CVector3D &standardPos)
{
	CVector3D distVect;
	for (int d = X; d <= Z; d++)
	{
		if (standardPos[d] < m_boxMinPos[d])
		{
			distVect[d] = m_boxMinPos[d] - standardPos[d];
		}
		else if (standardPos[d] > m_boxMaxPos[d])
		{
			distVect[d] = standardPos[d] - m_boxMaxPos[d];
		}
		else // standardPos[d] >= m_boxMinPos[d] && standardPos[d] <= m_boxMaxPos[d]
		{
			distVect[d] = 0;
		}
	}
	return distVect.Length();
}

void CBoxOnXYPlane::CreatePrtlsInBox(CPrtlFluid* pPrtlFluid,
									 CVector3D* btmCtrPos, CVector3D* direction,
									 SimuValue angleAboutZ, SimuValue angularVel,
									 int layers[Z+1], SimuValue layerDistance)
{
	CVector3D velocity;
	velocity.SetValue((SimuValue)0);
	CreatePrtlsInBoxWithVelocity(pPrtlFluid, btmCtrPos, direction, &velocity,
								angleAboutZ, angularVel, layers, layerDistance);
}

void CBoxOnXYPlane::CreatePrtlsInBoxWithVelocity(CPrtlFluid* pPrtlFluid, CVector3D* btmCtrPos,
												 CVector3D* direction, CVector3D* velocity,
												 SimuValue angleAboutZ, SimuValue angularVel,
												 int layers[Z+1], SimuValue layerDistance)
{
	CRotationToZAxis* rotationToZAxis = new CRotationToZAxis(direction, angleAboutZ);

	CVector3D minPos;
	for (int d=X; d < Z; d++)
	{
		if (layers[d] <= 1)
			minPos[d] = 0;
		else
			minPos[d] = -0.5*(layers[d]-1)*layerDistance;
	}
	minPos[Z] = 0;
	CVector3D prtlPos;
	CVector3D prtlVel;
	CVector3D unitVOnZ; unitVOnZ.SetElements(0, 0, 1);
	for (int x=0; x < layers[X]; x++)
	for (int y=0; y < layers[Y]; y++)
	for (int z=0; z < layers[Z]; z++)
	{
		prtlPos[X] = minPos[X] + x*layerDistance;
		prtlPos[Y] = minPos[Y] + y*layerDistance;
		prtlPos[Z] = minPos[Z] + z*layerDistance;
		CSimuUtility::JitterVectorValue(prtlPos, CSimuManager::m_prtlPosJitter*layerDistance);
		CFluidPrtl* pPrtl = pPrtlFluid->CreateOneFluidPrtl();
		pPrtlFluid->m_pfPrtls.AppendOnePointer(pPrtl);
		if (x == 0 || x == layers[X] - 1
		 || y == 0 || y == layers[Y] - 1
		 || z == 0 || z == layers[Z] - 1)
		{
			pPrtl->m_fpOnSurface = true;
			pPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
		}
		// compute angular velocity
		rotationToZAxis->RotateStdVectToOrgVect(prtlPos, *pPrtl->m_vpPos);
		pPrtl->m_vpPos->AddedBy(btmCtrPos);
		prtlPos[Z] = 0;
		prtlVel.SetValueAsCrossProduct(&unitVOnZ, &prtlPos);
		prtlVel.Normalize(angularVel*prtlPos.Length());
		rotationToZAxis->RotateStdVectToOrgVect(prtlVel, *pPrtl->m_vpVel);
		// add tranlational velocity
		pPrtl->m_vpVel->AddedBy(velocity);
	}

	delete rotationToZAxis;
}

// stretching is along the direction of x axis
void CBoxOnXYPlane::CreateStretchingFluidInBox(CPrtlFluid* pPrtlFluid,
											   CVector3D* btmCtrPos, CVector3D* direction,
											   SimuValue angleAboutZ, int layers[Z+1],
											   SimuValue layerDistance, SimuValue stretchSpeed,
											   int stretchLayers)
{
	if (stretchLayers*2 > layers[X])
	{
		CSimuMsg::ExitWithMessage("CBoxOnXYPlane::CreateStretchingFluidInBox(...)",
								"Stretch layers are more than fluid layers along x axis.");
		return;
	}
	CRotationToZAxis* rotationToZAxis = new CRotationToZAxis(direction, angleAboutZ);

	CVector3D minPos;
	for (int d=X; d < Z; d++)
	{
		if (layers[d] <= 1)
			minPos[d] = 0;
		else
			minPos[d] = -0.5*(layers[d]-1)*layerDistance;
	}
	minPos[Z] = 0;
	CVector3D prtlPos;
	CVector3D leftStretchVel, rightStretchVel;
	leftStretchVel.SetElements(-stretchSpeed, 0, 0);
	rightStretchVel.SetElements(stretchSpeed, 0, 0);
	for (int x=0; x < layers[X]; x++)
	for (int y=0; y < layers[Y]; y++)
	for (int z=0; z < layers[Z]; z++)
	{
		prtlPos[X] = minPos[X] + x*layerDistance;
		prtlPos[Y] = minPos[Y] + y*layerDistance;
		prtlPos[Z] = minPos[Z] + z*layerDistance;
		CFluidPrtl* pPrtl = pPrtlFluid->CreateOneFluidPrtl();
		pPrtlFluid->m_pfPrtls.AppendOnePointer(pPrtl);
		if (x == 0 || x == layers[X] - 1
		 || y == 0 || y == layers[Y] - 1
		 || z == 0 || z == layers[Z] - 1)
		{
			pPrtl->m_fpOnSurface = true;
			pPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
		}
		rotationToZAxis->RotateStdVectToOrgVect(prtlPos, *pPrtl->m_vpPos);
		pPrtl->m_vpPos->AddedBy(btmCtrPos);
		if (x < stretchLayers)
		{
			pPrtl->SetAsFixedMotion(pPrtlFluid->m_pfAirPressure);
			rotationToZAxis->RotateStdVectToOrgVect(leftStretchVel, *pPrtl->m_vpVel);
		}
		else if (x >= layers[X]-stretchLayers)
		{
			pPrtl->SetAsFixedMotion(pPrtlFluid->m_pfAirPressure);
			rotationToZAxis->RotateStdVectToOrgVect(rightStretchVel, *pPrtl->m_vpVel);
		}
	}

	delete rotationToZAxis;
}