// PrtlFluidUpsample.h: interface for the CPrtlFluidUpsample class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLFLUIDUPSAMPLE_H__5435086E_821B_4466_8051_713652D695B3__INCLUDED_)
#define AFX_PRTLFLUIDUPSAMPLE_H__5435086E_821B_4466_8051_713652D695B3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PrtlFluid.h"
#include "DelaunayTetrahedron.h"

class CPrtlFluidUpsample : public CPrtlFluid  
{
public:
	SimuValue m_pfuInitialAvgInsdPND;
	SimuValue m_pfuInitialAvgSrfcPND;
	SimuValue m_pfuInitialAvgInsdPNDGradLen;
	SimuValue m_pfuInitialAvgSrfcPNDGradLen;
	TGeomElemArray<CFluidPrtl> m_pfuSrfcPrtls;
	bool m_pfuSrfcCheckedByConvexHull;

	bool m_pfuDownSampled;
	SimuValue m_pfuLowerPrtlMassLimit;
	SimuValue m_pfuUpperPrtlMassLimit;
	SimuValue m_pfuMinPrtlDistRatio;
	SimuValue m_pfuDownsampleDistThrshd;
	TGeomElemArray<CFluidPrtl> m_pfuNewDownsampledPrtls;

	enum UPSAMPLE_POINT_STATUS
	{
		TOO_CLOSE_TO_EXISTING_PRTL,
		TOO_SMALL_MASS,
		OUTSIDE_FLUID_SURFACE,
		ON_FLUID_SURFACE,
		INSIDE_FLUID
	};
	SimuValue m_pfuSrfcThickness;
	SimuValue m_pfuUpsampleMinRadiusSqr;
	SimuValue m_pfuUpsampleMaxRadiusSqr;
	TGeomElemArray<CFluidPrtl> m_pfuUpsampledPrtls;
	CDelaunayTetrahedron m_pfuDTConstructor;
	bool m_pfuResampled;
	int m_pfuDownsamplePauseSteps;
	int m_pfuUpsamplePauseSteps;

	// debug variables
	SimuValue m_pfuMinPrtlMass;
	SimuValue m_pfuMaxPrtlMass;

	CPrtlFluidUpsample();
	virtual ~CPrtlFluidUpsample();

	virtual void RenderPrtlFluid();

	virtual void PreparePrtlEvolution();
	virtual void DetectSrfcPrtls();
	virtual void EvolveFluidParticles(SimuValue simuTime);
	virtual void ComposeSimuInfoAfterOneTimeStep(int numTimeSteps);

	void ComputeSrfcDetectionThresholdValues();
	bool IsPrtlStatusDefinite(CFluidPrtl* pPrtl, TGeomElemArray<CFluidPrtl> &srfcPrtls);
	bool HasAnySrfcPrtlsInNgbrhood(CVector3D* pos, SimuValue radius);
	void GetSrfcAndInsdPrtlNumbers(CFluidPrtl* pPrtl, int &numSrfc, int &numInsd);
	bool IsPosOnSrfcByItsConvexHull(CVector3D* pos, TGeomElemArray<CFluidPrtl> &ngbrPrtls);

	void GetMinAndMaxPrtlMasses();

	void DownsampleFluidParticles();
	CFluidPrtl* MergeTwoPrtls(CFluidPrtl* pPrtl0, CFluidPrtl* pPrtl1, bool bMergeIntoFirst);
	void UpsampleFluidParticles();
	bool IsPointBreakingFluidBoundaries(CPoint3D* upsampleP);
	bool IsPointBreakingFluidInterfaces(CPoint3D* upsampleP);
	int IsPointValidForUpsample(CPoint3D* upsampleP, TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	SimuValue ComputeMassAtPos(CVector3D* pPos, TGeomElemArray<CFluidPrtl> &ngbrPrtls);
	CFluidPrtl* AddingOnePrtlAtUpsamplePos(CPoint3D* upsampleP,
											TGeomElemArray<CFluidPrtl> &ngbrPrtls);

	TGeomElemArray<CFluidPrtl>* m_pfuPrtlNgbrs;
	CPoint3D* m_pfuHighlitPos;
	CPoint3D* m_pfuCircumSphCtr;
	SimuValue m_pfuCircumSphRSqr;
	bool m_pfuPause[6];
};

#endif // !defined(AFX_PRTLFLUIDUPSAMPLE_H__5435086E_821B_4466_8051_713652D695B3__INCLUDED_)
