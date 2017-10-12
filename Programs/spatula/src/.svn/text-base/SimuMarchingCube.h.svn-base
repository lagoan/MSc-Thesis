// SimuMarchingCube.h: interface for the CSimuMarchingCube class.
//
//////////////////////////////////////////////////////////////////////

#ifndef SIMUMARCHINGCUBE_H
#define SIMUMARCHINGCUBE_H

#include <vector>

#include "CGALDefinitions.h"

//#include "SimuPoint3D.h"
//#include "SimuSrfcPoint3D.h"
//#include "SimuTriangle.h"
//#include "TClassArray.h"
//#include "LinearTemperaTexture.h"


struct CSimuPoint3D {
	
	double m_x;
	double m_y;
	double m_z;
		
};


struct CSimuSrfcPoint3D : public CSimuPoint3D {
	
	double m_nx;
	double m_ny;
	double m_nz;
		

};

struct CSimuTriangle   {

	int	m_triPid[3];

};

#define LEN_DATA_LINE			2000
#define	SIMU_VALUE_EPSILON		pow(10.0f, -10)
//#define	X						0
//#define	Y						1
//#define	Z						2

class CSimuMarchingCube  
{
public:
	// 3D array for data on grid vertices
	double* m_data;	// assigned by caller
	// 3D array for grid vertices
	CSimuPoint3D** m_gridPs;	// assigned by caller

	// triangle vertices, i.e. intersection vertices on grid edges
	std::vector<CSimuSrfcPoint3D> m_srfcPs;
	double* m_srfcPointTemperatures;
	// triangles
	std::vector<CSimuTriangle> m_srfcTris;
	double m_prtlDistance;
	//QString m_mcExportVectorFormat;

	CSimuMarchingCube();
	virtual ~CSimuMarchingCube();

	//void DrawMarchingCubesByOpenGL();
	//void DrawMarchingCubeSurfaceMeshByOpenGL(CLinearTemperaTexture* pLTT);
	//void ExportMarchingCubeSurfaceMeshIntoPOVRayMesh2ObjectFile(QString &txFileName, FILE* fp);
	void ImportMarchingCubeSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp);
	void ExportVertexPositions(FILE* fp);
	void ExportVertexNormals(FILE* fp);
	//void ExportTextureList(FILE* fp, CLinearTemperaTexture* pLTT);
	void ExportTriangleIndices(FILE* fp);
	void ExportNormalIndices(FILE* fp);
	void ImportVertexPositions(FILE* fp);
	void ImportVertexNormals(FILE* fp);
	void SkipTextureList(FILE* fp);
	void ImportTriangleIndices(FILE* fp);

	void ExecuteMarchingCube(unsigned int gridDim[3]);

	void CreateGridDataStructure(int totalGridPoints);
	void DeleteGridDataStructure();

protected:
	void DeleteSurfaceElements();
	void InitializeTempMemory(int sizeX, int sizeY, int sizeZ);
	void ReleaseTempMemory();
	void ConstructSurfaceMesh();

private:
	int m_sizeX;
	int m_sizeY;
	int m_sizeZ;
	int m_sizePlaneXY;
	// 3D array for indices of intersection vertices
	int* m_idOnX;	// size = (m_sizeX-1)*m_sizeY*m_sizeZ*sizeof(int)
	int* m_idOnY;	// size = m_sizeX*(m_sizeY-1)*m_sizeZ*sizeof(int)
	int* m_idOnZ;	// size = m_sizeX*m_sizeY*(m_sizeZ-1)*sizeof(int)

	// variables for current cube being processed
	int m_numCube; // for debug only
	int m_x, m_y, m_z;
	double m_v[8];
	CSimuPoint3D* m_P[8];
	int m_case, m_config, m_subconfig;

	// the following are called within void ExecuteMarchingCube();
	void CalculateSrfcPoints();
	double GetDataValue(int idX, int idY, int idZ);
	CSimuPoint3D* GetGridPoint(int idX, int idY, int idZ);
	int AddSrfcPointOnX();
	int AddSrfcPointOnY();
	int AddSrfcPointOnZ();
	double GetGradOnX(int idX, int idY, int idZ);
	double GetGradOnY(int idX, int idY, int idZ);
	double GetGradOnZ(int idX, int idY, int idZ);
	void SetSrfcPointIdOnX(int idOnX, int idX, int idY, int idZ);
	void SetSrfcPointIdOnY(int idOnY, int idX, int idY, int idZ);
	void SetSrfcPointIdOnZ(int idOnZ, int idX, int idY, int idZ);
	int GetSrfcPointIdOnX(int idX, int idY, int idZ);
	int GetSrfcPointIdOnY(int idX, int idY, int idZ);
	int GetSrfcPointIdOnZ(int idX, int idY, int idZ);
	void ProcessCurrentCube(int cubeVertexState);
	void AddTriangles(const char* triEdges, char triNum, int v12 = -1);
	bool TestCubeFace(char face);
	bool TestCubeInterior(char s);
	int AddInteriorSrfcPoint();
};

#endif // !defined(AFX_SIMUMARCHINGCUBE_H__A0A5B45D_7B1C_4D58_BB4C_CA33BF5FF181__INCLUDED_)
