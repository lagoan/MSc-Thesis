// RigidBall.h: interface for the CRigidBall class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RIGIDBALL_H__B6F872EA_BA31_483B_AC21_1D5B56D4731A__INCLUDED_)
#define AFX_RIGIDBALL_H__B6F872EA_BA31_483B_AC21_1D5B56D4731A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BdrySolidOval.h"

class CRigidBall : public CBdrySolidOval  
{
public:
	static int m_rbMaxId;

	bool m_rbFixed;
	SimuValue m_rbMass;
	SimuValue m_rbRadius;
	SimuValue m_rbFrictionRate;
	SimuValue m_rbDistDumpingRate;
	SimuValue m_rbEnergyDumpingRate;

	CRigidBall();
	CRigidBall(SimuValue mass, SimuValue radius, CVector3D* ctrPos);
	virtual ~CRigidBall();

	void InitializeRigidBall();

	virtual void DrawBoundary();
	virtual void ExportBoundaryData(FILE* fp);
	virtual void ImportBoundaryData(FILE* fp);

	virtual void MoveBoundary(SimuValue curTime, SimuValue timeStep);
	virtual void EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid);
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);

	void EnforceBallMotionWithBoundary(CBoundary* pBdry);

protected:
	SimuValue m_rbRadiusSqre;
};

#endif // !defined(AFX_RIGIDBALL_H__B6F872EA_BA31_483B_AC21_1D5B56D4731A__INCLUDED_)
