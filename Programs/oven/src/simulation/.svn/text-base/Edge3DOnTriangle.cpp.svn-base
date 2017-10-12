// Edge3DOnTriangle.cpp: implementation of the CEdge3DOnTriangle class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "Edge3DOnTriangle.h"



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEdge3DOnTriangle::CEdge3DOnTriangle()
{
	m_e3dotTris[0] = NULL;
	m_e3dotTris[1] = NULL;
}

CEdge3DOnTriangle::~CEdge3DOnTriangle()
{

}

bool CEdge3DOnTriangle::AddOneTriangle(CTriangle3D* pTriangle)
{
	if (m_e3dotTris[0] == NULL)
		m_e3dotTris[0] = pTriangle;
	else if (m_e3dotTris[1] == NULL)
		m_e3dotTris[1] = pTriangle;
	else
	{
		CSimuMsg::ExitWithMessage("CEdge3DOnTriangle::AddOneTriangle(pTriangle)",
								  "Too many triangle on edge.");
		return false;
	}
	return true;
}

void CEdge3DOnTriangle::RemoveOneTriangle(CTriangle3D* pTriangle)
{
	ReplaceOneTriangle(pTriangle, NULL);
}

void CEdge3DOnTriangle::ReplaceOneTriangle(CTriangle3D* oldTriangle,
										   CTriangle3D* newTriangle)
{
	if (m_e3dotTris[0] == oldTriangle)
		m_e3dotTris[0] = newTriangle;
	else if (m_e3dotTris[1] == oldTriangle)
		m_e3dotTris[1] = newTriangle;
}

CTriangle3D* CEdge3DOnTriangle::GetTheOtherTriangle(CTriangle3D* pTriangle)
{
	if (m_e3dotTris[0] == pTriangle)
		return m_e3dotTris[1];
	else if (m_e3dotTris[1] == pTriangle)
		return m_e3dotTris[0];
	else
		return NULL;
}
