#ifndef CMESH_H
#define CMESH_H


#if 1 // option

#include <vcg/simplex/vertex/base.h>
#include <vcg/simplex/vertex/component_ocf.h>
#include <vcg/simplex/edge/base.h>
#include <vcg/simplex/face/base.h>
#include <vcg/simplex/face/component_ocf.h>
#include <vcg/simplex/face/component_rt.h>
#include <vcg/simplex/face/topology.h>

#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/update/topology.h>
#include <vcg/complex/trimesh/update/normal.h>
#include <vcg/complex/trimesh/update/flag.h>
#include <vcg/space/index/grid_static_ptr.h>

class CEdge;
class CFace;
class CVertex;

//Vert Mem Occupancy  --- 40b ---

class CVertex  : public vcg::VertexSimp2< CVertex, CEdge, CFace,
  vcg::vertex::InfoOcf,           /*  4b */
  vcg::vertex::Coord3f,           /* 12b */
  vcg::vertex::BitFlags,          /*  4b */
  vcg::vertex::Normal3f,          /* 12b */
  vcg::vertex::Qualityf,          /*  4b */
  vcg::vertex::Color4b,           /*  4b */
  vcg::vertex::VFAdjOcf,          /*  0b */
  vcg::vertex::MarkOcf,           /*  0b */
  vcg::vertex::TexCoordfOcf,      /*  0b */
  vcg::vertex::CurvaturefOcf,     /*  0b */
  vcg::vertex::CurvatureDirfOcf,  /*  0b */
  vcg::vertex::RadiusfOcf         /*  0b */
  >{
};


class CEdge : public vcg::EdgeSimp2<CVertex,CEdge,CFace, vcg::edge::EVAdj> {
public:
	inline CEdge(){};
  inline CEdge( CVertex * v0, CVertex * v1){ V(0)= v0 ; V(1)= v1;};
  static inline CEdge OrderedEdge(CVertex* v0,CVertex* v1){
   if(v0<v1) return CEdge(v0,v1);
   else return CEdge(v1,v0);
	}
};

//Face Mem Occupancy  --- 32b ---

class CFace    : public vcg::FaceSimp2<  CVertex, CEdge, CFace,
      vcg::face::InfoOcf,              /* 4b */
      vcg::face::VertexRef,            /*12b */
      vcg::face::BitFlags,             /* 4b */
      vcg::face::Normal3f,             /*12b */
      vcg::face::QualityfOcf,          /* 0b */
      vcg::face::MarkOcf,              /* 0b */
      vcg::face::Color4bOcf,           /* 0b */
      vcg::face::FFAdjOcf,             /* 0b */
      vcg::face::VFAdjOcf,             /* 0b */
//      vcg::face::WedgeTexCoordfOcf,     /* 0b */
	  vcg::face::EdgePlane
    > {};




class CMesh    : public vcg::tri::TriMesh< vcg::vertex::vector_ocf<CVertex>, vcg::face::vector_ocf<CFace> > {
public :
	int sfn; //The number of selected faces.
  vcg::Matrix44f Tr; // Usually it is the identity. It is applied in rendering and filters can or cannot use it. (most of the filter will ignore this)
  const vcg::Box3f &trBB() {
	static vcg::Box3f bb;
	bb.SetNull();
	bb.Add(Tr,bbox);
		return bb;
	}
};

typedef vcg::GridStaticPtr< CMesh::FaceType, CMesh::ScalarType> StaticPtrGrid;
#endif

#endif