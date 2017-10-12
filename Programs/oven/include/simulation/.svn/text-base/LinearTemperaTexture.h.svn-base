// LinearTemperaTexture.h: interface for the CLinearTemperaTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LINEARTEMPERATEXTURE_H__15F9E742_86D8_4146_ADF8_39949FACE198__INCLUDED_)
#define AFX_LINEARTEMPERATEXTURE_H__15F9E742_86D8_4146_ADF8_39949FACE198__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLinearTemperaTexture  
{
public:
	static int m_lttId;
	QString m_lttName;
	SimuValue m_lttRatioColorR;
	SimuValue m_lttRatioColorG;
	SimuValue m_lttRatioColorB;
	SimuValue m_lttConstColorR;
	SimuValue m_lttConstColorG;
	SimuValue m_lttConstColorB;
	SimuValue m_lttRatioFilter;
	SimuValue m_lttConstFilter;
	SimuValue m_lttMinTemperature;
	SimuValue m_lttMaxTemperature;

	CLinearTemperaTexture();
	virtual ~CLinearTemperaTexture();

	void CopyValuesFrom(CLinearTemperaTexture* anotherLTT);
};

#endif // !defined(AFX_LINEARTEMPERATEXTURE_H__15F9E742_86D8_4146_ADF8_39949FACE198__INCLUDED_)
