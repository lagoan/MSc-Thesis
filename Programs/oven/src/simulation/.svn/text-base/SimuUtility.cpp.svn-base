// SimuUtility.cpp: implementation of the CSimuUtility class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuUtility.h"
#include "SimuMsg.h"
#include "Edge3DOnTriangle.h"

#define _GLIBCPP_USE_C99
#include <float.h>
#include <cmath>


int CSimuUtility::m_suEdgeMatrixRow = 3;
int CSimuUtility::m_suEdgeMatrixCol = 4;
SimuValue** CSimuUtility::m_suEdgeMatrix = NULL;
SimuValue* CSimuUtility::m_suCtrPos = NULL;

CVector3D* CSimuUtility::m_suEdge0 = NULL;;
CVector3D* CSimuUtility::m_suEdge1 = NULL;;
CVector3D* CSimuUtility::m_suEdge2 = NULL;;
CVector3D* CSimuUtility::m_suMidP0 = NULL;;
CVector3D* CSimuUtility::m_suMidP1 = NULL;;
CVector3D* CSimuUtility::m_suMidP2 = NULL;;
CVector3D* CSimuUtility::m_suTriNormal = NULL;;
CVector3D* CSimuUtility::m_suIntersectLineVector0 = NULL;;
CVector3D* CSimuUtility::m_suCircumcircleCenter = NULL;;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuUtility::CSimuUtility()
{

}

CSimuUtility::~CSimuUtility()
{

}

// If yes, return true; otherwise false.
bool CSimuUtility::IsInvalidSimuValue(SimuValue value)
{
#ifdef Q_OS_WIN
	return (_isnan(value) != 0);
#else 
	return (std::isnan(value) != 0);
#endif
}

// If yes, return true; otherwise false.
bool CSimuUtility::IsInvalidCVector3D(CVector3D* vector)
{
	for (unsigned int i=0; i < 3; i++)
		if (IsInvalidSimuValue(vector->v[i]))
			return true;
	return false;
}

void CSimuUtility::AssertSimuValue(SimuValue value, QString location)
{
	if (IsInvalidSimuValue(value))
	{
		CSimuManager::m_bInvalidValue = true;
		location.prepend("Invalid Simuvalue @ ");
		std::cout << "CSimuUtility::AssertSimuValue(value) " << location.toStdString().c_str() << "\n";
		//CSimuMsg::ExitWithMessage("CSimuUtility::AssertSimuValue(value)",
			//					location.toStdString().c_str());
	}
}

void CSimuUtility::AssertSimuVector3D(CVector3D* vector, QString location)
{
	for (unsigned int i=0; i < 3; i++)
		AssertSimuValue(vector->v[i], location);
}

bool CSimuUtility::FileDoesExist(QString fileName)
{
	FILE* fp;
	if ((fp=fopen(fileName.toAscii(), "r")) == NULL)
		return false;
	fclose(fp);
	return true;
}

SimuValue CSimuUtility::ComputeSphereVolume(SimuValue radius)
{
	SimuValue volume = pow(radius, 3)*4*SIMU_PI/3;
	return volume;
}

void CSimuUtility::DrawSphereOr3DCrossAtPosition(CVector3D* pPos,
												 SimuValue radius)
{
	if (CSimuManager::DRAW_PARTICLES_BY_SPHERE)
	{
		GLUquadricObj* qObj = gluNewQuadric();
		glPushMatrix();
		glTranslated(pPos->v[X], pPos->v[Y], pPos->v[Z]);
		gluSphere(qObj, radius, CSimuManager::m_sphereSlices, CSimuManager::m_sphereStacks);
		glPopMatrix();
		gluDeleteQuadric(qObj);
	}
	else
	{
		glBegin(GL_LINES);
		SimuVertex3d(pPos->v[X]-radius, pPos->v[Y], pPos->v[Z]);
		SimuVertex3d(pPos->v[X]+radius, pPos->v[Y], pPos->v[Z]);
		SimuVertex3d(pPos->v[X], pPos->v[Y]-radius, pPos->v[Z]);
		SimuVertex3d(pPos->v[X], pPos->v[Y]+radius, pPos->v[Z]);
		SimuVertex3d(pPos->v[X], pPos->v[Y], pPos->v[Z]-radius);
		SimuVertex3d(pPos->v[X], pPos->v[Y], pPos->v[Z]+radius);
		glEnd();
	}
}

//
// This function draws a circle centered at origin on z=0 plane
// by drawing a closed series of line segments.
//
void CSimuUtility::DrawCircleOnZ0ByOpenGL(SimuValue radius, SimuValue height,
										  SimuValue segments)
{
	SimuValue dang = SIMU_PI*2/segments;
	glBegin(GL_LINE_LOOP);
	for(SimuValue ang=0; ang <= 2*SIMU_PI; ang += dang)
		SimuVertex3d(radius * cos(ang), radius * sin(ang), height);
	glEnd();
}

void CSimuUtility::DrawCylinderOnZ0ByOpenGL(SimuValue radius, SimuValue height,
											SimuValue segments)
{
	SimuValue dang = SIMU_PI*2/segments;
	glBegin(GL_LINES);
	for(SimuValue ang=0; ang <= 2*SIMU_PI; ang += dang)
	{
		SimuVertex3d(radius * cos(ang), radius * sin(ang), 0);
		SimuVertex3d(radius * cos(ang), radius * sin(ang), height);
	}
	glEnd();
}

void CSimuUtility::DrawBoundingBox(CVector3D &minPos, CVector3D &maxPos)
{
	glBegin(GL_LINES);
	SimuVertex3d(minPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], maxPos[Z]);

	SimuVertex3d(minPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], maxPos[Z]);

	SimuVertex3d(minPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], minPos[Y], maxPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(minPos[X], maxPos[Y], maxPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], minPos[Z]);
	SimuVertex3d(maxPos[X], maxPos[Y], maxPos[Z]);
	glEnd();
}

// This rotation will rotate unit vector on Y axis to the specified vector <dirV>
void CSimuUtility::RotateFromYAxis(CVector3D* dirV,
								   CVector3D* oldV, CVector3D* newV)
{
	// rotate about z axis
	SimuValue cosXY = dirV->v[Y];
	SimuValue sinXY = dirV->v[X];
	SimuValue tmpX = oldV->v[X]*cosXY + oldV->v[Y]*sinXY;
	newV->v[Y] = oldV->v[Y]*cosXY - oldV->v[X]*sinXY;
	// rotate about y axis
	SimuValue cosXZ = dirV->v[X];
	SimuValue sinXZ = dirV->v[Z];
	newV->v[X] = tmpX*cosXZ - oldV->v[Z]*sinXZ;
	newV->v[Z] = tmpX*sinXZ + oldV->v[Z]*cosXZ;
}

// This rotation will rotate unit vector on Y axis to the directional vector which
// is specified by four trigonometry values.
void CSimuUtility::RotateFromYAxis(SimuValue cosXY, SimuValue sinXY,
								   SimuValue cosXZ, SimuValue sinXZ,
								   CVector3D* oldV, CVector3D* newV)
{
	// rotate about z axis
	SimuValue tmpX = oldV->v[X]*cosXY + oldV->v[Y]*sinXY;
	newV->v[Y] = oldV->v[Y]*cosXY - oldV->v[X]*sinXY;
	// rotate about y axis
	newV->v[X] = tmpX*cosXZ - oldV->v[Z]*sinXZ;
	newV->v[Z] = tmpX*sinXZ + oldV->v[Z]*cosXZ;
}

// x, y, z axes are in 3D.
// the rotation is to sweep fingers of right hand from z to x
// and thumb pointing to positive y axis
void CSimuUtility::RotateAboutYAxis(SimuValue cosA, SimuValue sinA,
									SimuValue oldX, SimuValue oldZ,
									SimuValue &newX, SimuValue &newZ)
{
	newZ = oldZ*cosA - oldX*sinA;
	newX = oldZ*sinA + oldX*cosA;
}

void CSimuUtility::ComputeRightHandRotationMatrix(SimuValue rAX,
												  SimuValue rAY,
												  SimuValue rAZ,
												  SimuValue rAngle,
												  SimuValue rMatrix[3][3])
{
	SimuValue s = sin(rAngle/180*SIMU_PI);
	SimuValue c = cos(rAngle/180*SIMU_PI);
	SimuValue t = 1 - c;

	rMatrix[0][0] = t*rAX*rAX + c;
	rMatrix[0][1] = t*rAX*rAY - s*rAZ;
	rMatrix[0][2] = t*rAX*rAZ + s*rAY;

	rMatrix[1][0] = t*rAX*rAY + s*rAZ;
	rMatrix[1][1] = t*rAY*rAY + c;
	rMatrix[1][2] = t*rAY*rAZ - s*rAX;

	rMatrix[2][0] = t*rAX*rAZ - s*rAY;
	rMatrix[2][1] = t*rAY*rAZ + s*rAX;
	rMatrix[2][2] = t*rAZ*rAZ + c;
}

void CSimuUtility::RotateVector(SimuValue rMatrix[3][3],
								CVector3D &vOld, CVector3D &vNew)
{
	for (int i=0; i < 3; i++)
	{
		vNew[i] = 0;
		for (int j=0; j < 3; j++)
			vNew[i] += rMatrix[i][j]*vOld[j];
	}
}

bool CSimuUtility::Inverse3x3Matrix(SimuValue matrix[9], SimuValue inverse[9])
{
	inverse[0] = matrix[4]*matrix[8] - matrix[5]*matrix[7];
	inverse[3] = matrix[5]*matrix[6] - matrix[3]*matrix[8];
	inverse[6] = matrix[3]*matrix[7] - matrix[4]*matrix[6];

	SimuValue D = matrix[0]*inverse[0] + matrix[1]*inverse[3] + matrix[2]*inverse[6];
	if (fabs(D) <= SIMU_VALUE_EPSILON)
		return false;
	else
	{
		inverse[0] /= D;
		inverse[1] = (matrix[2]*matrix[7] - matrix[1]*matrix[8])/D;
		inverse[2] = (matrix[1]*matrix[5] - matrix[2]*matrix[4])/D;
		inverse[3] /= D;
		inverse[4] = (matrix[0]*matrix[8] - matrix[2]*matrix[6])/D;
		inverse[5] = (matrix[2]*matrix[3] - matrix[0]*matrix[5])/D;
		inverse[6] /= D;
		inverse[7] = (matrix[1]*matrix[6] - matrix[0]*matrix[7])/D;
		inverse[8] = (matrix[0]*matrix[4] - matrix[1]*matrix[0])/D;
	}
}

//=============================================================================
// gauselim.cpp : Gaussian Elimination / Backward Substitution / Partial Pivoting
//   by Kenny Hoff : 2/5/97 : VERSION 1
//   1) solves a system of linear equations in the form of an n x m augmented
//      matrix (n is the # of row, m is the # of cols).
//   2) solution X is returned as an n-element array.
//   3) the process proceeds in two basic parts:
//        a) reduction of the matrix M into row-echelon form.
//        b) solution X is found by backwards substitution.
//   4) the procedure terminates if their is no nonzero element in the first
//      column of the current submatrix or after the reduction to row-echelon
//      form if their is a row with all zeros (except for last elt). This
//      means either their is no solution, or an infinite number of solutions
//      - therefore, "no unique solution exists" (works for both cases).
//   5) typically, m should equal n+1 (# of cols should equal # of rows + 1).
//      This results in a equal number of equations and unknowns, and the coefficient
//      matrix (entire matrix except last col in augmented form) will be square (nxn)
//   6) returns 1 is successful, 0 is unsuccessful.
//   7) k refers to the current permutation (kth submatrix), i always refers
//      to rows, and j always refers to cols.
//   8) Alters input matrix M, stores solution in X
//   9) Augmented matrix M solves a linear set of equations in the form Ax=b
//      where A is the left nxn of M (the coefficient submatrix of M), x is
//      the vector being solved for, and b is the last column of M.
//============================================================================
//=============================================================================
// gauseli2.cpp : Gaussian Elimination / Backward Substitution / Partial Pivoting
//   by Kenny Hoff : 2/5/97 : VERSION 2
//   * Basically the same as in version 1, but row-echelon form is not obtained.
//     Instead, we only convert to an upper triangular matrix and leading
//     1's are not required (we just skip the normalization when swapping
//     top row of submatrix with pivot row). The backwards substitution had
//     to be modified to handle this.
//============================================================================
// Note: replace m in the original code with n+1.

//#define		SIMU_PARA_VERIFY_EQUATION_SOLUTION

//   9) Augmented matrix M solves a linear set of equations in the form Ax=b
//      where A is the left nxn of M (the coefficient submatrix of M), x is
//      the vector being solved for, and b is the last column of M.
bool CSimuUtility::SolveNxNLinearEquations(SimuValue** M, SimuValue* X, int n)
{
	char location[] = "CSimuUtility::SolveNxNLinearEquations(M, X, n)";

	int k,i,j,PivotRow;
	SimuValue *SwapRowTmp, NullFactor;

#ifdef	SIMU_PARA_VERIFY_EQUATION_SOLUTION
	SimuValue** N = new SimuValue*[n];
	for (i=0; i < n; i++)
	{
		N[i] = new SimuValue[n+1];
		for (j=0; j < n+1; j++) N[i][j] = M[i][j];
	}
#endif

	bool bSuccessful = true;
	//------- PART 1: CONVERSION OF MATRIX M INTO UPPER TRIANGULAR FORM -------
	for (k=0; k<n; k++)  // FOR EACH SUBMATRIX SIZE (n-k) x (n-m)
	{
		// FIND THE ELEMENT IN THE KTH COL WITH THE LARGEST ABS VAL (PIVOT ELEMENT)
		PivotRow = k;
		for (i=k+1; i<n; i++)
			if (fabs(M[i][k]) > fabs(M[PivotRow][k]))
				PivotRow=i;

		// IF THERE IS NO NONZERO ELEMENT IN THE FIRST COL OF THE SUBMATRIX, QUIT!
		if ((SimuValue)fabs(M[PivotRow][k]) <= SIMU_VALUE_EPSILON)
		{ 
			PrintNxNLinearEquations(M, X, n);
			CSimuMsg::ExitWithMessage(location, "NO UNIQUE SOLUTION EXISTS (PIVOT)!");
			//TRACE("NO UNIQUE SOLUTION EXISTS (PIVOT)!\n");
			bSuccessful = false;
			break;
		}

		// SWAP THE PIVOT ROW WITH THE KTH ROW
		SwapRowTmp = M[PivotRow];
		M[PivotRow] = M[k];
		M[k] = SwapRowTmp;

		// NULLIFY (TURN INTO ZERO) ALL ELEMENT IN COL BELOW TOP ROW BY
		// SUBTRACTING SOME FACTOR TIMES THE TOP ROW.
		for (i=k+1; i<n; i++)
		{
			NullFactor = -(M[i][k]/M[k][k]);
			M[i][k]=0;
			for (j=k+1; j<n+1; j++)
				M[i][j] += (M[k][j]*NullFactor);
		}
/*
		// IF THEIR IS A ROW CONTAINING ALL ZEROS (EXCEPT FOR LAST ELT), QUIT!
		if ((SimuValue)fabs(M[n-1][n-1]) <= SIMU_VALUE_EPSILON)
		{
			//TRACE("altered matrix:\n");
			//TRACENxNLinearEquations(M, X, n);
			CSimuMsg::ExitWithMessage(location, "NO UNIQUE SOLUTION EXISTS (ZERO ROW)!");
			//TRACE("NO UNIQUE SOLUTION EXISTS (ZERO ROW)!\n");
			bSuccessful = false;
			break;
		}
*/
	}

	if (bSuccessful)
	{
		//------- PART 2: BACKWARDS SUBSTITUTION -------
		for (k=(n-1); k>=0; k--)  // FOR EACH ROW STARTING AT THE BOTTOM
		{
			X[k] = M[k][n+1-1];      // SOLVE FOR CURRENT X[k]
			for (j=(k+1); j<(n+1-1); j++)
				X[k] -= (M[k][j]*X[j]);
			X[k] /= M[k][k];
		}

#ifdef	SIMU_PARA_VERIFY_EQUATION_SOLUTION
		if (!VerifyNxNLinearEquations(N, X, n))
		{
			CSimuMsg::ExitWithMessage(location, "Solution can not be verified with original matrix.");
			if (!VerifyNxNLinearEquations(M, X, n))
				CSimuMsg::ExitWithMessage(location, "Solution can not be verified.");
			bSuccessful = false;
		}
#endif
	}

	if (!bSuccessful)
	{
		//TRACE("altered matrix:\n");
		PrintNxNLinearEquations(M, X, n);
#ifdef	SIMU_PARA_VERIFY_EQUATION_SOLUTION
		//TRACE("original matrix:\n");
		//TRACENxNLinearEquations(N, X, n);
#endif
	}

#ifdef	SIMU_PARA_VERIFY_EQUATION_SOLUTION
	for (i=0; i < n; i++) delete N[i];
	delete N;
#endif

	return bSuccessful;  // true IF SUCCESSFUL!
}

// verify the solution from CSimuUtility::SolveNxNLinearEquations(...)
bool CSimuUtility::VerifyNxNLinearEquations(SimuValue** M, SimuValue* X, int n)
{
	bool bVerified = true;
	for (int i=0; i < n; i++)
	{
		SimuValue sum = 0;
		for (int j=0; j < n; j++)
			sum += M[i][j]*X[j];

		SimuValue delta = (SimuValue)fabs(sum - M[i][n]);
		if (delta > SIMU_VALUE_EPSILON)
		{
			//TRACE("row=%d: ", i);
			for (int j=0; j < n; j++)
				//TRACE("%+2.4f*%+2.4f ", M[i][j], X[j]);
			//TRACE("= %+2.4f\n", M[i][n]);
			bVerified = false;
		}
	}
	return bVerified;
}

//   Note: the row number is in the second index.
void CSimuUtility::PrintNxNLinearEquations(SimuValue** M, SimuValue* X, int n)
{
	//TRACE("linear equations: [M(%dx%d)][X] = [b]\n", n, n);
	for (int i=0; i < n; i++)
	{
		//TRACE("[");
		for (int j=0; j < n; j++){
			//TRACE("%+2.4f  ", M[i][j]);
		}
		//TRACE("]");
		//TRACE("[%+2.4f]", X[i]);
		//TRACE(" = [%+2.4f]\n", M[i][n]);
	}
}

//   Note: the row number is in the first index.
void CSimuUtility::Print3x3Matrix(SimuValue A[3][3])
{
	//TRACE("matrix: A(3x3)\n");
	for (int i=0; i < 3; i++)
	{
		for (int j=0; j < 3; j++) {
			std::cout << A[i][j] << " ";
		}
		std::cout << "\n";
	}
}

SimuValue CSimuUtility::Compute3x3MatrixTrace(SimuValue M[3][3])
{
	return M[0][0] + M[1][1] + M[2][2];
}

SimuValue CSimuUtility::Compute3x3MatrixFrobNorm(SimuValue M[3][3])
{
	SimuValue sum = 0;
	for (int i=0; i < 3; i++)
	for (int j=0; j < 3; j++)
		sum += M[i][j]*M[i][j];
	sum = sqrt(sum);
	return sum;
}

//	Solve cubic equation:
//		coeff[3] * x^3 + coeff[2] * x^2 + coeff[1] * x + coeff[0] = 0
// return 3: 3 real roots in x[0], x[1], [2]
// return 1: 1 real root in x[0]
int CSimuUtility::SolveCubicEquation(SimuValue coeff[4], SimuValue x[3])
{
	SimuValue a1 = coeff[2] / coeff[3];
	SimuValue a2 = coeff[1] / coeff[3];
	SimuValue a3 = coeff[0] / coeff[3];

	SimuValue Q = (a1 * a1 - 3 * a2) / 9;
	SimuValue R = (2 * a1 * a1 * a1 - 9 * a1 * a2 + 27 * a3) / 54;
	SimuValue Qcubed = Q * Q * Q;
	SimuValue d = Qcubed - R * R;

	// Three real roots
	if (d >= 0) {
		SimuValue theta = acos(R / sqrt(Qcubed));
		SimuValue sqrtQ = sqrt(Q);
		x[0] = -2 * sqrtQ * cos( theta           / 3) - a1 / 3;
		x[1] = -2 * sqrtQ * cos((theta + 2 * SIMU_PI) / 3) - a1 / 3;
		x[2] = -2 * sqrtQ * cos((theta + 4 * SIMU_PI) / 3) - a1 / 3;
		return 3;
	}

	// One real root
	else {
		SimuValue e = pow((double)sqrt(-d) + fabs(R), (double)1.0f / 3.0f);
		if (R > 0)
			e = -e;
		x[0] = (e + Q / e) - a1 / 3.;
		return 1;
	}
}

//	Solve quadratic equation:
//		coeff[2] * x^2 + coeff[1] * x + coeff[0] = 0
// return 2: 2 real roots in x[0], x[1]
// return 1: 1 real root in x[0]
// return 0: no real root
int CSimuUtility::SolveQuadraticEquation(SimuValue coeff[3], SimuValue x[2])
{
	SimuValue a = coeff[2];
	SimuValue b = coeff[1];
	SimuValue c = coeff[0];
	SimuValue d = b * b - 4 * a * c;
	// Two real, distinct roots
	if (d > 0) {
		SimuValue q = 2 * a;
		d = sqrt(d);
		x[0] = (-b + d) / q;
		x[1] = (-b - d) / q;
		return 2;
	}
	// One real double root
	else if (d == 0) {
		x[0] = x[1] = -b / (2 * a);
		return 1;
	}
	// Two complex conjugate roots
	else {	// d < 0
//
//		x[0] = x[1] = -b / (2 * a);
//		if (im != NULL) { // im is for imaginary parts of the complex roots
//			im[0] = d / (2 * a);
//			im[1] = -im[0];
//		}
//
		return 0;
	}
}

// compute dot product = v1 * v2
SimuValue CSimuUtility::DotProduct3D(SimuValue* v1, SimuValue* v2)
{
	return v1[X]*v2[X] + v1[Y]*v2[Y] + v1[Z]*v2[Z];
}

// compute dot product = v1 * v2
SimuValue CSimuUtility::DotProduct(CVector3D* v1, CVector3D* v2)
{
	return DotProduct3D(v1->v, v2->v);
}

// compute (v1 - vShared) * (v2 - vShared)
SimuValue CSimuUtility::DotProduct(CVector3D* v1, CVector3D* v2, CVector3D* vShared)
{
	CVector3D va, vb;
	va.SetValueAsSubstraction(v1, vShared);
	vb.SetValueAsSubstraction(v2, vShared);
	return CSimuUtility::DotProduct(&va, &vb); 
}

// compute (v1 x v2) * v3
SimuValue CSimuUtility::CrossAndDotProduct(CVector3D* v1,
										   CVector3D* v2,
										   CVector3D* v3)
{
	SimuValue nx, ny, nz;
	nx = v1->v[Y]*v2->v[Z] - v1->v[Z]*v2->v[Y];
	ny = v1->v[Z]*v2->v[X] - v1->v[X]*v2->v[Z];
	nz = v1->v[X]*v2->v[Y] - v1->v[Y]*v2->v[X];
	return nx*v3->v[X] + ny*v3->v[Y] + nz*v3->v[Z];
}

// compute cross product of two vectors = (p0 - pShared) x (p1 - pShared)
void CSimuUtility::CrossProduct(CVector3D* p0, CVector3D* p1, CVector3D* pShared, 
								CVector3D* result)
{
	CVector3D edge[2];
	edge[0].SetValueAsSubstraction(p0, pShared);
	edge[1].SetValueAsSubstraction(p1, pShared);
	result->SetValueAsCrossProduct(&(edge[0]), &(edge[1]));
}

// area = |(pb - pa)x(pc - pa)|/2;
SimuValue CSimuUtility::ComputeTriangleArea3D(SimuValue* pa, SimuValue* pb, SimuValue* pc)
{
	SimuValue vba[3] = {pb[0] - pa[0], pb[1] - pa[1], pb[2] - pa[2]};
	SimuValue vca[3] = {pc[0] - pa[0], pc[1] - pa[1], pc[2] - pa[2]};
	SimuValue nx, ny, nz;
	nx = vba[1]*vca[2] - vba[2]*vca[1];
	ny = vba[2]*vca[0] - vba[0]*vca[2];
	nz = vba[0]*vca[1] - vba[1]*vca[0];
	return sqrt(nx*nx + ny*ny + nz*nz)/2;
}

// area = |(pb - pa)x(pc - pa)|/2;
SimuValue CSimuUtility::ComputeTriangleArea(CVector3D* pa,
											CVector3D* pb,
											CVector3D* pc)
{
	return ComputeTriangleArea3D(pa->v, pb->v, pc->v);
}

// return the triangle area
SimuValue CSimuUtility::ComputeTriangleNormal(CVector3D* pa, CVector3D* pb,
											  CVector3D* pc, CVector3D* n)
{
	return ComputeTriangleNormal(pa, pb, pc, n->v);
}

SimuValue CSimuUtility::ComputeTriangleNormal(CVector3D* pa, CVector3D* pb,
											  CVector3D* pc, SimuValue* n)
{
	n[X] = (pb->v[Y] - pa->v[Y])*(pc->v[Z] - pa->v[Z]) - (pb->v[Z] - pa->v[Z])*(pc->v[Y] - pa->v[Y]);
	n[Y] = (pb->v[Z] - pa->v[Z])*(pc->v[X] - pa->v[X]) - (pb->v[X] - pa->v[X])*(pc->v[Z] - pa->v[Z]);
	n[Z] = (pb->v[X] - pa->v[X])*(pc->v[Y] - pa->v[Y]) - (pb->v[Y] - pa->v[Y])*(pc->v[X] - pa->v[X]);
	SimuValue area = sqrt(n[X]*n[X] + n[Y]*n[Y] + n[Z]*n[Z]);
	if (fabs(area) <= SIMU_VALUE_EPSILON)
	{
//		CSimuMsg::ExitWithMessage("CSimuUtility::ComputeTriangleNormal(...)",
//									"Degenerate triangle.");
		return area;
	}
	else
	{	n[X] /= area; n[Y] /= area; n[Z] /= area;	}
	return area/2;
}

void CSimuUtility::ComputeTriangleNormalByRefPos(CVector3D* pa, CVector3D* pb, CVector3D* pc,
												 CVector3D* pos, bool sameSide, CVector3D* n)
{
	CVector3D refDir;
	refDir.SetValueAsSubstraction(pos, pa);
	ComputeTriangleNormalByRefDir(pa, pb, pc, &refDir, sameSide, n);
}

void CSimuUtility::ComputeTriangleNormalByRefDir(CVector3D* pa, CVector3D* pb, CVector3D* pc,
												 CVector3D* dir, bool sameSide, CVector3D* n)
{
	CSimuUtility::CrossProduct(pa, pb, pc, n);
	SimuValue tmpVolume = dir->DotProduct(n);
	if ((sameSide && tmpVolume > 0)
	 || (!sameSide && tmpVolume < 0))
		n->Normalize(1);
	else
		n->Normalize(-1);
}

void CSimuUtility::CreateRandomPos(SimuValue boundingCubeLen, CVector3D* ranPos)
{
	for (int d=X; d <= Z; d++)
	{
		ranPos->v[d] = boundingCubeLen*(2.0*rand()/RAND_MAX - 1);
	}
}

void CSimuUtility::CreateRandomNormal(CVector3D* normal)
{
	for (unsigned int d=X; d <= Z; d++)
	{
		normal->v[d] = (rand()/RAND_MAX - 0.5);
		normal->v[d] += normal->v[d] >= 0? 0.5 : -0.5;
	}
	normal->Normalize();
}

// check if point p is in triangle <pa, pb, pc>
// return true if it is; otherwise, return false.
bool CSimuUtility::IsPointInTriangle3D(SimuValue* p,
									   SimuValue* pa, SimuValue* pb, SimuValue* pc)
{
	SimuValue a0 = ComputeTriangleArea3D(pa, pb, pc);
	SimuValue a1 = ComputeTriangleArea3D(p, pb, pc);
	SimuValue a2 = ComputeTriangleArea3D(pa, p, pc);
	SimuValue a3 = ComputeTriangleArea3D(pa, pb, p);
	SimuValue a = a1 + a2 + a3 - a0;
	if (fabs(a) > fabs(a0)*0.001)
		return false;
	return true;
}

// Determine whether or not point p is bounded by points pa,pb,pc.
// return true if it is; otherwise, return false.
bool CSimuUtility::IsPointInTriangle(CVector3D* p,
									 CVector3D* pa, CVector3D* pb, CVector3D* pc)
{
//	return IsPointInTriangle3D(p->v, pa->v, pb->v, pc->v);

	// this method makes simple ovel drop running farther than the above area-check
	// although, both don't fail on the check.
	// check if <p> is inside 3 verticle planes on the edges
	CVector3D* triPs[3] = {pa, pb, pc};
	CVector3D clstP, planeNormal;
	SimuValue planeD;
	for (int k=0; k < 3; k++)
	{
		GetClosestPointOnLine(triPs[k], triPs[(k+1)%3], triPs[(k+2)%3], &clstP);
		planeNormal.SetValueAsSubstraction(triPs[k], &clstP);
		planeD = planeNormal.DotProduct(triPs[(k+1)%3]);
		SimuValue deltaDist = p->DotProduct(&planeNormal) - planeD;
		if (deltaDist < - SIMU_PARA_TRIANGLE_EDGE_THICKNESS)
			return false;
	}
	return true;

/*
	// check if <p> is on the same side of 3 edges as the reference point, third tri point
	CVector3D* triPs[3] = {pa, pb, pc};
	CVector3D testingEdge, testedEdge, referenceEdge;
	CVector3D cp0, cp1;
	for (int k=0; k < 3; k++)
	{
		referenceEdge.SetValueAsSubstraction(triPs[(k+2)%3], triPs[k]);
		testingEdge.SetValueAsSubstraction(triPs[(k+1)%3], triPs[k]);
		testedEdge.SetValueAsSubstraction(p, triPs[k]);
		cp0.SetValueAsCrossProduct(&testingEdge, &referenceEdge);
		cp1.SetValueAsCrossProduct(&testingEdge, &testedEdge);
		if (CSimuUtility::DotProduct(&cp0, &cp1) < - SIMU_VALUE_EPSILON)
			return false;
	}
	return true;
*/
}

// return true if there is intersection; otherwise, return false.
// the intersection position is returned in <p>
bool CSimuUtility::SegmentIntersectTriangle(CVector3D* p1, CVector3D* p2,
											CVector3D* pa, CVector3D* pb,
											CVector3D* pc, CVector3D* p)
{
	CVector3D n;

	// Calculate the parameters for the plane
	ComputeTriangleNormal(pa, pb, pc, &n);
	SimuValue d = - CVector3D::DotProduct(&n, pa);

	return SegmentIntersectTriangle(p1, p2, pa, pb, pc, &n, d, p);
}

// return true if there is intersection; otherwise, return false.
// the intersection position is returned in <p>
bool CSimuUtility::SegmentIntersectTriangle(CVector3D* p1, CVector3D* p2,
											CVector3D* pa, CVector3D* pb,
											CVector3D* pc, CVector3D* n,
											SimuValue d, CVector3D* p)
{
	SimuValue denom,mu;

	// Calculate the position on the line that intersects the plane
	p->SetValueAsSubstraction(p2, p1);	// p = p2 - p1
	denom = CVector3D::DotProduct(n, p);
	if (fabs(denom) < SIMU_VALUE_EPSILON)	// Line and plane don't intersect
		return false;

	mu = - (d + CVector3D::DotProduct(n, p1)) / denom;
	p->AddedBy(mu, p1);
	if (mu < 0 || mu > 1)
		return false; // Intersection not on line segment

	return IsPointInTriangle(p, pa, pb, pc);
}

bool CSimuUtility::LineIntersectPlane(CVector3D* lineP, CVector3D* lineV,
									  CVector3D* planeP, CVector3D* planeN,
									  CVector3D* intersectP)
{
	CVector3D w;
	w.SetValueAsSubstraction(lineP, planeP);

	SimuValue denom = CVector3D::DotProduct(planeN, lineV);
	SimuValue nom = -CVector3D::DotProduct(planeN, &w);

    if (fabs(denom) < SIMU_VALUE_EPSILON)
	{	// line is parallel to plane
        if (fabs(nom) < SIMU_VALUE_EPSILON)
            return false;	// line lies in plane
        else
            return false;	// no intersection
    }
    // line is not parallel to plane
    // compute intersect param
    SimuValue lineParam = nom / denom;

	intersectP->SetValue(lineP);
	intersectP->AddedBy(lineV, lineParam);

	return true;
}

bool CSimuUtility::HalfLineIntersectPlane(CVector3D* lineP, CVector3D* lineV,
										  CVector3D* planeP, CVector3D* planeN,
										  CVector3D* intersectP)
{
	CVector3D w;
	w.SetValueAsSubstraction(lineP, planeP);

	SimuValue denom = CVector3D::DotProduct(planeN, lineV);
	SimuValue nom = -CVector3D::DotProduct(planeN, &w);

    if (fabs(denom) < SIMU_VALUE_EPSILON)
	{	// line is parallel to plane
        if (fabs(nom) < SIMU_VALUE_EPSILON)
            return false;	// line lies in plane
        else
            return false;	// no intersection
    }
    // line is not parallel to plane
    // compute intersect param
    SimuValue lineParam = nom / denom;
    if (lineParam < 0)
        return false;	// no intersection

	intersectP->SetValue(lineP);
	intersectP->AddedBy(lineV, lineParam);

	return true;
}

bool CSimuUtility::HalfLineIntersectTriangle(CVector3D* p0, CVector3D* dir,
											 CVector3D* pa, CVector3D* pb,
											 CVector3D* pc, CVector3D* p)
{
	CVector3D n;
	ComputeTriangleNormal(pa, pb, pc, &n);

	if (HalfLineIntersectPlane(p0, dir, pa, &n, p) == false)
		return false;
/*
	CVector3D w;
	w.SetValueAsSubstraction(p0, pa);

	SimuValue denom = CVector3D::DotProduct(&n, dir);
	SimuValue nom = -CVector3D::DotProduct(&n, &w);

    if (fabs(denom) < SIMU_VALUE_EPSILON)
	{	// line is parallel to plane
        if (fabs(nom) < SIMU_VALUE_EPSILON)
            return false;	// line lies in plane
        else
            return false;	// no intersection
    }
    // line is not parallel to plane
    // compute intersect param
    SimuValue lineParam = nom / denom;
    if (lineParam < 0)
        return false;	// no intersection

	p->SetValue(p0);
	p->AddedBy(dir, lineParam);
*/
	return IsPointInTriangle(p, pa, pb, pc);
}

// the closest point to <pos> on segment <p0, p1> is returned in clstP
// return 0 if the closest point is p0; return 1 if p1;
// otherwise, return 2.
int CSimuUtility::GetClosestPointOnSegment(CVector3D* pos, CVector3D* p0,
										   CVector3D* p1, CVector3D* clstP)
{
	CVector3D e1;
	CVector3D e2;
	e1.SetValueAsSubstraction(p0, pos);
	e2.SetValueAsSubstraction(p1, p0);
	SimuValue t = - DotProduct(&e1, &e2)/e2.LengthSquare();
	if (t <= 0)
	{
		clstP->SetValue(p0);
		return 0;
	}
	else if (t >= 1)
	{
		clstP->SetValue(p1);
		return 1;
	}
	else // 0 < t < 1
	{
		clstP->SetValue(p0);
		clstP->AddedBy(&e2, t);
		return 2;
	}
}

// the closest point to <pos> on line <p0, p1> is returned in clstP
void CSimuUtility::GetClosestPointOnLine(CVector3D* pos, CVector3D* p0,
										 CVector3D* p1, CVector3D* clstP)
{
	CVector3D e1;
	CVector3D e2;
	e1.SetValueAsSubstraction(p0, pos);
	e2.SetValueAsSubstraction(p1, p0);
	SimuValue t = - DotProduct(&e1, &e2)/e2.LengthSquare();
	clstP->SetValue(p0);
	clstP->AddedBy(&e2, t);
}

// return 0 if the closest point is within the triangle;
// return 1 if is on an edge and between two edge points;
// return 2 if is on a triangle point.
int CSimuUtility::GetClosestPointOnTriangle(CVector3D* pos, 
											CVector3D* p0, CVector3D* p1, CVector3D* p2,
											CVector3D* clstP)
{
	// compute distance to triangle plane
	SimuValue normal[3];
	SimuValue triArea = CSimuUtility::ComputeTriangleNormal(p0, p1, p2, normal);
	if (fabs(triArea) > SIMU_VALUE_EPSILON)
	{
		SimuValue planeD = CSimuUtility::DotProduct3D(normal, p0->v);
		SimuValue posD = - planeD + CSimuUtility::DotProduct3D(normal, pos->v);
		// compute point on triangle plane
		clstP->v[X] = pos->v[X] - normal[X]*posD;
		clstP->v[Y] = pos->v[Y] - normal[Y]*posD;
		clstP->v[Z] = pos->v[Z] - normal[Z]*posD;

		if (CSimuUtility::IsPointInTriangle(clstP, p0, p1, p2))
			return 0;
	}
	// else: triangle is degenerate

	// compute closest point on 3 edges
	CVector3D* p[3] = {p0, p1, p2};
	CVector3D tmpPos;
	SimuValue tmpDist, minDist = SIMU_VALUE_MAX;
	int closingStatus;
	for (int i=0; i < 3; i++)
	{
		int status = CSimuUtility::GetClosestPointOnSegment(pos, p[i], p[(i+1)%3], &tmpPos);
		tmpDist = pos->GetDistanceToVector(&tmpPos);
		if (minDist > tmpDist)
		{
			minDist = tmpDist;
			closingStatus = status;
			clstP->SetValue(&tmpPos);
		}
	}
	if (closingStatus == 2) // closest point is between two edge points
		return 1;
	return 2;
}

// convex hull is a closed triangular mesh.
// the distance is negative if <pos> is inside convex hull; otherwise, outside.
// zero means on convex hull, which is rarely a practical case.
// <normal> points to the closest distance to hull surface if <pos> is inside hull;
// otherwise, zero.
SimuValue CSimuUtility::GetClosestDistanceToConvexHull(CVector3D* pos,
													   TGeomElemArray<CTriangle3DIntNorm>
													   &triangles, CVector3D* normal)
{
	char location[] = "CSimuUtility::GetClosestDistanceToConvexHull(...)";

	CTriangle3DIntNorm* closestTri = NULL;
	int closingStatus;
	SimuValue minDistance = SIMU_VALUE_MAX;
	CPoint3DOnEdge* pPoints[3];
	CVector3D closestPos;
	for (int i=0; i < triangles.m_paNum; i++)
	{
		CTriangle3DIntNorm* pTri = triangles[i];
		pTri->GetTrianglePoints(pPoints);
		int status = CSimuUtility::GetClosestPointOnTriangle(pos, &pPoints[0]->m_p3dPos,
																  &pPoints[1]->m_p3dPos,
																  &pPoints[2]->m_p3dPos,
																  &closestPos);
		SimuValue distSqr = pos->GetDistanceSquareToVector(&closestPos);
		if (minDistance > distSqr)
		{
			minDistance = distSqr;
			closingStatus = status;
			closestTri = pTri;
		}
	}
	minDistance = sqrt(minDistance);
	normal->SetValue((SimuValue)0);
	if (closestTri != NULL)
	{
		CVector3D srfcNormal;
		if (closingStatus == 0) // closest to a triangle
			srfcNormal.SetValue(&closestTri->m_t3dinNormal);
		else if (closingStatus == 1) // closest to an edge
		{
			// search for the closest edge
			CEdge3DOnTriangle* closestEdge = NULL;
			SimuValue delta = SIMU_VALUE_MAX;
			SimuValue edgeLen, pointDist0, pointDist1;
			for (int i=0; i < 3; i++)
			{
				CEdge3DOnTriangle* pEdge = (CEdge3DOnTriangle*)closestTri->m_t3dEdges[i];
				edgeLen = pEdge->m_e3dPoints[0]->m_p3dPos.GetDistanceToVector(
						 &pEdge->m_e3dPoints[1]->m_p3dPos);
				pointDist0 = pos->GetDistanceToVector(&pEdge->m_e3dPoints[0]->m_p3dPos);
				pointDist1 = pos->GetDistanceToVector(&pEdge->m_e3dPoints[1]->m_p3dPos);
				SimuValue tempDelta = pointDist0 + pointDist1 - edgeLen;
				if (delta > fabs(tempDelta))
				{
					delta = fabs(tempDelta);
					closestEdge = pEdge;
				}
			}
			if (closestEdge == NULL)
				CSimuMsg::ExitWithMessage(location, "Closest edge is not found.");
			// search for the triangle sharing the closest edge
			CTriangle3DIntNorm* adjacentTris[2];
			adjacentTris[0] = (CTriangle3DIntNorm*)closestEdge->m_e3dotTris[0];
			adjacentTris[1] = (CTriangle3DIntNorm*)closestEdge->m_e3dotTris[1];
			srfcNormal.SetValue(&adjacentTris[0]->m_t3dinNormal);
			srfcNormal.AddedBy(&adjacentTris[1]->m_t3dinNormal);
		}
		else if (closingStatus == 2) // closest to a point
		{
			CPoint3DOnEdge* closestP = NULL;
			SimuValue closestDist = SIMU_VALUE_MAX;
			closestTri->GetTrianglePoints(pPoints);
			for (int i=0; i < 3; i++)
			{
				SimuValue dist = pos->GetDistanceToVector(&pPoints[i]->m_p3dPos);
				if (closestDist > dist)
				{
					closestDist = dist;
					closestP = pPoints[i];
				}
			}
			if (closestP == NULL)
				CSimuMsg::ExitWithMessage(location, "Closest point is not found.");
			CEdge3DOnTriangle* pEdge;
			CTriangle3DIntNorm* adjacentTris[2];
			for (int j=0; j < closestP->m_p3doeEdges->m_paNum; j++)
			{
				pEdge = (CEdge3DOnTriangle*)closestP->m_p3doeEdges->m_paPtrs[j];
				adjacentTris[0] = (CTriangle3DIntNorm*)pEdge->m_e3dotTris[0];
				adjacentTris[1] = (CTriangle3DIntNorm*)pEdge->m_e3dotTris[1];
				srfcNormal.AddedBy(&adjacentTris[0]->m_t3dinNormal);
				srfcNormal.AddedBy(&adjacentTris[1]->m_t3dinNormal);
			}
		}
		else
			CSimuMsg::ExitWithMessage(location, "Invalid closing status.");
		CVector3D relativePos;
		relativePos.SetValueAsSubstraction(pos,
										&closestTri->m_t3dEdges[0]->m_e3dPoints[0]->m_p3dPos);
		if (relativePos.DotProduct(&srfcNormal) < 0)
			minDistance = -minDistance;
		normal->SetValue(&srfcNormal);
		normal->Normalize();
	}
	return minDistance;
}

void CSimuUtility::GetBoundingBoxForParticles(CVector3D &minPos, CVector3D &maxPos,
											  int numPrtls, CVirtualPrtl** aryPrtls)
{
	minPos[X] = SIMU_VALUE_MAX; maxPos[X] = -SIMU_VALUE_MAX;
	minPos[Y] = SIMU_VALUE_MAX; maxPos[Y] = -SIMU_VALUE_MAX;
	minPos[Z] = SIMU_VALUE_MAX; maxPos[Z] = -SIMU_VALUE_MAX;
	for (int i=0; i < numPrtls; i++)
	{
		CVirtualPrtl* pPrtl = aryPrtls[i];
		for (int d=X; d <= Z; d++)
		{
			if (minPos[d] > pPrtl->m_vpPos->v[d])
				minPos[d] = pPrtl->m_vpPos->v[d];
			if (maxPos[d] < pPrtl->m_vpPos->v[d])
				maxPos[d] = pPrtl->m_vpPos->v[d];
		}
	}
}

// <pos> forms 4 tetrahedra with 4 triangles of the tetrahedron respectivly.
// if the total volume of these 4 tetrahedra is larger than that of the tetrahedron,
// <pos> is outside the tetrahedron and return true; otherwise, inside and return false.
bool CSimuUtility::IsPosInsideTetrahedron(CVector3D* pos,
										  CVector3D* p0, CVector3D* p1,
										  CVector3D* p2, CVector3D* p3)
{
	SimuValue vol0 = ParallelepipedVolume(p0, p1, p2, pos);
	SimuValue vol1 = ParallelepipedVolume(p1, p2, p3, pos);
	SimuValue vol2 = ParallelepipedVolume(p2, p3, p0, pos);
	SimuValue vol3 = ParallelepipedVolume(p3, p0, p1, pos);
	SimuValue vol4 = ParallelepipedVolume(p0, p1, p2, p3);
	SimuValue dvol = fabs(vol0) + fabs(vol1) + fabs(vol2) + fabs(vol3) - fabs(vol4);
	if (dvol > fabs(vol4)*0.001)
		return false;
	return true;
}

// tetrahedron is represented by 4 intersected planes. The plane normals points
// towards the inside of the infinite prism.
// if <pos> is at the other side of a plane than the side pointed by the plane normal,
// then, <pos> is outside tetrahedron. 
bool CSimuUtility::IsPosInsideTetrahedron(CVector3D* pos, bool bNormalPointInside,
										  CVector3D* planeNormals[4], SimuValue planeDs[4])
{
	for (int k=0; k < 4; k++)
	{
		// so that <0> can be used in <if> condition
		if ((bNormalPointInside
			 && pos->DotProduct(planeNormals[k]) - planeDs[k] < 0)
		 || (!bNormalPointInside
			 && pos->DotProduct(planeNormals[k]) - planeDs[k] > 0))
			return false;
	}
	return true;
}

SimuValue CSimuUtility::ParallelepipedVolume(CVector3D* p0, CVector3D* p1, 
											 CVector3D* p2, CVector3D* p3)
{
	CVector3D edge3, areaVector;
	CSimuUtility::CrossProduct(p0, p1, p2, &areaVector);
	edge3.SetValueAsSubstraction(p3, p0);
	SimuValue tmpVolume = CSimuUtility::DotProduct(&areaVector, &edge3);
	return fabs(tmpVolume);
}

bool CSimuUtility::ArePositionsCoplanar(CVector3D* p0, CVector3D* p1, CVector3D* p2, 
										CVector3D* p3, SimuValue tolerance)
{
	if (p0 == p1 || p0 == p2 || p0 == p3
	 || p1 == p2 || p1 == p3 || p2 == p3)
	{
		CSimuMsg::ExitWithMessage("CSimuUtility::ArePositionsCoplanar(...)",
								"4 positions are not distinct.");
		return true; // p0, p1, p2, p3 are not distinct.
	}
	CVector3D triangleNormal;
	CrossProduct(p0, p1, p2, &triangleNormal);
	if (triangleNormal.LengthSquare() <= tolerance)
		return true; // p0, p1, p2 are colinear.
	CVector3D edge;
	edge.SetValueAsSubstraction(p3, p0);
	edge.Normalize();
	SimuValue tmp = CSimuUtility::DotProduct(&triangleNormal, &edge);
	if (fabs(tmp) <= tolerance)
		return true;
	return false;
}

bool CSimuUtility::GetCircumCircleCenter(CVector3D* p0, CVector3D* p1, CVector3D* p2,
										 CVector3D* ctr)
{
	char location[] = "CSimuUtility::GetCircumCircleCenter(...)";

	CVector3D edge0, edge1;
	edge0.SetValueAsSubstraction(p0, p1);
	edge1.SetValueAsSubstraction(p1, p2);
	CVector3D midP0, midP1;
	midP0.SetValueAsAddition(p0, p1); midP0.ScaledBy(0.5);
	midP1.SetValueAsAddition(p1, p2); midP1.ScaledBy(0.5);

	CVector3D triNormal;
	triNormal.SetValueAsCrossProduct(&edge0, &edge1);
	CVector3D lineVector;
	lineVector.SetValueAsCrossProduct(&edge0, &triNormal);

	lineVector.Normalize();
	edge1.Normalize();
	if (!LineIntersectPlane(&midP0, &lineVector, &midP1, &edge1, ctr))
	{
//		CSimuMsg::ExitWithMessage(location, "Circumcenter is not found.");
		return false;
	}
	return true;
}

// the matrix method is about 3% faster than the geometric method
// when running CSimuModeler::OnTestDelaunaytetrahedronobject() with 10000 particles
// and when running a stretching case
#define		SIMU_PARA_CIRCUM_CENTER_BY_MATRIX_METHOD

bool CSimuUtility::GetCircumSphereCenter(CVector3D* p0, CVector3D* p1, CVector3D* p2,
										 CVector3D* p3, CVector3D* ctr)
{
	char location[] = "CSimuUtility::GetCircumSphereCenter(...)";

#ifdef	SIMU_PARA_CIRCUM_CENTER_BY_MATRIX_METHOD
	CVector3D* edges[3] = {m_suEdge0, m_suEdge1, m_suEdge2};
	edges[0]->SetValueAsSubstraction(p0, p3);
	edges[1]->SetValueAsSubstraction(p1, p3);
	edges[2]->SetValueAsSubstraction(p2, p3);

	for (int rowId=0; rowId < m_suEdgeMatrixRow; rowId++)
	{
		for (int colId=0; colId < m_suEdgeMatrixCol-1; colId++)
			m_suEdgeMatrix[rowId][colId] = 2*edges[rowId]->v[colId];
		m_suEdgeMatrix[rowId][m_suEdgeMatrixCol-1] = edges[rowId]->LengthSquare();
	}
	if (CSimuUtility::SolveNxNLinearEquations(m_suEdgeMatrix, m_suCtrPos, m_suEdgeMatrixRow))
	{
		ctr->v[X] = m_suCtrPos[X];
		ctr->v[Y] = m_suCtrPos[Y];
		ctr->v[Z] = m_suCtrPos[Z];
		ctr->AddedBy(p3);
		return true;
	}
	return false;
#else
	m_suEdge0->SetValueAsSubstraction(p0, p3);
	m_suEdge1->SetValueAsSubstraction(p1, p3);
	m_suEdge2->SetValueAsSubstraction(p2, p3);

	m_suMidP0->SetValueAsAddition(p0, p3); m_suMidP0->ScaledBy(0.5);
	m_suMidP1->SetValueAsAddition(p1, p3); m_suMidP1->ScaledBy(0.5);
	m_suMidP2->SetValueAsAddition(p2, p3); m_suMidP2->ScaledBy(0.5);

	m_suTriNormal->SetValueAsCrossProduct(m_suEdge0, m_suEdge1);
	m_suIntersectLineVector0->SetValueAsCrossProduct(m_suEdge0, m_suTriNormal);
	m_suIntersectLineVector0->Normalize();
	m_suEdge1->Normalize();
	if (!LineIntersectPlane(m_suMidP0, m_suIntersectLineVector0, m_suMidP1, m_suEdge1,
							m_suCircumcircleCenter))
	{
//		CSimuMsg::ExitWithMessage(location, "Circum circle center is not found.");
		return false;
	}

	m_suTriNormal->Normalize();
	m_suEdge2->Normalize();
	if (!LineIntersectPlane(m_suCircumcircleCenter, m_suTriNormal, m_suMidP2, m_suEdge2, ctr))
	{
//		CSimuMsg::ExitWithMessage(location, "Circum sphere center is not found.");
		return false;
	}
	return true;
#endif
}
#if 0
#define		SIMU_PARA_LINEAR_KERNEL_NORMALIZER	pow(10.0f, -5.0f)
SimuValue CSimuUtility::LinearDerivativeWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	if (distance >= 0 && distance <= radius)
	{
		weight = SIMU_PARA_LINEAR_KERNEL_NORMALIZER*(distance - 0.5*radius);
	}
	// else weight = 0
	return weight;
}

SimuValue CSimuUtility::LinearSecondDerivativeWeightFunction(SimuValue distance,
															 SimuValue radius)
{
	SimuValue weight = 0;
	if (distance >= 0 && distance <= radius)
	{
		weight = SIMU_PARA_LINEAR_KERNEL_NORMALIZER;
	}
	// else weight = 0
	return weight;
}

#define		SIMU_PARA_SINE_KERNEL_NORMALIZER	pow(10.0f, -5.0f)
SimuValue CSimuUtility::SineWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	if (distance >= 0 && distance <= radius)
	{
		weight = -SIMU_PARA_SINE_KERNEL_NORMALIZER*cos(2*SIMU_PI*distance/radius);
	}
	// else weight = 0
	return weight;
}

SimuValue CSimuUtility::SineFirstDerivativeWeightFunction(SimuValue distance,
														  SimuValue radius)
{
	SimuValue weight = 0;
	if (distance >= 0 && distance <= radius)
	{
		weight = SIMU_PARA_SINE_KERNEL_NORMALIZER*sin(2*SIMU_PI*distance/radius)*2*SIMU_PI/radius;
	}
	// else weight = 0
	return weight;
}

SimuValue CSimuUtility::SineSecondDerivativeWeightFunction(SimuValue distance,
														   SimuValue radius)
{
	SimuValue weight = 0;
	if (distance >= 0 && distance <= radius)
	{
		weight = SIMU_PARA_SINE_KERNEL_NORMALIZER*cos(2*SIMU_PI*distance/radius)*4*SIMU_PI*SIMU_PI/(radius*radius);
	}
	// else weight = 0
	return weight;
}


SimuValue CSimuUtility::SplineWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue h = radius*0.5;
	SimuValue q = distance/h;
	SimuValue weight = 0;
	if (q >= 0 && q < 1)
	{
		weight = 1 - 1.5*pow(q, 2.0) + 0.75*pow(q, 3.0);
	}
	else if (q >= 1 && q < 2)
	{
		weight = 0.25*pow((2.0-q), 3.0);
	}
	else // q < 0
		return weight; // weight = 0
	//weight /= SIMU_PI*pow(h, 3.0);
	weight /= SIMU_PI*h*h*h;

	return weight;
}

SimuValue CSimuUtility::SplineGradientWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue h = 0.5*radius;
	SimuValue q = distance/h;
	SimuValue weight = 0;
	if (q >= 0 && q < 1)
	{
		weight = -3*q + 2.25*q*q; // pow (2.25, 2);
	}
	else if (q >= 1 && q < 2)
	{
		//weight = -0.75*pow((2-q), 2.0f);
		weight = -0.75 * ((2-q) * (2-q));
	}
	else // weight = 0
		return weight;

	//weight /= SIMU_PI*pow(h, 4.0f);
	weight /= SIMU_PI*h*h*h*h;
	return weight;
}

SimuValue CSimuUtility::SplineSecondDerivativeFunction(SimuValue distance, SimuValue radius)
{
	SimuValue h = 0.5*radius;
	SimuValue q = distance/h;
	SimuValue weight = 0;
	if (q >= 0 && q < 1)
	{
		weight = -3 + 4.5*q;
	}
	else if (q >= 1 && q < 2)
	{
		weight = 1.5*(2-q);
	}
	else // weight = 0
		return weight;

	weight /= SIMU_PI*pow(h, 5.0f);
	return weight;
}


SimuValue CSimuUtility::SpikyGradientWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue s = radius - distance;
	SimuValue weight = 0;
	if (s > 0)
	{
//		weight = -14*pow(s, 4);
//		weight *= SIMU_CONST_SPIKY_NORMALIZER/pow(radius, 8);
//		weight = -7*pow(s, 3);
//		weight *= SIMU_CONST_SPIKY_NORMALIZER/pow(radius, 7);
		// original formula
		weight = -3*s*s;  //pow(s, 2.0f);
		weight *= SIMU_CONST_SPIKY_NORMALIZER/pow((double)radius, (double)6.0f);
//		weight = -s;
//		weight *= SIMU_CONST_SPIKY_NORMALIZER/pow(radius, 5);
	}
	// else: weight = 0;
	return weight;
}

SimuValue CSimuUtility::SpikySecondDerivativeFunction(SimuValue distance, SimuValue radius)
{
	SimuValue s = radius - distance;
	SimuValue weight = 0;
	if (s > 0)
	{
		// original formula
		weight = 6*s;
		weight *= SIMU_CONST_SPIKY_NORMALIZER/pow((double)radius, (double)6.0f);
	}
	// else: weight = 0;
	return weight;
}

SimuValue CSimuUtility::LaplacianWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	if (distance < radius)
	{
		weight = radius - distance;
		weight *= SIMU_CONST_PRESSURE_LAPLACIAN/pow(radius, 6);
	}
	return weight;
}

#define		SIMU_PARA_POLY_KERNEL_NORMALIZER	315/(64*SIMU_PI)
SimuValue CSimuUtility::PolyFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	if (distance < radius)
	{
		weight = -6*distance*pow((double)radius*radius - distance*distance , (double)2.0f);
		weight *= SIMU_PARA_POLY_KERNEL_NORMALIZER/pow((double)radius,(double) 9);
	}
	return weight;
}

SimuValue CSimuUtility::PolySecondDerivativeWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	if (distance < radius)
	{
		weight = (radius*radius - distance*distance)*(-6*radius*radius + 30*distance*distance);
		weight *= SIMU_PARA_POLY_KERNEL_NORMALIZER/pow(radius, 9);
	}
	return weight;
}

#define		SIMU_PARA_VISC_KERNEL_NORMALIZER	7.5/SIMU_PI
SimuValue CSimuUtility::ViscFirstDerivativeWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	SimuValue r = distance;
	SimuValue h = radius;
	if (distance < radius)
	{
		weight = -1.5*r*r/pow(h, 3) + 2*r/(h*h) - h/(2*r*r);
		weight *= SIMU_PARA_VISC_KERNEL_NORMALIZER/pow(h, 3);
	}
	return weight;
}

SimuValue CSimuUtility::ViscSecondDerivativeWeightFunction(SimuValue distance, SimuValue radius)
{
	SimuValue weight = 0;
	SimuValue r = distance;
	SimuValue h = radius;
	if (distance < radius)
	{
		weight = -3*r/pow(h, 3) + 2/(h*h) + h/pow(r, 3);
		weight *= SIMU_PARA_VISC_KERNEL_NORMALIZER/pow(h, 3);
	}
	return weight;
}
#endif
bool CSimuUtility::NumberIsWithinRange(int num, int endNum1, int endNum2)
{
	return endNum1 <= num && num <= endNum2 || endNum1 >= num && num >= endNum2;
}

bool CSimuUtility::NumberIsWithinRange(SimuValue num, SimuValue endNum1, SimuValue endNum2)
{
	return endNum1 <= num && num <= endNum2 || endNum1 >= num && num >= endNum2;
}

// return value for angle is between 0 and 180 degree.
SimuValue CSimuUtility::AngleBetween2Vectors(CVector3D* pV0, CVector3D* pV1)
{
	SimuValue dotProduct = DotProduct(pV0, pV1);
	CVector3D crossProduct;
	crossProduct.SetValueAsCrossProduct(pV0, pV1);
	return atan2(crossProduct.Length(), dotProduct)*180/SIMU_PI;
}

SimuValue CSimuUtility::GetAngleOf3Points(CVector3D* pTip0,
										  CVector3D* pMid,
										  CVector3D* pTip1)
{
	CVector3D angleEdge0, angleEdge1;
	angleEdge0.SetValueAsSubstraction(pTip0, pMid);
	angleEdge1.SetValueAsSubstraction(pTip1, pMid);
	return AngleBetween2Vectors(&angleEdge0, &angleEdge1);
}

SimuValue CSimuUtility::ComputeRadiusInOval(CVector3D* pPos, SimuValue ovalRs[3])
{
	SimuValue r = 0;
	for (int d=X; d <= Z; d++)
	{
		r += pow(pPos->v[d]/ovalRs[d], 2);
	}
	r = sqrt(1/r);
	return r;
}

void CSimuUtility::RestoreOpenGLPreviousColor()
{
	SimuValue tmpColor[4];
	SimuglGetv(GL_CURRENT_COLOR, tmpColor);
	SimuColor3d(tmpColor[X], tmpColor[Y], tmpColor[Z]);
}

void CSimuUtility::AdjustBoundingBoxWithPos(CVector3D &minPos,
											CVector3D &maxPos,
											CVector3D* pos)
{
	for (int d=X; d <= Z; d++)
	{
		if (minPos[d] > pos->v[d])
			minPos[d] = pos->v[d];
		if (maxPos[d] < pos->v[d])
			maxPos[d] = pos->v[d];
	}
}

bool CSimuUtility::IsPosOutsideBoundingBox(CVector3D &minPos,
										   CVector3D &maxPos,
										   CVector3D* pos)
{
	for (int d=X; d <= Z; d++)
	{
		if (pos->v[d] < minPos[d])
			return true;
		if (pos->v[d] > maxPos[d])
			return true;
	}
	return false;
}

void CSimuUtility::JitterVectorValue(CVector3D &vect, SimuValue maxDelta)
{
	for (int d=X; d <= Z; d++)
	{
		vect[d] += maxDelta*(2.0*rand()/RAND_MAX - 1);
	}
}

CMeshPrtl* CSimuUtility::AddDistinctMeshPrtl(TGeomElemArray<CMeshPrtl> &aryPrtls,
											 CVector3D &pos, SimuValue colocationEpsilon)
{
	for (int i=0; i < aryPrtls.m_paNum; i++)
		if (aryPrtls[i]->m_vpPos->IsSameVectorAs(&pos, colocationEpsilon))
			return aryPrtls[i];
	CMeshPrtl* newPrtl = new CMeshPrtl();
	newPrtl->m_vpPos->SetValue(&pos);
	aryPrtls.AppendOnePointer(newPrtl);
	return newPrtl;
}
