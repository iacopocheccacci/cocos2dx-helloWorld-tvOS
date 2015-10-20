//
//  InputController.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 31/10/14.
//
//
#include "InputController.h"
#include "VisibleRect.h"
#include "Tutorial.h"

InputController::InputController()
{
    _controllerTutorial = nullptr;
    _rightPressed = false;
    _leftPressed = false;
    _jumpPressed = false;
    _stopNotePressed = false;
    _zoomPressed = false;
    _pausePressed = false;
}

void InputController::setTouchInput(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump)
{
    switch (_inputType)
    {
        case eInputType::KEYBOARD:
        case eInputType::SPLIT_SCREEN:
            processInput_SplitScreen(location, event, isTouchBegan, isVerticalJump);
            break;
            
        default:
            break;
    }
}

void InputController::setKeyboardInput(EventKeyboard::KeyCode keyCode, bool pressed)
{
    switch (keyCode)
    {
        case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
            _rightPressed = pressed;
            break;
            
        case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
            _leftPressed = pressed;
            break;
            
        case EventKeyboard::KeyCode::KEY_SPACE:
            _jumpPressed = pressed;
            break;
            
        case EventKeyboard::KeyCode::KEY_X:
            _stopNotePressed = pressed;
            break;
            
        case EventKeyboard::KeyCode::KEY_Z:
            _zoomPressed = pressed;
            break;
            
        case EventKeyboard::KeyCode::KEY_P:
            _pausePressed = pressed;
            break;
            
        default:
            CCLOG("Invalid key pressed on keyboard");
            break;
    }
}

void InputController::setGamePadInput(cocos2d::Controller *controller, int keyCode, bool isPressed)
{
    switch (keyCode)
    {
        case cocos2d::Controller::Key::BUTTON_A:
            _jumpPressed = isPressed;
            break;
        case cocos2d::Controller::Key::BUTTON_B:
            CCLOG("Button B Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_X:
            CCLOG("Button X Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_Y:
            CCLOG("Button Y Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_DPAD_UP:
            CCLOG("Button DPAD_UP Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_DPAD_DOWN:
            CCLOG("Button DPAD_Down Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_DPAD_LEFT:
            _leftPressed = isPressed;
            break;
        case cocos2d::Controller::Key::BUTTON_DPAD_RIGHT:
            _rightPressed = isPressed;
            break;
        case cocos2d::Controller::Key::BUTTON_LEFT_SHOULDER:
            CCLOG("Button Left Shoulder Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_RIGHT_SHOULDER:
            CCLOG("Button Right Shoulder Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_LEFT_THUMBSTICK:
            CCLOG("Button Left ThumbStick Pressed");
            break;
        case cocos2d::Controller::Key::BUTTON_RIGHT_THUMBSTICK:
            CCLOG("Button Right Shoulder Pressed");
            break;
        default:
        {
            char ketStatus[30];
            sprintf(ketStatus,"Key Down:%d",keyCode);
            break;
        }
    }
}

void InputController::setButtonInput(std::string buttonName, bool selected)
{
    if (strcmp(buttonName.c_str(), appParams::LEFT_BUTTON_NAME) == 0)
    {
        _leftPressed  = selected;
    }
    else if (strcmp(buttonName.c_str(), appParams::RIGHT_BUTTON_NAME) == 0)
    {
        _rightPressed = selected;
    }
    else if (strcmp(buttonName.c_str(), appParams::JUMP_BUTTON_NAME) == 0)
    {
        _jumpPressed = selected;
    }
}

void InputController::resolveTouchMoveInput(Touch *touch, Event* event)
{
    switch (_inputType)
    {
        case eInputType::SPLIT_SCREEN:
            this->resolveTouchMoveInput_SplitScreen(touch);
            break;
            
        default:
            break;
    }
}

void InputController::processInput_SplitScreen(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump)
{
    Rect rect = VisibleRect::getVisibleRect();
    
    if (isTouchBegan)
    {
        if (location.x > rect.getMidX())
        {
            // Right side touch on screen
            if (_leftPressed)
            {
                if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
                {
                    _controllerTutorial->bothButtonsPressed(true);
                }
                
                handleJumpBegan_SplitScreen(isVerticalJump);
            }
            else
            {
                if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
                {
                    _controllerTutorial->rightButtonPressed(true);
                }
                
                _rightPressed = true;
            }
        }
        else if (location.x < rect.getMidX())
        {
            // Left side touch on screen
            if (_rightPressed)
            {
                if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
                {
                    _controllerTutorial->bothButtonsPressed(true);
                }
                
                handleJumpBegan_SplitScreen(isVerticalJump);
            }
            else
            {
                if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
                {
                    _controllerTutorial->leftButtonPressed(true);
                }
                
                _leftPressed = true;
            }
        }
    }
    else
    {
        if (location.x > rect.getMidX())
        {
            // Right side release on screen
            _rightPressed = false;
                
            if (_jumpPressed)
            {
                _leftPressed = true;
                _jumpPressed = false;
            }
            
            if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
            {
                _controllerTutorial->rightButtonPressed(false);
            }
        }
        else if (location.x < rect.getMidX())
        {
            // Left side release on screen
            
            _leftPressed = false;
                
            if (_jumpPressed)
            {
                _rightPressed = true;
                _jumpPressed = false;
            }
            
            if (_controllerTutorial && _controllerTutorial->getIscontrollerGuideEnabled())
            {
                _controllerTutorial->leftButtonPressed(false);
            }
        }
    }
}

void InputController::resetAllCommand()
{
    _leftPressed = false;
    _rightPressed = false;
    _jumpPressed = false;
}

void InputController::resolveTouchMoveInput_SplitScreen(Touch *touch)
{
    Rect rect = VisibleRect::getVisibleRect();
    
    if (!_jumpPressed)
    {
        if (touch->getStartLocation().x > rect.getMidX() && touch->getPreviousLocation().x < rect.getMidX())
        {
            _rightPressed = false;
            _leftPressed = true;
        }
        else if (touch->getStartLocation().x < rect.getMidX() && touch->getPreviousLocation().x > rect.getMidX())
        {
            _leftPressed = false;
            _rightPressed = true;
        }
        else if (touch->getStartLocation().x > rect.getMidX() && touch->getPreviousLocation().x > rect.getMidX())
        {
            _leftPressed = false;
            _rightPressed = true;
        }
        else if (touch->getStartLocation().x < rect.getMidX() && touch->getPreviousLocation().x < rect.getMidX())
        {
            _rightPressed = false;
            _leftPressed = true;
        }
    }
}

void InputController::handleJumpBegan_SplitScreen(bool isVerticalJump)
{
    _jumpPressed = true;
    
    if (isVerticalJump)
    {
        _leftPressed = false;
        _rightPressed = false;
    }
}
