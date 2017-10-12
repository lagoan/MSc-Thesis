#if !defined(AFX_SIMUMODELER_H__8C8AA662_CE50_11D5_9B85_0050BADE013B__INCLUDED_)
#define AFX_SIMUMODELER_H__8C8AA662_CE50_11D5_9B85_0050BADE013B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SimuModeler.h : header file
//

#include "PrtlFluid.h"
#include "PrtlFluidUpsample.h"
#include "PrtlFluidSpring.h"

#include "BoxOnXYPlane.h"
#include "PrtlInOval.h"
#include "PrtlTriangleMesh.h"

#include "SimuBoundaryBox.h"
#include "SimuBoundaryTable.h"
#include "FluidNozzleShuttle.h"
#include "BdrySolidBox.h"
#include "BdrySolidRod.h"
#include "BdrySolidTorus.h"
#include "BdryBowl.h"
#include "BdrySquareSlope.h"
#include "BdryBouncingSlope.h"
#include "BdryHollowCone.h"

#include "Point3DOnEdge.h"
#include "ConvexHull.h"
#include "DelaunayTetrahedron.h"

/////////////////////////////////////////////////////////////////////////////
// CSimuModeler window

class CSimuManager;
class CSimuView;

class CSimuModeler : public CWnd
{
// Construction
public:
	CSimuModeler(CSimuManager* simuManager);

// Attributes
public:
	CSimuManager*	m_pSimuManager;

// Operations
public:
	CPrtlFluid* CreateEggWithYolk(CPrtlFluid* eggWhite, int num, 
								CVector3D* eggCtrPos, CVector3D* eggIniVel,
								SimuValue eggRadius, SimuValue yolkRatio,
								SimuValue yolkAirPressure, SimuValue yolkShearModulus,
								SimuValue yolkRelaxationTime, SimuValue yolkPrtlMassRatio);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSimuModeler)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSimuModeler();

private:
	CSimuView*		m_pSimuView;

	// Generated message map functions
protected:
	//{{AFX_MSG(CSimuModeler)
	afx_msg void OnFluidmodelFluidball();
	afx_msg void OnTestConvexhull();
	afx_msg void OnTestDelaunaytetrahedron();
	afx_msg void OnTestDistancetoconvexhull();
	afx_msg void OnTestUpsampledelaunaytetrahedron();
	afx_msg void OnTestDelaunaytessellation();
	afx_msg void OnTestUpsampledelaunaytessellation();
	afx_msg void OnTestPartialdelaunaytessellation();
	afx_msg void OnTestConvexhullongrid();
	afx_msg void OnTestDelaunaytetrahedronobject();
	afx_msg void OnFluidmodelNozzleinbox();
	afx_msg void OnRigidmodelBall();
	afx_msg void OnRigidmodelCylinder();
	afx_msg void OnRigidmodelCubeonslope();
	afx_msg void OnFluidmodelDropletintowaterinbowl();
	afx_msg void OnFluidmodelFluidverticalstretching();
	afx_msg void OnFluidmodelPullingupfluid();
	afx_msg void OnTestSolidboxboundary();
	afx_msg void OnFluidmodelBallshootfluidtower();
	afx_msg void OnRotatefluidRodclimbing();
	afx_msg void OnRotatefluidBallonrotatedisk();
	afx_msg void OnFluidmodelFluidballbyrandomprtls();
	afx_msg void OnIntermodelTwocollidingballs();
	afx_msg void OnIntermodelRodhittingdisk();
	afx_msg void OnIntermodelBallhittingdisk();
	afx_msg void OnIntermodelBoardcolliding();
	afx_msg void OnIntermodelBallhittingtower();
	afx_msg void OnIntermodelMultipleballs();
	afx_msg void OnFluidmodelFluidstretching();
	afx_msg void OnRotatefluidRotaterod();
	afx_msg void OnIntermodelCollidingcubes();
	afx_msg void OnIntermodelBallintojello();
	afx_msg void OnIntermodelEggwithyork();
	afx_msg void OnIntermodelMultipleeggs();
	afx_msg void OnTestSurfacetensiononcube();
	afx_msg void OnIntermodelEggdropintobowl();
	afx_msg void OnIntermodelEggthroughfunnel();
	afx_msg void OnFluidmodelFluidcube();
	afx_msg void OnFluidmodelFluidbaroverrod();
	afx_msg void OnFluidmodelFluidcylinderoverrod();
	afx_msg void OnRotatefluidBoxonrotateplate();
	afx_msg void OnFluidmodelFluidoverhotring();
	afx_msg void OnFluidmodelFluidjumpingonslope();
	afx_msg void OnFluidmodelFluidwallhitbyhotball();
	afx_msg void OnRotatefluidFallondeclinedrotateplate();
	afx_msg void OnIntermodelTwocollidingbars();
	afx_msg void OnIntermodelEggontohotplate();
	afx_msg void OnRotatefluidBoxtwistsfluid();
	afx_msg void OnIntermodelMultiplecollidingcubes();
	afx_msg void OnRotatefluidPoleonrotateplate();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SIMUMODELER_H__8C8AA662_CE50_11D5_9B85_0050BADE013B__INCLUDED_)
