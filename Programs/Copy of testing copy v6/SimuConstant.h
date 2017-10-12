
#if !defined(AFX_SIMUCONSTANT_H__INCLUDED_)
#define AFX_SIMUCONSTANT_H__INCLUDED_

#include "float.h"

#define _CRT_SECURE_NO_DEPRECATE 1

//#define		SIMU_PARA_DEBUG_MARCHING_CUBE_GRID_STRUCTURE
#define		SIMU_PARA_SHADING_SMOOTH
#define		SIMU_PARA_DISABLE_MONITOR_MESSAGE
#define		SIMU_PARA_ENABLE_PARTICLE_COLOR
#define		SIMU_PARA_ENABLE_POINT_3D_COLOR

//#define		ASSERT_VECTOR
//#define		DEBUG_VECTOR_VALUE

#define DOUBLE_VALUE
#ifdef	DOUBLE_VALUE
	typedef double	SimuValue;
	#define	SIMU_VALUE_EPSILON		pow(10.0f, -DBL_DIG+1)
	#define	SIMU_VALUE_MAX			DBL_MAX
	#define	SimuColor3d		glColor3d
	#define	SimuColor3v		glColor3dv
	#define	SimuVertex3d	glVertex3d
	#define	SimuVertex3v	glVertex3dv
	#define	SimuNormal3d	glNormal3d
	#define	SimuNormal3v	glNormal3dv
	#define	SimuScale		glScaled
	#define	SimuRotate		glRotated
	#define	SimuTranslate	glTranslated
	#define	SimuglGetv		glGetDoublev
	#define PI				3.14159265358979323846
	#define SIMU_PARA_TRIANGLE_EDGE_THICKNESS		pow(10.0f, -6.0f)
	#define SIMU_PARA_MIN_TETRAHEDRAL_VOLUME		pow(10.0f, -6.0f)
	#define	SIMU_PARA_MIN_PRTL_DISTANCE_RATIO		1e-006  // ratio with respect to particle distance
#else	// float
	typedef float	SimuValue;
	#define	SIMU_VALUE_EPSILON		pow(10.0f, -FLT_DIG+1)
	#define	SIMU_VALUE_MAX			FLT_MAX
	#define	SimuColor3d		glColor3f
	#define	SimuColor3v		glColor3fv
	#define	SimuVertex3d	glVertex3f
	#define	SimuVertex3v	glVertex3fv
	#define	SimuNormal3d	glNormal3f
	#define	SimuNormal3v	glNormal3fv
	#define	SimuScale		glScalef
	#define	SimuRotate		glRotatef
	#define	SimuTranslate	glTranslatef
	#define	SimuglGetv		glGetFloatv
	#define PI				3.1415926535
//	#define PI				3.14159265358979323846
	#define SIMU_PARA_TRIANGLE_EDGE_THICKNESS		pow(10.0f, -3)
	#define SIMU_PARA_MIN_TETRAHEDRAL_VOLUME		pow(10.0f, -4)
	#define	SIMU_PARA_MIN_PRTL_DISTANCE_RATIO		1e-003  // ratio with respect to particle distance
#endif

#define	SIMU_PARA_PARTICLE_TRIANGLE_MIN_GAP			1e-003
#define	SIMU_PARA_AREA_ERROR_FOR_IN_TRIANGLE_TEST	1e-005

#define SIMU_VALUE_2PI		PI*2

////////////////////////////////////////////////////////////////////////////////////
// constant values
////////////////////////////////////////////////////////////////////////////////////
//#define PI						3.1415926535
//8979323846 2643383279 5028841971
//6939937510 1415926535 8979323846 
//2643383279 5028841971 6939937510
//5820974944 5923078164 0628620899 8628034825 3421170679
#define GRAVITY_ACCELERATION	"0.0, -100.0, 0.0"


#define	SIMU_PARA_INITIAL_DIAGONAL_STRESS	0
#define	SIMU_PARA_TET_SHAPE_THRESHOLD		0.2

#define SIMU_PARA_MEM_ALLOC_SIZE			65536
#define	SIMU_PARA_PRTL_NUM_ALLOC_SIZE		1024
#define	SIMU_PARA_TETRA_NUM_ALLOC_SIZE		1024
#define	SIMU_PARA_TRI_NUM_ALLOC_SIZE		1024
#define	SIMU_PARA_EDGE_NUM_ALLOC_SIZE		1024
#define	SIMU_PARA_NGBR_PRTL_ALLOC_SIZE		32
#define	SIMU_PARA_NGBR_TETRA_ALLOC_SIZE		32
#define	SIMU_PARA_NGBR_TRI_ALLOC_SIZE		32
#define	SIMU_PARA_NGBR_EDGE_ALLOC_SIZE		32
#define	SIMU_PARA_PRTL_IN_CELL_ALLOC_SIZE	32
#define	SIMU_PARA_TRI_IN_CELL_ALLOC_SIZE	32
#define	SIMU_PARA_TETRA_IN_CELL_ALLOC_SIZE	32
#define	SIMU_PARA_NGBR_ARRAY_ALLOC_SIZE		16
#define	SIMU_PARA_INSIDE_CUBE_ALLOC_SIZE	16
#define	SIMU_PARA_ARRAY_ALLOC_SIZE			256
#define	SIMU_PARA_DEFAULT_ARRAY_ALLOC_SIZE	64
#define	SIMU_PARA_BOUNDARY_NUM_ALLOC_SIZE	8

#define	SIMU_DEBUG_DATA_PRECISION	10
#define	SIMU_NEGATIVE_NUMBER		-1
#define	SIMU_POSITIVE_NUMBER		1
#define	SIMU_CLEAN_TAG				0
#define	SIMU_DIRTY_TAG				1

#define LEN_DIR_NAME			200
#define LEN_FILE_NAME			200
#define LEN_FLUID_NAME			100
#define LEN_BOUNDARY_NAME		100
#define LEN_VECTOR_NAME			100
#define LEN_MESSAGE				1500
#define LEN_DATA_LINE			2000
#define LEN_VECTOR_SCALAR		100
#define LEN_FLUID_PARTICLE_ID	10

#define SCALAR_DIMENSION		1
#define VECTOR_DIMENSION		3
#define	DIMENSION_3				3
#define	DIMENSION_2				2

#define	APP_RELATIVE_DATA_DIR		"data"
#define	APP_OUTPUT_INFO_FILE_NAME	"output_info.txt"	// for simulation info output
#define	APP_FLUID_PARA_FILE_NAME	"fluid_para.txt"	// for fluid import/export
#define	APP_PARAMETER_FILE_NAME		"SimuApp.para"

#define	START_OF_GLOBAL_PARAS	"[start of global parameters]"
#define	END_OF_GLOBAL_PARAS		"[end of global parameters]"
#define	RETURN_STRING			"\r\n"
#define	DOUBLE_RETURN_STRING	"\r\n\r\n"
#define SIMU_TIMER_ID			1
#define SIMU_TYPE_NO_NAME		"no name"
#define COLOR_WHITE_STR			"1.0, 1.0, 1.0"		// white
#define COLOR_RED_STR			"1.0, 0.0, 0.0"		// red
#define COLOR_BLUE_STR			"0.0, 0.0, 1.0"		// blue
#define COLOR_GREEN_STR			"0.0, 1.0, 0.0"		// green
#define COLOR_YELLOW_STR		"1.0, 1.0, 0.0"		// yellow
#define COLOR_MAGENTA_STR		"1.0, 0.0, 1.0"		// magenta
#define COLOR_CYAN_STR			"0.0, 1.0, 1.0"		// green, blue
#define COLOR_ORANGE_STR		"1.0, 0.5, 0.0"		// green, blue

#define MAX_FUNCTIONS			10

// assume regular tet with side length = 1
const SimuValue SIMU_CONST_REGULAR_TET_SIDE_LEN = 1;
const SimuValue SIMU_CONST_REGULAR_TET_HEIGHT = sqrt(6.0f)/3.0f;
const SimuValue SIMU_CONST_REGULAR_TET_CIRCUMRADIUS = sqrt(6.0f)/4.0;
const SimuValue SIMU_CONST_REGULAR_TET_INRADIUS = sqrt(6.0f)/12.0f;
const SimuValue SIMU_CONST_TET_SHAPE_MEASURE_NORMALIZER = SIMU_CONST_REGULAR_TET_INRADIUS;
const SimuValue SIMU_CONST_TET_SHAPE_RATIO_NORMALIZER = 6*sqrt(6.0f);

const SimuValue SIMU_CONST_DEGREE_RADIAN_RATIO = PI/180.0;
const SimuValue SIMU_CONST_SPIKY_NORMALIZER = 15.0/PI;
const SimuValue SIMU_CONST_PRESSURE_LAPLACIAN = 45/PI;
const SimuValue SIMU_CONST_2_OVER_3 = 0.66666666666;
const SimuValue SIMU_CONST_4_OVER_3 = 1.33333333333;
const SimuValue SIMU_CONST_ONE_THIRD = 0.33333333333;
const SimuValue SIMU_CONST_SQUARE_ROOT_OF_2 = sqrt(2.0f);
const SimuValue SIMU_CONST_LOG_10_2 = log10(2.0f);

// coordinates enumeration for 3D space vector
enum
{
	X = 0,
	Y = 1,
	Z = 2,
	R = 0,
	G = 1,
	B = 2,
	BOUNDARY_LEFT = 0,
	BOUNDARY_RIGHT,
	BOUNDARY_BOTTOM,
	BOUNDARY_TOP,
	BOUNDARY_NEAR,
	BOUNDARY_FAR,
	BOUNDARY_DIM
};

enum ERotation
{
	CONVECTED,
	COROTATIONAL,
	NO_ROTATION
};

template <class Obj_T>
void ExtendPointerBuffer(Obj_T** &buffer, int &maxNum, int extendSize)
{
	Obj_T** temp = buffer;
	buffer = new Obj_T*[maxNum + extendSize];
	if (maxNum > 0)
	{
		memcpy(buffer, temp, maxNum*sizeof(Obj_T*)) ;
		delete temp;
	}
	maxNum += extendSize;
}

template <class Obj_T>
bool CheckPointerBufferSize(Obj_T** &buffer,
							int &curNum, int &maxNum, int extendSize)
{
	if (curNum < maxNum)
	{
	}
	else if (curNum == maxNum)
	{
		ExtendPointerBuffer(buffer, maxNum, extendSize);
	}
	else // (curNum > maxNum)
		return false;
	return true;
}

#endif // #if !defined(AFX_SIMUCONSTANT_H__INCLUDED_)