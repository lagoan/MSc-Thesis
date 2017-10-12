// RotationToZAxis.h: interface for the CRotationToZAxis class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ROTATIONTOZAXIS_H__17027068_CE9B_4BB8_AF70_3C698D4A2BF0__INCLUDED_)
#define AFX_ROTATIONTOZAXIS_H__17027068_CE9B_4BB8_AF70_3C698D4A2BF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRotationToZAxis  
{
public:
	CVector3D m_rtzaOrgDirection;
	SimuValue m_rtzaRotationAngleAboutZAxis;

	CRotationToZAxis();
	CRotationToZAxis(CVector3D* orgDirection);
	CRotationToZAxis(CVector3D* orgDirection, SimuValue rotationAngleAboutZAxis);
	virtual ~CRotationToZAxis();

	void ExportRotationData(CString objName, FILE* fp);
	void ImportRotationData(FILE* fp);

	void RotateStdVectToOrgVect(CVector3D &oldV, CVector3D &newV);
	void RotateOrgVectToStdVect(CVector3D &oldV, CVector3D &newV);

	static time_t	m_t2005_JUNE_13_8AM;
	static void AddAngularVelocity(SimuValue angVel, SimuValue timeStep,
									CVector3D &pPos, CVector3D &pVel);
	static void ComputeAngularVelocity(SimuValue angVel, SimuValue timeStep,
										CVector3D &pPos, CVector3D &pAngVel);

protected:
	CVector3D m_rtzaAxisZDir;
	CVector3D m_rtzaRotationV;
	SimuValue m_rtzaRotationAngle;
	SimuValue m_rtzaRotateM[3][3];
	SimuValue m_rtzaInvRotM[3][3];

	void InitializeVariables(CVector3D* orgDirection, SimuValue rotationAngleAboutZAxis);
	void ComputeRotationVariables();
	void PrepareRotationMatrix();
};

#endif // !defined(AFX_ROTATIONTOZAXIS_H__17027068_CE9B_4BB8_AF70_3C698D4A2BF0__INCLUDED_)
