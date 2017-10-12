# Microsoft Developer Studio Project File - Name="SimuFlexApp" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=SimuFlexApp - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "SimuFlexApp.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "SimuFlexApp.mak" CFG="SimuFlexApp - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "SimuFlexApp - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "SimuFlexApp - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName "SimuFlexApp"
# PROP Scc_LocalPath "..\testing copy v6"
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "SimuFlexApp - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /incremental:yes /machine:I386

!ELSEIF  "$(CFG)" == "SimuFlexApp - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /w /W0 /Gm /GR /GX /ZI /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib glaux.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "SimuFlexApp - Win32 Release"
# Name "SimuFlexApp - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BdryBouncingSlope.cpp
# End Source File
# Begin Source File

SOURCE=.\BdryBowl.cpp
# End Source File
# Begin Source File

SOURCE=.\BdryHollowCone.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySolidBox.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySolidOval.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySolidRod.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySolidRodSticky.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySolidTorus.cpp
# End Source File
# Begin Source File

SOURCE=.\BdrySquareSlope.cpp
# End Source File
# Begin Source File

SOURCE=.\Boundary.cpp
# End Source File
# Begin Source File

SOURCE=.\BoxOnXYPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\ConeAlongZAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\ConvexHull.cpp
# End Source File
# Begin Source File

SOURCE=.\CylinderAlongZAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\DataExImport.cpp
# End Source File
# Begin Source File

SOURCE=.\DelaunayTetrahedron.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFileMoveCopy.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionElementSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionImExport.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionMesher.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionMeshTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionModeling.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionParticleSelect.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionSurface.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgOptionViewing.cpp
# End Source File
# Begin Source File

SOURCE=.\Edge3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Edge3DOnTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidMeshPrtl.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidNozzleShuttle.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidPrtl.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlNonNew.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlPoisson.cpp
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlSpring.cpp
# End Source File
# Begin Source File

SOURCE=.\GeometryElement.cpp
# End Source File
# Begin Source File

SOURCE=.\Grid.cpp
# End Source File
# Begin Source File

SOURCE=.\GridDlnyTetra.cpp
# End Source File
# Begin Source File

SOURCE=.\Index3D.cpp
# End Source File
# Begin Source File

SOURCE=.\LinearTemperaTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshPrtl.cpp
# End Source File
# Begin Source File

SOURCE=.\OvalAlong3DAxes.cpp
# End Source File
# Begin Source File

SOURCE=.\ParticleSearch.cpp
# End Source File
# Begin Source File

SOURCE=.\Point3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Point3DIntPos.cpp
# End Source File
# Begin Source File

SOURCE=.\Point3DOnEdge.cpp
# End Source File
# Begin Source File

SOURCE=.\Point3DOnTetra.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlFluid.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlFluidSpring.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlFluidUpsample.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlInBox.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlInOval.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlInSrfcMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\PrtlTriangleMesh.cpp
# End Source File
# Begin Source File

SOURCE=.\RigidBall.cpp
# End Source File
# Begin Source File

SOURCE=.\RotationToZAxis.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryBox.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryCylinder.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryTable.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuCGSolver.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuCGSolver3D.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuDriver.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuEnclosingBall.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuFlexApp.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuFlexApp.rc
# End Source File
# Begin Source File

SOURCE=.\SimuFluidNozzle.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuManager.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuMarchingCube.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuMCTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuMessageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuModeler.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuMonitor.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuMsg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuPoint3D.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuQuestionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuSrfcPoint3D.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuTriangle.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuUtility.cpp
# End Source File
# Begin Source File

SOURCE=.\SimuView.cpp
# End Source File
# Begin Source File

SOURCE=.\Spring.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\TemperatureToTexture.cpp
# End Source File
# Begin Source File

SOURCE=.\TetraDelaunay.cpp
# End Source File
# Begin Source File

SOURCE=.\Tetrahedron.cpp
# End Source File
# Begin Source File

SOURCE=.\TorusOnXYPlane.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangle3D.cpp
# End Source File
# Begin Source File

SOURCE=.\Triangle3DIntNorm.cpp
# End Source File
# Begin Source File

SOURCE=.\TriangleOnTetra.cpp
# End Source File
# Begin Source File

SOURCE=.\TSimuPtrStack.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector.cpp
# End Source File
# Begin Source File

SOURCE=.\Vector3D.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualPrtl.cpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\SimuFlexApp.ico
# End Source File
# Begin Source File

SOURCE=.\res\SimuFlexApp.rc2
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BdryBouncingSlope.h
# End Source File
# Begin Source File

SOURCE=.\BdryBowl.h
# End Source File
# Begin Source File

SOURCE=.\BdryHollowCone.h
# End Source File
# Begin Source File

SOURCE=.\BdrySolidBox.h
# End Source File
# Begin Source File

SOURCE=.\BdrySolidOval.h
# End Source File
# Begin Source File

SOURCE=.\BdrySolidRod.h
# End Source File
# Begin Source File

SOURCE=.\BdrySolidRodSticky.h
# End Source File
# Begin Source File

SOURCE=.\BdrySolidTorus.h
# End Source File
# Begin Source File

SOURCE=.\BdrySquareSlope.h
# End Source File
# Begin Source File

SOURCE=.\Boundary.h
# End Source File
# Begin Source File

SOURCE=.\BoxOnXYPlane.h
# End Source File
# Begin Source File

SOURCE=.\ConeAlongZAxis.h
# End Source File
# Begin Source File

SOURCE=.\ConvexHull.h
# End Source File
# Begin Source File

SOURCE=.\CylinderAlongZAxis.h
# End Source File
# Begin Source File

SOURCE=.\DataExImport.h
# End Source File
# Begin Source File

SOURCE=.\DelaunayTetrahedron.h
# End Source File
# Begin Source File

SOURCE=.\DlgFileMoveCopy.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionElementSelect.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionImExport.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionMesher.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionMeshTexture.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionModeling.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionParticleSelect.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionSurface.h
# End Source File
# Begin Source File

SOURCE=.\DlgOptionViewing.h
# End Source File
# Begin Source File

SOURCE=.\Edge3D.h
# End Source File
# Begin Source File

SOURCE=.\Edge3DOnTriangle.h
# End Source File
# Begin Source File

SOURCE=.\FluidMeshPrtl.h
# End Source File
# Begin Source File

SOURCE=.\FluidNozzleShuttle.h
# End Source File
# Begin Source File

SOURCE=.\FluidPrtl.h
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlNonNew.h
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlPoisson.h
# End Source File
# Begin Source File

SOURCE=.\FluidPrtlSpring.h
# End Source File
# Begin Source File

SOURCE=.\GeometryElement.h
# End Source File
# Begin Source File

SOURCE=.\Grid.h
# End Source File
# Begin Source File

SOURCE=.\GridDlnyTetra.h
# End Source File
# Begin Source File

SOURCE=.\Index3D.h
# End Source File
# Begin Source File

SOURCE=.\LinearTemperaTexture.h
# End Source File
# Begin Source File

SOURCE=.\MeshPrtl.h
# End Source File
# Begin Source File

SOURCE=.\OvalAlong3DAxes.h
# End Source File
# Begin Source File

SOURCE=.\ParticleSearch.h
# End Source File
# Begin Source File

SOURCE=.\Point3D.h
# End Source File
# Begin Source File

SOURCE=.\Point3DIntPos.h
# End Source File
# Begin Source File

SOURCE=.\Point3DOnEdge.h
# End Source File
# Begin Source File

SOURCE=.\Point3DOnTetra.h
# End Source File
# Begin Source File

SOURCE=.\PrtlFluid.h
# End Source File
# Begin Source File

SOURCE=.\PrtlFluidSpring.h
# End Source File
# Begin Source File

SOURCE=.\PrtlFluidUpsample.h
# End Source File
# Begin Source File

SOURCE=.\PrtlInBox.h
# End Source File
# Begin Source File

SOURCE=.\PrtlInOval.h
# End Source File
# Begin Source File

SOURCE=.\PrtlInSrfcMesh.h
# End Source File
# Begin Source File

SOURCE=.\PrtlTriangle.h
# End Source File
# Begin Source File

SOURCE=.\PrtlTriangleMesh.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\RigidBall.h
# End Source File
# Begin Source File

SOURCE=.\RotationToZAxis.h
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryBox.h
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryCylinder.h
# End Source File
# Begin Source File

SOURCE=.\SimuBoundaryTable.h
# End Source File
# Begin Source File

SOURCE=.\SimuCGSolver.h
# End Source File
# Begin Source File

SOURCE=.\SimuCGSolver3D.h
# End Source File
# Begin Source File

SOURCE=.\SimuConstant.h
# End Source File
# Begin Source File

SOURCE=.\SimuDriver.h
# End Source File
# Begin Source File

SOURCE=.\SimuEnclosingBall.h
# End Source File
# Begin Source File

SOURCE=.\SimuFlexApp.h
# End Source File
# Begin Source File

SOURCE=.\SimuFluidNozzle.h
# End Source File
# Begin Source File

SOURCE=.\SimuManager.h
# End Source File
# Begin Source File

SOURCE=.\SimuMarchingCube.h
# End Source File
# Begin Source File

SOURCE=.\SimuMarchingCubeLookupTables.h
# End Source File
# Begin Source File

SOURCE=.\SimuMCTexture.h
# End Source File
# Begin Source File

SOURCE=.\SimuMessageDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimuModeler.h
# End Source File
# Begin Source File

SOURCE=.\SimuMonitor.h
# End Source File
# Begin Source File

SOURCE=.\SimuMsg.h
# End Source File
# Begin Source File

SOURCE=.\SimuPoint3D.h
# End Source File
# Begin Source File

SOURCE=.\SimuQuestionDlg.h
# End Source File
# Begin Source File

SOURCE=.\SimuSrfcPoint3D.h
# End Source File
# Begin Source File

SOURCE=.\SimuTriangle.h
# End Source File
# Begin Source File

SOURCE=.\SimuUtility.h
# End Source File
# Begin Source File

SOURCE=.\SimuView.h
# End Source File
# Begin Source File

SOURCE=.\Spring.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TClassArray.h
# End Source File
# Begin Source File

SOURCE=.\TemperatureToTexture.h
# End Source File
# Begin Source File

SOURCE=.\TetraDelaunay.h
# End Source File
# Begin Source File

SOURCE=.\Tetrahedron.h
# End Source File
# Begin Source File

SOURCE=.\TGeomElemArray.h
# End Source File
# Begin Source File

SOURCE=.\TorusOnXYPlane.h
# End Source File
# Begin Source File

SOURCE=.\TPointerArray.h
# End Source File
# Begin Source File

SOURCE=.\TPointerStack.h
# End Source File
# Begin Source File

SOURCE=.\TPrtlGrid.h
# End Source File
# Begin Source File

SOURCE=.\Triangle3D.h
# End Source File
# Begin Source File

SOURCE=.\Triangle3DIntNorm.h
# End Source File
# Begin Source File

SOURCE=.\TriangleOnTetra.h
# End Source File
# Begin Source File

SOURCE=.\TSimuPtrStack.h
# End Source File
# Begin Source File

SOURCE=.\Vector.h
# End Source File
# Begin Source File

SOURCE=.\Vector3D.h
# End Source File
# Begin Source File

SOURCE=.\VirtualPrtl.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ApplicationMemo.txt
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
