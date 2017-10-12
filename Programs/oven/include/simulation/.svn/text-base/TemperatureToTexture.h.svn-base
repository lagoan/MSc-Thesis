// TemperatureToTexture.h: interface for the CTemperatureToTexture class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPERATURETOTEXTURE_H__9E00600C_89C5_438B_B3DD_3A3B72A17C80__INCLUDED_)
#define AFX_TEMPERATURETOTEXTURE_H__9E00600C_89C5_438B_B3DD_3A3B72A17C80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTemperatureToTexture  
{
public:
	CTemperatureToTexture();
	virtual ~CTemperatureToTexture();

	static void ConvertScheme(SimuValue temperature, QString &txString, CLinearTemperaTexture* pLTT);
	static void GetDisplayColor(SimuValue temperature, CVector3D* color, CLinearTemperaTexture* pLTT);

private:
	static void ConvertToColor(SimuValue temperature, CVector3D* color, CLinearTemperaTexture* pLTT);
	static SimuValue ConvertToFilter(SimuValue temperature, CLinearTemperaTexture* pLTT);

	static void ConvertToColor0(SimuValue temperature, CVector3D* color, CLinearTemperaTexture* pLTT);
	static SimuValue ConvertToFilter0(SimuValue temperature, CLinearTemperaTexture* pLTT);
};

#endif // !defined(AFX_TEMPERATURETOTEXTURE_H__9E00600C_89C5_438B_B3DD_3A3B72A17C80__INCLUDED_)
