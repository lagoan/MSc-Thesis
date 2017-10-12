/*
 *  SurfaceMesher.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 20/11/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef SURFACEMESHER_H
#define SURFACEMESHER_H


#if 0
#include "primitives/ParticleInstance.h"
#include "primitives/Polygons.h"
#include "system/types.h"


#include <CGAL/Polyhedron_incremental_builder_3.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Delaunay_triangulation_3.h>
#include <CGAL/Triangulation_hierarchy_3.h>
#include <CGAL/Alpha_shape_3.h>
#include <list>
#include <vector>
#include <map>

typedef Kernel											K;
//typedef CGAL::Exact_predicates_exact_constructions_kernel K;

typedef CGAL::Alpha_shape_vertex_base_3<K>               Vb;
typedef CGAL::Alpha_shape_cell_base_3<K>                 Fb;
typedef CGAL::Triangulation_hierarchy_vertex_base_3<Vb>  Vbh;
typedef CGAL::Triangulation_data_structure_3<Vbh,Fb>     Tds;
typedef CGAL::Delaunay_triangulation_3<K,Tds>            Delaunay;
typedef CGAL::Triangulation_hierarchy_3<Delaunay>        Delaunay_hierarchy;
typedef CGAL::Alpha_shape_3<Delaunay_hierarchy>          Alpha_shape_3;

typedef K::Point_3                                  PointA;
typedef Alpha_shape_3::Facet						FacetA;
typedef Alpha_shape_3::Vertex						VertexA;
typedef Alpha_shape_3::Vertex_handle				Vertex_handleA;	
typedef Alpha_shape_3::Cell_handle					Cell_handleA;						
typedef Alpha_shape_3::Alpha_iterator               Alpha_iterator;
typedef Alpha_shape_3::NT                           NT;

template <class HDS>
class BuildPolyhedronFromAShape : public CGAL::Modifier_base<HDS> {
public:
    BuildPolyhedronFromAShape() {}

    void operator()(HDS& hds){

		std::list<FacetA> facets;
		std::list<FacetA>::iterator facetIterator;
		std::list<Vertex_handleA> vertices;
		std::list<Vertex_handleA>::iterator verticeIterator;
		//std::vector<Point_3> vertices;
		std::map<Point_3, int> addedPoints;
		std::map<Point_3, int>::iterator addedPointsIterator;
		std::vector<int> pointsIdexes;
		std::vector<int> toAdd;
		Point_3 pointP;
		Vector_3 vectorA, vectorB, vectorC, normalVector;
		Halfedge_iterator hdsIterator;
		Vertex_handleA vertexHandler;
		Cell_handleA cellHandler;
		int index;

		__as->get_alpha_shape_facets(std::back_inserter(facets), Alpha_shape_3::REGULAR);
		__as->get_alpha_shape_vertices(std::back_inserter(vertices), Alpha_shape_3::REGULAR);

		std::cout << "Solids " << __as->number_of_solid_components() << "\n";
		// Postcondition: `hds' is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);

		B.begin_surface( vertices.size(), facets.size()); 
		//B.begin_surface( facets.size() * 3, facets.size());  
		
		/*
		for (verticeIterator = vertices.start(); verticesIterator != vertices.end(); ++verticesIterator) {



		}
		*/

		// XXX magic goes here
		ovInt numVertices = 0;
		ovInt addedVertices = 0;
		for (facetIterator = facets.begin(); facetIterator != facets.end(); ++facetIterator) {
			//hdsStart = hdsCirculator = facetIterator->facet_begin();

			
			cellHandler = facetIterator->first;
			index = facetIterator->second;

			// the following ensures that regular facets are output
			// in ccw order
			if ((__as->classify(*facetIterator) == Alpha_shape_3::REGULAR) &&
			   (__as->classify(cellHandler) == Alpha_shape_3::INTERIOR)) {
					cellHandler = cellHandler->neighbor(index);
					index = cellHandler->index(facetIterator->first);
			}

			ovInt current;
			toAdd.clear();
			for (ovInt k = 0; k<3; k++) {
				current = Alpha_shape_3::Triangulation_utils_3::vertex_triple_index(index, k);
				addedPointsIterator = addedPoints.find(cellHandler->vertex(current)->point());

				if ( addedPointsIterator != addedPoints.end() ) {
					
					toAdd.push_back(addedPointsIterator->second);
				}
				else {
					B.add_vertex( cellHandler->vertex(current)->point() );
					addedPoints[cellHandler->vertex(current)->point()] = addedVertices;
					toAdd.push_back(addedVertices);
					addedVertices++;
				}

			}

			B.begin_facet();
			for (ovInt i=0; i < 3; i++) {
				//B.add_vertex_to_facet(numVertices - (3-i));
				B.add_vertex_to_facet(toAdd[i]);
			}
			B.end_facet();

#if 0
			i0 = Alpha_shape_3::Triangulation_utils_3::vertex_triple_index(index,0);
			i1 = Alpha_shape_3::Triangulation_utils_3::vertex_triple_index(index,1);
			i2 = Alpha_shape_3::Triangulation_utils_3::vertex_triple_index(index,2);


			addedPoints[cellHandler->vertex(i0)->point()] = 1;

			B.add_vertex( cellHandler->vertex(i0)->point() );
			B.add_vertex( cellHandler->vertex(i1)->point() );
			B.add_vertex( cellHandler->vertex(i2)->point() );
	
			
			B.begin_facet();
			for (ovInt i=0; i < 3; i++) {
				//B.add_vertex_to_facet(numVertices - (3-i));
				B.add_vertex_to_facet(addedVertices++);
			}
			B.end_facet();
#endif	

			



/* // works perfectly
			B.begin_facet();
			for (ovInt i=0; i < 4; i++) {
				if (facetIterator->second == i) continue;
				vertexHandler = facetIterator->first->vertex(i);
				//std::cout << facetIterator->second << "\n";
				B.add_vertex( vertexHandler->point() );
				B.add_vertex_to_facet(numVertices++);
			}
			B.end_facet();
*/

		}



		B.end_surface();

    }

	
	void addAShape(Alpha_shape_3 *as_){ __as = as_; };
	

private:
	Alpha_shape_3 *__as;

};


class SurfaceMesher 
{
public:
	
	SurfaceMesher(){};
	~SurfaceMesher(){};
	
	static Polygons getMesh(std::vector<ParticleInstance*> &particles);
	static Polygons translatePolyhedron(Polyhedron &polyhedron);

private:

};

#endif

#endif