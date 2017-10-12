// SimuDriver.h: interface for the CSimuDriver class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUDRIVER_H__6282F702_D495_11D5_B423_00D0B76AB775__INCLUDED_)
#define AFX_SIMUDRIVER_H__6282F702_D495_11D5_B423_00D0B76AB775__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimuManager;

class CSimuDriver  
{
public:
	CSimuDriver(CSimuManager* pSimuManager);
	~CSimuDriver();

	// new in version 5.0
	void RunAnimationOneTimeStep();

	// new in version 6.0
	void DrivePrtlFluidOneTimeStep();

private:
	CSimuManager*	m_pSimuManager;
};

#endif // !defined(AFX_SIMUDRIVER_H__6282F702_D495_11D5_B423_00D0B76AB775__INCLUDED_)
