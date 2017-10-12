
QT += xml
QT += opengl

TEMPLATE = app
CONFIG += qt console
TARGET = spatula

VCGDIR  = vcglib

GLEWDIR = external/glew-1.5.1
GLEWCODE = $$GLEWDIR/src/glew.c

DEFINES *= GLEW_STATIC
DEFINES += GLEW_STATIC

MOC_DIR	= tmp
UI_DIR = tmp
RCC_DIR = tmp

DEPENDPATH  += $$VCGDIR  $$VCGDIR/vcg $$VCGDIR/wrap
CONFIG += stl

macx {

QMAKE_CXXFLAGS+=-fpermissive -ftree-vectorize -frounding-math -DCGAL_CH_NO_POSTCONDITIONS
QMAKE_LFLAGS+=-lmetis -lf2c

INCLUDEPATH += /Library/Frameworks/Collada14Dom.framework/Headers \
 			   /Users/kuervo/Workspace/CGAL-3.5-beta1/include \
			   /sw/include/ \
			   include/ \ 
			   vcglib/ \
			   external/glew-1.5.1/include/
			
			
LIBS += -framework Collada14Dom \
					/Users/kuervo/Workspace/CGAL-3.5-beta1/lib/libCGAL.dylib \
					/sw/lib/libgmp.dylib \
					/sw/lib/libmpfr.dylib \
					-framework Accelerate \
					/sw/lib/libboost_thread.dylib \
					/usr/local/lib/libtaucs.a \
					/usr/local/lib/libf2c.a \
					/usr/local/lib/libmetis.a

					
ICON = resources/spatula.icns					

}

linux-g++ {
INCLUDEPATH += 	/usr/erskine7/cshome/orodrigu/local/include/collada-dom \
		/usr/erskine7/cshome/orodrigu/local/include/collada-dom/1.4 \
		/usr/erskine7/cshome/orodrigu/local/include \
		/usr/local/include/colladadom \
                /usr/local/include/colladadom/1.4 \
                include/ \
                vcglib/ \
                external/glew-1.5.1/include/


QMAKE_CFLAGS+=-fpermissive -frounding-math -fopenmp -ftree-vectorize  -DCGAL_CH_NO_POSTCONDITIONS -DGL_GLEXT_PROTOTYPES
QMAKE_CXXFLAGS+=-fpermissive -frounding-math -fopenmp -ftree-vectorize  -DCGAL_CH_NO_POSTCONDITIONS -DGL_GLEXT_PROTOTYPES

LIBS += -L/usr/local/lib -L/usr/lib/gcc/x86_64-linux-gnu/4.4/ -L/usr/erskine7/cshome/orodrigu/local/lib -lgomp -lgmp -lmpfr -lcollada14dom -lCGAL -lboost_thread-mt -lboost_filesystem -lboost_system-mt  -lminizip 
}


HEADERS +=	src/Spatula.h \
			src/ProcessingThread.h \
			src/ProcessedScene.h \
			src/ProcessedNode.h \
			src/integrationHelper.h \
			src/SurfaceMesher.h \
			src/CGALDefinitions.h \
			src/ColladaReconstructor.h \
			src/Task.h \
			src/Particle.h \
			src/ovKDtree.h \
			src/SimuMarchingCube.h \
			src/SimuMarchingCubeLookupTables.h \
			src/Polyhedronification/AlphashapeHDSPolyhedronifikator.h \
			src/Polyhedronification/AlphashapePolyhedronifikator.h \
			src/Polyhedronification/AlphashapeHDSator.h \
			src/ExporterXYZ.h \
			src/meshlab/filterparameter.h \
			src/meshlab/meshmodel.h \
			src/meshlab/interfaces.h \
			src/CMesh.h \
			src/rayTriangleIntersect.h
			
			
SOURCES +=	src/Spatula.cpp \
			src/main.cpp \
			src/ProcessingThread.cpp \
			src/ProcessedScene.cpp \
			src/ProcessedNode.cpp \
			src/SurfaceMesher.cpp \
			src/ColladaReconstructor.cpp \
			src/Particle.cpp \
			src/ovKDtree.cpp \
			src/SimuMarchingCube.cpp \
			src/ExporterXYZ.cpp \
			src/meshlab/filterparameter.cpp \
			src/meshlab/meshmodel.cpp \
			src/CGALDefinitions.cpp \
			src/rayTriangleIntersect.cpp \
			$$GLEWCODE
		
