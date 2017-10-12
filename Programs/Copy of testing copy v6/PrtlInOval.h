// PrtlInOval.h: interface for the CPrtlInOval class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRTLINOVAL_H__C70EC142_31AC_4C44_AACB_7F8E7B3B5274__INCLUDED_)
#define AFX_PRTLINOVAL_H__C70EC142_31AC_4C44_AACB_7F8E7B3B5274__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OvalAlong3DAxes.h"
#include "PrtlFluid.h"

class CPrtlInOval : public COvalAlong3DAxes  
{
public:
	CPrtlInOval(SimuValue radiusOnX, SimuValue radiusOnY, SimuValue radiusOnZ,
				SimuValue angleAboutZ, CVector3D* ctrPos, CVector3D* direction);
	virtual ~CPrtlInOval();

	void CreatePrtlInOval(CPrtlFluid* prtlFluid, SimuValue srfcThickness,
						  SimuValue hollowOvalRatio);
	void CreatePrtlInOval(CPrtlFluid* prtlFluid, SimuValue srfcThickness,
						  SimuValue hollowOvalRatio, CVector3D* iniVelocity);
};

#endif // !defined(AFX_PRTLINOVAL_H__C70EC142_31AC_4C44_AACB_7F8E7B3B5274__INCLUDED_)
