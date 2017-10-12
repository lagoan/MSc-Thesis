/*
 *  SurfaceMesher.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 20/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

/*
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Skin_surface_3.h>
#include <CGAL/mesh_skin_surface_3.h>
#include <CGAL/subdivide_skin_surface_mesh_3.h>
#include <CGAL/Skin_surface_refinement_policy_3.h>
#include <list>
*/

//#define _HAS_ITERATOR_DEBUGGING 0

//#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
//#include <CGAL/Exact_predicates_exact_constructions_kernel.h>

#if 0
#include "SurfaceMesher.h"

#include <iostream>
using namespace std;








/*
struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

typedef CGAL::Alpha_shape_vertex_base_3<K>          Vb;
typedef CGAL::Alpha_shape_cell_base_3<K>            Fb;
typedef CGAL::Triangulation_data_structure_3<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_3<K,Tds>       Triangulation_3;
typedef CGAL::Alpha_shape_3<Triangulation_3>        Alpha_shape_3;

typedef K::Point_3                                  PointA;
typedef Alpha_shape_3::Alpha_iterator               Alpha_iterator;
typedef Alpha_shape_3::Facet						FacetA;
*/

Polygons SurfaceMesher::getMesh(vector<ParticleInstance*> &particles)
{
	Polygons polygons;
	Polyhedron polyhedron;
	Delaunay_hierarchy dt;
	//std::list<PointA>       points;
	BuildPolyhedronFromAShape<HalfedgeDS> newPolyhedron;

	ovInt particleCount = particles.size();
	std::cout << "Starting mesher\n";

	for (ovInt i=0; i < particleCount; i++) {
		
		//points.push_back( PointA(particles[i]->pos.x, particles[i]->pos.y, particles[i]->pos.z) );
		dt.insert( PointA(particles[i]->pos.x, particles[i]->pos.y, particles[i]->pos.z) );
	}

	//Delaunay_hierarchy dt(points.begin(), points.end());

	std::cout << "Delaunay computed." << std::endl;

	// compute alpha shape  
	Alpha_shape_3 as(dt);
	//Alpha_shape_3 as(points.begin(), points.end());
	
	
	
	
	// find optimal alpha values
	/*
	Alpha_shape_3::NT alpha_solid = as.find_alpha_solid();
	Alpha_iterator opt = as.find_optimal_alpha(1);
	std::cout << "Smallest alpha value to get a solid through data points is " << alpha_solid << std::endl;
	std::cout << "Optimal alpha value to get one connected component is " <<  *opt    << std::endl;
	as.set_alpha(*opt);
	assert(as.number_of_solid_components() == 1);
	*/

	// set it to particle distance
	as.set_alpha(0.20); 

	//std::list<FacetA>       facets;
	std::cout << "alpha set, adding shape\n";
	newPolyhedron.addAShape(&as);

	// breaking down
	polyhedron.delegate(newPolyhedron);

	//as.get_alpha_shape_facets(std::back_inserter(facets), Alpha_shape_3::REGULAR);
	//std::cout << facets.size() << " boundary facets" << std::endl;
	
	std::cout << "Done meshing points\n";

	CGAL::Subdivision_method_3::Sqrt3_subdivision(polyhedron);
	//CGAL::Subdivision_method_3::Loop_subdivision(polyhedron);
	
	std::cout << "finished subdivision\n";

	polygons = polyhedron;
	return polygons;
}
#endif




