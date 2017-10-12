/*
 *  SurfaceMesher.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 14/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SURFACEMESHER_H
#define SURFACEMESHER_H

#include <stdio.h>
#include <iostream>
#include <vcg/space/point3.h>
#include <vcg/space/box3.h>
#include <vcg/simplex/vertex/base.h>
#include <vcg/simplex/face/base.h>
#include <vcg/complex/trimesh/base.h>
#include <vcg/complex/trimesh/allocate.h>
#include <vcg/complex/trimesh/append.h>
#include <vcg/complex/trimesh/create/marching_cubes.h>
#include <vcg/complex/trimesh/create/extended_marching_cubes.h>
#include <vcg/complex/trimesh/create/mc_trivial_walker.h>
#include <vcg/space/box3.h>

using namespace std;
using namespace vcg;

typedef float ScalarType;
typedef SimpleVolume< SimpleVoxel > MyVolume;
typedef Box3<ScalarType> MyBox;

#include <QObject>
#include <QHash>

#include <map>
#include <list>




#include "ProcessedScene.h"
#include "ProcessedNode.h"
#include "Task.h"
#include "CGALDefinitions.h"
#include "meshlab/meshmodel.h"
#include "meshlab/interfaces.h"
#include "SimuMarchingCube.h"




class SurfaceMesher : public Task 
	{
		
		Q_OBJECT
		
	public:
		
	
		
		// copied from the meshlab mls plugin
		
		
		enum mlsNames {
			_RIMLS_       = 0x1,
			_APSS_        = 0x2,
			_PROJECTION_  = 0x1000,
			_AFRONT_      = 0x2000,
			_MCUBE_       = 0x4000,
			_COLORIZE_    = 0x8000,
			
			FP_RIMLS_PROJECTION = _RIMLS_ | _PROJECTION_,
			FP_APSS_PROJECTION	= _APSS_  | _PROJECTION_,
			
			FP_RIMLS_AFRONT = _RIMLS_ | _AFRONT_,
			FP_APSS_AFRONT  = _APSS_  | _AFRONT_,
			
			FP_RIMLS_MCUBE = _RIMLS_ | _MCUBE_,
			FP_APSS_MCUBE  = _APSS_  | _MCUBE_,
			
			FP_RIMLS_COLORIZE = _RIMLS_ | _COLORIZE_,
			FP_APSS_COLORIZE  = _APSS_  | _COLORIZE_,
			
			FP_RADIUS_FROM_DENSITY       = 0x10000,
			FP_SELECT_SMALL_COMPONENTS   = 0x20000
		};
		
		enum poissonNames{ 
			FP_POISSON_RECON } ;
		
			/* naming convention :
				 - FP -> Filter Plugin
				 - name of the plugin separated by _
			*/
		enum meshNames {  
				FP_LOOP_SS,
	          FP_BUTTERFLY_SS,
	          FP_REMOVE_UNREFERENCED_VERTEX,
					  FP_REMOVE_DUPLICATED_VERTEX,
	          FP_REMOVE_FACES_BY_AREA,
					  FP_REMOVE_FACES_BY_EDGE,
						FP_REMOVE_NON_MANIFOLD_FACE,
						FP_REMOVE_NON_MANIFOLD_VERTEX,
						FP_CLUSTERING,
	          FP_QUADRIC_SIMPLIFICATION,
	          FP_QUADRIC_TEXCOORD_SIMPLIFICATION,
	          FP_NORMAL_EXTRAPOLATION,
			  FP_COMPUTE_PRINC_CURV_DIR,
	          FP_MIDPOINT,
	          FP_REORIENT ,
	          FP_INVERT_FACES,
					  FP_TRANSFORM,
					  FP_FREEZE_TRANSFORM,
	          FP_CLOSE_HOLES_TRIVIAL,
	          FP_CLOSE_HOLES,
						FP_CYLINDER_UNWRAP,
	          } ;
	

		
		SurfaceMesher();
		~SurfaceMesher();
		
		void resetAlphaValues();
		void getMesh(ProcessedScene &scene, MeshDocument & meshDocument);
		static QString getPointKey(Point_3 point);
		static QString getPointKey(double x, double y, double z);
		
	private:
		
		bool __alphaSet;
		double __alpha;
		bool __cellSizeSet;
		double __cellSize;
		
		RichParameterSet filterParameters;
		RichParameterSet meshFilterParameters;
		RichParameterSet loopFilterParameters;
		
		Polyhedron getBooleanDifference(Polyhedron &A, Polyhedron &B);
		
		uint getEdgeIndex(std::vector< Halfedge * > &edgesList, Halfedge *edge);
		QString getEdgeKey(Halfedge &edge);
		//QString getPointKey(double, double, double);
		
		MeshFilterInterface *surfaceFilter;
		MeshFilterInterface *meshFilter;
		MeshFilterInterface *loopFilter;
		
		//TPrtlGrid<CFluidPrtl> m_pfMCGrid;
		CSimuMarchingCube m_pfMarchingCube;
		
		bool IsPointOnTriangle(Point_2 p, 
							   Point_2 a, 
							   Point_2 b, 
							   Point_2 c);
		bool IsPosOnSrfcByItsCGALConvexHull(Point_3 & target,
											std::vector<Point_3> &points,
											Vector_3 & normal);
		
		static bool QCallBack(const int pos, const char * str);
		
		
		void updateNormal(CVertexO * v);
		void smoothGrid(int smoothingGridSize, 
						int xGridD, 
		      		    int yGridD, 
						int zGridD,
						bool countZero,
						MyVolume &volume);
		bool isInside(Point_3 p, CutOutInformation cutOut);
		QString getKey(double a, double b, double c);
		
		//bool isInside(CMeshO *, StaticPtrGrid &, CMesh::CoordType &);
		
		/*
		void prepareGTSSurface(GtsSurface * s1);
		void prepareGTSNSurface(GtsSurface * s1);
		GtsSurface * createGTSNSurface(Polyhedron &P);
		*/
		/*
 		void createBubbleStructure(Nef_polyhedron & bubbleStructure, 
								   ProcessedNode* node);
		void createSingleBubblePolyhedron( ExactPolyhedron & singleBubble);
		 */
		
	};

#if 0

template <class HDS>
class BuildPolyhedronFromCMeshO : public CGAL::Modifier_base<HDS> 
{
public:
	BuildPolyhedronFromCMeshO() {}
	
	void operator()(HDS& hds){
	
		CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);	
		B.begin_surface( __cm->vn, __cm->fn);
		CMeshO::FaceIterator fIt;
		std::vector<int> toAdd;
		int currentPointIndex = 0;
		QHash<QString, int> addedPoints;
		
		for (fIt = __cm->face.begin(); fIt != __cm->face.end(); ++fIt) {
			
			// add vertices
			
			toAdd.clear();
			
			for (int k = 0; k<3; k++) {
				
				Point_3 currentPoint(fIt->cP(k)[0],
									 fIt->cP(k)[1],
									 fIt->cP(k)[2]);
				QString key = SurfaceMesher::getPointKey(currentPoint);
				
				// add vertex to polyhedron only once
				
				if (addedPoints.contains( key ) ) {
					
					toAdd.push_back( addedPoints[ key ] );
				} else {
					B.add_vertex( currentPoint );
					addedPoints[ key ] = currentPointIndex;
					toAdd.push_back(currentPointIndex);
					currentPointIndex++;
				}
				
			}
			
			
			// add facet
			if ( B.test_facet( toAdd.begin(), toAdd.end() ) ) {
				B.begin_facet();
				for (int i=0; i < 3; i++) {
					B.add_vertex_to_facet(toAdd[i]);
				}
				B.end_facet();
			}
			
			else {
				std::cout << " could not add facet \n";
			}
			
		}
		
		B.end_surface();
		
	};
	
	void addCMeshO(CMeshO *cm) { __cm = cm; };

private:
	
	CMeshO * __cm;
	
};
#endif
#if 0

template <class HDS>
class BuildPolyhedronFromAShape : public CGAL::Modifier_base<HDS> {
public:
    BuildPolyhedronFromAShape() {}

	
    void operator()(HDS& hds){
		
		std::list<FacetA> facets;
		std::list<FacetA>::iterator facetIterator;
		std::list<Vertex_handleA> vertices;
		QHash<QString, int> addedPoints;
		std::vector<int> toAdd;
		Cell_handleA cellHandler;
		int index;
		
		__as->get_alpha_shape_facets(std::back_inserter(facets), Alpha_shape_3::REGULAR);
		__as->get_alpha_shape_vertices(std::back_inserter(vertices), Alpha_shape_3::REGULAR);
		
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		
		B.begin_surface( vertices.size(), facets.size()); 		
		
		int addedVertices = 0;
		
		for (facetIterator = facets.begin(); facetIterator != facets.end(); ++facetIterator) {


			cellHandler = facetIterator->first;
			index = facetIterator->second;
			
			// the following ensures that regular facets are output
			// in ccw order
			
			
			if ((__as->classify(*facetIterator) == Alpha_shape_3::REGULAR) &&
				(__as->classify(cellHandler) == Alpha_shape_3::INTERIOR)) {
				cellHandler = cellHandler->neighbor(index);
				index = cellHandler->index(facetIterator->first);
			}
			
			
			int current;
			toAdd.clear();
			
		
			
			for (int k = 0; k<3; k++) {
				current = Alpha_shape_3::Triangulation_utils_3::vertex_triple_index(index, k);
				
				QString key = SurfaceMesher::getPointKey( cellHandler->vertex(current)->point() );
				
				// add vertex to polyhedron only once
				
				if (addedPoints.contains( key ) ) {
					
					toAdd.push_back( addedPoints[ key ] );
				}
				else {
					B.add_vertex( cellHandler->vertex(current)->point() );
					addedPoints[ key ] = addedVertices;
					toAdd.push_back(addedVertices);
					addedVertices++;
				}
				
			}
			
			if (toAdd[0] != toAdd[1] &&
				toAdd[0] != toAdd[2] &&
				toAdd[1] != toAdd[2] ) {
			
			
				if ( B.test_facet( toAdd.begin(), toAdd.end() ) ) {
					B.begin_facet();
					for (int i=0; i < 3; i++) {
						B.add_vertex_to_facet(toAdd[i]);
					}
					B.end_facet();
				}
				
				else {
					std::cout << " could not add facet \n";
				}
			
			}
		}
		
		
		B.end_surface();
		
    }
	
	
	
	void addAShape(Alpha_shape_3 *as_){ __as = as_; };
	
	
private:
	Alpha_shape_3 *__as;
	
	
	
};
#endif

/*
extern "C"  {
	static void pick_first_face (GtsFace * f, GtsFace ** first)
	{
		if (*first == NULL)
			*first = f;
	}
}
*/


/*
template <class HDS>
class BuildPolyhedronFromGTSSurface : public CGAL::Modifier_base<HDS> {
public:
    BuildPolyhedronFromGTSSurface() {}
	
	
	
	
	
    void operator()(HDS& hds){
		
		QHash<QString, int> addedPoints;
		std::vector<int> toAdd;
 
		int index;
		
		//get facets and vertices
		 
		uint vertexCount = gts_surface_vertex_number( __surface[0] ) +
						   gts_surface_vertex_number( __surface[1] );
		uint facetCount = gts_surface_face_number( __surface[0] ) +
						  gts_surface_face_number( __surface[1] );
		
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		
		B.begin_surface( vertexCount, facetCount); 		
		
		int addedVertices = 0;
		
		for (int i=0; i <2; i++) {
		
			GtsFace * first = NULL;
			
			gts_surface_foreach_face (__surface[i], (GtsFunc) pick_first_face, &first);
			
			GtsSurfaceTraverse * t = gts_surface_traverse_new ( __surface[i], first);
			GtsFace * f;
			guint level;
			
			while ((f = gts_surface_traverse_next (t, &level))) {
			
				// do something with f
				
				// add vertex to B
				
				toAdd.clear();
				
				//GtsPoint * p1, * p2, * p3;
				GtsPoint * points[3];
				
				uint p = 0, q = 0;
				
				// the second surface must be done backwards
				// to maintain the same orientation on the resulting surface
				
				if (i != 0) { 
					gts_triangle_vertices((GtsTriangle *) f, 
										  (GtsVertex **) &points[2], 
										  (GtsVertex **) &points[1], 
										  (GtsVertex **) &points[0]);
					
				} else {
					gts_triangle_vertices((GtsTriangle *) f, 
										  (GtsVertex **) &points[0], 
										  (GtsVertex **) &points[1], 
										  (GtsVertex **) &points[2]);
					
				}
				
								
				for (int k = 0; k<3; k++) { 
				
					QString key = SurfaceMesher::getPointKey(points[k]->x, 
															 points[k]->y, 
															 points[k]->z);
					
					if (addedPoints.contains( key ) ) {
						toAdd.push_back( addedPoints[ key ] );
					}
					else {
						B.add_vertex( Point_3(points[k]->x, 
											  points[k]->y, 
											  points[k]->z ) );
						
						addedPoints[ key ] = addedVertices;
						toAdd.push_back(addedVertices);
						addedVertices++;
						
					}
				}
				
				// add facet to B
				B.begin_facet();
				for (int k=0; k < 3; k++) {
					B.add_vertex_to_facet(toAdd[k]);
				}
				B.end_facet();
			}
			
			gts_surface_traverse_destroy(t);
		}

		B.end_surface();
		
    }
	
	
	
	void addSurface(GtsSurface *surface0_, GtsSurface *surface1_)
		{ __surface[0] = surface0_;
		  __surface[1] = surface1_;};
	
	
private:
	GtsSurface *__surface[2];
	
	
	
};
*/


template <class HDS>
class BuildPolyhedronFromCutOut : public CGAL::Modifier_base<HDS> {

public:
	
	BuildPolyhedronFromCutOut() {}
	
	void operator()(HDS& hds){

		CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		
		size_t vertexCount = __cutOutInformation->vertex.size();
		size_t facetCount = __cutOutInformation->facets.size();
				 
		B.begin_surface( vertexCount, facetCount); 
		
		for(int i=0; i< vertexCount; i++) {
			B.add_vertex( __cutOutInformation->vertex[i] );
		}
		
		
		for(int i=0; i< facetCount; i++) {
			
			B.begin_facet();
			for(int j=0; j < __cutOutInformation->facets[i].size(); j++) {
			
				B.add_vertex_to_facet( __cutOutInformation->facets[i][j] );
			
			}
			B.end_facet();
		}
		
		B.end_surface();
	}
	
	void addCutOut(CutOutInformation *cut_ ){ __cutOutInformation = cut_; };
	
private:
	
	
	CutOutInformation *__cutOutInformation;
};

// the following two templates should have been only one but prooved difficult to
// program.


/*
template <class HDS>
class BuildPolyhedronFromExactPolyhedron : public CGAL::Modifier_base<HDS> {
	
public:
	
	BuildPolyhedronFromExactPolyhedron() {}
	
	void operator()(HDS& hds){
		std::map<Point_3, int> addedPoints;
		std::map<Point_3, int>::iterator addedPointsIterator;
		
		CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		
		size_t vertexCount = __polyhedron->size_of_vertices();
		size_t facetCount = __polyhedron->size_of_vertices();
		
		B.begin_surface( vertexCount, facetCount); 
		
		ExactPolyhedron::Facet_iterator facetIterator;
		int addedVertices = 0;
		ExactFacet::Halfedge_around_facet_circulator halfEdgeCirculator;
		std::list<int> toAdd;
		
		for (facetIterator = __polyhedron->facets_begin(); facetIterator != __polyhedron->facets_end(); ++facetIterator) {
			
			toAdd.clear();
			
			halfEdgeCirculator = facetIterator->facet_begin();
			
			do {
								
				Point_3 currentPoint(CGAL::to_double( halfEdgeCirculator->vertex()->point().x() ), 
									 CGAL::to_double( halfEdgeCirculator->vertex()->point().y() ),
									 CGAL::to_double( halfEdgeCirculator->vertex()->point().z() )  );
				
				addedPointsIterator = addedPoints.find( currentPoint );
				
				if ( addedPointsIterator != addedPoints.end() ) { // already added
					toAdd.push_back( addedPointsIterator->second );
				}
				else {
					B.add_vertex( currentPoint );
					addedPoints[currentPoint] = addedVertices;
					toAdd.push_back(addedVertices++);
				}
				
				++halfEdgeCirculator;
			} while ( halfEdgeCirculator != facetIterator->facet_begin() );
			
			B.begin_facet();
			std::list<int>::iterator toAddIterator;
			for (toAddIterator = toAdd.begin(); toAddIterator != toAdd.end(); ++toAddIterator) { 
				B.add_vertex_to_facet( *toAddIterator );
			}
			
			B.end_facet();
			
		}
		
		B.end_surface();
	}
	
	void addExactPolyhedron(ExactPolyhedron *poly_){ __polyhedron = poly_; };
	
private:
	
	ExactPolyhedron * __polyhedron;
	
};

template <class HDS>
class BuildExactPolyhedronFromPolyhedron : public CGAL::Modifier_base<HDS> {
	
public:
	
	BuildExactPolyhedronFromPolyhedron() {}
	
	void operator()(HDS& hds){
		std::map<ExactPoint_3, int> addedPoints;
		std::map<ExactPoint_3, int>::iterator addedPointsIterator;
		
		CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
		
		size_t vertexCount = __polyhedron->size_of_vertices();
		size_t facetCount = __polyhedron->size_of_vertices();
		
		B.begin_surface( vertexCount, facetCount); 
		
		Polyhedron::Facet_iterator facetIterator;
		int addedVertices = 0;
		Facet::Halfedge_around_facet_circulator halfEdgeCirculator;
		std::list<int> toAdd;
		
		for (facetIterator = __polyhedron->facets_begin(); facetIterator != __polyhedron->facets_end(); ++facetIterator) {
			toAdd.clear();
			
			halfEdgeCirculator = facetIterator->facet_begin();
			
			do {
				
				ExactPoint_3 currentPoint(CGAL::to_double( halfEdgeCirculator->vertex()->point().x() ), 
										  CGAL::to_double( halfEdgeCirculator->vertex()->point().y() ),
										  CGAL::to_double( halfEdgeCirculator->vertex()->point().z() ) );
				
				addedPointsIterator = addedPoints.find( currentPoint );
				
				if ( addedPointsIterator != addedPoints.end() ) { // already added
					
					toAdd.push_back( addedPointsIterator->second );
				}
				else {
					B.add_vertex( currentPoint );
					addedPoints[currentPoint] = addedVertices;
					toAdd.push_back(addedVertices++);
				}
				
				++halfEdgeCirculator;
			} while ( halfEdgeCirculator != facetIterator->facet_begin() );
			
			B.begin_facet();
			std::list<int>::iterator toAddIterator;
			for (toAddIterator = toAdd.begin(); toAddIterator != toAdd.end(); ++toAddIterator) { 
				B.add_vertex_to_facet( *toAddIterator );
			}
			
			B.end_facet();
			
		}
		
		B.end_surface();
	}
	
	void addPolyhedron(Polyhedron *poly_){ __polyhedron = poly_; };
	
private:
	
	Polyhedron * __polyhedron;
	
};

*/
/////////


template <class Poly>
class WLoop_mask_3 {
	typedef Poly                                         Polyhedron;
	
	typedef typename Polyhedron::Vertex_iterator         Vertex_iterator;
	typedef typename Polyhedron::Halfedge_iterator       Halfedge_iterator;
	typedef typename Polyhedron::Facet_iterator          Facet_iterator;
	
	typedef typename Polyhedron::Halfedge_around_facet_circulator
		Halfedge_around_facet_circulator;
	typedef typename Polyhedron::Halfedge_around_vertex_circulator
		Halfedge_around_vertex_circulator;
	
	typedef typename Polyhedron::Traits                  Traits;
	typedef typename Traits::Kernel                      Kernel;
	
	typedef typename Kernel::FT                          FT;
	typedef typename Kernel::Point_3                     Point;
	typedef typename Kernel::Vector_3                    Vector;
	
public:
	void edge_node(Halfedge_iterator eitr, Point& pt) {
		Point& p1 = eitr->vertex()->point();
		Point& p2 = eitr->opposite()->vertex()->point();
		Point& f1 = eitr->next()->vertex()->point();
		Point& f2 = eitr->opposite()->next()->vertex()->point();
		
		pt = Point((3*(p1[0]+p2[0])+f1[0]+f2[0])/8,
				   (3*(p1[1]+p2[1])+f1[1]+f2[1])/8,
				   (3*(p1[2]+p2[2])+f1[2]+f2[2])/8 );
	}
	void vertex_node(Vertex_iterator vitr, Point& pt) {
		double R[] = {0.0, 0.0, 0.0};
		Point& S = vitr->point();
		
		Halfedge_around_vertex_circulator vcir = vitr->vertex_begin();
		int n = circulator_size(vcir);
		for (int i = 0; i < n; i++, ++vcir) {
			Point& p = vcir->opposite()->vertex()->point();
			R[0] += p[0]; 	R[1] += p[1]; 	R[2] += p[2];
		}
		if (n == 6) {
			pt = Point((10*S[0]+R[0])/16, (10*S[1]+R[1])/16, (10*S[2]+R[2])/16);
		} else if (n == 3) {
			double B = (5.0/8.0 - std::sqrt(3+2*std::cos(6.283/n))/64.0)/n;
			double A = 1-n*B;
			pt = Point((A*S[0]+B*R[0]), (A*S[1]+B*R[1]), (A*S[2]+B*R[2]));
		} else {
			double B = 3.0/8.0/n;
			double A = 1-n*B;
			pt = Point((A*S[0]+B*R[0]), (A*S[1]+B*R[1]), (A*S[2]+B*R[2]));
		}
	}
	void border_node(Halfedge_iterator eitr, Point& ept, Point& vpt) {
		Point& ep1 = eitr->vertex()->point();
		Point& ep2 = eitr->opposite()->vertex()->point();
		ept = Point((ep1[0]+ep2[0])/2, (ep1[1]+ep2[1])/2, (ep1[2]+ep2[2])/2);
		
		Halfedge_around_vertex_circulator vcir = eitr->vertex_begin();
		Point& vp1  = vcir->opposite()->vertex()->point();
		Point& vp0  = vcir->vertex()->point();
		Point& vp_1 = (--vcir)->opposite()->vertex()->point();
		vpt = Point((vp_1[0] + 6*vp0[0] + vp1[0])/8,
					(vp_1[1] + 6*vp0[1] + vp1[1])/8,
					(vp_1[2] + 6*vp0[2] + vp1[2])/8 );
	}
};






#endif