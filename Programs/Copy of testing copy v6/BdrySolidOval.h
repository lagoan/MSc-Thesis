// BdrySolidOval.h: interface for the CBdrySolidOval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BDRYSOLIDOVAL_H__B6AC3D57_6086_4D9A_AA95_9B40D37CC52C__INCLUDED_)
#define AFX_BDRYSOLIDOVAL_H__B6AC3D57_6086_4D9A_AA95_9B40D37CC52C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Boundary.h"
#include "OvalAlong3DAxes.h"

class CBdrySolidOval : public CBoundary, public COvalAlong3DAxes  
{
public:
	static int m_bsoMaxId;

	CBdrySolidOval();
	CBdrySolidOval(SimuValue radiusX, SimuValue radiusY, SimuValue radiusZ,
				   SimuValue angleAboutZ, CVector3D* ctrPos, CVector3D* direction);
	virtual ~CBdrySolidOval();

	void InitializeSolidOval();
};

#endif // !defined(AFX_BDRYSOLIDOVAL_H__B6AC3D57_6086_4D9A_AA95_9B40D37CC52C__INCLUDED_)
