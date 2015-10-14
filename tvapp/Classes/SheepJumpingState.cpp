//
//  SheepJumpingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#include "Sheep.h"
#include "SheepJumpingState.h"
#include "SheepWalkingState.h"
#include "SheepStandingState.h"
#include "SheepDeathState.h"
#include "SheepFlyingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

SheepJumpingState::SheepJumpingState(Sheep* sheep, eAction input)
{
    _animationJump = sheep->getAnimationJump();
    _animationFall = sheep->getAnimationFall();
    
    _soundJump = sheep->getSoundJump();
    _soundLanding = sheep->getSoundLand();
    
    _isFalling = false;
    
    switch (input)
    {
        case eAction::MOVE_RIGHT:
            this->setJumpingRightGraphics(sheep);
            break;
            
        case eAction::MOVE_LEFT:
            this->setJumpingLeftGraphics(sheep);
            break;
            
        case eAction::JUMP:
            this->setJumpingGraphics(sheep);
            break;
            
        default:
            break;
    }
}

SheepState* SheepJumpingState::handleInput(Sheep *sheep, eAction input)
{
    SheepState* newState = nullptr;
    
    switch (input)
    {
        case eAction::JUMP:
            newState = this->handleJump(sheep, input);
            break;
        
        case eAction::MOVE_LEFT:
        case eAction::MOVE_RIGHT:
            this->handleDirection(sheep, input);
            break;
            
        case eAction::STOP_JUMP:
            newState = this->handleStopJump(sheep);
            break;
            
        case eAction::BURN:
            newState = this->handleDeath(sheep, input);
            break;
            
        case eAction::FLY:
            newState = this->handleFlyInput(sheep, input);
            break;
            
        case eAction::STOP_FALL:
            newState = this->handleStopFall(sheep);
            break;
            
        default:
            break;
    }
    
    return newState;
}

void SheepJumpingState::update(Sheep *sheep, float dt)
{

}

void SheepJumpingState::setJumpingRightGraphics(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        this->createJumpAnimation(sprite);
        sprite->setScaleX(1);
    }
}

void SheepJumpingState::setJumpingLeftGraphics(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        this->createJumpAnimation(sprite);
        sprite->setScaleX(-1);
    }
}

void SheepJumpingState::setJumpingGraphics(Sheep *sheep)
{
    if (sheep->getBox2dBody()->GetLinearVelocity().x > 0.1f)
    {
        this->setJumpingRightGraphics(sheep);
    }
    else if(sheep->getBox2dBody()->GetLinearVelocity().x < -0.1f)
    {
        this->setJumpingLeftGraphics(sheep);
    }
    else
    {
        auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
        
        if (sprite)
        {
            this->createJumpAnimation(sprite);
        }
    }
}

void SheepJumpingState::createJumpAnimation(LHSprite *sprite)
{
    DelayTime* delay = DelayTime::create(0.5f);
    CallFunc * jumpCallback = CallFunc::create([this]()
    {
        _animationJump->startAnimation(false);
//        _animationJump->startAnimation(3, false);
        _soundJump->playOnce();
    });
    
    CallFunc * fallCallback = CallFunc::create([this]()
    {
        _animationFall->startAnimation(false);
//        _animationFall->startAnimation(2, false);
        _isFalling = true;
    });
    
    auto _action = Sequence::create(jumpCallback, delay, fallCallback, NULL);
    sprite->runAction(_action);
}

void SheepJumpingState::createLandingAnimation(Sheep* sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        sprite->setSpriteFrame("pecoraSalta_02_0002.png");
    }
}

void SheepJumpingState::stopAnimation()
{
    _animationJump->stopAnimation();
    _animationFall->stopAnimation();
    _isFalling = false;
}

SheepState* SheepJumpingState::handleJump(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepJumpingState(sheep, input);
}

void SheepJumpingState::handleDirection(Sheep *sheep, eAction input)
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

SheepState* SheepJumpingState::handleStopJump(Sheep *sheep)
{
    this->stopAnimation();
    this->createLandingAnimation(sheep);
    _soundLanding->playOnce();
    _isFalling = false;
    
    return new SheepStandingState(sheep);
}

SheepState* SheepJumpingState::handleStopFall(Sheep *sheep)
{
    if (_isFalling)
    {
        this->stopAnimation();
        this->createLandingAnimation(sheep);
        _soundLanding->playOnce();
        _isFalling = false;
        
        return new SheepStandingState(sheep);
    }
    
    return nullptr;
}

SheepState* SheepJumpingState::handleDeath(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepDeathState(sheep, input);
}

SheepState* SheepJumpingState::handleFlyInput(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    SheepState* newState = new SheepFlyingState(sheep, input);
    
    return newState;
}