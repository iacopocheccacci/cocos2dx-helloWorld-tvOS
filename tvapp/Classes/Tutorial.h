//
//  Tutorial.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 16/03/15.
//
//

#ifndef __SOP__TUTORIAL__
#define __SOP__TUTORIAL__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"

USING_NS_CC;

enum class eTutorialState
{
    TUTORIAL_NONE = 0,
    TUTORIAL_CONTROLLER_LEFT,
    TUTORIAL_CONTROLLER_RIGHT,
    TUTORIAL_CONTROLLER_JUMP_LEFT,
    TUTORIAL_CONTROLLER_JUMP_RIGHT,
    TUTORIAL_CONTROLLER_JUMP_VERTICAL,
    TUTORIAL_CONTROLLER_VIDEO,
};

enum class eTutorialAction
{
    EXPIRATION = 0,
};

class AnimationComponent;

namespace tutorialDefs
{
    static const char* CLASS_NAME = "Tutorial";
    static const char* GUIDE_NODE = "GuideNode";
    static const char* VIDEO_GUIDE = "tutorial";
    static const char* CENTRAL_LINE_SPRITE = "CentralLine";
    static const char* GUIDE_LEFT_SPRITE = "LeftGuideBtn";
    static const char* GUIDE_RIGHT_SPRITE = "RightGuideBtn";
    static const char* GLOW_SPRITE = "UI/tutorialGlow.png";
    static const char* GUIDE_LEFT_FRAME_PRESS_FIRST = "thumb_00.png";
    static const char* GUIDE_RIGHT_FRAME_PRESS_FIRST = "thumb_00.png";
    static const char* GUIDE_LEFT_FRAME_PRESS_SECOND = "thumb_01.png";
    static const char* GUIDE_RIGHT_FRAME_PRESS_SECOND = "thumb_01.png";
    static const char* GUIDE_LEFT_FRAME_HOLD = "thumb_02.png";
    static const char* GUIDE_RIGHT_FRAME_HOLD = "thumb_02.png";
    static const float START_DELAY_TIME = 2.0f;
    static const int ANIM_FRAME_NUMBER = 2;
    static const float FADE_TIME = 1.0f;
    static const float EXPIRATION_TIME = 30.0f;
    static const int VIDEO_ANIMATION_TAG = 101;
}

class Tutorial : public LHNode
{
public:
    NODEWITHDICT_FUNC(Tutorial);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    virtual ~Tutorial();
    
    void initVideoGuide();
    void initControllerGuide();
    void setIscontrollerGuideEnabled(bool enabled);
    bool getIscontrollerGuideEnabled()                        { return _controllerGuideEnabled; }
    
    void setTutorialState(eTutorialState newState);
    
    void leftButtonPressed(bool begin);
    void rightButtonPressed(bool begin);
    void bothButtonsPressed(bool begin);
    
private:
    void initVideoTutorial();
    
    void initSprites();
    void initGlow();
    void initAnimation();
    void initAnimationTap();
    void initAnimationLongPress();
    
    void showVideoTutorial();
    void showCentralLine();
    void showGuideLeft();
    void showGuideRight();
    void showGlowLeft();
    void showGlowRight();
    
    void hideVideoTutorial();
    void hideCentralLine();
    void hideGuideLeft();
    void hideGuideRight();
    void hideGlowLeft();
    void hideGlowRight();
    
    void toggleGuideLeftPressed(bool pressed);
    void toggleGuideRightPressed(bool pressed);
    
    void startAnimateGuideLeft(eTutorialState state);
    void startAnimateGuideRight(eTutorialState state);
    void stopAnimationGuideLeft();
    void stopAnimationGuideRight();
    
    bool _controllerGuideEnabled;
    eTutorialState _state;
    
    LHSprite * _video;
    Sprite * _centralLine;
    Sprite * _guideLeft;
    Sprite * _guideRight;
    
    Node * _glowLeft;
    Node * _glowRight;
    
    ActionInterval * _animTap;
    ActionInterval * _animLongPress;
    
    bool _leftPressed;
    bool _rightPressed;
};

#endif // __SOP__TUTORIAL__
