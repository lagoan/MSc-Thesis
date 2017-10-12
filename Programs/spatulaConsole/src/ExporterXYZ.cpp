/*
 *  ExporterXYZ.cpp
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 09-10-05.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include "SurfaceMesher.h"
#include "ExporterXYZ.h"
#include "ovKDtree.h"


#include <QDir>
#include <QString>
#include <QFile>

#define LENGTH 0.065*2

ExporterXYZ::ExporterXYZ()
{
	
}

ExporterXYZ::~ExporterXYZ()
{
	
}


QString ExporterXYZ::getData(ProcessedScene &scene)
{
	QString result;
	std::list<ProcessedNode*> nodes = scene.getNodes();
	std::list<ProcessedNode*>::iterator nodesIterator;
	
	QString filePath = "/home";
	
	QDir::setCurrent(filePath);
	QFile file;
	file.setFileName("file.xyz");
	file.open(QIODevice::WriteOnly);
	
	for (nodesIterator = nodes.begin(); 
		 nodesIterator != nodes.end(); 
		 ++nodesIterator) {
		
		emit( taskProgress(0) );
		
		if ((*nodesIterator)->nodeType == ProcessedNode::GeometricObjectNode &&
			((*nodesIterator)->particles.size() || 
			 (*nodesIterator)->bubbles.size() )) {
				
				
				//std::cout << "name " << (*nodesIterator)->id.toStdString() << "\n";
				
				emit( taskProgress(0) );

				std::vector<Particle> positionsWithNormals;
				std::vector<Particle>::iterator particleIterator;
				int particleCount = 0;
			
#if 1
				// kdTree with CGAL convex hull
				// create tree
                
				std::cout << "starting with tree exporting\n";
				ovKDtree kdTree;
				
				std::vector<Particle> newParticles;
				std::vector<Particle>::iterator particleIt;
				
				for (particleIt = (*nodesIterator)->particles.begin();
					 particleIt != (*nodesIterator)->particles.end();
					 ++particleIt) {
					if (particleIt->trueSurface ) {
						newParticles.push_back(*particleIt);
					}
				}
				
				for (particleIt = (*nodesIterator)->bubbles.begin();
					 particleIt != (*nodesIterator)->bubbles.end();
					 ++particleIt) {
                    if (particleIt->trueSurface ) {
						newParticles.push_back(*particleIt);
					}
				}
				
				
				kdTree.addNodes(newParticles);
                                std::cout << "tree built\n";
				//std::vector<Particle *> neighborParticles;
				std::vector<Particle *>::iterator neighborIt;
				//Vector_3 normal;
				std::map<Point_3, bool> addedPoints;
				// for each particle
				for (particleIterator = newParticles.begin(); 
					 particleIterator != newParticles.end(); 
					 ++particleIterator) {
					
						//if(particleIterator->trueSurface) {
					#if 1
						// get neighbors 
						particleIterator->neighborParticles.clear();
						kdTree.rangeSearch(particleIterator->position, 
                                           1.5 * particleIterator->smoothingLength,
										   particleIterator->neighborParticles);

						// get convexhull
						std::vector<Point_3> neighborPositions;
					
						for (neighborIt = particleIterator->neighborParticles.begin(); 
							 neighborIt != particleIterator->neighborParticles.end();
							 ++neighborIt) {
						
							neighborPositions.push_back((*neighborIt)->position);
						}

#endif
					
						// find if on true surface and add normal if so
						//if (IsPosOnSrfcByItsCGALConvexHull(particleIterator->position,
						//                                  neighborPositions,
						 //                                  particleIterator->normal)) {
							if (addedPoints.find(particleIterator->position) == addedPoints.end()/* && 
								particleIterator->normal.squared_length() > 0.01*/) { 
					
								addedPoints[particleIterator->position] = true;
								positionsWithNormals.push_back(*particleIterator);
							}
						//} 
					
					//}
					
				}	
				
                                std::cout << "almost done now\n";

				// std::vector<Vector_3> newNormals;
				// 			
				// 			for (particleIterator = positionsWithNormals.begin();
				// 				 particleIterator != positionsWithNormals.end();
				// 				 particleIterator++) {
				// 					std::vector<Particle *>::iterator particlePtrIter;
				// 					Vector_3 newNormal = particleIterator->normal;
				// 					
				// 					for (particlePtrIter = particleIterator->neighborParticles.begin();
				// 						 particlePtrIter != particleIterator->neighborParticles.end();
				// 						 ++particlePtrIter) {
				// 						
				// 						if ()
				// 						newNormal = newNormal + 
				// 									(*particlePtrIter)->normal;
				// 						
				// 					}
				// 					double len = sqrt(particleIterator->normal.squared_length());
				// 				newNormals.push_back( Vector_3(CGAL::to_double(newNormal[0]) / len,
				// 											   CGAL::to_double(newNormal[1]) / len,
				// 											   CGAL::to_double(newNormal[2]) / len) ); 				
				// 			}
				// 			int i=0;
				// 			for (particleIterator = positionsWithNormals.begin();
				// 				 particleIterator != positionsWithNormals.end();
				// 				 particleIterator++, i++) {
				// 				
				// 					particleIterator->normal = newNormals[i];	
				// 					
				// 			}
#endif
				
#if 0
				// CGAL normal estimator


				
				QHash<QString, bool> addedPoints;
				QString hashKey;
				for (particleIterator = (*nodesIterator)->particles.begin(); 
					 particleIterator != (*nodesIterator)->particles.end(); 
					 ++particleIterator) {
					
					//positionsWithNormals.push_back( PointVectorPair(particleIterator->position, Vector_3(0,1,0)));
					if(particleIterator->trueSurface) {
						
						hashKey = SurfaceMesher::getPointKey(particleIterator->position);
						
						if ( ! addedPoints.contains( hashKey ) ) {
							addedPoints[hashKey] = true;
							
							positionsWithNormals.push_back(Point_with_normal(particleIterator->position, Vector_3(0,0,0)));
						}
					}
				}
				
				addedPoints.clear();
				
				int numberPoints = positionsWithNormals.size();
				
				//std::cout << numberPoints << " number of Points\n";
				
				const int nb_neighbors = 24; // K-nearest neighbors = 3 rings
				CGAL::pca_estimate_normals(positionsWithNormals.begin(), positionsWithNormals.end(),
										   CGAL::make_normal_of_point_with_normal_pmap(positionsWithNormals.begin()),
										   nb_neighbors);
				
				
				std::list<Point_with_normal>::iterator unoriented_points_begin =
				CGAL::mst_orient_normals(positionsWithNormals.begin(), positionsWithNormals.end(),
										 CGAL::make_normal_of_point_with_normal_pmap(positionsWithNormals.begin()),
										 nb_neighbors);
				
				
				// Optional: delete points with an unoriented normal
				// if you plan to call a reconstruction algorithm that expects oriented normals.
				positionsWithNormals.erase(unoriented_points_begin, positionsWithNormals.end());
				
				// end of CGAL normal estimator
#endif
				
				
				for (particleIterator = positionsWithNormals.begin();
					 particleIterator != positionsWithNormals.end();
					 particleIterator++) {
					
					result += QString("%1 %2 %3 %4 %5 %6\n").arg(QString::number(CGAL::to_double(particleIterator->position[0])),
																 QString::number(CGAL::to_double(particleIterator->position[1])),
																 QString::number(CGAL::to_double(particleIterator->position[2])),
																 QString::number(CGAL::to_double(particleIterator->normal[0])),
																 QString::number(CGAL::to_double(particleIterator->normal[1])),
																 QString::number(CGAL::to_double(particleIterator->normal[2])));
				}		
			}
	}
	return result;
}

#if 0

bool ExporterXYZ::IsPosOnSrfcByItsCGALConvexHull(Point_3 & target,
												 std::vector<Point_3> &points,
												 Vector_3 & normal)
{
	
	
	normal = Vector_3(0.0f, 0.0f, 0.0f);
	
	CGAL::Object ch_object;
	
	// compute convex hull
	CGAL::convex_hull_3(points.begin(), points.end(), ch_object);
	
	// determine what kind of object it is
	if (CGAL::object_cast<Segment_3>(&ch_object) ) {
		std::cout << "convex hull is a segment " << std::endl;
		return true;
		
	}
	else if (CGAL::object_cast<Polyhedron>(&ch_object) ) {
		//std::cout << "convex hull is a polyhedron " << std::endl;
		// magic goes here
		
		
		
#if 1
		
		Vector_3 zeroVector(0, 0, 0);
		Vector_3 orthogonalVector;
		
		Polyhedron poly = CGAL::object_cast<Polyhedron>(ch_object);
		
		
		Polyhedron::Vertex_iterator vIt = poly.vertices_begin();
		while (vIt != poly.vertices_end()) {
			
			if (target == vIt->point()) {
				
				
				
				// get facets and calculate normal
				
				Halfedge_handle currentHE, endHE;
				currentHE = endHE = vIt->halfedge();
				
				do {
					// normal magick
					orthogonalVector = zeroVector;// currentHE->facet()->plane().orthogonal_vector();
					if (orthogonalVector != zeroVector && false) {
						
						normal = normal + orthogonalVector;
						
					}
					else {
						//orthogonalVector = Polygons::getFacetsNormal(currentHE->facet());
						
						orthogonalVector = CGAL::cross_product(currentHE->facet()->halfedge()->next()->vertex()->point()		 - currentHE->facet()->halfedge()->vertex()->point(),
															   currentHE->facet()->halfedge()->next()->next()->vertex()->point() - currentHE->facet()->halfedge()->next()->vertex()->point());
						
						
						
						
						Plane_3 newPlane(vIt->point(), orthogonalVector);
						currentHE->facet()->plane() = newPlane;
						normal = normal + orthogonalVector;
						
					}
					//// ^^^^^ turn into funtion
					currentHE = currentHE->next_on_vertex();
					
				} while (currentHE != endHE);
				
				normal = normal / CGAL::sqrt(normal.squared_length());	
				
				
				return true;
			}
			
			
			
			vIt++;
		}
		
		// if we got here it means that the point is not on the vertex
		// but could still lie on one of the facets
		
		
		Polyhedron::Facet_iterator fIt = poly.facets_begin();
#if 1		
		while (fIt != poly.facets_end()) {
			
			if (fIt->facet_degree() == 3) { // supose it is a triangle
				//	std::cout << "checking inside a triangle facet\n";
				Facet::Halfedge_around_facet_circulator	heCi = fIt->facet_begin();
				Point_3 a(heCi->vertex()->point());
				heCi++;
				Point_3 b(heCi->vertex()->point());
				heCi++;
				Point_3 c(heCi->vertex()->point());
				
				Triangle_3 triangleFacet(a,b,c);
				Plane_3 plane = triangleFacet.supporting_plane(); 
				
				Vector_3 planesOrthogonalVector = plane.orthogonal_vector();
				
				Vector_3 targetVector = target - a;
				
				// normalized
				planesOrthogonalVector = planesOrthogonalVector / CGAL::sqrt(planesOrthogonalVector.squared_length());
				targetVector = targetVector / CGAL::sqrt(targetVector.squared_length());
				
				Kernel::FT dot = planesOrthogonalVector * targetVector;
				
				if ( (dot == 0.0f) || CGAL::abs( dot ) <= 0.001 ) {
					// they are coplanar || close enough to be
					
					//std::cout << "they are coplanar || close enough to be\n";
					unsigned int x2d = 0;
					unsigned int y2d = 0;
					
					// find onto which plane to project to
					
					if (planesOrthogonalVector[0] >= planesOrthogonalVector[1] &&
						planesOrthogonalVector[0] >= planesOrthogonalVector[2] ) {
						// proyect on x	
						x2d = 1;
						y2d = 2;
						
					} else if (planesOrthogonalVector[1] >= planesOrthogonalVector[0] &&
							   planesOrthogonalVector[1] >= planesOrthogonalVector[2] ) {
						
						// proyect on y
						x2d = 0;
						y2d = 2;	
					} else {
						// proyect on z
						x2d = 0;
						y2d = 1;
					}
					
					Point_2 a2(a[x2d], a[y2d]);
					Point_2 b2(b[x2d], b[y2d]);
					Point_2 c2(c[x2d], c[y2d]);
					Point_2 target2(target[x2d], target[y2d]);
					
					//Triangle_2 triangle2(a2, b2, c2);
					
					if (IsPointOnTriangle(target2, a2, b2, c2)) {
						//std::cout << "FOUND!!!!!11!\n";
						normal = planesOrthogonalVector;
						normal = normal / CGAL::sqrt(normal.squared_length());	
						return true;
					}
					
					
				}
				
				
				
			} else {
				std::cout << "Found non trianglefacet\n";
			}
			
			
			fIt++;
		}
#endif		
		
#endif
		
	} else {
		std::cout << "convex hull error!" << std::endl;
		
	}
	
	return false;
	
}

bool ExporterXYZ::IsPointOnTriangle(Point_2 p, 
									Point_2 a, 
									Point_2 b, 
									Point_2 c)
{
	Vector_2 v0 = c - a;
	Vector_2 v1 = b - a;
	Vector_2 v2 = p - a;
	
	Kernel::FT dot00 = v0 * v0;
	Kernel::FT dot01 = v0 * v1;
	Kernel::FT dot02 = v0 * v2;
	Kernel::FT dot11 = v1 * v1;
	Kernel::FT dot12 = v1 * v2;
	
	Kernel::FT invDenom = 1.0 / (dot00 * dot11 - dot01 * dot01);
	Kernel::FT u = (dot11 * dot02 - dot01 * dot12) * invDenom;
	Kernel::FT v = (dot00 * dot12 - dot01 * dot02) * invDenom;
	
	return (u > 0) && (v > 0) && (u + v < 1);
}
#endif
