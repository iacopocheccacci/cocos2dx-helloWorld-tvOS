//
//  Portal.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 13/02/15.
//
//

#ifndef __SOP__PORTAL__
#define __SOP__PORTAL__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"

USING_NS_CC;

namespace portalDefs
{
    static const char* CLASS_NAME = "Portal";
    static const char* PORTAL_LIGHT_SPRITE_NAME = "PortalLightSprite";
    static const char* SHEEP_ICON_ACTIVE_SPRITE_NAME = "SheepIconActiveSprite";
    static const char* SHEEP_ICON_DISABLED_SPRITE_NAME = "SheepIconDisabledSprite";
    static const char* SHEEP_ICON_COMPLETE_SPRITE_NAME = "SheepIconCompleteSprite";
    static const char* FLUTE_ICON_ACTIVE_SPRITE_NAME = "FluteIconActiveSprite";
    static const char* FLUTE_ICON_DISABLED_SPRITE_NAME = "FluteIconDisabledSprite";
    static const char* FLUTE_ICON_COMPLETE_SPRITE_NAME = "FluteIconCompleteSprite";
    static const char* DESTINATION_LABEL_NODE_NAME = "DestinationLabelNode";
    
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
    static const char* PARTICLE_FILE = "particle_portal_light.plist";
#else
    static const char* PARTICLE_FILE = "particle_portal_heavy.plist";
#endif
    static const char* PARTICLE_UNDER_NAME = "particleUnder";
    static const float PARTICLE_RESIZE_TIME = 100.0f;
    static const float PARTICLE_OPACITY_TIME = 30.0f;
    static const float PARTICLE_IDLE_OPACITY_MULTIPLIER = 0.25f;
    static const float PARTICLE_DELAY = 40.0f;
    static const char* PARTICLE_TRANSITION_SCHEDULER_KEY = "particleTransitionSchedulerKey";
    static const int   PARTICLE_TOP_Z_ORDER = 32;
    
    static const Color4B DESTINATION_COLOR_ON = {255, 255, 255, 255};
    static const Color4B DESTINATION_COLOR_OFF = {128, 128, 128, 128};
    static const char* DESTINATION_LABEL_NAME = "DestinationLabel";
    static const char* DESTINATION_LABEL_EXIT_VALUE = "EXIT";
    static const float DESTINATION_LABEL_SCALE = 0.5f;
}

enum class ePortalState
{
    CLOSED_PORTAL = 0,
    OPEN_PORTAL,
    ENTERING_PORTAL,
};
    
enum class ePortalType
{
    NORMAL = 0,
    WORLD,
    LEVEL,
    EXIT
};
    
class SoundComponent;
enum class eScoreIconState;

class Portal : public LHNode
{
public:
    NODEWITHDICT_FUNC(Portal);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    ePortalState getState()                                                     { return _state; }
    void setState(ePortalState state)                                           { _state = state; }
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    void open();
    void openWithTransition(float time);
    void openFake(float time);
    void close();
    void closeWithTransition(float time);
    void setPlayerIn(bool playerIn)                                             { _playerIn = playerIn; }
    
    void initDecorations(Node * gameWorld);
    void setScoreSheepComplete(bool complete, float transitionTime);
    
    int getDestinationWorldNumber()                                             { return _worldDestination; }
    int getDestinationLevelNumber()                                             { return _levelDestination; }
    ePortalType getPortalType()                                                 { return _portalType; }
    
    void playSoundOpen();
    void stopSoundOpen();
    
    void setCompleteScoreSheep(float transitionTime = 0.0f);
    void setCompleteScoreFlute(float transitionTime = 0.0f);
    void removeScoreFluteIcon();
    
private:
    virtual void update(float dt);
    std::string getPortalName();
    
    void initAudio();
    void initPortalDestination();
    void initParticle(Node * gameWorld);
    void initPortalLight();
    void initScore();
    void initDestinationLabel();
    
    void setActiveParticle(bool active, float transitionTime = 0.0f);
    void setActiveDestinationLabel(bool active, float transitionTime = 0.0f);
    void setActivePortalLight(bool active, float transitionTime = 0.0f);
    void setStateScoreSheep(eScoreIconState newState, float transitionTime = 0.0f);
    void setStateScoreFlute(eScoreIconState newState, float transitionTime = 0.0f);
    
    ePortalState _state;
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
    float _timer;
    float _timerDelay;
    bool _playerIn;
    SoundComponent* _soundOpen;
    bool _isEntered;
    
    Color4F _originalEmitterStartColor;
    Color4F _originalEmitterStartColorVar;
    Color4F _originalEmitterEndColor;
    Color4F _originalEmitterEndColorVar;
    
    float _particleGlobalAlphaMultiplier;
    
    ParticleSystemQuad * _emitterTop;
    
    ePortalType _portalType;
    int _levelDestination;
    int _worldDestination;
};

#endif // __SOP__PORTAL__
