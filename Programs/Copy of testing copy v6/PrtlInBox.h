// PrtlInBox.h: interface for the CPrtlInBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLINBOX_H__EA861CA1_E33B_42B9_B36B_0A0B846D2EDC__INCLUDED_)
#define AFX_PRTLINBOX_H__EA861CA1_E33B_42B9_B36B_0A0B846D2EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BoxOnXYPlane.h"
#include "PrtlFluid.h"

class CPrtlInBox : public CBoxOnXYPlane  
{
public:
	CPrtlInBox(SimuValue heightOnZ, SimuValue widthOnX, SimuValue widthOnY,
			   SimuValue angleAboutZ, CVector3D* topCtrPos, CVector3D* direction);
	virtual ~CPrtlInBox();

	void CreatePrtlsInBox(CPrtlFluid* pPrtlFluid);
};

#endif // !defined(AFX_PRTLINBOX_H__EA861CA1_E33B_42B9_B36B_0A0B846D2EDC__INCLUDED_)
