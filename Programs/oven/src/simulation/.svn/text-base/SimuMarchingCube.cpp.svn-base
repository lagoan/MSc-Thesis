// SimuMarchingCube.cpp: implementation of the CSimuMarchingCube class.
//
//////////////////////////////////////////////////////////////////////

//#include "stdafx.h"
#include "SimuFlexApp.h"
#include "SimuMarchingCube.h"
#include "SimuMarchingCubeLookupTables.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimuMarchingCube::CSimuMarchingCube()
{
	// to balance between data size and data accuracy,
	// the digit number after decimal point is proportional to CSimuManager::m_prtlDistance
	// that is, epsilon error is not bigger than 1/1000 of CSimuManager::m_prtlDistance.
	SimuValue exp10 = log10(CSimuManager::m_prtlDistance*0.001);
	int numDigits;
	if (exp10 > 0)
		numDigits = 1;
	else
		numDigits = (int)ceil(-exp10);
	char tmpBuf[LEN_DATA_LINE];
	sprintf(tmpBuf, "%%.%df, %%.%df, %%.%df", numDigits, numDigits, numDigits);
	m_mcExportVectorFormat = tmpBuf;

	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeZ = 0;
	m_sizePlaneXY = 0;

	m_data = NULL;
	m_gridPs = NULL;
	m_idOnX = NULL;
	m_idOnY = NULL;
	m_idOnZ = NULL;

	m_srfcPs.InitializeClassArray(true, SIMU_PARA_MEM_ALLOC_SIZE);
	m_srfcTris.InitializeClassArray(true, SIMU_PARA_MEM_ALLOC_SIZE);
	m_srfcPointTemperatures = NULL;
}

CSimuMarchingCube::~CSimuMarchingCube()
{
	DeleteGridDataStructure();
	DeleteSurfaceElements();
	if (m_srfcPointTemperatures != NULL)
		delete m_srfcPointTemperatures;
}

//////////////////////////////////////////////////////////////////////
// Public Functions
//////////////////////////////////////////////////////////////////////
void CSimuMarchingCube::DrawMarchingCubesByOpenGL()
{
	glBegin(GL_LINES);
	for(int k = 0 ; k < m_sizeZ ; k++ )
	for(int j = 0 ; j < m_sizeY ; j++ )
	for(int i = 0 ; i < m_sizeX ; i++ )
	{
		m_v[0] = GetDataValue( i, j, k );
		m_P[0] = GetGridPoint( i, j, k );
		if( i < m_sizeX - 1) {	m_v[1] = GetDataValue(i+1, j , k );
								m_P[1] = GetGridPoint(i+1, j , k );
								glVertex3d(m_P[0]->m_x, m_P[0]->m_y, m_P[0]->m_z);
								glVertex3d(m_P[1]->m_x, m_P[1]->m_y, m_P[1]->m_z); }
		else					m_v[1] = m_v[0];

		if( j < m_sizeY - 1) {	m_v[3] = GetDataValue( i ,j+1, k );
								m_P[3] = GetGridPoint( i ,j+1, k );
								glVertex3d(m_P[0]->m_x, m_P[0]->m_y, m_P[0]->m_z);
								glVertex3d(m_P[3]->m_x, m_P[3]->m_y, m_P[3]->m_z); }
		else					m_v[3] = m_v[0];

		if( k < m_sizeZ - 1) {	m_v[4] = GetDataValue( i , j ,k+1);
								m_P[4] = GetGridPoint( i , j ,k+1 );
								glVertex3d(m_P[0]->m_x, m_P[0]->m_y, m_P[0]->m_z);
								glVertex3d(m_P[4]->m_x, m_P[4]->m_y, m_P[4]->m_z); }
		else					m_v[4] = m_v[0];

	}
	glEnd();
}

void CSimuMarchingCube::DrawMarchingCubeSurfaceMeshByOpenGL(CLinearTemperaTexture* pLTT)
{
	if (CSimuManager::DRAW_TRIANGLE_NORMALS)
	{
		SimuValue normalTipX, normalTipY, normalTipZ;
		glBegin(GL_LINES);
		for (int i=0; i < m_srfcPs.m_paNum; i++)
		{
			CSimuSrfcPoint3D* pP = m_srfcPs[i];
			normalTipX = pP->m_x + pP->m_nx*CSimuManager::m_displayTuner;
			normalTipY = pP->m_y + pP->m_ny*CSimuManager::m_displayTuner;
			normalTipZ = pP->m_z + pP->m_nz*CSimuManager::m_displayTuner;
			SimuVertex3d(pP->m_x, pP->m_y, pP->m_z);
			SimuVertex3d(normalTipX, normalTipY, normalTipZ);
		}
		glEnd();
	}

	if (CSimuManager::DRAW_EDGE_ONLY)
	{
		glBegin(GL_LINES);
		for (int i=0; i < m_srfcTris.m_paNum; i++)
		for (int n=0; n < 3; n++)
		{
			CSimuSrfcPoint3D* p1 = m_srfcPs[m_srfcTris[i]->m_triPid[n]];
			CSimuSrfcPoint3D* p2 = m_srfcPs[m_srfcTris[i]->m_triPid[(n+1)%3]];
			SimuVertex3d(p1->m_x, p1->m_y, p1->m_z);
			SimuVertex3d(p2->m_x, p2->m_y, p2->m_z);
		}
		glEnd();
	}
	else
	{
		glBegin(GL_TRIANGLES);
		for (int i=0; i < m_srfcTris.m_paNum; i++)
		{
			int pointIds[3];
			pointIds[0] = m_srfcTris[i]->m_triPid[0];
			pointIds[1] = m_srfcTris[i]->m_triPid[1];
			pointIds[2] = m_srfcTris[i]->m_triPid[2];
			CSimuSrfcPoint3D* p1 = m_srfcPs[pointIds[0]];
			CSimuSrfcPoint3D* p2 = m_srfcPs[pointIds[1]];
			CSimuSrfcPoint3D* p3 = m_srfcPs[pointIds[2]];
			if (CSimuManager::HEAT_TRANSFER && m_srfcPointTemperatures != NULL)
			{
				SimuValue triTemperature = m_srfcPointTemperatures[pointIds[0]];
				triTemperature += m_srfcPointTemperatures[pointIds[1]];
				triTemperature += m_srfcPointTemperatures[pointIds[2]];
				triTemperature *= SIMU_CONST_ONE_THIRD;
				CVector3D color;
				CTemperatureToTexture::GetDisplayColor(triTemperature, &color, pLTT);
				SimuColor3v(color.v);
			}
			#ifdef	SIMU_PARA_SHADING_SMOOTH
			SimuNormal3d(p1->m_nx, p1->m_ny, p1->m_nz);
			SimuVertex3d(p1->m_x, p1->m_y, p1->m_z);
			SimuNormal3d(p2->m_nx, p2->m_ny, p2->m_nz);
			SimuVertex3d(p2->m_x, p2->m_y, p2->m_z);
			SimuNormal3d(p3->m_nx, p3->m_ny, p3->m_nz);
			SimuVertex3d(p3->m_x, p3->m_y, p3->m_z);
			#else
			SimValue nx = p1->m_nx + p2->m_nx + p3->m_nx; 
			SimValue ny = p1->m_ny + p2->m_ny + p3->m_ny; 
			SimValue nz = p1->m_nz + p2->m_nz + p3->m_nz; 
			SimuNormal3d(nx/3, ny/3, nz/3);
			SimuVertex3d(p1->m_x, p1->m_y, p1->m_z);
			SimuVertex3d(p2->m_x, p2->m_y, p2->m_z);
			SimuVertex3d(p3->m_x, p3->m_y, p3->m_z);
			#endif
		}
		glEnd();
	}
}

void CSimuMarchingCube::ExportMarchingCubeSurfaceMeshIntoPOVRayMesh2ObjectFile(
															QString &txFileName,
															FILE* fp)
{
	// surface point is called vertex in mesh2 object file
	// so, in this function, surface point is called vertex in the comment

	ExportVertexPositions(fp);
	ExportVertexNormals(fp);
	if (CSimuManager::HEAT_TRANSFER)
	{
		fprintf(fp, "#include \"%s\"\n", txFileName);
	}
	ExportTriangleIndices(fp);
	ExportNormalIndices(fp);
}

void CSimuMarchingCube::ImportMarchingCubeSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp)
{
	// surface point is called vertex in mesh2 object file
	// so, in this function, surface point is called vertex in the comment

	ImportVertexPositions(fp);
	ImportVertexNormals(fp);
	if (CSimuManager::HEAT_TRANSFER)
	{
		// skip statement which includes texture file
		char tmpStr[LEN_DATA_LINE];
		fgets(tmpStr, LEN_DATA_LINE, fp);
	}
	ImportTriangleIndices(fp);
	// skip normal indices
}

void CSimuMarchingCube::ExportVertexPositions(FILE* fp)
{
	fprintf(fp, "vertex_vectors {\n");
	fprintf(fp, "%d,\n", m_srfcPs.m_paNum);
	for (int i=0; i < m_srfcPs.m_paNum; i++)
	{
		fprintf(fp, "<");
		fprintf(fp, m_mcExportVectorFormat.toAscii(),
					m_srfcPs[i]->m_x, m_srfcPs[i]->m_y, m_srfcPs[i]->m_z);
		fprintf(fp, ">");
		if (i < m_srfcPs.m_paNum - 1)
			fprintf(fp, ",");
		if (CSimuManager::HEAT_TRANSFER && m_srfcPointTemperatures != NULL)
			fprintf(fp, " // temper = %f", m_srfcPointTemperatures[i]);
		fprintf(fp, "\n");
	}
	fprintf(fp, "}\n");
}

void CSimuMarchingCube::ExportVertexNormals(FILE* fp)
{
	fprintf(fp, "normal_vectors {\n");
	fprintf(fp, "%d", m_srfcPs.m_paNum);
	for (int i=0; i < m_srfcPs.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		fprintf(fp, "<");
		fprintf(fp, m_mcExportVectorFormat.toAscii(),
					m_srfcPs[i]->m_nx, m_srfcPs[i]->m_ny, m_srfcPs[i]->m_nz);
		fprintf(fp, ">");
	}
	fprintf(fp, "\n}\n");
}

void CSimuMarchingCube::ExportTextureList(FILE* fp, CLinearTemperaTexture* pLTT)
{
	QString txString;

	fprintf(fp, "// rCR=%f, cCR=%f, rCG=%f, cCG=%f, rCB=%f, cCB=%f, rF=%f, cF=%f, minT=%f, maxT=%f\n",
		pLTT->m_lttRatioColorR, pLTT->m_lttConstColorR,
		pLTT->m_lttRatioColorG, pLTT->m_lttConstColorG,
		pLTT->m_lttRatioColorB, pLTT->m_lttConstColorB,
		pLTT->m_lttRatioFilter, pLTT->m_lttConstFilter,
		pLTT->m_lttMinTemperature, pLTT->m_lttMaxTemperature);
	fprintf(fp, "texture_list {\n");
	fprintf(fp, "%d", m_srfcPs.m_paNum);
	for (int i=0; i < m_srfcPs.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		CTemperatureToTexture::ConvertScheme(m_srfcPointTemperatures[i], txString, pLTT);
		fprintf(fp, "%s", txString);
	}
	fprintf(fp, "\n}\n");
}

void CSimuMarchingCube::ExportTriangleIndices(FILE* fp)
{
	fprintf(fp, "face_indices {\n");
	fprintf(fp, "%d", m_srfcTris.m_paNum);
	for (int i=0; i < m_srfcTris.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		fprintf(fp, "<%d,%d,%d>",
				m_srfcTris[i]->m_triPid[0],
				m_srfcTris[i]->m_triPid[1],
				m_srfcTris[i]->m_triPid[2]);
		if (CSimuManager::HEAT_TRANSFER)
		{ // export texture idices
			fprintf(fp, ",%d,%d,%d",
					m_srfcTris[i]->m_triPid[0],
					m_srfcTris[i]->m_triPid[1],
					m_srfcTris[i]->m_triPid[2]);
		}
	}
	fprintf(fp, "\n}\n");
}

void CSimuMarchingCube::ExportNormalIndices(FILE* fp)
{
	fprintf(fp, "normal_indices {\n");
	fprintf(fp, "%d", m_srfcTris.m_paNum);
	for (int i=0; i < m_srfcTris.m_paNum; i++)
	{
		fprintf(fp, ",\n");
		// since each vertex has a normal, both vertex and its normal have same index
		fprintf(fp, "<%d,%d,%d>",
				m_srfcTris[i]->m_triPid[0],
				m_srfcTris[i]->m_triPid[1],
				m_srfcTris[i]->m_triPid[2]);
	}
	fprintf(fp, "\n}\n");
}

void CSimuMarchingCube::ImportVertexPositions(FILE* fp)
{
	int number;
	float vectorX, vectorY, vectorZ;

	fscanf(fp, "vertex_vectors {\n");
	fscanf(fp, "%d,\n", &number);
	m_srfcPs.ResetArraySize(number);
	int i;
	char tmpStr[LEN_DATA_LINE];
	float temperature;
	for (i=0; i < m_srfcPs.m_paNum; i++)
	{
		fgets(tmpStr, LEN_DATA_LINE, fp);
		sscanf(tmpStr, "<%f, %f, %f>", &vectorX, &vectorY, &vectorZ);
		char* temperatureStr = strstr(tmpStr, "temper");
		if (temperatureStr != NULL)
		{
			if (i == 0)
			{
				if (m_srfcPointTemperatures != NULL)
					delete m_srfcPointTemperatures;
				m_srfcPointTemperatures = new SimuValue[m_srfcPs.m_paNum];
			}
			sscanf(temperatureStr, "temper = %f", &temperature);
			m_srfcPointTemperatures[i] = temperature;
		}
		m_srfcPs[i]->m_x = vectorX;
		m_srfcPs[i]->m_y = vectorY;
		m_srfcPs[i]->m_z = vectorZ;
	}
	fscanf(fp, "}\n");
}

void CSimuMarchingCube::ImportVertexNormals(FILE* fp)
{
	int number;
	float vectorX, vectorY, vectorZ;

	fscanf(fp, "normal_vectors {\n");
	fscanf(fp, "%d", &number);
	for (int i=0; i < m_srfcPs.m_paNum; i++)
	{
		fscanf(fp, ",\n");
		fscanf(fp, "<%f, %f, %f>", &vectorX, &vectorY, &vectorZ);
		m_srfcPs[i]->m_nx = vectorX;
		m_srfcPs[i]->m_ny = vectorY;
		m_srfcPs[i]->m_nz = vectorZ;
	}
	fscanf(fp, "\n}\n");
}

void CSimuMarchingCube::SkipTextureList(FILE* fp)
{
	int number;
	char tmpStr[LEN_DATA_LINE];

	fgets(tmpStr, LEN_DATA_LINE, fp); // skip the first line which is comment
	fscanf(fp, "texture_list {\n");
	fscanf(fp, "%d,\n", &number);
	if (number != m_srfcPs.m_paNum)
		CSimuMsg::ExitWithMessage("CSimuMarchingCube::SkipTextureList(fp)",
								"Wrong number of textures.");
	for (int i=0; i < m_srfcPs.m_paNum; i++)
	{
		fgets(tmpStr, LEN_DATA_LINE, fp);
	}
	fscanf(fp, "}\n");
}

void CSimuMarchingCube::ImportTriangleIndices(FILE* fp)
{
	int number;
	char tmpStr[LEN_DATA_LINE];

	fscanf(fp, "face_indices {\n");
	fscanf(fp, "%d,\n", &number);
	m_srfcTris.ResetArraySize(number);
	for (int i=0; i < m_srfcTris.m_paNum; i++)
	{
		fgets(tmpStr, LEN_DATA_LINE, fp);
		// since each vertex has a normal, both vertex and its normal have same index
		sscanf(tmpStr, "<%d,%d,%d>",
				&m_srfcTris[i]->m_triPid[0],
				&m_srfcTris[i]->m_triPid[1],
				&m_srfcTris[i]->m_triPid[2]);
	}
	fscanf(fp, "}\n");
}

void CSimuMarchingCube::ExecuteMarchingCube(unsigned int gridDim[3])
{
	DeleteSurfaceElements();
	InitializeTempMemory(gridDim[X], gridDim[Y], gridDim[Z]);
	ConstructSurfaceMesh();
	ReleaseTempMemory();
#ifndef	SIMU_PARA_DEBUG_MARCHING_CUBE_GRID_STRUCTURE
	DeleteGridDataStructure();
#endif
}

void CSimuMarchingCube::CreateGridDataStructure(int totalGridPoints)
{
	DeleteGridDataStructure();

	if (totalGridPoints > pow(10.0f, 8))
		CSimuMsg::ExitWithMessage("CSimuMarchingCube::CreateGridDataStructure(totalGridPoints)", 
									"Too many grid cells: %d.", totalGridPoints);
	m_data = new SimuValue[totalGridPoints];
	m_gridPs = new CSimuPoint3D*[totalGridPoints];
}

void CSimuMarchingCube::DeleteGridDataStructure()
{
	int numPoints = m_sizeX*m_sizeY*m_sizeZ;
	for (int n=0; n < numPoints; n++)
		delete m_gridPs[n];
	if (m_gridPs != NULL)	{ delete m_gridPs; m_gridPs = NULL; }
	if (m_data != NULL)		{ delete m_data; m_data = NULL; }
	m_sizeX = 0;
	m_sizeY = 0;
	m_sizeZ = 0;
	m_sizePlaneXY = 0;
}

void CSimuMarchingCube::DeleteSurfaceElements()
{
	m_srfcPs.ResetArraySize(0);
	m_srfcTris.ResetArraySize(0);
}

void CSimuMarchingCube::InitializeTempMemory(int sizeX, int sizeY, int sizeZ)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;
	m_sizeZ = sizeZ;
	m_sizePlaneXY = m_sizeX*m_sizeY;
	m_idOnX = new int[m_sizeX*m_sizeY*m_sizeZ];
	m_idOnY = new int[m_sizeX*m_sizeY*m_sizeZ];
	m_idOnZ = new int[m_sizeX*m_sizeY*m_sizeZ];
	memset( m_idOnX, -1, m_sizeX*m_sizeY*m_sizeZ * sizeof( int ) ) ;
	memset( m_idOnY, -1, m_sizeX*m_sizeY*m_sizeZ * sizeof( int ) ) ;
	memset( m_idOnZ, -1, m_sizeX*m_sizeY*m_sizeZ * sizeof( int ) ) ;
}

void CSimuMarchingCube::ReleaseTempMemory()
{
	delete m_idOnX;
	delete m_idOnY;
	delete m_idOnZ;
}

void CSimuMarchingCube::ConstructSurfaceMesh()
{
	CalculateSrfcPoints();

	m_numCube = 0;
	for(m_z = 0 ; m_z < m_sizeZ-1 ; m_z++ )
	for(m_y = 0 ; m_y < m_sizeY-1 ; m_y++ )
	for(m_x = 0 ; m_x < m_sizeX-1 ; m_x++ )
	{
		int cubeVertexState = 0 ;
		if( ( m_v[0] = GetDataValue( m_x , m_y , m_z ) ) > 0 ) cubeVertexState +=   1 ;
		if( ( m_v[1] = GetDataValue(m_x+1, m_y , m_z ) ) > 0 ) cubeVertexState +=   2 ;
		if( ( m_v[2] = GetDataValue(m_x+1,m_y+1, m_z ) ) > 0 ) cubeVertexState +=   4 ;
		if( ( m_v[3] = GetDataValue( m_x ,m_y+1, m_z ) ) > 0 ) cubeVertexState +=   8 ;
		if( ( m_v[4] = GetDataValue( m_x , m_y ,m_z+1) ) > 0 ) cubeVertexState +=  16 ;
		if( ( m_v[5] = GetDataValue(m_x+1, m_y ,m_z+1) ) > 0 ) cubeVertexState +=  32 ;
		if( ( m_v[6] = GetDataValue(m_x+1,m_y+1,m_z+1) ) > 0 ) cubeVertexState +=  64 ;
		if( ( m_v[7] = GetDataValue( m_x ,m_y+1,m_z+1) ) > 0 ) cubeVertexState += 128 ;
		ProcessCurrentCube(cubeVertexState) ;
		m_numCube++;
	}
}

//////////////////////////////////////////////////////////////////////
// Private Functions
//////////////////////////////////////////////////////////////////////
void CSimuMarchingCube::CalculateSrfcPoints()
{
	for(m_z = 0 ; m_z < m_sizeZ ; m_z++ )
	for(m_y = 0 ; m_y < m_sizeY ; m_y++ )
	for(m_x = 0 ; m_x < m_sizeX ; m_x++ )
	{
		m_v[0] = GetDataValue( m_x, m_y, m_z );
		m_P[0] = GetGridPoint( m_x, m_y, m_z );
		if( m_x < m_sizeX - 1) {m_v[1] = GetDataValue(m_x+1, m_y , m_z );
								m_P[1] = GetGridPoint(m_x+1, m_y , m_z ); }
		else					m_v[1] = m_v[0];

		if( m_y < m_sizeY - 1) {m_v[3] = GetDataValue( m_x ,m_y+1, m_z );
								m_P[3] = GetGridPoint( m_x ,m_y+1, m_z ); }
		else					m_v[3] = m_v[0];

		if( m_z < m_sizeZ - 1) {m_v[4] = GetDataValue( m_x , m_y ,m_z+1);
								m_P[4] = GetGridPoint( m_x , m_y ,m_z+1 ); }
		else					m_v[4] = m_v[0];

		// use -SIMU_VALUE_EPSILON such that the point is classifed as being outside,
		// which reduces the chance of the single-inside-point cube configuration
		if( fabs(m_v[0]) < SIMU_VALUE_EPSILON ) m_v[0] = -SIMU_VALUE_EPSILON;
		if( fabs(m_v[1]) < SIMU_VALUE_EPSILON ) m_v[1] = -SIMU_VALUE_EPSILON;
		if( fabs(m_v[3]) < SIMU_VALUE_EPSILON ) m_v[3] = -SIMU_VALUE_EPSILON;
		if( fabs(m_v[4]) < SIMU_VALUE_EPSILON ) m_v[4] = -SIMU_VALUE_EPSILON;

		if( m_v[0] < 0 )
		{
		  if( m_v[1] > 0 ) SetSrfcPointIdOnX( AddSrfcPointOnX(), m_x,m_y,m_z ) ;
		  if( m_v[3] > 0 ) SetSrfcPointIdOnY( AddSrfcPointOnY(), m_x,m_y,m_z ) ;
		  if( m_v[4] > 0 ) SetSrfcPointIdOnZ( AddSrfcPointOnZ(), m_x,m_y,m_z ) ;
		}
		else
		{
		  if( m_v[1] < 0 ) SetSrfcPointIdOnX( AddSrfcPointOnX(), m_x,m_y,m_z ) ;
		  if( m_v[3] < 0 ) SetSrfcPointIdOnY( AddSrfcPointOnY(), m_x,m_y,m_z ) ;
		  if( m_v[4] < 0 ) SetSrfcPointIdOnZ( AddSrfcPointOnZ(), m_x,m_y,m_z ) ;
		}
	}
}

SimuValue CSimuMarchingCube::GetDataValue(int idX, int idY, int idZ)
{
	return m_data[ idX + idY*m_sizeX + idZ*m_sizePlaneXY];
}

CSimuPoint3D* CSimuMarchingCube::GetGridPoint(int idX, int idY, int idZ)
{
	return m_gridPs[ idX + idY*m_sizeX + idZ*m_sizePlaneXY];
}

int CSimuMarchingCube::AddSrfcPointOnX()
{
	CSimuSrfcPoint3D* newP = m_srfcPs.CreateOneElement();

	SimuValue u = ( m_v[0] - 0) / ( m_v[0] - m_v[1] ) ;

	newP->m_x = m_P[0]->m_x + u*(m_P[1]->m_x - m_P[0]->m_x);
	newP->m_y = m_P[0]->m_y;
	newP->m_z = m_P[0]->m_z;

	newP->m_nx = (1-u)*GetGradOnX(m_x,m_y,m_z) + u*GetGradOnX(m_x+1,m_y,m_z) ;
	newP->m_ny = (1-u)*GetGradOnY(m_x,m_y,m_z) + u*GetGradOnY(m_x+1,m_y,m_z) ;
	newP->m_nz = (1-u)*GetGradOnZ(m_x,m_y,m_z) + u*GetGradOnZ(m_x+1,m_y,m_z) ;

	u = (float) sqrt( newP->m_nx * newP->m_nx + newP->m_ny * newP->m_ny +newP->m_nz * newP->m_nz ) ;
	if( u > 0 )
	{
	u *= -1; // flip the normal to point out from surface
	newP->m_nx /= u ;
	newP->m_ny /= u ;
	newP->m_nz /= u ;
	}

	return m_srfcPs.m_paNum-1 ;
}

int CSimuMarchingCube::AddSrfcPointOnY()
{
	CSimuSrfcPoint3D* newP = m_srfcPs.CreateOneElement();

	SimuValue u = ( m_v[0] - 0) / ( m_v[0] - m_v[3] ) ;

	newP->m_x = m_P[0]->m_x;
	newP->m_y = m_P[0]->m_y + u*(m_P[3]->m_y - m_P[0]->m_y);
	newP->m_z = m_P[0]->m_z;

	newP->m_nx = (1-u)*GetGradOnX(m_x,m_y,m_z) + u*GetGradOnX(m_x,m_y+1,m_z) ;
	newP->m_ny = (1-u)*GetGradOnY(m_x,m_y,m_z) + u*GetGradOnY(m_x,m_y+1,m_z) ;
	newP->m_nz = (1-u)*GetGradOnZ(m_x,m_y,m_z) + u*GetGradOnZ(m_x,m_y+1,m_z) ;

	u = (float) sqrt( newP->m_nx * newP->m_nx + newP->m_ny * newP->m_ny +newP->m_nz * newP->m_nz ) ;
	if( u > 0 )
	{
	u *= -1; // flip the normal to point out from surface
	newP->m_nx /= u ;
	newP->m_ny /= u ;
	newP->m_nz /= u ;
	}

	return m_srfcPs.m_paNum-1 ;
}

int CSimuMarchingCube::AddSrfcPointOnZ()
{
	CSimuSrfcPoint3D* newP = m_srfcPs.CreateOneElement();

	SimuValue u = ( m_v[0] - 0) / ( m_v[0] - m_v[4] ) ;

	newP->m_x = m_P[0]->m_x;
	newP->m_y = m_P[0]->m_y;
	newP->m_z = m_P[0]->m_z + u*(m_P[4]->m_z - m_P[0]->m_z);

	newP->m_nx = (1-u)*GetGradOnX(m_x,m_y,m_z) + u*GetGradOnX(m_x,m_y,m_z+1) ;
	newP->m_ny = (1-u)*GetGradOnY(m_x,m_y,m_z) + u*GetGradOnY(m_x,m_y,m_z+1) ;
	newP->m_nz = (1-u)*GetGradOnZ(m_x,m_y,m_z) + u*GetGradOnZ(m_x,m_y,m_z+1) ;

	u = (float) sqrt( newP->m_nx * newP->m_nx + newP->m_ny * newP->m_ny +newP->m_nz * newP->m_nz ) ;
	if( u > 0 )
	{
	u *= -1; // flip the normal to point out from surface
	newP->m_nx /= u ;
	newP->m_ny /= u ;
	newP->m_nz /= u ;
	}

	return m_srfcPs.m_paNum-1 ;
}

SimuValue CSimuMarchingCube::GetGradOnX(int idX, int idY, int idZ)
{
  if( idX > 0 )
  {
    if ( idX < m_sizeX - 1 )
      return ( GetDataValue( idX+1, idY, idZ ) - GetDataValue( idX-1, idY, idZ ) ) / 2 ;
    else
      return GetDataValue( idX, idY, idZ ) - GetDataValue( idX-1, idY, idZ ) ;
  }
  else
    return GetDataValue( idX+1, idY, idZ ) - GetDataValue( idX, idY, idZ ) ;
}

SimuValue CSimuMarchingCube::GetGradOnY(int idX, int idY, int idZ)
{
  if( idY > 0 )
  {
    if ( idY < m_sizeY - 1 )
      return ( GetDataValue( idX, idY+1, idZ ) - GetDataValue( idX, idY-1, idZ ) ) / 2 ;
    else
      return GetDataValue( idX, idY, idZ ) - GetDataValue( idX, idY-1, idZ ) ;
  }
  else
    return GetDataValue( idX, idY+1, idZ ) - GetDataValue( idX, idY, idZ ) ;
}

SimuValue CSimuMarchingCube::GetGradOnZ(int idX, int idY, int idZ)
{
  if( idZ > 0 )
  {
    if ( idZ < m_sizeZ - 1 )
      return ( GetDataValue( idX, idY, idZ+1 ) - GetDataValue( idX, idY, idZ-1 ) ) / 2 ;
    else
      return GetDataValue( idX, idY, idZ ) - GetDataValue( idX, idY, idZ-1 ) ;
  }
  else
    return GetDataValue( idX, idY, idZ+1 ) - GetDataValue( idX, idY, idZ ) ;
}

void CSimuMarchingCube::SetSrfcPointIdOnX(int idOnX, int idX, int idY, int idZ)
{
	m_idOnX[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ] = idOnX;
}

void CSimuMarchingCube::SetSrfcPointIdOnY(int idOnY, int idX, int idY, int idZ)
{
	m_idOnY[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ] = idOnY;
}

void CSimuMarchingCube::SetSrfcPointIdOnZ(int idOnZ, int idX, int idY, int idZ)
{
	m_idOnZ[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ] = idOnZ;
}

int CSimuMarchingCube::GetSrfcPointIdOnX(int idX, int idY, int idZ)
{
	return m_idOnX[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ];
}

int CSimuMarchingCube::GetSrfcPointIdOnY(int idX, int idY, int idZ)
{
	return m_idOnY[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ];
}

int CSimuMarchingCube::GetSrfcPointIdOnZ(int idX, int idY, int idZ)
{
	return m_idOnZ[ idX + idY*m_sizeX+ idZ*m_sizePlaneXY ];
}

// indices, corner data and points of current cube are stored in
// m_x, m_y, m_z, m_v[8] and m_P[8]
void CSimuMarchingCube::ProcessCurrentCube(int cubeVertexState)
{
	char* location="CSimuMarchingCube::ProcessCurrentCube(cubeVertexState)";

  m_case   = cases[cubeVertexState][0] ;
  m_config = cases[cubeVertexState][1] ;

  int   v12 = -1 ;
  m_subconfig = 0 ;

  switch( m_case )
  {
  case  0 :
    break ;

  case  1 :
    AddTriangles( tiling1[m_config], 1 ) ;
    break ;

  case  2 :
    AddTriangles( tiling2[m_config], 2 ) ;
    break ;

  case  3 :
    if( TestCubeFace( test3[m_config]) )
      AddTriangles( tiling3[m_config]     , 4 ) ; // 3.2
    else
      AddTriangles( tiling3[m_config] + 12, 2 ) ; // 3.1
    break ;

  case  4 :
    if( TestCubeInterior( test4[m_config]) )
      AddTriangles( tiling4[m_config]     , 2 ) ; // 4.1.1
    else
      AddTriangles( tiling4[m_config] +  6, 6 ) ; // 4.1.2
    break ;

  case  5 :
    AddTriangles( tiling5[m_config], 3 ) ;
    break ;

  case  6 :
    if( TestCubeFace( test6[m_config][0]) )
      AddTriangles( tiling6[m_config], 5 ) ; // 6.2
    else
    {
      if( TestCubeInterior( test6[m_config][1]) )
        AddTriangles( tiling6[m_config] + 15, 3 ) ; // 6.1.1
      else
        AddTriangles( tiling6[m_config] + 24, 7 ) ; // 6.1.2
    }
    break ;

  case  7 :
    if( TestCubeFace( test7[m_config][0] ) ) m_subconfig +=  1 ;
    if( TestCubeFace( test7[m_config][1] ) ) m_subconfig +=  2 ;
    if( TestCubeFace( test7[m_config][2] ) ) m_subconfig +=  4 ;
    switch( subconfig7[m_subconfig] )
      {
      case 0 :
        if( TestCubeInterior( test7[m_config][3]) )
          AddTriangles( tiling7[m_config] + 15, 9 ) ;
        else
          AddTriangles( tiling7[m_config]     , 5 ) ;
        break ;
      case 1 :
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling7[m_config] +  42, 9, v12 ) ; break ;
      case 2 :
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling7[m_config] +  69, 9, v12 ) ; break ;
      case 3 :
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling7[m_config] +  96, 9, v12 ) ; break ;
      case 4 :
        AddTriangles( tiling7[m_config] + 123, 5 ) ; break ;
      case 5 :
        AddTriangles( tiling7[m_config] + 138, 5 ) ; break ;
      case 6 :
        AddTriangles( tiling7[m_config] + 153, 5 ) ; break ;
      case 7 :
        AddTriangles( tiling7[m_config] + 168, 3 ) ; break ;
      };
    break ;

  case  8 :
    AddTriangles( tiling8[m_config], 2 ) ;
    break ;

  case  9 :
    AddTriangles( tiling9[m_config], 4 ) ;
    break ;

  case 10 :
    if( TestCubeFace( test10[m_config][0]) )
    {
      if( TestCubeFace( test10[m_config][1]) )
        AddTriangles( tiling10[m_config] + 60, 4 ) ; // 10.1.1
      else
      {
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling10[m_config]     , 8, v12 ) ; // 10.2
      }
    }
    else
    {
      if( TestCubeFace( test10[m_config][1]) )
      {
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling10[m_config] + 24, 8, v12 ) ; // 10.2
      }
      else
      {
        if( TestCubeInterior( test10[m_config][2]) )
          AddTriangles( tiling10[m_config] + 48, 4 ) ; // 10.1.1
        else
          AddTriangles( tiling10[m_config] + 72, 8 ) ; // 10.1.2
      }
    }
    break ;

  case 11 :
    AddTriangles( tiling11[m_config], 4 ) ;
    break ;

  case 12 :
    if( TestCubeFace( test12[m_config][0]) )
    {
      if( TestCubeFace( test12[m_config][1]) )
        AddTriangles( tiling12[m_config] + 60, 4 ) ; // 12.1.1
      else
      {
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling12[m_config]     , 8, v12 ) ; // 12.2
      }
    }
    else
    {
      if( TestCubeFace( test12[m_config][1]) )
      {
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling12[m_config] + 24, 8, v12 ) ; // 12.2
      }
      else
      {
        if( TestCubeInterior( test12[m_config][2]) )
          AddTriangles( tiling12[m_config] + 48, 4 ) ; // 12.1.1
        else
          AddTriangles( tiling12[m_config] + 72, 8 ) ; // 12.1.2
      }
    }
    break ;

  case 13 :
    if( TestCubeFace( test13[m_config][0] ) ) m_subconfig +=  1 ;
    if( TestCubeFace( test13[m_config][1] ) ) m_subconfig +=  2 ;
    if( TestCubeFace( test13[m_config][2] ) ) m_subconfig +=  4 ;
    if( TestCubeFace( test13[m_config][3] ) ) m_subconfig +=  8 ;
    if( TestCubeFace( test13[m_config][4] ) ) m_subconfig += 16 ;
    if( TestCubeFace( test13[m_config][5] ) ) m_subconfig += 32 ;
    switch( subconfig13[m_subconfig] )
    {
      case 0 :/* 13.1 */
        AddTriangles( tiling13_1[m_config], 4 ) ; break ;

      case 1 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][0], 6 ) ; break ;
      case 2 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][1], 6 ) ; break ;
      case 3 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][2], 6 ) ; break ;
      case 4 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][3], 6 ) ; break ;
      case 5 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][4], 6 ) ; break ;
      case 6 :/* 13.2 */
        AddTriangles( tiling13_2[m_config][5], 6 ) ; break ;

      case 7 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][0], 10, v12 ) ; break ;
      case 8 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][1], 10, v12 ) ; break ;
      case 9 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][2], 10, v12 ) ; break ;
      case 10 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][3], 10, v12 ) ; break ;
      case 11 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][4], 10, v12 ) ; break ;
      case 12 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][5], 10, v12 ) ; break ;
      case 13 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][6], 10, v12 ) ; break ;
      case 14 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][7], 10, v12 ) ; break ;
      case 15 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][8], 10, v12 ) ; break ;
      case 16 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][9], 10, v12 ) ; break ;
      case 17 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][10], 10, v12 ) ; break ;
      case 18 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3[m_config][11], 10, v12 ) ; break ;

      case 19 :/* 13.4 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_4[m_config][0], 12, v12 ) ; break ;
      case 20 :/* 13.4 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_4[m_config][1], 12, v12 ) ; break ;
      case 21 :/* 13.4 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_4[m_config][2], 12, v12 ) ; break ;
      case 22 :/* 13.4 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_4[m_config][3], 12, v12 ) ; break ;

      case 23 :/* 13.5 */
        m_subconfig = 0 ;
        if( TestCubeInterior( test13[m_config][6] ) )
          AddTriangles( tiling13_5_1[m_config][0], 6 ) ;
        else
          AddTriangles( tiling13_5_2[m_config][0], 10 ) ;
        break ;
      case 24 :/* 13.5 */
        m_subconfig = 1 ;
        if( TestCubeInterior( test13[m_config][6] ) )
          AddTriangles( tiling13_5_1[m_config][1], 6 ) ;
        else
          AddTriangles( tiling13_5_2[m_config][1], 10 ) ;
        break ;
      case 25 :/* 13.5 */
        m_subconfig = 2 ;
        if( TestCubeInterior( test13[m_config][6] ) )
          AddTriangles( tiling13_5_1[m_config][2], 6 ) ;
        else
          AddTriangles( tiling13_5_2[m_config][2], 10 ) ;
        break ;
      case 26 :/* 13.5 */
        m_subconfig = 3 ;
        if( TestCubeInterior( test13[m_config][6] ) )
          AddTriangles( tiling13_5_1[m_config][3], 6 ) ;
        else
          AddTriangles( tiling13_5_2[m_config][3], 10 ) ;
        break ;

      case 27 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][0], 10, v12 ) ; break ;
      case 28 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][1], 10, v12 ) ; break ;
      case 29 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][2], 10, v12 ) ; break ;
      case 30 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][3], 10, v12 ) ; break ;
      case 31 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][4], 10, v12 ) ; break ;
      case 32 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][5], 10, v12 ) ; break ;
      case 33 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][6], 10, v12 ) ; break ;
      case 34 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][7], 10, v12 ) ; break ;
      case 35 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][8], 10, v12 ) ; break ;
      case 36 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][9], 10, v12 ) ; break ;
      case 37 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][10], 10, v12 ) ; break ;
      case 38 :/* 13.3 */
        v12 = AddInteriorSrfcPoint() ;
        AddTriangles( tiling13_3_[m_config][11], 10, v12 ) ; break ;

      case 39 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][0], 6 ) ; break ;
      case 40 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][1], 6 ) ; break ;
      case 41 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][2], 6 ) ; break ;
      case 42 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][3], 6 ) ; break ;
      case 43 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][4], 6 ) ; break ;
      case 44 :/* 13.2 */
        AddTriangles( tiling13_2_[m_config][5], 6 ) ; break ;

      case 45 :/* 13.1 */
        AddTriangles( tiling13_1_[m_config], 4 ) ; break ;

      default :
        printf("Marching Cubes: Impossible case 13?\n" ) ;
      }
      break ;

  case 14 :
    AddTriangles( tiling14[m_config], 4 ) ;
    break ;
  };
}

void CSimuMarchingCube::AddTriangles(const char* triEdges, char triNum, int v12)
{
	char* location = "CSimuMarchingCube::AddTriangles(...)";

	int    tv[3] ;	// three triangle m_srfcPs

	for( int t = 0 ; t < 3*triNum ; t++ )
	{
		switch( triEdges[t] )
		{
		case  0 : tv[ t % 3 ] = GetSrfcPointIdOnX( m_x , m_y , m_z ) ; break ;
		case  1 : tv[ t % 3 ] = GetSrfcPointIdOnY(m_x+1, m_y , m_z ) ; break ;
		case  2 : tv[ t % 3 ] = GetSrfcPointIdOnX( m_x ,m_y+1, m_z ) ; break ;
		case  3 : tv[ t % 3 ] = GetSrfcPointIdOnY( m_x , m_y , m_z ) ; break ;
		case  4 : tv[ t % 3 ] = GetSrfcPointIdOnX( m_x , m_y ,m_z+1) ; break ;
		case  5 : tv[ t % 3 ] = GetSrfcPointIdOnY(m_x+1, m_y ,m_z+1) ; break ;
		case  6 : tv[ t % 3 ] = GetSrfcPointIdOnX( m_x ,m_y+1,m_z+1) ; break ;
		case  7 : tv[ t % 3 ] = GetSrfcPointIdOnY( m_x , m_y ,m_z+1) ; break ;
		case  8 : tv[ t % 3 ] = GetSrfcPointIdOnZ( m_x , m_y , m_z ) ; break ;
		case  9 : tv[ t % 3 ] = GetSrfcPointIdOnZ(m_x+1, m_y , m_z ) ; break ;
		case 10 : tv[ t % 3 ] = GetSrfcPointIdOnZ(m_x+1,m_y+1, m_z ) ; break ;
		case 11 : tv[ t % 3 ] = GetSrfcPointIdOnZ( m_x ,m_y+1, m_z ) ; break ;
		case 12 : tv[ t % 3 ] = v12 ; break ;
		default : break ;
		}

		if( tv[t%3] == -1 )
		{
			CSimuMsg::ExitWithMessage(location, "Marching Cubes: invalid triangle\n");
		}

		if( t%3 == 2 )
		{
			CSimuTriangle* newSrfcTri = m_srfcTris.CreateOneElement();

			newSrfcTri->m_triPid[0] = tv[0];
			newSrfcTri->m_triPid[1] = tv[1];
			newSrfcTri->m_triPid[2] = tv[2];
		}
	}
}

bool CSimuMarchingCube::TestCubeFace(char face)
{
	char* location = "CSimuMarchingCube::TestCubeFace(face)";

	float A,B,C,D ;

	switch( face )
	{
	case -1 : case 1 :  A = m_v[0] ;  B = m_v[4] ;  C = m_v[5] ;  D = m_v[1] ;  break ;
	case -2 : case 2 :  A = m_v[1] ;  B = m_v[5] ;  C = m_v[6] ;  D = m_v[2] ;  break ;
	case -3 : case 3 :  A = m_v[2] ;  B = m_v[6] ;  C = m_v[7] ;  D = m_v[3] ;  break ;
	case -4 : case 4 :  A = m_v[3] ;  B = m_v[7] ;  C = m_v[4] ;  D = m_v[0] ;  break ;
	case -5 : case 5 :  A = m_v[0] ;  B = m_v[3] ;  C = m_v[2] ;  D = m_v[1] ;  break ;
	case -6 : case 6 :  A = m_v[4] ;  B = m_v[7] ;  C = m_v[6] ;  D = m_v[5] ;  break ;
	default : CSimuMsg::ExitWithMessage(location, "Invalid face code\n") ;  A = B = C = D = 0 ;
	};

	return face * A * ( A*C - B*D ) >= 0  ;  // face and A invert signs
}

bool CSimuMarchingCube::TestCubeInterior(char s)
{
	char* location = "CSimuMarchingCube::TestCubeInterior(s)";

	float t, At=0, Bt=0, Ct=0, Dt=0, a, b ;
	char  test =  0 ;
	char  edge = -1 ; // reference edge of the triangulation

	switch( m_case )
	{
	case  4 :
	case 10 :
	a = ( m_v[4] - m_v[0] ) * ( m_v[6] - m_v[2] ) - ( m_v[7] - m_v[3] ) * ( m_v[5] - m_v[1] ) ;
	b =  m_v[2] * ( m_v[4] - m_v[0] ) + m_v[0] * ( m_v[6] - m_v[2] )
			 - m_v[1] * ( m_v[7] - m_v[3] ) - m_v[3] * ( m_v[5] - m_v[1] ) ;
	t = - b / (2*a) ;
	if( t<0 || t>1 ) return s>0 ;

	At = m_v[0] + ( m_v[4] - m_v[0] ) * t ;
	Bt = m_v[3] + ( m_v[7] - m_v[3] ) * t ;
	Ct = m_v[2] + ( m_v[6] - m_v[2] ) * t ;
	Dt = m_v[1] + ( m_v[5] - m_v[1] ) * t ;
	break ;

	case  6 :
	case  7 :
	case 12 :
	case 13 :
	switch( m_case )
	{
	case  6 : edge = tiling6 [m_config][15] ; break ;
	case  7 : edge = tiling7 [m_config][13] ; break ;
	case 12 : edge = tiling12[m_config][14] ; break ;
	case 13 : edge = tiling13_5_1[m_config][m_subconfig][2] ; break ;
	}
	switch( edge )
	{
	case  0 :
	  t  = m_v[0] / ( m_v[0] - m_v[1] ) ;
	  At = 0 ;
	  Bt = m_v[3] + ( m_v[2] - m_v[3] ) * t ;
	  Ct = m_v[7] + ( m_v[6] - m_v[7] ) * t ;
	  Dt = m_v[4] + ( m_v[5] - m_v[4] ) * t ;
	  break ;
	case  1 :
	  t  = m_v[1] / ( m_v[1] - m_v[2] ) ;
	  At = 0 ;
	  Bt = m_v[0] + ( m_v[3] - m_v[0] ) * t ;
	  Ct = m_v[4] + ( m_v[7] - m_v[4] ) * t ;
	  Dt = m_v[5] + ( m_v[6] - m_v[5] ) * t ;
	  break ;
	case  2 :
	  t  = m_v[2] / ( m_v[2] - m_v[3] ) ;
	  At = 0 ;
	  Bt = m_v[1] + ( m_v[0] - m_v[1] ) * t ;
	  Ct = m_v[5] + ( m_v[4] - m_v[5] ) * t ;
	  Dt = m_v[6] + ( m_v[7] - m_v[6] ) * t ;
	  break ;
	case  3 :
	  t  = m_v[3] / ( m_v[3] - m_v[0] ) ;
	  At = 0 ;
	  Bt = m_v[2] + ( m_v[1] - m_v[2] ) * t ;
	  Ct = m_v[6] + ( m_v[5] - m_v[6] ) * t ;
	  Dt = m_v[7] + ( m_v[4] - m_v[7] ) * t ;
	  break ;
	case  4 :
	  t  = m_v[4] / ( m_v[4] - m_v[5] ) ;
	  At = 0 ;
	  Bt = m_v[7] + ( m_v[6] - m_v[7] ) * t ;
	  Ct = m_v[3] + ( m_v[2] - m_v[3] ) * t ;
	  Dt = m_v[0] + ( m_v[1] - m_v[0] ) * t ;
	  break ;
	case  5 :
	  t  = m_v[5] / ( m_v[5] - m_v[6] ) ;
	  At = 0 ;
	  Bt = m_v[4] + ( m_v[7] - m_v[4] ) * t ;
	  Ct = m_v[0] + ( m_v[3] - m_v[0] ) * t ;
	  Dt = m_v[1] + ( m_v[2] - m_v[1] ) * t ;
	  break ;
	case  6 :
	  t  = m_v[6] / ( m_v[6] - m_v[7] ) ;
	  At = 0 ;
	  Bt = m_v[5] + ( m_v[4] - m_v[5] ) * t ;
	  Ct = m_v[1] + ( m_v[0] - m_v[1] ) * t ;
	  Dt = m_v[2] + ( m_v[3] - m_v[2] ) * t ;
	  break ;
	case  7 :
	  t  = m_v[7] / ( m_v[7] - m_v[4] ) ;
	  At = 0 ;
	  Bt = m_v[6] + ( m_v[5] - m_v[6] ) * t ;
	  Ct = m_v[2] + ( m_v[1] - m_v[2] ) * t ;
	  Dt = m_v[3] + ( m_v[0] - m_v[3] ) * t ;
	  break ;
	case  8 :
	  t  = m_v[0] / ( m_v[0] - m_v[4] ) ;
	  At = 0 ;
	  Bt = m_v[3] + ( m_v[7] - m_v[3] ) * t ;
	  Ct = m_v[2] + ( m_v[6] - m_v[2] ) * t ;
	  Dt = m_v[1] + ( m_v[5] - m_v[1] ) * t ;
	  break ;
	case  9 :
	  t  = m_v[1] / ( m_v[1] - m_v[5] ) ;
	  At = 0 ;
	  Bt = m_v[0] + ( m_v[4] - m_v[0] ) * t ;
	  Ct = m_v[3] + ( m_v[7] - m_v[3] ) * t ;
	  Dt = m_v[2] + ( m_v[6] - m_v[2] ) * t ;
	  break ;
	case 10 :
	  t  = m_v[2] / ( m_v[2] - m_v[6] ) ;
	  At = 0 ;
	  Bt = m_v[1] + ( m_v[5] - m_v[1] ) * t ;
	  Ct = m_v[0] + ( m_v[4] - m_v[0] ) * t ;
	  Dt = m_v[3] + ( m_v[7] - m_v[3] ) * t ;
	  break ;
	case 11 :
	  t  = m_v[3] / ( m_v[3] - m_v[7] ) ;
	  At = 0 ;
	  Bt = m_v[2] + ( m_v[6] - m_v[2] ) * t ;
	  Ct = m_v[1] + ( m_v[5] - m_v[1] ) * t ;
	  Dt = m_v[0] + ( m_v[4] - m_v[0] ) * t ;
	  break ;
	default : CSimuMsg::ExitWithMessage(location, "Invalid edge\n") ;  break ;
	}
	break ;

	default : CSimuMsg::ExitWithMessage(location, "Invalid ambiguous case\n") ;  break ;
	}

	if( At >= 0 ) test ++ ;
	if( Bt >= 0 ) test += 2 ;
	if( Ct >= 0 ) test += 4 ;
	if( Dt >= 0 ) test += 8 ;
	switch( test )
	{
	case  0 : return s>0 ;
	case  1 : return s>0 ;
	case  2 : return s>0 ;
	case  3 : return s>0 ;
	case  4 : return s>0 ;
	case  5 : if( At * Ct <  Bt * Dt ) return s>0 ; break ;
	case  6 : return s>0 ;
	case  7 : return s<0 ;
	case  8 : return s>0 ;
	case  9 : return s>0 ;
	case 10 : if( At * Ct >= Bt * Dt ) return s>0 ; break ;
	case 11 : return s<0 ;
	case 12 : return s>0 ;
	case 13 : return s<0 ;
	case 14 : return s<0 ;
	case 15 : return s<0 ;
	}

	return s<0 ;
}

int CSimuMarchingCube::AddInteriorSrfcPoint()
{
	CSimuSrfcPoint3D* newP = m_srfcPs.CreateOneElement();

	float  u = 0 ;
	int   vid ;

	newP->m_x = newP->m_y = newP->m_z =  newP->m_nx = newP->m_ny = newP->m_nz = 0 ;

	// Calculates the average of the intersection points of the cube
	vid = GetSrfcPointIdOnX( m_x , m_y , m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
			   u++ ;  newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnY(m_x+1, m_y , m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnX( m_x ,m_y+1, m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnY( m_x , m_y , m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnX( m_x , m_y ,m_z+1) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnY(m_x+1, m_y ,m_z+1) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnX( m_x ,m_y+1,m_z+1) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnY( m_x , m_y ,m_z+1) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnZ( m_x , m_y , m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnZ(m_x+1, m_y , m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnZ(m_x+1,m_y+1, m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }
	vid = GetSrfcPointIdOnZ( m_x ,m_y+1, m_z ) ;
	if( vid != -1 ) { newP->m_x  += m_srfcPs[vid]->m_x ;  newP->m_y  += m_srfcPs[vid]->m_y ;  newP->m_z  += m_srfcPs[vid]->m_z ;
				u++ ; newP->m_nx += m_srfcPs[vid]->m_nx ; newP->m_ny += m_srfcPs[vid]->m_ny ; newP->m_nz += m_srfcPs[vid]->m_nz ; }

	newP->m_x  /= u ;
	newP->m_y  /= u ;
	newP->m_z  /= u ;

	u = (float) sqrt( newP->m_nx * newP->m_nx + newP->m_ny * newP->m_ny +newP->m_nz * newP->m_nz ) ;
	if( u > 0 )
	{
	newP->m_nx /= u ;
	newP->m_ny /= u ;
	newP->m_nz /= u ;
	}

	return m_srfcPs.m_paNum-1 ;
}