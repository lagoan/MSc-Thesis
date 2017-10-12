// PrtlTriangle.h: interface for the CPrtlTriangle class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLTRIANGLE_H__727A23D2_6A04_4BE7_9A46_C8A368CB5798__INCLUDED_)
#define AFX_PRTLTRIANGLE_H__727A23D2_6A04_4BE7_9A46_C8A368CB5798__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"

class CMeshPrtl;

class CPrtlTriangle : public CGeometryElement  
{
public:
	CMeshPrtl* m_ptrPs[3];
	// use area normal instead of normal for the convenience to 
	// 1) proportionally contribute to particle normals
	// 2) compute enclosed volume by triangular mesh
	CVector3D m_ptrAreaNormal;
	int m_ptrTag;

	CPrtlTriangle();
	virtual ~CPrtlTriangle();

	virtual void RenderGeometry();

	virtual void AssertGeometricConnectivity();
	void AssertPrtlTriConnectivity(char location[]="CPrtlTriangle::AssertPrtlTriConnectivity()");
	virtual void GetElementPosition(CVector3D* pPos);

	void SetTriPrtls(CMeshPrtl* triP0, CMeshPrtl* triP1, CMeshPrtl* triP2);
	bool HasTriPrtl(CMeshPrtl* pPrtl);
	bool HasSameTriPrtlsAs(CPrtlTriangle* pTri);
	void ReplaceTriPrtl(CMeshPrtl* pOld, CMeshPrtl* pNew);
	bool SelfDeleteFromTriPrtls(char location[]);
	int GetThirdTriPrtlId(CMeshPrtl* triP0, CMeshPrtl* triP1);
	CMeshPrtl* GetThirdTriPrtl(CMeshPrtl* triP0, CMeshPrtl* triP1);
	CPrtlTriangle* GetNeighborTriangle(CMeshPrtl* triP0, CMeshPrtl* triP1);
	SimuValue ComputeAngleOnTriPrtl(CMeshPrtl* pPrtl);
	void ComputeTriAreaNormal();
	void ComputeTriAreaNormalByRefNormal(CVector3D* refNormal);
	void ComputeTriAreaNormalByRefPos(CVector3D* refPos, bool bInside);
	SimuValue GetTriangleArea();
	void ComputePrtlCurvatureNormalContribution(int idPrtl, SimuValue triArea,
												CVector3D &contributionVector);

	static SimuValue ComputeVolumeEnclosedByTriangles(TPointerArray<CPrtlTriangle> &srfcTris);
};

#endif // !defined(AFX_PRTLTRIANGLE_H__727A23D2_6A04_4BE7_9A46_C8A368CB5798__INCLUDED_)
