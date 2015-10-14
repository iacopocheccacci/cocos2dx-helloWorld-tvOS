//
//  Controller.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 31/10/14.
//
//

#ifndef __SOP_CONTROLLER__
#define __SOP_CONTROLLER__

#include <cocos2d.h>

USING_NS_CC;

enum class eInputType : int
{
    BUTTONS = 0,
    SPLIT_SCREEN,
    KEYBOARD,
};

class Tutorial;

class Controller
{
public:
    Controller();
    virtual ~Controller() {};
    
    void setInputType(eInputType inputType) { _inputType = inputType; }
    eInputType getInputType()               { return _inputType; }
    
    void setTouchInput(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump);         // touch screen input
    void setKeyboardInput(EventKeyboard::KeyCode keyCode, bool pressed);   // keyboard input
    void setButtonInput(std::string buttonName, bool selected);
    void resolveTouchMoveInput(Touch* touch, Event *event);
    
    bool isRightPressed()       { return _rightPressed; }
    bool isLeftPressed()        { return _leftPressed; }
    bool isJumpPressed()        { return _jumpPressed; }
    bool isStopNotePressed()    { return _stopNotePressed; }
    bool isZoomPressed()        { return _zoomPressed; }
    bool isPausePressed()       { return _pausePressed; }
    
    void resetStopNotePressed() { _stopNotePressed = false; }
    void resetPausePressed()    { _pausePressed = false; }
    
    void resetAllCommand();
    void setControllerTutorial(Tutorial* controllerTutorial)            { _controllerTutorial = controllerTutorial; }
    
private:

    void resolveTouchMoveInput_SplitScreen(Touch* touch);
    void processInput_SplitScreen(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump);
    
    void handleJumpBegan_SplitScreen(bool isVerticalJump);
    
    Tutorial* _controllerTutorial;
    eInputType _inputType;
    bool _rightPressed;
    bool _leftPressed;
    bool _jumpPressed;
    bool _stopNotePressed;
    bool _zoomPressed;
    bool _pausePressed;
};

#endif // __SOP_CONTROLLER__
