/*
 *  Polygons.h
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 19/08/07.
 *  Copyright 2007 __MyCompanyName__. All rights reserved.
 *
 */


#ifndef POLYGON_H
#define POLYGON_H



#include <iostream>


#include <QGLWidget>
#include <vector>


#include "system/types.h"






template <class HDS>
class BuildPolyhedron : public CGAL::Modifier_base<HDS> {
public:
    BuildPolyhedron() {}

    void operator()(HDS& hds){
        // Postcondition: `hds' is a valid polyhedral surface.
        CGAL::Polyhedron_incremental_builder_3<HDS> B( hds, true);
    
		B.begin_surface( _vertices.size(), _facets.size());  
		typedef typename HDS::Vertex   Vertex;
        typedef typename Vertex::Point Point;
        
		for (ovInt i=0; i<_vertices.size(); i++) {
			B.add_vertex( Point(_vertices[i].x, _vertices[i].y, _vertices[i].z));
		}
		for (ovInt i=0; i<_facets.size(); i++) {
			B.begin_facet();
			for (ovInt j=0; j<_facets[i].size(); j++) {
				B.add_vertex_to_facet( _facets[i][j]);
			}
			B.end_facet();
		}
        B.end_surface();
		// XXX should we clear them ?
		_vertices.clear();
		_facets.clear();
    }

	//void beginSurface(ovInt vertex, ovInt faces){};
	void setVertices(std::vector< ovTuple3f > vs){ _vertices = vs; };
	void addVertex(ovTuple3f v){ _vertices.push_back(v);};
	void addFacet(std::vector<ovInt> facet){_facets.push_back(facet);};

private:
	std::vector< ovTuple3f > _vertices;
	std::vector< std::vector< ovInt > > _facets;


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





class Polygons {
public:

	ovInt triangleCount;
	ovInt sourcePositionCount;
	ovInt sourceNormalCount;
	ovInt indexCount;

	ovTuple3f* sourcePosition;
	ovTuple3f* sourceNormal;
	
	//CGAL

	Polyhedron	   polyhedron;
	//Nef_polyhedron nefPolyhedron;

	struct Face {
		std::vector<ovInt>	indexPosition;
		std::vector<ovInt>	indexNormal;
		std::vector<ovInt>	indexColor;
		std::vector<ovInt>	indexTexCoord;

	};

	Polygons();
	~Polygons();
	Polygons(domTriangles&);
	Polygons(domPolygons&);
	Polygons(domPolylist&);
	Polygons(const Polygons& polygons);
	void operator=(const Polygons& polygons);
	void operator=(const Polyhedron& polyhedron_);
	
	void render();
	void triangulate();

	void facesPushBack(Face face);
	void sourcePositionPushBack(ovTuple3f pos);
	void sourceNormalPushBack(ovTuple3f norm);

	std::vector<ovTuple3f> getSourcePositionWorkingVector(){ return _sourcePosition; };

private:

	//GLuint* indexPosition;
	//GLuint* indexNormal;

	std::vector<Face>		_faces;
	std::vector<ovTuple3f>	_sourcePosition;
	std::vector<ovTuple3f>	_sourceNormal;
	std::vector<ovTuple3f>	_sourceColor;
	std::vector<ovTuple2f>	_sourceTexCoord;

	Vector_3 getFacetsNormal(Facet_iterator facet);


};


inline void Polygons::operator=(const Polygons& polygons_)
{

	if (sourcePosition)
		delete sourcePosition;
	if (sourceNormal)
		delete sourceNormal;

	triangleCount		= polygons_.triangleCount;
	sourcePositionCount = polygons_.sourcePositionCount;
	sourceNormalCount	= polygons_.sourceNormalCount;
	indexCount			= polygons_.indexCount;
	polyhedron			= polygons_.polyhedron;
	//nefPolyhedron		= polygons_.nefPolyhedron;


	sourcePosition = new ovTuple3f[sourcePositionCount];
	sourceNormal   = new ovTuple3f[sourceNormalCount];



	for (ovInt i = 0; i < sourcePositionCount; i++) {
		sourcePosition[i] = polygons_.sourcePosition[i];
		
	}	

 
	for (ovInt i = 0; i < sourceNormalCount; i++) {
		sourceNormal[i]   = polygons_.sourceNormal[i];
		
	}	


}

inline Vector_3 Polygons::getFacetsNormal(Facet_iterator facet) {


	

	return  CGAL::cross_product(facet->halfedge()->next()->vertex()->point() - facet->halfedge()->vertex()->point(),
			  				   facet->halfedge()->next()->next()->vertex()->point() - facet->halfedge()->next()->vertex()->point());

	//std::cout << CGAL::to_double( result.x() ) << " " << CGAL::to_double( result.y() ) << " " <<CGAL::to_double( result.z() ) << "\n";
	
	//return result;
	
	
}


/* this operator= assumes the polyhedron is triangulated
 */

inline void Polygons::operator=(const Polyhedron& polyhedron_)
{

	Polyhedron::Halfedge_around_facet_circulator facetCirculator, facetStart;
	Vertex::Halfedge_around_vertex_circulator halfedgeCirculator, firstHalfedge;
	
	Halfedge_handle h;
	Vector_3 zeroVector(0, 0, 0);
	Vector_3 orthogonalVector;
	Vector_3 currentNormal;

	

	polyhedron = polyhedron_;
	
	/*
	BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedron;
	
	ExactPolyhedron exactPoly;
	
	newExactPolyhedron.addPolyhedron(&polyhedron);
	exactPoly.delegate(newExactPolyhedron);
	
	nefPolyhedron = Nef_polyhedron(exactPoly);
	*/
  	
	/*
	
	if ( polyhedron.is_closed() ) {
		std::cout << "closed\n";
		try {
			Nef_polyhedron nef(polyhedron);
			nefPolyhedron = nef;		
		}
		catch (...) {
			std::cout << "an error occured\n";
		}
	}
	else {
		std::cout << "not closed\n";
		
	}
	*/

	triangleCount		= polyhedron.size_of_facets();
	sourcePositionCount = triangleCount * 3;
	sourceNormalCount	= triangleCount * 3;
	indexCount			= triangleCount * 3;
	Halfedge_handle currentHE, endHE;
	sourcePosition = new ovTuple3f[sourcePositionCount];
	sourceNormal   = new ovTuple3f[sourceNormalCount];
	
	ovInt i = 0;
	//std::cout << "triangles " << triangleCount << "\n";
	for (Facet_iterator facetIterator = polyhedron.facets_begin(); 
		 facetIterator != polyhedron.facets_end(); 
		 ++facetIterator ) {

			facetStart = facetCirculator = facetIterator->facet_begin();

			do {

				sourcePosition[i].x = CGAL::to_double( facetCirculator->vertex()->point().x() );
				sourcePosition[i].y = CGAL::to_double( facetCirculator->vertex()->point().y() );
				sourcePosition[i].z = CGAL::to_double( facetCirculator->vertex()->point().z() );
				
				// compute the normal

				
				currentNormal = Vector_3(0,0,0); 
				
				currentHE = endHE = facetCirculator->vertex()->halfedge();
				
				do {
					// magick
					orthogonalVector = currentHE->facet()->plane().orthogonal_vector();
					if (orthogonalVector != zeroVector) {
					
						currentNormal = currentNormal + currentHE->facet()->plane().orthogonal_vector();
						
					}
					else {
						orthogonalVector = getFacetsNormal(currentHE->facet());
						Plane_3 newPlane(facetCirculator->vertex()->point(), orthogonalVector);
						currentHE->facet()->plane() = newPlane;
						currentNormal = currentNormal + orthogonalVector;
						
					}
					
					
					
								
					currentHE = currentHE->next_on_vertex();
					
				} while (currentHE != endHE);
				
				
				
				currentNormal = currentNormal / CGAL::sqrt(CGAL::to_double(currentNormal.squared_length()));	
				
				
				sourceNormal[i].x = CGAL::to_double(currentNormal.x());
				sourceNormal[i].y = CGAL::to_double(currentNormal.y());
				sourceNormal[i].z = CGAL::to_double(currentNormal.z());
				

				
				//std::cout << sourceNormal[i].x << " " << sourceNormal[i].y << " " << sourceNormal[i].z << "\n";

				facetCirculator++;
				i++;

			} while ( facetCirculator != facetStart );

			
	}
	//std::cout << "vertices " << i << "\n";
	

}



#endif
