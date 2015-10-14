//
//  SheepFallingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#include "Sheep.h"
#include "SheepFallingState.h"
#include "SheepStandingState.h"
#include "SheepDeathState.h"
#include "SheepFlyingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

SheepFallingState::SheepFallingState(Sheep* sheep, eAction input)
{
    _animationFall = sheep->getAnimationFall();
    _soundLanding = sheep->getSoundLand();

    switch (input)
    {
        case eAction::MOVE_RIGHT:
            this->setRightFallingGraphics(sheep);
            break;
            
        case eAction::MOVE_LEFT:
            this->setLeftFallingGraphics(sheep);
            break;
            
        default:
            break;
    }
}

SheepState* SheepFallingState::handleInput(Sheep *sheep, eAction input)
{
    SheepState* newState = nullptr;
    
    switch (input)
    {
        case eAction::STOP_FALL:
            newState = this->handleStopFall(sheep);
            break;
            
        case eAction::MOVE_LEFT:
        case eAction::MOVE_RIGHT:
            this->handleDirection(sheep, input);
            break;
            
        case eAction::BURN:
            newState = this->handleDeath(sheep, input);
            break;
            
        case eAction::FLY:
            newState = this->handleFlyInput(sheep, input);
            break;
            
        case eAction::STOP_JUMP:
            CCLOG("STOP_JUMP");
            break;
            
        default:
            break;
    }
    
    return newState;
}

void SheepFallingState::update(Sheep *sheep, float dt)
{
    
}

void SheepFallingState::setRightFallingGraphics(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        _animationFall->startAnimation(false);
//        _animationFall->startAnimation(2, false);
        sprite->setScaleX(1);
    }
}

void SheepFallingState::setLeftFallingGraphics(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        _animationFall->startAnimation(false);
//        _animationFall->startAnimation(2, false);
        sprite->setScaleX(-1);
    }
}

void SheepFallingState::createLandingAnimation(Sheep* sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        sprite->setSpriteFrame("pecoraSalta_02_0002.png");
    }
}

void SheepFallingState::stopAnimation()
{
    _animationFall->stopAnimation();
}

void SheepFallingState::handleDirection(Sheep *sheep, eAction input)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        switch (input)
        {
            case eAction::MOVE_LEFT:
                sprite->setScaleX(-1);
                break;
                
            case eAction::MOVE_RIGHT:
                sprite->setScaleX(1);
                break;
                
            default:
                break;
        }
    }
}

SheepState* SheepFallingState::handleStopFall(Sheep *sheep)
{
    this->stopAnimation();
    this->createLandingAnimation(sheep);
    _soundLanding->playOnce();
    
    return new SheepStandingState(sheep);
}

SheepState* SheepFallingState::handleDeath(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepDeathState(sheep, input);
}

SheepState* SheepFallingState::handleFlyInput(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    SheepState* newState = new SheepFlyingState(sheep, input);
    
    return newState;
}