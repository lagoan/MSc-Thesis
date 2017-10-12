/*
 *  types.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 08/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef TYPES_H
#define TYPES_H



//#include <CGAL/Gmpz.h>
//#include <CGAL/Homogeneous.h>
//#include <CGAL/Gmpq.h>
//#include <CGAL/Quotient.h>
//#include <CGAL/MP_Float.h>
//#include <CGAL/Cartesian.h>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Point_3.h>
#include <CGAL/Nef_polyhedron_3.h>
#include <CGAL/Delaunay_triangulation_3.h>

#include <CGAL/Triangulation_3.h>
#include <CGAL/Subdivision_method_3.h>
#include <dom/domTriangles.h>
#include <dom/domPolygons.h>
#include <dom/domPolylist.h>

//typedef CGAL::Gmpq										Gmpq;
//typedef CGAL::Cartesian<CGAL::Quotient<CGAL::MP_Float>>  	Kernel;
//typedef CGAL::Cartesian<Gmpq>  							Kernel;

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Exact_predicates_exact_constructions_kernel   ExactKernel;


typedef CGAL::Triangulation_3<Kernel>      Triangulation;


typedef Kernel::Point_3									Point_3;
typedef Kernel::Vector_3								Vector_3;
typedef Kernel::Plane_3									Plane_3;
//typedef CGAL::2_traits_with_normals_3<Kernel>			Traits;
typedef CGAL::Polyhedron_3<Kernel>						Polyhedron;
typedef Polyhedron::HalfedgeDS							HalfedgeDS;
typedef Polyhedron::Vertex								Vertex;
typedef Polyhedron::Halfedge							Halfedge;
typedef Polyhedron::Facet								Facet;
typedef Polyhedron::Vertex_iterator                     Vertex_iterator;
typedef Polyhedron::Halfedge_iterator					Halfedge_iterator;
typedef Polyhedron::Facet_iterator						Facet_iterator;
typedef Polyhedron::Halfedge_handle						Halfedge_handle;
typedef Polyhedron::Vertex_handle						Vertex_handle;
typedef Polyhedron::Point_3								Point;

typedef CGAL::Polyhedron_3<ExactKernel>					ExactPolyhedron;
typedef ExactPolyhedron::Point_3						ExactPoint_3;
typedef ExactPolyhedron::Halfedge_iterator				ExactHalfedge_iterator;
typedef ExactPolyhedron::HalfedgeDS						ExactHalfedgeDS;
typedef ExactPolyhedron::Facet							ExactFacet;
typedef ExactPolyhedron::Vertex							ExactVertex;
typedef CGAL::Nef_polyhedron_3<ExactKernel>				Nef_polyhedron;

typedef Nef_polyhedron::Vertex_const_handle				Vertex_const_handle;
typedef Nef_polyhedron::Halfedge_const_handle			Halfedge_const_handle;
typedef Nef_polyhedron::Halffacet_const_handle			Halffacet_const_handle;
typedef Nef_polyhedron::Volume_const_handle				Volume_const_handle;
typedef Nef_polyhedron::Object_handle					Object_handle;

//typedef CGAL::Nef_polyhedron_3<Kernel>				Nef_polyhedron;
/*
 typedef Nef_polyhedron::Vertex_const_handle			Vertex_const_handle;
 typedef Nef_polyhedron::Halfedge_const_handle			Halfedge_const_handle;
 typedef Nef_polyhedron::Halffacet_const_handle			Halffacet_const_handle;
 typedef Nef_polyhedron::Volume_const_handle			Volume_const_handle;
 typedef Nef_polyhedron::Object_handle					Object_handle;
 */




#define ovInt32		int
#define ovInt16		short
#define ovInt		ovInt32
#define ovUInt		unsigned int
#define ovFloat		double
#define ovDouble	double
#define ovBool		bool
#define ovChar		char
#define ovByte		char

#define ovPI		3.14159265358979323846

union ovTuple4f {
	struct {
		ovFloat x, y, z, w;
	};
	struct {
		ovFloat r, g, b, a;
	};
	ovFloat T[4];

};

union ovTuple3f {
	struct {
		ovFloat x, y, z;
	};
	struct {
		ovFloat r, g, b;
	};
	ovFloat T[3];

};

union ovTuple2f {
public:
	struct {
		ovFloat x, y;
	};
	struct {
		ovFloat u, v;
	};
	ovFloat T[2];

};

enum ovWorkOn {
	AnimateFluids,
	AnimateBubbles
};




#endif

