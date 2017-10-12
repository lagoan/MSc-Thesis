// SimuModeler.cpp : implementation file
//

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuModeler.h"
#include "SimuView.h"

#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSimuModeler

CSimuModeler::CSimuModeler(CSimuManager* pSimuManager)
{
	if (pSimuManager == NULL)
	{
		CSimuMsg::ExitWithMessage("CSimuModeler::CSimuModeler()", "pSimuManager == NULL.");
	}
	if (pSimuManager->m_pSimuView == NULL)
	{
		CSimuMsg::ExitWithMessage("CSimuModeler::CSimuModeler()", "pSimuManager->m_pSimuView == NULL.");
	}
	m_pSimuManager	= pSimuManager;
	m_pSimuView		= pSimuManager->m_pSimuView;
}

CSimuModeler::~CSimuModeler()
{
}


/////////////////////////////////////////////////////////////////////////////
// CSimuModeler public methods
CPrtlFluid* CSimuModeler::CreateEggWithYolk(CPrtlFluid* eggWhite, int num, 
											CVector3D* eggCtrPos, CVector3D* eggIniVel,
											SimuValue eggRadius, SimuValue yolkRatio,
											SimuValue yolkAirPressure,
											SimuValue yolkShearModulus,
											SimuValue yolkRelaxationTime,
											SimuValue yolkPrtlMassRatio)
{
	SimuValue srfcThickness = CSimuManager::m_prtlDistance*0.5;
	CVector3D eggDirection; eggDirection.SetElements(0, 0, 1);
	CPrtlInOval* prtlInOval;
	if (eggWhite != NULL)
	{
		prtlInOval = new CPrtlInOval(eggRadius, eggRadius, eggRadius, 0, eggCtrPos, &eggDirection);
		prtlInOval->CreatePrtlInOval(eggWhite, srfcThickness, yolkRatio, eggIniVel);
		delete prtlInOval;
	}

	char yolkName[100];
	sprintf(yolkName, "yolk%d", num);
	CPrtlFluid* fluidYolk = CSimuManager::CreatePrtlFluid();
	fluidYolk->m_deiName = yolkName;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidYolk);
	fluidYolk->m_pfSimuManager = m_pSimuManager;
	fluidYolk->m_pfAirPressure = yolkAirPressure;
	fluidYolk->m_pfShearModulus = yolkShearModulus;
	fluidYolk->m_pfRelaxationTime = yolkRelaxationTime;
	fluidYolk->m_pfPrtlMassRatio = yolkPrtlMassRatio;
	fluidYolk->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	SimuValue yolkRadius = eggRadius*yolkRatio;
	prtlInOval = new CPrtlInOval(yolkRadius, yolkRadius, yolkRadius,
								  0, eggCtrPos, &eggDirection);
	prtlInOval->CreatePrtlInOval(fluidYolk, srfcThickness, 0, eggIniVel);
	delete prtlInOval;
	fluidYolk->PreparePrtlEvolution();
	switch (num)
	{
		case 0:
			fluidYolk->m_pfColor.SetValue(CSimuManager::COLOR_RED);
			fluidYolk->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
			break;
		case 1:
			fluidYolk->m_pfColor.SetValue(CSimuManager::COLOR_MAGENTA);
			fluidYolk->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
			break;
		default:
			fluidYolk->m_pfColor.SetValue(CSimuManager::COLOR_GREEN);
			fluidYolk->m_pfSrfcColor.SetValue(CSimuManager::COLOR_GREEN);
	}
	fluidYolk->ApplyFluidColorOntoPrtls();

	return fluidYolk;
}

/////////////////////////////////////////////////////////////////////////////
// CSimuModeler private methods


BEGIN_MESSAGE_MAP(CSimuModeler, CWnd)
	//{{AFX_MSG_MAP(CSimuModeler)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDBALL, OnFluidmodelFluidball)
	ON_COMMAND(ID_TEST_CONVEXHULL, OnTestConvexhull)
	ON_COMMAND(ID_TEST_DELAUNAYTETRAHEDRON, OnTestDelaunaytetrahedron)
	ON_COMMAND(ID_TEST_DISTANCETOCONVEXHULL, OnTestDistancetoconvexhull)
	ON_COMMAND(ID_TEST_UPSAMPLEDELAUNAYTETRAHEDRON, OnTestUpsampledelaunaytetrahedron)
	ON_COMMAND(ID_TEST_DELAUNAYTESSELLATION, OnTestDelaunaytessellation)
	ON_COMMAND(ID_TEST_UPSAMPLEDELAUNAYTESSELLATION, OnTestUpsampledelaunaytessellation)
	ON_COMMAND(ID_TEST_PARTIALDELAUNAYTESSELLATION, OnTestPartialdelaunaytessellation)
	ON_COMMAND(ID_TEST_CONVEXHULLONGRID, OnTestConvexhullongrid)
	ON_COMMAND(ID_TEST_DELAUNAYTETRAHEDRONOBJECT, OnTestDelaunaytetrahedronobject)
	ON_COMMAND(ID_FLUIDMODEL_NOZZLEINBOX, OnFluidmodelNozzleinbox)
	ON_COMMAND(ID_RIGIDMODEL_BALL, OnRigidmodelBall)
	ON_COMMAND(ID_RIGIDMODEL_CYLINDER, OnRigidmodelCylinder)
	ON_COMMAND(ID_RIGIDMODEL_CUBEONSLOPE, OnRigidmodelCubeonslope)
	ON_COMMAND(ID_FLUIDMODEL_DROPLETINTOWATERINBOWL, OnFluidmodelDropletintowaterinbowl)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDVERTICALSTRETCHING, OnFluidmodelFluidverticalstretching)
	ON_COMMAND(ID_FLUIDMODEL_PULLINGUPFLUID, OnFluidmodelPullingupfluid)
	ON_COMMAND(ID_TEST_SOLIDBOXBOUNDARY, OnTestSolidboxboundary)
	ON_COMMAND(ID_FLUIDMODEL_BALLSHOOTFLUIDTOWER, OnFluidmodelBallshootfluidtower)
	ON_COMMAND(ID_ROTATEFLUID_RODCLIMBING, OnRotatefluidRodclimbing)
	ON_COMMAND(ID_ROTATEFLUID_BALLONROTATEDISK, OnRotatefluidBallonrotatedisk)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDBALLBYRANDOMPRTLS, OnFluidmodelFluidballbyrandomprtls)
	ON_COMMAND(ID_INTERMODEL_TWOCOLLIDINGBALLS, OnIntermodelTwocollidingballs)
	ON_COMMAND(ID_INTERMODEL_RODHITTINGDISK, OnIntermodelRodhittingdisk)
	ON_COMMAND(ID_INTERMODEL_BALLHITTINGDISK, OnIntermodelBallhittingdisk)
	ON_COMMAND(ID_INTERMODEL_BOARDCOLLIDING, OnIntermodelBoardcolliding)
	ON_COMMAND(ID_INTERMODEL_BALLHITTINGTOWER, OnIntermodelBallhittingtower)
	ON_COMMAND(ID_INTERMODEL_MULTIPLEBALLS, OnIntermodelMultipleballs)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDSTRETCHING, OnFluidmodelFluidstretching)
	ON_COMMAND(ID_ROTATEFLUID_ROTATEROD, OnRotatefluidRotaterod)
	ON_COMMAND(ID_INTERMODEL_COLLIDINGCUBES, OnIntermodelCollidingcubes)
	ON_COMMAND(ID_INTERMODEL_BALLINTOJELLO, OnIntermodelBallintojello)
	ON_COMMAND(ID_INTERMODEL_EGGWITHYORK, OnIntermodelEggwithyork)
	ON_COMMAND(ID_INTERMODEL_MULTIPLEEGGS, OnIntermodelMultipleeggs)
	ON_COMMAND(ID_TEST_SURFACETENSIONONCUBE, OnTestSurfacetensiononcube)
	ON_COMMAND(ID_INTERMODEL_EGGDROPINTOBOWL, OnIntermodelEggdropintobowl)
	ON_COMMAND(ID_INTERMODEL_EGGTHROUGHFUNNEL, OnIntermodelEggthroughfunnel)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDCUBE, OnFluidmodelFluidcube)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDBAROVERROD, OnFluidmodelFluidbaroverrod)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDCYLINDEROVERROD, OnFluidmodelFluidcylinderoverrod)
	ON_COMMAND(ID_ROTATEFLUID_BOXONROTATEPLATE, OnRotatefluidBoxonrotateplate)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDOVERHOTRING, OnFluidmodelFluidoverhotring)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDJUMPINGONSLOPE, OnFluidmodelFluidjumpingonslope)
	ON_COMMAND(ID_FLUIDMODEL_FLUIDWALLHITBYHOTBALL, OnFluidmodelFluidwallhitbyhotball)
	ON_COMMAND(ID_ROTATEFLUID_FALLONDECLINEDROTATEPLATE, OnRotatefluidFallondeclinedrotateplate)
	ON_COMMAND(ID_INTERMODEL_TWOCOLLIDINGBARS, OnIntermodelTwocollidingbars)
	ON_COMMAND(ID_INTERMODEL_EGGONTOHOTPLATE, OnIntermodelEggontohotplate)
	ON_COMMAND(ID_ROTATEFLUID_BOXTWISTSFLUID, OnRotatefluidBoxtwistsfluid)
	ON_COMMAND(ID_INTERMODEL_MULTIPLECOLLIDINGCUBES, OnIntermodelMultiplecollidingcubes)
	ON_COMMAND(ID_ROTATEFLUID_POLEONROTATEPLATE, OnRotatefluidPoleonrotateplate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSimuModeler message handlers

void CSimuModeler::OnFluidmodelDropletintowaterinbowl() 
{
	// TODO: Add your command handler code here
	
	CVector3D bowlDirection; bowlDirection.SetElements(0, 1, 0); bowlDirection.Normalize();
	CVector3D ballDirection; ballDirection.SetValue(&bowlDirection);
	CVector3D ballCtrPos;	ballCtrPos.SetElements(0, 0, 0);

	// seed = 1089841186
//	SimuValue randomSeed = 1089841186; //(unsigned)time(NULL);
//	srand(randomSeed);
	CSimuManager::m_prtlPosJitter		= 0.01;
	CSimuManager::m_distDumpingRate		= 0.5;	// 0 means complete dumping
	CSimuManager::m_energyDumpingRate	= 0.5;	// 0 means complete dumping
	CSimuManager::GRAVITY->SetElements(0, -200, 0);
	CVector3D ballIniVel;	ballIniVel.SetElements(0, -2000, 0);

	SimuValue ballRadius = 55;
	SimuValue distToBowl = 10;

	CSimuManager::m_eFluidModel = CSimuManager::SPH; // UPSAMPLE; // 
	CSimuManager::m_bSpikyGradient	= true;
	CSimuManager::m_frictionRate	= 0;
	CSimuManager::m_prtlDensity		= 1;
	CSimuManager::NON_NEWTONIAN_FLUID = false;
	CSimuManager::m_shearModulus	= pow(10.0f, 2.0f);
	CSimuManager::m_relaxationTime	= pow(10.0f, -2.0f);

	m_pSimuManager->CreateCurrentPrtlFluid();
	CPrtlFluid* fluidBall = m_pSimuManager->m_curPrtlFluid;

	CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
											  0, &ballCtrPos, &ballDirection);
	prtlInOval->CreatePrtlInOval(fluidBall, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;

	// create bowl
	SimuValue bowlHeight = 100;
	SimuValue bowlRadius = 200;
	CVector3D bowlBtmPos;
	bowlBtmPos.SetValue(&ballCtrPos);
	bowlBtmPos.AddedBy(&bowlDirection, - (ballRadius + distToBowl));
	CBdryBowl* pBdryBowl = new CBdryBowl(&bowlDirection, &bowlBtmPos, bowlHeight, bowlRadius);
	pBdryBowl->m_bdryColor->SetValue(CSimuManager::COLOR_ORANGE);
	fluidBall->AddBoundary(pBdryBowl);
	// create square slope
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ballCtrPos);
	squareCtrPos.AddedBy(&bowlDirection, -(ballRadius + distToBowl));
	SimuValue squareSideLen = 1000;
	CBdrySquareSlope* bdrySquareSlope = new CBdrySquareSlope(&bowlDirection,
															 &squareCtrPos,
															 squareSideLen, 0);
	fluidBall->AddBoundary(bdrySquareSlope);

	fluidBall->PreparePrtlEvolution();
	fluidBall->InitializePrtlVelocities(ballIniVel);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidball() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = false;
	bool bSquarePlate = true;
	bool bFenceOnPlate = false;
	bool bFluidCylinder = false;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D plateDirection; plateDirection.SetElements(0, 1, 0); plateDirection.Normalize();
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CSimuManager::m_minTemperature = 10;
	CSimuManager::m_maxTemperature = 500;
	CSimuManager::m_minShearModulus = pow(10.0f, 3.0f);
	SimuValue plateRadius = 60;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_airPressure = -600;
	SimuValue ballRadius = 70;
	SimuValue plateHeight = 30;
	SimuValue fenceHeight = 70 + plateHeight;
	SimuValue fenceWidth = 30;

	CSimuManager::m_heatConductivity = 400;
	CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	SimuValue plateTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_maxShearModulus = 2*pow(10.0f, 6.0f);
	SimuValue distToPlate = 200;
	CSimuManager::m_onlyGravityTime = 1.95;
	CSimuManager::m_numStepDivPerFrame = 4;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.2;
	SimuValue plateTopToFloor = 200;

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "fluid_ball";
	pFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	if (bFluidCylinder)
	{
		CVector3D cylTopCtrPos;
		cylTopCtrPos.SetValue(&ctrPos);
		cylTopCtrPos[Y] += distToPlate + ballRadius*2;
		CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(ballRadius*2, ballRadius,
																 0, false, false,
																 &cylTopCtrPos, &floorDirection);
		pCylinder->CreateFluidPrtlsInCylinder(pFluid);
	}
	else
	{
		CVector3D ballCtrPos;
		ballCtrPos.SetValue(&ctrPos);
		ballCtrPos[Y] += distToPlate + ballRadius;
		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos, &floorDirection);
		prtlInOval->CreatePrtlInOval(pFluid, CSimuManager::m_prtlDistance*0.5, 0);
		delete prtlInOval;
	}
	pFluid->PreparePrtlEvolution();

	// create hot plate
	if (bSquarePlate)
	{
		CVector3D plateBtmCtrPos;
		plateBtmCtrPos.SetValue(&ctrPos);
		plateBtmCtrPos[Y] -= plateHeight;
		CBdrySolidBox* pPlate = new CBdrySolidBox(plateHeight, 2*plateRadius, 2*plateRadius,
										   0, &plateBtmCtrPos, &plateDirection);
		pPlate->m_bdryThermalBody = true;
		pPlate->m_bdryTemperature = plateTemperature;
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
		if (bFenceOnPlate)
		{
			// left fence
			CVector3D leftBoxDirection;
			leftBoxDirection.SetElements(-1, 0, 0);
			CVector3D leftBoxBtmPos;
			leftBoxBtmPos.SetValue(&plateBtmCtrPos);
			leftBoxBtmPos[X] -= plateRadius;
			leftBoxBtmPos[Y] += fenceHeight*0.5;
			CBdrySolidBox* pLeftFence = new CBdrySolidBox(fenceWidth, 2*plateRadius, fenceHeight,
											   0, &leftBoxBtmPos, &leftBoxDirection);
			pLeftFence->m_bdryThermalBody = false;
			m_pSimuManager->m_aryBoundaries.AppendOnePointer(pLeftFence);
			// right fence
			CVector3D rightBoxDirection;
			rightBoxDirection.SetElements(1, 0, 0);
			CVector3D rightBoxBtmPos;
			rightBoxBtmPos.SetValue(&plateBtmCtrPos);
			rightBoxBtmPos[X] += plateRadius;
			rightBoxBtmPos[Y] += fenceHeight*0.5;
			CBdrySolidBox* pRightFence = new CBdrySolidBox(fenceWidth, 2*plateRadius, fenceHeight,
											   0, &rightBoxBtmPos, &rightBoxDirection);
			pRightFence->m_bdryThermalBody = false;
			m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRightFence);
			// back fence
			CVector3D backBoxDirection;
			backBoxDirection.SetElements(0, 1, 0);
			CVector3D backBoxBtmPos;
			backBoxBtmPos.SetValue(&plateBtmCtrPos);
			backBoxBtmPos[Z] -= plateRadius + fenceWidth*0.5;
			CBdrySolidBox* pBackFence = new CBdrySolidBox(fenceHeight, 2*(plateRadius+fenceWidth),
											fenceWidth, 0, &backBoxBtmPos, &backBoxDirection);
			pBackFence->m_bdryThermalBody = false;
			m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBackFence);
		}
	}
	else
	{
		CBdrySolidRod* pPlate = new CBdrySolidRod(plateHeight, plateRadius,
												0, false, false, &ctrPos, &plateDirection);
		pPlate->m_bdryThermalBody = true;
		pPlate->m_bdryTemperature = plateTemperature;
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
	}
	// create pole supporting hot plate
	SimuValue poleRadius = 20;
	CVector3D poleTopCtrPos;
	poleTopCtrPos.SetValue(&ctrPos);
	poleTopCtrPos[Y] -= plateHeight;
	CBdrySolidRod* pRod = new CBdrySolidRod(plateTopToFloor-plateHeight, poleRadius,
										0, false, false, &poleTopCtrPos, &plateDirection);
	pRod->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= plateTopToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidcube() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	bool bRodEdge = false;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(0, 0, 1); cubeDirection.Normalize();
	CVector3D plateDirection; plateDirection.SetElements(0, 1, 0); plateDirection.Normalize();
	CSimuManager::m_maxTemperature = 500; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_airPressure = -500;
	CSimuManager::m_onlyGravityTime = 0.95;
	SimuValue distToPlate = 50;
	SimuValue fenceHeight = 150;
	SimuValue fenceWidth = 30;
	SimuValue plateTopToFloor = 240;
	SimuValue plateHeight = 40;

	int cubeLayers[3] = {20, 14, 10};
	CSimuManager::m_minShearModulus = 5*pow(10.0f, 3);
	CSimuManager::m_maxShearModulus = pow(10.0f, 5.0f);
	CSimuManager::m_heatConductivity = 2000;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_minTemperature = 300; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	SimuValue plateFriction = 0;
	SimuValue sideSpace = 5;
	SimuValue backSpace = 5;
	SimuValue frontSpace = 5;
	CVector3D backDirection; 
	backDirection.SetElements(0, 0, 1); backDirection.Normalize();

	CSimuManager::m_isoDensityRatio = 0.4;
	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);
	pFluid->m_deiName = "fluid_cube";
	pFluid->m_pfSimuManager = m_pSimuManager;

	CVector3D cubeBtmCtrPos;
	cubeBtmCtrPos.SetValue(&ctrPos);
	cubeBtmCtrPos[Y] += distToPlate+0.5*(cubeLayers[Y]-1)*CSimuManager::m_prtlDistance;
	cubeBtmCtrPos[Z] -= 0.5*(cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	CBoxOnXYPlane::CreatePrtlsInBox(pFluid, &cubeBtmCtrPos, &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
	pFluid->PreparePrtlEvolution();

	// create hot plate
	SimuValue plateLength = 2*sideSpace+(cubeLayers[X]-1)*CSimuManager::m_prtlDistance;
	SimuValue plateWidth = frontSpace+backSpace+(cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	CVector3D plateBtmCtrPos;
	plateBtmCtrPos.SetValue(&ctrPos);
	plateBtmCtrPos[Y] -= plateHeight;
	plateBtmCtrPos[Z] += 0.5*(cubeLayers[Z]-1)*CSimuManager::m_prtlDistance + frontSpace
						- 0.5*plateWidth;
	CBdrySolidBox* pPlate = new CBdrySolidBox(plateHeight, plateLength, plateWidth,
												0, &plateBtmCtrPos, &plateDirection);
	pPlate->m_bdryFrictionRate = plateFriction;
	pPlate->m_bdryThermalBody = true;
	pPlate->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
	if (bRodEdge)
	{
		SimuValue rodLen = plateLength + fenceWidth*2;
		SimuValue rodRadius = plateHeight*0.5;
		CVector3D rodDirection; rodDirection.SetElements(1, 0, 0);
		CVector3D rodTopCtrPos;
		rodTopCtrPos.SetValue(&plateBtmCtrPos);
		rodTopCtrPos[X] += rodLen*0.5;
		rodTopCtrPos[Y] += rodRadius;
		rodTopCtrPos[Z] += plateWidth*0.5;
		CBdrySolidRod* pRodEdge = new CBdrySolidRod(rodLen, rodRadius, 0, false, false,
													&rodTopCtrPos, &rodDirection);
		pRodEdge->m_bdryThermalBody = false;
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRodEdge);
	}
	// back fence
	const SimuValue extraHeight = plateHeight;
	SimuValue slopeRatio = backDirection[Y]/backDirection[Z];
	SimuValue backFenceHeight = fenceHeight + extraHeight;
	SimuValue backFenceLength = backFenceHeight*slopeRatio;
	CVector3D backBoxBtmPos;
	backBoxBtmPos.SetValue(&plateBtmCtrPos);
	backBoxBtmPos[Y] += plateHeight + 0.5*backFenceHeight - extraHeight;
	backBoxBtmPos[Z] -= plateWidth*0.5 + 0.5*backFenceLength - extraHeight*slopeRatio;
	backBoxBtmPos.AddedBy(&backDirection, -plateHeight);
	CBdrySolidBox* pBackFence = new CBdrySolidBox(plateHeight, plateLength,
												sqrt(pow(backFenceHeight, 2)+pow(backFenceLength, 2)),
												0, &backBoxBtmPos, &backDirection);
	pBackFence->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBackFence);
	// left fence
	CVector3D leftBoxBtmPos;
	leftBoxBtmPos.SetValue(&plateBtmCtrPos);
	leftBoxBtmPos[X] -= (plateLength + fenceWidth)*0.5;
	leftBoxBtmPos[Z] -= 0.5*(plateWidth+plateHeight+fenceHeight*slopeRatio) - 0.5*plateWidth;
	CBdrySolidBox* pLeftFence = new CBdrySolidBox(fenceHeight + plateHeight, fenceWidth,
												plateWidth+plateHeight+fenceHeight*slopeRatio,
												0, &leftBoxBtmPos, &plateDirection);
	pLeftFence->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pLeftFence);
	// right fence
	CVector3D rightBoxBtmPos;
	rightBoxBtmPos.SetValue(&plateBtmCtrPos);
	rightBoxBtmPos[X] += (plateLength + fenceWidth)*0.5;
	rightBoxBtmPos[Z] -= 0.5*(plateWidth+plateHeight+fenceHeight*slopeRatio) - 0.5*plateWidth;
	CBdrySolidBox* pRightFence = new CBdrySolidBox(fenceHeight + plateHeight, fenceWidth, 
												plateWidth+plateHeight+fenceHeight*slopeRatio,
												0, &rightBoxBtmPos, &plateDirection);
	pRightFence->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRightFence);

	// create pole supporting hot plate
	SimuValue poleRadius = 20;
	CVector3D poleTopCtrPos;
	poleTopCtrPos.SetValue(&plateBtmCtrPos);
	CBdrySolidRod* pRod = new CBdrySolidRod(plateTopToFloor-plateHeight, poleRadius,
										0, false, false, &poleTopCtrPos, &plateDirection);
	pRod->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= plateTopToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnTestConvexhull() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 100;
	SimuValue radius = 50;

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	do
	{
		unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
		srand(randomSeed);

		CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		CSimuManager::DRAW_TRIANGLE_NORMALS = true;
		TGeomElemArray<CPoint3DOnEdge> points(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
		TGeomElemArray<CTriangle3DIntNorm> triangles(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
		TGeomElemArray<CEdge3DOnTriangle> edges(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
		CConvexHull::GetDistinctPointFromFluidPrtl(ngbrPrtls, points, CConvexHull::m_chMinPointDistSqr);
		CConvexHull::AddOnePoint3DIntPos(ctrPrtl.m_vpPos, points);
		CConvexHull::m_chRunDebugVersion = true;
		CConvexHull::m_chPause[5] = true;
		CConvexHull::RobustConvexHullGeneration(points, triangles, edges);
		CConvexHull::m_chRunDebugVersion = false;
		CConvexHull::m_chPause[5] = false;

		ctrPrtl.m_vpPos->SetElements(0, 245, 0);
		SimuValue minDist = CConvexHull::GetMinDistanceToConvexHullFromInsidePos(
											ctrPrtl.m_vpPos, triangles);

	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestConvexhull()",
									"Test again or not?"));
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestDistancetoconvexhull() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 100;
	SimuValue radius = 50;

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	do
	{
		unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
		srand(randomSeed);

		CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		CPoint3D testPoint;
		CSimuUtility::CreateRandomPos(radius*2, &testPoint.m_p3dPos);
		testPoint.m_p3dPos.AddedBy(ctrPrtl.m_vpPos);
		CConvexHull::m_chRunDebugVersion = true;
		CConvexHull::m_chPause[7] = true;
		CVector3D srfcNormal;
		SimuValue dist = CConvexHull::DistanceFromExtraPointToConvexHull(&testPoint,
																		ngbrPrtls,
																		&srfcNormal);
		CConvexHull::m_chRunDebugVersion = false;
		CConvexHull::m_chPause[7] = false;
	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestDistancetoconvexhull()",
									"Test again or not?"));
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestDelaunaytetrahedron() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 500;
	SimuValue radius = 50;
	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	CDelaunayTetrahedron::m_dtrPause[6] = true;
	do
	{
		unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
		srand(randomSeed);

		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		TGeomElemArray<CPoint3DOnTetra> dataPoints(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
		TGeomElemArray<CTetraDelaunay> tetrahedra(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);
		tetrahedra.m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
		CPoint3DOnTetra* ctrPoint;
		CVector3D upsamplePos;
		bool bFound = CDelaunayTetrahedron::SearchForUpsamplePos(&ctrPrtl, ngbrPrtls,
														dataPoints, tetrahedra, ctrPoint,
														0.75*radius/2, 1.25*radius/2,
														&upsamplePos);
		while(bFound)
		{
			bFound = CDelaunayTetrahedron::ResumeSearchUpsamplePos(dataPoints,
															tetrahedra, ctrPoint,
															0.75*radius/2, 1.25*radius/2,
															&upsamplePos);
		}
	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestDelaunaytetrahedron()",
									"Test again or not?"));
	CDelaunayTetrahedron::m_dtrPause[6] = false;
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestUpsampledelaunaytetrahedron() 
{
	// TODO: Add your command handler code here
	
	unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
	srand(randomSeed);

	const int numTotalPrtls = 50;
	SimuValue radius = 50;
	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
	TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
	CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

	TGeomElemArray<CPoint3DOnTetra> dataPoints(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTetraDelaunay> tetrahedra(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);
	tetrahedra.m_geaColor.SetValue(CSimuManager::COLOR_WHITE);
	CPoint3DOnTetra* ctrPoint;
	CVector3D upsamplePos;
	bool bFound = true;
	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	CDelaunayTetrahedron::m_dtrPause[2] = true;
	CDelaunayTetrahedron::m_dtrPause[6] = true;
	while(bFound)
	{
		bFound = CDelaunayTetrahedron::SearchForUpsamplePos(&ctrPrtl, ngbrPrtls,
													dataPoints, tetrahedra, ctrPoint,
													0.75*radius/2, 1.25*radius/2,
													&upsamplePos);
		bool bUpsampled = false;
		while(bFound && !bUpsampled)
		{
			if (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
											"CSimuModeler::OnTestUpsampledelaunaytetrahedron()",
											"Upsample or not?"))
			{
				CFluidPrtl* newNgbrPrtl = ngbrPrtls.CreateOneElement();
				newNgbrPrtl->m_vpPos->SetValue(&upsamplePos);
				bUpsampled = true;
			}
			else
			{
				bFound = CDelaunayTetrahedron::ResumeSearchUpsamplePos(dataPoints,
																tetrahedra, ctrPoint,
																0.75*radius/2, 1.25*radius/2,
																&upsamplePos);
				bUpsampled = false;
			}
		}
	}
	CDelaunayTetrahedron::m_dtrPause[2] = false;
	CDelaunayTetrahedron::m_dtrPause[6] = false;
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestDelaunaytessellation() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 100;
	SimuValue radius = 20;
	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
	TGeomElemArray<CPoint3DOnTetra> dataPoints(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTetraDelaunay> tetrahedra(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);
	tetrahedra.m_geaColor.SetValue(CSimuManager::COLOR_WHITE);

	unsigned randomSeed = 1089841186; //(unsigned)time(NULL); //
	srand(randomSeed);

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	int iterations = 0;
	do
	{
		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		dataPoints.ResetArraySize(0);
		tetrahedra.ResetArraySize(0);
		CPoint3DOnTetra* ctrPoint;
		CPoint3DOnTetra* boundingPs[4];
		CVector3D upsamplePos;
//		CDelaunayTetrahedron::m_dtrPause[17] = true;
		long start = clock();
		CDelaunayTetrahedron::DelaunayTessellation(&ctrPrtl, ngbrPrtls,
												   CSimuManager::m_prtlDistance,
												   dataPoints, tetrahedra,
												   ctrPoint, boundingPs);
		long finish = clock();
		SimuValue timeTessellation = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		CDelaunayTetrahedron::m_dtrPause[17] = false;
		iterations++;
		sprintf(CSimuMsg::GetEmptyBuf(),
				"prtl=%d, tetra=%d, time=%f s. %stest again or not?",
				numTotalPrtls, tetrahedra.m_paNum, timeTessellation, RETURN_STRING);

	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestDelaunaytetrahedron()",
									CSimuMsg::GetCheckedBuf()));
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestPartialdelaunaytessellation() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 100;
	SimuValue radius = 20;
	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
	TGeomElemArray<CPoint3DOnTetra> dataPoints(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTetraDelaunay> tetrahedra(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);
	tetrahedra.m_geaColor.SetValue(CSimuManager::COLOR_WHITE);

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	do
	{
		unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
		srand(randomSeed);

		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		dataPoints.ResetArraySize(0);
		tetrahedra.ResetArraySize(0);
		CPoint3DOnTetra* ctrPoint;
		CPoint3DOnTetra* boundingPs[4];
		CVector3D upsamplePos;

		bool bOldFlag = CDelaunayTetrahedron::m_dtrOnlyOnCtrPoint;
		CDelaunayTetrahedron::m_dtrOnlyOnCtrPoint = false;
		long start0 = clock();
		CDelaunayTetrahedron::DelaunayTessellation(&ctrPrtl, ngbrPrtls,
												   CSimuManager::m_prtlDistance,
												   dataPoints, tetrahedra,
												   ctrPoint, boundingPs);
		long finish0 = clock();
		SimuValue timeCompleteTessellation = ((SimuValue)(finish0-start0))/CLOCKS_PER_SEC;

		dataPoints.ResetArraySize(0);
		tetrahedra.ResetArraySize(0);
		CDelaunayTetrahedron::m_dtrOnlyOnCtrPoint = true;
		long start1 = clock();
		CDelaunayTetrahedron::DelaunayTessellation(&ctrPrtl, ngbrPrtls,
												   CSimuManager::m_prtlDistance,
												   dataPoints, tetrahedra,
												   ctrPoint, boundingPs);
		long finish1 = clock();
		CDelaunayTetrahedron::m_dtrOnlyOnCtrPoint = bOldFlag;
		SimuValue timePartialTessellation = ((SimuValue)(finish1-start1))/CLOCKS_PER_SEC;
		sprintf(CSimuMsg::GetEmptyBuf(), "complete = %f sec. and partial = %f sec.",
									timeCompleteTessellation, timePartialTessellation);
		CSimuMsg::PauseMessage(CSimuMsg::GetCheckedBuf());

	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestDelaunaytetrahedron()",
									"Test again or not?"));
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestUpsampledelaunaytessellation() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 50;
	SimuValue radius = 50;
	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
	TGeomElemArray<CPoint3DOnTetra> dataPoints(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTetraDelaunay> tetrahedra(true, SIMU_PARA_NGBR_TETRA_ALLOC_SIZE);
	tetrahedra.m_geaColor.SetValue(CSimuManager::COLOR_WHITE);

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	do
	{
		unsigned randomSeed = (unsigned)time(NULL); //1089841186; //
		srand(randomSeed);

		TGeomElemArray<CFluidPrtl> ngbrPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, ngbrPrtls);

		dataPoints.ResetArraySize(0);
		tetrahedra.ResetArraySize(0);
		CPoint3DOnTetra* ctrPoint;
		CPoint3DOnTetra* boundingPs[4];
		CPoint3DOnTetra upsamplePoint;
		upsamplePoint.SetPoint3DColor(CDelaunayTetrahedron::m_dtrTestPColor);
		CDelaunayTetrahedron::m_dtrPause[17] = true;
		CDelaunayTetrahedron::DelaunayTessellation(&ctrPrtl, ngbrPrtls,
												   CSimuManager::m_prtlDistance,
												   dataPoints, tetrahedra,
												   ctrPoint, boundingPs);
		CDelaunayTetrahedron::m_dtrPause[17] = false;
		CDelaunayTetrahedron::m_dtrCtrPoint = ctrPoint;
		CDelaunayTetrahedron::m_dtrDataPoints = &dataPoints;
		CDelaunayTetrahedron::m_dtrTetras = &tetrahedra;
		CTetraDelaunay* pTetra = CDelaunayTetrahedron::SearchTetraToUpsample(
													ctrPoint, 0.75*radius/2, 1.25*radius/2);
		while (pTetra != NULL)
		{
			upsamplePoint.m_p3dPos.SetValue(&pTetra->m_ttrdCircumCtrPos);
			CDelaunayTetrahedron::m_dtrNewDataPoint = &upsamplePoint;
			CDelaunayTetrahedron::m_dtrCircumCenter = &upsamplePoint;
			CDelaunayTetrahedron::m_dtrCircumRSqr = pTetra->m_ttrdCircumRSqr;
			pTetra->SetTetraPointColor(CDelaunayTetrahedron::m_dtrTetraPColor);
			int questionAnswer = m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestUpsampledelaunaytessellation()",
									"Upsample or not?");
			pTetra->SetTetraPointColor(CDelaunayTetrahedron::m_dtrNgbrPColor);
			ctrPoint->SetPoint3DColor(CDelaunayTetrahedron::m_dtrCtrColor);
			if (questionAnswer == CSimuQuestionDlg::ANSWER_YES)
			{
				CPoint3DOnTetra* newPoint = dataPoints.CreateOneElement();
				newPoint->m_p3dPos.SetValue(&pTetra->m_ttrdCircumCtrPos);
				CDelaunayTetrahedron::m_dtrPause[16] = true;
				CDelaunayTetrahedron::AddPointToDelaunayTessellation(newPoint, dataPoints,
																	 tetrahedra, ctrPoint);
				CDelaunayTetrahedron::m_dtrPause[16] = false;
			}
			else
				pTetra->m_ttrdNoUpsample = true;
			CDelaunayTetrahedron::m_dtrNewDataPoint = NULL;
			CDelaunayTetrahedron::m_dtrCircumCenter = NULL;

			pTetra = CDelaunayTetrahedron::SearchTetraToUpsample(
													ctrPoint, 0.75*radius/2, 1.25*radius/2);
		}
		CDelaunayTetrahedron::m_dtrCtrPoint = NULL;
		CDelaunayTetrahedron::m_dtrDataPoints = NULL;
		CDelaunayTetrahedron::m_dtrTetras = NULL;
	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestUpsampledelaunaytessellation()",
									"Test again or not?"));
	delete m_dlgQuestion;
}

void CSimuModeler::OnTestConvexhullongrid() 
{
	// TODO: Add your command handler code here
	
	m_pSimuManager->CreateCurrentPrtlFluid();

	int cubeLayers[3] = {6, 6, 6};
	CVector3D cubeCtrPos;
	cubeCtrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection;
	cubeDirection.SetElements(1, 1, 1);
	cubeDirection.Normalize();
	CVector3D cubeBtmPos;
	cubeBtmPos.SetValue(&cubeCtrPos);
	cubeBtmPos.AddedBy(&cubeDirection, -(cubeLayers[Z]-1)*CSimuManager::m_prtlDistance/2);

	CBoxOnXYPlane::CreatePrtlsInBox(m_pSimuManager->m_curPrtlFluid,
									&cubeBtmPos, &cubeDirection, 0, 0,
									cubeLayers, CSimuManager::m_prtlDistance);

	CSimuManager::DRAW_TRIANGLE_NORMALS = true;
	TGeomElemArray<CPoint3DOnEdge> points(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CTriangle3DIntNorm> triangles(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	TGeomElemArray<CEdge3DOnTriangle> edges(true, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	CConvexHull::GetDistinctPointFromFluidPrtl(m_pSimuManager->m_curPrtlFluid->m_pfPrtls,
												points, CConvexHull::m_chMinPointDistSqr);
	CConvexHull::m_chRunDebugVersion = true;
	CConvexHull::m_chPause[6] = true;
	CConvexHull::RobustConvexHullGeneration(points, triangles, edges);
	CConvexHull::m_chRunDebugVersion = false;
	CConvexHull::m_chPause[6] = false;
	CSimuManager::m_prtlDistance = 10;
}


void CSimuModeler::OnTestDelaunaytetrahedronobject() 
{
	// TODO: Add your command handler code here
	
	const int numTotalPrtls = 10000;
	SimuValue radius = 100;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);

	unsigned randomSeed = 1089841186; //(unsigned)time(NULL); //
	srand(randomSeed);

	CDelaunayTetrahedron* testDTObject;
	testDTObject = new CDelaunayTetrahedron();

	CSimuQuestionDlg* m_dlgQuestion = new CSimuQuestionDlg();
	m_dlgQuestion->Create(CSimuQuestionDlg::IDD, NULL);
	int iterations = 0;
	do
	{
		TGeomElemArray<CFluidPrtl> aryPrtls(true, numTotalPrtls);
		CFluidPrtl::CreateRandomPrtls(&ctrPos, radius, numTotalPrtls, aryPrtls);

//		CDelaunayTetrahedron::m_dtrPause[17] = true;
		CDelaunayTetrahedron::m_dtrNumDeletedTetra		= 0;
		CDelaunayTetrahedron::m_dtrPreparationClocks	= 0;
		CDelaunayTetrahedron::m_dtrAddPointsClocks		= 0;
		CDelaunayTetrahedron::m_dtrSearchClocks			= 0;
		CDelaunayTetrahedron::m_dtrAdjacentSearchClocks	= 0;
		CDelaunayTetrahedron::m_dtrDeleteTetraClocks	= 0;
		CDelaunayTetrahedron::m_dtrCreateTetraClocks	= 0;
		CDelaunayTetrahedron::m_dtrSetupNgbrhoodClocks	= 0;
		CDelaunayTetrahedron::m_dtrDeRegisterTetraClocks= 0;
		CDelaunayTetrahedron::m_dtrRegisterTetraClocks	= 0;
		long start = clock();
		testDTObject->ConstructDelaunayTetrahedra(aryPrtls);
		testDTObject->m_dtrTetraSearchGrid.DeleteTetraArraysInCells();
		testDTObject->m_dtrPoints.ResetArraySize(0);
		testDTObject->m_dtrTetrahedra.ResetArraySize(0);
		long finish = clock();
		SimuValue timeTessellation = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		CDelaunayTetrahedron::m_dtrPause[17] = false;
		iterations++;
		int sumAddPointClocks = CDelaunayTetrahedron::m_dtrSearchClocks
								+CDelaunayTetrahedron::m_dtrAdjacentSearchClocks
								+CDelaunayTetrahedron::m_dtrDeleteTetraClocks
								+CDelaunayTetrahedron::m_dtrCreateTetraClocks
								+CDelaunayTetrahedron::m_dtrDeRegisterTetraClocks
								+CDelaunayTetrahedron::m_dtrRegisterTetraClocks;
		sprintf(CSimuMsg::GetEmptyBuf(),
		"n=%d, tn=%d, t=%f, pc=%d, a=%d, s=%d, as=%d, d=%d, c=%d, sn=%d, dr=%d, r=%d, sum=%d, %stest again or not?",
			numTotalPrtls, CDelaunayTetrahedron::m_dtrNumDeletedTetra,
			timeTessellation,
			CDelaunayTetrahedron::m_dtrPreparationClocks,
			CDelaunayTetrahedron::m_dtrAddPointsClocks,
			CDelaunayTetrahedron::m_dtrSearchClocks,
			CDelaunayTetrahedron::m_dtrAdjacentSearchClocks,
			CDelaunayTetrahedron::m_dtrDeleteTetraClocks,
			CDelaunayTetrahedron::m_dtrCreateTetraClocks,
			CDelaunayTetrahedron::m_dtrSetupNgbrhoodClocks,
			CDelaunayTetrahedron::m_dtrDeRegisterTetraClocks,
			CDelaunayTetrahedron::m_dtrRegisterTetraClocks,
			sumAddPointClocks,
			RETURN_STRING);

		CDelaunayTetrahedron::m_dtrDataPoints = &testDTObject->m_dtrPoints;
		CDelaunayTetrahedron::m_dtrTetras = &testDTObject->m_dtrTetrahedra;

	} while (CSimuQuestionDlg::ANSWER_YES == m_dlgQuestion->ShowQuestion(
									"CSimuModeler::OnTestDelaunaytetrahedronobject()",
									CSimuMsg::GetCheckedBuf()));
	CDelaunayTetrahedron::m_dtrPause[17] = false;
	delete m_dlgQuestion;
	delete testDTObject;
}

void CSimuModeler::OnFluidmodelNozzleinbox() 
{
	// TODO: Add your command handler code here
	
	bool bCreateBox = false;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D direction; direction.SetElements(1, 0, 0); direction.Normalize();
	CSimuManager::m_maxTemperature = 800;
	CSimuManager::m_minShearModulus = pow(10.0f, 1);
	SimuValue fluidIndent = 20;

	CSimuManager::m_eFluidModel = CSimuManager::SPH; // UPSAMPLE; // 
	CSimuManager::m_maxShearModulus	= pow(10.0f, 5);
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_airPressure = -100;
	CSimuManager::m_prtlPosJitter = 0;
	SimuValue nozzleLength = 200;
	SimuValue nozzleRadius = 60;
	SimuValue nozzleSpeed = 20;
	SimuValue distToFloor = 260;

	CPrtlFluid* fluidCylinder = CSimuManager::CreatePrtlFluid();
	fluidCylinder->m_deiName = "fluid_nozzle";
	fluidCylinder->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCylinder);

	// create particles
	CVector3D topCtrPos;
	topCtrPos.SetValue(&ctrPos);
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(nozzleLength, nozzleRadius,
															 0, false, false,
															 &topCtrPos, &direction);
	pCylinder->CreateFluidPrtlsInCylinder(fluidCylinder);
	delete pCylinder;

	CVector3D velocity;
	velocity.SetValue(&direction);
	velocity.ScaledBy(nozzleSpeed);
	fluidCylinder->PreparePrtlEvolution();
	fluidCylinder->InitializePrtlVelocities(velocity);

	// create nozzle
	topCtrPos.AddedBy(&direction, fluidIndent);
	nozzleLength += fluidIndent;
	SimuValue emptyLen = 0*CSimuManager::m_prtlDistance;
	nozzleLength += emptyLen;
	CSimuFluidNozzle* nozzle = new CSimuFluidNozzle(nozzleLength, nozzleRadius, 
													&topCtrPos, &direction);
	nozzle->m_deiName = "nozzle";
	nozzle->SetBdryParams(1);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(nozzle);
	// create torus
	SimuValue innerRadius = nozzleRadius;
	SimuValue outerRadius = innerRadius + 2*CSimuManager::m_prtlDistance;
	CBdrySolidTorus* pTorus = new CBdrySolidTorus(innerRadius, outerRadius,
													&topCtrPos, &direction);
	pTorus->m_deiName = "nozzle_outlet";
	pTorus->SetBdryParams(1);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pTorus);

	// create box
	CVector3D boxSideLen; boxSideLen.SetElements(250, 200, 250);
	if (bCreateBox)
	{
		CVector3D boxLBCCornor;
		boxLBCCornor.SetValue(&ctrPos);
		boxLBCCornor.AddedBy(&boxSideLen, -0.5);
		CVector3D boxRUFCornor;
		boxRUFCornor.SetValue(&ctrPos);
		boxRUFCornor.AddedBy(&boxSideLen, 0.5);
		CSimuBoundaryBox* pBox = new CSimuBoundaryBox(&boxLBCCornor, &boxRUFCornor);
		pBox->m_boxOpenRoof = true;
		pBox->m_deiName = "box_boundary";
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBox);
	}
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distToFloor + nozzleRadius;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRigidmodelBall() 
{
	// TODO: Add your command handler code here

	CVector3D slopeDirection; slopeDirection.SetElements(1, 2, 0); slopeDirection.Normalize();
	CVector3D ballDirection; ballDirection.SetValue(&slopeDirection);
	CVector3D ballCtrPos; ballCtrPos.SetElements(0, 0, 0);
	SimuValue ballRadius = 20;
	SimuValue ovalRs[3] = {ballRadius, ballRadius, ballRadius*2};
	SimuValue distToSlope = 0;

	CSimuManager::m_frictionRate = 0;
	CSimuManager::GRAVITY->SetElements(0, -1000, 0);
	CSimuManager::NON_NEWTONIAN_FLUID = false;
	CSimuManager::m_eFluidModel = CSimuManager::SPRING; // SPH; // 
	CSimuManager::m_springConstant	= pow(10.0f, 7);

	TGeomElemArray<CPrtlTriangleMesh> arySrfcMesh;
	// create ball
	CPrtlTriangleMesh* ballSrfcMesh = new CPrtlTriangleMesh();
	COvalAlong3DAxes::CreateTriangularMeshOnOval(ballSrfcMesh->m_ptmTris,
												 ballSrfcMesh->m_ptmTriPrtls,
												 ballCtrPos, ballDirection, ovalRs);
	arySrfcMesh.AppendOnePointer(ballSrfcMesh);

	m_pSimuManager->CreateCurrentPrtlFluid();
	CPrtlFluidSpring* pRigidBall = (CPrtlFluidSpring*)m_pSimuManager->m_curPrtlFluid;
	pRigidBall->m_deiName = "rigid_ball";
	pRigidBall->m_pfsPrtlInSrfcMesh.CreatePrtlInSrfcMesh(arySrfcMesh);
	pRigidBall->m_pfsMeshPrtls.TakeOverPointersFromArray(
									pRigidBall->m_pfsPrtlInSrfcMesh.m_pismGridPrtls);
	// take srfc elements from ball
	pRigidBall->m_pfsMeshPrtls.TakeOverPointersFromArray(ballSrfcMesh->m_ptmTriPrtls);
	pRigidBall->m_pfsSrfcTris.TakeOverPointersFromArray(ballSrfcMesh->m_ptmTris);

	pRigidBall->PreparePrtlEvolution();

	delete ballSrfcMesh;

	// create square slope
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ballCtrPos);
	squareCtrPos.AddedBy(&slopeDirection, -(ovalRs[Z] + distToSlope));
	SimuValue squareSideLen = 1000;
	CBdrySquareSlope* bdrySquareSlope = new CBdrySquareSlope(&slopeDirection,
															 &squareCtrPos,
															 squareSideLen, 0);
	pRigidBall->AddBoundary(bdrySquareSlope);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRigidmodelCylinder() 
{
	// TODO: Add your command handler code here
	
	CVector3D cylCtrPos; cylCtrPos.SetElements(0, 0, 0);
	CVector3D cylDirection; cylDirection.SetElements(0, 0, 1); cylDirection.Normalize();
	SimuValue distToSlope = 0;

	CSimuManager::m_frictionRate = 0;
	CSimuManager::GRAVITY->SetElements(0, -1000, 0);
	CSimuManager::m_eFluidModel = CSimuManager::SPRING; // SPH; // 
	if (CSimuManager::m_eFluidModel = CSimuManager::SPRING)
	{
		CSimuManager::NON_NEWTONIAN_FLUID = false;
		CSimuManager::m_springConstant	= pow(10.0f, 7);
	}
	else
	{
		CSimuManager::m_shearModulus	= pow(10.0f, 7);
		CSimuManager::m_relaxationTime	= pow(10.0f, 0);
	}
	SimuValue cylinderLen = 100;
	SimuValue cylinderRadius = 20;

	m_pSimuManager->CreateCurrentPrtlFluid();
	CPrtlFluid* pRigidCylinder = m_pSimuManager->m_curPrtlFluid;
	pRigidCylinder->m_deiName = "rigid_cylinder";

	// create cylinder
	CVector3D topCtrPos;
	topCtrPos.SetValue(&cylCtrPos);
	topCtrPos.AddedBy(&cylDirection, cylinderLen/2);
	SimuValue cylinderCapHeight = cylinderRadius;
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(cylinderLen, cylinderRadius,
															 0, false, false,
															 &topCtrPos, &cylDirection);
	pCylinder->CreateFluidPrtlsInCylinder(pRigidCylinder);
	// create square slope
	SimuValue squareSideLen = 1000;
	CVector3D slpDirection; slpDirection.SetElements(1, 1, 0); slpDirection.Normalize();
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&cylCtrPos);
	squareCtrPos.AddedBy(&slpDirection, -(cylinderRadius + distToSlope));
	CBdrySquareSlope* bdrySquareSlope = new CBdrySquareSlope(&slpDirection,
															 &squareCtrPos,
															 squareSideLen, 0);
	pRigidCylinder->AddBoundary(bdrySquareSlope);

	pRigidCylinder->PreparePrtlEvolution();

	delete pCylinder;

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRigidmodelCubeonslope() 
{
	// TODO: Add your command handler code here
	
	CVector3D cubeBtmCtrPos; cubeBtmCtrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(2, 1, 0);
	int cubeLayer = 6;
	int cubeLayers[3] = {cubeLayer, cubeLayer, cubeLayer*2};
	SimuValue angleAboutZ = atan(cubeDirection[Y]/cubeDirection[X])*180/PI;
	SimuValue distToSlop = 0;

	CSimuManager::m_eFluidModel = CSimuManager::SPRING; // SPH; // 
	CSimuManager::GRAVITY->SetElements(0, -1000, 0);
	CSimuManager::m_frictionRate	= 0;
	CSimuManager::NON_NEWTONIAN_FLUID = false;
	CSimuManager::m_springConstant	= pow(10.0f, 7);

	m_pSimuManager->CreateCurrentPrtlFluid();

	m_pSimuManager->m_curPrtlFluid->m_deiName = "rigid_cube";
	CBoxOnXYPlane::CreatePrtlsInBox(m_pSimuManager->m_curPrtlFluid,
									&cubeBtmCtrPos, &cubeDirection, angleAboutZ, 0,
									cubeLayers, CSimuManager::m_prtlDistance);
	// create square slope
	SimuValue squareSideLen = 1000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&cubeBtmCtrPos);
	squareCtrPos.AddedBy(&cubeDirection, -distToSlop);
	CBdrySquareSlope* bdrySquareSlope = new CBdrySquareSlope(&cubeDirection,
															 &squareCtrPos,
															 squareSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(bdrySquareSlope);

	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidverticalstretching() 
{
	// TODO: Add your command handler code here
	
	CVector3D fluidCtrPos; fluidCtrPos.SetElements(0, 0, 0);
	CVector3D direction; direction.SetElements(0, 1, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	SimuValue stretchEndLength = 20;
	CSimuManager::NOT_COUNT_FIXED_PRTLS	= false;
	CSimuManager::m_rotationFactor = 1;

	unsigned randomSeed = 1089841186; //(unsigned)time(NULL); //
	srand(randomSeed);
	bool bRandomPrtls = false;
	int numRanPrtls = 1200;

	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE;
	CSimuManager::m_shearModulus = 5*pow(10.0f, 4);
	CSimuManager::m_numStepDivPerFrame = 4;
	CSimuManager::m_airPressure = -1000;
	CSimuManager::m_framesPerResample = 0.5;
	CSimuManager::m_prtlPosJitter = 0;
	SimuValue stretchSpeed = 20;
	SimuValue fluidRadius = 50;
	SimuValue fluidLength = 100; // must be > 2*stretchEndLength

	if (CSimuManager::m_eFluidModel == CSimuManager::UPSAMPLE) {}
	else if (CSimuManager::m_eFluidModel == CSimuManager::SPH)
	{
		CSimuManager::m_bSpikyGradient = true;
		if (bRandomPrtls)
		{
			SimuValue cylVolume = PI*fluidRadius*fluidRadius*fluidLength;
			SimuValue prtlsInUnitVol = numRanPrtls/cylVolume;
			SimuValue numNgbrs = 30;
			SimuValue ngbrHoodVol = numNgbrs/prtlsInUnitVol;
			SimuValue ngbrHoodRadius = pow(ngbrHoodVol/(SIMU_CONST_4_OVER_3*PI), SIMU_CONST_ONE_THIRD);
			CSimuManager::SetPrtlDistance(ngbrHoodRadius/CSimuManager::m_smoothLengthRatio);
		}
	}
	else
		CSimuMsg::ExitWithMessage("CSimuModeler::OnFluidmodelFluidverticalstretching()",
									"m_eFluidModel is unknown.");

	bool bBtmRodEmpty = false;
	CVector3D topCtrPos;
	topCtrPos.SetValue(&fluidCtrPos);
	topCtrPos.AddedBy(&direction, fluidLength/2);

	m_pSimuManager->CreateCurrentPrtlFluid();
	CSimuManager::m_maxSrfcDetectPauseSteps = 1;
	m_pSimuManager->m_curPrtlFluid->m_pfSrfcDetectPauseSteps = 1;

	if (bBtmRodEmpty)
	{
		fluidLength -= stretchEndLength + 10;
	}
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius,
															 0, false, false,
															 &topCtrPos, &direction);
	if (CSimuManager::m_eFluidModel == CSimuManager::SPH
	 && bRandomPrtls)
	{
		pCylinder->CreateFluidPrtlsRandomlyInCylinder(m_pSimuManager->m_curPrtlFluid, numRanPrtls);
	}
	else
		pCylinder->CreateFluidPrtlsInCylinder(m_pSimuManager->m_curPrtlFluid);
	delete pCylinder;
	if (bBtmRodEmpty)
	{
		fluidLength += stretchEndLength + 10;
	}

	SimuValue halfFreeFluidLen = 0.5*(fluidLength - stretchEndLength*2);
	SimuValue rodWallThickness = 10;
	SimuValue rodEndThickness = 5;
	SimuValue rodLength = stretchEndLength + 2*rodEndThickness;
	// create btm rod
	CVector3D btmRodTopCtrPos;
	btmRodTopCtrPos.SetValue(&fluidCtrPos);
	btmRodTopCtrPos.AddedBy(&direction, -(halfFreeFluidLen - rodEndThickness));
	CBdrySolidRod* btmRod = new CBdrySolidRod(rodLength, fluidRadius + rodWallThickness,
											  0, false, false, &btmRodTopCtrPos, &direction);
	btmRod->m_bdryVel.SetValue(&direction, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(btmRod);
	// create top rod
	CVector3D topRodTopCtrPos;
	topRodTopCtrPos.SetValue(&fluidCtrPos);
	topRodTopCtrPos.AddedBy(&direction, halfFreeFluidLen - rodEndThickness);
	direction.ScaledBy(-1);
	CBdrySolidRod* topRod = new CBdrySolidRod(rodLength, fluidRadius + rodWallThickness, 
											  0, false, false, &topRodTopCtrPos, &direction);
	topRod->m_bdryVel.SetValue(&direction, -stretchSpeed);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(topRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&fluidCtrPos);
	squareCtrPos.AddedBy(&direction, halfFreeFluidLen - rodEndThickness + rodLength);
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(bdryFloor);

	m_pSimuManager->m_curPrtlFluid->FixPrtlMotionWithBoundaries();
	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "vertical_stretching";
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelPullingupfluid() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = false;
	CSimuManager::NOT_CONSERVE_HEAT = false;
	CSimuManager::NON_NEWTONIAN_FLUID = true;
	CVector3D fluidCtrPos; fluidCtrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D rodDirection; rodDirection.SetElements(0, 1, 0);
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue fixedFluidLength = 20;
	SimuValue rodWallThickness = 10;
	SimuValue rodEndThickness = 5;
	CSimuManager::NOT_COUNT_FIXED_PRTLS = false;

	CSimuManager::m_numStepDivPerFrame = 4;
	CSimuManager::m_shearModulus = pow(10.0f, 4);
	CSimuManager::m_airPressure = -1000;
	CSimuManager::m_framesPerResample = 0.5;
	SimuValue fluidLength = 70; // must be > fixedFluidLength
	SimuValue fluidRadius = 50;
	SimuValue pullingSpeed = 20; 

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "pull_up_fluid";
	pFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	CVector3D topCtrPos;
	topCtrPos.SetValue(&fluidCtrPos);
	topCtrPos.AddedBy(&rodDirection, fluidLength*0.5);
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius,
															 0, false, false,
															 &topCtrPos, &rodDirection);
	pCylinder->CreateFluidPrtlsInCylinder(pFluid);
	delete pCylinder;

	// create top rod
	CVector3D topRodTopCtrPos;
	topRodTopCtrPos.SetValue(&topCtrPos);
	topRodTopCtrPos.AddedBy(&rodDirection, rodEndThickness);
	SimuValue rodLength = fixedFluidLength + 2*rodEndThickness;
	CBdrySolidRod* topRod = new CBdrySolidRod(rodLength, fluidRadius + rodWallThickness, 
											  0, false, false, &topRodTopCtrPos, &rodDirection);
	topRod->m_bdryVel.SetValue(&rodDirection, pullingSpeed);
	topRod->m_bdryThermalBody = true;
	topRod->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(topRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&fluidCtrPos);
	squareCtrPos.AddedBy(&rodDirection, -fluidLength*0.5);
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

	pFluid->FixPrtlMotionWithBoundaries();
	pFluid->PreparePrtlEvolution();
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnTestSolidboxboundary() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::APPLY_GRAVITY = false;
	CSimuManager::m_eFluidModel = CSimuManager::SPH; // UPSAMPLE; // 
	CSimuManager::m_shearModulus	= pow(10.0f, 3);
	CSimuManager::m_relaxationTime	= pow(10.0f, -1);
	SimuValue ballSpeed = 100;
	SimuValue ballRadius = 20;
	SimuValue distToBox = ballRadius*2;

	m_pSimuManager->CreateCurrentPrtlFluid();
	CPrtlFluid* fluidBall = m_pSimuManager->m_curPrtlFluid;

	// create solid box
	SimuValue boxLen = ballRadius*1.5;
	CVector3D boxDirection; boxDirection.SetElements(0, 0, 1);
	CVector3D boxCtrPos; boxCtrPos.SetElements(0, 0, 0);
	CVector3D boxBtmCtrPos;
	boxBtmCtrPos.SetValue(&boxCtrPos);
	boxBtmCtrPos.AddedBy(&boxDirection, -boxLen*0.5);
	CBdrySolidBox* pBox = new CBdrySolidBox(boxLen, boxLen, boxLen, 0,
											&boxBtmCtrPos, &boxDirection);
	fluidBall->AddBoundary(pBox);

	CVector3D ballDirection; ballDirection.SetElements(0, 0, 1); ballDirection.Normalize();
	CVector3D ballCtrPos;
	CVector3D ballIniVel;
	CVector3D ballMovingDirection[6];
	ballMovingDirection[0].SetElements(-1, 0, 0); ballMovingDirection[0].Normalize();
	ballMovingDirection[1].SetElements( 1, 0, 0); ballMovingDirection[1].Normalize();
	ballMovingDirection[2].SetElements(0, -1, 0); ballMovingDirection[2].Normalize();
	ballMovingDirection[3].SetElements(0,  1, 0); ballMovingDirection[3].Normalize();
	ballMovingDirection[4].SetElements(0, 0, -1); ballMovingDirection[4].Normalize();
	ballMovingDirection[5].SetElements(0, 0,  1); ballMovingDirection[5].Normalize();
	for (int i=0; i < 6; i++)
	{
		ballIniVel.SetValue(&ballMovingDirection[i], ballSpeed);
		ballCtrPos.SetValue(&boxCtrPos);
		ballCtrPos.AddedBy(&ballMovingDirection[i], - distToBox - ballRadius - boxLen*0.5);
		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos, &ballDirection);
		prtlInOval->CreatePrtlInOval(fluidBall, CSimuManager::m_prtlDistance*0.5, 0, &ballIniVel);
		delete prtlInOval;
	}

	fluidBall->PreparePrtlEvolution();

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelBallshootfluidtower() 
{
	// TODO: Add your command handler code here
	
	CVector3D cubeDirection; cubeDirection.SetElements(0, 0, 1); cubeDirection.Normalize();
	CVector3D cubeCtrPos; cubeCtrPos.SetElements(0, 0, 0);
	SimuValue distToCubeX = 60;
	CSimuManager::m_onlyBdryMovingTime = 0.3;
	CVector3D ballIniVel; ballIniVel.SetElements(150, 0, 0);

	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_shearModulus = pow(10.0f, 4);
	CSimuManager::m_relaxationTime = pow(10.0f, 0);
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue ballMass = pow(10.0f, 8);
	SimuValue ballRadius = 30;
	int cubeLayers[3] = {4, 36, 6}; // layers in standard pose
	SimuValue distToCubeY = -280;

	m_pSimuManager->CreateCurrentPrtlFluid();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "ball_into_tower";

	SimuValue cubeLenX = (cubeLayers[X]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenY = (cubeLayers[Y]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenZ = (cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	// create prtls in cube
	CVector3D cubeBtmPos;
	cubeBtmPos.SetValue(&cubeCtrPos);
	cubeBtmPos.AddedBy(&cubeDirection, -cubeLenZ*0.5);
	SimuValue angleAboutZ = 0;
	CBoxOnXYPlane::CreatePrtlsInBox(m_pSimuManager->m_curPrtlFluid,
									&cubeBtmPos, &cubeDirection, angleAboutZ, 0,
									cubeLayers, CSimuManager::m_prtlDistance);
	// create rigid ball
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&cubeCtrPos);
	ballCtrPos[X] -= cubeLenX*0.5 + distToCubeX + ballRadius;
	ballCtrPos[Y] += cubeLenY*0.5 + distToCubeY + ballRadius;
	CRigidBall* pBall = new CRigidBall(ballMass, ballRadius, &ballCtrPos);
	pBall->m_rbFixed = false;
	pBall->m_bdryVel.SetValue(&ballIniVel);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pBall);
	// create floor
	SimuValue floorSideLen = 10000;
	CVector3D floorCtrPos;
	floorCtrPos.SetValue(&cubeCtrPos);
	floorCtrPos[Y] += -cubeLenY*0.5;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CBdrySquareSlope* bdrySquareSlope = new CBdrySquareSlope(&floorDirection,
															 &floorCtrPos,
															 floorSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(bdrySquareSlope);

	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();

	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRotatefluidRodclimbing() 
{
	// TODO: Add your command handler code here
	
	CVector3D rodDirection; rodDirection.SetElements(0, 1, 0); rodDirection.Normalize();
	CVector3D boxBtmCtrPos; boxBtmCtrPos.SetElements(0, -50, 0);
	CSimuManager::m_shearModulus = pow(10.0f, 4);
	CSimuManager::m_relaxationTime = pow(10.0f, -1);
	SimuValue rodRadius = 20;
	SimuValue rodLength = 200;
	CSimuManager::m_airPressure = -1000;
	SimuValue btmRodLen = 2*CSimuManager::m_prtlDistance;
	SimuValue rodThickness = 0.5*CSimuManager::m_prtlDistance;

	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_eStressModel = CSimuManager::VISCOELASTIC;
	CSimuManager::m_eRotationDerivative = COROTATIONAL; // NO_ROTATION; // CONVECTED; // 
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue fluidLength = 20;
	SimuValue fluidRadius = 110;
	SimuValue topRotateSpeed = 100;
	SimuValue btmRotateSpeed = -50;

	m_pSimuManager->CreateCurrentPrtlFluid();
	SimuValue btmRodRadius = fluidRadius + CSimuManager::m_prtlDistance;

	// create prtls
	CVector3D fluidTopPos;
	fluidTopPos.SetValue(&boxBtmCtrPos);
	fluidTopPos.AddedBy(&rodDirection, fluidLength + btmRodLen);
	CCylinderAlongZAxis* pCylinder;
	pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius, 0, false, false,
										&fluidTopPos, &rodDirection);
	pCylinder->CreateFluidPrtlsInHollowCylinder(m_pSimuManager->m_curPrtlFluid,
												rodRadius + CSimuManager::m_prtlDistance);
	delete pCylinder;
	// create prtls in rod
	CVector3D rodTopPos;
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&rodDirection, rodLength);
	pCylinder = new CCylinderAlongZAxis(rodLength, rodRadius, 0, false, false,
										&rodTopPos, &rodDirection);
	pCylinder->m_cylFixedPrtls = true;
	pCylinder->m_cylStickyTop = false;
	pCylinder->m_cylStickyBtm = false;
	pCylinder->m_cylStickyWall = true;
	pCylinder->CreateFluidPrtlsInCylinder(m_pSimuManager->m_curPrtlFluid);
	delete pCylinder;
	// create prtls in btm rod
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&rodDirection, btmRodLen - rodThickness);
	pCylinder = new CCylinderAlongZAxis(btmRodLen - 2*rodThickness, fluidRadius,
										0, false, false,
										&rodTopPos, &rodDirection);
	pCylinder->m_cylFixedPrtls = true;
	pCylinder->m_cylStickyTop = true;
	pCylinder->m_cylStickyBtm = false;
	pCylinder->m_cylStickyWall = true;
	pCylinder->CreateFluidPrtlsInHollowCylinder(m_pSimuManager->m_curPrtlFluid,
												rodRadius + CSimuManager::m_prtlDistance);
	delete pCylinder;
	// create rod
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&rodDirection, rodLength + rodThickness);
	CBdrySolidRod* pRod = new CBdrySolidRod(rodLength+2*rodThickness, rodRadius+rodThickness,
											0, false, false, &rodTopPos, &rodDirection);
	pRod->m_bdryAngVel = topRotateSpeed;
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pRod);
	// create btm rod
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&rodDirection, btmRodLen);
	pRod = new CBdrySolidRod(btmRodLen+rodThickness, btmRodRadius,
							0, false, false, &rodTopPos, &rodDirection);
	pRod->m_bdryAngVel = btmRotateSpeed;
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pRod);
	// create floor
	SimuValue floorThickness = 1;
	CVector3D floorDirection; floorDirection.SetValue(&rodDirection);
	CVector3D floorCtrPos;
	floorCtrPos.SetValue(&boxBtmCtrPos);
	floorCtrPos.AddedBy(&floorDirection, - floorThickness);
	SimuValue floorSideLen = 10000;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &floorCtrPos,
													floorSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pFloor);

	m_pSimuManager->m_curPrtlFluid->FixPrtlMotionWithBoundaries();
	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "rotate_fluid_on_box";

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRotatefluidRotaterod() 
{
	// TODO: Add your command handler code here
	
	CVector3D boxDirection; boxDirection.SetElements(0, 1, 0); boxDirection.Normalize();
	CVector3D fluidDirection; fluidDirection.SetValue(&boxDirection);
	CVector3D rodDirection; rodDirection.SetValue(&boxDirection);
	CVector3D boxBtmCtrPos; boxBtmCtrPos.SetElements(0, -50, 0);
	CSimuManager::m_shearModulus = pow(10.0f, 4);
	CSimuManager::m_relaxationTime = pow(10.0f, -1);
	SimuValue rodRadius = 20;
	SimuValue rodLength = 200;
	CSimuManager::m_airPressure = -1000;

	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_eStressModel = CSimuManager::VISCOELASTIC;
	CSimuManager::m_eRotationDerivative = NO_ROTATION; // COROTATIONAL; // CONVECTED; // 
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue fluidLength = 30;
	SimuValue fluidRadius = 110;
	SimuValue rodRotateSpeed = 100;
	SimuValue boxRotateSpeed = -100;

	SimuValue btmRodRadius = fluidRadius + CSimuManager::m_prtlDistance;
	m_pSimuManager->CreateCurrentPrtlFluid();

	// create prtls
	CVector3D fluidTopPos;
	fluidTopPos.SetValue(&boxBtmCtrPos);
	fluidTopPos.AddedBy(&fluidDirection, fluidLength);
	CCylinderAlongZAxis* pCylinder;
	pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius, 0, false, false,
										&fluidTopPos, &fluidDirection);
	pCylinder->CreateFluidPrtlsInHollowCylinder(m_pSimuManager->m_curPrtlFluid,
												rodRadius + CSimuManager::m_prtlDistance);
	delete pCylinder;
	// create prtls in rod
	CVector3D rodTopPos;
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&rodDirection, rodLength);
	pCylinder = new CCylinderAlongZAxis(rodLength, rodRadius, 0, false, false,
										&rodTopPos, &rodDirection);
	pCylinder->m_cylFixedPrtls = true;
	pCylinder->m_cylStickyTop = false;
	pCylinder->m_cylStickyBtm = true;
	pCylinder->m_cylStickyWall = true;
	pCylinder->CreateFluidPrtlsInCylinder(m_pSimuManager->m_curPrtlFluid);
	delete pCylinder;
	// create rod
	SimuValue rodThickness = 0.5*CSimuManager::m_prtlDistance;
	rodTopPos.AddedBy(&rodDirection, rodThickness);
	CBdrySolidRod* pRod = new CBdrySolidRod(rodLength+2*rodThickness, rodRadius+rodThickness,
											0, false, false, &rodTopPos, &rodDirection);
	pRod->m_bdryAngVel = rodRotateSpeed;
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pRod);
	// create btm rod
	SimuValue btmRodLen = CSimuManager::m_prtlDistance;
	rodTopPos.SetValue(&boxBtmCtrPos);
	rodTopPos.AddedBy(&boxDirection, btmRodLen + rodThickness);
	pRod = new CBdrySolidRod(btmRodLen+2*rodThickness, btmRodRadius,
							0, false, false, &rodTopPos, &rodDirection);
	pRod->m_bdryAngVel = boxRotateSpeed;
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pRod);
	// create floor
	SimuValue floorThickness = 1;
	CVector3D floorDirection; floorDirection.SetValue(&boxDirection);
	CVector3D floorCtrPos;
	floorCtrPos.SetValue(&boxBtmCtrPos);
	floorCtrPos.AddedBy(&floorDirection, - floorThickness);
	SimuValue floorSideLen = 10000;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &floorCtrPos,
													floorSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(pFloor);

	m_pSimuManager->m_curPrtlFluid->FixPrtlMotionWithBoundaries();
	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "rotate_fluid_on_box";

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRotatefluidBallonrotatedisk() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D floorCtrPos; floorCtrPos.SetElements(0, -200, 0);
	CVector3D diskDirection; diskDirection.SetElements(0, 0, 1); diskDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_minTemperature = 200; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 600; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_maxShearModulus = pow(10.0f, 5);
	SimuValue ballRadius = 60;
	SimuValue diskThickness = 200;
	SimuValue distToFloor = 200;

	CSimuManager::m_heatConductivity = 100;
	CSimuManager::m_minShearModulus = pow(10.0f, 4);
	SimuValue diskRadius = 8000;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue ballShiftX = -75;
	SimuValue rotateSpeed = 0.75;
	CSimuManager::m_onlyGravityTime = 0;
	SimuValue diskShiftY = -3800;

	CPrtlFluid* fluidBall = CSimuManager::CreatePrtlFluid();
	fluidBall->m_deiName = "fluid_ball";
	fluidBall->m_pfSimuManager = m_pSimuManager;
	fluidBall->m_pfShearModulus = CSimuManager::m_shearModulus;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBall);

	SimuValue ovalRadii[3] = {ballRadius, ballRadius, ballRadius};
	CVector3D ballDirection; ballDirection.SetElements(0, 0, 1); ballDirection.Normalize();
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&floorCtrPos);
	ballCtrPos[X] += ballShiftX;
	ballCtrPos[Y] += distToFloor + ballRadius;
	CPrtlInOval* prtlInOval = new CPrtlInOval(ovalRadii[X], ovalRadii[Y], ovalRadii[Z],
											  0, &ballCtrPos, &ballDirection);
	prtlInOval->CreatePrtlInOval(fluidBall, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;
	fluidBall->PreparePrtlEvolution();

	// create disk
	CVector3D diskTopCtrPos;
	diskTopCtrPos.SetValue(&floorCtrPos);
	diskTopCtrPos[X] += - sqrt(pow(diskRadius,2) - pow(diskShiftY,2));
	diskTopCtrPos[Y] += diskShiftY;
	diskTopCtrPos[Z] += diskThickness*0.5;
	CBdrySolidRod* pDisk = new CBdrySolidRod(diskThickness, diskRadius,
											0, false, false, &diskTopCtrPos, &diskDirection);
	pDisk->m_bdryAngVel = rotateSpeed;
	pDisk->m_bdryThermalBody = true;
	pDisk->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pDisk);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection;
	floorDirection.SetElements(0, 1, 0);
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &floorCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidballbyrandomprtls() 
{
	// TODO: Add your command handler code here
	
	SimuValue radius = 40;
	int numTotalPrtls = 3650;
	unsigned randomSeed = 1089841186; //(unsigned)time(NULL); //
	srand(randomSeed);

	CSimuManager::m_eFluidModel = CSimuManager::SPH;

	m_pSimuManager->CreateCurrentPrtlFluid();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "random_ball";

	CFluidPrtl ctrPrtl; ctrPrtl.m_vpPos->SetElements(0, 0, 0);
	TGeomElemArray<CFluidPrtl> randomPrtls(true, numTotalPrtls);
	CFluidPrtl::CreateRandomPrtls(ctrPrtl.m_vpPos, radius, numTotalPrtls, randomPrtls);
	for (int i=0; i < randomPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pSimuManager->m_curPrtlFluid->CreateOneFluidPrtl();
		m_pSimuManager->m_curPrtlFluid->m_pfPrtls.AppendOnePointer(pPrtl);
		pPrtl->m_vpPos->SetValue(randomPrtls[i]->m_vpPos);
	}
	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();

	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelTwocollidingballs() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D ballDirection; ballDirection.SetValue(&floorDirection);
	CVector3D ballIniVel[4];
	ballIniVel[2].SetElements(150, 0, 0);
	ballIniVel[3].SetElements(-150, 0, 0);
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue shearModulus[4] = {pow(10.0f, 3), pow(10.0f, 3), pow(10.0f, 3), pow(10.0f, 3)};
	SimuValue distToFloor = 150;
	SimuValue ballDistance = 200;

	CSimuManager::m_heatConductivity = 10000;
	CSimuManager::m_minTemperature = 200;
	CSimuManager::m_maxTemperature = 600;
	CSimuManager::m_minShearModulus = pow(10.0f, 3);
	CSimuManager::m_maxShearModulus = pow(10.0f, 5);
	CSimuManager::m_numStepDivPerFrame = 2;
	CSimuManager::m_subThermalSteps = 3;
	SimuValue ballRadius = 50;
	SimuValue iniSpeed = 50;
	SimuValue verticalShift = 0;
	if (iniSpeed == 50)
	{
		if (ballRadius == 50)
		{
			if (verticalShift == 0)
				CSimuManager::m_onlyGravityTime = 0.9;
			else if (verticalShift == 50)
				CSimuManager::m_onlyGravityTime = 1.1;
			else if (verticalShift == 75)
				CSimuManager::m_onlyGravityTime = 1.3;
		}
		else if (ballRadius == 60)
		{
			if (verticalShift == 0)
				CSimuManager::m_onlyGravityTime = 0.7;
		}
	}
	else if (iniSpeed == 20)
	{
		if (ballRadius == 50)
			CSimuManager::m_onlyGravityTime = 2.4;
		else if (ballRadius == 40)
			CSimuManager::m_onlyGravityTime = 2.9;
		else if (ballRadius == 30)
			CSimuManager::m_onlyGravityTime = 3.4;
	}
	ballIniVel[0].SetElements(iniSpeed, 0, 0);
	ballIniVel[1].SetElements(-iniSpeed, 0, 0);
	bool bSingleFluid = false;

	SimuValue ballRadii[4] = {ballRadius, ballRadius, ballRadius, ballRadius};

	CVector3D ballCtrPos[4];
	ballCtrPos[0].SetValue(&ctrPos);
	ballCtrPos[1].SetValue(&ctrPos);
	ballCtrPos[2].SetValue(&ctrPos);
	ballCtrPos[3].SetValue(&ctrPos);
	ballCtrPos[0][X] -= ballDistance*0.5;
	ballCtrPos[1][X] += ballDistance*0.5;
	ballCtrPos[2][X] -= ballDistance*0.5;
	ballCtrPos[3][X] += ballDistance*0.5;
	ballCtrPos[1][Y] += verticalShift;
	ballCtrPos[2][Y] += verticalShift+ballCtrPos[1][Y];
	ballCtrPos[3][Y] += verticalShift+ballCtrPos[2][Y];

	CPrtlFluid* fluidBall[4];
	CPrtlFluid* pFluid = NULL;
	if (bSingleFluid)
	{
		pFluid = CSimuManager::CreatePrtlFluid();
		pFluid->m_deiName = "ball_merge";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);
		pFluid->m_pfSimuManager = m_pSimuManager;
		pFluid->m_pfShearModulus = shearModulus[0];
	}
	char tmpName[LEN_FLUID_NAME];
	for (int i=0; i < 2; i++)
	{
		if (bSingleFluid)
		{
			CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadii[i], ballRadii[i], ballRadii[i],
													  0, &ballCtrPos[i], &ballDirection);
			prtlInOval->CreatePrtlInOval(pFluid, CSimuManager::m_prtlDistance*0.5, 0, &ballIniVel[i]);
			delete prtlInOval;
		}
		else
		{
			fluidBall[i] = CSimuManager::CreatePrtlFluid();
			sprintf(tmpName, "fluid_ball_%d", fluidBall[i]->m_pfFluidId);
			fluidBall[i]->m_deiName = tmpName;
			m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBall[i]);
			fluidBall[i]->m_pfSimuManager = m_pSimuManager;
			fluidBall[i]->m_pfShearModulus = shearModulus[i];

			CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadii[i], ballRadii[i], ballRadii[i],
													  0, &ballCtrPos[i], &ballDirection);
			prtlInOval->CreatePrtlInOval(fluidBall[i], CSimuManager::m_prtlDistance*0.5, 0);
			delete prtlInOval;
			switch (i)
			{
				case 0:
					fluidBall[0]->m_pfIniTemperature = CSimuManager::m_minTemperature;
					break;
				case 1:
					fluidBall[1]->m_pfIniTemperature = CSimuManager::m_maxTemperature;
					break;
				case 2:
					fluidBall[2]->m_pfIniTemperature = CSimuManager::m_minTemperature;
					break;
				case 3:
					fluidBall[3]->m_pfIniTemperature = CSimuManager::m_maxTemperature;
					break;
			}
			fluidBall[i]->PreparePrtlEvolution();
			fluidBall[i]->InitializePrtlVelocities(ballIniVel[i]);

			switch (i)
			{
				case 0:
//					fluidBall[0]->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
//					fluidBall[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
					break;
				case 1:
//					fluidBall[1]->m_pfColor.SetValue(CSimuManager::COLOR_RED);
//					fluidBall[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
					break;
				case 2:
					fluidBall[2]->m_pfColor.SetValue(CSimuManager::COLOR_CYAN);
					fluidBall[2]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_CYAN);
					break;
				case 3:
					fluidBall[3]->m_pfColor.SetValue(CSimuManager::COLOR_MAGENTA);
					fluidBall[3]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
					break;
			}
		}
	}
	if (bSingleFluid)
		pFluid->PreparePrtlEvolution();

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelRodhittingdisk() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(-50, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D rodDirection; rodDirection.SetElements(1, 0, 0); rodDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_eInterFluidModel = CSimuManager::DETECT_BY_CONVEX_HULL; // REPULSIVE_FORCE; // DETECT_BY_ISO_SRFC; // 
//	CSimuManager::m_interFluidTension = pow(10.0f, 6)*1.25;
	SimuValue distance = 100;
	CVector3D boardIniVel; boardIniVel.SetElements(0, 0, 0);
	SimuValue rodAirPressure = -100;
	SimuValue boardAirPressure = -100;

	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue rodPrtlMassRatio = 1;
	SimuValue rodShearModulus = 10000;
	SimuValue boardShearModulus = 1000;
	SimuValue rodLen = 100;
	SimuValue rodUpDist = 20;
	SimuValue rodRadius = 25;
	SimuValue rodOnlyGravityTime = 0.3;
	int boardLayers[3] = {10, 18, 4};
	CVector3D rodIniVel; rodIniVel.SetElements(300, 0, 0);

	SimuValue boardOnlyBdryMovingTime = rodOnlyGravityTime;
	CVector3D btmCtrPos;
	btmCtrPos.SetValue(&ctrPos);
	btmCtrPos.AddedBy(&rodDirection, distance*0.5);
	bool bSingleFluid = false;

	CVector3D rodTopCtrPos;
	rodTopCtrPos.SetValue(&ctrPos);
	rodTopCtrPos.AddedBy(&rodDirection, -distance*0.5);
	rodTopCtrPos[Y] += rodUpDist;
	CPrtlFluid* rodFluid = CSimuManager::CreatePrtlFluid();
	rodFluid->m_deiName = "fluid_rod";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(rodFluid);
	rodFluid->m_pfSimuManager = m_pSimuManager;
	rodFluid->m_pfPrtlMassRatio = rodPrtlMassRatio;
	rodFluid->m_pfAirPressure = rodAirPressure;
	rodFluid->m_pfShearModulus = rodShearModulus;
	rodFluid->m_pfOnlyGravityTime = rodOnlyGravityTime;
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(rodLen, rodRadius,
															 0, false, false,
															 &rodTopCtrPos, &rodDirection);
	pCylinder->CreateFluidPrtlsInCylinderWithVelocity(rodFluid, &rodIniVel);
	delete pCylinder;
	rodFluid->PreparePrtlEvolution();
	rodFluid->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
	rodFluid->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);

	if (bSingleFluid)
	{
		rodFluid->m_deiName = "rod_hit_board";
		CBoxOnXYPlane::CreatePrtlsInBoxWithVelocity(rodFluid,  &btmCtrPos, 
													&rodDirection, &boardIniVel, 0, 0,
													boardLayers, CSimuManager::m_prtlDistance);
		rodFluid->PreparePrtlEvolution();
	}
	else
	{
		CPrtlFluid* boardFluid = CSimuManager::CreatePrtlFluid();
		boardFluid->m_deiName = "fluid_board";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(boardFluid);
		boardFluid->m_pfSimuManager = m_pSimuManager;
		boardFluid->m_pfAirPressure = boardAirPressure;
		boardFluid->m_pfShearModulus = boardShearModulus;
		boardFluid->m_pfOnlyBdryMovingTime = boardOnlyBdryMovingTime;
		CBoxOnXYPlane::CreatePrtlsInBox(boardFluid,
										&btmCtrPos, &rodDirection, 0, 0,
										boardLayers, CSimuManager::m_prtlDistance);
		boardFluid->PreparePrtlEvolution();
		boardFluid->InitializePrtlVelocities(boardIniVel);
		boardFluid->m_pfColor.SetValue(CSimuManager::COLOR_RED);
		boardFluid->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&btmCtrPos);
	squareCtrPos[Y] -= (boardLayers[Y]-1)*CSimuManager::m_prtlDistance*0.5;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelBallhittingdisk() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D diskDirection; diskDirection.SetElements(1, 0, 0); diskDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_maxShearModulus = pow(10.0f, 6);
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue distToFloor = 100;
	SimuValue ballCoeffTemperature = 0.001;
	SimuValue speed = 150;
	SimuValue ballRadius = 40;
	SimuValue diskRadius = 100;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.2;

	CSimuManager::m_heatConductivity = 5000;
	CSimuManager::m_minShearModulus = pow(10.0f, 2);
	CSimuManager::m_subThermalSteps = 3;
	SimuValue ballMaxShearModulus = pow(10.0f, 3);
	SimuValue ballIniTemperature = 6000;
	SimuValue diskIniTemperature = 200;
	SimuValue distance = 200;
	CSimuManager::m_onlyGravityTime = 0.65;
	SimuValue diskLength = 30;
	SimuValue ballMassRatio = 5;

	CVector3D ballIniVel; ballIniVel.SetElements(speed, 0, 0);
	CVector3D diskIniVel; diskIniVel.SetElements(-speed, 0, 0);
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);
	ballCtrPos.AddedBy(&diskDirection, -(distance*0.5 + ballRadius));
	CPrtlFluid* ballFluid = CSimuManager::CreatePrtlFluid();
	ballFluid->m_deiName = "fluid_ball";
	ballFluid->m_pfSimuManager = m_pSimuManager;
	ballFluid->m_pfPrtlMassRatio = ballMassRatio;
	ballFluid->m_pfMaxShearModulus = ballMaxShearModulus;
	ballFluid->m_pfIniTemperature = ballIniTemperature;
	ballFluid->m_pfCoeffTemperature = ballCoeffTemperature;
	ballFluid->m_pfLinrTempTxtr.m_lttMinTemperature = 1000;
	ballFluid->m_pfLinrTempTxtr.m_lttMaxTemperature = ballIniTemperature;
	ballFluid->m_pfLinrTempTxtr.m_lttRatioFilter = 0;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(ballFluid);

	SimuValue bRodHitDisk = false;
	SimuValue rodLen = 100;
	SimuValue rodRadius = 25;
	if (bRodHitDisk)
	{
		CVector3D rodTopCtrPos;
		rodTopCtrPos.SetValue(&ctrPos);
		rodTopCtrPos.AddedBy(&diskDirection, -distance*0.5);
		CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(rodLen, rodRadius,
																 0, false, false,
																 &rodTopCtrPos, &diskDirection);
		pCylinder->CreateFluidPrtlsInCylinderWithVelocity(ballFluid, &ballIniVel);
		delete pCylinder;
		ballFluid->PreparePrtlEvolution();
	}
	else
	{
		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos, &diskDirection);
		prtlInOval->CreatePrtlInOval(ballFluid, CSimuManager::m_prtlDistance*0.5, 0);
		delete prtlInOval;
		ballFluid->PreparePrtlEvolution();
		ballFluid->InitializePrtlVelocities(ballIniVel);
	}

	CPrtlFluid* diskFluid = CSimuManager::CreatePrtlFluid();
	diskFluid->m_deiName = "fluid_disk";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(diskFluid);
	diskFluid->m_pfSimuManager = m_pSimuManager;
	diskFluid->m_pfIniTemperature = diskIniTemperature;
	diskFluid->m_pfLinrTempTxtr.m_lttMinTemperature = diskIniTemperature;
	diskFluid->m_pfLinrTempTxtr.m_lttMaxTemperature = ballIniTemperature;
	diskFluid->m_pfLinrTempTxtr.m_lttRatioColorG = 0;
	CVector3D diskTopCtrPos;
	diskTopCtrPos.SetValue(&ctrPos);
	diskTopCtrPos.AddedBy(&diskDirection, distance*0.5 + diskLength);
	CCylinderAlongZAxis* pDisk = new CCylinderAlongZAxis(diskLength, diskRadius,
														0, false, false,
														&diskTopCtrPos, &diskDirection);
	pDisk->CreateFluidPrtlsInCylinderWithVelocity(diskFluid, &diskIniVel);
	delete pDisk;
	diskFluid->PreparePrtlEvolution();

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distToFloor + diskRadius;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelBoardcolliding() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D boardDirection; boardDirection.SetElements(1, 0, 0); boardDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue distance = 100;
	SimuValue angleAboutZ[2] = {0, 90};

	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue shearModulus[4] = {pow(10.0f, 3), pow(10.0f, 3), pow(10.0f, 4), pow(10.0f, 3)};
	int boardLayers[3] = {7, 26, 5};
	SimuValue boardSpeed = 150;
	SimuValue distToFloor = 200;
	CSimuManager::m_onlyGravityTime = 0.3;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.5;

	CVector3D boardIniVel[2];
	boardIniVel[0].SetElements(boardSpeed, 0, 0);
	boardIniVel[1].SetElements(-boardSpeed, 0, 0);
	SimuValue boardDim[3] = {(boardLayers[0]-1)*CSimuManager::m_prtlDistance,
							 (boardLayers[1]-1)*CSimuManager::m_prtlDistance,
							 (boardLayers[2]-1)*CSimuManager::m_prtlDistance};
	CVector3D btmCtrPos[2];
	btmCtrPos[0].SetValue(&ctrPos);
	btmCtrPos[1].SetValue(&ctrPos);
	btmCtrPos[0][X] += -0.5*distance - boardDim[Z];
	btmCtrPos[1][X] += 0.5*distance;

	CPrtlFluid* fluidBoard[4];
	char tmpName[LEN_FLUID_NAME];
	for (int i=0; i < 2; i++)
	{
		fluidBoard[i] = CSimuManager::CreatePrtlFluid();
		sprintf(tmpName, "fluid_board_%d", fluidBoard[i]->m_pfFluidId);
		fluidBoard[i]->m_deiName = tmpName;
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBoard[i]);
		fluidBoard[i]->m_pfSimuManager = m_pSimuManager;
		fluidBoard[i]->m_pfShearModulus = shearModulus[i];

		CBoxOnXYPlane::CreatePrtlsInBox(fluidBoard[i],
										&btmCtrPos[i], &boardDirection, angleAboutZ[i], 0,
										boardLayers, CSimuManager::m_prtlDistance);

		fluidBoard[i]->PreparePrtlEvolution();
		fluidBoard[i]->InitializePrtlVelocities(boardIniVel[i]);

		switch (i)
		{
			case 0:
				fluidBoard[0]->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
				fluidBoard[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
				break;
			case 1:
				fluidBoard[1]->m_pfColor.SetValue(CSimuManager::COLOR_RED);
				fluidBoard[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
				break;
			case 2:
				fluidBoard[2]->m_pfColor.SetValue(CSimuManager::COLOR_CYAN);
				fluidBoard[2]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_CYAN);
				break;
			case 3:
				fluidBoard[3]->m_pfColor.SetValue(CSimuManager::COLOR_MAGENTA);
				fluidBoard[3]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
				break;
		}
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelBallhittingtower() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(0, 0, 1); cubeDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue towerShearModulus = pow(10.0f, 3);
	int cubeLayers[3] = {4, 36, 6}; // layers in standard pose
	SimuValue distToCubeX = 60;
	SimuValue distToCubeY = -160;

	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue ballShearModulus = pow(10.0f, 5);
	SimuValue ballRelaxationTime = pow(10.0f, 1);
	CVector3D ballIniVel; ballIniVel.SetElements(150, 0, 0);
	SimuValue ballOnlyGravityTime = 0.35;
	SimuValue towerOnlyBdryMovingTime = 0.35;
	SimuValue ballRadius = 40;
	SimuValue ballIsoDensituRadius = 8.5*CSimuManager::m_prtlDistance;
	SimuValue ballIsoDensityRatio = 0.6;
	SimuValue ballAirPressure = -100;

	bool bSingleFluid = false;

	CPrtlFluid* fluidTower = CSimuManager::CreatePrtlFluid();
	fluidTower->m_deiName = "fluid_tower";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidTower);
	fluidTower->m_pfSimuManager = m_pSimuManager;
	fluidTower->m_pfShearModulus = towerShearModulus;
	fluidTower->m_pfOnlyBdryMovingTime = towerOnlyBdryMovingTime;
	SimuValue cubeLenX = (cubeLayers[X]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenY = (cubeLayers[Y]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenZ = (cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	// create prtls in cube
	CVector3D cubeBtmPos;
	cubeBtmPos.SetValue(&ctrPos);
	cubeBtmPos.AddedBy(&cubeDirection, -cubeLenZ*0.5);
	CBoxOnXYPlane::CreatePrtlsInBox(fluidTower,
									&cubeBtmPos, &cubeDirection, 0, 0,
									cubeLayers, CSimuManager::m_prtlDistance);
	fluidTower->PreparePrtlEvolution();
	fluidTower->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
	fluidTower->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
	// create fluid ball
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);
	ballCtrPos[X] -= distToCubeX + ballRadius + cubeLenX*0.5;
	ballCtrPos[Y] += cubeLenY*0.5 + distToCubeY + ballRadius;
	if (bSingleFluid)
	{
		fluidTower->m_deiName = "ball_tower";
		fluidTower->m_pfOnlyGravityTime = 0;
		fluidTower->m_pfAirPressure = ballAirPressure;
		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos, &cubeDirection);
		prtlInOval->CreatePrtlInOval(fluidTower, CSimuManager::m_prtlDistance*0.5, 0, &ballIniVel);
		delete prtlInOval;
		fluidTower->PreparePrtlEvolution();
	}
	else
	{
		CPrtlFluid* ballFluid = CSimuManager::CreatePrtlFluid();
		ballFluid->m_deiName = "fluid_ball";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(ballFluid);
		ballFluid->m_pfSimuManager = m_pSimuManager;
		ballFluid->m_pfShearModulus = ballShearModulus;
		ballFluid->m_pfOnlyGravityTime = ballOnlyGravityTime;
		ballFluid->m_pfAirPressure = ballAirPressure;
		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos, &cubeDirection);
		prtlInOval->CreatePrtlInOval(ballFluid, CSimuManager::m_prtlDistance*0.5, 0);
		delete prtlInOval;
		ballFluid->PreparePrtlEvolution();
		ballFluid->InitializePrtlVelocities(ballIniVel);
		ballFluid->m_pfColor.SetValue(CSimuManager::COLOR_RED);
		ballFluid->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
	}
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] += -cubeLenY*0.5;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelMultipleballs() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D ballDirection; ballDirection.SetValue(&floorDirection);
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue ballRadius = 50;
	SimuValue ballDistance = 200;

	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue shearModulus[6] = {pow(10.0f, 3), pow(10.0f, 3), pow(10.0f, 4), pow(10.0f, 4), pow(10.0f, 5), pow(10.0f, 5)};
	SimuValue distToFloor = 250;
	CSimuManager::m_onlyGravityTime = 1.95;

	CVector3D ballCtrPos[6];
	ballCtrPos[0].SetValue(&ctrPos);
	ballCtrPos[1].SetValue(&ctrPos);
	ballCtrPos[2].SetValue(&ctrPos);
	ballCtrPos[3].SetValue(&ctrPos);
	ballCtrPos[4].SetValue(&ctrPos);
	ballCtrPos[5].SetValue(&ctrPos);
	ballCtrPos[0][X] -= ballDistance*0.5;
	ballCtrPos[1][X] += ballDistance*0.5;
	ballCtrPos[2][X] -= ballDistance*0.5;
	ballCtrPos[3][X] += ballDistance*0.5;
	ballCtrPos[4][X] -= ballDistance*0.5;
	ballCtrPos[5][X] += ballDistance*0.5;
	ballCtrPos[0][Z] -= ballDistance*0.5;
	ballCtrPos[1][Z] -= ballDistance*0.5;
	ballCtrPos[2][Z] += ballDistance*0.5;
	ballCtrPos[3][Z] += ballDistance*0.5;
	ballCtrPos[4][Z] += ballDistance*1.5;
	ballCtrPos[5][Z] += ballDistance*1.5;

	CPrtlFluid* fluidBall[6];
	char tmpName[LEN_FLUID_NAME];
	for (int i=0; i < 6; i++)
	{
		fluidBall[i] = CSimuManager::CreatePrtlFluid();
		sprintf(tmpName, "fluid_ball_%d", fluidBall[i]->m_pfFluidId);
		fluidBall[i]->m_deiName = tmpName;
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBall[i]);
		fluidBall[i]->m_pfSimuManager = m_pSimuManager;
		fluidBall[i]->m_pfShearModulus = shearModulus[i];

		CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
												  0, &ballCtrPos[i], &ballDirection);
		prtlInOval->CreatePrtlInOval(fluidBall[i], CSimuManager::m_prtlDistance*0.5, 0);
		delete prtlInOval;

		fluidBall[i]->PreparePrtlEvolution();

		switch (i)
		{
			case 0:
				fluidBall[0]->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
				fluidBall[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
				break;
			case 1:
				fluidBall[1]->m_pfColor.SetValue(CSimuManager::COLOR_RED);
				fluidBall[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
				break;
			case 2:
				fluidBall[2]->m_pfColor.SetValue(CSimuManager::COLOR_CYAN);
				fluidBall[2]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_CYAN);
				break;
			case 3:
				fluidBall[3]->m_pfColor.SetValue(CSimuManager::COLOR_MAGENTA);
				fluidBall[3]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
				break;
			case 4:
				fluidBall[4]->m_pfColor.SetValue(CSimuManager::COLOR_ORANGE);
				fluidBall[4]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
				break;
			case 5:
				fluidBall[5]->m_pfColor.SetValue(CSimuManager::COLOR_GREEN);
				fluidBall[5]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_MAGENTA);
				break;
		}
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidstretching() 
{
	// TODO: Add your command handler code here
	
	CVector3D fluidCtrPos; fluidCtrPos.SetElements(0, 0, 0);
	CVector3D fluidDirection; fluidDirection.SetElements(1, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	SimuValue distToFloor = 120;
	SimuValue fluidRadius = 50;
	SimuValue fluidLength = 100; // must be > 2*stretchEndLength
	SimuValue stretchEndLength = 20; 
//	CSimuManager::m_numFramesPerSecond = 64;
	bool bRandomPrtls = false;
	unsigned randomSeed = 1089841186; //(unsigned)time(NULL); //
	srand(randomSeed);
	CSimuManager::m_prtlPosJitter = 0;
	CSimuManager::m_rotationFactor = 1;

	CSimuManager::APPLY_GRAVITY = false;
	CSimuManager::NOT_COUNT_FIXED_PRTLS = false;
	CSimuManager::NON_NEWTONIAN_FLUID = true;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_shearModulus	= pow(10.0f, 4);
	CSimuManager::m_relaxationTime	= pow(10.0f, -1);
	CSimuManager::m_numStepDivPerFrame = 2;
	CSimuManager::m_airPressure = -1000;
	SimuValue stretchSpeed = 20;
	int numRanPrtls = 1200;
	if (CSimuManager::m_eFluidModel == CSimuManager::UPSAMPLE) {}
	else if (CSimuManager::m_eFluidModel == CSimuManager::SPH)
	{
		CSimuManager::m_bSpikyGradient = true;
		if (bRandomPrtls)
		{
			SimuValue cylVolume = PI*fluidRadius*fluidRadius*fluidLength;
			SimuValue prtlsInUnitVol = numRanPrtls/cylVolume;
			SimuValue numNgbrs = 30;
			SimuValue ngbrHoodVol = numNgbrs/prtlsInUnitVol;
			SimuValue ngbrHoodRadius = pow(ngbrHoodVol/(SIMU_CONST_4_OVER_3*PI), SIMU_CONST_ONE_THIRD);
			CSimuManager::SetPrtlDistance(ngbrHoodRadius/CSimuManager::m_smoothLengthRatio);
		}
	}

	m_pSimuManager->CreateCurrentPrtlFluid();
	CSimuManager::m_maxSrfcDetectPauseSteps = 1;
	m_pSimuManager->m_curPrtlFluid->m_pfSrfcDetectPauseSteps = 1;

	SimuValue rodSrfcThickness = 10;
	SimuValue rodEndThickness = 5;
	CVector3D topCtrPos;
	topCtrPos.SetValue(&fluidCtrPos);
	topCtrPos.AddedBy(&fluidDirection, fluidLength/2);
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius,
															 0, false, false,
															 &topCtrPos, &fluidDirection);
	if (CSimuManager::m_eFluidModel == CSimuManager::SPH
	 && bRandomPrtls)
	{
		pCylinder->CreateFluidPrtlsRandomlyInCylinder(m_pSimuManager->m_curPrtlFluid, numRanPrtls);
	}
	else
		pCylinder->CreateFluidPrtlsInCylinder(m_pSimuManager->m_curPrtlFluid);
	delete pCylinder;

	SimuValue halfFreeFluidLen = 0.5*(fluidLength - stretchEndLength*2);
	SimuValue rodLength = stretchEndLength + 2*rodEndThickness;
	// create left rod
	CVector3D leftRodTopCtrPos;
	leftRodTopCtrPos.SetValue(&fluidCtrPos);
	leftRodTopCtrPos.AddedBy(&fluidDirection, - (halfFreeFluidLen - rodEndThickness));
	CBdrySolidRod* leftRod = new CBdrySolidRod(rodLength, fluidRadius+rodSrfcThickness,
											   0, false, false, &leftRodTopCtrPos, &fluidDirection);
	leftRod->m_bdryVel.SetValue(&fluidDirection, -stretchSpeed);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(leftRod);
	// create right rod
	CVector3D rightRodTopCtrPos;
	rightRodTopCtrPos.SetValue(&fluidCtrPos);
	rightRodTopCtrPos.AddedBy(&fluidDirection, halfFreeFluidLen - rodEndThickness);
	fluidDirection.ScaledBy(-1);
	CBdrySolidRod* rightRod = new CBdrySolidRod(rodLength, fluidRadius+rodSrfcThickness,
											    0, false, false, &rightRodTopCtrPos, &fluidDirection);
	rightRod->m_bdryVel.SetValue(&fluidDirection, -stretchSpeed);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(rightRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&fluidCtrPos);
	squareCtrPos.AddedBy(&floorDirection, -(fluidRadius + distToFloor));
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_curPrtlFluid->AddBoundary(bdryFloor);

	m_pSimuManager->m_curPrtlFluid->FixPrtlMotionWithBoundaries();
	m_pSimuManager->m_curPrtlFluid->PreparePrtlEvolution();
	m_pSimuManager->m_curPrtlFluid->m_deiName = "fluid_stretching";
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelCollidingcubes() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D cubeDirection; cubeDirection.SetValue(&floorDirection);
	CVector3D cubeIniVel[2];
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_eInterFluidModel = CSimuManager::DETECT_BY_CONVEX_HULL; // REPULSIVE_FORCE; // 
	bool bRightCubeStill = false;
	SimuValue massRatio = 1; // left cube vs. right cube
	CSimuManager::m_interFluidTension = pow(10.0f, 6);

	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue shearModulus[2] = {pow(10.0f, 4), pow(10.0f, 4)};
	SimuValue cubeDim[3] = {60, 60, 110};
	SimuValue distToFloor = 150;
	SimuValue cubeDistance = 200;
	SimuValue iniSpeed = 150;
	CSimuManager::m_onlyGravityTime = 0.65;

	if (bRightCubeStill)
		iniSpeed *= 2;
	cubeIniVel[0].SetElements(iniSpeed, 0, 0);
	if (bRightCubeStill)
		cubeIniVel[1].SetElements(0, 0, 0);
	else
		cubeIniVel[1].SetElements(-iniSpeed, 0, 0);
	bool bSingleFluid = false;

	int cubeLayers[3];
	cubeLayers[X] = (int)floor(cubeDim[X]/CSimuManager::m_prtlDistance)+1;
	cubeLayers[Y] = (int)floor(cubeDim[Y]/CSimuManager::m_prtlDistance)+1;
	cubeLayers[Z] = (int)floor(cubeDim[Z]/CSimuManager::m_prtlDistance)+1;

	CVector3D cubeBtmCtrPos[2];
	cubeBtmCtrPos[0].SetValue(&ctrPos);
	cubeBtmCtrPos[1].SetValue(&ctrPos);
	cubeBtmCtrPos[0][X] -= cubeDistance*0.5 + cubeDim[X]*0.5;
	cubeBtmCtrPos[1][X] += cubeDistance*0.5 + cubeDim[X]*0.5;
	cubeBtmCtrPos[0][Y] -= cubeDim[Y]*0.5;
	cubeBtmCtrPos[1][Y] -= cubeDim[Y]*0.5;

	CPrtlFluid* fluidCube[2];
	CPrtlFluid* pFluid = NULL;
	if (bSingleFluid)
	{
		pFluid = CSimuManager::CreatePrtlFluid();
		pFluid->m_deiName = "cube_merge";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);
		pFluid->m_pfSimuManager = m_pSimuManager;
		pFluid->m_pfShearModulus = shearModulus[0];
	}
	char tmpName[LEN_FLUID_NAME];
	for (int i=0; i < 2; i++)
	{
		if (bSingleFluid)
		{
			CBoxOnXYPlane::CreatePrtlsInBox(pFluid, &cubeBtmCtrPos[i], &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
		}
		else
		{
			fluidCube[i] = CSimuManager::CreatePrtlFluid();
			sprintf(tmpName, "fluid_cube_%d", fluidCube[i]->m_pfFluidId);
			fluidCube[i]->m_deiName = tmpName;
			m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCube[i]);
			fluidCube[i]->m_pfSimuManager = m_pSimuManager;
			fluidCube[i]->m_pfShearModulus = shearModulus[i];
			CBoxOnXYPlane::CreatePrtlsInBox(fluidCube[i], &cubeBtmCtrPos[i], &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
			fluidCube[i]->PreparePrtlEvolution();
			fluidCube[i]->InitializePrtlVelocities(cubeIniVel[i]);
			switch (i)
			{
				case 0:
					fluidCube[0]->m_pfPrtlMassRatio = massRatio;
					fluidCube[0]->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
					fluidCube[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
					break;
				case 1:
					fluidCube[1]->m_pfColor.SetValue(CSimuManager::COLOR_RED);
					fluidCube[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
					break;
			}
		}
	}
	if (bSingleFluid)
		pFluid->PreparePrtlEvolution();

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&(cubeBtmCtrPos[0]));
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelMultiplecollidingcubes() 
{
	// TODO: Add your command handler code here

	// after GI06 review, it is needed to produce an example of multiple fluid collision.
	// this function and the following code are added to produce such an example
	// in a previous version of SimuFlexApp near Oct. 31, 2005. In order to grow the version
	// without branching, they are inserted on Mar. 28, 2006 in the current version.
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D cubeDirection; cubeDirection.SetValue(&floorDirection);
	CVector3D cubeIniVel[2];
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_eInterFluidModel = CSimuManager::DETECT_BY_CONVEX_HULL; // REPULSIVE_FORCE; // 
	CSimuManager::APPLY_GRAVITY = true;
	bool bRightCubeStill = false;
	SimuValue massRatio = 1; // left cube vs. right cube
	CSimuManager::m_interFluidTension = pow(10.0f, 6);

	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue shearModulus[2] = {5*pow(10.0f, 3), 5*pow(10.0f, 3)};
	SimuValue cubeDim[3] = {60, 60, 110};
	SimuValue distToFloor = 150;
	SimuValue cubeDistance = 200;
	SimuValue iniSpeed = 150;
	CSimuManager::m_onlyGravityTime = 0.65;

	if (bRightCubeStill)
		iniSpeed *= 2;
	cubeIniVel[0].SetElements(iniSpeed, 0, 0);
	if (bRightCubeStill)
		cubeIniVel[1].SetElements(0, 0, 0);
	else
		cubeIniVel[1].SetElements(-iniSpeed, 0, 0);
	bool bSingleFluid = false;
	bool bMultipleFluids = true;
	SimuValue verticalDist = 50;

	int cubeLayers[3];
	cubeLayers[X] = (int)floor(cubeDim[X]/CSimuManager::m_prtlDistance)+1;
	cubeLayers[Y] = (int)floor(cubeDim[Y]/CSimuManager::m_prtlDistance)+1;
	cubeLayers[Z] = (int)floor(cubeDim[Z]/CSimuManager::m_prtlDistance)+1;

	CVector3D cubeBtmCtrPos[2];
	cubeBtmCtrPos[0].SetValue(&ctrPos);
	cubeBtmCtrPos[1].SetValue(&ctrPos);
	cubeBtmCtrPos[0][X] -= cubeDistance*0.5 + cubeDim[X]*0.5;
	cubeBtmCtrPos[1][X] += cubeDistance*0.5 + cubeDim[X]*0.5;
	cubeBtmCtrPos[0][Y] -= cubeDim[Y]*0.5;
	cubeBtmCtrPos[1][Y] -= cubeDim[Y]*0.5;

	CPrtlFluid* fluidCube[2];
	CPrtlFluid* pFluid = NULL;
	if (bSingleFluid)
	{
		pFluid = CSimuManager::CreatePrtlFluid();
		pFluid->m_deiName = "cube_merge";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);
		pFluid->m_pfSimuManager = m_pSimuManager;
		pFluid->m_pfShearModulus = shearModulus[0];
	}
	char tmpName[LEN_FLUID_NAME];
	for (int i=0; i < 2; i++)
	{
		if (bSingleFluid)
		{
			CBoxOnXYPlane::CreatePrtlsInBox(pFluid, &cubeBtmCtrPos[i], &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
		}
		else
		{
			fluidCube[i] = CSimuManager::CreatePrtlFluid();
			sprintf(tmpName, "cube_%d", fluidCube[i]->m_pfFluidId);
			fluidCube[i]->m_deiName = tmpName;
			m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCube[i]);
			fluidCube[i]->m_pfSimuManager = m_pSimuManager;
			fluidCube[i]->m_pfShearModulus = shearModulus[i];
			CBoxOnXYPlane::CreatePrtlsInBox(fluidCube[i], &cubeBtmCtrPos[i], &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
			fluidCube[i]->PreparePrtlEvolution();
			fluidCube[i]->InitializePrtlVelocities(cubeIniVel[i]);
			switch (i)
			{
				case 0:
					fluidCube[0]->m_pfPrtlMassRatio = massRatio;
					fluidCube[0]->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
					fluidCube[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
					break;
				case 1:
					fluidCube[1]->m_pfColor.SetValue(CSimuManager::COLOR_RED);
					fluidCube[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
					break;
			}
		}
	}
	if (bSingleFluid)
		pFluid->PreparePrtlEvolution();

	if (bMultipleFluids)
	{
		for (int i=0; i < 1; i++)
		{
			fluidCube[i] = CSimuManager::CreatePrtlFluid();
			sprintf(tmpName, "cube_%d", fluidCube[i]->m_pfFluidId);
			fluidCube[i]->m_deiName = tmpName;
			m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCube[i]);
			fluidCube[i]->m_pfSimuManager = m_pSimuManager;
			fluidCube[i]->m_pfShearModulus = shearModulus[i];
			cubeBtmCtrPos[i].v[X] = ctrPos[X];
			cubeBtmCtrPos[i].v[Y] += verticalDist + cubeDim[Z];
			CBoxOnXYPlane::CreatePrtlsInBox(fluidCube[i], &cubeBtmCtrPos[i], &cubeDirection,
											0, 0, cubeLayers, CSimuManager::m_prtlDistance);
			fluidCube[i]->PreparePrtlEvolution();
			cubeBtmCtrPos[i].v[Y] -= verticalDist + cubeDim[Z];
			switch (i)
			{
				case 0:
					fluidCube[0]->m_pfPrtlMassRatio = massRatio;
					fluidCube[0]->m_pfColor.SetValue(CSimuManager::COLOR_GREEN);
					fluidCube[0]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_GREEN);
					break;
				case 1:
					fluidCube[1]->m_pfColor.SetValue(CSimuManager::COLOR_YELLOW);
					fluidCube[1]->m_pfSrfcColor.SetValue(CSimuManager::COLOR_YELLOW);
					break;
			}
		}
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&(cubeBtmCtrPos[0]));
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelBallintojello() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(0, 1, 0); cubeDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_minTemperature = 200; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 600; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_onlyGravityTime = 1.38;
	SimuValue distance = 100;
	SimuValue cubeIniTemperature = 200;
	SimuValue ballIniTemperature = 10000;
	SimuValue cubeMinShearModulus = pow(10.0f, 2);

	CSimuManager::m_heatConductivity = 1000;
	int cubeLayers[3] = {10, 14, 14};
	SimuValue ballMassRatio = 50;
	SimuValue ballRadius = 40;
	SimuValue shiftOnX = 30;
	CSimuManager::m_numStepDivPerFrame = 4;
	SimuValue ballMinShearModulus = pow(10.0f, 6);
	SimuValue ballMaxShearModulus = 5*pow(10.0f, 6);
	SimuValue ballCoeffTemperature = 0.0001;
	SimuValue cubeMaxShearModulus = 5*pow(10.0f, 6);

	CPrtlFluid* fluidBall = CSimuManager::CreatePrtlFluid();
	fluidBall->m_deiName = "fluid_ball";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBall);
	fluidBall->m_pfSimuManager = m_pSimuManager;
	fluidBall->m_pfPrtlMassRatio = ballMassRatio;
	fluidBall->m_pfIniTemperature = ballIniTemperature;
	fluidBall->m_pfMinShearModulus = ballMinShearModulus;
	fluidBall->m_pfMaxShearModulus = ballMaxShearModulus;
	fluidBall->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);
	ballCtrPos[X] += shiftOnX;
	ballCtrPos[Y] += distance + ballRadius;
	CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
											  0, &ballCtrPos, &cubeDirection);
	prtlInOval->CreatePrtlInOval(fluidBall, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;
	fluidBall->PreparePrtlEvolution();

	CPrtlFluid* fluidCube = CSimuManager::CreatePrtlFluid();
	fluidCube->m_deiName = "fluid_cube";
	fluidCube->m_pfIniTemperature = cubeIniTemperature;
	fluidCube->m_pfMinShearModulus = cubeMinShearModulus;
	fluidCube->m_pfMaxShearModulus = cubeMaxShearModulus;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCube);
	SimuValue cubeHeight = (cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	CVector3D cubeBtmCtrPos;
	cubeBtmCtrPos.SetValue(&ctrPos);
	cubeBtmCtrPos[Y] -= cubeHeight;
	fluidCube->m_pfSimuManager = m_pSimuManager;
	fluidCube->m_pfOnlyBdryMovingTime = CSimuManager::m_onlyGravityTime;
	CBoxOnXYPlane::CreatePrtlsInBox(fluidCube, &cubeBtmCtrPos, &cubeDirection,
									0, 0, cubeLayers, CSimuManager::m_prtlDistance);
	fluidCube->PreparePrtlEvolution();

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&cubeBtmCtrPos);
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelEggwithyork() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 100, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue bdryFriction = 0.75; // '1' means no friction
	SimuValue boardTiltAngle = 0;
	SimuValue boardThick = 100;
	SimuValue boardWidth = 1000;
	SimuValue boardLength = 1000;
	bool bYolkOnly = false;
	bool bHasBoard = false;
	bool bOnlyRod = false;

	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue eggShearModulus = 500;
	SimuValue yolkShearModulus = 8000;
	SimuValue yolkPrtlMassRatio = 100;
	SimuValue eggRadius = 80;
	SimuValue yolkRatio = 0.6;
	SimuValue distanceToFloor = 100;
	CSimuManager::m_onlyGravityTime = 1.35;
	CSimuManager::m_interFluidDamping = 0;

	SimuValue shiftAwayEdge = 70;
	SimuValue distanceToBoard = 50;

	CVector3D eggCtrPos; eggCtrPos.SetValue(&ctrPos);
	CVector3D eggIniVel; eggIniVel.SetElements(0, 0, 0);

	CPrtlFluid* eggWhite = NULL;
	if (!bYolkOnly)
	{
		eggWhite = CSimuManager::CreatePrtlFluid();
		eggWhite->m_deiName = "eggwhite";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
		eggWhite->m_pfSimuManager = m_pSimuManager;
		eggWhite->m_pfShearModulus = eggShearModulus;
		eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	}

	CreateEggWithYolk(eggWhite, 0, &(eggCtrPos), &(eggIniVel), eggRadius, yolkRatio, 
					CSimuManager::m_airPressure, yolkShearModulus,
					CSimuManager::m_relaxationTime, yolkPrtlMassRatio);

	if (!bYolkOnly)
	{
		eggWhite->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
		eggWhite->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
		eggWhite->PreparePrtlEvolution();
		eggWhite->ApplyFluidColorOntoPrtls();
	}

	// create board
	if (bHasBoard)
	{
		CVector3D rodDirection;
		rodDirection.SetElements(0, 0, 1);
		CVector3D rodTopCtrPos;
		rodTopCtrPos.SetValue(&eggCtrPos);
		rodTopCtrPos[X] -= shiftAwayEdge;
		rodTopCtrPos[Y] -= eggRadius + distanceToBoard + boardThick*0.5;
		rodTopCtrPos[Z] += boardLength*0.5;
		CBdrySolidRod* boardEdge = new CBdrySolidRod(boardLength, boardThick*0.5,
													0, false, false, 
													&rodTopCtrPos, &rodDirection);
		boardEdge->m_bdryDistDumpingRate	= 0;
		boardEdge->m_bdryEnergyDumpingRate	= 0;
		boardEdge->m_bdryFrictionRate		= bdryFriction;
		boardEdge->m_bdryStressDumpingRate	= 0;
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(boardEdge);

		if (!bOnlyRod)
		{
			CVector3D boardDirection;
			boardDirection.SetValue(&rodDirection);
			CVector3D boxBtmCtrPos;
			boxBtmCtrPos.SetValue(&eggCtrPos);
			boxBtmCtrPos[X] -= shiftAwayEdge + boardWidth*0.5;
			boxBtmCtrPos[Y] -= eggRadius + distanceToBoard + boardThick*0.5;
			boxBtmCtrPos[Z] -= boardLength*0.5;
			boxBtmCtrPos[X] += boardWidth*0.5*(1-cos(boardTiltAngle*PI/180));
			boxBtmCtrPos[Y] += boardWidth*0.5*sin(boardTiltAngle*PI/180);
			CBdrySolidBox* pBoard = new CBdrySolidBox(boardLength, boardWidth, boardThick,
													-boardTiltAngle, &boxBtmCtrPos, &boardDirection);
			pBoard->m_bdryDistDumpingRate	= 0;
			pBoard->m_bdryEnergyDumpingRate	= 0;
			pBoard->m_bdryFrictionRate		= bdryFriction;
			pBoard->m_bdryStressDumpingRate	= 0;
			m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBoard);
		}
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= eggRadius + distanceToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	pFloor->m_bdryDistDumpingRate	= 0;
	pFloor->m_bdryEnergyDumpingRate	= 0;
	pFloor->m_bdryFrictionRate		= bdryFriction;
	pFloor->m_bdryStressDumpingRate	= 0;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelMultipleeggs() 
{
	// TODO: Add your command handler code here

	CVector3D ctrPos; ctrPos.SetElements(0, 100, 0);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue bdryFriction = 0.75; // '1' means no friction
	bool bYolkOnly = false;

	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue eggShearModulus = 500;
	SimuValue yolkShearModulus = 10000;
	SimuValue yolkPrtlMassRatio = 5;
	SimuValue eggRadius = 80;
	SimuValue yolkRatio = 0.7;
	SimuValue eggShift = 120;
	SimuValue eggSpeed = 100;
	SimuValue eggDistance = 100;
	SimuValue distanceToFloor = 75;
	CSimuManager::m_onlyGravityTime = 0.65;

	CVector3D eggCtrPos[2];
	eggCtrPos[0].SetValue(&ctrPos);
	eggCtrPos[1].SetValue(&ctrPos);
	eggCtrPos[0][X] += eggDistance*0.5 + eggRadius;
	eggCtrPos[1][X] -= eggDistance*0.5 + eggRadius;
	eggCtrPos[1][Y] += eggShift;
	CVector3D eggIniVel[2];
	eggIniVel[0].SetElements(-eggSpeed, 0, 0);
	eggIniVel[1].SetElements(eggSpeed, 0, 0);

	CPrtlFluid* eggWhite = NULL;
	if (!bYolkOnly)
	{
		eggWhite = CSimuManager::CreatePrtlFluid();
		eggWhite->m_deiName = "eggwhite";
		m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
		eggWhite->m_pfSimuManager = m_pSimuManager;
		eggWhite->m_pfShearModulus = eggShearModulus;
		eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	}

	CreateEggWithYolk(eggWhite, 0, &(eggCtrPos[0]), &(eggIniVel[0]), eggRadius, yolkRatio, 
					CSimuManager::m_airPressure, yolkShearModulus,
					CSimuManager::m_relaxationTime, yolkPrtlMassRatio);
	CreateEggWithYolk(eggWhite, 1, &(eggCtrPos[1]), &(eggIniVel[1]), eggRadius, yolkRatio,
					CSimuManager::m_airPressure, yolkShearModulus,
					CSimuManager::m_relaxationTime, yolkPrtlMassRatio);

	if (!bYolkOnly)
	{
		eggWhite->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
		eggWhite->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
		eggWhite->PreparePrtlEvolution();
		eggWhite->ApplyFluidColorOntoPrtls();
	}

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= eggRadius + distanceToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	pFloor->m_bdryDistDumpingRate	= 0;
	pFloor->m_bdryEnergyDumpingRate	= 0;
	pFloor->m_bdryFrictionRate		= bdryFriction;
	pFloor->m_bdryStressDumpingRate	= 0;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnTestSurfacetensiononcube() 
{
	// TODO: Add your command handler code here
	
	CVector3D btmCtrPos; btmCtrPos.SetElements(0, 0, 0);
	CVector3D direction; direction.SetElements(0, 1, 0);
	int layer = 7;
	int cubeLayers[3] = {2*layer, layer, layer};

	CSimuManager::APPLY_GRAVITY = false;
	CSimuManager::APPLY_SURFACE_TENSION = true;
	CSimuManager::m_surfaceTension = 100000;
	CSimuManager::m_numStepDivPerFrame = 2;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_shearModulus	= 1000;
	CSimuManager::m_relaxationTime	= 0.1;

	CPrtlFluid* fluidCube = m_pSimuManager->CreatePrtlFluid();
	CBoxOnXYPlane::CreatePrtlsInBox(fluidCube, &btmCtrPos, &direction,
									0, 0, cubeLayers, CSimuManager::m_prtlDistance);
	fluidCube->m_deiName = "fluid_cube";
	fluidCube->PreparePrtlEvolution();

	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCube);
	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelEggdropintobowl() 
{


	CSimuManager::m_bStickyInterFluids = true;
	CSimuManager::HEAT_TRANSFER = false; // true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	SimuValue slopeDimX = 400; 
	SimuValue slopeDimZ = 60; 
	SimuValue slopeDimY = 700;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_framesPerResample = 2;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_maxShearModulus = pow(10.0f, 6);
	SimuValue eggConstSummation = 0;
	SimuValue eggCoeffTemperature = -0.01;
	SimuValue airPressure = -300;
	SimuValue yolkPrtlMassRatio = 50;
	SimuValue yolkIniTemperature = 0;
	SimuValue distEggToPlate = 100;
	CSimuManager::m_onlyGravityTime = 1.35;
	CSimuManager::m_rotationFactor	= 1;

	SimuValue eggConductivity = 1000;
	SimuValue yolkConductivity = 1000;
	SimuValue slopeFriction = 1;
	SimuValue eggRadius = 85;
	SimuValue yolkRatio = 0.55;
	SimuValue plateTemperature = 2000;
	SimuValue edgeTemperature = 2000;
	SimuValue yolkConstSummation = 4;
	SimuValue yolkCoeffTemperature = -0.003;
	SimuValue eggIniTemperature = -1000;
	SimuValue yolkMinShearModulus = pow(10.0f, 4);
	CVector3D slopeDirection; slopeDirection.SetElements(0, 1, 0.125); slopeDirection.Normalize();
	SimuValue slopeDistToFloor = 200;
	CSimuManager::m_interFluidStickyness = pow(10.0f,-3);

	SimuValue slopeRatio = slopeDirection[Z]/slopeDirection[Y]; // tagent value
	SimuValue slopeCosine = sqrt(1/(1+pow(slopeRatio, 2)));
	SimuValue slopeSine = sqrt(1-1/(1+pow(slopeRatio, 2)));


/*	
	CPrtlFluid* eggWhite = CSimuManager::CreatePrtlFluid();
	eggWhite->m_deiName = "eggwhite";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
	eggWhite->m_pfSimuManager = m_pSimuManager;
	eggWhite->m_pfAirPressure = airPressure;
	eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	eggWhite->m_pfHeatConductivity = eggConductivity;
	eggWhite->m_pfIniTemperature = eggIniTemperature;
	eggWhite->m_pfCoeffTemperature = eggCoeffTemperature;
	eggWhite->m_pfConstSummation = eggConstSummation;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorR = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorG = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorB = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorR = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorG = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorB = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttConstFilter = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioFilter = -0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttMinTemperature = 200;
	eggWhite->m_pfLinrTempTxtr.m_lttMaxTemperature = 700;

	CVector3D eggCtrPos; eggCtrPos.SetValue(&ctrPos);
	eggCtrPos[Y] += distEggToPlate + eggRadius;
	CVector3D eggIniVel; eggIniVel.SetElements(0, 0, 0);
	CPrtlFluid* yolk = CreateEggWithYolk(eggWhite, 0, &(eggCtrPos), &(eggIniVel),
										eggRadius, yolkRatio, 
										CSimuManager::m_airPressure, pow(10.0f, 5),
										CSimuManager::m_relaxationTime, yolkPrtlMassRatio);
	yolk->m_pfAirPressure = airPressure;
	yolk->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	yolk->m_pfHeatConductivity = yolkConductivity;
	yolk->m_pfMinShearModulus = yolkMinShearModulus;
	yolk->m_pfIniTemperature = yolkIniTemperature;
	yolk->m_pfCoeffTemperature = yolkCoeffTemperature;
	yolk->m_pfConstSummation = yolkConstSummation;
	yolk->m_pfLinrTempTxtr.m_lttConstColorR = 0.95;
	yolk->m_pfLinrTempTxtr.m_lttConstColorG = 0;
	yolk->m_pfLinrTempTxtr.m_lttConstColorB = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorR = -0.95;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorG = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorB = 0.95;
	yolk->m_pfLinrTempTxtr.m_lttConstFilter = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioFilter = 0;
	yolk->m_pfLinrTempTxtr.m_lttMinTemperature = 500;
	yolk->m_pfLinrTempTxtr.m_lttMaxTemperature = 700;

	eggWhite->PreparePrtlEvolution();
	yolk->PreparePrtlEvolution();

*/
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; //

	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;

	CSimuManager::m_onlyGravityTime = 2.05;
	CSimuManager::m_interFluidDamping = 0;
	SimuValue bdryParam = 1;
	// start of new animation

#if 1


	// cube 1
	SimuValue slopeDimX1 = 60; 
	SimuValue slopeDimZ1 = 60; 
	SimuValue slopeDimY1 = 60;

	CVector3D slopeBtmCtrPos1;
	CVector3D ctrPos1; ctrPos1.SetElements(0, 60, 0);
	CVector3D slopeDirection1; slopeDirection1.SetElements(0, 1, 0); slopeDirection1.Normalize();
	slopeBtmCtrPos1.SetValue(&ctrPos1);
	slopeBtmCtrPos1.AddedBy(&slopeDirection1, -slopeDimZ1);
	CBdrySolidBox* pSlope1 = new CBdrySolidBox(slopeDimZ1, slopeDimX1, slopeDimY1,
						  0, &slopeBtmCtrPos1, &slopeDirection1);
	

	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope1);
	// end of cube 1


	// cube 2
	SimuValue slopeDimX2 = 60; 
	SimuValue slopeDimZ2 = 60; 
	SimuValue slopeDimY2 = 60;

	CVector3D slopeBtmCtrPos2;
	CVector3D ctrPos2; ctrPos2.SetElements(-40, 160, -40);
	CVector3D slopeDirection2; slopeDirection2.SetElements(0.325, 1, 0.325); slopeDirection2.Normalize();
	slopeBtmCtrPos2.SetValue(&ctrPos2);
	slopeBtmCtrPos2.AddedBy(&slopeDirection2, -slopeDimZ2);
	CBdrySolidBox* pSlope2 = new CBdrySolidBox(slopeDimZ2, slopeDimX2, slopeDimY2,
						  0, &slopeBtmCtrPos2, &slopeDirection2);
	

	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope2);
	// end of cube 2


	// cube 3
	SimuValue slopeDimX3 = 60; 
	SimuValue slopeDimZ3 = 60; 
	SimuValue slopeDimY3 = 60;

	CVector3D slopeBtmCtrPos3;
	CVector3D ctrPos3; ctrPos3.SetElements(30, 180, 35);
	CVector3D slopeDirection3; slopeDirection3.SetElements(-0.225, 1, -0.225); slopeDirection3.Normalize();
	slopeBtmCtrPos3.SetValue(&ctrPos3);
	slopeBtmCtrPos3.AddedBy(&slopeDirection3, -slopeDimZ3);
	CBdrySolidBox* pSlope3 = new CBdrySolidBox(slopeDimZ3, slopeDimX3, slopeDimY3,
						  0, &slopeBtmCtrPos3, &slopeDirection3);
	

	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope3);
	// end of cube 3
#endif

#if 0
	// cube 1
	SimuValue slopeDimX1 = 60; 
	SimuValue slopeDimZ1 = 60; 
	SimuValue slopeDimY1 = 60;

	CVector3D slopeBtmCtrPos1;
	CVector3D ctrPos1; ctrPos1.SetElements(0, 60, 0);
	CVector3D slopeDirection1; slopeDirection1.SetElements(0, 1, 0); slopeDirection1.Normalize();
	slopeBtmCtrPos1.SetValue(&ctrPos1);
	slopeBtmCtrPos1.AddedBy(&slopeDirection1, -slopeDimZ1);
	CBdrySolidBox* pSlope1 = new CBdrySolidBox(slopeDimZ1, slopeDimX1, slopeDimY1,
						  0, &slopeBtmCtrPos1, &slopeDirection1);
	

	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope1);
	// end of cube 1
#endif 
	// floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	CVector3D floorBtmCtrPos;


    floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	floorBtmCtrPos.SetValue(&floorBtmCtrPos);

	squareCtrPos.SetValue(&floorBtmCtrPos);
	squareCtrPos.AddedBy(&floorDirection, -1);

	
	squareCtrPos[Y] = -40;
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);
	


	// floor end

#if 1

	// ball

	/*CVector3D ctrPos;*/ ctrPos.SetElements(0, 0, 0);
	floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CVector3D ballDirection; ballDirection.SetValue(&floorDirection);

	SimuValue ballRadius = 50;


	SimuValue shearModulus[6] = {pow(10.0f, 3), pow(10.0f, 3), pow(10.0f, 4), pow(10.0f, 4), pow(10.0f, 5), pow(10.0f, 5)};

	CSimuManager::m_onlyGravityTime = 1.95;

	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);

	ballCtrPos[Y] = 250;

	CPrtlFluid* fluidBall;
	char tmpName[LEN_FLUID_NAME];

	fluidBall = CSimuManager::CreatePrtlFluid();
	sprintf(tmpName, "fluid_ball_%d", fluidBall->m_pfFluidId);
	fluidBall->m_deiName = tmpName;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBall);
	fluidBall->m_pfSimuManager = m_pSimuManager;
	fluidBall->m_pfShearModulus = shearModulus[0];

	CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
						0, &ballCtrPos, &ballDirection);
	prtlInOval->CreatePrtlInOval(fluidBall, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;


	fluidBall->PreparePrtlEvolution();

	
	fluidBall->m_pfColor.SetValue(CSimuManager::COLOR_BLUE);
	fluidBall->m_pfSrfcColor.SetValue(CSimuManager::COLOR_BLUE);

	// end of ball

#endif


	// end
	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();


////////////////////////////////////////////////////////////////////////////////////////////

#if 0
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 100, 0);
	CVector3D bowlDirection; bowlDirection.SetElements(0, 1, 0); bowlDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue eggRadius = 80;
	SimuValue yolkRatio = 0.55;

	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue eggShearModulus = 500;
	SimuValue yolkShearModulus = 20000;
	SimuValue yolkPrtlMassRatio = 100;
	SimuValue eggShift = 115;
	SimuValue bowlRadius = 200;
	SimuValue distToBowlBtm = 300;
	CSimuManager::m_onlyGravityTime = 2.05;
	CSimuManager::m_interFluidDamping = 0;
	SimuValue bdryParam = 1;

	CPrtlFluid* eggWhite = CSimuManager::CreatePrtlFluid();
	eggWhite->m_deiName = "eggwhite";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
	eggWhite->m_pfSimuManager = m_pSimuManager;
	eggWhite->m_pfShearModulus = eggShearModulus;
	eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;

	CVector3D eggCtrPos; eggCtrPos.SetValue(&ctrPos);
	eggCtrPos[X] += eggShift;
	CVector3D eggIniVel; eggIniVel.SetElements(0, 0, 0);
	CreateEggWithYolk(eggWhite, 0, &(eggCtrPos), &(eggIniVel), eggRadius, yolkRatio, 
					CSimuManager::m_airPressure, yolkShearModulus,
					CSimuManager::m_relaxationTime, yolkPrtlMassRatio);

	eggWhite->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
	eggWhite->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
	eggWhite->PreparePrtlEvolution();
	eggWhite->ApplyFluidColorOntoPrtls();

	// create bowl
	SimuValue bowlHeight = bowlRadius;
	CVector3D bowlBtmPos;
	bowlBtmPos.SetValue(&ctrPos);
	bowlBtmPos.AddedBy(&bowlDirection, - (eggRadius + distToBowlBtm));
	CBdryBowl* pBowl = new CBdryBowl(&bowlDirection, &bowlBtmPos, bowlHeight, bowlRadius);
	pBowl->m_bdryColor->SetValue(CSimuManager::COLOR_ORANGE);
	pBowl->m_bdryDistDumpingRate	= bdryParam;
	pBowl->m_bdryEnergyDumpingRate	= bdryParam;
	pBowl->m_bdryFrictionRate		= bdryParam;
	pBowl->m_bdryStressDumpingRate	= bdryParam;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBowl);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();

#endif
}

void CSimuModeler::OnIntermodelEggthroughfunnel() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 100, 0);
	CVector3D funnelDirection; funnelDirection.SetElements(0, -1, 0); funnelDirection.Normalize();
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	int numCones = 3;
	SimuValue funnelAngle = 45;
	SimuValue eggRadius = 80;
	SimuValue yolkRatio = 0.6;
	SimuValue funnelPipeHeight = 50;
	SimuValue funnelConeHeight = 200;
	SimuValue funnelTopToFloor = 150;
	SimuValue eggShiftOnX = 0;

	SimuValue yolkPrtlMassRatio = 100;
	CSimuManager::m_framesPerResample = 4;
	CSimuManager::m_numStepDivPerFrame = 2;
	SimuValue yolkAirPressure = -100;
	SimuValue eggShearModulus = 500;
	SimuValue yolkShearModulus = 10000;
	SimuValue yolkRelaxationTime = 0.05;
	SimuValue funnelPipeRadius = 40;
	SimuValue eggToFunnelPipe = 70;
	CSimuManager::m_onlyGravityTime = 0.55;
	CSimuManager::m_interFluidDamping = 0;
	SimuValue bdryFriction = 0; // '1' means no friction

	CVector3D eggCtrPos; eggCtrPos.SetValue(&ctrPos);
	eggCtrPos[X] += eggShiftOnX;
	CVector3D eggIniVel; eggIniVel.SetElements(0, 0, 0);

	CPrtlFluid* eggWhite = NULL;
	eggWhite = CSimuManager::CreatePrtlFluid();
	eggWhite->m_deiName = "eggwhite";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
	eggWhite->m_pfSimuManager = m_pSimuManager;
	eggWhite->m_pfShearModulus = eggShearModulus;
	eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;

	CreateEggWithYolk(eggWhite, 0, &(eggCtrPos), &(eggIniVel), eggRadius, yolkRatio, 
					yolkAirPressure, yolkShearModulus,
					yolkRelaxationTime, yolkPrtlMassRatio);

	eggWhite->m_pfColor.SetValue(CSimuManager::COLOR_WHITE);
	eggWhite->m_pfSrfcColor.SetValue(CSimuManager::COLOR_WHITE);
	eggWhite->PreparePrtlEvolution();
	eggWhite->ApplyFluidColorOntoPrtls();

	SimuValue coneHeight = funnelConeHeight/numCones;

	CVector3D coneBtmPos; coneBtmPos.SetValue(&ctrPos);
	coneBtmPos[Y] -= eggRadius + eggToFunnelPipe;
	coneBtmPos[Y] += coneHeight;
	SimuValue coneTopRadius = funnelPipeRadius;
	SimuValue coneAngle = funnelAngle/numCones;
	// create funnel top: hollow cones
	for (int i=0; i < numCones; i++)
	{
		SimuValue coneBtmRadius = coneHeight*tan(coneAngle*PI/180) + coneTopRadius;
		CBdryHollowCone* pCone = new CBdryHollowCone(coneHeight, coneTopRadius,
													coneBtmRadius, &coneBtmPos,
													&funnelDirection);
		pCone->m_bdryDistDumpingRate	= 0;
		pCone->m_bdryEnergyDumpingRate	= 0;
		pCone->m_bdryFrictionRate		= bdryFriction;
		pCone->m_bdryStressDumpingRate	= 0;
		m_pSimuManager->m_aryBoundaries.AppendOnePointer(pCone);
		// prepare for the next cone
		coneTopRadius = coneBtmRadius;
		coneAngle += funnelAngle/numCones;
		coneBtmPos[Y] += coneHeight;
	}
	// create funnel pipe: cylinder
	CVector3D funnelTopCtrPos; funnelTopCtrPos.SetValue(&ctrPos);
	funnelTopCtrPos[Y] -= eggRadius + eggToFunnelPipe + funnelPipeHeight;
	CSimuBoundaryCylinder* pCylinder = new CSimuBoundaryCylinder(funnelPipeHeight,
																 funnelPipeRadius,
																 &funnelTopCtrPos,
																 &funnelDirection);
	pCylinder->m_bcOpenBtm = true;
	pCylinder->m_bdryDistDumpingRate	= 0;
	pCylinder->m_bdryEnergyDumpingRate	= 0;
	pCylinder->m_bdryFrictionRate		= bdryFriction;
	pCylinder->m_bdryStressDumpingRate	= 0;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pCylinder);

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&funnelTopCtrPos);
	squareCtrPos[Y] -= funnelTopToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	pFloor->m_bdryDistDumpingRate	= 0;
	pFloor->m_bdryEnergyDumpingRate	= 0;
	pFloor->m_bdryFrictionRate		= bdryFriction;
	pFloor->m_bdryStressDumpingRate	= 0;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidbaroverrod() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D barCtrPos; barCtrPos.SetElements(0, 50, 0);
	CVector3D barDirection; barDirection.SetElements(1, 0, 0);
	CVector3D rodDirection; rodDirection.SetElements(0, 0, 1);
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	SimuValue rodLength = 300;
	CSimuManager::m_prtlPosJitter = 0.01;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	int numResampleSteps = 2;

	int barLayers[3] = {6, 8, 25};
	CSimuManager::m_heatConductivity = 300;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_minShearModulus = pow(10.0f, 4.0f);
	CSimuManager::m_maxShearModulus = pow(10.0f, 5.5f);
	CSimuManager::m_minTemperature = 10; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 400; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_onlyGravityTime = 1.7;
	SimuValue distToRod = 150;
	SimuValue rodRadius = 40;
	SimuValue distToFloor = 500;

	CPrtlFluid* fluidBar = CSimuManager::CreatePrtlFluid();
	fluidBar->m_deiName = "fluid_bar";
	fluidBar->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBar);

	CVector3D btmCtrPos;
	btmCtrPos.SetValue(&barCtrPos);
	btmCtrPos.AddedBy(&barDirection, -0.5*CSimuManager::m_prtlDistance*(barLayers[2]-1));
	CBoxOnXYPlane::CreatePrtlsInBox(fluidBar, &btmCtrPos, &barDirection, 0, 0,
									barLayers, CSimuManager::m_prtlDistance);
	fluidBar->PreparePrtlEvolution();

	// create thermal rod
	CVector3D todTopCtrPos;
	todTopCtrPos.SetValue(&barCtrPos);
	todTopCtrPos[Y] -= distToRod + rodRadius + 0.5*CSimuManager::m_prtlDistance*(barLayers[Y]-1);
	todTopCtrPos.AddedBy(&rodDirection, 0.5*rodLength);
	CBdrySolidRod* pRod = new CBdrySolidRod(rodLength, rodRadius,
										   0, false, false, &todTopCtrPos, &rodDirection);
	pRod->m_bdryThermalBody = true;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRod);

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&barCtrPos);
	squareCtrPos[Y] -= distToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
	CSimuManager::m_maxDownsamplePauseSteps = numResampleSteps;
	CSimuManager::m_maxUpsamplePauseSteps = numResampleSteps;
}

void CSimuModeler::OnFluidmodelFluidcylinderoverrod() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = false;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D rodDirection; rodDirection.SetElements(0, 0, 1);
	CVector3D cylDirection; cylDirection.SetElements(1, 0, 0);
	SimuValue rodLength = 300;
	SimuValue rodToFloor = 200;
	CSimuManager::m_prtlPosJitter = 0.01;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	int numResampleSteps = 2;
	CSimuManager::m_airPressure = -500;
	CSimuManager::m_minTemperature = 10; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 400; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;

	CSimuManager::m_heatConductivity = 800;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_minShearModulus = pow(10.0f, 5);
	CSimuManager::m_maxShearModulus = 6*pow(10.0f, 5);
	SimuValue rodRadius = 40;
	SimuValue cylinderLength = 280;
	SimuValue cylinderRadius = 30;
	SimuValue cylinderCapHeight = 0;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.2;
	CSimuManager::m_onlyGravityTime = 1.38;
	SimuValue distToRod = 100;

//	SimuValue rodFriction = 0;
//	CSimuManager::m_rotationFactor = 1;
//	SimuValue rodEnergyRate = 1;
	SimuValue shiftOnX = 0;

	CPrtlFluid* fluidCylinder = CSimuManager::CreatePrtlFluid();
	fluidCylinder->m_deiName = "fluid_cylinder";
	fluidCylinder->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidCylinder);

	CVector3D cylTopCtrPos;
	cylTopCtrPos.SetValue(&ctrPos);
	cylTopCtrPos.AddedBy(&cylDirection, 0.5*cylinderLength);
	cylTopCtrPos[X] += shiftOnX;
	cylTopCtrPos[Y] += rodRadius + distToRod + cylinderRadius;
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(cylinderLength, cylinderRadius,
															 cylinderCapHeight, true, true,
															 &cylTopCtrPos, &cylDirection);
	pCylinder->CreateFluidPrtlsInCylinder(fluidCylinder);
	delete pCylinder;
	fluidCylinder->PreparePrtlEvolution();

	// create rod
	CVector3D todTopCtrPos;
	todTopCtrPos.SetValue(&ctrPos);
	todTopCtrPos.AddedBy(&rodDirection, 0.5*rodLength);
	CBdrySolidRod* pRod = new CBdrySolidRod(rodLength, rodRadius,
										   0, false, false, &todTopCtrPos, &rodDirection);
//	pRod->m_bdryFrictionRate = rodFriction;
//	pRod->m_bdryEnergyDumpingRate = rodEnergyRate;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRod);

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= rodToFloor + rodRadius;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
	CSimuManager::m_maxDownsamplePauseSteps = numResampleSteps;
	CSimuManager::m_maxUpsamplePauseSteps = numResampleSteps;
}

void CSimuModeler::OnRotatefluidBoxonrotateplate() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = false;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D plateDirection; plateDirection.SetElements(0, 1, 0); plateDirection.Normalize();
	CVector3D boxDirection; boxDirection.SetElements(0, 0, 1); boxDirection.Normalize();
	CSimuManager::m_minTemperature = 100; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 500; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue plateHeight = 30;
	SimuValue plateToFloor = 50;
	SimuValue distToDisk = 25;
	CSimuManager::m_onlyGravityTime = 0.6;

	CSimuManager::m_heatConductivity = 2000;
	CSimuManager::m_minShearModulus = 5*pow(10.0f, 4);
	CSimuManager::m_maxShearModulus = 4*pow(10.0f, 5);
	SimuValue plateRadius = 300;
	SimuValue rotateSpeed = -72;
	SimuValue offCtrPlusX = 20;
	SimuValue offCtrPlusZ = 0;
	int boxLayers[3] = {9, 24, 9};
	CSimuManager::m_airPressure = -100;

	CPrtlFluid* fluidBox = CSimuManager::CreatePrtlFluid();
	fluidBox->m_deiName = "fluid_box";
	fluidBox->m_pfSimuManager = m_pSimuManager;
	fluidBox->m_pfShearModulus = CSimuManager::m_shearModulus;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(fluidBox);

	CVector3D btmCtrPos;
	btmCtrPos.SetValue(&ctrPos);
	btmCtrPos[X] += offCtrPlusX;
	btmCtrPos[Y] += distToDisk + 0.5*(boxLayers[Y]-1)*CSimuManager::m_prtlDistance;
	btmCtrPos[Z] += offCtrPlusZ - 0.5*(boxLayers[Z]-1)*CSimuManager::m_prtlDistance;
	CBoxOnXYPlane::CreatePrtlsInBox(fluidBox, &btmCtrPos, &boxDirection, 0, 0,
									boxLayers, CSimuManager::m_prtlDistance);
	fluidBox->PreparePrtlEvolution();

	// create plate
	CBdrySolidRod* pPlate = new CBdrySolidRod(plateHeight, plateRadius, 
											0, false, false, &ctrPos, &plateDirection);
	pPlate->m_bdryAngVel = rotateSpeed;
	pPlate->m_bdryThermalBody = true;
	pPlate->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
	// create pole supporting hot plate
	SimuValue poleRadius = 20;
	CVector3D poleTopCtrPos;
	poleTopCtrPos.SetValue(&ctrPos);
	poleTopCtrPos[Y] -= plateHeight;
	CBdrySolidRod* pRod = new CBdrySolidRod(plateToFloor-plateHeight, poleRadius,
											0, false, false, &poleTopCtrPos, &plateDirection);
	pRod->m_bdryThermalBody = false;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRod);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection;
	floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= plateToFloor + 0.1;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidoverhotring() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D ringDirection; ringDirection.SetElements(0, 1, 0); ringDirection.Normalize();
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();

	CSimuManager::m_minTemperature = 10; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue distToRing = 150;
	CSimuManager::m_onlyGravityTime = 1.6;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.2;
	SimuValue ringCtrToFloor = 200;

	CSimuManager::m_heatConductivity = 500; 
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_minShearModulus = pow(10.0f, 4);
	CSimuManager::m_maxShearModulus = 5*pow(10.0f, 5);
	SimuValue ringOuterRadius = 90;
	SimuValue ringInnerRadius = 25;
	SimuValue ballRadius = 60;
	CSimuManager::m_maxTemperature = 500; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;

	CPrtlFluid* ballFluid = CSimuManager::CreatePrtlFluid();
	ballFluid->m_deiName = "fluid_ball";
	ballFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(ballFluid);

	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);
	ballCtrPos[Y] += distToRing + ballRadius;
	CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
											  0, &ballCtrPos, &floorDirection);
	prtlInOval->CreatePrtlInOval(ballFluid, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;
	ballFluid->PreparePrtlEvolution();

	// create hot ring
	CBdrySolidTorus* pRing = new CBdrySolidTorus(ringInnerRadius, ringOuterRadius,
												&ctrPos, &ringDirection);
	pRing->m_bdryThermalBody = true;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRing);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= ringCtrToFloor;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidjumpingonslope() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CSimuManager::NON_NEWTONIAN_FLUID = true;
	CVector3D slopeTopCtrPos; slopeTopCtrPos.SetElements(0, -50, 0);
	CVector3D rodDirection; rodDirection.SetElements(0, 1, 0); rodDirection.Normalize();
	CSimuManager::m_minTemperature = 100;
	CSimuManager::m_maxTemperature = 500;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	SimuValue slopeDistToFloor = 200;
	SimuValue slopeDimX = 300; 
	SimuValue slopeDimY = 300; 
	SimuValue slopeDimZ = 30; 
	SimuValue fluidRadius = 30;
	SimuValue fluidLength = 240;
	SimuValue distToSlope = 100;

	CSimuManager::m_heatConductivity = 200;
	CSimuManager::m_minShearModulus = pow(10.0f, 3);
	CSimuManager::m_maxShearModulus = 4*pow(10.0f, 5);
	CSimuManager::m_airPressure = -400;
	CSimuManager::m_onlyGravityTime = 1.15;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_prtlIniTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_bdryTemperature = CSimuManager::m_minTemperature;
	CVector3D slopeDirection; slopeDirection.SetElements(0, 1, 1); slopeDirection.Normalize();

	SimuValue slopeRatio = slopeDirection[Z]/slopeDirection[Y]; // tagent value
	SimuValue slopeCosine = sqrt(1/(1+pow(slopeRatio, 2)));
	SimuValue slopeSine = sqrt(1-1/(1+pow(slopeRatio, 2)));

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "pull_up_fluid";
	pFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	CVector3D fluidTopCtrPos;
	fluidTopCtrPos.SetValue(&slopeTopCtrPos);
	fluidTopCtrPos[Y] += distToSlope;
	fluidTopCtrPos[Y] += 0.25*slopeDimY*slopeSine;
	fluidTopCtrPos[Z] -= 0.25*slopeDimY*slopeCosine;
	fluidTopCtrPos.AddedBy(&rodDirection, fluidLength);
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(fluidLength, fluidRadius,
															 0, false, false,
															 &fluidTopCtrPos, &rodDirection);
	pCylinder->CreateFluidPrtlsInCylinder(pFluid);
	delete pCylinder;

	// create slope
	CVector3D slopeBtmCtrPos;
	slopeBtmCtrPos.SetValue(&slopeTopCtrPos);
	slopeBtmCtrPos.AddedBy(&slopeDirection, -slopeDimZ);
	CBdrySolidBox* pSlope = new CBdrySolidBox(slopeDimZ, slopeDimX, slopeDimY,
											0, &slopeBtmCtrPos, &slopeDirection);
	pSlope->m_bdryThermalBody = true;
	pSlope->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&slopeBtmCtrPos);
	squareCtrPos.AddedBy(&slopeDirection, slopeDimZ);
	squareCtrPos[Y] -= 0.5*slopeDimY*slopeRatio;
	squareCtrPos[Y] -= slopeDistToFloor;
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

	pFluid->FixPrtlMotionWithBoundaries();
	pFluid->PreparePrtlEvolution();
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnFluidmodelFluidwallhitbyhotball() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D cubeCtrPos; cubeCtrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(0, 0, 1); cubeDirection.Normalize();
	CSimuManager::m_minTemperature = 100; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 700; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_subThermalSteps = 3;
	CSimuManager::m_lowerPrtlMassLimitRatio = 0.2;
	SimuValue distToWall = 100;

	CSimuManager::m_heatConductivity = 50000;
	CSimuManager::m_numStepDivPerFrame = 4;
	CSimuManager::m_minShearModulus = pow(10.0f, 1);
	CSimuManager::m_maxShearModulus = 5*pow(10.0f, 6);
	int cubeLayers[3] = {5, 25, 10}; // layers in standard pose on XY plane
	SimuValue ballRadius = 25;
	SimuValue ballMass = 5*pow(10.0f, 4);
	SimuValue ballSpeed = 300;
	CSimuManager::m_onlyBdryMovingTime = 0.32;
	SimuValue vertShift = 20;

	vertShift += 0.5*fabs(CSimuManager::GRAVITY->v[Y])*pow(distToWall/ballSpeed, 2);

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "ball_hit_wall";
	pFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	SimuValue cubeLenX = (cubeLayers[X]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenY = (cubeLayers[Y]-1)*CSimuManager::m_prtlDistance;
	SimuValue cubeLenZ = (cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	// create prtls in cube
	CVector3D cubeBtmPos;
	cubeBtmPos.SetValue(&cubeCtrPos);
	cubeBtmPos.AddedBy(&cubeDirection, -cubeLenZ*0.5);
	SimuValue angleAboutZ = 0;
	CBoxOnXYPlane::CreatePrtlsInBox(pFluid, &cubeBtmPos, &cubeDirection, angleAboutZ, 0,
									cubeLayers, CSimuManager::m_prtlDistance);
	pFluid->PreparePrtlEvolution();

	// create hot ball
	CVector3D ballIniVel; ballIniVel.SetElements(ballSpeed, 0, 0);
	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&cubeCtrPos);
	ballCtrPos[X] -= cubeLenX*0.5 + distToWall + ballRadius;
	ballCtrPos[Y] += vertShift;
	CRigidBall* pBall = new CRigidBall(ballMass, ballRadius, &ballCtrPos);
	pBall->m_rbFixed = false;
	pBall->m_bdryVel.SetValue(&ballIniVel);
	pBall->m_bdryThermalBody = true;
	pBall->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBall);
	// create floor
	SimuValue floorSideLen = 10000;
	CVector3D floorCtrPos;
	floorCtrPos.SetValue(&cubeCtrPos);
	floorCtrPos[Y] += -cubeLenY*0.5;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0); floorDirection.Normalize();
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &floorCtrPos,
														floorSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRotatefluidFallondeclinedrotateplate() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, -50, 0);
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_minTemperature = 100; CSimuManager::m_prtlIniTemperature = CSimuManager::m_minTemperature;
	CSimuManager::m_maxTemperature = 500; CSimuManager::m_bdryTemperature = CSimuManager::m_maxTemperature;
	CSimuManager::m_airPressure = -500;
	SimuValue plateRadius = 250;
	SimuValue plateHeight = 30;
	SimuValue plateToFloor = 50;
	SimuValue offCtrPlusZ = 0;

	CSimuManager::m_heatConductivity = 1000; 
	CSimuManager::m_minShearModulus = 5*pow(10.0f, 4);
	CSimuManager::m_maxShearModulus = 5*pow(10.0f, 6);
	CSimuManager::m_numStepDivPerFrame = 4;
	SimuValue rotateSpeed = -60;
	SimuValue offCtrPlusX = -150;
	SimuValue distToPlate = 25;
	CSimuManager::m_onlyGravityTime = 0;
	SimuValue ballRadius = 50;
	CVector3D plateDirection; plateDirection.SetElements(0, 1, 1); plateDirection.Normalize();

	SimuValue slopeRatio = plateDirection[Z]/plateDirection[Y]; // tagent value
	SimuValue slopeCosine = sqrt(1/(1+pow(slopeRatio, 2)));
	SimuValue slopeSine = sqrt(1-1/(1+pow(slopeRatio, 2)));

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "rotate_hot_plate";
	pFluid->m_pfSimuManager = m_pSimuManager;
	pFluid->m_pfShearModulus = CSimuManager::m_shearModulus;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	CVector3D ballCtrPos;
	ballCtrPos.SetValue(&ctrPos);
	ballCtrPos[X] += offCtrPlusX;
	ballCtrPos[Y] += distToPlate + ballRadius;
	ballCtrPos[Y] -= offCtrPlusZ*slopeRatio;
	ballCtrPos[Z] += offCtrPlusZ;
	CPrtlInOval* prtlInOval = new CPrtlInOval(ballRadius, ballRadius, ballRadius,
											  0, &ballCtrPos, &plateDirection);
	prtlInOval->CreatePrtlInOval(pFluid, CSimuManager::m_prtlDistance*0.5, 0);
	delete prtlInOval;
	pFluid->PreparePrtlEvolution();

	// create plate
	CBdrySolidRod* pPlate = new CBdrySolidRod(plateHeight, plateRadius, 
											0, false, false, &ctrPos, &plateDirection);
	pPlate->m_bdryAngVel = rotateSpeed;
	pPlate->m_bdryThermalBody = true;
	pPlate->m_bdryTemperature = CSimuManager::m_bdryTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= plateRadius*slopeSine;
	squareCtrPos[Y] -= plateToFloor;
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelTwocollidingbars() 
{
	// TODO: Add your command handler code here
	
	CSimuManager::HEAT_TRANSFER = false;
	CVector3D ctrPos; ctrPos.SetElements(0, -50, 0);
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_prtlPosJitter = 0.01;
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue distRodTopToFloor = 250;
	SimuValue barRadius = 30;
	SimuValue rodRadius = 30;
	SimuValue rodLength = 300;
	SimuValue btmBarIniTemperature = 200;
	SimuValue distBtmBarToRod = 20;
	SimuValue btmBarGraivityTime = 0;//0.6;
	SimuValue topBarGraivityTime = 0;//1.1;
	SimuValue distBetweenBars = 40;

	CSimuManager::m_heatConductivity = 1000;
	SimuValue topMaxShearModulus = pow(10.0f, 4);
	CSimuManager::m_minShearModulus = pow(10.0f, 3);
	CSimuManager::m_maxShearModulus = pow(10.0f, 8);
	CSimuManager::m_airPressure = -200;
	SimuValue topBarCoeffTemperature = 0.0015;
	SimuValue topBarIniTemperature = 2500;
	SimuValue rodDistance = 80;
	SimuValue topBarLength = 80;
	SimuValue btmBarLength = 200;

	CPrtlFluid* pFluidTop = CSimuManager::CreatePrtlFluid();
	pFluidTop->m_deiName = "top_bar";
	pFluidTop->m_pfSimuManager = m_pSimuManager;
	pFluidTop->m_pfOnlyGravityTime = topBarGraivityTime;
	pFluidTop->m_pfIniTemperature = topBarIniTemperature;
	pFluidTop->m_pfMaxShearModulus = topMaxShearModulus;
	pFluidTop->m_pfCoeffTemperature = topBarCoeffTemperature;
	pFluidTop->m_pfLinrTempTxtr.m_lttMaxTemperature = 1500;
	pFluidTop->m_pfLinrTempTxtr.m_lttMinTemperature = 1000;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluidTop);

	CPrtlFluid* pFluidBtm = CSimuManager::CreatePrtlFluid();
	pFluidBtm->m_deiName = "btm_bar";
	pFluidBtm->m_pfSimuManager = m_pSimuManager;
	pFluidBtm->m_pfOnlyGravityTime = btmBarGraivityTime;
	pFluidBtm->m_pfIniTemperature = btmBarIniTemperature;
	pFluidBtm->m_pfLinrTempTxtr.m_lttMinTemperature = 200;
	pFluidBtm->m_pfLinrTempTxtr.m_lttMaxTemperature = 500;
	pFluidBtm->m_pbApplyGravity = false;
	pFluidBtm->m_pfDensity = 1;
	pFluidBtm->m_pfShearModulus = 4000000;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluidBtm);

	CVector3D topBarDirection; topBarDirection.SetElements(0, 0, 1); topBarDirection.Normalize();
	CVector3D btmBarDirection; btmBarDirection.SetElements(1, 0, 0); btmBarDirection.Normalize();
	CVector3D barTopCtrPos;
	barTopCtrPos.SetValue(&ctrPos);
	barTopCtrPos[X] += 0.5*btmBarLength;
	barTopCtrPos[Y] += distBtmBarToRod + rodRadius + barRadius;
	CCylinderAlongZAxis* pBtmBar = new CCylinderAlongZAxis(btmBarLength, barRadius,
														0, false, false,
														&barTopCtrPos, &btmBarDirection);
	pBtmBar->CreateFluidPrtlsInCylinder(pFluidBtm);
	delete pBtmBar;
	pFluidBtm->PreparePrtlEvolution();

	barTopCtrPos.SetValue(&ctrPos);
	barTopCtrPos[Z] += 0.5*topBarLength;
	barTopCtrPos[Y] += distBtmBarToRod + rodRadius + barRadius*2 + distBetweenBars + barRadius;
	CCylinderAlongZAxis* pTopBar = new CCylinderAlongZAxis(topBarLength, barRadius,
															0, false, false,
															&barTopCtrPos, &topBarDirection);
	pTopBar->CreateFluidPrtlsInCylinder(pFluidTop);
	delete pTopBar;
	pFluidTop->PreparePrtlEvolution();
	if (!CSimuManager::HEAT_TRANSFER)
	{
		pFluidTop->m_pfColor.SetValue(CSimuManager::COLOR_RED);
		pFluidTop->m_pfSrfcColor.SetValue(CSimuManager::COLOR_RED);
		pFluidBtm->m_pfColor.SetValue(CSimuManager::COLOR_GREEN);
		pFluidBtm->m_pfSrfcColor.SetValue(CSimuManager::COLOR_GREEN);
	}

	// create two parallel rods
	CVector3D rodDirection; rodDirection.SetElements(0, 0, 1); rodDirection.Normalize();
	CVector3D rodTopCtrPos;
	rodTopCtrPos.SetValue(&ctrPos);
	rodTopCtrPos[X] -= 0.5*rodDistance + rodRadius;
	rodTopCtrPos[Z] += 0.5*rodLength;
	CBdrySolidRod* pRodLeft = new CBdrySolidRod(rodLength, rodRadius, 
												0, false, false, &rodTopCtrPos, &rodDirection);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRodLeft);
	rodTopCtrPos.SetValue(&ctrPos);
	rodTopCtrPos[X] += 0.5*rodDistance + rodRadius;
	rodTopCtrPos[Z] += 0.5*rodLength;
	CBdrySolidRod* pRodRight = new CBdrySolidRod(rodLength, rodRadius, 
												0, false, false, &rodTopCtrPos, &rodDirection);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pRodRight);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= distRodTopToFloor - rodRadius;
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnIntermodelEggontohotplate() 
{
	// TODO: Add your command handler code here
#if 1
	CSimuManager::m_bStickyInterFluids = true;
	CSimuManager::HEAT_TRANSFER = true;
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	SimuValue slopeDimX = 60; 
	SimuValue slopeDimZ = 60; 
	SimuValue slopeDimY = 60;
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_framesPerResample = 2;
	CSimuManager::m_numStepDivPerFrame = 3;
	CSimuManager::m_maxShearModulus = pow(10.0f, 6);
	SimuValue eggConstSummation = 0;
	SimuValue eggCoeffTemperature = -0.01;
	SimuValue airPressure = -300;
	SimuValue yolkPrtlMassRatio = 50;
	SimuValue yolkIniTemperature = 0;
	SimuValue distEggToPlate = 130;
	CSimuManager::m_onlyGravityTime = 1.35;
	CSimuManager::m_rotationFactor	= 1;

	SimuValue eggConductivity = 1000;
	SimuValue yolkConductivity = 1000;
	SimuValue slopeFriction = 1;
	SimuValue eggRadius = 85;
	SimuValue yolkRatio = 0.55;
	SimuValue plateTemperature = 2000;
	SimuValue edgeTemperature = 2000;
	SimuValue yolkConstSummation = 4;
	SimuValue yolkCoeffTemperature = -0.003;
	SimuValue eggIniTemperature = -1000;
	SimuValue yolkMinShearModulus = pow(10.0f, 4);
	CVector3D slopeDirection; slopeDirection.SetElements(0, 1, 0); slopeDirection.Normalize();
	SimuValue slopeDistToFloor = 200;
	CSimuManager::m_interFluidStickyness = pow(10.0f,-3);

	SimuValue slopeRatio = slopeDirection[Z]/slopeDirection[Y]; // tagent value
	SimuValue slopeCosine = sqrt(1/(1+pow(slopeRatio, 2)));
	SimuValue slopeSine = sqrt(1-1/(1+pow(slopeRatio, 2)));

	CPrtlFluid* eggWhite = CSimuManager::CreatePrtlFluid();
	eggWhite->m_deiName = "eggwhite";
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(eggWhite);
	eggWhite->m_pfSimuManager = m_pSimuManager;
	eggWhite->m_pfAirPressure = airPressure;
	eggWhite->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	eggWhite->m_pfHeatConductivity = eggConductivity;
	eggWhite->m_pfIniTemperature = eggIniTemperature;
	eggWhite->m_pfCoeffTemperature = eggCoeffTemperature;
	eggWhite->m_pfConstSummation = eggConstSummation;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorR = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorG = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttConstColorB = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorR = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorG = 0;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioColorB = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttConstFilter = 0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttRatioFilter = -0.95;
	eggWhite->m_pfLinrTempTxtr.m_lttMinTemperature = 200;
	eggWhite->m_pfLinrTempTxtr.m_lttMaxTemperature = 700;

	CVector3D eggCtrPos; eggCtrPos.SetValue(&ctrPos);
	eggCtrPos[Y] += distEggToPlate + eggRadius;
	CVector3D eggIniVel; eggIniVel.SetElements(0, 0, 0);
	CPrtlFluid* yolk = CreateEggWithYolk(eggWhite, 0, &(eggCtrPos), &(eggIniVel),
										eggRadius, yolkRatio, 
										CSimuManager::m_airPressure, pow(10.0f, 5),
										CSimuManager::m_relaxationTime, yolkPrtlMassRatio);
	yolk->m_pfAirPressure = airPressure;
	yolk->m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;
	yolk->m_pfHeatConductivity = yolkConductivity;
	yolk->m_pfMinShearModulus = yolkMinShearModulus;
	yolk->m_pfIniTemperature = yolkIniTemperature;
	yolk->m_pfCoeffTemperature = yolkCoeffTemperature;
	yolk->m_pfConstSummation = yolkConstSummation;
	yolk->m_pfLinrTempTxtr.m_lttConstColorR = 0.95;
	yolk->m_pfLinrTempTxtr.m_lttConstColorG = 0;
	yolk->m_pfLinrTempTxtr.m_lttConstColorB = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorR = -0.95;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorG = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioColorB = 0.95;
	yolk->m_pfLinrTempTxtr.m_lttConstFilter = 0;
	yolk->m_pfLinrTempTxtr.m_lttRatioFilter = 0;
	yolk->m_pfLinrTempTxtr.m_lttMinTemperature = 500;
	yolk->m_pfLinrTempTxtr.m_lttMaxTemperature = 700;

	eggWhite->PreparePrtlEvolution();
	yolk->PreparePrtlEvolution();

	// create hot plate
	CVector3D slopeBtmCtrPos;
	slopeBtmCtrPos.SetValue(&ctrPos);
	slopeBtmCtrPos.AddedBy(&slopeDirection, -slopeDimZ);
	CBdrySolidBox* pSlope = new CBdrySolidBox(slopeDimZ, slopeDimX, slopeDimY,
											0, &slopeBtmCtrPos, &slopeDirection);
	pSlope->m_bdryFrictionRate = slopeFriction;
	pSlope->m_bdryThermalBody = true;
	pSlope->m_bdryTemperature = plateTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope);

/*
	//plate 2
	ctrPos.SetElements(-35, 90, -35);

	slopeBtmCtrPos.SetValue(&ctrPos);
	slopeDirection.SetElements(.3, 1, .3); slopeDirection.Normalize();
	


	slopeBtmCtrPos.AddedBy(&slopeDirection, -slopeDimZ);
	CBdrySolidBox* pSlope2 = new CBdrySolidBox(slopeDimZ, slopeDimX, slopeDimY,
											0, &slopeBtmCtrPos, &slopeDirection);
	pSlope->m_bdryFrictionRate = slopeFriction;
	pSlope->m_bdryThermalBody = true;
	pSlope->m_bdryTemperature = plateTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope2);
	ctrPos.SetElements(0, 0, 0);
	
	//plate 3

	ctrPos.SetElements(35, 120, 35);

	slopeBtmCtrPos.SetValue(&ctrPos);
	slopeDirection.SetElements(-0.3, 1, -0.3); slopeDirection.Normalize();
	


	slopeBtmCtrPos.AddedBy(&slopeDirection, -slopeDimZ);
	CBdrySolidBox* pSlope3 = new CBdrySolidBox(slopeDimZ, slopeDimX, slopeDimY,
											0, &slopeBtmCtrPos, &slopeDirection);
	pSlope->m_bdryFrictionRate = slopeFriction;
	pSlope->m_bdryThermalBody = true;
	pSlope->m_bdryTemperature = plateTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pSlope3);
	ctrPos.SetElements(0, 0, 0);

*/

/*
	// create hot plate edge
	CVector3D edgeDirection; edgeDirection.SetElements(1, 0, 0); edgeDirection.Normalize();
	CVector3D rodTopCtrPos;
	rodTopCtrPos.SetValue(&ctrPos);
	rodTopCtrPos.AddedBy(&slopeDirection, -0.5*slopeDimZ);
	rodTopCtrPos[X] += 0.5*slopeDimX;
	rodTopCtrPos[Y] -= 0.5*slopeDimY*slopeSine;
	rodTopCtrPos[Z] += 0.5*slopeDimY*slopeCosine;
	CBdrySolidRod* pEdge = new CBdrySolidRod(slopeDimX, 0.5*slopeDimZ, 0,
											false, false, &rodTopCtrPos, &edgeDirection);
	pEdge->m_bdryFrictionRate = slopeFriction;
	pEdge->m_bdryThermalBody = true;
	pEdge->m_bdryTemperature = edgeTemperature;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pEdge);
*/
	

	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&slopeBtmCtrPos);
	squareCtrPos.AddedBy(&slopeDirection, slopeDimZ);
	squareCtrPos[Y] -= 0.5*slopeDimY*slopeSine;
	squareCtrPos[Y] -= slopeDistToFloor;
	CBdrySquareSlope* bdryFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(bdryFloor);

#endif
	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}

void CSimuModeler::OnRotatefluidBoxtwistsfluid() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D cubeDirection; cubeDirection.SetElements(0, 1, 0); cubeDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_airPressure = -500;
	SimuValue boxMargin = 5;
	CSimuManager::NOT_COUNT_FIXED_PRTLS = false;

	CSimuManager::m_maxShearModulus = pow(10.0f, 5);
	CSimuManager::m_numStepDivPerFrame = 4;
	SimuValue rotateSpeed = 15;
	SimuValue pullingSpeed = 20;
	int cubeLayers[3] = {12, 12, 8};
	int numResampleSteps = 4;

	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);
	pFluid->m_deiName = "fluid_cube";
	pFluid->m_pfSimuManager = m_pSimuManager;

	CVector3D cubeBtmCtrPos;
	cubeBtmCtrPos.SetValue(&ctrPos);
	cubeBtmCtrPos[Y] -= (cubeLayers[Z]-1)*CSimuManager::m_prtlDistance;
	CBoxOnXYPlane::CreatePrtlsInBox(pFluid, &cubeBtmCtrPos, &cubeDirection,
									0, 0, cubeLayers, CSimuManager::m_prtlDistance);
	// create top box
	SimuValue boxDimX = boxMargin*2 + (cubeLayers[X]-1)*CSimuManager::m_prtlDistance;
	SimuValue boxDimY = boxMargin*2 + (cubeLayers[Y]-1)*CSimuManager::m_prtlDistance;
	SimuValue boxDimZ = boxMargin*2 + 2*CSimuManager::m_prtlDistance;
	CVector3D boxBtmCtrPos;
	boxBtmCtrPos.SetValue(&ctrPos);
	boxBtmCtrPos[Y] -= boxDimZ - boxMargin;
	CBdrySolidBox* pBox = new CBdrySolidBox(boxDimZ, boxDimX, boxDimY, 0,
											&boxBtmCtrPos, &cubeDirection);
	pBox->m_bdryAngVel = rotateSpeed;
	pBox->m_bdryVel.SetElements(0, 1, 0);
	pBox->m_bdryVel.ScaledBy(pullingSpeed);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pBox);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection; floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&cubeBtmCtrPos);
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
													squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	pFluid->FixPrtlMotionWithBoundaries();
	pFluid->PreparePrtlEvolution();
	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
	CSimuManager::m_maxDownsamplePauseSteps = numResampleSteps;
	CSimuManager::m_maxUpsamplePauseSteps = numResampleSteps;
}

void CSimuModeler::OnRotatefluidPoleonrotateplate() 
{
	// TODO: Add your command handler code here
	
	CVector3D ctrPos; ctrPos.SetElements(0, 0, 0);
	CVector3D poleDirection; poleDirection.SetElements(0, -1, 0); poleDirection.Normalize();
	CVector3D plateDirection; plateDirection.SetElements(0, 1, 0); plateDirection.Normalize();
	CSimuManager::m_eFluidModel = CSimuManager::UPSAMPLE; // SPH; // 
	CSimuManager::m_numStepDivPerFrame = 3;
	SimuValue plateHeight = 30;
	SimuValue plateToFloor = 50;

	CSimuManager::m_shearModulus = pow(10.0f, 0);
	SimuValue plateRadius = 300;
	SimuValue rotateSpeed = 0;
	SimuValue poleLength = 200;
	SimuValue poleRadius = 40;
	SimuValue poleOffset = 100;

	// create pole
	CPrtlFluid* pFluid = CSimuManager::CreatePrtlFluid();
	pFluid->m_deiName = "pole";
	pFluid->m_pfSimuManager = m_pSimuManager;
	m_pSimuManager->m_aryPrtlFluid.AppendOnePointer(pFluid);

	CVector3D topCtrPos;
	topCtrPos.SetValue(&ctrPos);
	topCtrPos[X] += poleOffset;
	CCylinderAlongZAxis* pCylinder = new CCylinderAlongZAxis(poleLength, poleRadius,
															 0, false, false,
															 &topCtrPos, &poleDirection);
	pCylinder->CreateFluidPrtlsInCylinder(pFluid);
	delete pCylinder;

	// create plate
	CBdrySolidRod* pPlate = new CBdrySolidRod(plateHeight, plateRadius, 
											0, false, false, &ctrPos, &plateDirection);
	pPlate->m_bdryAngVel = rotateSpeed;
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pPlate);
	// create floor
	SimuValue squareSideLen = 10000;
	CVector3D floorDirection;
	floorDirection.SetElements(0, 1, 0);
	CVector3D squareCtrPos;
	squareCtrPos.SetValue(&ctrPos);
	squareCtrPos[Y] -= plateToFloor + 0.1;
	CBdrySquareSlope* pFloor = new CBdrySquareSlope(&floorDirection, &squareCtrPos,
														squareSideLen, 0);
	m_pSimuManager->m_aryBoundaries.AppendOnePointer(pFloor);

	m_pSimuManager->m_pSimuMonitor->UpdateData(false);
	m_pSimuManager->InitializeOptions();
	m_pSimuManager->RefreshView();
}
