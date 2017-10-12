// OvalAlong3DAxes.cpp: implementation of the COvalAlong3DAxes class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "OvalAlong3DAxes.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

COvalAlong3DAxes::COvalAlong3DAxes()
{
	m_ovalRadiusOnX = 0;
	m_ovalRadiusOnY = 0;
	m_ovalRadiusOnZ = 0;
}

COvalAlong3DAxes::COvalAlong3DAxes(SimuValue radiusOnX, SimuValue radiusOnY,
								   SimuValue radiusOnZ, SimuValue angleAboutZ,
								   CVector3D* ctrPos, CVector3D* direction)
: CRotationToZAxis(direction, angleAboutZ)
{
	m_ovalRadiusOnX = radiusOnX;
	m_ovalRadiusOnY = radiusOnY;
	m_ovalRadiusOnZ = radiusOnZ;
	m_ovalRX2 = pow(m_ovalRadiusOnX, 2);
	m_ovalRY2 = pow(m_ovalRadiusOnY, 2);
	m_ovalRZ2 = pow(m_ovalRadiusOnZ, 2);

	m_ovalCtrPos.SetValue(ctrPos);
}

COvalAlong3DAxes::~COvalAlong3DAxes()
{

}

void COvalAlong3DAxes::DrawOvalAlong3DAxes()
{
	const int numSegments = 20;
	const SimuValue radius = 1;
	// draw oval on XY-plane
	glPushMatrix();
	SimuScale(m_ovalRadiusOnX, m_ovalRadiusOnY, 1);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(radius, 0, numSegments);
	glPopMatrix();
	// draw oval on XZ-plane
	glPushMatrix();
	SimuScale(m_ovalRadiusOnX, 1, m_ovalRadiusOnZ);
	SimuRotate(90, 1, 0, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(radius, 0, numSegments);
	glPopMatrix();
	// draw oval on YZ-plane
	glPushMatrix();
	SimuScale(1, m_ovalRadiusOnY, m_ovalRadiusOnZ);
	SimuRotate(90, 0, 1, 0);
	CSimuUtility::DrawCircleOnZ0ByOpenGL(radius, 0, numSegments);
	glPopMatrix();
}

void COvalAlong3DAxes::ExportOvalData(CString objName, FILE* fp)
{
	fprintf(fp, "#declare %s__m_ovalCtrPos = <%f, %f, %f>;\n", objName,
							  m_ovalCtrPos[X],
							  m_ovalCtrPos[Y],
							  m_ovalCtrPos[Z]);
	fprintf(fp, "#declare %s__m_ovalRadiusOnX = %f;\n", objName, m_ovalRadiusOnX);
	fprintf(fp, "#declare %s__m_ovalRadiusOnY = %f;\n", objName, m_ovalRadiusOnY);
	fprintf(fp, "#declare %s__m_ovalRadiusOnZ = %f;\n", objName, m_ovalRadiusOnZ);
}

void COvalAlong3DAxes::ImportOvalData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_ovalCtrPos[X] = x;
							m_ovalCtrPos[Y] = y;
							m_ovalCtrPos[Z] = z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_ovalRadiusOnX = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_ovalRadiusOnY = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_ovalRadiusOnZ = tmpFloat;
}

void COvalAlong3DAxes::GetStandardPosition(CVector3D &transformedPos, CVector3D &standardPos)
{
	transformedPos.AddedBy(&m_ovalCtrPos, -1);
	RotateOrgVectToStdVect(transformedPos, standardPos);
	transformedPos.AddedBy(&m_ovalCtrPos, 1);
}

void COvalAlong3DAxes::GetTransformedPosition(CVector3D &standardPos, CVector3D &transformedPos)
{
	RotateStdVectToOrgVect(standardPos, transformedPos);
	transformedPos.AddedBy(&m_ovalCtrPos, 1);
}

void COvalAlong3DAxes::CreateTriangularMeshOnOval(TGeomElemArray<CPrtlTriangle> &aryTris,
												  TGeomElemArray<CMeshPrtl> &aryPrtls,
												  CVector3D &ctrPos, CVector3D &direction,
												  SimuValue ovalRs[3])
{
	// compute necessary level
	SimuValue maxR = ovalRs[0];
	if (maxR < ovalRs[1])
		maxR = ovalRs[1];
	if (maxR < ovalRs[2])
		maxR = ovalRs[2];
	// level <d> is such that the triangle side length is smaller than CSimuManager::m_prtlDistance
	SimuValue alpha = 2*asin(CSimuManager::m_prtlDistance*0.5/maxR);
	int d = (int)ceil(0.25*PI/alpha - 0.25);

	// create oval in standard coordinates
	CVector3D topPos, btmPos, lefPos, rhtPos, frtPos, bakPos;
	topPos.SetElements(0, ovalRs[Y], 0); btmPos.SetElements(0, -ovalRs[Y], 0);
	lefPos.SetElements(-ovalRs[X], 0, 0); rhtPos.SetElements(ovalRs[X], 0, 0);
	frtPos.SetElements(0, 0, ovalRs[Z]); bakPos.SetElements(0, 0, -ovalRs[Z]);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, topPos, frtPos, rhtPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, topPos, lefPos, frtPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, topPos, bakPos, lefPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, topPos, rhtPos, bakPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, btmPos, frtPos, rhtPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, btmPos, lefPos, frtPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, btmPos, bakPos, lefPos);
	CreateTrianglesOnOval(aryTris, aryPrtls, d, ovalRs, btmPos, rhtPos, bakPos);

	CRotationToZAxis* rotationToZAxis = new CRotationToZAxis(&direction, 0);
	CVector3D orgPos;
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = aryPrtls[i];
		orgPos.SetValue(pPrtl->m_vpPos);
		rotationToZAxis->RotateStdVectToOrgVect(orgPos, *pPrtl->m_vpPos);
		pPrtl->m_vpPos->AddedBy(&ctrPos);
		pPrtl->m_mpOnSurface = true;
	}
	for (int i=0; i < aryTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = aryTris[i];
		pTri->ComputeTriAreaNormalByRefPos(&ctrPos, true);
	}
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = aryPrtls[i];
		pPrtl->ComputeNormalFromNgbrTris();
	}
	delete rotationToZAxis;
}

void COvalAlong3DAxes::CreateTrianglesOnOval(TGeomElemArray<CPrtlTriangle> &aryTris,
											 TGeomElemArray<CMeshPrtl> &aryPrtls,
											 int level, SimuValue ovalRs[3],
											 CVector3D &p0, CVector3D &p1, CVector3D &p2)
{
	if (level == 0)
	{
		CMeshPrtl* pPrtls[3];

		pPrtls[0] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, p0, CSimuManager::m_minPrtlDistance);
		pPrtls[1] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, p1, CSimuManager::m_minPrtlDistance);
		pPrtls[2] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, p2, CSimuManager::m_minPrtlDistance);

		CPrtlTriangle* newTri = new CPrtlTriangle();
		newTri->SetTriPrtls(pPrtls[0], pPrtls[1], pPrtls[2]);
		aryTris.AppendOnePointer(newTri);
	}
	else
	{
		CVector3D m0, m1, m2;
		m0.SetValueAsAddition(&p0, &p1); m0.NormalizeOntoOval(ovalRs);
		m1.SetValueAsAddition(&p1, &p2); m1.NormalizeOntoOval(ovalRs);
		m2.SetValueAsAddition(&p2, &p0); m2.NormalizeOntoOval(ovalRs);
		CreateTrianglesOnOval(aryTris, aryPrtls, level-1, ovalRs, p0, m0, m2);
		CreateTrianglesOnOval(aryTris, aryPrtls, level-1, ovalRs, m0, p1, m1);
		CreateTrianglesOnOval(aryTris, aryPrtls, level-1, ovalRs, m2, m1, p2);
		CreateTrianglesOnOval(aryTris, aryPrtls, level-1, ovalRs, m0, m1, m2);
	}
}

