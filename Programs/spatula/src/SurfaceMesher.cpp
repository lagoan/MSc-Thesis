/*
 *  SurfaceMesher.cpp
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 14/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */


#include <omp.h>
#include <QHash>
#include <map>
#include <QPluginLoader>
#include <QObject>

#include <cmath>
#include <list>
#include <utility>

#include "SurfaceMesher.h"
#include <vcg/complex/trimesh/closest.h>





//#include "SimuMarchingCube.h"
#include "ovKDtree.h"
#include "rayTriangleIntersect.h"
// #include "Polyhedronification/AlphashapeHDSPolyhedronifikator.h"
// #include "Polyhedronification/AlphashapeHDSator.h"
// #include "Polyhedronification/AlphashapePolyhedronifikator.h"




//#include "vcg/complex/trimesh/allocate.h"
#include "vcg/complex/trimesh/update/bounding.h"
#include "vcg/complex/trimesh/inside.h"
#include "meshlab/meshmodel.h"


#if 0

using namespace std;
using namespace vcg;


typedef float ScalarType;

class MyFace;
class MyVertex;

struct MyUsedTypes : public UsedTypes< Use<MyVertex>::AsVertexType, Use<MyFace>::AsFaceType>{};
class MyVertex     : public Vertex< MyUsedTypes, vertex::Coord3f>{};
class MyFace       : public Face< MyUsedTypes, face::VertexRef, face::BitFlags> {};

//class MyVertex  : public vcg::Vertex< ScalarType, MyEdge, MyFace > {};
//class MyFace		: public vcg::Face< MyVertex, MyEdge, MyFace> {};

class MyMesh		: public vcg::tri::TriMesh< std::vector< MyVertex>, std::vector< MyFace > > {};



typedef SimpleVolume<SimpleVoxel> MyVolume;
#endif
/////


#define SMOOTH_LENGHT 0.15*2
//#define SMOOTH_LENGHT 2.0




SurfaceMesher::SurfaceMesher()
{
	//resetAlphaValues();
	// test
		
		#ifdef Q_OS_LINUX
        QPluginLoader pluginLoader("/home/omar/Workspace/MeshLabSrc_AllInc_v122/meshlab/src/meshlab/plugins/libfilter_mls.so");
        QPluginLoader mesherPluginLoader("/home/omar/Workspace/MeshLabSrc_AllInc_v122/meshlab/src/meshlab/plugins/libfilter_meshing.so");
        #else
		QPluginLoader pluginLoader("/Users/kuervo/Workspace/MeshLabSrc_AllInc_v122/meshlab/src/meshlabplugins/filter_mls/build/Release/libfilter_mls.dylib");
		QPluginLoader mesherPluginLoader("/Users/kuervo/Workspace/MeshLabSrc_AllInc_v122/meshlab/src/meshlabplugins/filter_meshing/build/Release/libfilter_meshing.dylib");
        #endif
		//QPluginLoader pluginLoader("/Users/kuervo/Workspace/MeshLabSrc_AllInc_v122/meshlab/src/meshlabplugins/filter_poisson/build/Release/libfilter_poisson.dylib");
	
	pluginLoader.load();
	mesherPluginLoader.load();
	
	QObject * plugin = pluginLoader.instance();
	QObject * mesherPlugin = mesherPluginLoader.instance();
	
	if (pluginLoader.isLoaded()) {
		std::cout << "plugin loader failed succesfull\n";
	} else {
		std::cout << "plugin loader failed\n";
	}
	if (mesherPluginLoader.isLoaded()) {
		std::cout << "plugin loader failed succesfull\n";
	} else {
		std::cout << "plugin loader failed\n";
	}	
	surfaceFilter = qobject_cast<MeshFilterInterface *>(plugin);
	meshFilter = qobject_cast<MeshFilterInterface *>(mesherPlugin);
	
	
	__cellSizeSet = false;
	


	
}

SurfaceMesher::~SurfaceMesher()
{
	
}


void SurfaceMesher::resetAlphaValues()
{
	__alphaSet = false;
	__alpha = 0;
}

			
uint SurfaceMesher::getEdgeIndex(std::vector< Halfedge * > &edgesList, 
								 Halfedge *edge)
{
	
	uint i = 0;
 
	for (std::vector< Halfedge* >::iterator edgeIterator = edgesList.begin(); 
		 edgeIterator != edgesList.end(); 
		 ++edgeIterator, i++) {
		
		 		
		if ((*edgeIterator) == edge) {
			return i;
		}
		
	}
	return 0;
}



void SurfaceMesher::getMesh(ProcessedScene &scene, MeshDocument & meshDocument)
{
	std::list<ProcessedNode*> nodes = scene.getNodes();
	std::list<ProcessedNode*>::iterator nodesIterator;
	Polyhedron batterVolume;
	//BuildPolyhedronFromCMesh<HalfedgeDS> newPolyhedronFromCMesh;


	MyVolume volume;
	MyVolume smoothedVolume;
	typedef vcg::tri::TrivialWalker<CMesh,MyVolume>	MyWalker;
	typedef vcg::tri::MarchingCubes<CMesh, MyWalker>	MyMarchingCubes;
	//typedef vcg::tri::MarchingCubes<CMesh, MyWalker>	MyMarchingCubes;
	MyWalker walker;

	
	
	CMesh mc_mesh;
	
	// get negative space
	
	// get cutouts
	

	 
	for (nodesIterator = nodes.begin(); 
		 nodesIterator != nodes.end(); 
		 ++nodesIterator) {
		
		emit( taskProgress(0) );
		
		if ((*nodesIterator)->nodeType == ProcessedNode::GeometricObjectNode &&
			((*nodesIterator)->particles.size() || 
			 (*nodesIterator)->bubbles.size() )) {
				
				
				
				emit( taskProgress(0) );
				
				std::vector<Particle> positionsWithNormals;
				std::vector<Particle>::iterator particleIterator;
				
				

				// kdTree with CGAL convex hull
				// create tree
				std::cout << "starting with tree\n";
				ovKDtree kdTree;
				MyBox myBox;

				std::vector<Particle> newParticles;
				std::vector<Particle>::iterator particleIt;
				
				double avgDensity = 0;
		
				

				// should check for every scene if particles are outside a range
				// and cut it off after a threshold
				for (particleIt = (*nodesIterator)->particles.begin();
					 particleIt != (*nodesIterator)->particles.end();
					 ++particleIt) {
                 //	if (particleIt->trueSurface ) {
					if (particleIt->position[1] > -5){
						avgDensity += particleIt->density;
						newParticles.push_back(*particleIt);
						myBox.Add(Point3<ScalarType>( particleIt->position[0], 
						 							  particleIt->position[1],
													  particleIt->position[2] ));
					}
				}

				for (particleIt = (*nodesIterator)->bubbles.begin();
					 particleIt != (*nodesIterator)->bubbles.end();
					 ++particleIt) {
                	//if (particleIt->trueSurface ) {
					if (particleIt->position[1] > -5){

						avgDensity += particleIt->density;
						newParticles.push_back(*particleIt);
						myBox.Add(Point3<ScalarType>( particleIt->position[0], 
						 							  particleIt->position[1],
													  particleIt->position[2] ));
					}
				}
				
				avgDensity /= ((*nodesIterator)->bubbles.size() + (*nodesIterator)->particles.size());
				std::cout << "HERE\n";
				//std::cout << X << "\n";
//				myBox.max.X();
				kdTree.addNodes(newParticles);
				
 				if (!__cellSizeSet) {
					__cellSizeSet = true;
					//__cellSize = (myBox.max.X() - myBox.min.X()) / 200.0;
					//std::cout << "__cellSizeSet " << __cellSizeSet << "\n";
					//__cellSize = .10 * .3;
					__cellSize = 0.03;
					__cellSize = 0.05;
					__cellSize = 0.04;
				}			
				
				
				double isoDensityRadius = 0.15; // max value
				double isoThreshold = .3;
				
				isoDensityRadius = .3;
				isoThreshold = .2;

				
				double isoDensityRatio = 0.;//0.1;//8;//64.0;//16.0;//8.0;//4.0; 
				std::cout << "__cellSize " << __cellSize << "\n";
				double offset = 25; 
				// set dimention and add discrete offset 
				int xGridD = ceil((myBox.max.X() - myBox.min.X()) / __cellSize) + offset;
				int yGridD = ceil((myBox.max.Y() - myBox.min.Y()) / __cellSize) + offset;				
				int zGridD = ceil((myBox.max.Z() - myBox.min.Z()) / __cellSize) + offset;
				
				std::cout << "xGridD " << xGridD << "\n";
				std::cout << "yGridD " << yGridD << "\n";
				std::cout << "zGridD " << zGridD << "\n";
				int gridD[3] = {xGridD, yGridD, zGridD};
				
				m_pfMarchingCube.CreateGridDataStructure(xGridD * yGridD * zGridD);
				
			
				bool *** negativeSpace;
			
				std::vector< std::pair<VoxelCoord, double> > toAverageValues;
				
				#ifdef NEGATIVE
				negativeSpace = (bool ***) malloc(xGridD * sizeof(bool **));
				for(int r=0;r<xGridD;r++) {
					negativeSpace[r] = (bool **) malloc(yGridD * sizeof(bool *));
					#pragma omp parallel for
					for(int q=0;q<yGridD;q++){
						negativeSpace[r][q] = (bool *) malloc(zGridD * sizeof(bool));
						for(int p=0;p<zGridD;p++){
							negativeSpace[r][q][p] = false;
						}
					}
				}
				#endif
				//for (uint i=0; i< scene.cutOuts.size(); i++ ) {
				//	std::cout << "ith cutout " << i << "\n";
				//	
				//}
				
				int numPoints = 0;
				volume.Init(Point3i(xGridD,yGridD,zGridD));
#if 0				
				#pragma omp parallel for
				for(int r=0;r<xGridD;r++) {
					#pragma omp parallel for
				   	for(int q=0;q<yGridD;q++){
						#pragma omp parallel for
				     	for(int p=0;p<zGridD;p++){
							volume.Val(r,q,p) = 0;
							
							Point_3 currentPoint (r * __cellSize + myBox.min.X() - offset/2 * __cellSize,
												  q * __cellSize + myBox.min.Y() - offset/2 * __cellSize,
												  p * __cellSize + myBox.min.Z() - offset/2 * __cellSize);
												
							
							#ifdef NEGATIVE				
							for (uint i=0; i< scene.cutOuts.size(); i++ ) {
								if (!negativeSpace[r][q][p] && isInside(currentPoint, scene.cutOuts[i])) {
									//QString key = QString::number(r) + " " + QString::number(p) + " " + QString::number(q);
									//#pragma omp atomic
									//negativeSpace[key] = true;
									//negativeSpace[VoxelCoord(r,p,q)] = true;
									negativeSpace[r][q][p] = true;	
									i=scene.cutOuts.size(); // break
								}
							}			
							#endif		
						}
					}
				}


				double maxGridVal = 0;
				#pragma omp parallel for
				for (int i=0; i<newParticles.size();i++) {
					int localI = (newParticles[i].position.x() + (offset/2 * __cellSize) -  myBox.min.X()) / __cellSize;
					int localJ = (newParticles[i].position.y() + (offset/2 * __cellSize) -  myBox.min.Y()) / __cellSize;
					int localK = (newParticles[i].position.z() + (offset/2 * __cellSize) -  myBox.min.Z()) / __cellSize;
					//VoxelCoord currentCenter(localI, localJ, localK);
					 
					
					
					//negativeSpaceIterator = find(negativeSpace.begin(), negativeSpace.end(), currentCenter);
					
					double currentRadius = newParticles[i].originalDensity / newParticles[i].density;
					currentRadius = .2 + (currentRadius * .05);
					if (currentRadius < 0.2) currentRadius = 0.2;
					currentRadius = isoDensityRadius;
					
					int radiusLength = ceil(currentRadius / __cellSize);

					Point_3 newPositionMin(localI - radiusLength, localJ - radiusLength, localK - radiusLength);
					Point_3 newPositionMax(localI + radiusLength, localJ + radiusLength, localK + radiusLength);

					if (newPositionMin.x() < 0) newPositionMin.x() = 0;
					if (newPositionMin.y() < 0) newPositionMin.y() = 0;
					if (newPositionMin.z() < 0) newPositionMin.z() = 0;

					if (newPositionMax.x() > xGridD) newPositionMin.x() = xGridD;
					if (newPositionMax.y() > yGridD) newPositionMin.y() = yGridD;
					if (newPositionMax.z() > zGridD) newPositionMin.z() = zGridD;

					#pragma omp parallel for
					for (int p=newPositionMin.x(); p<newPositionMax.x(); ++p) {
						#pragma omp parallel for
						for (int q=newPositionMin.y(); q<newPositionMax.y(); ++q) {
							#pragma omp parallel for
							for (int r=newPositionMin.z(); r<newPositionMax.z(); ++r) {


								//std::vector<VoxelCoord>::iterator negativeSpaceIterator;
								VoxelCoord currentVoxel(p, q, r);
								#pragma omp atomic
								//negativeSpaceIterator = find(negativeSpace.begin(), negativeSpace.end(), currentVoxel);
								//negativeSpaceIterator = negativeSpace.end();
								 
								QString key = QString::number(p) + " " + QString::number(q) + " " + QString::number(r);
								
								
									
								Point_3 currentPoint(p * __cellSize + myBox.min.X() - offset/2 * __cellSize,
								 			   		 q * __cellSize + myBox.min.Y() - offset/2 * __cellSize,
											   		 r * __cellSize + myBox.min.Z() - offset/2 * __cellSize);
								double distance = newParticles[i].position.getDistanceTo(currentPoint);
								//std::cout << "distance " << distance << "\n";

								if (distance <= currentRadius) {
									//volume.Val(r,q,p) = 1.0;
									double ron = distance / currentRadius;
									double add =  pow(1.0 - (ron*ron), 2.0); 
									
									#ifdef NEGATIVE
									if (!negativeSpace[p][q][r]) {	
									
										#pragma omp atomic
										volume.Val(p,q,r) += add;

									} else {
										toAverageValues.push_back( std::pair<VoxelCoord, double>(currentVoxel, 0) );
									
										
									}
									#else
									#pragma omp atomic
									volume.Val(p,q,r) += add;
									#endif
								}
							}
						}	
					}
				}
				//std::cout <<maxGridVal << "\n";
				// get values
				//#pragma omp parallel for
				#if 1
				for (int p =0; p< toAverageValues.size(); p++) {
					double currentValue = 0;
					for (int i=toAverageValues[p].first.i-1; i<toAverageValues[p].first.i+1; ++i) {
						for (int j=toAverageValues[p].first.j-1; j<toAverageValues[p].first.j+1; ++j) {
							for (int k=toAverageValues[p].first.k-1; k<toAverageValues[p].first.k+1; ++k) {
								currentValue += volume.Val(i,j,k);
							}
						}
						
					}
					toAverageValues[p].second = currentValue / 26.0; // 3*3-1 center is not taken into account
				}
				// apply values
				#pragma omp parallel for
				for (int p =0; p< toAverageValues.size(); p++) {
					if (toAverageValues[p].second > 0)
					volume.Val(toAverageValues[p].first.i, toAverageValues[p].first.j, toAverageValues[p].first.k) = 0.01;//toAverageValues[p].second; 
				}
				#endif
				
				// for (avrgValIt = toAverageValues.begin();
				// 					 avrgValIt != toAverageValues.end(); 
				// 					 ++avrgValIt) {
				// 					
				// 					
				// 						avrgValIt->second = 0;
				// 					
				// 				}
				// 				// apply values
				// 				for (avrgValIt = toAverageValues.begin(); 
				// 					 avrgValIt != toAverageValues.end(); 
				// 					 ++avrgValIt) {
				// 					
				// 						volume.Val(avrgValIt->first.i,avrgValIt->first.j,avrgValIt->first.k) = avrgValIt->second;	
				// 				}
				
				#ifdef NEGATIVE
				for(int r=0;r<xGridD;r++) {
					#pragma omp parallel for
					for(int q=0;q<yGridD;q++){
						free(negativeSpace[r][q]);
					}
					free(negativeSpace[r]);
				}
				free(negativeSpace);
				#endif
				#if 0
				#pragma omp parallel for
				for(int p=0;p<zGridD;p++){
					#pragma omp parallel for
				   	for(int q=0;q<yGridD;q++){
						#pragma omp parallel for
				     	for(int r=0;r<xGridD;r++) {
							//volume.Val(r,q,p) /= maxGridVal;
							volume.Val(r,q,p) /= 6;
							//if (volume.Val(k,j,i) >1) std::cout <<volume.Val(k,j,i)<<"\n";
						}
					}
				}
				#endif
#endif
				
				#if 1
				//smoothedVolume.Init(Point3i(xGridD,yGridD,zGridD));
				
				 // Point_3 center(xGridD/2.0 * __cellSize + myBox.min.X() - offset/2 * __cellSize ,
				 // 				  			   yGridD/2.0 * __cellSize + myBox.min.Y() - offset/2 * __cellSize ,
				 // 				 			   zGridD/2.0 * __cellSize + myBox.min.Z() - offset/2 * __cellSize );
				

				std::cout << "avgDensity " << avgDensity << "\n";	
				double maxValue = 0;


// create scalar field from cells searching particles
				double maxGridVal = 0;
 #pragma omp parallel for
				for(int i=0;i<zGridD;i++){
 #pragma omp parallel for
			    	for(int j=0;j<yGridD;j++){
 #pragma omp parallel for
			      		for(int k=0;k<xGridD;k++) {
							std::vector<Particle *> range;
							CMesh::CoordType pTest(k * __cellSize + myBox.min.X() - offset/2 * __cellSize,
												   j * __cellSize + myBox.min.Y() - offset/2 * __cellSize,
												   i * __cellSize + myBox.min.Z() - offset/2 * __cellSize);
							
							std::vector<Particle *>::iterator rangeIterator;
							#if 0
							if (scene.voids.size() && scene.grids.size() ) {			
								
								//if (isInside(scene.voids[0], scene.grids[0], pTest)) {
								//if ( vcg::tri::Inside<StaticPtrGrid, CMesh>::Is_Inside( *(scene.voids[0]), 
								//													   scene.grids[0], 
								//													   pTest)) {
								//	volume.Val(k,j,i) = 0;
								//	continue;
								//}
							} else {
								std::cout << "no void\n";
							}
							#endif
							//range.clear();
							
						

							Point_3 newPos(k * __cellSize + myBox.min.X() - offset/2 * __cellSize,
							 			   j * __cellSize + myBox.min.Y() - offset/2 * __cellSize,
										   i * __cellSize + myBox.min.Z() - offset/2 * __cellSize);
 										
							kdTree.rangeSearch(newPos, isoDensityRadius, range);
						
							
						
							//double ovenSizeModifier = 1000.0f;
							bool isNearFound = false;
							double prtlsDensity = 0;
							int prtlCount = 0;
							//bool trueSurfaceDetected = false;
							//std::vector<Particle *> surfaceParticles;
							for (rangeIterator = range.begin();
								 rangeIterator != range.end();
								 ++rangeIterator) {
									//std::cout << (*rangeIterator)->smoothingLength <<"\n";		
								
									


									double currentDistance = newPos.getDistanceTo((*rangeIterator)->position);
									//double currentDistance = 0;
									//if (currentDistance <= (*rangeIterator)->smoothingLength ) {
									isNearFound =true;
									//prtlCount++;
								
									//if ((*rangeIterator)->trueSurface) {
										//if (currentDistance <= isoDensityRadius / 2.) {
									//		surfaceParticles.push_back((*rangeIterator));
										//}
									//}
									
								
								
									//double currentDelimiter = ((*rangeIterator)->temperature - 26.0) / (130.0-26.0);


#if 0									
									double currentDelimiter = (*rangeIterator)->originalDensity / (*rangeIterator)->density;
									// std::cout << "(*rangeIterator)->originalDensity " << (*rangeIterator)->originalDensity << "\n";
									// 								std::cout << "(*rangeIterator)->density " << (*rangeIterator)->density << "\n";
									// 								std::cout << "currentDelimiter " << currentDelimiter << "\n";
									//currentDelimiter = .15 + (currentDelimiter * .05);
									currentDelimiter = .2 + (currentDelimiter * .2);
									//currentDelimiter = .1;
									//std::cout << currentDistance << " " << currentDelimiter << "\n";
									if (currentDistance <= currentDelimiter) {
										//prtlsDensity += pow(1.0 - (currentDistance / (*rangeIterator)->smoothingLength), 2.0);
										//double r = currentDistance / currentDelimiter;
										//double r = currentDistance / isoDensityRadius;
										double r = currentDistance / currentDelimiter;
										prtlsDensity += pow(1.0 - (r*r), 2.0);
									}
#endif

									// for non varying smoothing length
									
									// wyvill brothers soft bodies
									double b = isoDensityRadius;
									double a = 1.0;
									double result = 0;
									if (currentDistance < b) {
										result = a * (1.0 - ((4 * pow(currentDistance,6.0))/(9 * pow(b,6.0))) + 
															((17 * pow(currentDistance,4.0))/(9 * pow(b,4.0))) - 
															((22 * pow(currentDistance,2.0))/(9 * pow(b,2.0))));
									}
									
									prtlsDensity += result;
									#if 0
									double b = isoDensityRadius;
									double a = 1.0;
									double result = 0;
									if ( currentDistance >= 0 && currentDistance <= b/3.0) {
										result = a * (1.0 - ((3 * pow(currentDistance, 2))/(b*b)));
									} else if (currentDistance > b/3.0 && currentDistance<= b) {
										result = (3.0*a/2.0) * pow((1.0 - currentDistance/b), 2);
									} else if ( b <= currentDistance) {
										result = 0;
									}
									prtlsDensity += result;
									#endif
									
								//}
							
							}
							
						//	prtlsDensity = sqrt(prtlsDensity);
							
							// data
						
						//	double gridVal = prtlsDensity - isoDensityRatio ;//(prtlsDensity / avgDensity) - isoDensityRatio;
						//	double gridVal = (prtlsDensity / avgDensity) - isoDensityRatio;
							double gridVal = prtlsDensity;
							//bool insideFluid = true;
					
													// 	
													// if (range.size()) {
													// 	gridVal /= range.size();
													// 	gridVal /= pow(1.0 - (.2*.2), 2.0);
													// } else {
													// 	gridVal = 0;
													// }
													// 						
							
						//	if (insideFluid) {
						volume.Val(k,j,i) = gridVal;
								
						//#pragma opm atomic
						//if (gridVal>maxGridVal) maxGridVal = gridVal;
						//	} else {
						//		volume.Val(k,j,i) = 0.;
						//	}
							
						//	if (volume.Val(k,j,i) != 0)
						//	std::cout << volume.Val(k,j,i) << "\n";
							
						
			 	  		}	
					}
				}
			// normalize	
			/*
			 #pragma omp parallel for
							for(int i=0;i<zGridD;i++){
			 #pragma omp parallel for
						    	for(int j=0;j<yGridD;j++){
			 #pragma omp parallel for
						      		for(int k=0;k<xGridD;k++) {
										volume.Val(k,j,i) /= maxGridVal;
										//if (volume.Val(k,j,i) >1) std::cout <<volume.Val(k,j,i)<<"\n";
									}
								}
							}
			*/	
#endif
			



				//for (int s = 0; s < 8; s++) {
				smoothGrid(1,xGridD,yGridD,zGridD,true,volume);
				//smoothGrid(1,xGridD,yGridD,zGridD,false,volume);
				//}
				CMesh mc_mesh;
			
			
				MyMarchingCubes mc(mc_mesh, walker);
				mc_mesh.vert.EnableVFAdjacency();				
				mc_mesh.face.EnableVFAdjacency();
				mc_mesh.face.EnableFFAdjacency();
				
				// walker.BuildMesh<MyMarchingCubes>(mc_mesh, volume, mc, 300.);//.1); // 100 100
				walker.BuildMesh<MyMarchingCubes>(mc_mesh, volume, mc, isoThreshold);//.1); // 100 100

				//std::cout << "de marching " << mc_mesh.vn << "\n";
				
				/// append



			
				MeshModel *mcMesh = new MeshModel;

				vcg::tri::Append<CMeshO, CMesh>::Mesh(mcMesh->cm, mc_mesh);
	
				// scale and translate
#if 1
#pragma omp parallel for
				for( int p=0; p < mcMesh->cm.vert.size(); ++p) {
					mcMesh->cm.vert[p].P().X() = mcMesh->cm.vert[p].P().X() * __cellSize + myBox.min.X() - offset/2 * __cellSize;
					mcMesh->cm.vert[p].P().Y() = mcMesh->cm.vert[p].P().Y() * __cellSize + myBox.min.Y() - offset/2 * __cellSize;
					mcMesh->cm.vert[p].P().Z() = mcMesh->cm.vert[p].P().Z() * __cellSize + myBox.min.Z() - offset/2 * __cellSize;
				}
#endif
#if 0
				CMeshO::VertexIterator vi;
				for (vi = mcMesh->cm.vert.begin();
					 vi != mcMesh->cm.vert.end();
					 ++vi) {

						vi->P().X() = vi->P().X() * __cellSize + myBox.min.X() - offset/2 * __cellSize;//- offset * __cellSize + __cellSize / 2.0;
						vi->P().Y() = vi->P().Y() * __cellSize + myBox.min.Y() - offset/2 * __cellSize;//- offset * __cellSize + __cellSize / 2.0;
						vi->P().Z() = vi->P().Z() * __cellSize + myBox.min.Z() - offset/2 * __cellSize;//- offset * __cellSize + __cellSize / 2.0;
				} 
#endif			


				
				//delete mcMesh
				(*nodesIterator)->newSurfaces.push_back( mcMesh );				
				
			
		


			}
	}
	
	
	
	 
	
}


#if 0
void SurfaceMesher::getMesh(ProcessedScene &scene)
{
	std::list<ProcessedNode*> nodes = scene.getNodes();
	std::list<ProcessedNode*>::iterator nodesIterator;
	
	// Poisson options
    //FT sm_angle = 20.0; // Min triangle angle in degrees.
    //FT sm_radius = 0.03; // Max triangle size w.r.t. point set radius.
    //FT sm_distance = 0.003; // Surface approximation error w.r.t. p.s.r.

	FT sm_angle = 30.0; // Min triangle angle in degrees. 20 fast - 30 guarantees convergence
    FT sm_radius = 0.1; // Max triangle size w.r.t. point set radius.
    FT sm_distance = 0.002; // Surface approximation error w.r.t. p.s.r.
	
	
	
		
	
	//FT sm_radius = 10.0; // Max triangle size w.r.t. point set radius.
	//FT sm_distance = 1; // Surface approximation error w.r.t. p.s.r.
	
	
	for (nodesIterator = nodes.begin(); 
		 nodesIterator != nodes.end(); 
		 ++nodesIterator) {
		
		emit( taskProgress(0) );
		
		if ((*nodesIterator)->nodeType == ProcessedNode::GeometricObjectNode &&
			((*nodesIterator)->particles.size() || 
			 (*nodesIterator)->bubbles.size() )) {
			
			
			//std::cout << "name " << (*nodesIterator)->id.toStdString() << "\n";
			
			emit( taskProgress(0) );
		/*	
			Point_3 p( 0.0, 0.0, 0.0);
			Point_3 q( 1.0, 0.0, 0.0);
			Point_3 r( 0.0, 1.0, 0.0);
			Point_3 s( 0.0, 0.0, 1.0);
		*/	
			
			
			/////////////////////////////////////


				MeshModel * meshModel = meshDocument.addNewMesh("vertexAndNormals");	
			
				if (meshModel) {
					std::cout << " AOK\n";
				} else {
					std::cout << " null or something \n";
				}
				//meshModel->cm.vert.EnableNormal();
				meshModel->cm.Clear();

				
				
				
				vcg::tri::Allocator<CMesh>::AddVertices(meshModel->cm,3);
				
				CMesh::VertexIterator vi = meshModel->cm.vert.begin();
				
				vi->P() = CMesh::CoordType( 1.0, 1.0, 1.0);
				vi->N() = vcg::Point3f(1.0,0.0,0.0); 
				++vi;
				vi->P() = CMesh::CoordType(-1.0, 1.0,-1.0); 
				vi->N() = vcg::Point3f(0.0,1.0,0.0);
				++vi;
				vi->P() = CMesh::CoordType(-1.0,-1.0, 1.0); 
				vi->N() = vcg::Point3f(0.0,0.0,1.0);
				++vi;
				 
				std::cout << " vertex number " << meshModel->cm.vn << "\n";
				
				vi = meshModel->cm.vert.begin();
				std::cout << " vertex 0 normal " << vi->N()[0] << " " <<
													vi->N()[1] << " " <<
													vi->N()[2] << " " << "\n";
				//meshModel->cm	
				
				//meshModel->cm.face
				
				
				if ( meshDocument.delMesh(meshModel) ){
					std::cout << " document deleted succesfully\n";
					
				} else {
					std::cout << " HUH?\n";
				}

			/////////////////////////////////////	
				
			
			Polyhedron batterVolume;
			
			batterVolume.make_tetrahedron( p, q, r, s);
			
			std::list<Point_with_normal> positionsWithNormals;
			std::vector<Particle>::iterator particleIterator;
			
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
			
			const int nb_neighbors = 24; //24; // K-nearest neighbors = 3 rings
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
			
			
			// simplifying
				/*
			double cell_size = 0.01;
			positionsWithNormals.erase(CGAL::grid_simplify_point_set(positionsWithNormals.begin(), 
																	 positionsWithNormals.end(), 
																	 cell_size),
									   positionsWithNormals.end());
			
			std::cout << " done simplifying\n";
			
			// outlier removal
			
			const double removed_percentage = 5.0; // percentage of points to remove

			positionsWithNormals.erase(CGAL::remove_outliers(positionsWithNormals.begin(), 
															 positionsWithNormals.end(),
															 CGAL::Dereference_property_map<Point_3>(),
															 nb_neighbors, 
															 removed_percentage), 
									   positionsWithNormals.end());
			
			std::cout << " done outlier removal\n";
			*/
			
			
			// smoothing 
			
			//CGAL::jet_smooth_point_set(positionsWithNormals.begin(), positionsWithNormals.end(), nb_neighbors);
			
			
			//std::cout << " done smoothing\n";
			
			
			// Optional: after erase(), use Scott Meyer's "swap trick" to trim excess capacity
			//std::list<Point_with_normal>(positionsWithNormals).swap(positionsWithNormals);
			
			
			// print out the xyz file
	/*

			for (std::list<Point_with_normal>::iterator pnIt = positionsWithNormals.begin();
				 pnIt != positionsWithNormals.end();
				 pnIt++) {
				
				pnIt->normal();
				std::cout << pnIt->position()[0] << " " <<
							 pnIt->position()[1] << " " <<
							 pnIt->position()[2] << " " <<
							 pnIt->normal()[0] << " " <<
							 pnIt->normal()[1] << " " <<
							 pnIt->normal()[2] <<
							 std::endl;
			}		
	*/	
			// end of printing
			
			
			
			
		//	positionsWithNormals.clear();

			Poisson_reconstruction_function function(positionsWithNormals.begin(), positionsWithNormals.end(),
													 CGAL::make_normal_of_point_with_normal_pmap(positionsWithNormals.begin()));
			
			
			
			
			positionsWithNormals.clear();
			
			if ( ! function.compute_implicit_function() ) {
				std::cout << " something went wrong\n";
			} else {
				std::cout << " function AOK\n";
			}
			
			// Gets one point inside the implicit surface
			// and computes implicit function bounding sphere radius.
			Point_3 inner_point = function.get_inner_point();
			
			FT inner_point_value = function(inner_point);
			if(inner_point_value >= 0.0)
			{
				std::cout << "Error: unable to seed (" << inner_point_value << " at inner_point)" << std::endl;
			}
			
			
			Sphere bsphere = function.bounding_sphere();
			FT radius = std::sqrt(bsphere.squared_radius());
			std::cout << radius << " radius\n";
			
			FT sm_sphere_radius =  radius + std::sqrt(CGAL::squared_distance(bsphere.center(),inner_point));
			sm_sphere_radius *= 1.01; // make sure that the bounding sphere contains the surface

			
			FT sm_dichotomy_error = sm_distance/10.0; // Dichotomy error must be << sm_distance
			Surface_3 surface(function,
							  Sphere(inner_point,sm_sphere_radius*sm_sphere_radius),
							  sm_dichotomy_error);
			std::cout << " done surface\n";
			// Defines surface mesh generation criteria
			CGAL::Surface_mesh_default_criteria_3<STr> criteria(sm_angle,  // Min triangle angle (degrees)
																sm_radius*radius,  // Max triangle size
																sm_distance*radius); // Approximation error
			std::cout << " done criteria\n";
			// Generates surface mesh with manifold option
			STr tr; // 3D Delaunay triangulation for surface mesh generation
			C2t3 c2t3(tr); // 2D complex in 3D Delaunay triangulation
			CGAL::make_surface_mesh(c2t3,                  // reconstructed mesh
									surface,               // implicit surface
									criteria,              // meshing criteria
									//CGAL::Manifold_with_boundary_tag()); // require manifold mesh with no boundary
									CGAL::Manifold_tag()); // require manifold mesh with no boundary
									//CGAL::Non_manifold_tag());
			
			std::cout << tr.number_of_vertices() << " vertices\n";
			
			if (CGAL::output_surface_facets_to_polyhedron(c2t3, batterVolume) ) {
				std::cout << "yeah!\n";
			}

			
		
			
			//CGAL::Subdivision_method_3::Loop_subdivision(batterVolume, 1);
			//CGAL::Subdivision_method_3::CatmullClark_subdivision(batterVolume);
			
			//CGAL::Subdivision_method_3::PTQ(batterVolume, 
			//								WLoop_mask_3<Polyhedron>(), 1);

			
			(*nodesIterator)->newSurfaces.push_back( batterVolume );
			
			emit( taskProgress(100) );
		
		}
		
		emit( taskProgress(100) );
	}
}
#endif
bool SurfaceMesher::isInside(Point_3 p, CutOutInformation cutOut) {

	double direction[3] = {0,1,0};
	double u,v,t;
	int intersectionCount = 0;
	double orig[3] = {p.x(), p.y(), p.z()};
	//std::cout << "facets size " << cutOut.facets.size() << "\n";
	//std::cout << "vertex size " << cutOut.vertex.size() << "\n";
	for (int i=0; i<cutOut.facets.size(); ++i) {

		// assume all facets are triangles
		double a[3] = { cutOut.vertex[cutOut.facets[i][0]].x(), 
			  			cutOut.vertex[cutOut.facets[i][0]].y(), 
						cutOut.vertex[cutOut.facets[i][0]].z() };
		double b[3] = { cutOut.vertex[cutOut.facets[i][1]].x(), 
			  			cutOut.vertex[cutOut.facets[i][1]].y(), 
						cutOut.vertex[cutOut.facets[i][1]].z() };
		double c[3] = { cutOut.vertex[cutOut.facets[i][2]].x(), 
			  			cutOut.vertex[cutOut.facets[i][2]].y(), 
						cutOut.vertex[cutOut.facets[i][2]].z() };
						
		int intersectionTest = intersectsRayTriangle(orig, direction, a, b, c, &t, &u, &v);
					                   
			
		if (intersectionTest == 1 && t > 0) {
			++intersectionCount;
		}
		
		
	}

	bool result  = intersectionCount % 2 == 0;
	
	//if (!result) std::cout << "found outside\n";
	//else std::cout << "found inside\n";
	return result;
}

#if 0 // old copy
void SurfaceMesher::getMesh(ProcessedScene &scene)
{
	
	std::list<ProcessedNode*> nodes = scene.getNodes();
	std::list<ProcessedNode*>::iterator nodesIterator;

	BuildPolyhedronFromAShape<HalfedgeDS> newPolyhedronFromAShape;
	//BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedronFromPolyhedron;
	BuildPolyhedronFromCutOut<HalfedgeDS> newPolyhedronFromCutOut;
	
	
	CSimuMarchingCube marchingCubes;
	

	
	// get cutouts
	
	std::vector< Polyhedron > cutouts;

 
	for (uint i=0; i< scene.cutOuts.size(); i++ ) {
		
		// should cut down the steps
		
		Polyhedron newCut;
		newPolyhedronFromCutOut.addCutOut( &( scene.cutOuts[i] ) );
		newCut.delegate( newPolyhedronFromCutOut );
		
		cutouts.push_back(newCut);
		
	}
 
	

	
	// create scaling transformation to be able to do skin meshing for the bubbles
	
	double scalingFactor = 100.0f;
	
	Aff_transformation_3 skinScale(scalingFactor, 0, 0,
									0, scalingFactor, 0,
									0, 0, scalingFactor);
	
	
	for (nodesIterator = nodes.begin(); 
		 nodesIterator != nodes.end(); 
		 ++nodesIterator) {
		
		emit( taskProgress(0) );
		
		if ((*nodesIterator)->nodeType == ProcessedNode::GeometricObjectNode &&
			((*nodesIterator)->particles.size() || 
			 (*nodesIterator)->bubbles.size() )) {
			

			//std::cout << "name " << (*nodesIterator)->id.toStdString() << "\n";
			
			emit( taskProgress(0) );
			
			Polyhedron batterVolume;
			
			
			
			
			//ExactPolyhedron exactPolyhedron;
			//BuildExactPolyhedronFromPolyhedron<ExactHalfedgeDS> newExactPolyhedron;
			
			//Nef_polyhedron bubbleStructure;
			Polyhedron bubbleStructure;
			std::list<Weighted_point> weighetPointList;
			
			std::vector<Particle>::iterator particleIterator;

			// XXX should check that points are only added once !!
			
			//std::cout<< "starting\n";
			
			QHash<QString, bool> addedPoints;
			QString hashKey;
				

				
				
#if 0
#if USE_DELAUNAY_HIERARCHY
			
			Delaunay_hierarchy delaunay;
			
			for (particleIterator = (*nodesIterator)->particles.begin(); 
				 particleIterator != (*nodesIterator)->particles.end(); 
				 particleIterator++) {
				
				delaunay.insert( particleIterator->position );
				
			}
			
			emit( taskProgress(10) );
			
			
			
			for (particleIterator = (*nodesIterator)->bubbles.begin(); 
				 particleIterator != (*nodesIterator)->bubbles.end(); 
				 particleIterator++) {
				delaunay.insert( particleIterator->position );
				//weighetPointList.push_back( Weighted_point( particleIterator->position, 0.001) );
				
			}
			
			emit( taskProgress(20) );
			
			Alpha_shape_3 as(delaunay);
#else
			
			/*
			
			jointPosition.insert( jointPosition.begin(), (*nodesIterator)->particlePosition.begin(), (*nodesIterator)->particlePosition.end() );
			emit( taskProgress(10) );
			jointPosition.insert( jointPosition.begin(), (*nodesIterator)->bubblePosition.begin(), (*nodesIterator)->bubblePosition.end() );
			*/
			
			//sstd::cout << "before\n";
			
			std::list< Weighted_point > jointPosition;
			for (particleIterator = (*nodesIterator)->particles.begin(); 
				 particleIterator != (*nodesIterator)->particles.end(); 
				 ++particleIterator) {
				
				hashKey = SurfaceMesher::getPointKey(particleIterator->position);
				
				if ( ! addedPoints.contains( hashKey ) ) {
					addedPoints[hashKey] = true;
					jointPosition.push_back( particleIterator->position );
					//XXX testing
					 
					Point_3 newPoint(particleIterator->position.x(), 
									 particleIterator->position.y(), 
									 particleIterator->position.z());
									 
									 
					weighetPointList.push_back( 2(  newPoint.transform(skinScale), 1.0f) );
					 
				}
			}
			
			emit( taskProgress(10) );
			
			for (particleIterator = (*nodesIterator)->bubbles.begin(); 
				 particleIterator != (*nodesIterator)->bubbles.end(); 
				 ++particleIterator) {
				
				hashKey = SurfaceMesher::getPointKey(particleIterator->position);
				
				if ( ! addedPoints.contains( hashKey ) ) {
					addedPoints[hashKey] = true;
					jointPosition.push_back( particleIterator->position );
					
					/*
					Point_3 newPoint(particleIterator->position.x() * 10, 
									 particleIterator->position.y() * 10, 
									 particleIterator->position.z() * 10);
					
					weighetPointList.push_back( Weighted_point( newPoint.transform(skinScale), 0.01) );
					*/
				}
				//weighetPointList.push_back( Weighted_point(*particleIterator, 0.001) );
				
			}
			
			emit( taskProgress(20) );
			
			/*
			 for (Point_3List::iterator particleIterator = (*nodesIterator)->bubblePosition.begin(); 
			 particleIterator != (*nodesIterator)->bubblePosition.end(); 
			 particleIterator++) {
			 
			 double scalingFactor = 1.0f;
			 double xp = CGAL::to_double( particleIterator->x() ) * scalingFactor;
			 double yp = CGAL::to_double( particleIterator->y() ) * scalingFactor;
			 double zp = CGAL::to_double( particleIterator->z() ) * scalingFactor;
			 weighetPointList.push_back( Weighted_point(Bare_point(xp, yp, zp), 0.005f) );
			 
			 
			 
			 }
			 */			
			emit( taskProgress(20) );
			//std::cout << "about to do alpha\n";
			Alpha_shape_3 as(jointPosition.begin(), jointPosition.end());
#endif			
			//as.set_mode(Alpha_shape_3::GENERAL);
			
			
			
			jointPosition.clear();
			addedPoints.clear();
			
#endif			

			//////////////
			
			std::vector<Particle *> range;
			std::vector<Particle *>::iterator rangeIterator;
			
			// go through all bubbles and change surrounding batter 
			// particles visible value to false if it false within its distance
			
			ovKDTree kdTree;
			
			kdTree.addNodes((*nodesIterator)->particles);
			
			for (particleIterator = (*nodesIterator)->bubbles.begin(); 
				 particleIterator != (*nodesIterator)->bubbles.end(); 
				 ++particleIterator) {
				
				range.clear();
				
				kdTree.rangeSearch(particleIterator->position, 0.05, range);
				
				//std::cout << "range size " << range.size() << "\n";
				
				bool abbandon = false;
				
				for (rangeIterator = range.begin(); 
					 rangeIterator != range.end(); 
					 ++rangeIterator) {
					
					if ((*rangeIterator)->trueSurface == true) {
						abbandon = true;
						break;
					}
 				}
				if (abbandon) {

					for (rangeIterator = range.begin(); 
						 rangeIterator != range.end(); 
						 ++rangeIterator) {
						
						//(*rangeIterator)->visible = false;
					}
				}
			}
			
			//std::cout << "before\n";
			
			double weight = 0;
			
			Delaunay_hierarchy dt;
			
			std::list< Weighted_point > jointPosition;
			for (particleIterator = (*nodesIterator)->particles.begin(); 
				 particleIterator != (*nodesIterator)->particles.end(); 
				 ++particleIterator) {
				
				
				// either a bubble moved it or its inside a cutout
				if ( ! particleIterator->visible ) continue;				
				
				hashKey = SurfaceMesher::getPointKey(particleIterator->position);
				
				if ( ! addedPoints.contains( hashKey ) ) {
					addedPoints[hashKey] = true;
					
					//jointPosition.push_back( Weighted_point(Bare_point(particleIterator->position)), 4.0f);
					
					// XXX make this automatic
					//weight = 0.005f;
					weight = 0.05f;
					
					if (particleIterator->trueSurface) {
						weight *= 1.2f;
					}
					
					/*
					jointPosition.push_back(Weighted_point(particleIterator->position, 
														   weight));
					*/
					 //XXX testing
					dt.insert(Weighted_point(particleIterator->position, weight));
				}
			}
			
			emit( taskProgress(10) );
			
			
			
			//////////////
			
			
			//std::cout<< "alpha done\n";
			 
			//double shrinkfactor = 0.5f;
			//std::cout << "about to start making the skin\n";
			//weighetPointList.push_back( Weighted_point( Point_3(0,0,0), 1) );
			//CGAL::make_skin_surface_mesh_3(bubbleStructure, weighetPointList.begin(), weighetPointList.end(), shrinkfactor );
			//std::transform( bubbleStructure.points_begin(), bubbleStructure.points_end(), bubbleStructure.points_begin(), skinScale.inverse());
			//std::cout << " done with the skin\n";
			 
			emit( taskProgress(40) );
			
			/*
			Alpha_shape_3  as(jointPosition.begin(), jointPosition.end(),
							  0, Alpha_shape_3::GENERAL);
			*/
			Alpha_shape_3 as(dt, 0, Alpha_shape_3::GENERAL);
			
			//Alpha_shape_3 as(dt);

			// find optimal alpha values
			Alpha_shape_3::NT alpha_solid = as.find_alpha_solid();
			Alpha_iterator opt = as.find_optimal_alpha(1);
			
 
			//explore the 0-shape - It is dual to the boundary of the union.
			/*
			std::list<Cell_handleA> cells;
			std::list<FacetA>       facets;
			std::list<EdgeA>        edges;
			as.get_alpha_shape_cells(std::back_inserter(cells),
									 Alpha_shape_3::INTERIOR);
			as.get_alpha_shape_facets(std::back_inserter(facets),
									  Alpha_shape_3::REGULAR);
			as.get_alpha_shape_facets(std::back_inserter(facets),
									  Alpha_shape_3::SINGULAR);
			as.get_alpha_shape_edges(std::back_inserter(edges),
									 Alpha_shape_3::SINGULAR);
			std::cout << " The 0-shape has : " << std::endl;
			std::cout << cells.size() << " interior tetrahedra" << std::endl;
			std::cout << facets.size() << " boundary facets" << std::endl;
			std::cout << edges.size()  << " singular edges" << std::endl;
			*/
			
			if ( ! __alphaSet ) {
			
				Alpha_iterator opt = as.find_optimal_alpha(1);
			
				//__alpha = CGAL::to_double( (*opt) * 5.0f );
				__alpha = 10 * CGAL::to_double( (*opt) );
				std::cout << " alpha " << __alpha << "\n";
				//__alphaSet = true;
			}
		
			
			as.set_alpha(__alpha);
			
	
			/*
			AlphashapeHDSPolyhedronifikator<Alpha_shape_3, Polyhedron > polif(as);
			
			int Qsize = polif.addAllComponents(batterVolume);
			*/
			
			//AlphashapePolyhedronifikator<Alpha_shape_3, Polyhedron> polif(as); // where A is an alphaShape
			//bool polyhedronOk = polif.addAllComponents(batterVolume); // returns 'true' if the process suceeded
			/*
			AlphashapeHDSator<Alpha_shape_3, HDS> add(&as); // A is an Alpha_shape_3
			HDS H0;
			// insert the whole border of A to H0
			add.addAllComponents(H0);
			
			
						
			std::cout << "hds size of facet " << H0.size_of_faces() << "\n";
			*/
			
			//Polyhedron Q; // Les P,Q be polhedra
			AlphashapeHDSPolyhedronifikator<Alpha_shape_3,Polyhedron> polif(as);
			int Qsize = polif.addAllComponents(batterVolume);
			
#if 0			
			newPolyhedronFromAShape.addAShape(&as);
			std::cout<< "done new \n";
			emit( taskProgress(60) );
			
			// breaking down
			batterVolume.delegate(newPolyhedronFromAShape);
#endif			
			
			// get bubble structure
			
			emit( taskProgress(80) );
		
			//CGAL::Subdivision_method_3::Loop_subdivision(batterVolume, 2);
 			
			
			/*
		
			CGAL::Subdivision_method_3::PTQ(batterVolume, 
											WLoop_mask_3<Polyhedron>(), 1);
 		
			*/
			
			
			CGAL::Subdivision_method_3::Sqrt3_subdivision(batterVolume, 2);

			
			if ( batterVolume.is_closed() ) {
				
				
				std::cout<< "closed\n";
				
				//batterVolume = getBooleanDifference(batterVolume, cutouts[0]);
				//batterVolume = getBooleanDifference(batterVolume,  cutouts[0]);
			 				
				 				
				//std::transform( batterVolume.points_begin(), batterVolume.points_end(), batterVolume.points_begin(), skinScale);
				//batterVolume = getBooleanDifference(batterVolume, bubbleStructure);
				std::cout << "batterVolume.size_of_facets() "  << batterVolume.size_of_facets() << "\n";
			}
#if 0
			if ( batterVolume.is_closed() ) {
				std::cout<< "closed\n";
				
				newExactPolyhedronFromPolyhedron.addPolyhedron(&batterVolume);
				
				ExactPolyhedron tempExactPolyhedron;
				tempExactPolyhedron.delegate(newExactPolyhedronFromPolyhedron);
				
				Nef_polyhedron nefPolyhedron(tempExactPolyhedron);
				std::cout<< "building nef OK\n";
				
				// perform the cut on the nef polyhedron
				
				for (uint k=0; k<cutouts.size(); k++) {
					
					nefPolyhedron = nefPolyhedron - cutouts[k];
					
				}
				
				
				// bubbles ?
				
				
				createBubbleStructure( nefPolyhedron, *nodesIterator );
				
				
				// scale back
				
				//std::transform( bubbleStructure.points_begin(), bubbleStructure.points_end(), bubbleStructure.points_begin(), skinScale.inverse());
				
				/*
				newExactPolyhedronFromPolyhedron.addPolyhedron(&bubbleStructure);
				
				tempExactPolyhedron.delegate(newExactPolyhedronFromPolyhedron);
				
				Nef_polyhedron nefBubbles(tempExactPolyhedron);
				
				nefPolyhedron = nefPolyhedron - nefBubbles;
				*/
				
				// return to polyhedron
				
				nefPolyhedron.convert_to_Polyhedron(tempExactPolyhedron);
			
				// and finally return to simple polyhedron
				
				BuildPolyhedronFromExactPolyhedron<HalfedgeDS> newPolyhedronFromAShape;
				
				newPolyhedronFromAShape.addExactPolyhedron(&tempExactPolyhedron);
				
				batterVolume.clear();
				
				batterVolume.delegate(newPolyhedronFromAShape);
				
			}
#endif 		
			// add bubbles
			
			// create bubble structure
			
			//Nef_polyhedron bubbleStructure;
			//std::cout << " creating bubble structure \n";
			//createBubbleStructure( bubbleStructure, *nodesIterator );
			//createBubbleStructure( batterVolume, *nodesIterator );

			
			// add cutout (?)
			
			//CGAL::Subdivision_method_3::CatmullClark_subdivision(batterVolume); // ???
			//CGAL::Subdivision_method_3::DooSabin_subdivision(batterVolume); // ???
			//CGAL::Subdivision_method_3::Sqrt3_subdivision(batterVolume);
			
			//CGAL::Subdivision_method_3::Loop_subdivision(batterVolume);
			
			//node.newSurfaces.push_back( batterVolume );
			
						
			//(*nodesIterator)->newSurfaces.push_back( bubbleStructure );
			
			 
			
 			(*nodesIterator)->newSurfaces.push_back( batterVolume );
			
			emit( taskProgress(100) );
		}
		
		emit( taskProgress(100) );
		
		
	}
	
	
}
#endif


#if 0
void SurfaceMesher::createBubbleStructure(Nef_polyhedron & bubbleStructure, 
										  ProcessedNode* node) 
{
	
	
	ExactPolyhedron singleBubblePolyhedron;
	
	createSingleBubblePolyhedron(singleBubblePolyhedron);
	
	Nef_polyhedron singleBubbleNef( singleBubblePolyhedron );
	
	//Nef_polyhedron singleBubbleNef;
	
	// iterate through all bubbles and position and scale them accordingly
	
	/*
	std::list<Point_3>::iterator bubblePositionIterator;
	std::list<double>::iterator bubbleSizeIterator;
	bubbleSizeIterator = node->bubbleSize.begin();
	 for (bubblePositionIterator = node->bubblePosition.begin(); bubblePositionIterator != node->bubblePosition.end(); ++bubblePositionIterator) {
	 
	 Vector_3 translationVector(bubblePositionIterator->x(),
	 bubblePositionIterator->y(),
	 bubblePositionIterator->z() );
	 
	 Aff_transformation_3 translate(CGAL::TRANSLATION, translationVector);
	 
	 double s = *bubbleSizeIterator;
	 bubbleSizeIterator++;
	 Aff_transformation_3 scale(s, 0, 0,
	 0, s, 0,
	 0, 0, s);
	 
	 singleBubbleNef.transform(scale);
	 
	 singleBubbleNef.transform(translate);
	 
	 bubbleStructure = bubbleStructure + singleBubbleNef;
	 
	 singleBubbleNef.transform(translate.inverse());
	 singleBubbleNef.transform(scale.inverse());
	 
	 
	 }
	 
	 */
	
	std::vector<Particle>::iterator bubblesIterator;
	
	
	// XXX testing with particles
	
	int i = 1;
	int count = node->particles.size();
	
	for (bubblesIterator = node->particles.begin(); 
		 bubblesIterator != node->particles.end(); 
		 ++bubblesIterator) {

		
		double s = 1;
		ExactAff_transformation_3 scale(s, 0, 0,
										0, s, 0,
										0, 0, s);
		
		
		ExactVector_3 translationVector(bubblesIterator->position.x(),
										bubblesIterator->position.y(),
										bubblesIterator->position.z());
		
		ExactAff_transformation_3 translate(CGAL::TRANSLATION, 
											translationVector);
		
		Nef_polyhedron tempPoly = singleBubbleNef;
		
		tempPoly.transform(scale);
		tempPoly.transform(translate);
		
		bubbleStructure = bubbleStructure - tempPoly;
		
		/*
		singleBubbleNef.transform(scale);
		singleBubbleNef.transform(translate);
		
		//bubbleStructure + singleBubbleNef;
		
		singleBubbleNef.transform(translate.inverse());
		singleBubbleNef.transform(scale.inverse());
		*/
		std::cout << i++ << " of " << count << "\n";
		
	}
	
	
	
}

#endif


QString SurfaceMesher::getPointKey(double x, double y, double z)
{
	QString xS = QString::number( x, 'e', 16 ) ;
	QString yS = QString::number( y, 'e', 16 ) ;
	QString zS = QString::number( z, 'e', 16 ) ;
	QString mid = "$";
	
	return xS + mid + yS + mid + zS;
	
}

QString SurfaceMesher::getPointKey(Point_3 point)
{ 	
	return getPointKey(CGAL::to_double( point.x() ), 
					   CGAL::to_double( point.y() ), 
					   CGAL::to_double( point.z() ) );
}


#if 0

bool SurfaceMesher::IsPosOnSrfcByItsCGALConvexHull(Point_3 & target,
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
					orthogonalVector = zeroVector;
					//orthogonalVector = currentHE->facet()->plane().orthogonal_vector();
					if (orthogonalVector != zeroVector && false) {
						
						normal = normal + orthogonalVector;
						
					}
					else {
						//orthogonalVector = Polygons::getFacetsNormal(currentHE->facet());
						
						orthogonalVector = CGAL::cross_product(currentHE->facet()->halfedge()->next()->vertex()->point() - 
															   currentHE->facet()->halfedge()->vertex()->point(),
															   currentHE->facet()->halfedge()->next()->next()->vertex()->point() - 
															   currentHE->facet()->halfedge()->next()->vertex()->point());
						
						
						
						
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

bool SurfaceMesher::IsPointOnTriangle(Point_2 p, 
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


void SurfaceMesher::smoothGrid(int smoothingGridSize, 
							   int xGridD, 
							   int yGridD, 
							   int zGridD,
							   bool countZero,
							   MyVolume &volume)
{
	
	MyVolume smoothedVolume;
	smoothedVolume.Init(Point3i(xGridD,yGridD,zGridD));
	double totalCells = std::pow(smoothingGridSize * 2 + 1, 3.0);
	double maxValue = 0;
	for(int i=0;i<zGridD;i++){
    	for(int j=0;j<yGridD;j++){
      		for(int k=0;k<xGridD;k++) {
				//volume.Val(k,j,i) /= maxValue;
				//volume.Val(k,j,i) /= maxValue;
				
				if (volume.Val(k,j,i) != 0 || countZero) {
					
					double value = 0;
				
					for (int p = -smoothingGridSize; p < smoothingGridSize+1; p++) {
						for (int q = -smoothingGridSize; q < smoothingGridSize+1; q++) {
							for (int r = -smoothingGridSize; r < smoothingGridSize+1; r++) {
							
								double cx = k+p;
								double cy = j+q;
								double cz = i+r;
							
								if (cx > 0 && cx < xGridD &&
									cy > 0 && cy < yGridD &&
									cz > 0 && cz < zGridD) {
									
										//if (p==0 && q==0 && r==0) {
										value += volume.Val(cx,cy,cz);
										//} else 
										//	value += volume.Val(cx,cy,cz) * 0.5;
									
								}
							}
						}
					}
				
					smoothedVolume.Val(k,j,i) = value / totalCells;
					if (smoothedVolume.Val(k,j,i) > maxValue) {
						maxValue = smoothedVolume.Val(k,j,i);
					}	
				} else {
					smoothedVolume.Val(k,j,i) = 0;
				}
				
			}
		}	
	}
#if 1	
	for(int i=0;i<zGridD;i++){
    	for(int j=0;j<yGridD;j++){
      		for(int k=0;k<xGridD;k++) {
				volume.Val(k,j,i) = smoothedVolume.Val(k,j,i) / maxValue;
				//volume.Val(k,j,i) = smoothedVolume.Val(k,j,i) / (maxValue/4);
		//		volume.Val(k,j,i) = smoothedVolume.Val(k,j,i) / (maxValue/6);

//				//volume.Val(k,j,i) = smoothedVolume.Val(k,j,i) / 6.0;
			}
		}
	}
	std::cout << "(maxValue/2) " << (maxValue/2) << "\n";
#endif	
	
}

bool SurfaceMesher::QCallBack(const int pos, const char * str)
{
	
	std::cout << pos << " " << str << "\n";
	

	return true;
}

QString SurfaceMesher::getKey(double a, double b, double c) {
	return QString::number(a) + " " + 
		   QString::number(b) + " " + 
		   QString::number(c);
}

void SurfaceMesher::updateNormal(CVertexO * v) {
	
    

        CFaceO * f = v->VFp();
        CFaceO * one_face = f;
        int pos = v->VFi();


	if (f && one_face) {
                v->N()[0]=0;
                v->N()[1]=0;
                v->N()[2]=0;

		do {

                    CFaceO * temp=one_face->VFp(pos);

                    if (one_face!=0 && !one_face->IsD()) {
                                vcg::face::ComputeNormalizedNormal(*one_face);
                                //for (int lauf=0; lauf<3; lauf++)
                                //if (pos!=lauf) {
                                //v->N()+=one_face->V(lauf)->cN();
                                //}
                                //vcg::face::ComputeNormalizedNormal(*one_face);

                        v->N() += one_face->N();
                        pos=one_face->VFi(pos);
                    }
                    one_face=temp;
		} while (one_face!=f && one_face!=0);
		v->N().Normalize();
	}

}



//bool SurfaceMesher::isInside(CMesh *cm, StaticPtrGrid &grid, CMesh::CoordType &test) {
// 	
//	return false;
//}
