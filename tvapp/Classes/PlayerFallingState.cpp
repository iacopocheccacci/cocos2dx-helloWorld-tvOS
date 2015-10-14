//
//  PlayerFallingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 10/02/15.
//
//

#include "Player.h"
#include "PlayerFallingState.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerDeathState.h"
#include "PlayerFlyingState.h"
#include "PlayerExultingState.h"
#include "SoundComponent.h"
#include "AnimationComponent.h"

PlayerFallingState::PlayerFallingState(Player* player, eAction input)
{
    _isPlaying = player->getIsPlaying();
    
    _animationFall = player->getAnimationFall();
    _animationRebounce = player->getAnimationRebounce();
    _animationPlayingFall = player->getAnimationPlayingFall();
    _animationPlayingRebounce = player->getAnimationPlayingRebounce();
    
    switch (input)
    {
        case eAction::FALL:
            this->initFallingState(player);
            break;
            
        case eAction::MOVE_RIGHT:
            this->initFallingRightState(player);
            break;
            
        case eAction::MOVE_LEFT:
            this->initFallingLeftState(player);
            break;
            
        case eAction::NONE:
            this->initFallingState(player);
            break;
            
        default:
            break;
    }
    
    _soundWalk = player->getSoundWalk();
}

PlayerState* PlayerFallingState::handleInput(Player *player, eAction input)
{
    PlayerState* newState = nullptr;
    
    switch (input)
    {
        case eAction::MOVE_RIGHT:
            this->handleMoveRightInput(player);
            break;
            
        case eAction::MOVE_LEFT:
            this->handleMoveLeftInput(player);
            break;
            
        case eAction::STOP_MOVE_RIGHT:
            this->handleStopMoveRightInput(player);
            break;
            
        case eAction::STOP_MOVE_LEFT:
            this->handleStopMoveLeftInput(player);
            break;
            
        case eAction::STOP_FALL:
            newState = this->handleStopFallInput(player);
            break;
            
        case eAction::BURN:
            newState = this->handleDeathInput(player, input);
            break;
            
        case eAction::FLY:
            newState = this->handleFlyInput(player, input);
            break;
            
        case eAction::EXULT:
            newState = this->handleExultInput(player, input);
            break;
            
        default:
            break;
    }
    
    return newState;
}

void PlayerFallingState::update(Player *player)
{
    if (_isRightPressed)
    {
        this->updateRightPressed(player);
    }
    else if (_isLeftPressed)
    {
        this->updateLeftPressed(player);
    }
}

void PlayerFallingState::setFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                _animationPlayingFall->startAnimation(4, false);
            }
            else
            {
                _animationFall->startAnimation(4, false);
            }
        }
    }
}

void PlayerFallingState::setRightFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                _animationPlayingFall->startAnimation(4, false);
            }
            else
            {
                _animationFall->startAnimation(4, false);
            }
        }
        
        sprite->setFlippedX(false);
    }
}

void PlayerFallingState::setLeftFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                _animationPlayingFall->startAnimation(4, false);
            }
            else
            {
                _animationFall->startAnimation(4, false);
            }
        }
        
        sprite->setFlippedX(true);
    }
}

void PlayerFallingState::initFallingRightState(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    _isLeftPressed = false;
    player->setMovingLeft(false);
    this->setRightFallingGraphics(player, true);
}

void PlayerFallingState::initFallingLeftState(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    _isRightPressed = false;
    player->setMovingRight(false);
    this->setLeftFallingGraphics(player, true);
}

void PlayerFallingState::initFallingState(Player *player)
{
    _isRightPressed = false;
    _isLeftPressed = false;
    player->setMovingRight(false);
    player->setMovingLeft(false);
    this->setFallingGraphics(player, true);
}

void PlayerFallingState::handleMoveRightInput(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    this->setRightFallingGraphics(player, false);
}

void PlayerFallingState::handleMoveLeftInput(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    this->setLeftFallingGraphics(player, false);
}

void PlayerFallingState::handleStopMoveRightInput(Player *player)
{
    _isRightPressed = false;
    player->oppositeImpulse();
    player->setMovingRight(false);
    if (_isLeftPressed)
    {
        this->setLeftFallingGraphics(player, false);
    }
    else
    {
        this->setFallingGraphics(player, false);
    }
}

void PlayerFallingState::handleStopMoveLeftInput(Player *player)
{
    _isLeftPressed = false;
    player->oppositeImpulse();
    player->setMovingLeft(false);
    if (_isRightPressed)
    {
        this->setRightFallingGraphics(player, false);
    }
    else
    {
        this->setFallingGraphics(player, false);
    }
}

PlayerState* PlayerFallingState::handleStopFallInput(Player *player)
{
    PlayerState* newState = nullptr;
    _animationFall->stopAnimation();
    _soundWalk->playOnce();
    
    if (!_isLeftPressed && !_isRightPressed)
    {
        if (_isPlaying)
        {
            _animationPlayingRebounce->startAnimation(4, false);
        }
        else
        {
            _animationRebounce->startAnimation(4, false);
        }
        
        newState = new PlayerStandingState(player, true);
    }
    else if (_isRightPressed)
    {
        newState = new PlayerWalkingState(player, eAction::MOVE_RIGHT);
    }
    else if (_isLeftPressed)
    {
        newState = new PlayerWalkingState(player, eAction::MOVE_LEFT);
    }
    
    return newState;
}

PlayerState* PlayerFallingState::handleDeathInput(Player *player, eAction input)
{
    _animationFall->stopAnimation();
    PlayerState* newState = new PlayerDeathState(player, input);
    
    return newState;
}

PlayerState* PlayerFallingState::handleFlyInput(Player *player, eAction input)
{
    _animationFall->stopAnimation();
    PlayerState* newState = new PlayerFlyingState(player, input);
    
    return newState;
}

PlayerState* PlayerFallingState::handleExultInput(Player *player, eAction input)
{
    _animationFall->stopAnimation();
    
    if (_isLeftPressed)
    {
        input = eAction::MOVE_LEFT;
    }
    else if (_isRightPressed)
    {
        input = eAction::MOVE_RIGHT;
    }
    else
    {
        input = eAction::NONE;
    }
    
    PlayerState* newState = new PlayerExultingState(player, input);
    
    return newState;
}

void PlayerFallingState::updateRightPressed(Player *player)
{
    player->movePlayerRight();
    
    if (_isPlaying != player->getIsPlaying())
    {
        if (_isPlaying)
        {
            _animationPlayingFall->stopAnimation();
        }
        else
        {
            _animationFall->stopAnimation();
        }
        
        _isPlaying = player->getIsPlaying();
        this->setRightFallingGraphics(player, true);
    }
}

void PlayerFallingState::updateLeftPressed(Player *player)
{
    player->movePlayerLeft();
    
    if (_isPlaying != player->getIsPlaying())
    {
        if (_isPlaying)
        {
            _animationPlayingFall->stopAnimation();
        }
        else
        {
            _animationFall->stopAnimation();
        }
        
        _isPlaying = player->getIsPlaying();
        this->setLeftFallingGraphics(player, true);
    }
}
