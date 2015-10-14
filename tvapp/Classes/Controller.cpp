//
//  Controller.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 31/10/14.
//
//

#include "Controller.h"
#include "VisibleRect.h"
#include "Tutorial.h"

Controller::Controller()
{
    _controllerTutorial = nullptr;
    _rightPressed = false;
    _leftPressed = false;
    _jumpPressed = false;
    _stopNotePressed = false;
    _zoomPressed = false;
    _pausePressed = false;
}

void Controller::setTouchInput(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump)
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

void Controller::setKeyboardInput(EventKeyboard::KeyCode keyCode, bool pressed)
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

void Controller::setButtonInput(std::string buttonName, bool selected)
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

void Controller::resolveTouchMoveInput(Touch *touch, Event* event)
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

void Controller::processInput_SplitScreen(Vec2 location, Event* event, bool isTouchBegan, bool isVerticalJump)
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

void Controller::resetAllCommand()
{
    _leftPressed = false;
    _rightPressed = false;
    _jumpPressed = false;
}

void Controller::resolveTouchMoveInput_SplitScreen(Touch *touch)
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

void Controller::handleJumpBegan_SplitScreen(bool isVerticalJump)
{
    _jumpPressed = true;
    
    if (isVerticalJump)
    {
        _leftPressed = false;
        _rightPressed = false;
    }
}