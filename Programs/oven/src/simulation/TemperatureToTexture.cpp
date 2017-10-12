// TemperatureToTexture.cpp: implementation of the CTemperatureToTexture class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "TemperatureToTexture.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemperatureToTexture::CTemperatureToTexture()
{

}

CTemperatureToTexture::~CTemperatureToTexture()
{

}

void CTemperatureToTexture::ConvertScheme(SimuValue temperature, QString &txString,
										  CLinearTemperaTexture* pLTT)
{
	CVector3D color;
	ConvertToColor(temperature, &color, pLTT);
	char colorStr[LEN_DATA_LINE];
	if (color[R] < 0)
		sprintf(colorStr, "Fluid_Color"); // color will be specified in povray variable
	else
		sprintf(colorStr, "<%f, %f, %f>", color[R], color[G], color[B]);

	SimuValue filter = ConvertToFilter(temperature, pLTT);
	char filterStr[LEN_DATA_LINE];
	if (filter < 0)
		sprintf(filterStr, "Fluid_Filter"); // filter will be specified in povray variable
	else
		sprintf(filterStr, "%f", filter);

	txString = "texture { pigment { color ";
	txString += colorStr;
	txString += " filter ";
	txString += filterStr;
	txString += " } finish { ";
	txString += pLTT->m_lttName;
	txString += "_finish } }";
}

void CTemperatureToTexture::GetDisplayColor(SimuValue temperature, CVector3D* color,
											CLinearTemperaTexture* pLTT)
{
	ConvertToColor(temperature, color, pLTT);
	if (color->v[R] < 0)
	{
		color->SetElements(1, 1, 1);
	}
}

void CTemperatureToTexture::ConvertToColor(SimuValue temperature, CVector3D* color,
										   CLinearTemperaTexture* pLTT)
{
	ConvertToColor0(temperature, color, pLTT);
}

SimuValue CTemperatureToTexture::ConvertToFilter(SimuValue temperature,
												 CLinearTemperaTexture* pLTT)
{
	return ConvertToFilter0(temperature, pLTT);
}

// 3 color elements are linear functions of temperature 
void CTemperatureToTexture::ConvertToColor0(SimuValue temperature, CVector3D* color,
											CLinearTemperaTexture* pLTT)
{
	SimuValue colorRatio = (temperature-pLTT->m_lttMinTemperature)
						/(pLTT->m_lttMaxTemperature - pLTT->m_lttMinTemperature);
	if (colorRatio < 0)
	{
		colorRatio = 0;
	}
	else if (colorRatio > 1)
	{
		colorRatio = 1;
	}
	color->SetElements(pLTT->m_lttRatioColorR*colorRatio + pLTT->m_lttConstColorR,
					   pLTT->m_lttRatioColorG*colorRatio + pLTT->m_lttConstColorG,
					   pLTT->m_lttRatioColorB*colorRatio + pLTT->m_lttConstColorB);
}

// filter is a linear function of temperature
SimuValue CTemperatureToTexture::ConvertToFilter0(SimuValue temperature,
												  CLinearTemperaTexture* pLTT)
{
	SimuValue filterRatio = (temperature-pLTT->m_lttMinTemperature)
						/(pLTT->m_lttMaxTemperature - pLTT->m_lttMinTemperature);
	if (filterRatio < 0)
	{
		filterRatio = 0;
	}
	else if (filterRatio > 1)
	{
		filterRatio = 1;
	}
	return pLTT->m_lttRatioFilter*filterRatio + pLTT->m_lttConstFilter;
}
