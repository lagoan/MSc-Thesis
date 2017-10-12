/*
 *  CGALDefinitions.h
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 12/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef CGALDEFINITIONS_H
#define CGALDEFINITIONS_H

#include <CGAL/Cartesian.h>
//#include <CGAL/Lazy_exact_nt.h>
//#include <CGAL/Gmpq.h>
#include <CGAL/Filtered_kernel.h>
#include <CGAL/Simple_cartesian.h>

#include <list>
#include <utility> // defines std::pair

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/pca_estimate_normals.h>
//#include <CGAL/mst_orient_normals.h>
//#include <CGAL/property_map.h>
//#include <CGAL/Point_with_normal_3.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

#include <CGAL/Aff_transformation_3.h>
#include <CGAL/aff_transformation_tags.h>


#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_hierarchy_3.h>
#include <CGAL/Alpha_shape_3.h>


#include <CGAL/Gmpz.h>
//#include <CGAL/Extended_homogeneous.h>
//#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Weighted_alpha_shape_euclidean_traits_3.h>
#include <CGAL/Regular_triangulation_3.h>
#include <CGAL/Weighted_point.h>
//#include <CGAL/make_skin_surface_mesh_3.h>


#include <CGAL/Subdivision_method_3.h>
#include <CGAL/Sphere_3.h>
#include <CGAL/make_surface_mesh.h>
#include <CGAL/Implicit_surface_3.h>
//#include <CGAL/Poisson_reconstruction_function.h>
#include <CGAL/Surface_mesh_default_triangulation_3.h>
//#include <CGAL/grid_simplify_point_set.h>
//#include <CGAL/remove_outliers.h>
//#include <CGAL/IO/output_surface_facets_to_polyhedron.h>
//#include <CGAL/jet_smooth_point_set.h>


#include <CGAL/Convex_hull_traits_3.h>
#include <CGAL/convex_hull_3.h>
//typedef CGAL::Gmpq										NT;

//typedef CGAL::Cartesian<NT>  							Kernel;

//typedef CGAL::Cartesian< CGAL::Lazy_exact_nt<NT> >		Kernel;  // probaste este ayer y estubo funcinoando

//typedef CGAL::Extended_cartesian<double>			Kernel;

//typedef CGAL::Extended_cartesian<CGAL::Gmpq>  Kernel;

// XXX
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
//typedef CGAL::Simple_cartesian<double>		Kernel;
//typedef CGAL::Cartesian<double>		Kernel;

//typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt Kernel;
//typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;

//typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;

//typedef CGAL::Cartesian<double>						Kernel;
//typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<double> > Kernel;



//typedef  CGAL::Simple_cartesian<double>					Kernel;

//typedef CGAL::Filtered_kernel<CGAL::Simple_cartesian<double> > Kernel;

//struct Kernel : public CGAL::Exact_predicates_exact_constructions_kernel {};

//typedef CGAL::Cartesian<NT>								ExactKernel;

typedef CGAL::Polyhedron_3<Kernel>						Polyhedron;
typedef Polyhedron::Halfedge_iterator					Halfedge_iterator;
typedef Polyhedron::HalfedgeDS							HalfedgeDS;
typedef Polyhedron::Halfedge							Halfedge;
typedef Polyhedron::Facet								Facet;
typedef Polyhedron::Vertex								Vertex;
typedef CGAL::Vector_3<Kernel>							Vector_3;

typedef CGAL::Aff_transformation_3<Kernel>				Aff_transformation_3;
//typedef CGAL::Aff_transformation_3<ExactKernel>			ExactAff_transformation_3;
//typedef CGAL::Nef_polyhedron_3<ExactKernel>				Nef_polyhedron;


//typedef CGAL::Cartesian<CGAL::Gmpq>					ExactKernel;
//typedef CGAL::Exact_predicates_exact_constructions_kernel ExactKernel;

//typedef CGAL::Gmpz  NT;
//typedef CGAL::Extended_homogeneous<NT>  Kernel2;
//typedef CGAL::Polyhedron_3<ExactKernel>					ExactPolyhedron;
//typedef ExactPolyhedron::Halfedge_iterator					ExactHalfedge_iterator;
//typedef ExactPolyhedron::HalfedgeDS							ExactHalfedgeDS;
//typedef ExactPolyhedron::Facet								ExactFacet;
//typedef ExactPolyhedron::Vertex								ExactVertex;
//typedef ExactKernel::Point_3							ExactPoint_3;


//typedef CGAL::Nef_polyhedron_3<ExactKernel>				Nef_polyhedron;
//typedef CGAL::Aff_transformation_3<ExactKernel>			ExactAff_transformation_3;
//typedef CGAL::Vector_3<ExactKernel>						ExactVector_3;

/*
typedef CGAL::Extended_homogeneous<CGAL::Gmpz>			nefKernel;
typedef CGAL::Polyhedron_3<nefKernel>						extendedPolyhedron;
typedef CGAL::Nef_polyhedron_3<nefKernel>					Nef_polyhedron;
typedef Nef_polyhedron::Vector_3						nefVector_3;
*/

//typedef CGAL::Polyhedron_3<ExactKernel>					ExactPolyhedron;
//typedef ExactPolyhedron::HalfedgeDS						ExactHalfedgeDS;
//typedef ExactKernel::Point_3							ExactPoint_3;

typedef Kernel::FT FT;
typedef Kernel::Point_3									CGALPoint_3; // old
//typedef std::pair<Point_3, Vector_3>					PointVectorPair;
//typedef CGAL::Point_with_normal_3<Kernel>				Point_with_normal;
typedef Kernel::Sphere_3								Sphere;
//typedef std::vector<Point_with_normal>					PointList;
//typedef CGAL::Poisson_reconstruction_function<Kernel>	Poisson_reconstruction_function;
//typedef CGAL::Implicit_surface_3<Kernel, Poisson_reconstruction_function> Surface_3;
typedef CGAL::Surface_mesh_default_triangulation_3 STr;
typedef CGAL::Surface_mesh_complex_2_in_triangulation_3<STr> C2t3;

typedef Kernel::Segment_3								Segment_3;
typedef Kernel::Point_2									Point_2;
typedef Kernel::Triangle_2								Triangle_2;
typedef Kernel::Vector_2								Vector_2;
typedef Kernel::Triangle_3								Triangle_3;
typedef Kernel::Plane_3									Plane_3;

typedef Polyhedron::Halfedge_handle						Halfedge_handle;
typedef CGAL::Convex_hull_traits_3<Kernel>				Traits;
//typedef CGAL::Search_traits_3<Kernel>					SearchTraits;
// for alpha shape


 

 //wegihted
/*
typedef CGAL::Weighted_alpha_shape_euclidean_traits_3<Kernel> Gt;
typedef CGAL::Alpha_shape_vertex_base_3<Gt>         AVb;
typedef CGAL::Alpha_shape_cell_base_3<Gt>           AFb;
typedef CGAL::Triangulation_data_structure_3<AVb,AFb> Tds;
typedef CGAL::Regular_triangulation_3<Gt,Tds>       Triangulation_3;
typedef CGAL::Alpha_shape_3<Triangulation_3>        Alpha_shape_3;

typedef Gt::Weighted_point                  Weighted_point;
typedef Gt::Bare_point                      Bare_point;
*/


typedef CGAL::Weighted_alpha_shape_euclidean_traits_3<Kernel> Gt;

typedef CGAL::Alpha_shape_vertex_base_3<Gt>               Vb;
typedef CGAL::Triangulation_hierarchy_vertex_base_3<Vb>  Vbh;
typedef CGAL::Alpha_shape_cell_base_3<Gt>                 Fb;
typedef CGAL::Triangulation_data_structure_3<Vbh,Fb>     Tds;
typedef CGAL::Delaunay_triangulation_3<Gt,Tds>            Delaunay;
typedef CGAL::Triangulation_hierarchy_3<Delaunay>        Delaunay_hierarchy;
typedef CGAL::Alpha_shape_3<Delaunay_hierarchy>          Alpha_shape_3;


typedef Alpha_shape_3::Edge                 EdgeA;

typedef Alpha_shape_3::Facet							FacetA;
typedef Alpha_shape_3::Vertex							VertexA;
typedef Alpha_shape_3::Vertex_handle					Vertex_handleA;	
typedef Alpha_shape_3::Cell_handle						Cell_handleA;						
typedef Alpha_shape_3::Alpha_iterator					Alpha_iterator;
typedef Alpha_shape_3::NT								Alpha_NT;





// for skin surface
//typedef CGAL::Weighted_point<Point_3, Kernel::RT>  Weighted_point;

typedef CGAL::Exact_predicates_inexact_constructions_kernel ExPInCKernel;
typedef ExPInCKernel::Point_3                               Bare_point;
typedef CGAL::Weighted_point<Bare_point, ExPInCKernel::RT>  Weighted_point;
typedef CGAL::Polyhedron_3<ExPInCKernel>                    SurfacePolyhedron;


typedef CGAL::HalfedgeDS_default<Kernel, CGAL::Polyhedron_items_3>	HDS;


// custom Point_3 class

class Point_3 {
	
public:
	Point_3();
	Point_3(double, double, double);
	~Point_3(){};
	double & x();
	double & y();
	double & z();
	
	double & operator[](int i) {
		return data[i];
	}
	
	bool operator <(const Point_3& b) const {
		if (data[0] < b.x()) return true;
		else if (data[0] > b.x()) return false;
		else if (data[1] < b.y()) return true;
		else if (data[1] > b.y()) return false;
		else if (data[2] < b.z()) return true;
		else if (data[2] > b.z()) return false;
		
		return false;
	}

	// should return vector but we want it simple
	Point_3 operator- (const Point_3& b) {
		return Point_3(data[0] - b.x(),
					   data[1] - b.y(),
					   data[2] - b.z());
	}
	
	double getDistanceTo(const Point_3& b);
	
	double dotP(const Point_3& b);

	
private:
	double data[3];
	
	
};

class VoxelCoord {
public:
	int i;
	int j;
	int k;
	
	VoxelCoord() : i(0), j(0), k(0){};
	VoxelCoord(int a, int b, int c) : i(a), j(b), k(c){};
	~VoxelCoord() {};
	
	bool operator <(const VoxelCoord& b) const {
		if (i < b.i) return true;
		else if (i > b.i) return false;
		else if (j < b.j) return true;
		else if (j > b.j) return false;
		else if (k < b.k) return true;
		else if (k > b.k) return false;
		
		return false;
	};
	
	bool operator ==(const VoxelCoord& b) const {
		return i==b.i && j==b.j && k==b.k;
	};
	
};

#endif
