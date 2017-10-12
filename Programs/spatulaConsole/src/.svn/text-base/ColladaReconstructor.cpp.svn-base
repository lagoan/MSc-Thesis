/*
 *  ColladaReconstructor.cpp
 *  Spatula
 *
 *  Created by Omar Rodriguez-Arenas on 15/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include <map>
#include <cstdlib>
#include <dae/daeURI.h>
#include <dom.h>
#include <dom/domProfile_COMMON.h>

#include "ColladaReconstructor.h"
#include "ovKDtree.h"


void ColladaReconstructor::addBubbleToCollada( DAE &daeObject, ProcessedScene &scene)
{

        // add bubble geometry if not there already


        domGeometryRef bubbleShape;
        QString bubbleFile = "file:///./bubble.dae";

		#ifdef Q_OS_LINUX
		bubbleFile = "file:////home/omar/Dropbox/dae/bubble.dae";
		#else
		bubbleFile = "file:////Users/kuervo/Dropbox/dae/bubble.dae";
		#endif
 		int loadres = daeObject.load( bubbleFile.toAscii() );
		std::cout << loadres << "loadres \n";
        daeObject.getDatabase()->getElement((daeElement**) &bubbleShape,
                                              0,
                                              NULL,
                                              COLLADA_ELEMENT_GEOMETRY,
                                              bubbleFile.toAscii() );


	bubbleShape->setId("OvensUniqueBubbleShape");
	bubbleShape->setName("OvensUniqueBubbleShape");


	domCOLLADA *domRoot = daeObject.getRoot(scene.fileName.toStdString());
	domLibrary_geometries_Array libGeos = domRoot->getLibrary_geometries_array();


        libGeos[0]->placeElement( bubbleShape->clone());

        daeObject.unload( bubbleFile.toAscii() );

}

void ColladaReconstructor::addBubbleInstancesToCollada(DAE &daeObject, ProcessedScene &scene)
{
		int bubbleCount;
		QString bubbleID;
		QString nodeID;
		domNode *newNode;
		domNode *node;				
		domVisual_scene *visScene;
	
		daeObject.getDatabase()->getElement((daeElement**)&visScene, 
											  0, NULL, COLLADA_ELEMENT_VISUAL_SCENE);
		
		daeObject.getDatabase()->getElement((daeElement**)&node, 
										  0, NULL, COLLADA_ELEMENT_NODE, scene.fileName.toAscii());

		// remove past bubbles if such exist
	
		int currentNodeCount = daeObject.getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
		
		// this is ok
		
		for (int p=0; p < currentNodeCount; p++) {
			daeObject.getDatabase()->getElement((daeElement**)&newNode, 
												  p, NULL, COLLADA_ELEMENT_NODE);
			
			nodeID = newNode->getId();
			
			if ( nodeID.contains("bubbleInstance_") ) {
				daeElement::removeFromParent(newNode);
				currentNodeCount = daeObject.getDatabase()->getElementCount(NULL, COLLADA_ELEMENT_NODE, NULL);
				p=0;
				
			}
			
		}
		
		// ugly code, will fix later. I promise
		// get the new bubble shape
		
		QString bubbleFile;
		domGeometry* bubbleShape;
		
		daeObject.getDatabase()->getElement((daeElement**) &bubbleShape, 0,
											  "OvensUniqueBubbleShape", COLLADA_ELEMENT_GEOMETRY );
											

		////
		
		std::list<ProcessedNode*> nodes = scene.getNodes();
		std::list<ProcessedNode*>::iterator nodesIterator;
		std::vector<Particle *> bubbles;
		std::vector<Particle *>::iterator bubbleIterator;
		std::vector<Particle>::iterator particleIt;
		
		for (nodesIterator = nodes.begin(); 
			 nodesIterator != nodes.end(); 
			 ++nodesIterator) {
			//std::cout << "first inside\n";
			
			for (particleIt = (*nodesIterator)->bubbles.begin();
				 particleIt != (*nodesIterator)->bubbles.end();
				 ++particleIt) {
                   //std::cout << " inside\n";
									
					bubbles.push_back(&(*particleIt));
				
			}
		
		
		}
		
		bubbleCount = bubbles.size();//_workingAnimationFrame.listBubblesCount[workIndex];
		domTranslate *trans;
		domScale     *scale;
		domInstance_geometry *instanceGeom;
		QString geoPath;
		//daeURI bubbleURI;
		domBind_material *bindMat;
		domBind_material::domTechnique_common *bmtc;
		domInstance_material *instMat;

		for (int p=0; p<bubbleCount; p++) {
			
			
			//std::cout << "adding\n";
			bubbleID = QString("bubbleInstance_%1").arg(p);
			
			
			
			//newNode = daeSafeCast<domNode>( visScene->createAndPlace( COLLADA_ELEMENT_NODE ) );
#if 1
			newNode = daeSafeCast<domNode>( node->createAndPlace( COLLADA_ELEMENT_NODE ) );
			newNode->setId( bubbleID.toAscii() );
			
			trans = daeSafeCast<domTranslate>( newNode->createAndPlace( COLLADA_ELEMENT_TRANSLATE ) );
			trans->setSid( "translate" );
			
			trans->getValue().set3(bubbles[p]->position.x(), 
								   bubbles[p]->position.y(), 
								   bubbles[p]->position.z());
			
			// set bubble size adjustment
			scale = daeSafeCast<domScale>( newNode->createAndPlace( COLLADA_ELEMENT_SCALE ) );
			scale->setSid( "scale" );
			
			// XXX W CHANGE ovTuple4f
			float scaleVar = bubbles[p]->size;
			//std::cout << "scaleVar " << scaleVar << "\n";
			scale->getValue().set3( scaleVar, scaleVar, scaleVar);
			
			geoPath = /*saveDestination +*/ QString("#OvensUniqueBubbleShape");
			
			instanceGeom = daeSafeCast<domInstance_geometry>( newNode->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
			daeURI bubbleURI(*instanceGeom, geoPath.toStdString());
			//daeURI bubbleURI(geoPath.toStdString());
			//bubbleURI.setURI(geoPath.toAscii());
			instanceGeom->setUrl( bubbleURI );
			
			// instance geom
			/*
			 domInstance_geometry *instGeo = daeSafeCast<domInstance_geometry>( node->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
			 uri.setElement( geo );
			 uri.resolveURI();
			 instGeo->setUrl( uri );
			 */
			
			// material
			//daeURI uri;
			domInstance_geometry_Array instanceGeometryArray = node->getInstance_geometry_array();
			
			
			
			//domLibrary_materials	 *libMats	   = daeSafeCast<domLibrary_materials>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_MATERIALS ) );
			//domLibrary_effects		 *libEffects   = daeSafeCast<domLibrary_effects>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_EFFECTS ) );
			
			//domEffect *effect = daeSafeCast<domEffect>( libEffects->createAndPlace( COLLADA_ELEMENT_EFFECT ) );
			//effect->setId( "simple_texture-fx" );
			
			//domMaterial *material = daeSafeCast<domMaterial>( libMats->createAndPlace( COLLADA_ELEMENT_MATERIAL ) );
			//material->setId( "texture_material" );  // ESTE
			
			//domInstance_effect *instEf = daeSafeCast<domInstance_effect>( material->createAndPlace( COLLADA_ELEMENT_INSTANCE_EFFECT ) );
			//uri.setElement( effect );
			//uri.resolveURI();
			//instEf->setUrl( uri );
			
			bindMat = daeSafeCast<domBind_material>( instanceGeom->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
			bmtc = daeSafeCast<domBind_material::domTechnique_common>( bindMat->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			instMat = daeSafeCast<domInstance_material>( bmtc->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
			//uri.setElement( material ); //este
			//uri.resolveURI();
			
			// XXX tempo, currently only sets one bubble
			//bubbleShape->getMesh()->getPolylist_array()[0]->setMaterial(instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol());
			bubbleShape->getMesh()->getTriangles_array()[0]->setMaterial(instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol());
			
			
			
			instMat->setSymbol( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getSymbol() ); // ESTE

			instMat->setTarget( instanceGeometryArray[0]->getBind_material()->getTechnique_common()->getInstance_material_array()[0]->getTarget() );


#endif		
		
			//std::cout << "added\n";
		}
	
}

void ColladaReconstructor::releaseMemory()
{
	//daeElement::removeFromParent(upAxis);
	
	
	for (size_t i=0; i< instanceGeometries.size(); i++) {
		daeElement::removeFromParent(instanceGeometries[i]);
	}
	instanceGeometries.clear();
	
	for (size_t i=0; i< nodes.size(); i++) {
		daeElement::removeFromParent(nodes[i]);
	}
	nodes.clear();
	
	daeElement::removeFromParent(libEffects);
	daeElement::removeFromParent(libImages);
	daeElement::removeFromParent(libMaterials);
	daeElement::removeFromParent(libGeos);
	daeElement::removeFromParent(upAxis);
	daeElement::removeFromParent(asset);
	daeElement::removeFromParent(instanceVisualScene);
	daeElement::removeFromParent(currentScene);
	daeElement::removeFromParent(visualScene);
	daeElement::removeFromParent(libVisualScenes); 
	

	
}

void ColladaReconstructor::reconstruct(ProcessedScene &scene, 
				       DAE &daeObject, 
				       MeshDocument & meshDocument)
{
	//domNode *node;
	
	std::list<ProcessedNode*>  sceneNodes = scene.getNodes();
	std::list<ProcessedNode*>::iterator nodeIterator;
	
	
	

	//domCOLLADA *domRoot = daeObject.getDom( scene.fileName.toAscii() );

	domCOLLADA *domRoot = daeObject.getRoot( scene.fileName.toStdString() );
	
#if 1	
	createTextureEffect(domRoot);

	// create library_visual_scene
	// create visual_scene
	
	asset = daeSafeCast<domAsset>( domRoot->createAndPlace(COLLADA_ELEMENT_ASSET) );
	upAxis = daeSafeCast<domAsset::domUp_axis>( asset->createAndPlace(COLLADA_ELEMENT_UP_AXIS) );
	upAxis->setValue(UPAXISTYPE_Y_UP);
	
	libVisualScenes = daeSafeCast<domLibrary_visual_scenes>( domRoot->createAndPlace(COLLADA_ELEMENT_LIBRARY_VISUAL_SCENES) );
	
	visualScene = daeSafeCast<domVisual_scene>( libVisualScenes->createAndPlace( COLLADA_ELEMENT_VISUAL_SCENE ) );
	
	visualScene->setId("VisualSceneNode");
	
	currentScene = daeSafeCast<domCOLLADA::domScene>( domRoot->createAndPlace( COLLADA_ELEMENT_SCENE ) );
	
	instanceVisualScene = daeSafeCast<domInstanceWithExtra>( currentScene->createAndPlace( COLLADA_ELEMENT_INSTANCE_VISUAL_SCENE ) );
	//domInstanceWithExtraRef  instanceVisualScene = currentScene->getInstance_visual_scene();
	
	instanceVisualScene->setUrl("#VisualSceneNode");
	
	for (nodeIterator = sceneNodes.begin(); nodeIterator != sceneNodes.end(); nodeIterator++) {
		
		if ( (*nodeIterator)->newSurfaces.size() ) {
			// found a node with newly generated surfaces

			QString nodeId = (*nodeIterator)->id;
			
			/*
			daeObject.getDatabase()->getElement((daeElement**)&node, 
												0, nodeId.toAscii(), COLLADA_ELEMENT_NODE);
			*/
			
			//node
			nodes.push_back(daeSafeCast<domNode>(visualScene->createAndPlace( COLLADA_ELEMENT_NODE ) ));
			
			nodes[nodes.size()-1]->setId(nodeId.toStdString().c_str());
			
			// get visual scene array
			
			// remove node transformations if they exist since they are already
			// taken into account
			
			//removeTransformations(node);
			
			// delete current geometry and add current instance
			//domInstance_geometry_Array instanceGeometry = node->getInstance_geometry_array();
			
			
			//domInstance_geometry *instanceGeometry = daeSafeCast<domInstance_geometry>( nodes[nodes.size()-1]->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) );
			instanceGeometries.push_back(daeSafeCast<domInstance_geometry>( nodes[nodes.size()-1]->createAndPlace( COLLADA_ELEMENT_INSTANCE_GEOMETRY ) ));
			
			
			//get the first ID 
			//QString newID = instanceGeometry->getUrl().getID();
			
			QString newID ="pBatter";
			QString newUrl = newID;
			newUrl.prepend("#");
			
			
			instanceGeometries[instanceGeometries.size()-1]->setUrl(newUrl.toStdString().c_str());
			
			// get material
			
			
			/*
			daeObject.getDatabase()->getElement((daeElement**)&tempGeometry, 
												0, newID.toAscii(), COLLADA_ELEMENT_GEOMETRY);
			
			// for now we use the first usable material we encounter
			
			QString materialEasy;
			
			
			if (tempGeometry->getMesh()->getPolylist_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolylist_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getPolygons_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getPolygons_array()[0]->getMaterial();
			} else if (tempGeometry->getMesh()->getTriangles_array().getCount() ) {
				materialEasy = tempGeometry->getMesh()->getTriangles_array()[0]->getMaterial();
			}
			
			*/
			
			
			
			// remove old geometries of current node
			//domGeometry *tempGeometry;
			
			//domBind_material *instanceBindMaterial;
			//domInstance_material_Array instanceMaterialarray;
			//bindVertexInput;
			
			
			/*
			instanceTechniqueCommon = instanceGeometry->getBind_material()->getTechnique_common();
			instanceMaterialarray = instanceTechniqueCommon->getInstance_material_array();
			*/
			
			domBind_material *bindMaterial = daeSafeCast<domBind_material>(instanceGeometries[instanceGeometries.size()-1]->createAndPlace( COLLADA_ELEMENT_BIND_MATERIAL ) );
			domBind_material::domTechnique_common *instanceTechniqueCommon = daeSafeCast<domBind_material::domTechnique_common>(bindMaterial->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
			domInstance_material *instanceMaterial = daeSafeCast<domInstance_material>( instanceTechniqueCommon->createAndPlace( COLLADA_ELEMENT_INSTANCE_MATERIAL ) );
			
			// we only assume one material
			instanceMaterial->setSymbol("cakeTextureSG");
			instanceMaterial->setTarget("#cakeMaterial");
			domInstance_material::domBind_vertex_input *bindVertexInput = daeSafeCast<domInstance_material::domBind_vertex_input>(instanceMaterial->createAndPlace( COLLADA_ELEMENT_BIND_VERTEX_INPUT ) );
			
			bindVertexInput->setSemantic("TEX0");
			bindVertexInput->setInput_semantic("TEXCOORD");
			bindVertexInput->setInput_set(0);
			
			
			
			
			/*
			for (uint j=0 ; j < instanceGeometry.getCount(); j++) {
				daeObject.getDatabase()->getElement((daeElement**)&tempGeometry, 
													0, instanceGeometry[j]->getUrl().getID(), COLLADA_ELEMENT_GEOMETRY);
				
				daeElement::removeFromParent( tempGeometry );
			
				
				// change bind matrial and add our own material created for the batter
				// the way it is programmed right now it will be repeating work 
				
				instanceTechniqueCommon = instanceGeometry[j]->getBind_material()->getTechnique_common();
				instanceMaterialarray = instanceTechniqueCommon->getInstance_material_array();
				
				// we only assume one material
				
				instanceMaterialarray[0]->setAttribute("symbol", "cakeTextureSG");
				instanceMaterialarray[0]->setAttribute("target", "#cakeMaterial");
				bindVertexInput = daeSafeCast<domInstance_material::domBind_vertex_input>(instanceMaterialarray[0]->createAndPlace( COLLADA_ELEMENT_BIND_VERTEX_INPUT ) );
				
				bindVertexInput->setSemantic("TEX0");
				bindVertexInput->setInput_semantic("TEXCOORD");
				bindVertexInput->setInput_set(0);
			}
			
			*/
			
			
			//domLibrary_geometries_Array	 libGeos = domRoot->getLibrary_geometries_array();
			
			libGeos = daeSafeCast<domLibrary_geometries>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_GEOMETRIES ) );
			
			
			
			// add new geometry  for the node
			
			domGeometry *geo = daeSafeCast<domGeometry>( libGeos->createAndPlace( COLLADA_ELEMENT_GEOMETRY ) );
			
			
			geo->setId( newID.toAscii() );
			

			
			domMesh *mesh = daeSafeCast<domMesh>( geo->createAndPlace( COLLADA_ELEMENT_MESH ) );
			
			// remove extra 
			int extraCount = nodes[nodes.size()-1]->getExtra_array().getCount();
			
			for (int j=0; j < extraCount; j++) {
				
				domExtraRef extra = nodes[nodes.size()-1]->getExtra_array().get(j);
				daeElement::removeFromParent( extra );
				
				
			}
			
			
			
			///////////
			//int numberOfVertices = (*nodeIterator)->newSurfaces.begin()->size_of_vertices();
			//int numberOfFacets =   (*nodeIterator)->newSurfaces.begin()->size_of_facets();
			
			domSource *positionSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *normalSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			//domSource *colorSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );
			domSource *mapSrc = daeSafeCast<domSource>( mesh->createAndPlace( COLLADA_ELEMENT_SOURCE ) );

			
			
			std::map<QString, int> vertexIndexes;

						
			createPositionSource(positionSrc, (*nodeIterator), vertexIndexes );
			
			createPositionAccessor(positionSrc, (*nodeIterator) );
			
			createNormalSource(normalSrc, (*nodeIterator) );
			
			createNormalAccessor(normalSrc, (*nodeIterator) );
			
			
			createMapSource( mapSrc, (*nodeIterator));
			
			createMapAccessor( mapSrc, (*nodeIterator));
			
			//createColorSource(colorSrc, (*nodeIterator));
			
			//createColorAccessor(colorSrc, (*nodeIterator));
			
			createVertices(positionSrc, (*nodeIterator), mesh , vertexIndexes, "cakeTextureSG");		

			
			for (size_t k =0; k < (*nodeIterator)->newSurfaces.size(); ++k) {
				//std::cout << "size before " << meshDocument.meshList.size() << "\n";
				//std::cout <<  " deleting " << (*nodeIterator)->newSurfaces[k]->fileName << "\n";
				meshDocument.delMesh((*nodeIterator)->newSurfaces[k]);

				//std::cout << "size after  " << meshDocument.meshList.size() << "\n";
			}
			
			
		}
		
	}
	#endif
#if 0
	for (nodeIterator = sceneNodes.begin(); nodeIterator != sceneNodes.end(); nodeIterator++) {
		for (size_t k =0; k < (*nodeIterator)->newSurfaces.size(); ++k) {
			delete (*nodeIterator)->newSurfaces[k];
		}
	}
#endif
	//std::cout << "here0\n";
//	addBubbleToCollada(daeObject ,  scene);
//std::cout << "here1\n";
//	addBubbleInstancesToCollada(daeObject, scene);	
//	std::cout << "here2\n";
}


void ColladaReconstructor::createTextureEffect(domCOLLADA *domRoot)
{

	
	// work with materials library
	
	//domLibrary_materials_Array libMaterials = domRoot->getLibrary_materials_array();
	
	//domLibrary_materials *libMaterials = daeSafeCast<domLibrary_materials>( domRoot->add("library_materials") ); //domRoot->getLibrary_materials_array();
	
	libMaterials = daeSafeCast<domLibrary_materials>( domRoot->createAndPlace(COLLADA_ELEMENT_LIBRARY_MATERIALS) );
	
	domMaterial *textureMaterial = daeSafeCast<domMaterial>( libMaterials->createAndPlace( COLLADA_ELEMENT_MATERIAL ) );
	
	textureMaterial->setId("cakeMaterial");
	textureMaterial->setName("cakeMaterial");
	
	domInstance_effect *instanceEffect = daeSafeCast<domInstance_effect>( textureMaterial->createAndPlace( COLLADA_ELEMENT_INSTANCE_EFFECT ) );
	
	instanceEffect->setUrl("#cakeTexture-fx");
	
	// add entry to library images
	
	libImages = daeSafeCast<domLibrary_images>( domRoot->createAndPlace( COLLADA_ELEMENT_LIBRARY_IMAGES ) );
	
	
	

	domImage *textureImage = daeSafeCast<domImage>( libImages->createAndPlace( COLLADA_ELEMENT_IMAGE ) );
	
	textureImage->setId("cakeTexture");
	textureImage->setName("cakeTexture");
	textureImage->setDepth(1);
	
	domImage::domInit_from *textureInitFrom = daeSafeCast<domImage::domInit_from>( textureImage->createAndPlace( COLLADA_ELEMENT_INIT_FROM) );
	textureInitFrom->setValue("./cake.png");
	
	
	// add and or get effects library
	
	//domLibrary_effects_Array libEffects = domRoot->getLibrary_effects_array();
	libEffects = daeSafeCast<domLibrary_effects>( domRoot->createAndPlace(COLLADA_ELEMENT_LIBRARY_EFFECTS) );
	
	
	domEffect *textureEffect = daeSafeCast<domEffect>( libEffects->createAndPlace( COLLADA_ELEMENT_EFFECT ) );
	textureEffect->setId("cakeTexture-fx");
	
	domProfile_COMMON *textureProfile = daeSafeCast<domProfile_COMMON>( textureEffect->createAndPlace(COLLADA_ELEMENT_PROFILE_COMMON) );
	
	
	domCommon_newparam_type *textureSurface = daeSafeCast<domCommon_newparam_type>( textureProfile->createAndPlace(COLLADA_ELEMENT_NEWPARAM) );
	textureSurface->setSid("cakeTexture-surface");
	domCommon_newparam_type *textureSampler = daeSafeCast<domCommon_newparam_type>( textureProfile->createAndPlace(COLLADA_ELEMENT_NEWPARAM) );
	textureSampler->setSid("cakeTexture-sampler");
	
	// fill in the newparams
	
	fillTextureSurface(textureSurface);
	fillTextureSampler(textureSampler);
	
	
	domProfile_COMMON::domTechnique *textureTechnique = daeSafeCast<domProfile_COMMON::domTechnique>( textureProfile->createAndPlace(COLLADA_ELEMENT_TECHNIQUE) );
	
	textureTechnique->setSid("common");
	
	domProfile_COMMON::domTechnique::domBlinn *textureShading =  daeSafeCast<domProfile_COMMON::domTechnique::domBlinn>( textureTechnique->createAndPlace( COLLADA_ELEMENT_BLINN ) );  ;
	
	// fill in shading
	
	fillTextureShading(textureShading);
	
	
	
	
	
	
}


void ColladaReconstructor::fillTextureSampler(domCommon_newparam_type *textureSampler)
{
	
	domFx_sampler2D_common *sampler = daeSafeCast<domFx_sampler2D_common>(textureSampler->createAndPlace( COLLADA_ELEMENT_SAMPLER2D) );

	domFx_sampler2D_common_complexType::domSource *source = daeSafeCast<domFx_sampler2D_common_complexType::domSource>( sampler->createAndPlace(COLLADA_ELEMENT_SOURCE) );
	
	source->setCharData("cakeTexture-surface");
	
	 
	domFx_sampler2D_common_complexType::domMinfilter *minFilter =  daeSafeCast<domFx_sampler2D_common_complexType::domMinfilter>(sampler->createAndPlace(COLLADA_ELEMENT_MINFILTER));
	minFilter->setValue(FX_SAMPLER_FILTER_COMMON_LINEAR_MIPMAP_LINEAR);

	domFx_sampler2D_common_complexType::domMagfilter *magFilter =  daeSafeCast<domFx_sampler2D_common_complexType::domMagfilter>(sampler->createAndPlace(COLLADA_ELEMENT_MAGFILTER));
	magFilter->setValue(FX_SAMPLER_FILTER_COMMON_LINEAR);
}

void ColladaReconstructor::fillTextureSurface(domCommon_newparam_type *textureSurface)
{
	domFx_surface_common *surface = daeSafeCast<domFx_surface_common>( textureSurface->createAndPlace(COLLADA_ELEMENT_SURFACE));
	
	surface->setType(FX_SURFACE_TYPE_ENUM_2D);
	
	
	domFx_surface_init_from_common *initFrom = daeSafeCast<domFx_surface_init_from_common>(surface->createAndPlace( COLLADA_ELEMENT_INIT_FROM) );
	initFrom->setCharData("cakeTexture");
	
	
}

void ColladaReconstructor::fillTextureShading(domProfile_COMMON::domTechnique::domBlinn *textureShading)
{
	domCommon_color_or_texture_type_complexType::domColor *color;
	domCommon_float_or_param_type_complexType::domFloat *floatType;
	
	// emission
	
	domCommon_color_or_texture_type *emission = daeSafeCast<domCommon_color_or_texture_type>( textureShading->createAndPlace(COLLADA_ELEMENT_EMISSION) );
	
	color = daeSafeCast<domCommon_color_or_texture_type_complexType::domColor>( emission->createAndPlace(COLLADA_ELEMENT_COLOR) );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 1 );
	
	// ambient
	
	domCommon_color_or_texture_type *ambient = daeSafeCast<domCommon_color_or_texture_type>( textureShading->createAndPlace(COLLADA_ELEMENT_AMBIENT) );
	
	color = daeSafeCast<domCommon_color_or_texture_type_complexType::domColor>( ambient->createAndPlace(COLLADA_ELEMENT_COLOR) );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 1 );
	
	// diffuse
	
	domCommon_color_or_texture_type *diffuse = daeSafeCast<domCommon_color_or_texture_type>( textureShading->createAndPlace(COLLADA_ELEMENT_DIFFUSE) );
	
	domCommon_color_or_texture_type_complexType::domTexture *diffuseTexture = daeSafeCast<domCommon_color_or_texture_type_complexType::domTexture>( diffuse->createAndPlace(COLLADA_ELEMENT_TEXTURE) );
	diffuseTexture->setAttribute("texture", "cakeTexture-sampler");
	diffuseTexture->setAttribute("texcoord", "TEX0" );
	
	// specular
	
	domCommon_color_or_texture_type *specular = daeSafeCast<domCommon_color_or_texture_type>( textureShading->createAndPlace(COLLADA_ELEMENT_SPECULAR) );
	
	color = daeSafeCast<domCommon_color_or_texture_type_complexType::domColor>( specular->createAndPlace(COLLADA_ELEMENT_COLOR) );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 1 );
	
	// shininess
	
	domCommon_float_or_param_type *shininess = daeSafeCast<domCommon_float_or_param_type>( textureShading->createAndPlace(COLLADA_ELEMENT_SHININESS) );

	/*
	 domCommon_float_or_param_type *fop = daeSafeCast< domCommon_float_or_param_type >( phong->createAndPlace( "shininess" ) );
	 domCommon_float_or_param_type_complexType::domFloat *f = daeSafeCast< domCommon_float_or_param_type_complexType::domFloat >( fop->createAndPlace( "float" ) );
	 f->setValue( shininess );
	*/
	
	floatType = daeSafeCast<domCommon_float_or_param_type_complexType::domFloat>(shininess->createAndPlace(COLLADA_ELEMENT_FLOAT) );
	
	floatType->setValue(0.3);
	
	// reflective
	
	domCommon_color_or_texture_type *reflective = daeSafeCast<domCommon_color_or_texture_type>( textureShading->createAndPlace(COLLADA_ELEMENT_REFLECTIVE) );
	
	color = daeSafeCast<domCommon_color_or_texture_type_complexType::domColor>( reflective->createAndPlace(COLLADA_ELEMENT_COLOR) );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 1 );
	
	// reflectivity
	
	domCommon_float_or_param_type *reflectivity = daeSafeCast<domCommon_float_or_param_type>( textureShading->createAndPlace(COLLADA_ELEMENT_REFLECTIVITY) );
	
	floatType = daeSafeCast<domCommon_float_or_param_type_complexType::domFloat>(reflectivity->createAndPlace(COLLADA_ELEMENT_FLOAT));
	
	floatType->setValue(0.5);

	// transparent
	
	
	domCommon_transparent_type *transparent = daeSafeCast<domCommon_transparent_type>( textureShading->createAndPlace(COLLADA_ELEMENT_TRANSPARENT) ); 
	
	color = daeSafeCast<domCommon_color_or_texture_type_complexType::domColor>( transparent->createAndPlace(COLLADA_ELEMENT_COLOR) );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 0 );
	color->getValue().append( 1 );
	
	// transparency
	
	domCommon_float_or_param_type *transparency = daeSafeCast<domCommon_float_or_param_type>( textureShading->createAndPlace(COLLADA_ELEMENT_TRANSPARENCY) );
	
	floatType = daeSafeCast<domCommon_float_or_param_type_complexType::domFloat>(transparency->createAndPlace(COLLADA_ELEMENT_FLOAT));
	
	floatType->setValue(1);
	
	
	// index_of_refraction
	
	domCommon_float_or_param_type *indexOfRefraction = daeSafeCast<domCommon_float_or_param_type>( textureShading->createAndPlace(COLLADA_ELEMENT_INDEX_OF_REFRACTION) );
	
	floatType = daeSafeCast<domCommon_float_or_param_type_complexType::domFloat>(indexOfRefraction->createAndPlace(COLLADA_ELEMENT_FLOAT));
	 
	floatType->setValue(1);
	 
}

Vector_3 ColladaReconstructor::computeSmoothNormal(Vertex& vertex)
{
	
	std::map< Facet, Vector_3 >::iterator facetNormalsIterator; 
		
	// get faces and compute normal
	Vector_3 newNormal(0, 0, 0);
	
	Polyhedron::Halfedge_around_vertex_const_circulator vertexCirculator = vertex.vertex_begin();

	do {
	 
		Facet incidentFacet = *(vertexCirculator->facet());
		
		newNormal = newNormal + getFacetsNormal(incidentFacet);
		 
 		++vertexCirculator;
		
	} while ( vertexCirculator != vertex.vertex_begin() ); 
	
	
	double length = CGAL::sqrt( CGAL::to_double(newNormal.squared_length() ) );
	
	newNormal = newNormal / length;
	return newNormal;

	
}

Vector_3 ColladaReconstructor::getFacetsNormal(Facet &facet) {
	
	
	return  CGAL::cross_product(facet.halfedge()->next()->vertex()->point() - facet.halfedge()->vertex()->point(),
								facet.halfedge()->next()->next()->vertex()->point() - facet.halfedge()->next()->vertex()->point());
	
}


void ColladaReconstructor::createNormalSource(domSource *normalSrc,  ProcessedNode* processedNode)
{
	std::string srcName = processedNode->id.toStdString() + std::string("-Normals");
	normalSrc->setId( srcName.c_str() );
	domFloat_array *floatArray = daeSafeCast<domFloat_array>( normalSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
	std::string arrayName = srcName + std::string("-array");
	int numberOfVertices = (*processedNode->newSurfaces.begin())->cm.vn ;
	floatArray->setId( arrayName.c_str() );
	floatArray->setCount( numberOfVertices * 3 );
	
	domListOfFloats &normalSrcArray = floatArray->getValue();
	
		
	
	for (CMeshO::VertexIterator vertexIterator = (*processedNode->newSurfaces.begin())->cm.vert.begin();
		 vertexIterator != (*processedNode->newSurfaces.begin())->cm.vert.end();
		 ++vertexIterator) {

		
		normalSrcArray.append3(vertexIterator->N()[0], 
							   vertexIterator->N()[1], 
							   vertexIterator->N()[2]);
		
												 
			
	}
	
	
}

void ColladaReconstructor::createNormalAccessor(domSource *normalSrc, ProcessedNode* processedNode)
{
	//create the accessor
	
	int numberOfVertices =   (*processedNode->newSurfaces.begin())->cm.vn;
	
	domSource::domTechnique_common * srcTeqC = daeSafeCast<domSource::domTechnique_common>( normalSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
	domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
	acc->setCount( numberOfVertices );
	acc->setStride( 3 );
	
	QString nodeId = processedNode->id;
	std::string srcName = nodeId.toStdString() + std::string("-Normals");
	std::string arrayName = srcName + std::string("-array");
	acc->setSource(  (std::string("#").append(arrayName)).c_str() ); 
		
	domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "X" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "Y" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "Z" );
	param->setType( "float" );
	
	
}


void ColladaReconstructor::createPositionSource(domSource *positionSrc, ProcessedNode* processedNode, std::map<QString, int> & vertexIndexes)
{
	//create the positions source.
	
	QString nodeId = processedNode->id;
	
	std::string srcName = nodeId.toStdString() + std::string("-Position");
	positionSrc->setId( srcName.c_str() );
	domFloat_array *floatArray = daeSafeCast<domFloat_array>( positionSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
	std::string arrayName = srcName + std::string("-array");
	floatArray->setId( arrayName.c_str() );
	
	
	// XXX using only the first of the surfaces (all ?)
	
	
	
	int currentVertexIndex = 0;
	int numberOfVertices = (*processedNode->newSurfaces.begin())->cm.vn;
	floatArray->setCount( numberOfVertices * 3 );
	
	domListOfFloats &posSrcArray = floatArray->getValue();
	
	
	for (CMeshO::VertexIterator vertexIterator = (*processedNode->newSurfaces.begin())->cm.vert.begin();
		 vertexIterator != (*processedNode->newSurfaces.begin())->cm.vert.end();
		 ++vertexIterator) {
		
	
		
		vertexIndexes[getPointKey(*vertexIterator)] = currentVertexIndex++;
		
		posSrcArray.append3(vertexIterator->P()[0], 
							vertexIterator->P()[1], 
							vertexIterator->P()[2]);
		
		
	}
	
}

void ColladaReconstructor::createPositionAccessor(domSource *positionSrc,  ProcessedNode* processedNode)
{
	// create position accessor
	
	int numberOfVertices =   (*processedNode->newSurfaces.begin())->cm.vn;
	
	domSource::domTechnique_common *srcTeqC = daeSafeCast<domSource::domTechnique_common>( positionSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
	domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
	acc->setCount( numberOfVertices );
	acc->setStride( 3 );
	
	
	QString nodeId = processedNode->id;
	std::string srcName = nodeId.toStdString() + std::string("-Position");
	std::string arrayName = srcName + std::string("-array");
	acc->setSource(  (std::string("#").append(arrayName)).c_str() ); 
	
	domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "X" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "Y" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "Z" );
	param->setType( "float" );
	
	
}

void ColladaReconstructor::createMapSource(domSource *mapSrc, ProcessedNode* processedNode)
{
	QString nodeId = processedNode->id;
	
	std::string srcName = nodeId.toStdString() + std::string("-map1");
	mapSrc->setId( srcName.c_str() );
	domFloat_array *floatArray = daeSafeCast<domFloat_array>( mapSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
	std::string arrayName = srcName + std::string("-array");
	floatArray->setId( arrayName.c_str() );
	
	// setup kdtree
	
	ovKDtree kdTree;
	
	
	////
	
	std::vector<Particle> newParticles;
	std::vector<Particle>::iterator particleIt;
	
			
	
	// should check for every scene if particles are outside a range
	// and cut it off after a threshold
	for (particleIt = processedNode->particles.begin();
		 particleIt != processedNode->particles.end();
		 ++particleIt) {
		newParticles.push_back(*particleIt);
			
	}
	
	for (particleIt = processedNode->bubbles.begin();
		 particleIt != processedNode->bubbles.end();
		 ++particleIt) {
			
		newParticles.push_back(*particleIt);

	}
	
	
	////
	kdTree.addNodes(newParticles);

	
	// XXX using only the first of the surfaces (all ?)
	
	
	int numberOfVertices =  (*processedNode->newSurfaces.begin())->cm.vn;
	floatArray->setCount( numberOfVertices * 2 );
	
	domListOfFloats &mapSrcArray = floatArray->getValue();
	
	std::vector<Particle *> result;
	std::vector<Particle *>::iterator resultIterator;
	
	int mapCount = 0;
	double lastTemp = 0;
	for (CMeshO::VertexIterator vertexIterator = (*processedNode->newSurfaces.begin())->cm.vert.begin();
		 vertexIterator != (*processedNode->newSurfaces.begin())->cm.vert.end();
		 ++vertexIterator) {
		
		// testing 
		
		// set somewhere around original prtl distance
		Point_3 newPoint(vertexIterator->P()[0],
						 vertexIterator->P()[1],
						 vertexIterator->P()[2]);

#if 0
                ovKDTree* res = kdTree.findNearestNeighbor(newPoint, 2 * 0.065);
		
                double sum = ((res->getData()->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__);
				

                if (sum > 0.7) {
                    sum = 0.7;
                }
                if (sum < 0.3) {
                    sum = 0.3;
                }

                if ( ! res->getData()->trueSurface ) {
                    sum *= 0.4;
                }

                mapCount++;
                mapSrcArray.append2( sum, sum );
#endif


#if 1 // buenero
		result.clear();
		double currentSmoothing = 0.1f*5.0;
		kdTree.rangeSearch(newPoint, currentSmoothing, result); //0.4
		double sum = 0;

		bool surface = false;
		int count = 0;
		double totalWeight = 0;
		for (resultIterator = result.begin(); 
			 resultIterator != result.end(); 
			 ++resultIterator) {
			
			//sum += (((*resultIterator)->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__); // buenero
			
			
			double distance = newPoint.getDistanceTo((*resultIterator)->position);
			double weight =  (*resultIterator)->massDensityRatio *  
							 kdTree.SplineWeightFunction(distance*1000.0f, currentSmoothing*1000.0f);
			//std::cout << "((*resultIterator)->mass / (*resultIterator)->density) " << ((*resultIterator)->mass / (*resultIterator)->density) << "\n";
			//std::cout << "((*resultIterator)->massDensityRatio " << (*resultIterator)->massDensityRatio ;
		//	if ((*resultIterator)->isBubble) {
//				sum +=  .1 * weight;
//			} else {
//				sum += 1.0 * weight;
//			}
			if ( distance < currentSmoothing){
				//double temp = (*resultIterator)->temperature;

#if 1 // for temperature			
				double temp = (((*resultIterator)->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__);
				//temp = .1;
				temp *= .9;
				temp += .05;
				//std::cout << (*resultIterator)->temperature << " " <<temp << "\n";
				
				sum += temp * weight;
#endif				
				//sum += temp * weight;
#if 0 // for bubbles
				if ((*resultIterator)->isBubble) {
					sum += 0.9 * weight;
				} else {
					sum += 0.1 * weight;
				}
#endif

#if 0 // for temperature and bubbles (used in volcano)			
				double temp = (((*resultIterator)->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__);
				//temp = .1;
				temp *= .5;
				temp += .05;
				//std::cout << (*resultIterator)->temperature << " " <<temp << "\n";
				
				sum += temp * weight;
				
				if ((*resultIterator)->isBubble) {
					sum += 0.25 * weight;
				}
#endif					
				
				
				
				totalWeight += weight;
				count++;
			}
			

			//if ((*resultIterator)->trueSurface) surface = true;

			/*
			if ( (*resultIterator)->trueSurface ) {
				sum += (((*resultIterator)->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__);
				//				count += 10;
				count++;


			} else {
				sum += 0.5 * (((*resultIterator)->temperature - __MIN_TEMP__)) / (__MAX_TEMP__ - __MIN_TEMP__);
				count++;
			}
			
			*/
			
		}
		
		
		//if (totalWeight > 0.0000001) {
		sum /= totalWeight;
		//}
		//sum /= count;
		//	sum += 0.1;
		
		//sum /= count;
		
//		sum *= 1.5;
//		sum += 0.1;
//		
//		
//		if (sum > .9) {
//			sum = .9;
//		}
//		
		//sum /= 1000.0;
		//sum /=8.0;
				
		//sum /= count * 1.0;
		
		//sum *= 9;
		//sum += 0.1;
		
		
		//sum /= result.size();
		
		//sum /= (120.0 - 26.0); // max temp - min temp
	
		
		// map 0 - 1 to 0.1 - 0.9 to avoid the corners of the texture
		//sum = sum * 0.8f;
		//sum += 0.1;
		
		// change to bubbles
		
		
		
		/*
		if (sum > 0.7) {
			sum = 0.7;
		}
		if (sum < 0.3) {
			sum = 0.3;
		}
		*/
        //ovKDTree* res = kdTree.findNearestNeighbor(newPoint, 2 * 0.065);

                //if ( ! surface) {
                 //      sum *= 0.3;
                //}

		

		mapCount++;
		mapSrcArray.append2( sum, sum );
		//mapSrcArray.append2((float)rand()/(float)RAND_MAX, (float)rand()/(float)RAND_MAX);
		result.clear();

#endif
	}
	//std::cout << " map count " << mapCount << "\n";
	
	
}

void ColladaReconstructor::createMapAccessor(domSource *mapSrc, ProcessedNode* processedNode)
{
	
	int numberOfVertices =   (*processedNode->newSurfaces.begin())->cm.vn;
	
	domSource::domTechnique_common *srcTeqC = daeSafeCast<domSource::domTechnique_common>( mapSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
	domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
	acc->setCount( numberOfVertices );
	acc->setStride( 2 );
	
	
	QString nodeId = processedNode->id;
	std::string srcName = nodeId.toStdString() + std::string("-map1");
	std::string arrayName = srcName + std::string("-array");
	acc->setSource(  (std::string("#").append(arrayName)).c_str() ); 
	
	domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "S" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "T" );
	param->setType( "float" );
		
	
	
}



void ColladaReconstructor::createColorSource(domSource *colorSrc, ProcessedNode* processedNode)
{
	
	//create the positions source.
	
	
	QString nodeId = processedNode->id;
	
	std::string srcName = nodeId.toStdString() + std::string("-Color");
	colorSrc->setId( srcName.c_str() );
	domFloat_array *floatArray = daeSafeCast<domFloat_array>( colorSrc->createAndPlace( COLLADA_ELEMENT_FLOAT_ARRAY ) );
	std::string arrayName = srcName + std::string("-array");
	floatArray->setId( arrayName.c_str() );
	
	
	// setup kdtree
	
	ovKDtree kdTree;
	
	kdTree.addNodes(processedNode->particles);
	
	
	// XXX using only the first of the surfaces (all ?)
	
	
	
	//int currentVertexIndex = 0;
	int numberOfVertices = (*processedNode->newSurfaces.begin())->cm.vn;
	floatArray->setCount( numberOfVertices * 3 );
	
	domListOfFloats &colorSrcArray = floatArray->getValue();
	
	std::vector<Particle *> result;
	std::vector<Particle *>::iterator resultIterator;
	
	for (CMeshO::VertexIterator vertexIterator = (*processedNode->newSurfaces.begin())->cm.vert.begin();
		 vertexIterator != (*processedNode->newSurfaces.begin())->cm.vert.end();
		 ++vertexIterator) {		
		// testing 
		
		Point_3 newPoint(vertexIterator->P()[0],
						 vertexIterator->P()[1],
						 vertexIterator->P()[2]);
		kdTree.rangeSearch(newPoint, 0.15f, result);
		double sum = 0;
		
		for (resultIterator = result.begin(); 
			 resultIterator != result.end(); 
			 ++resultIterator) {
			
			if ((*resultIterator)->trueSurface) {
				sum += (*resultIterator)->temperature - 26;
			} else {
				sum += 0.5 * ((*resultIterator)->temperature - 26);
			}
			
			if ((*resultIterator)->temperature > __MAX_TEMP__) {
				std::cout << "ke paso " << (*resultIterator)->temperature << "\n";
				
			}
		}
		
		sum /= result.size();
		
		sum /= (__MAX_TEMP__ - __MIN_TEMP__);
		
		sum = 1.0 - sum;
		
		if (sum > 0.8) {
			sum = 0.8;
		}
		
		if (sum < 0.2) {
			sum = 0.2;
		}
		
		//colorSrcArray.append3( sum, 1 - sum, sum );
		colorSrcArray.append3( sum, 1 - sum, sum );
		
		/*
		colorSrcArray.append3(((double) rand()) / RAND_MAX , 
							  ((double) rand()) / RAND_MAX , 
							  ((double) rand()) / RAND_MAX);
		
		*/
		result.clear();
	}
	
	
}


void ColladaReconstructor::createColorAccessor(domSource *colorSrc, ProcessedNode* processedNode)
{
	
	int numberOfVertices = (*processedNode->newSurfaces.begin())->cm.vn;
	
	domSource::domTechnique_common *srcTeqC = daeSafeCast<domSource::domTechnique_common>( colorSrc->createAndPlace( COLLADA_ELEMENT_TECHNIQUE_COMMON ) );
	domAccessor *acc = daeSafeCast<domAccessor>( srcTeqC->createAndPlace( COLLADA_ELEMENT_ACCESSOR ) );
	acc->setCount( numberOfVertices );
	acc->setStride( 3 );
	
	
	QString nodeId = processedNode->id;
	std::string srcName = nodeId.toStdString() + std::string("-Color");
	std::string arrayName = srcName + std::string("-array");
	acc->setSource(  (std::string("#").append(arrayName)).c_str() ); 
	
	domParam *param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "R" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "G" );
	param->setType( "float" );
	param = daeSafeCast<domParam>( acc->createAndPlace( COLLADA_ELEMENT_PARAM ) );
	param->setName( "B" );
	param->setType( "float" );
	
}

void ColladaReconstructor::createVertices(domSource *positionSrc, ProcessedNode* processedNode, domMesh *mesh, std::map<QString, int> &vertexIndexes, QString materialEasy)
{
	//create the vertices 
	
	QString nodeId = processedNode->id;
	
	//int numberOfFacets = (*processedNode->newSurfaces.begin())->cm.fn;
	
	// XXX FIX ME 
	
	int numberOfFacets = 0;
	int proxyVertex[3];
	
	//std::cout << "should go up to facet " << (*processedNode->newSurfaces.begin())->cm.fn << "\n";
	//numberOfFacets = (*processedNode->newSurfaces.begin())->cm.fn;
	
	#if 0
	for (CMeshO::FaceIterator faceIterator = (*processedNode->newSurfaces.begin())->cm.face.begin();
		 faceIterator != (*processedNode->newSurfaces.begin())->cm.face.end();
		 ++faceIterator) {
		//(*processedNode->newSurfaces.begin())->cm.sfn=0;	
		if (! faceIterator->IsD() ){
			std::map<QString, int>::iterator vertexIndexesIterator;
			for (int k=0; k<3; ++k) {
	
				vertexIndexesIterator = vertexIndexes.find( getPointKey(*(faceIterator->V(k)) )) ;
				proxyVertex[k] = vertexIndexesIterator->second;

			 
			}
		
			if (proxyVertex[0] != proxyVertex[1] &&
				proxyVertex[0] != proxyVertex[2] &&
				proxyVertex[1] != proxyVertex[2]) {
					numberOfFacets++;

			}
			//(*processedNode->newSurfaces.begin())->cm.sfn++;
		}
		
	}
	#endif

	domVertices *verts = daeSafeCast<domVertices>( mesh->createAndPlace( COLLADA_ELEMENT_VERTICES ) );
	std::string srcName = nodeId.toStdString() + std::string("-Vertex");
	std::string normalSrcName = nodeId.toStdString() + std::string("-Normals");
	//std::string colorSrcName = nodeId.toStdString() + std::string("-Color");
	std::string mapSrcName = nodeId.toStdString() + std::string("-map1");
	
	verts->setId( srcName.c_str() );
	
	domInputLocal *inputLocal = daeSafeCast<domInputLocal>( verts->createAndPlace( COLLADA_ELEMENT_INPUT ) );
	
	inputLocal->setSemantic( COMMON_PROFILE_INPUT_POSITION );
	inputLocal->setSource( ( std::string("#").append( positionSrc->getID() ) ).c_str() );
	
	
	
	
	domTriangles *polyList = daeSafeCast<domTriangles>( mesh->createAndPlace( COLLADA_ELEMENT_TRIANGLES ) );
	
	polyList->setMaterial( materialEasy.toAscii() );
	
	
	domInputLocalOffset *inputLocalOffset = daeSafeCast<domInputLocalOffset>( polyList->createAndPlace( COLLADA_ELEMENT_INPUT ) );
	inputLocalOffset->setSemantic( COMMON_PROFILE_INPUT_VERTEX );
	inputLocalOffset->setOffset( 0 );
	inputLocalOffset->setSource( ( std::string("#").append( verts->getID() ) ).c_str() );
	
	
	inputLocalOffset = daeSafeCast<domInputLocalOffset>( polyList->createAndPlace( COLLADA_ELEMENT_INPUT ) );
	inputLocalOffset->setSemantic( COMMON_PROFILE_INPUT_NORMAL );
	inputLocalOffset->setOffset( 1 );
	inputLocalOffset->setSource( ( std::string("#").append( normalSrcName ) ).c_str() );
	
	
	inputLocalOffset = daeSafeCast<domInputLocalOffset>( polyList->createAndPlace( COLLADA_ELEMENT_INPUT ) );
	inputLocalOffset->setSemantic( COMMON_PROFILE_INPUT_TEXCOORD );
	inputLocalOffset->setOffset( 2 );
	inputLocalOffset->setSource( ( std::string("#").append( mapSrcName ) ).c_str() );
	
	
	
	/*
	inputLocalOffset = daeSafeCast<domInputLocalOffset>( polyList->createAndPlace( COLLADA_ELEMENT_INPUT ) );
	inputLocalOffset->setSemantic( COMMON_PROFILE_INPUT_COLOR );
	inputLocalOffset->setOffset( 2 );
	inputLocalOffset->setSource( ( std::string("#").append( colorSrcName ) ).c_str() );
	*/
	
	//domPolylist::domVcount *vcount = daeSafeCast<domPolylist::domVcount>( polyList->createAndPlace( COLLADA_ELEMENT_VCOUNT ) );
	//each number in the vcount element represents how many vertices a polygon has. Here we deal only with triangles
	
	// for (int p=0; p < numberOfFacets ; p++) {
	// 		vcount->getValue().append(3);
	// 	}
	
	//each set of three is one number per input-offset. for this example it's vert, normal.
	domP *q = daeSafeCast<domP>( polyList->createAndPlace( COLLADA_ELEMENT_P ) );
	domListOfUInts &indices = q->getValue();
	
	
	/*
	for ( CGAL::Polyhedron_3<Kernel>::Facet_iterator facetIterator = processedNode->newSurfaces.begin()->facets_begin();
		 facetIterator != processedNode->newSurfaces.begin()->facets_end();
		 facetIterator++) {
	*/
	int faceCount = 0;
	for (CMeshO::FaceIterator faceIterator = (*processedNode->newSurfaces.begin())->cm.face.begin();
		 faceIterator != (*processedNode->newSurfaces.begin())->cm.face.end();
		 ++faceIterator) {
		
		if (! faceIterator->IsD() ){
			std::map<QString, int>::iterator vertexIndexesIterator;
		
			for (int k=0; k<3; ++k) {
	
				vertexIndexesIterator = vertexIndexes.find( getPointKey(*(faceIterator->V(k)) )) ;
				proxyVertex[k] = vertexIndexesIterator->second;

			 
			}
		
			if (proxyVertex[0] != proxyVertex[1] &&
				proxyVertex[0] != proxyVertex[2] &&
				proxyVertex[1] != proxyVertex[2]) {

					faceCount++;
					for (int k=0; k<3; ++k) {
						

						indices.append3(proxyVertex[k],
										proxyVertex[k],
										proxyVertex[k]);
										//rand() % 1);
									
					}

			}
		}
		
		// for (int k=0; k<3; ++k) {
		// 
		// 		vertexIndexesIterator = vertexIndexes.find( getPointKey(*(faceIterator->V(k)) )) ;
		// 		indices.append3(vertexIndexesIterator->second, 
		// 						vertexIndexesIterator->second, 
		// 						vertexIndexesIterator->second );
		// 		 
		// 		 
		// 	}
		
	}
	polyList->setCount( faceCount );
}

QString ColladaReconstructor::getPointKey(CVertexO &vertex)
{
	return QString("%1-%2-%3").arg(vertex.P()[0]).arg(vertex.P()[1]).arg(vertex.P()[2]);
}


void ColladaReconstructor::removeTransformations(domNode *node)
{
	domTranslate_Array tranAry = node->getTranslate_array();
	for(uint j = 0; j < tranAry.getCount(); j++) {
		daeElement::removeFromParent( tranAry[j] );
	}
	
	domRotate_Array rotAry = node->getRotate_array();
	for(uint j = 0; j < rotAry.getCount(); j++) {
		daeElement::removeFromParent( rotAry[j] );
	}
	
	domSkew_Array skeAry = node->getSkew_array();
	for(uint j = 0; j < skeAry.getCount(); j++) {
		daeElement::removeFromParent( skeAry[j] );
	}
	
	domScale_Array scaleAry = node->getScale_array();
	for(uint j = 0; j < scaleAry.getCount(); j++) {
		daeElement::removeFromParent( scaleAry[j] );
	}

}
