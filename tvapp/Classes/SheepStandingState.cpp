//
//  SheepStandingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#include "Sheep.h"
#include "SheepStandingState.h"
#include "SheepWalkingState.h"
#include "SheepJumpingState.h"
#include "SheepFallingState.h"
#include "SheepDeathState.h"
#include "SheepFlyingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

SheepStandingState::SheepStandingState(Sheep* sheep)
{
    _animationIdle1 = sheep->getAnimationIdle1();
    _animationIdle2 = sheep->getAnimationIdle2();
    _animationIdle3 = sheep->getAnimationIdle3();
    _animationCatch = sheep->getAnimationCatch();
    
    _soundExcited = nullptr;
    
    _animationDuration = 0.0f;
    _timer = 0.0f;
    
    this->setGraphics(sheep);
}

SheepState* SheepStandingState::handleInput(Sheep *sheep, eAction input)
{
    SheepState* newState = nullptr;
    
    switch (input)
    {
        case eAction::MOVE_RIGHT:
        case eAction::MOVE_LEFT:
            newState = this->handleMove(sheep, input);
            break;

        case eAction::JUMP:
            newState = this->handleJump(sheep, input/*this->getDirection(sheep)*/);
            break;
            
        case eAction::FALL:
            newState = this->handleFall(sheep, input);
            break;
            
        case eAction::BURN:
            newState = this->handleDeath(sheep, input);
            break;
            
        case eAction::JUMP_ON:
            this->createCatchAnimation(sheep);
            break;
            
        case eAction::FLY:
            newState = this->handleFlyInput(sheep, input);
            break;
            
        default:
            break;
    }
    
    return newState;
}

void SheepStandingState::update(Sheep *sheep, float dt)
{
    _timer += dt;
    
    if (_timer >= _animationDuration)
    {
        _timer = 0.0f;
        this->createRandomAnimation(sheep);
    }
}

void SheepStandingState::setGraphics(Sheep *sheep)
{
    this->createRandomAnimation(sheep);
}

void SheepStandingState::createRandomAnimation(Sheep * sheep)
{
    unsigned int random = CCRANDOM_0_1() * 100;
    
    if (sheep->getIsPlayerOver())
    {
        sheep->setIsPlayerOver(false);
    }
    
    if (sheep->containsTag(gameTags::MONSTER_TAG))
    {
        if (random < 80 || sheep->getIsExcited() )
        {
            if (_animationIdle1)
            {
                _animationIdle1->startAnimation(false);
                // L'animazione è a singolo frame ma voglio che l'animazione successiva parta più tardi,
                // così ho una casistica di animazioni idle simile alla pecora normale
                _animationDuration = 1.2f;
            }
        }
        else
        {
            if (_animationIdle2)
            {
                if ( sheep->getSoundIdle2() )
                {
                    sheep->getSoundIdle2()->playOnce(true, false);
                    sheep->updateAudio(0);
                    sheep->getSoundIdle2()->forceUpdate();
                }
                _animationIdle2->startAnimation(false);
                _animationDuration = _animationIdle2->getDuration();
            }
        }
    }
    else
    {
        if (random < 70 || sheep->getIsExcited() )
        {
            if (_animationIdle1)
            {
                _animationIdle1->startAnimation(false);
                _animationDuration = _animationIdle1->getDuration();
                //          _animationIdle1->startAnimation(12, false);
                //          _animationDuration = 1.2f;
            }
        }
        else if (random < 85)
        {
            if (_animationIdle2)
            {
                if ( sheep->getSoundIdle2() )
                {
                    sheep->getSoundIdle2()->playOnce(true, false);
                    sheep->updateAudio(0);
                    sheep->getSoundIdle2()->forceUpdate();
                }
                _animationIdle2->startAnimation(false);
                _animationDuration = _animationIdle2->getDuration();
                //          _animationIdle2->startAnimation(24, false);
                //          _animationDuration = 2.0f;
            }
        }
        else
        {
            if (_animationIdle3)
            {
                if ( sheep->getSoundIdle3() )
                {
                    sheep->getSoundIdle3()->playOnce(true, false);
                    sheep->updateAudio(0);
                    sheep->getSoundIdle3()->forceUpdate();
                }
                _animationIdle3->startAnimation(false);
                _animationDuration = _animationIdle3->getDuration();
                //          _animationIdle3->startAnimation(10, false);
                //          _animationDuration = 1.3f;
            }
        }
    }
}

void SheepStandingState::createCatchAnimation(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    if (sprite)
    {
        _timer = 0.0f;
        sheep->setIsPlayerOver(true);
        this->stopAnimation();
        
        if (_animationCatch)
        {
            if (_soundExcited == nullptr)
            {
                _soundExcited = sheep->getSoundExcited();
            }
            
            _soundExcited->playOnce();
            _animationCatch->startAnimation(false);
            _animationDuration = _animationCatch->getDuration();
        }
    }
}

void SheepStandingState::stopAnimation()
{
    if (_animationIdle1)
    {
        _animationIdle1->stopAnimation();
    }
    if (_animationIdle2)
    {
        _animationIdle2->stopAnimation();
    }
    if (_animationIdle3)
    {
        _animationIdle3->stopAnimation();
    }
}

eAction SheepStandingState::getDirection(Sheep *sheep)
{
    auto action = eAction::NONE;
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        action = (sprite->isFlippedX()) ? eAction::MOVE_LEFT : eAction::MOVE_RIGHT;
    }
    
    return action;
}

SheepState* SheepStandingState::handleMove(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepWalkingState(sheep, input);
}

SheepState* SheepStandingState::handleJump(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepJumpingState(sheep, input);
}

SheepState* SheepStandingState::handleFall(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepFallingState(sheep, input);
}

SheepState* SheepStandingState::handleDeath(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    return new SheepDeathState(sheep, input);
}

SheepState* SheepStandingState::handleFlyInput(Sheep *sheep, eAction input)
{
    this->stopAnimation();
    SheepState* newState = new SheepFlyingState(sheep, input);
    
    return newState;
}
