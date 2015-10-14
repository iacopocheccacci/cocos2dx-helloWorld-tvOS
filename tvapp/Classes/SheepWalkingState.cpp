//
//  SheepWalkingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#include "Sheep.h"
#include "SheepWalkingState.h"
#include "SheepJumpingState.h"
#include "SheepStandingState.h"
#include "SheepFallingState.h"
#include "SheepDeathState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"
#include "SheepFlyingState.h"

SheepWalkingState::SheepWalkingState(Sheep* sheep, eAction input)
{
    _direction = input;
    _animationWalk = sheep->getAnimationWalk();
    _animationCatch = sheep->getAnimationCatch();
    
    _soundExcited = sheep->getSoundExcited();
    _soundWalk = sheep->getSoundWalk();
    
    switch (_direction)
    {
        case eAction::MOVE_RIGHT:
            this->setWalkingRightGraphics(sheep, true);
            break;
            
        case eAction::MOVE_LEFT:
            this->setWalkingLeftGraphics(sheep, true);
            break;
            
        default:
            break;
    }
}

SheepState* SheepWalkingState::handleInput(Sheep *sheep, eAction input)
{
    SheepState* newState = nullptr;
    
    switch (input)
    {
        case eAction::MOVE_RIGHT:
            if (!_isWalkingRight)
            {
                this->setWalkingRightGraphics(sheep, false);
            }
            break;
            
        case eAction::MOVE_LEFT:
            if (!_isWalkingLeft)
            {
                this->setWalkingLeftGraphics(sheep, false);
            }
            break;
            
        case eAction::STOP_MOVE_RIGHT:
        case eAction::STOP_MOVE_LEFT:
            newState = this->handleStopMove(sheep);
            break;
            
        case eAction::JUMP:
            newState = this->handleJump(sheep);
            break;
            
        case eAction::FALL:
            newState = this->handleFall(sheep, input);
            break;
            
        case eAction::BURN:
            newState = this->handleBurn(sheep, input);
            break;
            
        case eAction::FLY:
            newState = this->handleFlyInput(sheep, input);
            break;
            
        default:
            break;
    }
    
    return newState;
}

void SheepWalkingState::update(Sheep *sheep, float dt)
{
    
}

void SheepWalkingState::setWalkingRightGraphics(Sheep *sheep, bool begin)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (begin)
        {
            if (sheep->getIsExcited())
            {
                _animationWalk->startAnimation(8, true);
            }
            else
            {
                this->createStartWalkAnimation(sprite);
                sheep->setIsExcited(true);
            }
            
            _soundWalk->startPlay();
        }
        
        sprite->setScaleX(1);
        
        _isWalkingRight = true;
        _isWalkingLeft = false;
    }
}

void SheepWalkingState::setWalkingLeftGraphics(Sheep *sheep, bool begin)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (begin)
        {
            if (sheep->getIsExcited())
            {
                _animationWalk->startAnimation(8, true);
            }
            else
            {
                this->createStartWalkAnimation(sprite);
                sheep->setIsExcited(true);
            }
            
            _soundWalk->startPlay();
        }
        
        sprite->setScaleX(-1);
        
        _isWalkingRight = false;
        _isWalkingLeft = true;
    }
}

SheepState* SheepWalkingState::handleStopMove(Sheep *sheep)
{
    this->stopAnimation();
    return new SheepStandingState(sheep);
}

SheepState* SheepWalkingState::handleJump(Sheep *sheep)
{
    this->stopAnimation();
    return new SheepJumpingState(sheep, _direction);
}

SheepState* SheepWalkingState::handleFall(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepFallingState(sheep, input);
}

SheepState* SheepWalkingState::handleBurn(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return  new SheepDeathState(sheep, input);
}

SheepState* SheepWalkingState::handleFlyInput(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    SheepState* newState = new SheepFlyingState(sheep, input);
    
    return newState;
}

void SheepWalkingState::createStartWalkAnimation(LHSprite *sprite)
{
    _soundExcited->startPlay();
    _animationWalk->startAnimation(true);
//    _animationWalk->startAnimation(8, true);
}

void SheepWalkingState::stopAnimation()
{
    _soundWalk->stopPlay();
    _animationCatch->stopAnimation();
    _animationWalk->stopAnimation();
    _isWalkingRight = false;
    _isWalkingLeft = false;
}
