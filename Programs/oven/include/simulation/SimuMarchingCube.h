// SimuMarchingCube.h: interface for the CSimuMarchingCube class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SIMUMARCHINGCUBE_H__A0A5B45D_7B1C_4D58_BB4C_CA33BF5FF181__INCLUDED_)
#define AFX_SIMUMARCHINGCUBE_H__A0A5B45D_7B1C_4D58_BB4C_CA33BF5FF181__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SimuPoint3D.h"
#include "SimuSrfcPoint3D.h"
#include "SimuTriangle.h"
#include "TClassArray.h"
#include "LinearTemperaTexture.h"

class CSimuMarchingCube  
{
public:
	// 3D array for data on grid vertices
	SimuValue* m_data;	// assigned by caller
	// 3D array for grid vertices
	CSimuPoint3D** m_gridPs;	// assigned by caller

	// triangle vertices, i.e. intersection vertices on grid edges
	TClassArray<CSimuSrfcPoint3D> m_srfcPs;
	SimuValue* m_srfcPointTemperatures;
	// triangles
	TClassArray<CSimuTriangle> m_srfcTris;

	QString m_mcExportVectorFormat;

	CSimuMarchingCube();
	virtual ~CSimuMarchingCube();

	void DrawMarchingCubesByOpenGL();
	void DrawMarchingCubeSurfaceMeshByOpenGL(CLinearTemperaTexture* pLTT);
	void ExportMarchingCubeSurfaceMeshIntoPOVRayMesh2ObjectFile(QString &txFileName, FILE* fp);
	void ImportMarchingCubeSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp);
	void ExportVertexPositions(FILE* fp);
	void ExportVertexNormals(FILE* fp);
	void ExportTextureList(FILE* fp, CLinearTemperaTexture* pLTT);
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
	SimuValue m_v[8];
	CSimuPoint3D* m_P[8];
	int m_case, m_config, m_subconfig;

	// the following are called within void ExecuteMarchingCube();
	void CalculateSrfcPoints();
	SimuValue GetDataValue(int idX, int idY, int idZ);
	CSimuPoint3D* GetGridPoint(int idX, int idY, int idZ);
	int AddSrfcPointOnX();
	int AddSrfcPointOnY();
	int AddSrfcPointOnZ();
	SimuValue GetGradOnX(int idX, int idY, int idZ);
	SimuValue GetGradOnY(int idX, int idY, int idZ);
	SimuValue GetGradOnZ(int idX, int idY, int idZ);
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
