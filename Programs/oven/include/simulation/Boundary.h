// Boundary.h: interface for the CBoundary class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BOUNDARY_H__D7023D07_5168_48EF_8605_2C0C5495F1D3__INCLUDED_)
#define AFX_BOUNDARY_H__D7023D07_5168_48EF_8605_2C0C5495F1D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "DataExImport.h"
#include "VirtualPrtl.h"
#include "TGeomElemArray.h"

class CPrtlFluid;

class CBoundary  /*: public CDataExImport*/
{
public:
	static SimuValue m_bdryMaxClsSpeed;
	static TClassArray<CBoundary>* m_bdryBalls;
	static TClassArray<CBoundary>* m_bdryNotBalls;

	enum EBoundaryType
	{
		BOX,
		CYLINDER,
		SOLID_BOX,
		SOLID_ROD,
		SOLID_TORUS,
		NOZZLE,
		BOWL,
		SQUARE_SLOPE,
		TABLE,
		SOLID_OVAL,
		RIGID_BALL,
		HOLLOW_CONE,
	};
	EBoundaryType m_bdryType;

	SimuValue m_bdryDistDumpingRate;	// 0 means complete dumping
	SimuValue m_bdryEnergyDumpingRate;	// 0 means complete dumping
	SimuValue m_bdryFrictionRate;		// 0 means full stop friction
	SimuValue m_bdryStressDumpingRate;	// 0 means complete dumping

	bool m_bdryVisible;
	CVector3D m_bdryVel;
	SimuValue m_bdryAngVel;

	CVector3D m_bdryMinPos;
	CVector3D m_bdryMaxPos;
	TGeomElemArray<CFluidPrtl> m_bdryEnclosedPrtls;
	TClassArray<CVector3D> m_bdryOrgPrtlPos;

	bool m_bdryThermalBody;
	SimuValue m_bdryMaxHeatingDist;
	SimuValue m_bdryTemperature;

	CVector3D* m_bdryColor;

	CBoundary();
	virtual ~CBoundary();

	static void AddBoundary(CBoundary* boundary);
	static void MoveBoundaries(SimuValue curTime, SimuValue timeStep);
	static void DrawBoundaries();

	virtual void DrawBoundary();
	//virtual void ExportBoundaryData(FILE* fp);
	//virtual void ExportBoundaryParameters(FILE* fp);
	//virtual void ImportBoundaryData(FILE* fp);
	//virtual void ImportBoundaryParameters(FILE* fp);

	virtual void MoveBoundary(SimuValue curTime, SimuValue timeStep);
	virtual void EnforceBoundaryConstraint(CPrtlFluid* pPrtlFluid); 
	virtual bool EnforceBoundaryConstraint(CVirtualPrtl* pPrtl, SimuValue timeStep);
	virtual bool PrtlBreakBoundary(CVirtualPrtl* pPrtl);
	virtual bool PosBreakBoundary(CVector3D* pPos);
	virtual void AddEnclosedPrtl(CFluidPrtl* pPrtl);

	virtual SimuValue GetDistanceToOpenOutletPlane(CVirtualPrtl* pPrtl);
	virtual bool StretchPrtlOntoBoundary(CVirtualPrtl* pPrtl);

	void SetBdryParams(SimuValue bdryParams);
	void HeatTransferOnBdry(CPrtlFluid* pPrtlFluid);
	void HeatTransferBetweenPrtlAndBdry(CFluidPrtl* pPrtl, SimuValue conductivity,
										SimuValue smoothLen, SimuValue timeStep);
	virtual SimuValue GetDistanceBetweenPrtlAndBdry(CFluidPrtl* pPrtl);

	void ReflectPosOnPlane(CVector3D &pPos, CVector3D &planeNormal, CVector3D &intersectPos);
	bool ReflectVelOnPlane(CVector3D &pVel, CVector3D &planeNormal);
};

#endif // !defined(AFX_BOUNDARY_H__D7023D07_5168_48EF_8605_2C0C5495F1D3__INCLUDED_)
