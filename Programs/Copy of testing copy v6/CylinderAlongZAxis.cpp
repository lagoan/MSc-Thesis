// CylinderAlongZAxis.cpp: implementation of the CCylinderAlongZAxis class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "CylinderAlongZAxis.h"
#include "PrtlFluid.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCylinderAlongZAxis::CCylinderAlongZAxis()
{
	InitializeCylinder();
}

CCylinderAlongZAxis::CCylinderAlongZAxis(SimuValue length, SimuValue radius,
										 SimuValue capHeight,
										 bool bCylTopCap, bool bCylBtmCap,
										 CVector3D* topCtrPos, CVector3D* direction)
: CRotationToZAxis(direction)
{
	m_cylLength = length;
	m_cylRadius = radius;
	m_cylCapHeight = capHeight;
	m_cylTopCap = bCylTopCap;
	m_cylBtmCap = bCylBtmCap;
	m_cylTopCtrPos.SetValue(topCtrPos);
	m_cylBtmCtrPos.SetValue(topCtrPos);
	m_cylBtmCtrPos.AddedBy(&m_rtzaOrgDirection, -m_cylLength);
	// m_rtzaOrgDirection points from bottom to top

	InitializeCylinder();
}

CCylinderAlongZAxis::~CCylinderAlongZAxis()
{

}

void CCylinderAlongZAxis::InitializeCylinder()
{
	m_cazNewImportFormatTime = CSimuManager::GetSpecifiedTime(2005, 4, 10, 23); // 2005 May. 10, 11pm

	m_cylFixedPrtls = false;
	m_cylStickyTop = false;
	m_cylStickyBtm = false;
	m_cylStickyWall = false;

	m_cylSlices = 200;
	m_cylStacks = 200;
}

void CCylinderAlongZAxis::DrawCylinderAlongZAxis()
{
	GLUquadricObj* qObj = gluNewQuadric();
	const bool bDrawCylinderTop = true;
	if (bDrawCylinderTop)
	{
		glPushMatrix();
		SimuTranslate(m_cylTopCtrPos[X], m_cylTopCtrPos[Y], m_cylTopCtrPos[Z]);
		SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
		gluDisk(qObj, 0, m_cylRadius, m_cylSlices, m_cylStacks);
		glPopMatrix();
	}
	const bool bDrawCylinderBtm = true;
	if (bDrawCylinderBtm)
	{
		glPushMatrix();
		SimuTranslate(m_cylBtmCtrPos[X], m_cylBtmCtrPos[Y], m_cylBtmCtrPos[Z]);
		SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
		gluDisk(qObj, 0, m_cylRadius, m_cylSlices, m_cylStacks);
		glPopMatrix();
	}
	const bool bDrawCylinderWall = true;
	if (bDrawCylinderWall)
	{
		const SimuValue thickness = 0;
		glPushMatrix();
		SimuTranslate(m_cylBtmCtrPos[X], m_cylBtmCtrPos[Y], m_cylBtmCtrPos[Z]);
		SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
		gluCylinder(qObj, m_cylRadius+thickness, m_cylRadius+thickness,
							m_cylLength, m_cylSlices, m_cylStacks);
		glPopMatrix();
	}
	gluDeleteQuadric(qObj);

	SimuColor3v(CSimuManager::COLOR_RED->v);
	glPushMatrix();
	SimuTranslate(m_cylBtmCtrPos[X], m_cylBtmCtrPos[Y], m_cylBtmCtrPos[Z]);
	SimuRotate(m_rtzaRotationAngle, m_rtzaRotationV[X], m_rtzaRotationV[Y], m_rtzaRotationV[Z]);
	SimuRotate(m_rtzaRotationAngleAboutZAxis, 0, 0, 1);
	glBegin(GL_LINES);
	// draw direction
	SimuVertex3d(0, 0, m_cylLength);
	SimuVertex3d(0, 0, m_cylLength*2);
	// draw cross on top
	int numSlices = 4;
	SimuValue angle, posX, posY, posZ = m_cylLength+0.5;
	for (int i=0; i < numSlices; i++)
	{
		if (i % 2 == 0)
			SimuColor3v(CSimuManager::COLOR_RED->v);
		else
			SimuColor3v(CSimuManager::COLOR_MAGENTA->v);
		angle = 2*PI*i/numSlices;
		posX = m_cylRadius*cos(angle);
		posY = m_cylRadius*sin(angle);
		SimuVertex3d(posX, posY, posZ);
		SimuVertex3d(-posX, -posY, posZ);
	}
	glEnd();
	glPopMatrix();
}

void CCylinderAlongZAxis::ExportCylinderData(CString objName, FILE* fp)
{
	fprintf(fp, "#declare %s__m_cylLength = %f;\n", objName, m_cylLength);
	fprintf(fp, "#declare %s__m_cylRadius = %f;\n", objName, m_cylRadius);
	fprintf(fp, "#declare %s__m_cylCapHeight = %f;\n", objName, m_cylCapHeight);
	fprintf(fp, "#declare %s__m_cylTopCtrPos = <%f, %f, %f>;\n", objName,
							  m_cylTopCtrPos[X],
							  m_cylTopCtrPos[Y],
							  m_cylTopCtrPos[Z]);
	fprintf(fp, "#declare %s__m_cylBtmCtrPos = <%f, %f, %f>;\n", objName,
							  m_cylBtmCtrPos[X],
							  m_cylBtmCtrPos[Y],
							  m_cylBtmCtrPos[Z]);
	ExportRotationData(objName, fp);
}

void CCylinderAlongZAxis::ImportCylinderData(FILE* fp)
{
	char tmpName[LEN_FLUID_NAME];
	float tmpFloat, x, y, z;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_cylLength = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_cylRadius = tmpFloat;
	fscanf(fp, "#declare %s = %f;\n", tmpName, &tmpFloat); m_cylCapHeight = tmpFloat;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_cylTopCtrPos[X] = x;
							m_cylTopCtrPos[Y] = y;
							m_cylTopCtrPos[Z] = z;
	fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
							m_cylBtmCtrPos[X] = x;
							m_cylBtmCtrPos[Y] = y;
							m_cylBtmCtrPos[Z] = z;

	time_t fileTime = CSimuManager::GetFileModificationTime(fp);
	if (CSimuManager::FirstTimeIsLaterThanSecondTime(fileTime, m_cazNewImportFormatTime))
		ImportRotationData(fp);
	else
	{
		fscanf(fp, "#declare %s = <%f, %f, %f>;\n", tmpName, &x, &y, &z);
								m_rtzaOrgDirection[X] = x;
								m_rtzaOrgDirection[Y] = y;
								m_rtzaOrgDirection[Z] = z;
		m_rtzaOrgDirection.Normalize();
		ComputeRotationVariables();
		PrepareRotationMatrix();
	}
}

void CCylinderAlongZAxis::GetStandardPosition(CVector3D &transformedPos,
											  CVector3D &standardPos)
{
	transformedPos.AddedBy(&m_cylBtmCtrPos, -1);
	RotateOrgVectToStdVect(transformedPos, standardPos);
	transformedPos.AddedBy(&m_cylBtmCtrPos, 1);
}

void CCylinderAlongZAxis::GetTransformedPosition(CVector3D &standardPos,
												 CVector3D &transformedPos)
{
	RotateStdVectToOrgVect(standardPos, transformedPos);
	transformedPos.AddedBy(&m_cylBtmCtrPos, 1);
}

// return 0 if on or inside cylinder; otherwise, return positive value.
SimuValue CCylinderAlongZAxis::GetDistanceToCylinder(CVector3D &standardPos)
{
	SimuValue distToEnd;
	if (standardPos[Z] > m_cylLength)
		distToEnd = standardPos[Z] - m_cylLength; // prtl is over cylinder top.
	else if (standardPos[Z] < 0)
		distToEnd = - standardPos[Z]; // prtl is below cylinder bottom.
	else
		distToEnd = 0; // prtl is on or between top and bottom planes.

	SimuValue distToWall = sqrt(standardPos[X]*standardPos[X] + standardPos[Y]*standardPos[Y]);
	if (distToWall >= m_cylRadius)
		distToWall = distToWall - m_cylRadius; // prtl is outside cylinder wall.
	else
		distToWall = 0; // prtl is on or inside cylinder wall.

	return sqrt(distToEnd*distToEnd + distToWall*distToWall); 
}

void CCylinderAlongZAxis::TransferElementsToExpectedPosition(TGeomElemArray<CPrtlTriangle>
															 &aryTris,
															 TGeomElemArray<CMeshPrtl>
															 &aryPrtls)
{
	// rotate the cylinder from the standard position to expected position
	CVector3D tmpPos;
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = aryPrtls[i]; 
		RotateStdVectToOrgVect(*pPrtl->m_vpPos, tmpPos);
		pPrtl->m_vpPos->SetValue(&tmpPos);
		pPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
	}
	for (int i=0; i < aryTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = aryTris[i];
		pTri->ComputeTriAreaNormal();
	}
}

void CCylinderAlongZAxis::CreateFluidPrtlsRandomlyInCylinder(CPrtlFluid* pPrtlFluid,
															 int maxNumPrtls)
{
	int numPrtls = 0;
	CVector3D ranPos;
	while (numPrtls < maxNumPrtls)
	{
		CSimuUtility::CreateRandomPos(m_cylRadius, &ranPos);
		ranPos[Z] = 0.5*m_cylLength*(ranPos[Z] + m_cylRadius)/m_cylRadius;
		SimuValue radiusOnZPlane = sqrt(ranPos[X]*ranPos[X] + ranPos[Y]*ranPos[Y]);
		if (radiusOnZPlane <= m_cylRadius)
		{
			numPrtls ++;
			CFluidPrtl* newPrtl = pPrtlFluid->CreateOneFluidPrtl();
			pPrtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
			RotateStdVectToOrgVect(ranPos, *newPrtl->m_vpPos);
			newPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
		}
	}
}

void CCylinderAlongZAxis::CreateFluidPrtlsInCylinderWithVelocity(CPrtlFluid* pPrtlFluid,
																 CVector3D *velocity)
{
	CreateStretchingFluidInCylinder(pPrtlFluid, velocity, 0, 0, -1, false, false, false, false);
}

void CCylinderAlongZAxis::CreateFluidPrtlsInCylinder(CPrtlFluid* pPrtlFluid)
{
	bool bHasCap = false;
	if (m_cylCapHeight > SIMU_VALUE_EPSILON)
		bHasCap = true;
	CVector3D velocity;
	velocity.SetValue((SimuValue)0);
	CreateStretchingFluidInCylinder(pPrtlFluid, &velocity, 0, 0, -1, false, false, bHasCap, bHasCap);
	if (bHasCap)
	{
		if (m_cylTopCap)
			CreateFluidPrtlsInCap(pPrtlFluid, &m_cylTopCtrPos, m_cylCapHeight, true);
		if (m_cylBtmCap)
			CreateFluidPrtlsInCap(pPrtlFluid, &m_cylBtmCtrPos, m_cylCapHeight, false);
	}
}

void CCylinderAlongZAxis::CreateFluidPrtlsInHollowCylinder(CPrtlFluid* pPrtlFluid,
														   SimuValue hollowRadius)
{
	CVector3D velocity;
	velocity.SetValue((SimuValue)0);
	CreateStretchingFluidInCylinder(pPrtlFluid, &velocity, hollowRadius,
									0, -1, false, false, false, false);
}

void CCylinderAlongZAxis::CreateFluidPrtlsInCylinderWithFixedTop(CPrtlFluid* pPrtlFluid,
																 SimuValue fixedPartWidth,
																 SimuValue btmCapHeight)
{
	CVector3D velocity;
	velocity.SetValue((SimuValue)0);
	CreateStretchingFluidInCylinder(pPrtlFluid, &velocity, 0, 0, 
									fixedPartWidth, true, false, false, true);
	CreateFluidPrtlsInCap(pPrtlFluid, &m_cylBtmCtrPos, btmCapHeight, false);
}

// bCapUp indicates if the cap is on top of the cylinder or at the bottom, for standard cylinder position
void CCylinderAlongZAxis::CreateFluidPrtlsInCap(CPrtlFluid* pPrtlFluid,
												CVector3D* btmCtrPos,
												SimuValue capHeight,
												bool bCapUp)
{
	CVector3D tmpPos;
	SimuValue prtlDistance = CSimuManager::m_prtlDistance;
	int verticalLayers = 1 + (int)floor(capHeight/prtlDistance);
	int horizontalLayers = 1+2*(int)ceil(m_cylRadius/prtlDistance);
	SimuValue minX = -prtlDistance*(horizontalLayers-1)/2;
	SimuValue minY = -prtlDistance*(horizontalLayers-1)/2;
	SimuValue RX2 = m_cylRadius*m_cylRadius;
	SimuValue RY2 = m_cylRadius*m_cylRadius;
	SimuValue RZ2 = capHeight*capHeight;
	SimuValue normalizedThickness = prtlDistance*3/(m_cylRadius+m_cylRadius+capHeight);
	for (int i=1; i < verticalLayers; i++)
	{	// i=1, skip the bottom layer which has been created as the end layer of cylinder
		for (int j=0; j < horizontalLayers; j++)
		{
			for (int k=0; k < horizontalLayers; k++)
			{
				if (bCapUp)
					tmpPos[Z] = i*prtlDistance;
				else
					tmpPos[Z] = -i*prtlDistance;
				tmpPos[X] = minX + j*prtlDistance;
				tmpPos[Y] = minY + k*prtlDistance;
				SimuValue tmpDist = pow(tmpPos[X], 2)/RX2+pow(tmpPos[Y], 2)/RY2+pow(tmpPos[Z], 2)/RZ2;
				tmpDist = sqrt(tmpDist);
				if (tmpDist > 1 + normalizedThickness)
					continue;
				CFluidPrtl* newPrtl = pPrtlFluid->CreateOneFluidPrtl();
				// enforce the oval radius
				if (tmpDist > 1 - normalizedThickness)
				{
					newPrtl->m_fpOnSurface = true;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
					if (tmpDist > 1)
						tmpPos.NormalizeOntoOval(m_cylRadius, m_cylRadius, capHeight);
				}
				else
				{
					newPrtl->m_fpOnSurface = false;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
				}
				pPrtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
				RotateStdVectToOrgVect(tmpPos, *newPrtl->m_vpPos);
				newPrtl->m_vpPos->AddedBy(btmCtrPos);
				newPrtl->m_fpMass = pPrtlFluid->m_pfPrtlMass;
			}
		}
	}
}

void CCylinderAlongZAxis::CreateStretchingFluidInCylinder(CPrtlFluid* pPrtlFluid,
														  CVector3D *velocity,
														  SimuValue hollowRadius,
														  SimuValue stretchSpeed,
														  SimuValue stretchPartWidth,
														  bool bTopStretch, bool bBtmStretch,
														  bool bTopCovered, bool bBtmCovered)
{
	CVector3D tmpPos;
	SimuValue prtlDistance = CSimuManager::m_prtlDistance;
	int verticalLayers = 1 + (int)floor(m_cylLength/prtlDistance);
	int horizontalLayers = 1+2*(int)ceil(m_cylRadius/prtlDistance);
	SimuValue minX = -prtlDistance*(horizontalLayers-1)/2;
	SimuValue minY = -prtlDistance*(horizontalLayers-1)/2;
	int upStretchVLayer = (int)ceil((m_cylLength - stretchPartWidth)/prtlDistance);
	int downStretchVLayer = (int)floor(stretchPartWidth/prtlDistance);
	SimuValue radiusLayer = prtlDistance/2;
	for (int i=0; i < verticalLayers; i++)
	{
		for (int j=0; j < horizontalLayers; j++)
		{
			for (int k=0; k < horizontalLayers; k++)
			{
				tmpPos[Z] = i*prtlDistance;
				tmpPos[X] = minX + j*prtlDistance;
				tmpPos[Y] = minY + k*prtlDistance;
				SimuValue tmpDist = sqrt(pow(tmpPos[X], 2)+pow(tmpPos[Y], 2));
				CSimuUtility::JitterVectorValue(tmpPos, CSimuManager::m_prtlPosJitter*prtlDistance);
				if (tmpDist > m_cylRadius + radiusLayer
				 || tmpDist < hollowRadius - radiusLayer)
					continue;
				// enforce the radius
				if (tmpDist > m_cylRadius - radiusLayer)
				{
					tmpPos[X] *= m_cylRadius/tmpDist;
					tmpPos[Y] *= m_cylRadius/tmpDist;
				}
				else if (hollowRadius - radiusLayer > SIMU_VALUE_EPSILON
					  && tmpDist > SIMU_VALUE_EPSILON
					  && tmpDist < hollowRadius + radiusLayer)
				{
					tmpPos[X] *= hollowRadius/tmpDist;
					tmpPos[Z] *= hollowRadius/tmpDist;
				}

				CFluidPrtl* newPrtl = pPrtlFluid->CreateOneFluidPrtl();
				pPrtlFluid->m_pfPrtls.AppendOnePointer(newPrtl);
				RotateStdVectToOrgVect(tmpPos, *newPrtl->m_vpPos);
				newPrtl->m_vpPos->AddedBy(&m_cylBtmCtrPos);
				newPrtl->m_vpVel->SetValue(velocity);
				newPrtl->m_fpMass = pPrtlFluid->m_pfPrtlMass;
				if (tmpDist > m_cylRadius - radiusLayer
				 || (!bTopCovered && i == 0)
				 || (!bBtmCovered && i == verticalLayers-1))
				{
					newPrtl->m_fpOnSurface = true;
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_SRFC);
				}
				else
				{
					newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_INSIDE);
				}
				if (bTopStretch && i >= upStretchVLayer)
				{
					newPrtl->SetAsFixedMotion(pPrtlFluid->m_pfAirPressure);
					newPrtl->m_vpVel->SetValue(&m_rtzaOrgDirection, stretchSpeed);
				}
				if (bBtmStretch && i <= downStretchVLayer)
				{
					newPrtl->SetAsFixedMotion(pPrtlFluid->m_pfAirPressure);
					newPrtl->m_vpVel->SetValue(&m_rtzaOrgDirection, -stretchSpeed);
				}
				if (m_cylFixedPrtls)
				{
					newPrtl->m_fpNoDelaunay = true;
					if (m_cylStickyTop && i == verticalLayers-1)
						newPrtl->m_fpNoDelaunay = false;
					if (m_cylStickyBtm && i == 0)
						newPrtl->m_fpNoDelaunay = false;
					if (m_cylStickyWall && tmpDist > m_cylRadius - CSimuManager::m_prtlDistance)
						newPrtl->m_fpNoDelaunay = false;
					if (newPrtl->m_fpNoDelaunay)
						newPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_ORANGE);
				}
			}
		}
	}
}

void CCylinderAlongZAxis::CreateTriangularMeshOnCylinder(TGeomElemArray<CPrtlTriangle>
														 &aryTris,
														 TGeomElemArray<CMeshPrtl> &aryPrtls,
														 SimuValue capHeight)
{
	// compute necessary level
	SimuValue maxR = capHeight;
	if (maxR < m_cylRadius)
		maxR = m_cylRadius;
	// level <d> is such that the triangle side length is smaller than CSimuManager::m_prtlDistance
	int d = (int)ceil(0.125*PI/asin(CSimuManager::m_prtlDistance*0.5/maxR));
	
	SimuValue capRs[3] = {m_cylRadius, m_cylRadius, capHeight};
	CVector3D topPos, lefPos, rhtPos, frtPos, bakPos;
	topPos.SetElements(0, 0, capRs[Z]);
	lefPos.SetElements(-capRs[X], 0, 0); rhtPos.SetElements(capRs[X], 0, 0);
	frtPos.SetElements(0, -capRs[Y], 0); bakPos.SetElements(0, capRs[Y], 0);
	CreateTrianglesOnCapCylinder(aryTris, aryPrtls, d, true, capRs, topPos, frtPos, rhtPos);
	CreateTrianglesOnCapCylinder(aryTris, aryPrtls, d, true, capRs, topPos, lefPos, frtPos);
	CreateTrianglesOnCapCylinder(aryTris, aryPrtls, d, true, capRs, topPos, bakPos, lefPos);
	CreateTrianglesOnCapCylinder(aryTris, aryPrtls, d, true, capRs, topPos, rhtPos, bakPos);

	// compute surface triangle normals
	CVector3D ctrP; ctrP.SetElements(0, 0, m_cylLength/2);
	for (int i=0; i < aryTris.m_paNum; i++)
	{
		CPrtlTriangle* pTri = aryTris[i];
		pTri->ComputeTriAreaNormalByRefPos(&ctrP, true);
	}

	TransferElementsToExpectedPosition(aryTris, aryPrtls);

	// compute surface prtl normals
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CMeshPrtl* pPrtl = aryPrtls[i];
		pPrtl->ComputeNormalFromNgbrTris();
	}
}

void CCylinderAlongZAxis::CreateTrianglesOnCapCylinder(TGeomElemArray<CPrtlTriangle> &aryTris,
													   TGeomElemArray<CMeshPrtl> &aryPrtls,
													   int level, bool bOnRim,
													   SimuValue capRs[3],
													   CVector3D &p0,
													   CVector3D &p1,
													   CVector3D &p2)
{
	if (level == 0)
	{
		CMeshPrtl* pPrtls[3];
		CPrtlTriangle* newTri;
		// create triangle on upper cap
		CVector3D upP0, upP1, upP2;
		upP0.SetValue(&p0); upP0[Z] = p0[Z] + m_cylLength;
		upP1.SetValue(&p1); upP1[Z] = p1[Z] + m_cylLength;
		upP2.SetValue(&p2); upP2[Z] = p2[Z] + m_cylLength;
		pPrtls[0] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, upP0, CSimuManager::m_minPrtlDistance);
		pPrtls[1] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, upP1, CSimuManager::m_minPrtlDistance);
		pPrtls[2] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, upP2, CSimuManager::m_minPrtlDistance);
		newTri = new CPrtlTriangle();
		newTri->SetTriPrtls(pPrtls[0], pPrtls[1], pPrtls[2]);
		aryTris.AppendOnePointer(newTri);
		// create triangle on lower cap
		CVector3D lowP0, lowP1, lowP2;
		lowP0.SetValue(&p0); lowP0[Z] = - p0[Z];
		lowP1.SetValue(&p1); lowP1[Z] = - p1[Z];
		lowP2.SetValue(&p2); lowP2[Z] = - p2[Z];
		pPrtls[0] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, lowP0, CSimuManager::m_minPrtlDistance);
		pPrtls[1] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, lowP1, CSimuManager::m_minPrtlDistance);
		pPrtls[2] = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, lowP2, CSimuManager::m_minPrtlDistance);
		newTri = new CPrtlTriangle();
		newTri->SetTriPrtls(pPrtls[0], pPrtls[1], pPrtls[2]);
		aryTris.AppendOnePointer(newTri);
		if (bOnRim)
		{
			SimuValue width = upP1.GetDistanceToVector(&upP2);
			int stackNumber = (int)floor(m_cylLength/width + 0.5);
			SimuValue stackHeight = m_cylLength/stackNumber;
			// create triangles on the cylinder wall
			CVector3D topPos0, topPos1, btmPos0, btmPos1;
			topPos0.SetValue(&upP1); topPos1.SetValue(&upP2);
			btmPos0.SetValue(&upP1); btmPos1.SetValue(&upP2);
			for (int j=0; j < stackNumber; j++)
			{
				topPos0[Z] = m_cylLength - j*stackHeight;
				topPos1[Z] = topPos0[Z];
				btmPos0[Z] = topPos0[Z] - stackHeight;
				btmPos1[Z] = btmPos0[Z];
				CMeshPrtl* topP0 = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, topPos0, CSimuManager::m_minPrtlDistance);
				CMeshPrtl* topP1 = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, topPos1, CSimuManager::m_minPrtlDistance);
				CMeshPrtl* btmP0 = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, btmPos0, CSimuManager::m_minPrtlDistance);
				CMeshPrtl* btmP1 = CSimuUtility::AddDistinctMeshPrtl(aryPrtls, btmPos1, CSimuManager::m_minPrtlDistance);
				newTri = new CPrtlTriangle();
				newTri->SetTriPrtls(topP0, topP1, btmP1);
				aryTris.AppendOnePointer(newTri);
				newTri = new CPrtlTriangle();
				newTri->SetTriPrtls(topP0, btmP0, btmP1);
				aryTris.AppendOnePointer(newTri);
			}
		}
	}
	else
	{
		CVector3D m0, m1, m2;
		m0.SetValueAsAddition(&p0, &p1); m0.NormalizeOntoOval(capRs);
		m1.SetValueAsAddition(&p1, &p2); m1.NormalizeOntoOval(capRs);
		m2.SetValueAsAddition(&p2, &p0); m2.NormalizeOntoOval(capRs);
		CreateTrianglesOnCapCylinder(aryTris, aryPrtls, level-1, false, capRs, p0, m0, m2);
		CreateTrianglesOnCapCylinder(aryTris, aryPrtls, level-1, bOnRim, capRs, m0, p1, m1);
		CreateTrianglesOnCapCylinder(aryTris, aryPrtls, level-1, bOnRim, capRs, m2, m1, p2);
		CreateTrianglesOnCapCylinder(aryTris, aryPrtls, level-1, false, capRs, m0, m1, m2);
	}
}
