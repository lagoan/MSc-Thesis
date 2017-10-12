// PrtlEdge.h: interface for the CPrtlEdge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLEDGE_H__0D55AA24_6AB2_4A3C_87E2_75977DF083B5__INCLUDED_)
#define AFX_PRTLEDGE_H__0D55AA24_6AB2_4A3C_87E2_75977DF083B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GeometryElement.h"

class CMeshPrtl;

class CPrtlEdge : public CGeometryElement  
{
public:
	CMeshPrtl* m_pePs[2];

	CPrtlEdge();
	virtual ~CPrtlEdge();

	virtual void RenderGeometry();

	bool HasEdgePrtl(CMeshPrtl* pPrtl);
	void ReplaceEdgePrtl(CMeshPrtl* oldPrtl, CMeshPrtl* newPrtl);
	CMeshPrtl* GetTheOtherMeshPrtl(CMeshPrtl* pPrtl);
	bool IsSameEdgeAs(CPrtlEdge* pEdge);
	SimuValue ComputeEdgeLength();
};

#endif // !defined(AFX_PRTLEDGE_H__0D55AA24_6AB2_4A3C_87E2_75977DF083B5__INCLUDED_)
