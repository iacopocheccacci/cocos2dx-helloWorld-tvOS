//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#include "LHUtils.h"
#include "LHScene.h"
#include "LHUserProperties.h"

CameraThreshold* CameraThreshold::customClassInstanceWithNode(Node* n)
{
	CameraThreshold* instance = new CameraThreshold();
	instance->setNode(n);
	return instance;
}

void CameraThreshold::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("zoomLowerDeltaY"))
		setZoomLowerDeltaY(dict->floatForKey("zoomLowerDeltaY"));

	if(dict->objectForKey("zoomUpperDeltaY"))
		setZoomUpperDeltaY(dict->floatForKey("zoomUpperDeltaY"));

	if(dict->objectForKey("followOffsetLowerDeltaY"))
		setFollowOffsetLowerDeltaY(dict->floatForKey("followOffsetLowerDeltaY"));

	if(dict->objectForKey("followOffsetUpperDeltaY"))
		setFollowOffsetUpperDeltaY(dict->floatForKey("followOffsetUpperDeltaY"));

}



CameraFocus* CameraFocus::customClassInstanceWithNode(Node* n)
{
	CameraFocus* instance = new CameraFocus();
	instance->setNode(n);
	return instance;
}

void CameraFocus::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("panningSpeed"))
		setPanningSpeed(dict->floatForKey("panningSpeed"));

	if(dict->objectForKey("zoomingSpeed"))
		setZoomingSpeed(dict->floatForKey("zoomingSpeed"));

	if(dict->objectForKey("duration"))
		setDuration(dict->floatForKey("duration"));

	if(dict->objectForKey("fitMode"))
		setFitMode(dict->stringForKey("fitMode"));

	if(dict->objectForKey("isOneShot"))
		setIsOneShot(dict->boolForKey("isOneShot"));

	if(dict->objectForKey("targetNode"))
		targetNode_uuid = dict->stringForKey("targetNode");

	if(dict->objectForKey("boundsNode"))
		boundsNode_uuid = dict->stringForKey("boundsNode");

}

Node* CameraFocus::getTargetNode(){
	if(!targetNode && targetNode_uuid.length()>0){
		LHScene* scene = (LHScene*)getNode()->getScene();
		targetNode= scene->getChildNodeWithUUID(targetNode_uuid);
	}
	return targetNode;
}

Node* CameraFocus::getBoundsNode(){
	if(!boundsNode && boundsNode_uuid.length()>0){
		LHScene* scene = (LHScene*)getNode()->getScene();
		boundsNode= scene->getChildNodeWithUUID(boundsNode_uuid);
	}
	return boundsNode;
}



CameraAttractor* CameraAttractor::customClassInstanceWithNode(Node* n)
{
	CameraAttractor* instance = new CameraAttractor();
	instance->setNode(n);
	return instance;
}

void CameraAttractor::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("innerRadius"))
		setInnerRadius(dict->floatForKey("innerRadius"));

	if(dict->objectForKey("outerRadius"))
		setOuterRadius(dict->floatForKey("outerRadius"));

	if(dict->objectForKey("fitMode"))
		setFitMode(dict->stringForKey("fitMode"));

}



InspirationAudio* InspirationAudio::customClassInstanceWithNode(Node* n)
{
	InspirationAudio* instance = new InspirationAudio();
	instance->setNode(n);
	return instance;
}

void InspirationAudio::setPropertiesFromDictionary(LHDictionary* dict)
{

	if(dict->objectForKey("soundName"))
		setSoundName(dict->stringForKey("soundName"));

	if(dict->objectForKey("delay"))
		setDelay(dict->floatForKey("delay"));

}


