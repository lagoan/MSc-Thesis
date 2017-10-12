/*
 *  ProcessedNode.cpp
 *  oven
 *
 *  Created by Omar Rodriguez-Arenas on 12/01/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "ProcessedNode.h"
#include "integrationHelper.h"
#include "CGALDefinitions.h"


ProcessedNode::ProcessedNode()
{
	//parent = NULL;
}


ProcessedNode::ProcessedNode(domNode &nodeElement )
{
	//*this = ProcessedNode();
	readNamesFromCollada(nodeElement);
	affTransformation3 = ProcessedNode::readTransformationsFromCollada(nodeElement);
	
	
	id = nodeElement.getId();
	name = nodeElement.getName();
	
	for (size_t i = 0; i < nodeElement.getNode_array().getCount(); i++) {
		children.push_back(lookup<ProcessedNode, domNode>(*nodeElement.getNode_array()[i]));
		//children.push_back(&lookup<ProcessedNode, domNode>(*nodeElement.getNode_array()[i]));
	}
	
	for (size_t i = 0; i < nodeElement.getInstance_node_array().getCount(); i++) {
		domNode* child = daeSafeCast<domNode>( nodeElement.getInstance_node_array()[i]->getUrl().getElement() );
		children.push_back( lookup<ProcessedNode, domNode>(*child));
		//children.push_back(&lookup<ProcessedNode, domNode>(*child));
	}
	
	/*
	for (size_t i = 0; i < nodeElement.getInstance_geometry_array().getCount(); i++) {
		domInstance_geometry* instanceGeom = nodeElement.getInstance_geometry_array()[i];
		InstanceGeometry instance;
		instanceGeometries.push_back( instance ); 		
		domGeometry* geom = daeSafeCast<domGeometry>(instanceGeom->getUrl().getElement());
		instanceGeometries[ instanceGeometries.size() - 1 ].geometry = &lookup<Geometry, domGeometry>(*geom);
	}
	*/
	
	if ( nodeElement.getInstance_geometry_array().getCount() ) {
		nodeType = GeometricObjectNode;
	}
	else if ( nodeElement.getInstance_light_array().getCount() ) {
		nodeType = LampNode;
	}
	else if ( nodeElement.getInstance_camera_array().getCount() ) {
		nodeType = CameraNode;
	}
	else {
		nodeType = UnknownNode;
	}		
	
	
	// get extra
	int extraCount = nodeElement.getExtra_array().getCount();
	
	for (int i=0; i < extraCount; i++) {
		
		domExtraRef extra = nodeElement.getExtra_array().get(i);
		
		if (extra != NULL) {
			processTechniqueArray( extra->getTechnique_array() );
		}
	}

	
}


void ProcessedNode::clear() {
		
	particles.clear();
	bubbles.clear();
	
	children.clear();
	newSurfaces.clear();
		
	
	
}

ProcessedNode::~ProcessedNode()
{

	for (int i=0; i < newSurfaces.size(); i++){
		delete newSurfaces[i];
	}
	clear();
}


void ProcessedNode::processTechniqueArray(domTechnique_Array & techs)
{
	
	for (uint j=0; j<techs.getCount(); j++) {
				
		domTechniqueRef tech = techs.get(j);
		
		if (tech != NULL) {
			

			QString profile = tech->getProfile();
			
			if (profile == "oven") {	
				processElementsArray( tech->getContents() );
			
			}
			 
		}
	}
}

void ProcessedNode::processElementsArray(daeElementRefArray &techContents)
{
	for (uint k=0; k<techContents.getCount(); k++) {
		
		domFloat_array* data = daeSafeCast<domFloat_array>(techContents.get( k ));
		
		if (data) {
			QString dataID( data->getID() );
			
			if ( dataID == "particlePosition") {
				processParticlePosition(data);	
			}
			else if	( dataID == "particleTemperature" ) {
				processParticleTemperature(data);
			}
			else if	( dataID == "particleMass" ) {
				processParticleMass(data);
			}
			else if	( dataID == "particleDensity" ) {
				processParticleDensity(data);
			}
			else if	( dataID == "particleOriginalDensity" ) {
				processParticleOriginalDensity(data);
			}			
			else if ( dataID == "particleMassDensityRatio" ) {
				processParticleMassDensityRatio(data);
			}
			else if ( dataID == "particleVisible" ) {
				processParticleVisible(data);
			}
			else if ( dataID == "particleTrueSurface" ) {
				processParticleTrueSurface(data);
			}
			else if ( dataID == "particleNormal" ) {
				processParticleNormal(data);
			}
			else if ( dataID == "particleSmoothingLength" ) {
				processParticleSmoothingLength(data);
			}
			else if	( dataID == "bubblePosition") {
				processBubblePosition(data);
			}
			else if	( dataID == "bubbleTemperature") {
				processBubbleTemperature(data);
			}
			else if	( dataID == "bubbleMass") {
				processBubbleMass(data);
			}
			else if	( dataID == "bubbleDensity") {
				processBubbleDensity(data);
			}
			else if	( dataID == "bubbleOriginalDensity") {
				processBubbleOriginalDensity(data);
			}
			else if	( dataID == "bubbleAddedDensity") {
				processBubbleBubbleDensity(data);
			}
			else if	( dataID == "bubbleSize") {
				processBubbleSize(data);
			}
			else if ( dataID == "bubbleMassDensityRatio" ) {
				processBubbleMassDensityRatio(data);
			}
			else if ( dataID == "bubbleVisible" ) {
				processBubbleVisible(data);
			}
			else if ( dataID == "bubbleTrueSurface" ) {
				processBubbleTrueSurface(data);
			}
			else if ( dataID == "bubbleNormal" ) {
				processBubbleNormal(data);
			}
			else if ( dataID == "bubbleSmoothingLength" ) {
				processBubbleSmoothingLength(data);
			}
		}
		
		
		
		
	}
	
}





void ProcessedNode::readNamesFromCollada(domNode& nodeElement) 
{
	if ( nodeElement.getSid() ) {
		sid	= nodeElement.getSid();
		workName = sid;
	}
	if ( nodeElement.getId() ) {	
		id	= nodeElement.getId();
		workName = id;
	}
	if ( nodeElement.getName() ) {	
		name = nodeElement.getName();
	}
	
}

static double** ProcessedNode::readSimpleTransformationsFromCollada(domNode& nodeElement)
{
	double** result;
	
	result = (double**) malloc(3 * sizeof(double*));
	
	for (int i =0; i < 3; i++) {
		result[i] = (double*) malloc(3 * sizeof(double));
	}
	
	for (int i=0; i<3; i++) {
		for (int j=0; j<3; j++) {
			result[i][j] = 0;
		}
	}
	
	return result;
}


Aff_transformation_3 ProcessedNode::readTransformationsFromCollada(domNode& nodeElement)
{
	
	
	char* typeName;
	TransformationType transformationType;
	
	Aff_transformation_3 affTransformation3 = Aff_transformation_3(CGAL::IDENTITY);
	
	for ( uint i = 0; i < nodeElement.getContents().getCount(); i++) {
		
		typeName = (char *)nodeElement.getContents()[i]->getTypeName(); 
		
		transformationType = getTransformationType(typeName);
		
		switch (transformationType) {
				
			case ovTranslate:
				
				affTransformation3 = affTransformation3 * ProcessedNode::getTranslationTransformation((domTranslate * )(domElement * )nodeElement.getContents()[i]);
				break;
				
			case ovRotate:
				
				affTransformation3 = affTransformation3 * ProcessedNode::getRotationTransformation( (domRotate * )(domElement * )nodeElement.getContents()[i]) ;
				break;
				
			case ovScale:
											
				affTransformation3 = affTransformation3 * ProcessedNode::getScalingTransformation((domScale * )(domElement * )nodeElement.getContents()[i]);
				break;
				
			case ovLookAt:
			case ovSkew:
			case ovMatrix:
			case ovUnknown:
				break;
		}
		
	}	
	
	return affTransformation3;
}

ProcessedNode::TransformationType ProcessedNode::getTransformationType(char* nameType)
{
	
	if ( strcmp( nameType, "translate" ) == 0)
		return (ovTranslate);
	if ( strcmp( nameType, "rotate" ) == 0)
		return (ovRotate); 
	if ( strcmp( nameType, "scale" ) == 0)
		return (ovScale);
	if ( strcmp( nameType, "lookat" ) == 0)
		return (ovLookAt);
	if ( strcmp( nameType, "skew" ) == 0)
		return (ovSkew);
	if ( strcmp( nameType, "matrix" ) == 0)
		return (ovMatrix);
	
	return ovUnknown;	
	
}

void ProcessedNode::processParticlePosition(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	Point_3 currentPosition;
	
	if (initialize) {
		
		for (int i=0, j =0; i < dataCount; i+=3, j++) {
			currentPosition = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			
			particles.push_back( Particle() );
			particles[j].position =  currentPosition;
		}
		
		
	}
	else {
		for (int i=0, j=0; i < dataCount; i+=3, j++) {
			
			currentPosition = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			particles[j].position =  currentPosition;

		}
	}
	
	
}

void ProcessedNode::processParticleNormal(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	Point_3 currentNormal;
	
	if (initialize) {
		
		for (int i=0, j =0; i < dataCount; i+=3, j++) {
			currentNormal = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			
			particles.push_back( Particle() );
			particles[j].normal =  currentNormal;
		}
		
		
	}
	else {
		for (int i=0, j=0; i < dataCount; i+=3, j++) {
			
			currentNormal = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			particles[j].normal =  currentNormal;
			
		}
	}
	
}	

void ProcessedNode::processParticleTemperature(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].temperature =  data->getValue()[i];
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].temperature =  data->getValue()[i];
			
		}
	}
	
	
	
	/*
	for (int i=0; i < dataCount; i++) {
		//particleTemperature.push_back( data->getValue()[i] );
	}
	 */
}

void ProcessedNode::processParticleMass(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].mass =  data->getValue()[i];
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].mass =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processParticleDensity(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].density =  data->getValue()[i];
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].density =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processParticleOriginalDensity(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].originalDensity =  data->getValue()[i];
			//std::cout << "particles[i].originalDensity " << particles[i].originalDensity << "\n";
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].originalDensity =  data->getValue()[i];
			//std::cout << "particles[i].originalDensity " << particles[i].originalDensity << "\n";
			
		}
	}
}

void ProcessedNode::processBubbleMass(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].mass =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].mass =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processBubbleDensity(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].density =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].density =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processBubbleOriginalDensity(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].originalDensity =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].originalDensity =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processBubbleBubbleDensity(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].bubbleDensity =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].bubbleDensity =  data->getValue()[i];
			
		}
	}
}


void ProcessedNode::processBubblePosition(domFloat_array* data)
{
	assert(data);
								   
	int dataCount = data->getCount();

	bool initialize = bubbles.empty();
	Point_3 currentPosition;
	
	if (initialize) {
		
		for (int i=0, j = 0; i < dataCount; i+=3, j++) {
			currentPosition = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			
			bubbles.push_back( Particle() );
			bubbles[j].position =  currentPosition;
			bubbles[j].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0, j=0; i < dataCount; i+=3, j++) {
			currentPosition = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			bubbles[j].isBubble = true;
			bubbles[j].position =  currentPosition;
		}
	}
	
	/*
	for (int i=0; i < dataCount; i+=3) {
		bubblePosition.push_back( Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] ) );
	}
	*/
}

void ProcessedNode::processBubbleNormal(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	Point_3 currentNormal;
	
	if (initialize) {
		
		for (int i=0, j = 0; i < dataCount; i+=3, j++) {
			currentNormal = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			
			bubbles.push_back( Particle() );
			bubbles[j].normal =  currentNormal;
			bubbles[j].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0, j=0; i < dataCount; i+=3, j++) {
			currentNormal = Point_3( data->getValue()[i], data->getValue()[i + 1], data->getValue()[i + 2] );
			bubbles[j].isBubble = true;	
			bubbles[j].normal =  currentNormal;
		}
	}
}	

void ProcessedNode::processBubbleTemperature(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].temperature =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].temperature =  data->getValue()[i];
			
		}
	}
	
	/*
	for (int i=0; i < dataCount; i++) {
		//bubbleTemperature.push_back( data->getValue()[i] );
	}
	*/
}

void ProcessedNode::processBubbleSize(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].size =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].size =  data->getValue()[i];
			
		}
	}
	
	
	/*
	for (int i=0; i < dataCount; i++) {
		//bubbleSize.push_back( data->getValue()[i] );
	}
	*/
}


void ProcessedNode::processParticleMassDensityRatio(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].massDensityRatio =  data->getValue()[i];
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].massDensityRatio =  data->getValue()[i];
			
		}
	}
	
}

void ProcessedNode::processBubbleMassDensityRatio(domFloat_array* data)
{
	
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].massDensityRatio =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].massDensityRatio =  data->getValue()[i];
			
		}
	}
	
	
}

void ProcessedNode::processParticleVisible(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].visible =  data->getValue()[i] == 1;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].visible =  data->getValue()[i] == 1;
			
		}
	}
	
}


void ProcessedNode::processBubbleVisible(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].visible =  data->getValue()[i] == 1;
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].visible =  data->getValue()[i] == 1;
			
		}
	}
	
	
}

void ProcessedNode::processParticleTrueSurface(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].trueSurface =  data->getValue()[i] == 1;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].trueSurface =  data->getValue()[i] == 1;
			
		}
	}
	
}

void ProcessedNode::processBubbleTrueSurface(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].trueSurface =  data->getValue()[i] == 1;
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].trueSurface =  data->getValue()[i] == 1;
			
		}
	}
	
}

void ProcessedNode::processParticleSmoothingLength(domFloat_array* data)
{
	assert(data);
	
	int dataCount = data->getCount();
	bool initialize = particles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			particles.push_back( Particle() );
			particles[i].smoothingLength =  data->getValue()[i];

		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			
			particles[i].smoothingLength =  data->getValue()[i];
			
		}
	}
}

void ProcessedNode::processBubbleSmoothingLength(domFloat_array* data)
{
		assert(data);
	
	int dataCount = data->getCount();
	bool initialize = bubbles.empty();
	
	if (initialize) {
		
		for (int i=0; i < dataCount; i++) {
			
			bubbles.push_back( Particle() );
			bubbles[i].smoothingLength =  data->getValue()[i];
			bubbles[i].isBubble = true;
		}
		
		
	}
	else {
		for (int i=0; i < dataCount; i++) {
			bubbles[i].isBubble = true;
			bubbles[i].smoothingLength =  data->getValue()[i];
			
		}
	}
}

Aff_transformation_3 ProcessedNode::getRotationTransformation(domRotateRef rotateArray)
{
	assert( rotateArray->getValue().getCount() == 4 );
	
	double x = 0;
	double y = 0; 
	double z = 0;
	double angle = 0; 
	double _norm = 0;
	double _sin = 0;
	double _cos = 0;
	const double pi = 4.0f * std::atan(1.0f);
	
	x = rotateArray->getValue()[0];
	y = rotateArray->getValue()[1];
	z = rotateArray->getValue()[2];
	angle = rotateArray->getValue()[3];
	
	_norm = sqrt(x * x + y * y + z * z);
	_cos = cos( angle * pi / 180.0f );
	_sin = sin( angle * pi / 180.0f );
	
	x /= _norm;
	y /= _norm;
	z /= _norm;
	
	return Aff_transformation_3(					 
		x * x * ( 1 - _cos ) + _cos,
		x * y * ( 1 - _cos ) - z * _sin,
		x * z * ( 1 - _cos ) + y * _sin,
		0,
		y * x * ( 1 - _cos ) + z * _sin,
		y * y * ( 1 - _cos ) + _cos,
		y * z * ( 1 - _cos ) - x * _sin,
		0,
		x * z * ( 1 - _cos ) - y * _sin,
		y * z * ( 1 - _cos ) + x * _sin,
		z * z * ( 1 - _cos ) + _cos,
		0
	);
	
}

Aff_transformation_3 ProcessedNode::getTranslationTransformation(domTranslateRef translateArray)
{
	assert( translateArray->getValue().getCount() == 3 ); 
	
	return Aff_transformation_3(CGAL::TRANSLATION, Vector_3(translateArray->getValue()[0],
															translateArray->getValue()[1],
															translateArray->getValue()[2]) );
}

Aff_transformation_3 ProcessedNode::getScalingTransformation(domScaleRef scaleArray)
{
	assert( scaleArray->getValue().getCount() == 3 ); 
	
	double x = scaleArray->getValue()[0];
	double y = scaleArray->getValue()[1];
	double z = scaleArray->getValue()[2];
	
	return Aff_transformation_3(x, 0, 0, 0,
								0, y, 0, 0,
								0, 0, z, 0);
	
	
}




