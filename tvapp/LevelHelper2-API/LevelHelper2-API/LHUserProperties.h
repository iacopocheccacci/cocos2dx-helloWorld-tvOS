//This file was generated automatically by LevelHelper 2
//based on the class template defined by the user.
//For more info please visit: http://www.gamedevhelper.com

#ifndef __LEVELHELPER_API_USER_PROPERTIES_H__
#define __LEVELHELPER_API_USER_PROPERTIES_H__

#include "LHNodeProtocol.h"
#include "LHUserPropertyProtocol.h"


#ifndef __LH_USER_PROPERTY_CAMERATHRESHOLD__
#define __LH_USER_PROPERTY_CAMERATHRESHOLD__
class CameraThreshold : public LHUserPropertyProtocol
{
public:
	float getZoomLowerDeltaY(){return zoomLowerDeltaY;}
	void setZoomLowerDeltaY(const float& zoomLowerDeltaYValue){zoomLowerDeltaY = zoomLowerDeltaYValue;}

	float getZoomUpperDeltaY(){return zoomUpperDeltaY;}
	void setZoomUpperDeltaY(const float& zoomUpperDeltaYValue){zoomUpperDeltaY = zoomUpperDeltaYValue;}

	float getFollowOffsetLowerDeltaY(){return followOffsetLowerDeltaY;}
	void setFollowOffsetLowerDeltaY(const float& followOffsetLowerDeltaYValue){followOffsetLowerDeltaY = followOffsetLowerDeltaYValue;}

	float getFollowOffsetUpperDeltaY(){return followOffsetUpperDeltaY;}
	void setFollowOffsetUpperDeltaY(const float& followOffsetUpperDeltaYValue){followOffsetUpperDeltaY = followOffsetUpperDeltaYValue;}

	static CameraThreshold* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "CameraThreshold";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	float zoomLowerDeltaY;
	float zoomUpperDeltaY;
	float followOffsetLowerDeltaY;
	float followOffsetUpperDeltaY;
};
#endif //__LH_USER_PROPERTY_CAMERATHRESHOLD__


#ifndef __LH_USER_PROPERTY_CAMERAFOCUS__
#define __LH_USER_PROPERTY_CAMERAFOCUS__
class CameraFocus : public LHUserPropertyProtocol
{
public:
	float getPanningSpeed(){return panningSpeed;}
	void setPanningSpeed(const float& panningSpeedValue){panningSpeed = panningSpeedValue;}

	float getZoomingSpeed(){return zoomingSpeed;}
	void setZoomingSpeed(const float& zoomingSpeedValue){zoomingSpeed = zoomingSpeedValue;}

	float getDuration(){return duration;}
	void setDuration(const float& durationValue){duration = durationValue;}

	std::string getFitMode(){return fitMode;}
	void setFitMode(const std::string& fitModeValue){fitMode = fitModeValue;}

	bool getIsOneShot(){return isOneShot;}
	void setIsOneShot(const bool& isOneShotValue){isOneShot = isOneShotValue;}

	Node* getTargetNode();
	void setTargetNode(Node* targetNodeValue){targetNode = targetNodeValue;}

	Node* getBoundsNode();
	void setBoundsNode(Node* boundsNodeValue){boundsNode = boundsNodeValue;}

	static CameraFocus* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "CameraFocus";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	float panningSpeed;
	float zoomingSpeed;
	float duration;
	std::string fitMode;
	bool isOneShot;
	std::string targetNode_uuid;
	Node* targetNode;
	std::string boundsNode_uuid;
	Node* boundsNode;
};
#endif //__LH_USER_PROPERTY_CAMERAFOCUS__


#ifndef __LH_USER_PROPERTY_CAMERAATTRACTOR__
#define __LH_USER_PROPERTY_CAMERAATTRACTOR__
class CameraAttractor : public LHUserPropertyProtocol
{
public:
	float getInnerRadius(){return innerRadius;}
	void setInnerRadius(const float& innerRadiusValue){innerRadius = innerRadiusValue;}

	float getOuterRadius(){return outerRadius;}
	void setOuterRadius(const float& outerRadiusValue){outerRadius = outerRadiusValue;}

	std::string getFitMode(){return fitMode;}
	void setFitMode(const std::string& fitModeValue){fitMode = fitModeValue;}

	static CameraAttractor* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "CameraAttractor";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	float innerRadius;
	float outerRadius;
	std::string fitMode;
};
#endif //__LH_USER_PROPERTY_CAMERAATTRACTOR__


#ifndef __LH_USER_PROPERTY_INSPIRATIONAUDIO__
#define __LH_USER_PROPERTY_INSPIRATIONAUDIO__
class InspirationAudio : public LHUserPropertyProtocol
{
public:
	std::string getSoundName(){return soundName;}
	void setSoundName(const std::string& soundNameValue){soundName = soundNameValue;}

	float getDelay(){return delay;}
	void setDelay(const float& delayValue){delay = delayValue;}

	static InspirationAudio* customClassInstanceWithNode(Node* node);

	std::string getClassName(){return "InspirationAudio";}

	virtual void setPropertiesFromDictionary(LHDictionary* dict);

private:
	std::string soundName;
	float delay;
};
#endif //__LH_USER_PROPERTY_INSPIRATIONAUDIO__



class LHUserCustomPropertiesManager
{
public:
	static LHUserPropertyProtocol* customClassInstanceWithName(const std::string& className, Node* node){
		if("CameraThreshold" == className){ return CameraThreshold::customClassInstanceWithNode(node);}
		if("CameraFocus" == className){ return CameraFocus::customClassInstanceWithNode(node);}
		if("CameraAttractor" == className){ return CameraAttractor::customClassInstanceWithNode(node);}
		if("InspirationAudio" == className){ return InspirationAudio::customClassInstanceWithNode(node);}
		return NULL;
	}

};

#endif //__LEVELHELPER_API_USER_PROPERTIES_H__
