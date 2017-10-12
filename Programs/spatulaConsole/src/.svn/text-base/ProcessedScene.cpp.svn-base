

#include <iostream>

#include "ProcessedScene.h"
#include "integrationHelper.h"

#include <vcg/complex/trimesh/allocate.h>
#include <vcg/complex/trimesh/append.h>
#include <vcg/complex/trimesh/update/bounding.h>

ProcessedScene::ProcessedScene()
{
	
}

ProcessedScene::ProcessedScene(domVisual_scene& visual_scene)
{
	
	if ( visual_scene.getName() ) {
		sceneName = visual_scene.getName(); 
	}
	else {
		sceneName = "Scene";
	}
	
	sceneRoot.nodeType = ProcessedNode::SceneNode;
	
	domNode_Array& nodes = visual_scene.getNode_array();
	
	sceneRoot.children.clear();
	
	for ( uint i = 0; i < nodes.getCount(); i++) {
		
		//sceneRoot.children.push_back(& lookup<ProcessedNode, domNode>(*nodes[i]) );
		sceneRoot.children.push_back( lookup<ProcessedNode, domNode>(*nodes[i]) );
		//sceneRoot.children[sceneRoot.children.size() - 1]->parent = &sceneRoot;
		
	}
	
	// get cutOuts
	
	getCutouts(visual_scene);

}

ProcessedScene::~ProcessedScene()
{
	//sceneRoot.clear();
	
}

void ProcessedScene::getCutouts(domVisual_scene& visual_scene)
{
#if 1
	domExtra_Array &extraArray = visual_scene.getExtra_array();
	
	
	/* not the best way to do things, but we know the format of the information
	 * that we will be receiving
	 */
	
	if ( extraArray.getCount() ) { // there is something extra ( our cutouts )
		
		domExtra *extra = extraArray[0];
		
		domTechnique_Array & techs = extra->getTechnique_array();
		
		for (uint j=0; j<techs.getCount(); j++) {
			
			domTechniqueRef tech = techs.get(j);

			if (tech != NULL) {
				
				if ( QString(tech->getProfile())  == "oven") {	
										
					domIDREF_array *idRefArray = daeSafeCast<domIDREF_array>( tech->getChild("IDREF_array") );

					for (uint k=0; k<idRefArray->getCount(); k++) {
						
						xsIDREFS ids = idRefArray[k].getValue();
						
						for (uint i = 0; i<ids.getCount();i++) {
							
							
							CutOutInformation newCutOut;
							
							CMesh *  newVoidSpace = new CMesh;
							std::vector< CMesh::VertexPointer> ivp; 
							
							newVoidSpace->Clear();
						
							
							domNode *node = daeSafeCast<domNode>(ids[i].getElement());
							
							// get transformation
							Aff_transformation_3 transformation = ProcessedNode::readTransformationsFromCollada(*node);
							
							//double ** transformation = ProcessedNode::readSimpleTransformationsFromCollada(*node);
							
							// get geometry
							
							domInstance_geometry *instanceGeometry = (domInstance_geometry *) (domElement *)  node->getChild("instance_geometry");
							domGeometry *geometry = (domGeometry *) (domElement *) instanceGeometry->getUrl().getElement();
							domMesh *mesh = geometry->getMesh();
							
							#if 0

							VertexIterator vi=m.vert.begin(); 
							ivp.push_back(&*vi;(*vi).P()=CoordType ( 1.0, 1.0, 1.0) ); 

							FaceIterator fi=m.face.begin(); 
							(*fi).V(0)=ivp[0];  (*fi).V(1)=ivp[1]; (*fi).V(2)=ivp[2];
							#endif
							
							
							// get vertices
							
							for(uint p=0; p < mesh->getVertices()->getInput_array().getCount(); p++) {
								
								if ( QString( mesh->getVertices()->getInput_array()[p]->getSemantic() ) == "POSITION" ) {
									
								
									domSource *source = (domSource *) (domElement *) mesh->getVertices()->getInput_array()[p]->getSource().getElement();
									domFloat_array *floatArray = source->getFloat_array();
									
									
									//domElement *test = (domElement *) mesh->getVertices()->getInput_array()[p]->getSource().getElement();
									//std::cout << "test is " << test->getTypeName() <<"\n";
									
									uint count = floatArray->getCount();
									
									vcg::tri::Allocator<CMesh>::AddVertices(*newVoidSpace, count / 3);
									CMesh::VertexIterator vi = newVoidSpace->vert.begin(); 
									
									for (uint q=0; q<count; q+=3) {
										(*vi).P()=CMesh::CoordType(floatArray->getValue()[q], 
																	floatArray->getValue()[q+1], 
																	floatArray->getValue()[q+2]); 
										ivp.push_back(&*vi);										
										++vi;
										//Point_3 newPoint( floatArray->getValue()[q], floatArray->getValue()[q+1], floatArray->getValue()[q+2] );
										CGALPoint_3 newCGALPoint( floatArray->getValue()[q], floatArray->getValue()[q+1], floatArray->getValue()[q+2] );
										
										newCGALPoint = newCGALPoint.transform(transformation);
										// XXX ADD TRANSFORMATION
										//was working before
										//newPoint = transformation.transform(newPoint);
										Point_3 newPoint(newCGALPoint.x(), newCGALPoint.y(), newCGALPoint.z());
										newCutOut.vertex.push_back(newPoint);

										
									}
								}
								
								
							}
							
							
							// get facets. right now we are only dealing with triangles but we could add trifans, polylist, etc

							std::vector< uint> newFacet;
							
							for (uint p=0; p < mesh->getTriangles_array().getCount(); p++) {
								
								domTriangles* triangles = daeSafeCast<domTriangles>( mesh->getTriangles_array()[p]);
								
								uint offset = 0;
								uint triangleCount = triangles->getCount();
								uint inputCount = triangles->getInput_array().getCount();
								for (uint q=0; q < inputCount; q++) {

									
									if( QString(triangles->getInput_array()[q]->getSemantic() ) == "VERTEX" ) {
										
										offset = triangles->getInput_array()[q]->getOffset();
									}
									
								}
								
								
								// allocate triangles 						
								vcg::tri::Allocator<CMesh>::AddFaces(*newVoidSpace, triangleCount);
								CMesh::FaceIterator fi = newVoidSpace->face.begin(); 
								
								// duh !!
								for(uint t=0; t<triangleCount; t++) {
									
									newFacet.clear();
									
									for ( uint q=0; q<3; q++ ) {
										(*fi).V(q)=ivp[ triangles->getP()->getValue()[ (t * inputCount * 3) + (q * inputCount) ] ];
										newFacet.push_back( triangles->getP()->getValue()[ (t * inputCount * 3) + (q * inputCount) ] ) ;

									}
									++fi;
									
									newCutOut.facets.push_back( newFacet );

									
								}
																
							}
							vcg::tri::UpdateBounding<CMesh>::Box(*newVoidSpace);
												
							newVoidSpace->vert.EnableMark();				
							newVoidSpace->face.EnableMark();
							
							voids.clear();
							grids.clear();
									
							voids.push_back(newVoidSpace);
							StaticPtrGrid g;
							g.Set(newVoidSpace->face.begin(), newVoidSpace->face.end());
							grids.push_back(g);
							std::cout << "ivp.size() "  << ivp.size() << "\n";
							ivp.clear();
							std::cout << "newCutOut.vertex.size() " << newCutOut.vertex.size() << "\n";
							cutOuts.push_back( newCutOut );
						}
					}
				}
			}
		}
	}
#endif
}





std::list<ProcessedNode*>   ProcessedScene::getNodes()
{
		
	std::list< ProcessedNode* > result = getNodes(sceneRoot);
	result.push_back(&sceneRoot);

	return result;
	
}

std::list<ProcessedNode*>  ProcessedScene::getNodes(ProcessedNode &node)
{
	
	std::list<ProcessedNode*> result;
	
	
	
	std::vector<ProcessedNode>::iterator childrenIterator;
	
	for (childrenIterator = node.children.begin(); childrenIterator != node.children.end(); childrenIterator++ ) {
		
		std::list<ProcessedNode*> intermediateResult = getNodes(*childrenIterator);
							
		result.insert( result.begin(), intermediateResult.begin(), intermediateResult.end() );
		result.push_back( &(*childrenIterator) );
		
	}
	
	return result;
	
}
