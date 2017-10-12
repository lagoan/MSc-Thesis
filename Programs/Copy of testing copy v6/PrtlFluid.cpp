// PrtlFluid.cpp: implementation of the CPrtlFluid class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SimuFlexApp.h"
#include "PrtlFluid.h"
#include "SimuManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

int CPrtlFluid::m_pfMaxFluidId = 0;
time_t	CPrtlFluid::m_tNewFormatForPrtlDataImExport0 = CSimuManager::GetSpecifiedTime(2005, 5, 7, 10); // 2005 Jun. 7 10am 
time_t	CPrtlFluid::m_tNewFormatForPrtlDataImExport1 = CSimuManager::GetSpecifiedTime(2005, 5, 25, 10); // 2005 Jun. 25 10am 
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPrtlFluid::CPrtlFluid()
{
	m_pfMaxFluidId++;
	char tmpName[LEN_FLUID_NAME];
	sprintf(tmpName, "prtl_fluid_%d", m_pfMaxFluidId);
	m_deiName = tmpName;

	m_pfFluidId = m_pfMaxFluidId;

	m_pfSimuManager = NULL;
	m_pfPrtlEvolutionIsPrepared = false;

	m_pfPrtlDist = CSimuManager::m_prtlDistance;
	m_pfUniPrtlVolume = pow(m_pfPrtlDist, 3);
	if (CSimuManager::UNIFORM_FIXED_PARTICLE_MASS)
	{
		m_pfPrtlMass = CSimuManager::m_prtlMass;
	}
	else // fluid density is specified
	{
		m_pfPrtlMass = CSimuManager::m_prtlDensity*m_pfUniPrtlVolume;
	}
	m_pfTotalMass = SIMU_POSITIVE_NUMBER;
	m_pfTotalFreePrtlMass = SIMU_POSITIVE_NUMBER;
	m_pfSmoothLen = m_pfPrtlDist*CSimuManager::m_smoothLengthRatio;
	m_pfSmoothLenSqr = m_pfSmoothLen*m_pfSmoothLen;

	ComputeUniformWeightSum();
	m_pfDensity = m_pfPrtlMass*m_pfUniWeightSum; // it should be an approximation to CSimuManager::m_prtlDensity.
	m_pfUniPrtlNumDensity = m_pfUniPrtlVolume*m_pfUniWeightSum;
	m_pfPrtlMassRatio = 1;
	m_pfInterFluidDamping = CSimuManager::m_interFluidDamping;
	m_pfInterFluidRadius = m_pfPrtlDist*CSimuManager::m_smoothLengthRatio;
	m_pfKEVariation = 0;

	m_pfAirPressure = CSimuManager::m_airPressure;
	m_pfShearModulus = CSimuManager::m_shearModulus;
	m_pfRelaxationTime = CSimuManager::m_relaxationTime;
	m_pfRotationDerivative = CSimuManager::m_eRotationDerivative;

	m_pfSubThermalSteps = CSimuManager::m_subThermalSteps;
	m_pfHeatConductivity = CSimuManager::m_heatConductivity;
	m_pfIniTemperature = CSimuManager::m_prtlIniTemperature;
	m_pfMinShearModulus = CSimuManager::m_minShearModulus;
	m_pfMaxShearModulus = CSimuManager::m_maxShearModulus;
	m_pfCoeffTemperature = 0.01;
	m_pfCoeffShearModulus = 1;
	m_pfConstSummation = 8;
	m_pfLinrTempTxtr.m_lttName = m_deiName;
	
	m_pbApplyGravity = CSimuManager::APPLY_GRAVITY;

	m_pfOnlyBdryMovingTime = CSimuManager::m_onlyBdryMovingTime;
	m_pfOnlyGravityTime = CSimuManager::m_onlyGravityTime;

	SimuValue timeStep = CSimuManager::ComputeTimeStep();
	SetTimeStep(timeStep);
	m_pfAvgVERatio = 0;

	m_pfBoundaries.InitializeClassArray(true, SIMU_PARA_BOUNDARY_NUM_ALLOC_SIZE);

	m_pfPrtls.InitializeClassArray(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	m_pfAvgNumNgbrs = 0;

	m_pfPoissonPauseSteps = CSimuManager::m_maxPoissonPauseSteps;
	m_pfSrfcDetectPauseSteps = CSimuManager::m_maxSrfcDetectPauseSteps;

	m_pfIsoDensituRadius = CSimuManager::m_isoDenRadiusRatio*m_pfPrtlDist;
	m_pfIsoDensityRatio = CSimuManager::m_isoDensityRatio;
	m_pfMCGrid.m_gCellSize = CSimuManager::m_meshSizeOverPrtlDist*m_pfPrtlDist;

	m_pfInteractTime = 0;
	m_pfMotionTime = 0;
	m_pfSurfaceTime = 0;

	m_pfColor.SetValue(CSimuManager::COLOR_INSIDE);
	m_pfSrfcColor.SetValue(CSimuManager::COLOR_SRFC);

	m_pfNeedToRegisterMovedPrtls = true;
	m_pfNeedToRefreshPrtlNeighbors = true;
	m_pfNeedToUpdatePrtlDensities = true;
	m_pfPrtlSearch.m_psGrid.m_gCellSize = m_pfSmoothLen;

	m_pfCGTolerance = CSimuManager::m_CGSolverTolerance;
	m_pfInterations = 0;
	m_pfCGSolver3D.m_scgs3dBlockSymmetric = true;
	m_pfInterations3D = 0;
	m_pfDeltaPos.InitializeClassArray(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);

	m_pfMaxVortex = 0; // debug use only
	m_pfMaxAngTen = 0; // debug use only
	m_pfMaxTensor = 0; // debug use only

	m_pfHighlitPrtl = NULL;
	m_pfHighlitPrtlAry = NULL;
}

CPrtlFluid::~CPrtlFluid()
{
}

void CPrtlFluid::RenderPrtlFluid()
{
	if (CSimuManager::DRAW_PARTICLES)
		RenderFluidPrtls();

	if (CSimuManager::DRAW_SURFACE_MESH)
	{
		glPushAttrib(GL_CURRENT_BIT);
		SimuColor3d(m_pfColor[R], m_pfColor[G], m_pfColor[B]);
		m_pfMarchingCube.DrawMarchingCubeSurfaceMeshByOpenGL(&m_pfLinrTempTxtr);
		glPopAttrib();
	}

	if (CSimuManager::DRAW_BOUNDARY)
		DrawBoundaries();

	if (CSimuManager::DRAW_MC_BOUNDING_BOX)
		m_pfMCGrid.DrawGridBoundingEdges();

	RenderHighlitPrtls();
}

void CPrtlFluid::RenderFluidPrtls()
{
	CVector3D color;
	glPushAttrib(GL_CURRENT_BIT);
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
#ifdef	SIMU_PARA_ENABLE_PARTICLE_COLOR
		if (CSimuManager::COLOR_SURFACE_PARTICLES)
			color.SetValue(&pPrtl->m_vpColor);
		else
#endif
			ComputeParticleColor(color, pPrtl);
		SimuColor3d(color[R], color[G], color[B]);
		pPrtl->RenderPrtlByOpenGL();
	}
	glPopAttrib();
}

void CPrtlFluid::ComputeParticleColor(CVector3D &color, CFluidPrtl* pPrtl)
{
	SimuValue deltaColor;
	bool varyingColor = false;
	if (CSimuManager::DRAW_PARTICLE_DENSITIES_BY_COLOR)
	{
		deltaColor = CSimuManager::m_displayTuner*pPrtl->m_fpDensity;
		if (pPrtl->m_fpDensity > m_pfDensity*m_pfIsoDensityRatio)
			varyingColor = true;
		else
			varyingColor = false;
	}
	else if (CSimuManager::DRAW_PARTICLE_SPEED_BY_COLOR)
	{
		deltaColor = CSimuManager::m_displayTuner*pPrtl->m_vpVel->Length();
		varyingColor = true;
	}
	else if (CSimuManager::DRAW_PARTICLE_MASS_BY_COLOR)
	{
		deltaColor = CSimuManager::m_displayTuner*pPrtl->m_fpMass;
		varyingColor = true;
	}
	else if (CSimuManager::HEAT_TRANSFER)
	{
		varyingColor = false;
		CTemperatureToTexture::GetDisplayColor(pPrtl->m_fpTemperature, &color, &m_pfLinrTempTxtr);
		pPrtl->SetVirtualPrtlColor(&color);
	}
	if (varyingColor)
	{
		color[R] = 1 + deltaColor;
		color[G] = 1 - deltaColor;
		color[B] = 1 - deltaColor;
	}
	else
		color.SetValue(&pPrtl->m_vpColor);
}

void CPrtlFluid::RenderHighlitPrtls()
{
	if (m_pfHighlitPrtl != NULL)
	{
		glPushAttrib(GL_CURRENT_BIT);
		bool tmpBool = m_pfHighlitPrtl->m_geHighlit;
		m_pfHighlitPrtl->m_geHighlit = true;
		m_pfHighlitPrtl->RenderPrtlByOpenGL();
		m_pfHighlitPrtl->m_geHighlit = tmpBool;
		glPopAttrib();
	}
	if (m_pfHighlitPrtlAry != NULL)
	{
		glPushAttrib(GL_CURRENT_BIT);
		for (int i=0; i < m_pfHighlitPrtlAry->m_paNum; i++)
		{
			bool tmpBool = (*m_pfHighlitPrtlAry)[i]->m_geHighlit;
			(*m_pfHighlitPrtlAry)[i]->m_geHighlit = true;
			(*m_pfHighlitPrtlAry)[i]->RenderPrtlByOpenGL();
			(*m_pfHighlitPrtlAry)[i]->m_geHighlit = tmpBool;
		}
		glPopAttrib();
	}
}

void CPrtlFluid::DrawBoundaries()
{
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		if (pBdry->m_bdryVisible)
			pBdry->DrawBoundary();
	}
}

void CPrtlFluid::LoadPrtlFluidVariablesFrom(CPrtlFluid* anotherFluid)
{
	m_pfLinrTempTxtr.CopyValuesFrom(&anotherFluid->m_pfLinrTempTxtr);
}

void CPrtlFluid::ExportPrtlFluidVariables(FILE* fp)
{
	fprintf(fp, "[start of particle fluid variables]\n");
	fprintf(fp, "m_deiName=%s\n", m_deiName);
	fprintf(fp, "m_pfRandomSeed=%u\n", m_pfRandomSeed);
	fprintf(fp, "m_pfDensity=%f\n", m_pfDensity);
	fprintf(fp, "m_pfPrtlDist=%f\n", m_pfPrtlDist);
	fprintf(fp, "m_pfSmoothLen=%f\n", m_pfSmoothLen);
	fprintf(fp, "m_pfTimeStep=%f\n", m_pfTimeStep);

	fprintf(fp, "m_pfPrtlMassRatio=%f\n", m_pfPrtlMassRatio);
	fprintf(fp, "m_pfInterFluidDamping=%f\n", m_pfInterFluidDamping);

	fprintf(fp, "m_pfAirPressure=%f\n", m_pfAirPressure);
	fprintf(fp, "m_pfShearModulus=%f\n", m_pfShearModulus);
	fprintf(fp, "m_pfRelaxationTime=%f\n", m_pfRelaxationTime);
	fprintf(fp, "m_pfRotationDerivative=%d\n", m_pfRotationDerivative);

	fprintf(fp, "m_pfSubThermalSteps=%d\n", m_pfSubThermalSteps);
	fprintf(fp, "m_pfHeatConductivity=%f\n", m_pfHeatConductivity);
	fprintf(fp, "m_pfIniTemperature=%f\n", m_pfIniTemperature);
	fprintf(fp, "m_pfMinShearModulus=%f\n", m_pfMinShearModulus);
	fprintf(fp, "m_pfMaxShearModulus=%f\n", m_pfMaxShearModulus);
	fprintf(fp, "m_pfCoeffTemperature=%f\n", m_pfCoeffTemperature);
	fprintf(fp, "m_pfCoeffShearModulus=%f\n", m_pfCoeffShearModulus);
	fprintf(fp, "m_pfConstSummation=%f\n", m_pfConstSummation);

	fprintf(fp, "m_lttRatioColorR=%f\n", m_pfLinrTempTxtr.m_lttRatioColorR);
	fprintf(fp, "m_lttRatioColorG=%f\n", m_pfLinrTempTxtr.m_lttRatioColorG);
	fprintf(fp, "m_lttRatioColorB=%f\n", m_pfLinrTempTxtr.m_lttRatioColorB);
	fprintf(fp, "m_lttConstColorR=%f\n", m_pfLinrTempTxtr.m_lttConstColorR);
	fprintf(fp, "m_lttConstColorG=%f\n", m_pfLinrTempTxtr.m_lttConstColorG);
	fprintf(fp, "m_lttConstColorB=%f\n", m_pfLinrTempTxtr.m_lttConstColorB);
	fprintf(fp, "m_lttRatioFilter=%f\n", m_pfLinrTempTxtr.m_lttRatioFilter);
	fprintf(fp, "m_lttConstFilter=%f\n", m_pfLinrTempTxtr.m_lttConstFilter);
	fprintf(fp, "m_lttMinTemperature=%f\n", m_pfLinrTempTxtr.m_lttMinTemperature);
	fprintf(fp, "m_lttMaxTemperature=%f\n", m_pfLinrTempTxtr.m_lttMaxTemperature);

	fprintf(fp, "m_pfOnlyBdryMovingTime=%f\n", m_pfOnlyBdryMovingTime);
	fprintf(fp, "m_pfOnlyGravityTime=%f\n", m_pfOnlyGravityTime);

	fprintf(fp, "m_pfIsoDensituRadius=%f\n", m_pfIsoDensituRadius);
	fprintf(fp, "m_pfIsoDensityRatio=%f\n", m_pfIsoDensityRatio);

	fprintf(fp, "m_pfColor=[%f, %f, %f]\n", m_pfColor[X], m_pfColor[Y], m_pfColor[Z]);
	fprintf(fp, "[end of particle fluid variables]\n");
}

void CPrtlFluid::ImportPrtlFluidVariables(FILE* fp)
{
	char location[] = "CSimuManager::ImportPrtlFluidVariables(fp)";

	char buf[LEN_DATA_LINE];
	char tmp[LEN_DATA_LINE];
	float tmpFloat, x, y, z;

	while(!feof(fp)) // Read the file until the end.
	{
		fgets(buf, sizeof(buf), fp);
		if (ferror(fp))
		{
			fclose(fp);
			CSimuMsg::ExitWithMessage(location, "Error in reading global parameters.");
			return;
		}
		// Any empty line or the line that starts with '#' will be skipped.
		if ((strlen(buf) <=0)||(buf[0] == '#')) continue;
		TRACE("%s\n", buf);

		if (strncmp(buf, "[end of particle fluid variables]",
						strlen("[end of particle fluid variables]")) == 0)
			return;
		else if (strncmp(buf, "m_deiName", strlen("m_deiName")) == 0)
		{
			sscanf(buf, "m_deiName=%s\n", tmp); m_deiName = tmp;
		}
		else if (strncmp(buf, "m_pfRandomSeed", strlen("m_pfRandomSeed")) == 0)
		{
			sscanf(buf, "m_pfRandomSeed=%f\n", &tmpFloat); m_pfRandomSeed = tmpFloat;
		}
		else if (strncmp(buf, "m_pfDensity", strlen("m_pfDensity")) == 0)
		{
			sscanf(buf, "m_pfDensity=%f\n", &tmpFloat); m_pfDensity = tmpFloat;
		}
		else if (strncmp(buf, "m_pfPrtlDist", strlen("m_pfPrtlDist")) == 0)
		{
			sscanf(buf, "m_pfPrtlDist=%f\n", &tmpFloat); m_pfPrtlDist = tmpFloat;
		}
		else if (strncmp(buf, "m_pfSmoothLen", strlen("m_pfSmoothLen")) == 0)
		{
			sscanf(buf, "m_pfSmoothLen=%f\n", &tmpFloat); m_pfSmoothLen = tmpFloat;
		}
		else if (strncmp(buf, "m_pfTimeStep", strlen("m_pfTimeStep")) == 0)
		{
			sscanf(buf, "m_pfTimeStep=%f\n", &tmpFloat); m_pfTimeStep = tmpFloat;
		}
		else if (strncmp(buf, "m_pfIsoDensituRadius", strlen("m_pfIsoDensituRadius")) == 0)
		{
			sscanf(buf, "m_pfIsoDensituRadius=%f\n", &tmpFloat); m_pfIsoDensituRadius = tmpFloat;
		}
		else if (strncmp(buf, "m_pfIsoDensityRatio", strlen("m_pfIsoDensityRatio")) == 0)
		{
			sscanf(buf, "m_pfIsoDensityRatio=%f\n", &tmpFloat); m_pfIsoDensityRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_pfPrtlMassRatio", strlen("m_pfPrtlMassRatio")) == 0)
		{
			sscanf(buf, "m_pfPrtlMassRatio=%f\n", &tmpFloat); m_pfPrtlMassRatio = tmpFloat;
		}
		else if (strncmp(buf, "m_pfInterFluidDamping", strlen("m_pfInterFluidDamping")) == 0)
		{
			sscanf(buf, "m_pfInterFluidDamping=%f\n", &tmpFloat); m_pfInterFluidDamping = tmpFloat;
		}
		else if (strncmp(buf, "m_pfAirPressure", strlen("m_pfAirPressure")) == 0)
		{
			sscanf(buf, "m_pfAirPressure=%f\n", &tmpFloat); m_pfAirPressure = tmpFloat;
		}
		else if (strncmp(buf, "m_pfShearModulus", strlen("m_pfShearModulus")) == 0)
		{
			sscanf(buf, "m_pfShearModulus=%f\n", &tmpFloat); m_pfShearModulus = tmpFloat;
		}
		else if (strncmp(buf, "m_pfRelaxationTime", strlen("m_pfRelaxationTime")) == 0)
		{
			sscanf(buf, "m_pfRelaxationTime=%f\n", &tmpFloat); m_pfRelaxationTime = tmpFloat;
		}
		else if (strncmp(buf, "m_pfRotationDerivative", strlen("m_pfRotationDerivative")) == 0)
		{
			sscanf(buf, "m_pfRotationDerivative=%d\n", &m_pfRotationDerivative);
		}
		else if (strncmp(buf, "m_lttRatioColorR", strlen("m_lttRatioColorR")) == 0)
		{
			sscanf(buf, "m_lttRatioColorR=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttRatioColorR = tmpFloat;
		}
		else if (strncmp(buf, "m_lttRatioColorG", strlen("m_lttRatioColorG")) == 0)
		{
			sscanf(buf, "m_lttRatioColorG=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttRatioColorG = tmpFloat;
		}
		else if (strncmp(buf, "m_lttRatioColorB", strlen("m_lttRatioColorB")) == 0)
		{
			sscanf(buf, "m_lttRatioColorB=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttRatioColorB = tmpFloat;
		}
		else if (strncmp(buf, "m_lttConstColorR", strlen("m_lttConstColorR")) == 0)
		{
			sscanf(buf, "m_lttConstColorR=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttConstColorR = tmpFloat;
		}
		else if (strncmp(buf, "m_lttConstColorG", strlen("m_lttConstColorG")) == 0)
		{
			sscanf(buf, "m_lttConstColorG=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttConstColorG = tmpFloat;
		}
		else if (strncmp(buf, "m_lttConstColorB", strlen("m_lttConstColorB")) == 0)
		{
			sscanf(buf, "m_lttConstColorB=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttConstColorB = tmpFloat;
		}
		else if (strncmp(buf, "m_lttRatioFilter", strlen("m_lttRatioFilter")) == 0)
		{
			sscanf(buf, "m_lttRatioFilter=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttRatioFilter = tmpFloat;
		}
		else if (strncmp(buf, "m_lttConstFilter", strlen("m_lttConstFilter")) == 0)
		{
			sscanf(buf, "m_lttConstFilter=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttConstFilter = tmpFloat;
		}
		else if (strncmp(buf, "m_lttMinTemperature", strlen("m_lttMinTemperature")) == 0)
		{
			sscanf(buf, "m_lttMinTemperature=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttMinTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_lttMaxTemperature", strlen("m_lttMaxTemperature")) == 0)
		{
			sscanf(buf, "m_lttMaxTemperature=%f\n", &tmpFloat); m_pfLinrTempTxtr.m_lttMaxTemperature = tmpFloat;
		}
		else if (strncmp(buf, "m_pfOnlyBdryMovingTime", strlen("m_pfOnlyBdryMovingTime")) == 0)
		{
			sscanf(buf, "m_pfOnlyBdryMovingTime=%f\n", &tmpFloat); m_pfOnlyBdryMovingTime = tmpFloat;
		}
		else if (strncmp(buf, "m_pfOnlyGravityTime", strlen("m_pfOnlyGravityTime")) == 0)
		{
			sscanf(buf, "m_pfOnlyGravityTime=%f\n", &tmpFloat); m_pfOnlyGravityTime = tmpFloat;
		}
		else if (strncmp(buf, "m_pfColor", strlen("m_pfColor")) == 0)
		{
			sscanf(buf, "m_pfColor=[%f, %f, %f]\n", &x, &y, &z);
			m_pfColor[X] = x; m_pfColor[Y] = y; m_pfColor[Z] = z;
		}
	}
}

void CPrtlFluid::ExportPrtlFluidData(CString exportDir, UINT nCurFrame)
{
	char location[] = "CPrtlFluid::ExportPrtlFluidData(exportDir, nCurFrame)";

	ExportSimulationInfo(exportDir, (LPCTSTR)m_pfSimuInfo);

	if (CSimuManager::IM_EXPORT_FLUID_PARTICLES)
		ExportPrtlData(exportDir, nCurFrame);

	if (CSimuManager::IM_EXPORT_FLUID_SURFACE)
	{
		FILE* fp;
		CString fileName = exportDir;
		fileName += "\\";
		fileName += MakeSurfaceMeshDataFileName(nCurFrame);
		if ((fp=fopen(fileName, "w")) == NULL)
		{
			sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", fileName);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		}
		else
		{
			CString txFileName = MakeSurfaceMeshTextureDataFileName(nCurFrame);
			ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(txFileName, fp);
			fclose(fp);
			if (CSimuManager::HEAT_TRANSFER)
			{
				CString txFile = exportDir;
				txFile += "\\";
				txFile += txFileName;
				FILE* txFP;
				if ((txFP=fopen(txFile, "w")) == NULL)
				{
					sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", txFile);
					CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
				}
				else
				{
					m_pfMarchingCube.ExportTextureList(txFP, &m_pfLinrTempTxtr);
					fclose(txFP);
				}
			}
		}
	}
}

void CPrtlFluid::ImportPrtlFluidData(CString importDir, UINT nCurFrame)
{
	char location[] = "CPrtlFluid::ImportPrtlFluidData(importDir, nCurFrame)";

	if (CSimuManager::IM_EXPORT_FLUID_PARTICLES)
	{
		ImportPrtlData(importDir, nCurFrame);
		if (CSimuManager::CREATE_SURFACE_MESH)
		{
			m_pfNeedToRegisterMovedPrtls = true;
			long start = clock();
			ConstructFluidSurfaceMeshByMarchingCube();
			long finish = clock();
			m_pfSurfaceTime = ((SimuValue)(finish-start))/CLOCKS_PER_SEC;
		}
	}
	
	ImportSimulationInfo(importDir, nCurFrame);
	
	if (CSimuManager::IM_EXPORT_FLUID_SURFACE)
	{
		bool bExportTextureList = false;
		FILE* fp;
		CString fileName = importDir;
		fileName += "\\";
		fileName += MakeSurfaceMeshDataFileName(nCurFrame);
		if (CSimuManager::CREATE_SURFACE_MESH
		 && CSimuManager::EXPORT_FLUID_DATA)
		{
			if (CSimuUtility::FileDoesExist(fileName)
			 && !CSimuManager::OVERWRITE_EXISTING_DATA)
				return;
			if ((fp=fopen(fileName, "w")) == NULL)
			{
				sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", fileName);
				CSimuMsg::ExitWithMessage("CPrtlFluid::ImportPrtlFluidData(...)",
											CSimuMsg::GetCheckedBuf());
			}
			else
			{
				CString txFileName = MakeSurfaceMeshTextureDataFileName(nCurFrame);
				ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(txFileName, fp);
				fclose(fp);
				if (CSimuManager::HEAT_TRANSFER)
					bExportTextureList = true;
			}
		}
		else if (!CSimuManager::CREATE_SURFACE_MESH
			  && !CSimuManager::EXPORT_FLUID_DATA)
		{
			if ((fp=fopen(fileName, "r")) == NULL)
			{
				CSimuManager::IMPORT_FLUID_DATA = false;
			}
			else
			{
				ImportSurfaceMeshFromPOVRayMesh2ObjectFile(fp);
				fclose(fp);
				if (CSimuManager::HEAT_TRANSFER
				 && CSimuManager::OVERWRITE_TEXTURE)
					bExportTextureList = true;
			}
		}
		if (bExportTextureList)
		{
			CString txFileName = MakeSurfaceMeshTextureDataFileName(nCurFrame);
			CString txFile = importDir;
			txFile += "\\";
			txFile += txFileName;
			FILE* txFP;
			if ((txFP=fopen(txFile, "w")) == NULL)
			{
				sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to write.", txFile);
				CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
			}
			else
			{
				m_pfMarchingCube.ExportTextureList(txFP, &m_pfLinrTempTxtr);
				fclose(txFP);
			}
		}
	}
}

void CPrtlFluid::ExportPrtlFluidParameters(FILE* fp)
{
	fprintf(fp, "name=%s\n", m_deiName);
	fprintf(fp, "random seed=%u\n", m_pfRandomSeed);
	fprintf(fp, "density=%f\n", m_pfDensity);
	fprintf(fp, "particle distance=%f\n", m_pfPrtlDist);
	fprintf(fp, "smoothing length=%f\n", m_pfSmoothLen);
	fprintf(fp, "compressibility constant=%f\n", CSimuManager::m_soundSpeed);
	fprintf(fp, "adaptive time step=%d\n", CSimuManager::ADAPTIVE_TIME_STEP);
	fprintf(fp, "time step=%f\n", m_pfTimeStep);

	fprintf(fp, "sphere radius=%f\n", CSimuManager::m_sphereRadius);
	fprintf(fp, "shear modulus=%f\n", CSimuManager::m_shearModulus);
	fprintf(fp, "relaxation time=%f\n", CSimuManager::m_relaxationTime);
	fprintf(fp, "position jitter=%f\n", CSimuManager::m_prtlPosJitter);

	fprintf(fp, "iso-surface value=%f\n", m_pfIsoDensityRatio);
	fprintf(fp, "iso-surface radius=%f\n", m_pfIsoDensituRadius);

	fprintf(fp, "color=[%f, %f, %f]\n", m_pfColor[X], m_pfColor[Y], m_pfColor[Z]);

	fprintf(fp, "boundary number=%d\n", m_pfBoundaries.m_paNum);
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		fprintf(fp, "boundary type=%d\n", pBdry->m_bdryType);
		pBdry->ExportBoundaryParameters(fp);
	}
}

void CPrtlFluid::ImportPrtlFluidParameters(FILE* fp)
{
	char location[] = "CPrtlFluid::ImportPrtlFluidParameters(fp)";

	char fbName[LEN_FLUID_NAME];
	float tmpValue;
	float tmpX, tmpY, tmpZ;
	int tmpInt;

	fscanf(fp, "name=%s\n", &fbName);
	m_deiName = fbName;
	fscanf(fp, "random seed=%u\n", &tmpValue); m_pfRandomSeed = tmpValue;
	fscanf(fp, "density=%f\n", &tmpValue); m_pfDensity = tmpValue;
	fscanf(fp, "particle distance=%f\n", &tmpValue); m_pfPrtlDist = tmpValue;
	fscanf(fp, "smoothing length=%f\n", &tmpValue); m_pfSmoothLen = tmpValue;
	fscanf(fp, "compressibility constant=%f\n", &tmpValue); CSimuManager::m_soundSpeed = tmpValue;
	fscanf(fp, "adaptive time step=%d\n", &tmpInt); CSimuManager::ADAPTIVE_TIME_STEP = tmpInt;
	fscanf(fp, "time step=%f\n", &tmpValue); m_pfTimeStep = tmpValue;

	fscanf(fp, "sphere radius=%f\n", &tmpValue); //CSimuManager::m_sphereRadius = tmpValue;
	fscanf(fp, "shear modulus=%f\n", &tmpValue); CSimuManager::m_shearModulus = tmpValue;
	fscanf(fp, "relaxation time=%f\n", &tmpValue); CSimuManager::m_relaxationTime = tmpValue;
	fscanf(fp, "position jitter=%f\n", &tmpValue); CSimuManager::m_prtlPosJitter = tmpValue;

	fscanf(fp, "iso-surface value=%f\n", &tmpValue); m_pfIsoDensityRatio = tmpValue;
	fscanf(fp, "iso-surface radius=%f\n", &tmpValue); m_pfIsoDensituRadius = tmpValue;

	fscanf(fp, "color=[%f, %f, %f]\n", &tmpX, &tmpY, &tmpZ);
	m_pfColor[X] = tmpX; m_pfColor[Y] = tmpY; m_pfColor[Z] = tmpZ;

	int bdryType;
	fscanf(fp, "boundary number=%d\n", &tmpInt);
	for (int i=0; i < tmpInt; i++)
	{
		fscanf(fp, "boundary type=%d\n", &bdryType);
		CBoundary* pBdry = NULL;
		switch (bdryType)
		{
			case CBoundary::BOX:
				pBdry = new CSimuBoundaryBox();
				break;
			case CBoundary::CYLINDER:
				pBdry = new CSimuBoundaryCylinder();
				break;
			case CBoundary::SOLID_TORUS:
				pBdry = new CBdrySolidTorus();
				break;
			case CBoundary::NOZZLE:
				pBdry = new CSimuFluidNozzle();
				break;
			case CBoundary::SQUARE_SLOPE:
				pBdry = new CBdrySquareSlope();
				break;
			case CBoundary::SOLID_ROD:
				pBdry = new CBdrySolidRod();
				break;
			case CBoundary::SOLID_BOX:
				pBdry = new CBdrySolidBox();
				break;
			case CBoundary::TABLE:
				pBdry = new CSimuBoundaryTable();
				break;
			case CBoundary::RIGID_BALL:
				pBdry = new CRigidBall();
				break;
			case CBoundary::HOLLOW_CONE:
				pBdry = new CBdryHollowCone();
				break;
			default:
				CSimuMsg::ExitWithMessage(location, "Unknown boundary type.");
				break;
		}
		if (pBdry != NULL)
		{
			pBdry->ImportBoundaryParameters(fp);
			m_pfBoundaries.AppendOnePointer(pBdry);
		}
	}
}

void CPrtlFluid::ExportPrtlFluidData(CString exportDir, UINT nCurFrame, FILE* POVRayFp)
{
	ExportPOVRayEnvData(nCurFrame, POVRayFp);

	ExportPrtlFluidData(exportDir, nCurFrame);
}

void CPrtlFluid::ImportPrtlFluidData(CString importDir, UINT nCurFrame, FILE* POVRayFp)
{
	ImportPOVRayEnvData(POVRayFp);

	ImportPrtlFluidData(importDir, nCurFrame);
}

void CPrtlFluid::ExportSimulationInfo(CString exportDir, const char* info)
{
	FILE *infoFP;

	CString fileName = MakeCompleteSimuInfoFileName(exportDir);
	if ((infoFP=fopen(fileName, "a")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open file %s to export simulation information.",
								CSimuManager::m_strSimuInfoFile);
		CSimuMsg::ExitWithMessage("CSimuMsg::ExportSimulationInfo()", CSimuMsg::GetCheckedBuf());
	}
	else
	{
		CTime curTime	= CTime::GetCurrentTime();
		CString timeStr	= curTime.Format("[%Y/%m/%d %H:%M:%S]");
		fprintf(infoFP, "%s", timeStr.GetBuffer(timeStr.GetLength()));
		fprintf(infoFP, "%s\n", info);
		fclose(infoFP);
	}
}

void CPrtlFluid::ImportSimulationInfo(CString importDir, UINT nCurFrame)
{
	FILE *infoFP;

	CString fileName = MakeCompleteSimuInfoFileName(importDir);
	if ((infoFP=fopen(fileName, "r")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(),
				"Can't open file %s to import simulation information.", fileName);
		CSimuMsg::ExitWithMessage("CPrtlFluid::ImportSimulationInfo()", CSimuMsg::GetCheckedBuf());
	}
	else
	{
		UINT numFrames = 0;
		while (numFrames++ < nCurFrame)
			fgets(CSimuMsg::GetEmptyBuf(), LEN_DATA_LINE, infoFP);
		m_pfSimuInfo = CSimuMsg::GetCheckedBuf();
		fclose(infoFP);
		// display simulation info
		if (CSimuManager::CREATE_SURFACE_MESH)
			sprintf(CSimuMsg::GetEmptyBuf(), "[%d] stm=%.2f %s", nCurFrame, m_pfSurfaceTime, m_pfSimuInfo);
		else
			sprintf(CSimuMsg::GetEmptyBuf(), "[%d] %s", nCurFrame, m_pfSimuInfo);
		if (CSimuManager::m_bShowMsg)
			CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf(), false);
		TRACE(CSimuMsg::GetCheckedBuf());
	}
}

void CPrtlFluid::ExportPOVRayEnvData(UINT nCurFrame, FILE* POVRayFp)
{
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		fprintf(POVRayFp, "\n");
		m_pfBoundaries[i]->ExportBoundaryData(POVRayFp);
	}
	fprintf(POVRayFp, "\n");
	fprintf(POVRayFp, "#include \"Fluid_Animation_Env.pov\"\n\n");
	fprintf(POVRayFp, "#if (display_particles)\n");
	CString fileNamePrtlSpheres = MakePrtlDataFileName(nCurFrame);
	fprintf(POVRayFp, "\t#include \"%s\"\n", fileNamePrtlSpheres);
	fprintf(POVRayFp, "#else\n");
	CString fileNameSurfaceMesh = MakeSurfaceMeshDataFileName(nCurFrame);
	fprintf(POVRayFp, "\t#include \"%s\"\n", fileNameSurfaceMesh);
	fprintf(POVRayFp, "\tobject { %s translate Translate_%s material { My_Fluid_Material } }\n", m_deiName, m_deiName);
	fprintf(POVRayFp, "#end\n");
}

void CPrtlFluid::ImportPOVRayEnvData(FILE* POVRayFp)
{
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		fscanf(POVRayFp, "\n");
		m_pfBoundaries[i]->ImportBoundaryData(POVRayFp);
	}
	char tmpStr[LEN_DATA_LINE];
	fscanf(POVRayFp, "\n");
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
	fscanf(POVRayFp, "%s", tmpStr);
}

void CPrtlFluid::ExportPrtlData(CString exportDir, UINT nCurFrame)
{
	char location[] = "CPrtlFluid::ExportPrtlData(...)";

	FILE	*fp;

	CString fileName = MakeCompletePrtlDataFileName(exportDir, nCurFrame);

	// Open the particle positions file
	if ((fp=fopen(fileName, "w")) == NULL)
	{
		sprintf(CSimuMsg::GetEmptyBuf(), "Can't open data file %s to write.", fileName);
		CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
		return;
	}
	sprintf(CSimuMsg::GetEmptyBuf(), "Output data of fluid(%s) into file: %s.",
										m_deiName, fileName);
	CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());

	time_t curTime;
	time(&curTime);
	if (CSimuManager::FirstTimeIsLaterThanSecondTime(curTime, m_tNewFormatForPrtlDataImExport1))
	{
		ExportTwoTypePrtlsAsSpheresInPOVRay(fp);
	}
	else if (CSimuManager::FirstTimeIsLaterThanSecondTime(curTime, m_tNewFormatForPrtlDataImExport0))
	{
		ExportPrtlsAsSpheresInPOVRay(fp);
	}
	else
	{
		ExportPrtlData(fp);
	}
	// Close the data file
	fclose(fp);
}

void CPrtlFluid::ImportPrtlData(CString importDir, UINT nCurFrame)
{
	char location[] = "CSimuManager::ImportPrtlData(importDir, nCurFrame)";

	FILE	*fp;

	CString fileName = MakeCompletePrtlDataFileName(importDir, nCurFrame);

	// Open the particle positions file
	if ((fp=fopen(fileName, "r")) == NULL)
	{
		if (nCurFrame == 1)
		{
			sprintf(CSimuMsg::GetEmptyBuf(), "Can't open data file %s to read.", fileName);
			CSimuMsg::ExitWithMessage(location, CSimuMsg::GetCheckedBuf());
			return;
		}
		else
		{
			CSimuManager::IMPORT_FLUID_DATA = false; // no data file anymore
			return;
		}
	}
	sprintf(CSimuMsg::GetEmptyBuf(), "Read particle positions from file: %s.", fileName);
	CSimuMsg::DisplayMessage(CSimuMsg::GetCheckedBuf());

	time_t dirTime = CSimuManager::GetDirectoryCreationTime(importDir);
	if (CSimuManager::FirstTimeIsLaterThanSecondTime(dirTime, m_tNewFormatForPrtlDataImExport1))
	{
		ImportTwoTypePrtlsAsSpheresInPOVRay(fp);
	}
	else if (CSimuManager::FirstTimeIsLaterThanSecondTime(dirTime, m_tNewFormatForPrtlDataImExport0))
	{
		ImportPrtlsAsSpheresInPOVRay(fp);
	}
	else
	{
		ImportPrtlData(fp);
	}
	// Close the data file
	fclose(fp);
}

void CPrtlFluid::ExportPrtlData(FILE* fp)
{
	// export particle numbers
	fprintf(fp, "%d\n", m_pfPrtls.m_paNum);

	// export particle data
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		SimuValue *pos = pPrtl->m_vpPos->v;
		fprintf(fp, "%f, %f, %f, %f\n", pos[X], pos[Y], pos[Z], pPrtl->m_fpMass);
	}
}

void CPrtlFluid::ImportPrtlData(FILE* fp)
{
	// read in particle numbers
	int i, newNumPrtls;
	fscanf(fp, "%d\n", &newNumPrtls);

	// adjust particle numbers if necessary
	int addedNumPrtls = newNumPrtls - m_pfPrtls.m_paNum;
	while (m_pfPrtls.m_paNum < newNumPrtls)
	{
		CFluidPrtl* pPrtl = CreateOneFluidPrtl();
		m_pfPrtls.AppendOnePointer(pPrtl); // appending increases m_pfPrtls.m_paNum
	}
	while (m_pfPrtls.m_paNum > newNumPrtls)
	{
		delete m_pfPrtls[m_pfPrtls.m_paNum-1];
		m_pfPrtls.m_paNum --;
	}

	// read in particle data
	float vecX, vecY, vecZ, mass;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		fscanf(fp, "%f, %f, %f, %f\n", &vecX, &vecY, &vecZ, &mass);
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_vpPos->v[X] = vecX;
		pPrtl->m_vpPos->v[Y] = vecY;
		pPrtl->m_vpPos->v[Z] = vecZ;
		pPrtl->m_fpMass = mass;
		pPrtl->SetVirtualPrtlColor(&m_pfColor);
	}
}

void CPrtlFluid::ExportPrtlsAsSpheresInPOVRay(FILE* fp)
{
	// export particle numbers
	fprintf(fp, "// particle number = %d\n", m_pfPrtls.m_paNum);

	// export particle data
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		SimuValue *pos = pPrtl->m_vpPos->v;
		fprintf(fp, "sphere { <%f, %f, %f>, s_r material {Particle_Material} } // mass = %f\n",
					pos[X], pos[Y], pos[Z], pPrtl->m_fpMass);
	}
}

void CPrtlFluid::ExportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp)
{
	// export particle numbers
	fprintf(fp, "// particle number = %d\n", m_pfPrtls.m_paNum);

	// export particle data
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		SimuValue *pos = pPrtl->m_vpPos->v;
		fprintf(fp, "sphere { <%f, %f, %f>, s_r material {",
							pos[X], pos[Y], pos[Z]);
		if (pPrtl->m_fpNewPrtl)
			fprintf(fp, "New_Particle_Material");
		else
			fprintf(fp, "Old_Particle_Material");
		fprintf(fp, "} } // mass = %f, fixed = %d, temper = %f\n",
						pPrtl->m_fpMass, pPrtl->m_fpFixedMotion, pPrtl->m_fpTemperature);
	}
}

void CPrtlFluid::ImportPrtlsAsSpheresInPOVRay(FILE* fp)
{
	// read in particle numbers
	int i, newNumPrtls;
	fscanf(fp, "// particle number = %d\n", &newNumPrtls);

	// adjust particle numbers if necessary
	int addedNumPrtls = newNumPrtls - m_pfPrtls.m_paNum;
	while (m_pfPrtls.m_paNum < newNumPrtls)
	{
		CFluidPrtl* pPrtl = CreateOneFluidPrtl();
		m_pfPrtls.AppendOnePointer(pPrtl); // appending increases m_pfPrtls.m_paNum
	}
	while (m_pfPrtls.m_paNum > newNumPrtls)
	{
		delete m_pfPrtls[m_pfPrtls.m_paNum-1];
		m_pfPrtls.m_paNum --;
	}

	// read in particle data
	float vecX, vecY, vecZ, mass;
	char tmpStr[LEN_DATA_LINE];
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		fgets(tmpStr, LEN_DATA_LINE, fp);
		if (strlen(tmpStr) >= LEN_DATA_LINE)
			CSimuMsg::ExitWithMessage("CPrtlFluid::ImportPrtlsAsSpheresInPOVRay(fp)",
									"Data line is too long.");
		sscanf(tmpStr, "sphere { <%f, %f, %f>, s_r material {Particle_Material} } // mass = %f\n",
									&vecX, &vecY, &vecZ, &mass);
		pPrtl->m_vpPos->v[X] = vecX;
		pPrtl->m_vpPos->v[Y] = vecY;
		pPrtl->m_vpPos->v[Z] = vecZ;
		pPrtl->m_fpMass = mass;
		pPrtl->SetVirtualPrtlColor(&m_pfColor);
	}
}

void CPrtlFluid::ImportTwoTypePrtlsAsSpheresInPOVRay(FILE* fp)
{
	char location[] = "CPrtlFluid::ImportTwoTypePrtlsAsSpheresInPOVRay(fp)";

	// read in particle numbers
	int i, newNumPrtls;
	fscanf(fp, "// particle number = %d\n", &newNumPrtls);

	// adjust particle numbers if necessary
	int addedNumPrtls = newNumPrtls - m_pfPrtls.m_paNum;
	while (m_pfPrtls.m_paNum < newNumPrtls)
	{
		CFluidPrtl* pPrtl = CreateOneFluidPrtl();
		m_pfPrtls.AppendOnePointer(pPrtl); // appending increases m_pfPrtls.m_paNum
	}
	while (m_pfPrtls.m_paNum > newNumPrtls)
	{
		delete m_pfPrtls[m_pfPrtls.m_paNum-1];
		m_pfPrtls.m_paNum --;
	}

	// read in particle data
	int tmpInt;
	float vecX, vecY, vecZ, mass, temperature;
	char tmpStr[LEN_DATA_LINE];
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		fgets(tmpStr, LEN_DATA_LINE, fp);
		if (strlen(tmpStr) >= LEN_DATA_LINE)
			CSimuMsg::ExitWithMessage(location, "Data line is too long.");
		// read prtl position
		sscanf(tmpStr, "sphere { <%f, %f, %f>", &vecX, &vecY, &vecZ);
		if (strstr(tmpStr, "New_Particle_Material") != NULL)
	 		pPrtl->m_fpNewPrtl = true;
		else
			pPrtl->m_fpNewPrtl = false;
		// read prtl extra data
		mass = 0; tmpInt = 0;
		char* extraDataStr = strstr(tmpStr, "// mass");
		if (extraDataStr != NULL)
			sscanf(extraDataStr, "// mass = %f, fixed = %d", &mass, &tmpInt);
		else
			CSimuMsg::ExitWithMessage(location, "Format error: extra data string is not found.");
		// read prtl temperature
		temperature = 0;
		char* temperatureStr = strstr(tmpStr, "temper");
		if (temperatureStr != NULL)
			sscanf(temperatureStr, "temper = %f", &temperature);

		pPrtl->m_fpFixedMotion = tmpInt;
		pPrtl->m_vpPos->v[X] = vecX;
		pPrtl->m_vpPos->v[Y] = vecY;
		pPrtl->m_vpPos->v[Z] = vecZ;
		pPrtl->m_fpMass = mass;
		pPrtl->m_fpTemperature = temperature;
		if (pPrtl->m_fpFixedMotion)
			pPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_FIXED);
		else if (pPrtl->m_fpNewPrtl && m_pfSimuManager == NULL)
			pPrtl->SetVirtualPrtlColor(CSimuManager::COLOR_ORANGE);
		else
			pPrtl->SetVirtualPrtlColor(&m_pfColor);
	}
}

void CPrtlFluid::ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(CString &txFileName, FILE* fp)
{
	if (!CSimuManager::CREATE_SURFACE_MESH)
		return;

	char location[] = "CPrtlFluid::ExportSurfaceMeshIntoPOVRayMesh2ObjectFile(...)";

	// Export mesh2 header
	fprintf(fp, "// iso-value=%f, cell-size=%f, iso-radius=%f\n",
				m_pfIsoDensityRatio, m_pfMCGrid.m_gCellSize, m_pfIsoDensituRadius);
	fprintf(fp, "#declare %s = mesh2 {\n", m_deiName);

	m_pfMarchingCube.ExportMarchingCubeSurfaceMeshIntoPOVRayMesh2ObjectFile(txFileName, fp);

	// Export mesh2 tailer
	fprintf(fp, "}\n");
}

void CPrtlFluid::ImportSurfaceMeshFromPOVRayMesh2ObjectFile(FILE* fp)
{
	// Import mesh2 header
	char tmpStr[LEN_DATA_LINE];
	strcpy(tmpStr, "");
	fgets(tmpStr, LEN_DATA_LINE, fp);
	fscanf(fp, "#declare %s = mesh2 {\n", tmpStr);
	m_deiName = tmpStr;

	m_pfMarchingCube.ImportMarchingCubeSurfaceMeshFromPOVRayMesh2ObjectFile(fp);

	// Import mesh2 tailer
	fscanf(fp, "}\n");
}

//#define		SIMU_PARA_NOT_COUNT_FIXED_PRTLS_IN_OLD_PRTL_DATA_FORMAT
void CPrtlFluid::ConstructFluidSurfaceMeshByMarchingCube()
{
	char location[] = "CPrtlFluid::ConstructFluidSurfaceMeshByMarchingCube()";

#ifdef	SIMU_PARA_NOT_COUNT_FIXED_PRTLS_IN_OLD_PRTL_DATA_FORMAT
	CSimuManager::NOT_COUNT_FIXED_PRTLS = true;
	FixPrtlMotionWithBoundaries();
#endif
	RegisterPrtlsForNeighborSearchIfNotYet();
	SimuValue expandMinBdry[3], expandMaxBdry[3];
	for (int d=X; d <= Z; d++)
	{ // expand the bounding box with the thickness of iso-density radius
		expandMinBdry[d] = m_pfPrtlSearch.m_psMinPrtlPos[d] - m_pfIsoDensituRadius;
		expandMaxBdry[d] = m_pfPrtlSearch.m_psMaxPrtlPos[d] + m_pfIsoDensituRadius;
	}
	m_pfMCGrid.m_gCellSize = CSimuManager::m_meshSizeOverPrtlDist*m_pfPrtlDist;
	m_pfMCGrid.UpdateGridBoundaryAndDimension(expandMinBdry, expandMaxBdry);
	m_pfMCGrid.UpdateGridNumbers();

	m_pfIsoDensityRatio = CSimuManager::m_isoDensityRatio;
	m_pfIsoDensituRadius = CSimuManager::m_isoDenRadiusRatio*m_pfPrtlDist;
	m_pfMarchingCube.CreateGridDataStructure(m_pfMCGrid.m_gNumPoints);
	CVector3D tmpPos;
	int numPoints = 0;
	for (int k=0; k < m_pfMCGrid.m_gPointDim[Z]; k++)
	{	tmpPos[Z] = m_pfMCGrid.m_gMinPos[Z] + m_pfMCGrid.m_gCellSize * k;
	for (int j=0; j < m_pfMCGrid.m_gPointDim[Y]; j++)
	{	tmpPos[Y] = m_pfMCGrid.m_gMinPos[Y] + m_pfMCGrid.m_gCellSize * j;
	for (int i=0; i < m_pfMCGrid.m_gPointDim[X]; i++)
	{	tmpPos[X] = m_pfMCGrid.m_gMinPos[X] + m_pfMCGrid.m_gCellSize * i;

		m_pfMarchingCube.m_data[numPoints] = ComputeIsoDensityAtPos(tmpPos, m_pfIsoDensituRadius);
		CSimuPoint3D* gridPoint = new CSimuPoint3D();
		gridPoint->m_x = tmpPos[X];
		gridPoint->m_y = tmpPos[Y];
		gridPoint->m_z = tmpPos[Z];
		m_pfMarchingCube.m_gridPs[numPoints++] = gridPoint;
	}}}
	m_pfMarchingCube.ExecuteMarchingCube(m_pfMCGrid.m_gPointDim);

	if (CSimuManager::HEAT_TRANSFER)
		InterpolateSurfaceMeshPointTemperatures();
}

void CPrtlFluid::InterpolateSurfaceMeshPointTemperatures()
{
	if (m_pfMarchingCube.m_srfcPointTemperatures != NULL)
		delete m_pfMarchingCube.m_srfcPointTemperatures;

	m_pfMarchingCube.m_srfcPointTemperatures = new SimuValue[m_pfMarchingCube.m_srfcPs.m_paNum];
	for (int i=0; i < m_pfMarchingCube.m_srfcPs.m_paNum; i++)
	{
		m_pfMarchingCube.m_srfcPointTemperatures[i]
			= InterpolateTemperatureAtPoint(m_pfMarchingCube.m_srfcPs[i], m_pfSmoothLen);
	}
}

SimuValue CPrtlFluid::InterpolateTemperatureAtPoint(CSimuPoint3D* point, SimuValue radius)
{
	CVector3D pos;
	pos[X] = point->m_x; pos[Y] = point->m_y; pos[Z] = point->m_z;
	return InterpolateTemperatureAtPoint(&pos, radius);
}

SimuValue CPrtlFluid::InterpolateTemperatureAtPoint(CVector3D* pos, SimuValue radius)
{
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(pos, radius, ngbrPrtls);
	SimuValue radiusSquare = radius*radius;
	SimuValue temperature = 0;
	SimuValue dist, weight, totalWeight = 0;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		dist = pos->GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		temperature += weight*ngbrPrtl->m_fpTemperature;
		totalWeight += weight;
	}
	if (fabs(totalWeight) > SIMU_VALUE_EPSILON)
		temperature /= totalWeight;
	return temperature;
}

void CPrtlFluid::SetTimeStep(SimuValue timeStep)
{
	m_pfTimeStep = timeStep;
	m_pfMaxTimeStep = timeStep;
}

CFluidPrtl* CPrtlFluid::CreateOneFluidPrtl()
{
	CFluidPrtl* newPrtl;

	if (CSimuManager::NON_NEWTONIAN_FLUID)
		newPrtl = new CFluidPrtlNonNew();
	else if (CSimuManager::m_eCompressibility == CSimuManager::POISSON)
		newPrtl = new CFluidPrtlPoisson();
	else
		newPrtl = new CFluidPrtl();

	return newPrtl;
}

void CPrtlFluid::AddBoundary(CBoundary* boundary)
{
	m_pfBoundaries.AppendOnePointer(boundary);
}

void CPrtlFluid::RegisterPrtlsForNeighborSearchIfNotYet()
{
	if (m_pfNeedToRegisterMovedPrtls)
	{
		m_pfPrtlSearch.RegisterPrtls(m_pfPrtls.m_paNum, m_pfPrtls.m_paPtrs);
		m_pfNeedToRegisterMovedPrtls = false;
		m_pfNeedToRefreshPrtlNeighbors = true;
	}
}

void CPrtlFluid::SearchPrtlNeighbors()
{
	if (!m_pfNeedToRefreshPrtlNeighbors) return;
	m_pfNeedToRefreshPrtlNeighbors = false;
	m_pfNeedToUpdatePrtlDensities = true;

	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpId = i; // for symmetric neighbor search
		pPrtl->m_fpNgbrs.ResetArraySize(0);
	}
	SimuValue distSquare;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		ngbrPrtls.ResetArraySize(0);
		m_pfPrtlSearch.GetPossibleNgbrPrtls(pPrtl->m_vpPos, m_pfSmoothLen, ngbrPrtls);
		for (int j=0; j < ngbrPrtls.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = ngbrPrtls[j];
			if (pPrtl == ngbrPrtl) continue;
			if (pPrtl->m_fpId > ngbrPrtl->m_fpId) continue;
			distSquare = pPrtl->m_vpPos->GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
			if (distSquare > m_pfSmoothLenSqr) continue;
			pPrtl->m_fpNgbrs.AppendOnePointer(ngbrPrtl);
			ngbrPrtl->m_fpNgbrs.AppendOnePointer(pPrtl);
		}
	}
}

void CPrtlFluid::ConfirmSymmetricPrtlNeighbors()
{
	char location[] = "CPrtlFluid::ConfirmSymmetricPrtlNeighbors()";

	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = pPrtl->m_fpNgbrs[j];
			if (pPrtl == ngbrPrtl)
				CSimuMsg::ExitWithMessage(location, "Self neighboring is found.");
			if (ngbrPrtl->m_fpNgbrs.SearchForOnePointer(pPrtl) == -1)
				CSimuMsg::ExitWithMessage(location, "Neighboring is not symmetric.");
		}
	}
}

void CPrtlFluid::JitterPrtlPositions(SimuValue maxJitterDist)
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		CSimuUtility::JitterVectorValue(*pPrtl->m_vpPos, maxJitterDist);
	}
}

void CPrtlFluid::ComputeUniformWeightSum()
{
	m_pfUniWeightSum = 0;
	int layers = (int)ceil(m_pfSmoothLen/m_pfPrtlDist);
	SimuValue dist, pos[3];
	for (int i = -layers; i <= layers; i++)
	for (int j = -layers; j <= layers; j++)
	for (int k = -layers; k <= layers; k++)
	{
		pos[X] = i*m_pfPrtlDist;
		pos[Y] = j*m_pfPrtlDist;
		pos[Z] = k*m_pfPrtlDist;
		dist = sqrt(pow(pos[X], 2) + pow(pos[Y], 2) + pow(pos[Z], 2));
		if (dist > m_pfSmoothLen)
			continue;
		m_pfUniWeightSum += CSimuUtility::SplineWeightFunction(dist, m_pfSmoothLen);
	}
	if (m_pfUniWeightSum <= SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage("CPrtlFluid::ComputeUniformWeightSum()",
								"m_pfUniWeightSum = 0.");
}

void CPrtlFluid::InitializePrtlDensities(SimuValue prtlDensity)
{
	int i;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpDensity = prtlDensity;
	}
}

void CPrtlFluid::InitializePrtlVelocities(CVector3D &velocity)
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_vpVel->SetValue(&velocity);
	}
}

void CPrtlFluid::InitializeSurfacePrtlPressures()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface)
			pPrtl->m_fpPressure = m_pfAirPressure;
	}
}

void CPrtlFluid::ApplyFluidColorOntoPrtls()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->SetVirtualPrtlColor(&m_pfColor);
	}
}

void CPrtlFluid::AssignInitialPrtlMass()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpMass = m_pfPrtlMass;
	}
}

void CPrtlFluid::ComputeVariablePrtlMasses()
{
	m_pfTotalMass = 0;
	m_pfTotalFreePrtlMass = 0;
	SimuValue massOverUniWeightSum = m_pfPrtlMass/m_pfUniWeightSum;
	SimuValue weightSum;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		weightSum = pPrtl->ComputeDistWeightSum(m_pfSmoothLen);
		pPrtl->m_fpMass = massOverUniWeightSum*weightSum;
		m_pfTotalMass += pPrtl->m_fpMass;
		if (!pPrtl->m_fpFixedMotion)
			m_pfTotalFreePrtlMass += pPrtl->m_fpMass;
	}
}

SimuValue CPrtlFluid::ComputePrtlDensities()
{
	if (!m_pfNeedToUpdatePrtlDensities) return 0;
	m_pfNeedToUpdatePrtlDensities = false;

	m_pfTotalMass = 0;
	m_pfAvgNumNgbrs = 0;
	SimuValue avgDensity = 0;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->ComputePrtlDensity(m_pfSmoothLen);
		avgDensity += pPrtl->m_fpDensity;

		m_pfAvgNumNgbrs += pPrtl->m_fpNgbrs.m_paNum;
		m_pfTotalMass += pPrtl->m_fpMass;
	}
	if (m_pfPrtls.m_paNum > 0)
	{
		avgDensity /= m_pfPrtls.m_paNum;
		m_pfAvgNumNgbrs /= m_pfPrtls.m_paNum;
	}
	return avgDensity;
}

void CPrtlFluid::AssignOrgPrtlDensities()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpOrgDensity = pPrtl->m_fpDensity;
	}
}

void CPrtlFluid::AssignIniTemperatureForPrtls()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpTemperature = m_pfIniTemperature;
	}
}

void CPrtlFluid::FixPrtlMotionWithBoundaries()
{
	char location[] = "CPrtlFluid::FixPrtlMotionWithBoundaries()";

	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpFixedMotion = false;
	}
	for (int i=0; m_pfSimuManager != NULL && i < m_pfSimuManager->m_aryBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfSimuManager->m_aryBoundaries[i];
		if (pBdry->m_bdryType == CBoundary::SOLID_ROD
		 || pBdry->m_bdryType == CBoundary::SOLID_BOX)
		{
			for (int j=0; j < m_pfPrtls.m_paNum; j++)
			{
				CFluidPrtl* pPrtl = m_pfPrtls[j];
				if (pBdry->PrtlBreakBoundary(pPrtl))
				{
					if (pPrtl->m_fpFixedMotion
					 && !pPrtl->m_vpVel->IsSameVectorAs(&pBdry->m_bdryVel))
						continue;
//						CSimuMsg::ExitWithMessage(location, "Prtl can not be fixed by 2 boundaries moving differently.");
					pPrtl->SetAsFixedMotion(m_pfAirPressure);
					pPrtl->m_vpVel->SetValue(&pBdry->m_bdryVel);
					pBdry->AddEnclosedPrtl(pPrtl);
				}
			}
		}
	}
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		if (pBdry->m_bdryType == CBoundary::SOLID_ROD
		 || pBdry->m_bdryType == CBoundary::SOLID_BOX)
		{
			for (int j=0; j < m_pfPrtls.m_paNum; j++)
			{
				CFluidPrtl* pPrtl = m_pfPrtls[j];
				if (pBdry->PrtlBreakBoundary(pPrtl))
				{
					if (pPrtl->m_fpFixedMotion
					 && !pPrtl->m_vpVel->IsSameVectorAs(&pBdry->m_bdryVel))
						continue;
//						CSimuMsg::ExitWithMessage(location, "Prtl can not be fixed by 2 boundaries moving differently.");
					pPrtl->SetAsFixedMotion(m_pfAirPressure);
					pPrtl->m_vpVel->SetValue(&pBdry->m_bdryVel);
					pBdry->AddEnclosedPrtl(pPrtl);
				}
			}
		}
	}
}

void CPrtlFluid::PreparePrtlEvolution()
{
	m_pfNeedToRegisterMovedPrtls = true;
	m_pfNeedToRefreshPrtlNeighbors = true;
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();

	AssignInitialPrtlMass();

	m_pfNeedToUpdatePrtlDensities = true;
	m_pfDensity = ComputePrtlDensities();
	AssignOrgPrtlDensities();
	ComputeSrfcIsoDensity();

	AssignIniTemperatureForPrtls();
	if (CSimuManager::HEAT_TRANSFER)
		m_pfShearModulus = GetShearModulusFromLinearMelting0(m_pfIniTemperature,
															m_pfCoeffTemperature,
															m_pfCoeffShearModulus,
															m_pfConstSummation);
	m_pfPrtlEvolutionIsPrepared = true;
}

void CPrtlFluid::DetectSrfcPrtls()
{
	SimuValue weightSumThreshold = CSimuManager::m_srfcWSThreshold*m_pfUniWeightSum;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion)
		{
			pPrtl->m_fpPressure = m_pfAirPressure;
			continue;
		}
		if (pPrtl->m_fpNgbrs.m_paNum < CSimuManager::m_srfcNgbrNumThreshold
		 || pPrtl->m_fpDensity < m_pfDensity*CSimuManager::m_srfcDenThreshold
		 || pPrtl->ComputeDistWeightSum(m_pfSmoothLen) < weightSumThreshold)
		{
			SetSrfcPrtl(pPrtl);
		}
		else
		{
			if (pPrtl->m_fpNgbrs.m_paNum <= 0)
				CSimuMsg::ExitWithMessage("CPrtlFluid::DetectSrfcPrtls()",
											"Ngbr prtls are not found for insd prtl.");
			SetInsdPrtl(pPrtl);
		}
	}
}

void CPrtlFluid::SetSrfcPrtl(CFluidPrtl* pPrtl)
{
	pPrtl->m_fpOnSurface = true;
	pPrtl->m_fpPressure = m_pfAirPressure;
	pPrtl->SetVirtualPrtlColor(&m_pfSrfcColor);
}

void CPrtlFluid::SetInsdPrtl(CFluidPrtl* pPrtl)
{
	pPrtl->m_fpOnSurface = false;
	pPrtl->SetVirtualPrtlColor(&m_pfColor);
}

void CPrtlFluid::EvolveFluidParticles(SimuValue simuTime)
{
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();
	ComputePrtlDensities();

	IntegratePrtlMotions(simuTime);
}

void CPrtlFluid::MovePrtlsByGravity()
{
	if (CSimuManager::APPLY_GRAVITY)
		IntegratePrtlVelocitiesByGravity();
	IntegratePrtlPositions();
}

void CPrtlFluid::ComputeAvgVERatio()
{
	m_pfAvgVERatio = 0;
	int numFreePrtls = 0;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		m_pfAvgVERatio += CFluidPrtl::ComputeVERatio(&pPrtl->m_fpPresGrad, &pPrtl->m_fpStress);
		numFreePrtls ++;
	}
	if (numFreePrtls > 0)
		m_pfAvgVERatio /= numFreePrtls;
}

// since subclasses may do sth after computing prtl densities,
// it is convenient to make this function seperate from density computation
void CPrtlFluid::IntegratePrtlMotions(SimuValue simuTime)
{
	if (!m_pfPrtlEvolutionIsPrepared)
		CSimuMsg::ExitWithMessage("IntegratePrtlMotions(SimuValue simuTime)",
									"PreparePrtlEvolution() is not called first.");

	if (CSimuManager::APPLY_SURFACE_TENSION)
		ApplySurfaceTension();

	if (CSimuManager::m_eCompressibility == CSimuManager::GAS_STATE)
		IntegratePrtlVelocitiesWithGasStateEquation();

	if (CSimuManager::HEAT_TRANSFER)
		IntegratePrtlTemperatures();

	if (CSimuManager::VISCOSITY)
		IntegratePrtlVelocitiesWithViscosity();

	if (m_pbApplyGravity)
		IntegratePrtlVelocitiesByGravity();

	if (CSimuManager::NON_NEWTONIAN_FLUID)
		IntegratePrtlVelocitiesWithNonNewtonianStress();

	if (CSimuManager::ADJUST_VEL_WITH_NGBRS)
		AdjustPrtlVelocitiesWithNeighbors();

	IntegratePrtlPositions();

	if (CSimuManager::m_eCompressibility == CSimuManager::POISSON
	 && m_pfPoissonPauseSteps == 1)
	{
		m_pfPoissonPauseSteps = CSimuManager::m_maxPoissonPauseSteps;
		IntegratePrtlMotionsWithPoissonEquation();
	}
	else
		m_pfPoissonPauseSteps --;
}

void CPrtlFluid::ResetHeatTransferRateOnPrtls()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		m_pfPrtls[i]->m_fpHTRate = 0;
	}
}

void CPrtlFluid::ComputeTemperatureFromHeatTransferRateOnPrtls(SimuValue thermalTimeStep)
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		pPrtl->m_fpTemperature += pPrtl->m_fpHTRate*thermalTimeStep/pPrtl->m_fpMass;
	}
}

void CPrtlFluid::HeatTransferWithOtherFluids(CPrtlFluid* pOtherFluid)
{
	SimuValue heatConductivity = 0.5*(m_pfHeatConductivity+pOtherFluid->m_pfHeatConductivity);
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		ngbrPrtls.ResetArraySize(0);
		pOtherFluid->m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls);
		if (ngbrPrtls.m_paNum <= 0)
			continue;
		for (int j=0; j < ngbrPrtls.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = ngbrPrtls[j];
			SimuValue heatChange = heatConductivity;
			heatChange *= pPrtl->ComputeHeatChangeFromOneNgbr(ngbrPrtl, m_pfInterFluidRadius);
			pPrtl->m_fpHTRate += heatChange;
			ngbrPrtl->m_fpHTRate += -heatChange;
		}
	}
}

void CPrtlFluid::InteractWithOtherFluids()
{
	if (m_pfSimuManager == NULL) return;

	if (CSimuManager::m_eInterFluidModel == CSimuManager::REPULSIVE_FORCE)
		InteractWithOtherFluidsByRepulsiveForce();
	else if (CSimuManager::m_eInterFluidModel == CSimuManager::DETECT_BY_ISO_SRFC)
		InteractWithOtherFluidsBasedOnIsoSrfc();
	else if (CSimuManager::m_eInterFluidModel == CSimuManager::DETECT_BY_CONVEX_HULL)
		InteractWithOtherFluidsBasedOnConvexHull();
	else
		CSimuMsg::ExitWithMessage("CPrtlFluid::InteractWithOtherFluids()",
									"Unknown CSimuManager::m_eInterFluidModel.");
}

void CPrtlFluid::InteractWithOtherFluidsByRepulsiveForce()
{
	CVector3D normal0, normal1;
	SimuValue curvature0, curvature1;
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int k=0; k < m_pfSimuManager->m_aryPrtlFluid.m_paNum; k++)
	{
		CPrtlFluid* pFluid = m_pfSimuManager->m_aryPrtlFluid[k];
		if (pFluid == this) continue;
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			ngbrPrtls.ResetArraySize(0);
			pFluid->m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls);
			if (ngbrPrtls.m_paNum <= 0)
				continue;
			// compute surface normal0 of the other fluid
			CFluidPrtl::ComputeWeightGradient(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls, &normal0);
			// compute curvature0 of the other fluid
			curvature0 = CFluidPrtl::ComputeWeightLaplacian(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls);
			// compute surface normal1 of current fluid
			ngbrPrtls.ResetArraySize(0);
			m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls);
			CFluidPrtl::ComputeWeightGradient(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls, &normal1);
			// compute curvature1 of current fluid
			curvature1 = CFluidPrtl::ComputeWeightLaplacian(pPrtl->m_vpPos, m_pfInterFluidRadius, ngbrPrtls);
			// combine two normals
			normal0.AddedBy(&normal1, -1);
			// modify velocity
			pPrtl->m_vpVel->AddedBy(&normal0, -(curvature0 - curvature1)
											*CSimuManager::m_interFluidTension
											*m_pfTimeStep);
		}
	}
}

void CPrtlFluid::InteractWithOtherFluidsBasedOnIsoSrfc()
{
	if (m_pfSimuManager == NULL) return;

	SimuValue isoDensity;
	CVector3D tmpPos;
	CVector3D isoGradient;
	CVector3D velocity;
	SimuValue minDeltaIso = 0.01;
	int maxIterations = 50;
	for (int k=0; k < m_pfSimuManager->m_aryPrtlFluid.m_paNum; k++)
	{
		CPrtlFluid* pFluid = m_pfSimuManager->m_aryPrtlFluid[k];
		if (pFluid == this) continue;
		SimuValue curSMWgt = sqrt(m_pfShearModulus);
		SimuValue curRTWgt = sqrt(m_pfRelaxationTime);
		SimuValue othSMWgt = sqrt(pFluid->m_pfShearModulus);
		SimuValue othRTWgt = sqrt(pFluid->m_pfRelaxationTime);
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			tmpPos.SetValue(pPrtl->m_vpPos);
			isoDensity = pFluid->ComputeIsoDensityAndGradAndVelAtPos(tmpPos, isoGradient,
																	velocity, m_pfSmoothLen);
			if (isoDensity <= m_pfSrfcIsoDensity)
				continue;
			// modify velocity
			SimuValue wgtRatio = ModifyPrtlVelocity(pPrtl, &isoGradient, &velocity,
													curSMWgt, curRTWgt, othSMWgt, othRTWgt);
			m_pfNeedToRegisterMovedPrtls = true;
			// move prtl to iso-surface
			SimuValue stepSize = CSimuManager::m_prtlDistance*0.5;
			SimuValue deltaIso = fabs((isoDensity - m_pfSrfcIsoDensity)/m_pfDensity);
			int numSteps = 0;
			isoGradient.Normalize(-1); // such that <isoGradient> points outward surface
			while (deltaIso > minDeltaIso)
			{
				tmpPos.AddedBy(&isoGradient, stepSize);
				isoDensity = pFluid->ComputeDensityAtPos(tmpPos, m_pfSmoothLen);
				deltaIso = fabs((isoDensity - m_pfSrfcIsoDensity)/m_pfDensity);
				if (deltaIso <= minDeltaIso)
					break;
				if (isoDensity < m_pfSrfcIsoDensity && stepSize > 0)
				{
					stepSize *= -0.5;
				}
				if (isoDensity > m_pfSrfcIsoDensity && stepSize < 0)
				{
					stepSize *= -0.5;
				}
				numSteps ++;
				if (numSteps > maxIterations)
					break;
			}
			if (numSteps <= maxIterations)
				pPrtl->m_vpPos->SetValue(&tmpPos);
			else
				int dumb = 0;
		}
	}
}

void CPrtlFluid::InteractWithOtherFluidsBasedOnConvexHullWithOldWgt()
{
	CPoint3D tmpPoint;
	CVector3D srfcNormal;
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int k=0; k < m_pfSimuManager->m_aryPrtlFluid.m_paNum; k++)
	{
		CPrtlFluid* pFluid = m_pfSimuManager->m_aryPrtlFluid[k];
		if (pFluid == this) continue;
		SimuValue fluidRatio = sqrt(pFluid->m_pfShearModulus)
								/(sqrt(pFluid->m_pfShearModulus) + sqrt(m_pfShearModulus));
		bool bHarderFluid;
		if (m_pfShearModulus > pFluid->m_pfShearModulus
		 && fabs(m_pfShearModulus - pFluid->m_pfShearModulus) > SIMU_VALUE_EPSILON)
			bHarderFluid = true;
		else
			bHarderFluid = false;
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			ngbrPrtls.ResetArraySize(0);
			pFluid->m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, m_pfSmoothLen, ngbrPrtls);
			if (ngbrPrtls.m_paNum < CSimuManager::m_interSrfcNgbrNumThreshold)
				continue;
			tmpPoint.m_p3dPos.SetValue(pPrtl->m_vpPos);
			SimuValue distToSrfc = CConvexHull::DistanceFromExtraPointToConvexHull(&tmpPoint,
																				   ngbrPrtls,
																				   &srfcNormal);
			if (distToSrfc >= 0) // on or outside the other fluid
				continue;
			// move prtl towards fluid interface in proportion to ratio of two fluid rigidity
			if (bHarderFluid)
				pPrtl->m_vpPos->AddedBy(&srfcNormal, -distToSrfc*fluidRatio);
			else
				pPrtl->m_vpPos->AddedBy(&srfcNormal, -distToSrfc);

			SimuValue penetratingSpeed = pPrtl->m_vpVel->DotProduct(&srfcNormal);
			// cancel off penetrating velocity
			// and pPrtl->m_vpVel is tangential velocity now.
			pPrtl->m_vpVel->AddedBy(&srfcNormal, -penetratingSpeed);
			if (penetratingSpeed < 0) // indeed penetrating
			{
				// apply friction to tangential velocity
				pPrtl->m_vpVel->ScaledBy(CSimuManager::m_interFluidFriction);
				if (bHarderFluid)
					// add back penetrating velocity
					pPrtl->m_vpVel->AddedBy(&srfcNormal, penetratingSpeed*(1 - fluidRatio));
				// else not add back penetrating velocity
			}
			else // add back the non-penetrate velocity
				pPrtl->m_vpVel->AddedBy(&srfcNormal, penetratingSpeed);
		}
	}
}

void CPrtlFluid::InteractWithOtherFluidsBasedOnConvexHull()
{
	m_pfKEVariation = 0;

	CPoint3D tmpPoint;
	CVector3D srfcNormal;
	SimuValue vrtlMass;
	SimuValue wgtRatio;
	TGeomElemArray<CFluidPrtl> ngbrPrtls(false, SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	for (int k=0; k < m_pfSimuManager->m_aryPrtlFluid.m_paNum; k++)
	{
		CPrtlFluid* pFluid = m_pfSimuManager->m_aryPrtlFluid[k];
		if (pFluid == this) continue;
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			ngbrPrtls.ResetArraySize(0);
			pFluid->m_pfPrtlSearch.GetNgbrPrtls(pPrtl->m_vpPos, m_pfSmoothLen, ngbrPrtls);
			if (ngbrPrtls.m_paNum < CSimuManager::m_interSrfcNgbrNumThreshold)
			{
				if (CSimuManager::m_bStickyInterFluids)
					AttractedToNgbrPrtls(pPrtl, ngbrPrtls);
				continue;
			}
			tmpPoint.m_p3dPos.SetValue(pPrtl->m_vpPos);
			// collision detection
			SimuValue distToSrfc = CConvexHull::DistanceFromExtraPointToConvexHull(&tmpPoint,
																				   ngbrPrtls,
																				   &srfcNormal);
			if (distToSrfc >= 0)
			{
				if (CSimuManager::m_bStickyInterFluids)
					AttractedToNgbrPrtls(pPrtl, ngbrPrtls);
				continue; // no collision: prtl is on or outside the other fluid
			}
			// collision response
			CollisionResponse(pPrtl, ngbrPrtls, m_pfSmoothLen,
							m_pfPrtlMassRatio, pFluid->m_pfPrtlMassRatio,
							&srfcNormal, distToSrfc);
			m_pfNeedToRegisterMovedPrtls = true;
		}
	}
	// debug use only for kinetic energy watch
	SimuValue totalKE = 0;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		totalKE += pPrtl->m_fpMass*pPrtl->m_vpVel->LengthSquare();
	}
	if (totalKE > SIMU_VALUE_EPSILON)
		m_pfKEVariation /= totalKE;
	else
		m_pfKEVariation = 0;
}

void CPrtlFluid::AttractedToNgbrPrtls(CFluidPrtl* pPrtl,
									  TGeomElemArray<CFluidPrtl> &ngbrPrtls)
{
	CVector3D relativePos;
	CVector3D interfaceNormal; interfaceNormal.SetElements(0, 0, 0);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		relativePos.SetValueAsSubstraction(ngbrPrtl->m_vpPos, pPrtl->m_vpPos);
		SimuValue dist = relativePos.Length();
		SimuValue weight = CSimuUtility::SplineWeightFunction(dist, m_pfSmoothLen);
		weight *= ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		interfaceNormal.AddedBy(&relativePos, weight/dist);
	}
	interfaceNormal.Normalize();

	SimuValue moveawaySpeed = 0;
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		SimuValue relativeSpeed = interfaceNormal.DotProduct(ngbrPrtl->m_vpVel);
		SimuValue dist = pPrtl->m_vpPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
		SimuValue weight = CSimuUtility::SplineWeightFunction(dist, m_pfSmoothLen);
		weight *= ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		moveawaySpeed += weight*relativeSpeed;
	}
	if (moveawaySpeed > 0) // means ngbr prtls moving away
		pPrtl->m_vpVel->AddedBy(&interfaceNormal, 
								moveawaySpeed*m_pfTimeStep*CSimuManager::m_interFluidStickyness);
}

SimuValue CPrtlFluid::CollisionResponse(CFluidPrtl* pPrtl,
										TGeomElemArray<CFluidPrtl> &ngbrPrtls,
										SimuValue smoothLen,
										SimuValue prtlMassRatio, SimuValue vrtlPrtlMassRatio,
										CVector3D* srfcNormal, SimuValue distToSrfc)
{
	SimuValue preKE = 0, postKE = 0;
	// interpolate virtual particle mass and velocity
	CVector3D vrtlVel; vrtlVel.SetValue((SimuValue)0);
	SimuValue vrtlMass = 0;
	SimuValue dist, totalWeight = 0;
	SimuValue *weight; weight = new SimuValue[ngbrPrtls.m_paNum];
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		dist = pPrtl->m_vpPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
		if (dist > smoothLen) { weight[i] = 0; continue; }
		weight[i] = CSimuUtility::SplineWeightFunction(dist, smoothLen);
		weight[i] *= ngbrPrtl->m_fpMass/ngbrPrtl->m_fpDensity;
		totalWeight += weight[i];
		vrtlMass += ngbrPrtl->m_fpMass*weight[i];
		vrtlVel.AddedBy(ngbrPrtl->m_vpVel, weight[i]);
		// debug
		preKE += ngbrPrtl->m_fpMass*ngbrPrtl->m_vpVel->LengthSquare();
	}
	preKE += pPrtl->m_fpMass*pPrtl->m_vpVel->LengthSquare();
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		vrtlMass /= totalWeight;
		vrtlVel.ScaledBy(1/totalWeight);
	}
	SimuValue m1 = pPrtl->m_fpMass*prtlMassRatio;
	SimuValue m2 = vrtlMass*vrtlPrtlMassRatio;
	// move prtl onto surface and compute displacement velocity
	SimuValue movingDist;
	if (!CSimuManager::m_bRepulsiveResponseOnly)
	{
		movingDist = -distToSrfc; // no weight to pos modification
	}
	else
	{
		movingDist = -distToSrfc*m2/(m1+m2);
	}
	pPrtl->m_vpPos->AddedBy(srfcNormal, movingDist);
	// add prtl displacement velocity
	CVector3D oldPrtlVel;
	oldPrtlVel.SetValue(pPrtl->m_vpVel);
	pPrtl->m_vpVel->AddedBy(srfcNormal, m_pfInterFluidDamping*movingDist/m_pfTimeStep);
	if (!CSimuManager::m_bRepulsiveResponseOnly)
	{
		// compute after-collision velocity
		pPrtl->m_vpVel->ScaledBy(m1 - m2);
		pPrtl->m_vpVel->AddedBy(&vrtlVel, 2*m2);
		pPrtl->m_vpVel->ScaledBy(1/(m1 + m2));
	}
	// conserve delta momentum
	if (totalWeight > SIMU_VALUE_EPSILON)
	{
		// compute delta momentum
		CVector3D deltaMomentum;
		deltaMomentum.SetValue(&oldPrtlVel);
		deltaMomentum.AddedBy(pPrtl->m_vpVel, -1);
		deltaMomentum.ScaledBy(m1);
		// modify neighboring particle velocities for momentum conservation
		CVector3D prtlDeltaMomentum;
		for (int i=0; i < ngbrPrtls.m_paNum; i++)
		{
			CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
			if (weight[i] <= SIMU_VALUE_EPSILON) continue;
			prtlDeltaMomentum.SetValue(&deltaMomentum, weight[i]/totalWeight); 
			prtlDeltaMomentum.ScaledBy(1/(ngbrPrtl->m_fpMass*vrtlPrtlMassRatio));
			ngbrPrtl->m_vpVel->AddedBy(&prtlDeltaMomentum);
			// debug
			postKE += ngbrPrtl->m_fpMass*ngbrPrtl->m_vpVel->LengthSquare();
		}
		postKE += pPrtl->m_fpMass*pPrtl->m_vpVel->LengthSquare();
	}
	else if (totalWeight <= SIMU_VALUE_EPSILON)
		CSimuMsg::ExitWithMessage("CPrtlFluid::ModifyPrtlVelocity(...)",
								"Total weight is zero.");

	m_pfKEVariation += postKE - preKE;

	delete weight;
	return vrtlMass;
}

// return false if prtl velocity is not modified.
bool CPrtlFluid::ModifyPrtlVelocity(CFluidPrtl* pPrtl, CVector3D* fluidVelocity,
									CVector3D* srfcNormal, 
									SimuValue wgtPrtl, SimuValue wgtFluid)
{
	bool velModified = true;
	SimuValue prtlSpeed = pPrtl->m_vpVel->DotProduct(srfcNormal);
	SimuValue fluidSpeed = fluidVelocity->DotProduct(srfcNormal);
	SimuValue resultSpeed = (prtlSpeed*wgtPrtl + fluidSpeed*wgtFluid)/(wgtPrtl+wgtFluid);
	// cancel off prtl speed such that pPrtl->m_vpVel becomes tangential velocity.
	pPrtl->m_vpVel->AddedBy(srfcNormal, -prtlSpeed);
	// apply friction to tangential velocity
	pPrtl->m_vpVel->ScaledBy(CSimuManager::m_interFluidFriction);
	// add resulting velocity
/*	if (fabs(resultSpeed) > fabs(prtlSpeed))
	{
		if (resultSpeed > prtlSpeed && prtlSpeed > 0)
			pPrtl->m_vpVel->AddedBy(srfcNormal, prtlSpeed);
		else if (resultSpeed < prtlSpeed && prtlSpeed < 0)
			pPrtl->m_vpVel->AddedBy(srfcNormal, prtlSpeed);
		else
			pPrtl->m_vpVel->AddedBy(srfcNormal, -prtlSpeed);
	}
	else
*/		pPrtl->m_vpVel->AddedBy(srfcNormal, resultSpeed);
	CSimuUtility::AssertSimuVector3D(pPrtl->m_vpVel);
	return velModified;
}

// return -1, if peneratingSpeed is not modified.
SimuValue CPrtlFluid::ModifyPrtlVelocity(CFluidPrtl* pPrtl, CVector3D* srfcNormal,
										 CVector3D* resistVelocity,
										 SimuValue curSMWgt, SimuValue curRTWgt,
										 SimuValue othSMWgt, SimuValue othRTWgt)
{
	SimuValue wgtRatio = -1;
	SimuValue penetratingSpeed = pPrtl->m_vpVel->DotProduct(srfcNormal);
	// cancel off penetrating velocity
	// and pPrtl->m_vpVel is tangential velocity now.
	pPrtl->m_vpVel->AddedBy(srfcNormal, -penetratingSpeed);
	if (penetratingSpeed < 0) // indeed penetrating
	{
		// apply friction to tangential velocity
		pPrtl->m_vpVel->ScaledBy(CSimuManager::m_interFluidFriction);

		SimuValue resistSpeed = resistVelocity->DotProduct(srfcNormal);
		SimuValue prtlWgt = fabs(penetratingSpeed)*curSMWgt*curRTWgt;
		SimuValue resistWgt = fabs(resistSpeed)*othSMWgt*othRTWgt;
		wgtRatio = prtlWgt/(prtlWgt + resistWgt);
		if (resistSpeed > 0) // indeed resist
		{
			if (prtlWgt > resistWgt)
			{
				// add some of penetrating velocity
				pPrtl->m_vpVel->AddedBy(srfcNormal, penetratingSpeed*wgtRatio);
			}
			// else prtl is stopped and not add any penetrating velocity
		}
		else if (penetratingSpeed < resistSpeed) // resistSpeed slower than prtl
		{
			// add back weighted average of penetrating and resist velocity
			pPrtl->m_vpVel->AddedBy(srfcNormal, penetratingSpeed*wgtRatio
												+resistSpeed*(1-wgtRatio));
		}
		else // resistSpeed is faster than prtl
		{
			// add back penetrating velocity
			pPrtl->m_vpVel->AddedBy(srfcNormal, penetratingSpeed);
			wgtRatio = 1; // set new return value such that prtl pos is not moved.
		}
	}
	else // add back the non-penetrate velocity
		pPrtl->m_vpVel->AddedBy(srfcNormal, penetratingSpeed);
	return wgtRatio;
}

void CPrtlFluid::ComputeSrfcIsoDensity()
{
	m_pfSrfcIsoDensity = 0;
	int numSrfcPrtls = 0;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface)
		{
			m_pfSrfcIsoDensity += pPrtl->m_fpDensity;
			numSrfcPrtls ++;
		}
	}
	if (numSrfcPrtls > 0)
	{
		m_pfSrfcIsoDensity /= numSrfcPrtls;
	}
}

void CPrtlFluid::ComputeVelocityAtPos(CVector3D &pos, CVector3D &vel, SimuValue radius)
{
	vel.SetElements(0, 0, 0);
	SimuValue radiusSquare = radius*radius;
	SimuValue dist, weight, totalVelWeight = 0;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		dist = pos.GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		weight *= ngbrPrtl->m_fpMass;
		weight /= ngbrPrtl->m_fpDensity;
		vel.AddedBy(ngbrPrtl->m_vpVel, weight);
		totalVelWeight += weight;
	}
	if (totalVelWeight > 0)
		vel.ScaledBy(1/totalVelWeight);
}

SimuValue CPrtlFluid::ComputeIsoDensityAndGradAndVelAtPos(CVector3D &pos, CVector3D &grad,
														  CVector3D &vel, SimuValue radius)
{
	grad.SetElements(0, 0, 0);
	vel.SetElements(0, 0, 0);
	SimuValue radiusSquare = radius*radius;
	SimuValue isoDensity = 0;
	SimuValue dist, weight, gradWeight, totalVelWeight = 0;
	CVector3D vector;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		vector.SetValueAsSubstraction(&pos, ngbrPrtl->m_vpPos);
		dist = vector.LengthSquare();
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		weight *= ngbrPrtl->m_fpMass;
		isoDensity += weight;
		weight /= ngbrPrtl->m_fpDensity;
		vel.AddedBy(ngbrPrtl->m_vpVel, weight);
		totalVelWeight += weight;
		if (dist <= SIMU_VALUE_EPSILON) continue;
		gradWeight = CSimuUtility::SplineGradientWeightFunction(dist, radius);
		vector.ScaledBy(1/dist);
		grad.AddedBy(&vector, gradWeight);
	}
	if (totalVelWeight > 0)
		vel.ScaledBy(1/totalVelWeight);
	return isoDensity;
}

void CPrtlFluid::MoveFluidBoundary(SimuValue simuTime)
{
	for (int i=0; m_pfSimuManager != NULL && i < m_pfSimuManager->m_aryBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfSimuManager->m_aryBoundaries[i];
		pBdry->MoveBoundary(simuTime, m_pfTimeStep);
		if (pBdry->m_bdryType == CBoundary::RIGID_BALL)
		{
			CRigidBall* pBall = (CRigidBall*)pBdry;
			for (int j=0; j < m_pfSimuManager->m_aryBoundaries.m_paNum; j++)
			{
				CBoundary* otherBdry = m_pfSimuManager->m_aryBoundaries[j];
				if (otherBdry == pBdry) continue;
				pBall->EnforceBallMotionWithBoundary(otherBdry);
			}
		}
	}
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		pBdry->MoveBoundary(simuTime, m_pfTimeStep);
		if (pBdry->m_bdryType == CBoundary::RIGID_BALL)
		{
			CRigidBall* pBall = (CRigidBall*)pBdry;
			for (int j=0; j < m_pfBoundaries.m_paNum; j++)
			{
				CBoundary* otherBdry = m_pfBoundaries[j];
				if (otherBdry == pBdry) continue;
				pBall->EnforceBallMotionWithBoundary(otherBdry);
			}
		}
	}
}

void CPrtlFluid::EnforceFluidBoundary(SimuValue simuTime)
{
	for (int i=0; m_pfSimuManager != NULL && i < m_pfSimuManager->m_aryBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfSimuManager->m_aryBoundaries[i];
		pBdry->EnforceBoundaryConstraint(this);
		if (CSimuManager::HEAT_TRANSFER && pBdry->m_bdryThermalBody)
			pBdry->HeatTransferOnBdry(this);
		if (pBdry->m_bdryType == CBoundary::NOZZLE)
		{
			CSimuFluidNozzle* pNozzle = (CSimuFluidNozzle*)pBdry;
			pNozzle->InjectFluidParticles(simuTime, this);
			pNozzle->StopFlowing(simuTime, this);
		}
	}
	for (int i=0; i < m_pfBoundaries.m_paNum; i++)
	{
		CBoundary* pBdry = m_pfBoundaries[i];
		pBdry->EnforceBoundaryConstraint(this);
		if (pBdry->m_bdryType == CBoundary::NOZZLE)
		{
			CSimuFluidNozzle* pNozzle = (CSimuFluidNozzle*)pBdry;
			pNozzle->InjectFluidParticles(simuTime, this);
			pNozzle->StopFlowing(simuTime, this);
		}
	}
	if (CSimuManager::NON_NEWTONIAN_FLUID
	 && CSimuManager::DUMP_STRESS_TENSOR)
		DumpStressTensorOnPrtlsInCollision();

	m_pfNeedToRegisterMovedPrtls = true;
}

void CPrtlFluid::DumpStressTensorOnPrtlsInCollision()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpInCollision)
		{
			CFluidPrtlNonNew* nnPrtl = (CFluidPrtlNonNew*)pPrtl;
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
			{
				nnPrtl->m_fpnnStrTen[m][n] *= CSimuManager::m_stressDumpingRate;
			}
		}
	}
}

void CPrtlFluid::ApplySurfaceTension()
{
	CVector3D normal;
	SimuValue curvature;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (!pPrtl->m_fpOnSurface || pPrtl->m_fpFixedMotion) continue;
		// compute surface normal
		CFluidPrtl::ComputeWeightGradient(pPrtl->m_vpPos, m_pfSmoothLen, pPrtl->m_fpNgbrs, &normal);
		normal.Normalize();
		// compute curvature
		curvature = CFluidPrtl::ComputeWeightLaplacian(pPrtl->m_vpPos, m_pfSmoothLen, pPrtl->m_fpNgbrs);
		// modify velocity
		pPrtl->m_vpVel->AddedBy(&normal,
								-curvature*CSimuManager::m_surfaceTension*m_pfTimeStep);
	}
}

void CPrtlFluid::IntegratePrtlVelocitiesWithGasStateEquation()
{
	ComputePrtlPressuresUsingGasStateEquation();
	ComputePrtlPressureGradients();
	IntegratePrtlVelocitiesByPresGrad();
}

void CPrtlFluid::IntegratePrtlTemperatures()
{
	SimuValue thermalTimeStep = m_pfTimeStep/m_pfSubThermalSteps;
	for (int d=0; d < m_pfSubThermalSteps; d++)
	{
		// compute heat changes on prtls
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			pPrtl->m_fpHTRate = m_pfHeatConductivity*pPrtl->ComputeHeatChange(m_pfSmoothLen);
		}
		// integrate temperatures on prtls
		for (int i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtl* pPrtl = m_pfPrtls[i];
			if (CSimuManager::NOT_CONSERVE_HEAT)
				pPrtl->m_fpTemperature += pPrtl->m_fpHTRate*thermalTimeStep;
			else
				pPrtl->m_fpTemperature += pPrtl->m_fpHTRate*thermalTimeStep/pPrtl->m_fpMass;
			if (CSimuUtility::IsInvalidSimuValue(pPrtl->m_fpTemperature))
				int dumpStop = 0;
		}
	}
}

void CPrtlFluid::IntegratePrtlVelocitiesWithViscosity()
{
	CVector3D viscosity;
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->ComputeViscosity(viscosity, m_pfSmoothLen);
		pPrtl->AddVelocity(&viscosity, m_pfTimeStep);
	}
}

void CPrtlFluid::IntegratePrtlVelocitiesWithNonNewtonianStress()
{
	CSimuManager::m_maxAngVelTenElmnt = 0;
	if (CSimuManager::m_eStressModel == CSimuManager::PLASTIC)
		IntegratePrtlStressTensorsByPlasticModel();
	else if (CSimuManager::m_eStressModel == CSimuManager::VISCOELASTIC)
		IntegratePrtlStressTensorsByElasticModel();
	else if (CSimuManager::m_eStressModel == CSimuManager::LINEAR)
		IntegratePrtlStressTensorsByLinearModel();
	else
		CSimuMsg::ExitWithMessage("CPrtlFluid::IntegratePrtlVelocitiesWithNonNewtonianStress()",
								"Unknow stress model");
	if (CSimuManager::ADJUST_TENSOR_WITH_NGBRS)
		AdjustPrtlStressTensorsWithNgbrs();
	if (CSimuManager::IMPLICIT_STRESS_INTEGRATION)
		ImplicitIntegratePrtlVelocitiesWithNonNewtonianStress();
	else
		IntegratePrtlVelocitiesByStressTensor();
}

// prtl densities and stress tensors have been updated before calling this function
void CPrtlFluid::ImplicitIntegratePrtlVelocitiesWithNonNewtonianStress()
{
	char location[] = "CPrtlFluid::ImplicitIntegratePrtlVelocitiesWithNonNewtonianStress()";

	SetupSymmetricSparseMatrixForImplicitStressIntegration();
	m_pfCGSolver3D.AssertSymmetricSparseMatrix();
	m_pfInterations3D = m_pfCGSolver3D.SolveSymmetricMatrix(m_pfCGSolver3D.m_matrixSize,
															CSimuManager::m_minPrtlDistance);
	m_pfDeltaPos.ResetArraySize(m_pfPrtls.m_paNum);
	int i;
	int notFixedPrtls = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		m_pfCGSolver3D.GetVariableVectorElement(notFixedPrtls++, m_pfDeltaPos[i]);
//		CSimuUtility::AssertSimuVector3D(deltaPos[i]);
		if (CSimuManager::m_bInvalidValue)
		{
			pPrtl->m_geHighlit = true;
			CSimuMsg::PauseMessage("Invalid deltaPos on highlit prtl.");
			pPrtl->m_geHighlit = false;
			CSimuManager::m_bInvalidValue = false;
		}
	}
	// compute prtl vel
	SimuValue timeStepReciprocal = 1/m_pfTimeStep;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_vpVel->SetValue(m_pfDeltaPos[i], timeStepReciprocal);
	}
}

void CPrtlFluid::IntegratePrtlMotionsWithPoissonEquation()
{
	RegisterPrtlsForNeighborSearchIfNotYet();
	SearchPrtlNeighbors();
	ComputePrtlDensities();
	if (m_pfSrfcDetectPauseSteps == 1)
	{
		m_pfSrfcDetectPauseSteps = CSimuManager::m_maxSrfcDetectPauseSteps;
		DetectSrfcPrtls();
	}
	else
		m_pfSrfcDetectPauseSteps --;
	EnforceIncompressibilityUsingPoissonEquation();
}

void CPrtlFluid::ComposeSimuInfoAfterOneTimeStep(int numTimeSteps)
{
	char tmpStr[32];

	sprintf(tmpStr, "[%d]", numTimeSteps); m_pfSimuInfo = tmpStr;
	sprintf(tmpStr, " n=%d(%d)", m_pfPrtls.m_paNum, (int)ceil(m_pfAvgNumNgbrs)); m_pfSimuInfo += tmpStr;
	if (CSimuManager::ADAPTIVE_TIME_STEP)
	{
		sprintf(tmpStr, " dt=%.5f", m_pfTimeStep); m_pfSimuInfo += tmpStr;
	}
	if (CSimuManager::NON_NEWTONIAN_FLUID)
	{
		sprintf(tmpStr, " mv=%.1f", m_pfMaxVortex); m_pfSimuInfo += tmpStr;
		sprintf(tmpStr, " ma=%.0f", m_pfMaxAngTen); m_pfSimuInfo += tmpStr;
		sprintf(tmpStr, " mt=%.0f", m_pfMaxTensor); m_pfSimuInfo += tmpStr;
		if (CSimuManager::IMPLICIT_STRESS_INTEGRATION)
		{
			sprintf(tmpStr, " it3=%d", m_pfInterations3D); m_pfSimuInfo += tmpStr;
		}
	}
//	sprintf(tmpStr, " de=%2.0f", m_pfKEVariation*100); m_pfSimuInfo += tmpStr;
	if (CSimuManager::m_eCompressibility == CSimuManager::POISSON)
	{
		sprintf(tmpStr, " it=%d", m_pfInterations); m_pfSimuInfo += tmpStr;
		m_pfInterations = 0;
	}
	sprintf(tmpStr, " itm=%.2f", m_pfInteractTime); m_pfSimuInfo += tmpStr;
	sprintf(tmpStr, " mtm=%.2f", m_pfMotionTime); m_pfSimuInfo += tmpStr;
	if (CSimuManager::CREATE_SURFACE_MESH)
	{
		sprintf(tmpStr, " stm=%.1f", m_pfSurfaceTime); m_pfSimuInfo += tmpStr;
	}
	sprintf(tmpStr, " mcs=%.1f", CBoundary::m_bdryMaxClsSpeed); m_pfSimuInfo += tmpStr;
	CBoundary::m_bdryMaxClsSpeed = 0;
}

void CPrtlFluid::ShowSimuInfoAfterOneTimeStep(int numTimeSteps)
{
	ComposeSimuInfoAfterOneTimeStep(numTimeSteps);

	TRACE((LPCTSTR)m_pfSimuInfo);
	TRACE("\n");
	if (CSimuManager::m_bShowMsg)
		CSimuMsg::DisplayMessage((LPCTSTR)m_pfSimuInfo, false);
}

void CPrtlFluid::SelectParticlesEnclosedBySphere(CVector3D &center, SimuValue radius)
{
	RegisterPrtlsForNeighborSearchIfNotYet();
	TPointerArray<CFluidPrtl> aryPrtls;
	aryPrtls.InitializePointerArray(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetNgbrPrtls(&center, radius, aryPrtls);
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = aryPrtls[i];
		pPrtl->m_geSelected = true;
	}
}

void CPrtlFluid::DeSelectParticlesEnclosedBySphere(CVector3D &center, SimuValue radius)
{
	RegisterPrtlsForNeighborSearchIfNotYet();
	TPointerArray<CFluidPrtl> aryPrtls;
	aryPrtls.InitializePointerArray(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetNgbrPrtls(&center, radius, aryPrtls);
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		aryPrtls[i]->m_geSelected = false;
	}
}

void CPrtlFluid::DeSelectAllParticles()
{
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		m_pfPrtls[i]->m_geSelected = false;
	}
}

void CPrtlFluid::ComputePrtlPressuresUsingGasStateEquation()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
		m_pfPrtls[i]->m_fpPressure
			= ComputeGasStatePressure(m_pfPrtls[i]->m_fpDensity, m_pfDensity);
}

void CPrtlFluid::EnforceIncompressibilityUsingPoissonEquation()
{
	char location[] = "CPrtlFluid::EnforceIncompressibilityUsingPoissonEquation()";

	int i;

	bool bSurfacePrtlExist = false;
	SimuValue timeStepSqre = m_pfTimeStep*m_pfTimeStep;
	// set up and solve Poisson equation
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlPoisson* pPrtl = (CFluidPrtlPoisson*)m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface)
			bSurfacePrtlExist = true;
		else
			pPrtl->ComputeLaplacianCoefficients(m_pfSmoothLen, m_pfDensity, timeStepSqre);
	}
	if (!bSurfacePrtlExist)
	{
//		CSimuMsg::ExitWithMessage(location,
//								"Need at least one surface particle for Poisson equation.");
		return;
	}
	if (SetupSymmetricSparseMatrix())
	{
		m_pfCGSolver.AssertSymmetricSparseMatrix();
		m_pfInterations = m_pfCGSolver.SolveSymmetricMatrix(100, m_pfCGTolerance);

		// assign solved pressures to particles
		int idInsidePrtls = 0;
		for(i=0; i < m_pfPrtls.m_paNum; i++)
		{
			CFluidPrtlPoisson* pPrtl = (CFluidPrtlPoisson*)m_pfPrtls[i];
			if (!pPrtl->m_fpOnSurface)
			{
				pPrtl->m_fpPressure = m_pfCGSolver.m_X[idInsidePrtls++];
//				CSimuUtility::AssertSimuValue(pPrtl->m_fpPressure);
				if (CSimuManager::m_bInvalidValue)
				{
					pPrtl->m_geHighlit = true;
//					CSimuMsg::PauseMessage("Invalid pressure on highlit prtl.");
					pPrtl->m_geHighlit = false;
					pPrtl->m_fpPressure = m_pfAirPressure;
					CSimuManager::m_bInvalidValue = false;
				}
			}
		}
		if (idInsidePrtls != m_pfCGSolver.m_matrixSize)
			CSimuMsg::ExitWithMessage(location, "Inside particles are not correctly numbered.");
	}
	else
	{
//		CSimuMsg::ExitWithMessage(location, "No inside particle is found.");
		ComputePrtlPressuresUsingGasStateEquation();
	}

	ComputePrtlPressureGradients();

	// integrate particles under pressure gradiens
	if (CSimuManager::ADAPTIVE_TIME_STEP)
	{
		SimuValue maxMagnitude = -SIMU_VALUE_MAX;
		for (i=0; i < m_pfPrtls.m_paNum; i++)
		{
			SimuValue magnitude = m_pfPrtls[i]->ComputeMaxRelativePresGradMagnitude();
			if (maxMagnitude < magnitude)
				maxMagnitude = magnitude;
		}
		if (maxMagnitude <= SIMU_VALUE_EPSILON)
			m_pfTimeStep = m_pfMaxTimeStep;
		
		const SimuValue maxRelativeMovement = CSimuManager::m_courantFactor* m_pfPrtlDist;
		//const SimuValue maxRelativeMovement = CSimuManager::m_courantFactor*CSimuManager::m_prtlDistance;
		SimuValue curMaxRelaMovement = maxMagnitude*m_pfMaxTimeStep*m_pfMaxTimeStep;
		SimuValue timeStep = sqrt(maxRelativeMovement/maxMagnitude);
		int timeStepLevel = (int)floor(log10(timeStep)/SIMU_CONST_LOG_10_2);
		m_pfTimeStep = pow(2.0f, timeStepLevel);
		if (m_pfTimeStep > m_pfMaxTimeStep)
			m_pfTimeStep = m_pfMaxTimeStep;
	}
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_vpPos->AddedBy(&pPrtl->m_fpPresGrad, timeStepSqre);
		pPrtl->AddVelocity(&pPrtl->m_fpPresGrad, m_pfTimeStep);
//		CSimuUtility::AssertSimuVector3D(pPrtl->m_vpPos);
	}
	m_pfNeedToRegisterMovedPrtls = true;
}

//////////////////////////////////////////////////////////////////////////////////
// Data Structure for preconditioned biconjugate gradient method.
// Note: all particles' integration values must be ready.
//////////////////////////////////////////////////////////////////////////////////
// 2 arrays, m_indexArray and m_valueArray, are constructed based on
// the information of all the particles
bool CPrtlFluid::SetupSymmetricSparseMatrix()
{
	char location[] = "CPrtlFluid::SetupSymmetricSparseMatrix()";

	int i;

	m_pfCGSolver.FreeSparseMatrixStorage();

	// m_arraySize = m_matrixSize + 1 + (total inside neighbors on all inside particles)
	m_pfCGSolver.m_arraySize = 0;
	int idInsidePrtls = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlPoisson* pPrtl = (CFluidPrtlPoisson*)m_pfPrtls[i];
		if (!pPrtl->m_fpOnSurface)
		{
			pPrtl->m_fpId = idInsidePrtls++;
			m_pfCGSolver.m_arraySize += pPrtl->m_fppNumInsideNgbrs;
		}
	}
	if (idInsidePrtls == 0)
	{
		return false; // "No inside particle, and empty matrix.";
	}

	m_pfCGSolver.m_matrixSize = idInsidePrtls;
	m_pfCGSolver.m_arraySize += m_pfCGSolver.m_matrixSize + 1;

	m_pfCGSolver.AllocateSparseMatrixStorage();

	// fill in 2 1D-arrays with the matrix values from all particles
	int nArrayId = m_pfCGSolver.m_matrixSize + 1;
	int nMatrixId = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlPoisson* pPrtl = (CFluidPrtlPoisson*)m_pfPrtls[i];
		if (pPrtl->m_fpOnSurface) continue;
		m_pfCGSolver.m_B[nMatrixId] = pPrtl->m_fppConst;
		// initial guesses = 0
		m_pfCGSolver.m_X[nMatrixId] = 0;
		// In the range 0 to matrixSize-1 in the 2 1D-arrays,
		// m_valueArray is filled with diagonal values for rows, 0 to matrixSize-1;
		// m_indexArray is filled with starting indices for the rows in the 2 1D-arrays
		m_pfCGSolver.m_valueArray[nMatrixId] = pPrtl->m_fppDiagCoeff;
		m_pfCGSolver.m_indexArray[nMatrixId] = nArrayId;
		nMatrixId ++;

		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtlPoisson* pNgbrPrtl = (CFluidPrtlPoisson*)pPrtl->m_fpNgbrs[j];
			if (pNgbrPrtl->m_fpOnSurface) continue;
			if (pNgbrPrtl->m_fpId < 0 || pNgbrPrtl->m_fpId >= m_pfCGSolver.m_matrixSize)
				CSimuMsg::ExitWithMessage(location, "Invalid inside particle id");
			if (fabs(pPrtl->m_fppNgbrCoeffs[j] - SIMU_VALUE_MAX) < SIMU_VALUE_EPSILON)
				CSimuMsg::ExitWithMessage(location, "Invalid off-diagonal element");
			m_pfCGSolver.m_indexArray[nArrayId] = pNgbrPrtl->m_fpId;
			m_pfCGSolver.m_valueArray[nArrayId] = pPrtl->m_fppNgbrCoeffs[j];
			nArrayId ++;
		}
	}
	if (nArrayId == m_pfCGSolver.m_arraySize)
		m_pfCGSolver.m_indexArray[m_pfCGSolver.m_matrixSize] = nArrayId;
		// and m_valueArray[m_matrixSize] will be undefined and not used.
	else
		CSimuMsg::ExitWithMessage(location, "Sparse matrix arrays are not correct in size.");
	return true;
}

void CPrtlFluid::SetupSymmetricSparseMatrixForImplicitStressIntegration()
{
	char location[] = "CPrtlFluid::SetupSymmetricSparseMatrixForImplicitStressIntegration()";

	int i;

	m_pfCGSolver3D.FreeSparseMatrixStorage();

	// m_arraySize = m_matrixSize + 1 + (total neighbors on all not-fixed particles)
	m_pfCGSolver3D.m_arraySize = 0;
	int notFixedPrtls = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_fpId = notFixedPrtls++;
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = pPrtl->m_fpNgbrs[j];
			if (ngbrPrtl->m_fpFixedMotion) continue;
			m_pfCGSolver3D.m_arraySize ++;
		}
	}
	m_pfCGSolver3D.m_matrixSize = notFixedPrtls;
	m_pfCGSolver3D.m_arraySize += m_pfCGSolver3D.m_matrixSize + 1;

	m_pfCGSolver3D.AllocateSparseMatrixStorage();

	CVector3D tmpStress;
	SimuValue matrixForceDerivative[9];
	// fill in 2 1D-arrays with the matrix values from all particles
	SimuValue timeStepSqr = m_pfTimeStep*m_pfTimeStep;
	int nArrayId = m_pfCGSolver3D.m_matrixSize + 1;
	int nMatrixId = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		// initial guesses = 0
		m_pfCGSolver3D.SetVariableVectorElement(nMatrixId, (SimuValue)0);
		// set constants
		m_pfCGSolver3D.SetConstantVectorElement(nMatrixId, pPrtl->m_vpVel, m_pfTimeStep);
		pPrtl->ComputeStressOnPrtl(tmpStress, m_pfSmoothLen);
		m_pfCGSolver3D.AddConstantVectorElement(nMatrixId, &tmpStress, timeStepSqr);
		// In the range 0 to matrixSize-1 on the 2 1D-arrays,
		// m_valueArray is filled with diagonal values for rows, 0 to matrixSize-1;
		// m_indexArray is filled with starting indices for the rows in the 2 1D-arrays
		m_pfCGSolver3D.m_indexArray[nMatrixId] = nArrayId;
		m_pfCGSolver3D.SetValueArrayElementAsUnitMatrix(nMatrixId);
		for (int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtlNonNew* pNgbrPrtl = (CFluidPrtlNonNew*)pPrtl->m_fpNgbrs[j];
			ComputeStressDerivativeOnPrtl(pPrtl, pNgbrPrtl, matrixForceDerivative);
			// accumulate diagonal element from neighbor spring
			m_pfCGSolver3D.AddValueArrayElement(nMatrixId, matrixForceDerivative, -timeStepSqr);

			if (pNgbrPrtl->m_fpFixedMotion)
				continue; // ngbr of fixed motion is not present in matrix

			// fill in off-diagonal elements from neighbor springs
			// m_pfCGSolver3D.m_indexArray[nArrayId] is the column number in sparse matrix
			m_pfCGSolver3D.m_indexArray[nArrayId] = pNgbrPrtl->m_fpId;
			// m_pfCGSolver3D.m_valueArray[nArrayId] is the element in sparse matrix
			m_pfCGSolver3D.SetValueArrayElement(nArrayId, matrixForceDerivative, timeStepSqr);
			nArrayId ++;
		}
		nMatrixId ++;
	}

	if (nArrayId == m_pfCGSolver3D.m_arraySize)
		m_pfCGSolver3D.m_indexArray[m_pfCGSolver3D.m_matrixSize] = nArrayId;
		// and m_valueArray[m_matrixSize] will be undefined and not used.
	else
		CSimuMsg::ExitWithMessage(location, "Sparse matrix arrays are not correct in size.");
}

void CPrtlFluid::ComputeStressDerivativeOnPrtl(CFluidPrtlNonNew* pPrtl,
											   CFluidPrtlNonNew* pNgbrPrtl,
											   SimuValue matrixFD[9])
{
	SimuValue prtlDenReciprocal = 1/(pPrtl->m_fpDensity*pPrtl->m_fpDensity);
	SimuValue ngbrDenReciprocal = 1/(pNgbrPrtl->m_fpDensity*pNgbrPrtl->m_fpDensity);
	CVector3D deltaPos; deltaPos.SetValueAsSubstraction(pPrtl->m_vpPos, pNgbrPrtl->m_vpPos);
	SimuValue distSqr = deltaPos.LengthSquare();
	SimuValue dist = sqrt(distSqr);
	SimuValue tmp1 = CFluidPrtlNonNew::StressFirstDerivativeWeightFunction(dist, m_pfSmoothLen);
	tmp1 /= dist;
	SimuValue tmp2 = CFluidPrtlNonNew::StressSecondDerivativeWeightFunction(dist, m_pfSmoothLen);
	tmp2 -= tmp1;
	tmp2 /= distSqr;
	for (int i=0; i < 3; i++) // loop on alpha
	for (int j=0; j < 3; j++) // loop on beta
	{
		matrixFD[i*3+j] = 0;
		SimuValue tmp3 = deltaPos[j]*tmp2;
		for (int k=0; k < 3; k++) // loop on gama
		{
			SimuValue tmp4 = deltaPos[k]*tmp3;
			if (j == k)
				tmp4 += tmp1;
			SimuValue coeff = (pPrtl->m_fpMass + pNgbrPrtl->m_fpMass)*0.5
							  * (pPrtl->m_fpnnStrTen[i][k]*prtlDenReciprocal
								 + pNgbrPrtl->m_fpnnStrTen[i][k]*ngbrDenReciprocal);
			matrixFD[i*3+j] += coeff*tmp4;
		}
	}
}

void CPrtlFluid::ComputePrtlPressureGradients()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->ComputePrtlPressureGradient(m_pfSmoothLen);
	}
}

void CPrtlFluid::IntegratePrtlVelocitiesByPresGrad()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->AddVelocity(&pPrtl->m_fpPresGrad, m_pfTimeStep);
	}
}

void CPrtlFluid::IntegratePrtlVelocitiesByGravity()
{
	for (int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->AddVelocity(CSimuManager::GRAVITY, m_pfTimeStep);
	}
}

void CPrtlFluid::AdjustPrtlVelocitiesWithNeighbors()
{
	TClassArray<CVector3D> tmpVelocities(true, SIMU_PARA_PRTL_NUM_ALLOC_SIZE);
	tmpVelocities.ResetArraySize(m_pfPrtls.m_paNum);

	int i;
	SimuValue tmpDist, weight, totalWeight;
	CVector3D tmpVect;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpInCollision) continue;
		tmpVelocities[i]->SetValue((SimuValue)0);
		totalWeight = 0;
		for(int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtl* ngbrPrtl = pPrtl->m_fpNgbrs[j];
			tmpVect.SetValueAsSubstraction(ngbrPrtl->m_vpVel, pPrtl->m_vpVel);
			tmpDist = pPrtl->m_vpPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
			weight = CSimuUtility::SplineWeightFunction(tmpDist, m_pfSmoothLen);
			weight /= 0.5*(pPrtl->m_fpDensity + ngbrPrtl->m_fpDensity);
			weight *= 0.5*(pPrtl->m_fpMass + ngbrPrtl->m_fpMass);
			totalWeight += weight;
			tmpVelocities[i]->AddedBy(&tmpVect, weight);
		}
		if (fabs(totalWeight) > SIMU_VALUE_EPSILON)
			tmpVelocities[i]->ScaledBy(CSimuManager::m_velocityTuner/fabs(totalWeight));
		else
			tmpVelocities[i]->ScaledBy(CSimuManager::m_velocityTuner);
	}
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpInCollision) continue;
		pPrtl->AddVelocity(tmpVelocities[i]);
	}
}

void CPrtlFluid::IntegratePrtlPositions()
{
	int i;
	if (CSimuManager::ADAPTIVE_TIME_STEP)
	{
		SimuValue maxRelaVelMagnitude = -SIMU_VALUE_MAX;
		for (i=0; i < m_pfPrtls.m_paNum; i++)
		{
			SimuValue relaVelMagnitude = m_pfPrtls[i]->ComputeMaxRelativeVelMagnitude();
			if (maxRelaVelMagnitude < relaVelMagnitude)
				maxRelaVelMagnitude = relaVelMagnitude;
		}
		if (maxRelaVelMagnitude <= SIMU_VALUE_EPSILON)
			m_pfTimeStep = m_pfMaxTimeStep;
		else
		{
			const SimuValue maxRelativeMovement = CSimuManager::m_courantFactor*CSimuManager::m_prtlDistance;
			
			SimuValue curMaxRelaMovement = maxRelaVelMagnitude*m_pfMaxTimeStep;
			SimuValue timeStep = maxRelativeMovement/maxRelaVelMagnitude;
			int timeStepLevel = (int)floor(log10(timeStep)/SIMU_CONST_LOG_10_2);
			m_pfTimeStep = pow(2.0f, timeStepLevel);
			if (m_pfTimeStep > m_pfMaxTimeStep)
				m_pfTimeStep = m_pfMaxTimeStep;
		}
	}
	for (i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->m_vpPos->AddedBy(pPrtl->m_vpVel, m_pfTimeStep);
		pPrtl->m_fpInCollision = false;
//		CSimuUtility::AssertSimuVector3D(pPrtl->m_vpPos);
	}
	m_pfNeedToRegisterMovedPrtls = true;
}

void CPrtlFluid::IntegratePrtlStressTensorsByPlasticModel()
{
	m_pfMaxVortex = 0;
	m_pfMaxTensor = 0;
	SimuValue velGrad[3][3];
	SimuValue tensorIntegrationTimeStep = m_pfTimeStep/CSimuManager::m_numTensorSubIntegraions;
	SimuValue decayRate = 1/m_pfRelaxationTime;
	SimuValue prtlShearModulus;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		pPrtl->ComputeVelocityGradient(velGrad, m_pfSmoothLen);
		prtlShearModulus = GetShearModulusFromTemperature(pPrtl->m_fpTemperature);
		for (int k=0; k < CSimuManager::m_numTensorSubIntegraions; k++)
			pPrtl->IntegratePrtlStressTensorByPlasticModel(velGrad,
														tensorIntegrationTimeStep,
														prtlShearModulus, decayRate,
														m_pfRotationDerivative);
		// debug values
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
		{
			if (m_pfMaxVortex < fabs(velGrad[m][n]))
				m_pfMaxVortex = fabs(velGrad[m][n]);
			if (m_pfMaxTensor < fabs(pPrtl->m_fpnnStrTen[m][n]))
				m_pfMaxTensor = fabs(pPrtl->m_fpnnStrTen[m][n]);
		}
	}
	m_pfMaxAngTen = CSimuManager::m_maxAngVelTenElmnt;
}

void CPrtlFluid::IntegratePrtlStressTensorsByElasticModel()
{
	m_pfMaxVortex = 0;
	m_pfMaxTensor = 0;
	SimuValue velGrad[3][3];
	SimuValue tensorIntegrationTimeStep = m_pfTimeStep/CSimuManager::m_numTensorSubIntegraions;
	SimuValue decayRate = 1/m_pfRelaxationTime;
	SimuValue prtlShearModulus;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		pPrtl->ComputeVelocityGradient(velGrad, m_pfSmoothLen);
		prtlShearModulus = GetShearModulusFromTemperature(pPrtl->m_fpTemperature);
		for (int k=0; k < CSimuManager::m_numTensorSubIntegraions; k++)
			pPrtl->IntegratePrtlStressTensorBySPHModel(velGrad, tensorIntegrationTimeStep,
														prtlShearModulus, decayRate,
														m_pfRotationDerivative);
		// debug values
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
		{
			if (m_pfMaxVortex < fabs(velGrad[m][n]))
				m_pfMaxVortex = fabs(velGrad[m][n]);
			if (m_pfMaxTensor < fabs(pPrtl->m_fpnnStrTen[m][n]))
				m_pfMaxTensor = fabs(pPrtl->m_fpnnStrTen[m][n]);
		}
	}
	m_pfMaxAngTen = CSimuManager::m_maxAngVelTenElmnt;
}

void CPrtlFluid::IntegratePrtlStressTensorsByLinearModel()
{
	m_pfMaxVortex = 0;
	m_pfMaxTensor = 0;
	SimuValue velGrad[3][3];
	SimuValue prtlShearModulus;
	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		pPrtl->ComputeVelocityGradient(velGrad, m_pfSmoothLen);
		prtlShearModulus = GetShearModulusFromTemperature(pPrtl->m_fpTemperature);
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
		{
			pPrtl->m_fpnnStrTen[m][n] = 0.5*prtlShearModulus
										*(velGrad[m][n]+velGrad[n][m])
										*m_pfTimeStep;
			// debug values
			if (m_pfMaxVortex < fabs(velGrad[m][n]))
				m_pfMaxVortex = fabs(velGrad[m][n]);
			if (m_pfMaxTensor < fabs(pPrtl->m_fpnnStrTen[m][n]))
				m_pfMaxTensor = fabs(pPrtl->m_fpnnStrTen[m][n]);
		}
	}
}

SimuValue CPrtlFluid::GetShearModulusFromTemperature(SimuValue temperature)
{
	char location[] = "CPrtlFluid::GetShearModulusFromTemperature(temperature)";

	if (!CSimuManager::HEAT_TRANSFER)
		return m_pfShearModulus;

	switch (CSimuManager::m_eETptrCtrl)
	{
		case CSimuManager::LINEAR_MELTING_0:
			return GetShearModulusFromLinearMelting0(temperature, m_pfCoeffTemperature,
												m_pfCoeffShearModulus, m_pfConstSummation);
			break;
		default:
			CSimuMsg::ExitWithMessage(location, "No shear modulus for temperature.");
			break;
	}
	CSimuMsg::ExitWithMessage(location, "No shear modulus for temperature.");
	return GetShearModulusFromLinearMelting0(temperature, m_pfCoeffTemperature,
										m_pfCoeffShearModulus, m_pfConstSummation);
}

SimuValue CPrtlFluid::GetShearModulusFromLinearMelting0(SimuValue temperature,
														SimuValue coeffT,
														SimuValue coeffSM,
														SimuValue constant)
{
	SimuValue shearModulus = pow(10.0f, (float)(1.0f * (constant - coeffT * temperature)/coeffSM));
	if (shearModulus > m_pfMaxShearModulus)
	{
		return m_pfMaxShearModulus;
	}
	if (shearModulus < m_pfMinShearModulus)
	{
		return m_pfMinShearModulus;
	}
	return shearModulus;
}

//#define		SIMU_PARA_LINEAR_AVERAGE_STRESS_TENSOR

void CPrtlFluid::AdjustPrtlStressTensorsWithNgbrs()
{
	SimuValue** stressTensor;
	stressTensor = new SimuValue*[m_pfPrtls.m_paNum];

	int i;
	SimuValue tmpTunerRatio = 1 - CSimuManager::m_stressTensorTuner;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpInCollision) continue;
		stressTensor[i] = new SimuValue[9];
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
			stressTensor[i][m*3+n] = 0;
		SimuValue totalWeight = 0;
		for(int j=0; j < pPrtl->m_fpNgbrs.m_paNum; j++)
		{
			CFluidPrtlNonNew* ngbrPrtl = (CFluidPrtlNonNew*)pPrtl->m_fpNgbrs[j];
			SimuValue tmpDist = pPrtl->m_vpPos->GetDistanceToVector(ngbrPrtl->m_vpPos);
			SimuValue weight = CSimuUtility::SplineWeightFunction(tmpDist, m_pfSmoothLen);
			weight /= 0.5*(pPrtl->m_fpDensity + ngbrPrtl->m_fpDensity);
			weight *= 0.5*(pPrtl->m_fpMass + ngbrPrtl->m_fpMass);
			totalWeight += fabs(weight);
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
			{
#ifdef	SIMU_PARA_LINEAR_AVERAGE_STRESS_TENSOR
				stressTensor[i][m*3+n] += weight * ngbrPrtl->m_fpnnStrTen[m][n];
#else
				stressTensor[i][m*3+n] += weight *
									(ngbrPrtl->m_fpnnStrTen[m][n] - pPrtl->m_fpnnStrTen[m][n]);
#endif
			}
		}
		totalWeight = fabs(totalWeight);
		if (totalWeight > SIMU_VALUE_EPSILON)
		{
			for (int m=0; m < 3; m++)
			for (int n=0; n < 3; n++)
				stressTensor[i][m*3+n] /= totalWeight;
		}
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
		{
			stressTensor[i][m*3+n] *= CSimuManager::m_stressTensorTuner;
#ifdef	SIMU_PARA_LINEAR_AVERAGE_STRESS_TENSOR
			stressTensor[i][m*3+n] += pPrtl->m_fpnnStrTen[m][n]*tmpTunerRatio;
#else
			stressTensor[i][m*3+n] += pPrtl->m_fpnnStrTen[m][n];
#endif
		}
	}
	m_pfMaxTensor = 0;
	for(i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		if (pPrtl->m_fpInCollision) continue;
		for (int m=0; m < 3; m++)
		for (int n=0; n < 3; n++)
		{
			pPrtl->m_fpnnStrTen[m][n] = stressTensor[i][m*3+n];
			// debug values
			if (m_pfMaxTensor < fabs(pPrtl->m_fpnnStrTen[m][n]))
				m_pfMaxTensor = fabs(pPrtl->m_fpnnStrTen[m][n]);
		}
		delete stressTensor[i];
	}
	delete stressTensor;
}

void CPrtlFluid::IntegratePrtlVelocitiesByStressTensor()
{
	CVector3D tmpStress;

	for(int i=0; i < m_pfPrtls.m_paNum; i++)
	{
		CFluidPrtlNonNew* pPrtl = (CFluidPrtlNonNew*)m_pfPrtls[i];
		if (pPrtl->m_fpFixedMotion) continue;
		pPrtl->IntegratePrtlVelocityByStressTensor(tmpStress, m_pfTimeStep, m_pfSmoothLen);
	}
}

void CPrtlFluid::ComputePosOnIsoSrfc(SimuValue isoValue, SimuValue value0, SimuValue value1,
									 CVector3D* srfcPos, CVector3D* pos0, CVector3D* pos1)
{
	SimuValue u = (value0 - isoValue)/(value0 - value1);
	srfcPos->SetValue(pos0, 1 - u);
	srfcPos->AddedBy(pos1, u);
}

SimuValue CPrtlFluid::ComputeDensityAtPos(CVector3D &pos, SimuValue radius)
{
	SimuValue radiusSquare = radius*radius;
	SimuValue dist, weight;
	SimuValue density = 0;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		if (CSimuManager::NOT_COUNT_FIXED_PRTLS
		 && ngbrPrtl->m_fpFixedMotion)
			continue;
		dist = pos.GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		weight = CSimuUtility::SplineWeightFunction(dist, radius);
		density += weight*ngbrPrtl->m_fpMass;
	}
	return density;
}

SimuValue CPrtlFluid::ComputePrtlNumDensityAtPos(CVector3D &pos, SimuValue radius)
{
	SimuValue radiusSquare = radius*radius;
	SimuValue dist, density = 0;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		if (CSimuManager::NOT_COUNT_FIXED_PRTLS
		 && ngbrPrtl->m_fpFixedMotion)
			continue;
		dist = pos.GetDistanceSquareToVector(ngbrPrtl->m_vpPos);
		if (dist > radiusSquare) continue;
		dist = sqrt(dist);
		density += CSimuUtility::SplineWeightFunction(dist, radius);
	}
	return density;
}

SimuValue CPrtlFluid::ComputeIsoDensityAtPos(CVector3D &pos, SimuValue radius)
{
	if (CSimuManager::USE_PRTL_NUM_DENSITY)
	{
		SimuValue prtlNumDen = ComputePrtlNumDensityAtPos(pos, radius);
		return prtlNumDen/m_pfUniPrtlNumDensity - m_pfIsoDensityRatio;
	}
	else
		return ComputeDensityAtPos(pos, radius)/m_pfDensity - m_pfIsoDensityRatio;
}

SimuValue CPrtlFluid::ComputeGasStatePressure(SimuValue curDensity, SimuValue orgDensity)
{
	return CSimuManager::m_soundSpeed*(curDensity - orgDensity);
}

void CPrtlFluid::ComputeDensityGradient(CVector3D &pos, CVector3D &grad, SimuValue radius)
{
	grad.SetElements(0, 0, 0);
	SimuValue radiusSquare = radius*radius;
	SimuValue dist, weight;
	CVector3D vector;
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	for (int i=0; i < ngbrPrtls.m_paNum; i++)
	{
		CFluidPrtl* ngbrPrtl = ngbrPrtls[i];
		vector.SetValueAsSubstraction(&pos, ngbrPrtl->m_vpPos);
		dist = vector.LengthSquare();
		if (dist > radiusSquare) continue;
		if (dist <= SIMU_VALUE_EPSILON) continue;
		dist = sqrt(dist);
		vector.ScaledBy(1/dist);
		weight = CSimuUtility::SplineGradientWeightFunction(dist, radius);
		grad.AddedBy(&vector, ngbrPrtl->m_fpMass*weight);
	}
}

SimuValue CPrtlFluid::ComputeWeightedPrtlNumDenAndGrad(CVector3D &pos, CVector3D &grad,
													   SimuValue radius)
{
	TPointerArray<CFluidPrtl> ngbrPrtls(SIMU_PARA_NGBR_PRTL_ALLOC_SIZE);
	m_pfPrtlSearch.GetPossibleNgbrPrtls(&pos, radius, ngbrPrtls);
	return CFluidPrtl::ComputeWeightedPrtlNumDenAndGrad(pos, grad, ngbrPrtls, radius);
}

SimuValue CPrtlFluid::ComputeKineticEnergy(TPointerArray<CFluidPrtl> &aryPrtls)
{
	SimuValue totalKineticEnergy = 0;
	for (int i=0; i < aryPrtls.m_paNum; i++)
	{
		CFluidPrtl* pPrtl = aryPrtls[i];
		totalKineticEnergy += pPrtl->m_fpMass*pPrtl->m_vpVel->LengthSquare();
	}
	return 0.5*totalKineticEnergy;
}
