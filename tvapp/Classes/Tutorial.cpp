//
//  Tutorial.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 16/03/15.
//
//

#include "Tutorial.h"
#include "AnimationComponent.h"
#include "VisibleRect.h"
#include "Definitions.h"
#include "cocosGUI.h"
#include "Utility.h"

Tutorial::~Tutorial()
{
    this->stopAnimationGuideLeft();
    this->stopAnimationGuideRight();
    CC_SAFE_RELEASE(_animTap);
    CC_SAFE_RELEASE(_animLongPress);
}

bool Tutorial::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _state = eTutorialState::TUTORIAL_NONE;
        _controllerGuideEnabled = false;
        
        this->initSprites();
 //       this->initGlow();
        this->initAnimation();
    }
    
    return status;
}

void Tutorial::initSprites()
{
    _video = dynamic_cast<LHSprite*>(this->getChildNodeWithName(tutorialDefs::VIDEO_GUIDE));
    if (_video)
    {
        _video->setOpacity(0);
    }
    
    _centralLine = dynamic_cast<LHSprite*>(this->getChildNodeWithName(tutorialDefs::CENTRAL_LINE_SPRITE));
    if (_centralLine)
    {
        _centralLine->setOpacity(0);
        // TEMP !! Se la linea centrale dev'essere definitivamente rimossa, farlo da Level Helper.
        _centralLine = nullptr;
    }
    
    _guideLeft = dynamic_cast<LHSprite*>(this->getChildNodeWithName(tutorialDefs::GUIDE_LEFT_SPRITE));
    if (_guideLeft)
    {
        _guideLeft->setPosition(this->convertToNodeSpace(VisibleRect::leftBottom()));
        _guideLeft->setOpacity(0);
    }
    
    _guideRight = dynamic_cast<LHSprite*>(this->getChildNodeWithName(tutorialDefs::GUIDE_RIGHT_SPRITE));
    if (_guideRight)
    {
        _guideRight->setPosition(this->convertToNodeSpace(VisibleRect::rightBottom()));
        _guideRight->setOpacity(0);
    }
}

void Tutorial::initGlow()
{
    Size visibleSize = VisibleRect::getVisibleRect().size;
    Size glowSize = Size(visibleSize.width / 2.0f, visibleSize.height);
    Sprite * sprite = Sprite::create(tutorialDefs::GLOW_SPRITE);
    
    _glowLeft = ui::Scale9Sprite::createWithSpriteFrame(sprite->getSpriteFrame(), Rect(40,40,40,40));
    _glowLeft->setAnchorPoint(Point(1.0f, 0.0f));
    _glowLeft->setContentSize(glowSize);
    _glowLeft->setPosition(this->convertToNodeSpace(VisibleRect::bottom()));
    _glowLeft->setOpacity(0);
    this->addChild(_glowLeft);
    
    _glowRight = ui::Scale9Sprite::createWithSpriteFrame(sprite->getSpriteFrame(), Rect(40,40,40,40));
    _glowRight->setAnchorPoint(Point(0.0f, 0.0f));
    _glowRight->setContentSize(glowSize);
    _glowRight->setPosition(this->convertToNodeSpace(VisibleRect::bottom()));
    _glowRight->setOpacity(0);
    this->addChild(_glowRight);
}

void Tutorial::initAnimation()
{
    this->initAnimationTap();
    this->initAnimationLongPress();
}

void Tutorial::initAnimationTap()
{
    DelayTime * delayA = DelayTime::create(0.7f);
    CallFuncN * releaseCallback = CallFuncN::create([this](Node * node){
        Sprite * sprite = dynamic_cast<Sprite *>(node);
        if (sprite)
        {
            if ((sprite == _guideLeft && !_leftPressed) || (sprite == _guideRight && !_rightPressed))
            {
                sprite->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_SECOND);
            }
        }
    });
    DelayTime * delayB = DelayTime::create(0.3f);
    CallFuncN * pressCallback = CallFuncN::create([this](Node * node){
        Sprite * sprite = dynamic_cast<Sprite *>(node);
        if (sprite)
        {
            if ((sprite == _guideLeft && !_leftPressed) || (sprite == _guideRight && !_rightPressed))
            {
                sprite->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_FIRST);
            }
        }
    });
    _animTap = RepeatForever::create(Sequence::create(pressCallback, delayA, releaseCallback, delayB, NULL));
    CC_SAFE_RETAIN(_animTap);
}

void Tutorial::initAnimationLongPress()
{
    DelayTime * delayA = DelayTime::create(1.0f);
    CallFuncN * releaseCallback = CallFuncN::create([this](Node * node){
        Sprite * sprite = dynamic_cast<Sprite *>(node);
        if (sprite)
        {
            if ((sprite == _guideLeft && !_leftPressed) || (sprite == _guideRight && !_rightPressed))
            {
                sprite->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_SECOND);
            }
        }
    });
    DelayTime * delayB = DelayTime::create(3.0f);
    CallFuncN * pressCallback = CallFuncN::create([this](Node * node){
        Sprite * sprite = dynamic_cast<Sprite *>(node);
        if (sprite)
        {
            if ((sprite == _guideLeft && !_leftPressed) || (sprite == _guideRight && !_rightPressed))
            {
                sprite->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_FIRST);
            }
        }
    });
    _animLongPress = RepeatForever::create(Sequence::create(pressCallback, delayA, releaseCallback, delayB, NULL));
    CC_SAFE_RETAIN(_animLongPress);
}

void Tutorial::initVideoGuide()
{
    if (_video)
    {
        _controllerGuideEnabled = true;
    }
}

void Tutorial::initControllerGuide()
{
    DelayTime* delay = DelayTime::create(tutorialDefs::START_DELAY_TIME);
    CallFunc* fadeInCallback = CallFunc::create([this]()
    {
        this->setTutorialState(eTutorialState::TUTORIAL_CONTROLLER_RIGHT);
        this->showCentralLine();
    });
    
    auto action = Sequence::create(delay, fadeInCallback, NULL);
    this->runAction(action);
    
    _controllerGuideEnabled = true;
}

void Tutorial::setIscontrollerGuideEnabled(bool enabled)
{
    if (_controllerGuideEnabled != enabled)
    {
        if (false == enabled)
        {
            this->setTutorialState(eTutorialState::TUTORIAL_NONE);
            this->hideCentralLine();
        }
        else
        {
            this->showCentralLine();
        }
        _controllerGuideEnabled = enabled;
    }
}

void Tutorial::setTutorialState(eTutorialState newState)
{
    if (newState != _state && true == _controllerGuideEnabled)
    {
        switch (newState)
        {
            case eTutorialState::TUTORIAL_NONE:
                this->hideVideoTutorial();
                this->hideGuideLeft();
                this->hideGuideRight();
                this->hideGlowLeft();
                this->hideGlowRight();
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_LEFT:
                this->hideGuideRight();
                this->hideGlowRight();
                this->showGuideLeft();
                this->showGlowLeft();
                this->startAnimateGuideLeft(newState);
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_RIGHT:
                this->hideGuideLeft();
                this->hideGlowLeft();
                this->showGuideRight();
                this->showGlowRight();
                this->startAnimateGuideRight(newState);
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_LEFT:
                this->showGuideLeft();
                this->showGuideRight();
                this->showGlowLeft();
                this->showGlowRight();
                this->startAnimateGuideLeft(newState);
                this->startAnimateGuideRight(newState);
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_RIGHT:
                this->showGuideLeft();
                this->showGuideRight();
                this->showGlowLeft();
                this->showGlowRight();
                this->startAnimateGuideLeft(newState);
                this->startAnimateGuideRight(newState);
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_VERTICAL:
                this->showGuideLeft();
                this->showGuideRight();
                this->showGlowLeft();
                this->showGlowRight();
                this->startAnimateGuideLeft(newState);
                this->startAnimateGuideRight(newState);
                break;
                
            case eTutorialState::TUTORIAL_CONTROLLER_VIDEO:
                this->showVideoTutorial();
                break;
                
            default:
                break;
        }
        _state = newState;
    }
}

void Tutorial::leftButtonPressed(bool begin)
{
    this->toggleGuideLeftPressed(begin);
}

void Tutorial::rightButtonPressed(bool begin)
{
    this->toggleGuideRightPressed(begin);
}

void Tutorial::bothButtonsPressed(bool begin)
{
    this->toggleGuideLeftPressed(begin);
    this->toggleGuideRightPressed(begin);
}

#pragma mark - Private Methods

void Tutorial::showVideoTutorial()
{
    if (_video)
    {
        auto delay0 = DelayTime::create(2.f);
        
        auto idleRightCallback = CallFunc::create([this]()
        {
            Utility::fadeInNodeWithTime(_video, tutorialDefs::FADE_TIME);
            auto idleAnimation = _video->getAnimationWithName("idle");
            if (idleAnimation)
            {
                idleAnimation->setActive(true);
                _video->setScaleX(1);
                idleAnimation->restart();
            }
        });
        
        auto delay1 = DelayTime::create(2.f);
        
        auto playRightCallback = CallFunc::create([this]()
        {
            auto playAnimation = _video->getAnimationWithName("play");
            if (playAnimation)
            {
                playAnimation->setActive(true);
                playAnimation->restart();
            }
        });
        
        auto delay2 = DelayTime::create(7.f);
        
        auto fadeOutCallback = CallFunc::create([this]()
        {
            Utility::fadeOutNodeWithTime(_video, 2.f);
        });
        
        auto delay3 = DelayTime::create(2.f);
        
        auto idleLeftCallback = CallFunc::create([this]()
        {
            Utility::fadeInNodeWithTime(_video, tutorialDefs::FADE_TIME);
            auto idleAnimation = _video->getAnimationWithName("idle");
            if (idleAnimation)
            {
                idleAnimation->setActive(true);
                _video->setScaleX(-1);
                idleAnimation->restart();
            }
        });
        
        auto delay4 = DelayTime::create(1.f);
        
        auto playLeftCallback = CallFunc::create([this]()
        {
            auto playAnimation = _video->getAnimationWithName("play");
            if (playAnimation)
            {
                playAnimation->setActive(true);
                playAnimation->restart();
            }
        });
        
        auto sequence = Sequence::create(delay0, idleRightCallback, delay1, playRightCallback, delay2, fadeOutCallback, delay3, idleLeftCallback, delay4, playLeftCallback, delay2, fadeOutCallback, NULL);
        
        auto action = RepeatForever::create(sequence);
        action->setTag(tutorialDefs::VIDEO_ANIMATION_TAG);
        
        _video->runAction(action);
    }
}

void Tutorial::showCentralLine()
{
    Utility::fadeInNodeWithTime(_centralLine, tutorialDefs::FADE_TIME);
}

void Tutorial::showGuideLeft()
{
    Utility::fadeInNodeWithTime(_guideLeft, tutorialDefs::FADE_TIME);
}

void Tutorial::showGuideRight()
{
    Utility::fadeInNodeWithTime(_guideRight, tutorialDefs::FADE_TIME);
}

void Tutorial::showGlowLeft()
{
    if (_leftPressed)
    {
        Utility::fadeInNodeWithTime(_glowLeft, 0.5);
    }
}

void Tutorial::showGlowRight()
{
    if (_rightPressed)
    {
       Utility::fadeInNodeWithTime(_glowRight, 0.5);
    }
}

void Tutorial::hideVideoTutorial()
{
    if (_video)
    {
        _video->stopAllActionsByTag(tutorialDefs::VIDEO_ANIMATION_TAG);
        Utility::fadeOutNodeWithTime(_video, tutorialDefs::FADE_TIME);
    }
}

void Tutorial::hideCentralLine()
{
    Utility::fadeOutNodeWithTime(_centralLine, tutorialDefs::FADE_TIME);
}

void Tutorial::hideGuideLeft()
{
    Utility::fadeOutNodeWithTime(_guideLeft, tutorialDefs::FADE_TIME);
}

void Tutorial::hideGuideRight()
{
    Utility::fadeOutNodeWithTime(_guideRight, tutorialDefs::FADE_TIME);
}

void Tutorial::hideGlowLeft()
{
    Utility::fadeOutNodeWithTime(_glowLeft, 0.5);
}

void Tutorial::hideGlowRight()
{
    Utility::fadeOutNodeWithTime(_glowRight, 0.5);
}

void Tutorial::toggleGuideLeftPressed(bool pressed)
{
    _leftPressed = pressed;
    if (pressed)
    {
        if (_guideLeft)
        {
//            this->stopAnimationGuideLeft();
            _guideLeft->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_HOLD);
        }
        if (eTutorialState::TUTORIAL_NONE != _state && eTutorialState::TUTORIAL_CONTROLLER_RIGHT != _state)
        {
            Utility::fadeInNodeWithTime(_glowLeft, 0.5f);
        }
    }
    else
    {
        if (_guideLeft)
        {
            _guideLeft->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_FIRST);
//            this->startAnimateGuideLeft(_state);
        }
        Utility::fadeOutNodeWithTime(_glowLeft, 0.5f);
    }
}

void Tutorial::toggleGuideRightPressed(bool pressed)
{
    _rightPressed = pressed;
    if (pressed)
    {
        if (_guideRight)
        {
//            this->stopAnimationGuideRight();
            _guideRight->setSpriteFrame(tutorialDefs::GUIDE_RIGHT_FRAME_HOLD);
        }
        if (eTutorialState::TUTORIAL_NONE != _state && eTutorialState::TUTORIAL_CONTROLLER_LEFT != _state)
        {
            Utility::fadeInNodeWithTime(_glowRight, 0.5f);
        }
    }
    else
    {
        if (_guideRight)
        {
            _guideRight->setSpriteFrame(tutorialDefs::GUIDE_LEFT_FRAME_PRESS_FIRST);
//            this->startAnimateGuideRight(_state);
        }
        Utility::fadeOutNodeWithTime(_glowRight, 0.5f);
    }
}

void Tutorial::startAnimateGuideLeft(eTutorialState state)
{
    if (nullptr != _guideLeft)
//    if (nullptr != _guideLeft && false == _leftPressed)
    {
        this->stopAnimationGuideLeft();
        ActionInterval * action = nullptr;
        switch (state)
        {
            case eTutorialState::TUTORIAL_CONTROLLER_LEFT:
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_LEFT:
                action = _animLongPress->clone();
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_RIGHT:
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_VERTICAL:
                action = _animTap->clone();
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_RIGHT:
            case eTutorialState::TUTORIAL_NONE:
            default:
                break;
        }
        if (action)
        {
            action->setTag(static_cast<int>(eAction::ANIMATION));
            _guideLeft->runAction(action);
        }
    }
}

void Tutorial::startAnimateGuideRight(eTutorialState state)
{
    if (nullptr != _guideRight)
//    if (nullptr != _guideRight && false == _rightPressed)
    {
        this->stopAnimationGuideRight();
        ActionInterval * action = nullptr;
        switch (state)
        {
            case eTutorialState::TUTORIAL_CONTROLLER_RIGHT:
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_RIGHT:
                action = _animLongPress->clone();
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_LEFT:
            case eTutorialState::TUTORIAL_CONTROLLER_JUMP_VERTICAL:
                action = _animTap->clone();
                break;
            case eTutorialState::TUTORIAL_CONTROLLER_LEFT:
            case eTutorialState::TUTORIAL_NONE:
            default:
                break;
        }
        if (action)
        {
            action->setTag(static_cast<int>(eAction::ANIMATION));
            _guideRight->runAction(action);
        }
    }
}

void Tutorial::stopAnimationGuideLeft()
{
    if (_guideLeft)
    {
        _guideLeft->stopActionByTag(static_cast<int>(eAction::ANIMATION));
    }
}

void Tutorial::stopAnimationGuideRight()
{
    if (_guideRight)
    {
        _guideRight->stopActionByTag(static_cast<int>(eAction::ANIMATION));
    }
}
