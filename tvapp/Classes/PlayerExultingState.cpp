//
//  PlayerExultingState.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 30/04/15.
//
//

#include "Player.h"
#include "PlayerExultingState.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerDeathState.h"
#include "PlayerFlyingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

PlayerExultingState::PlayerExultingState(Player* player, eAction input)
{
    player->getAnimationFall()->stopAnimation();
    _animationExultFall = player->getAnimationExultFall();
    _animationExultRebounce = player->getAnimationExultRebounce();
    this->initFallingState(player);
    
    switch (input)
    {
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

PlayerState* PlayerExultingState::handleInput(Player *player, eAction input)
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
            
        default:
            break;
    }
    
    return newState;
}

void PlayerExultingState::update(Player *player)
{
    if (_isRightPressed == _isLeftPressed)
    {
        this->updateBothDirectionsPressed(player);
    }
    else
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
}

void PlayerExultingState::setFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            _animationExultFall->startAnimation(3, false);
        }
    }
}

void PlayerExultingState::setRightFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            _animationExultFall->startAnimation(3, false);
        }
        
        sprite->setFlippedX(false);
    }
}

void PlayerExultingState::setLeftFallingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            _animationExultFall->startAnimation(3, false);
        }
        
        sprite->setFlippedX(true);
    }
}

void PlayerExultingState::initFallingRightState(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    _isLeftPressed = false;
    player->setMovingLeft(false);
    this->setRightFallingGraphics(player, true);
}

void PlayerExultingState::initFallingLeftState(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    _isRightPressed = false;
    player->setMovingRight(false);
    this->setLeftFallingGraphics(player, true);
}

void PlayerExultingState::initFallingState(Player *player)
{
    _isRightPressed = false;
    _isLeftPressed = false;
    player->setMovingRight(false);
    player->setMovingLeft(false);
    this->setFallingGraphics(player, true);
}

void PlayerExultingState::handleMoveRightInput(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    this->setRightFallingGraphics(player, false);
}

void PlayerExultingState::handleMoveLeftInput(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    this->setLeftFallingGraphics(player, false);
}

void PlayerExultingState::handleStopMoveRightInput(Player *player)
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

void PlayerExultingState::handleStopMoveLeftInput(Player *player)
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

PlayerState* PlayerExultingState::handleStopFallInput(Player *player)
{
    PlayerState* newState = nullptr;
    _animationExultFall->stopAnimation();
    _soundWalk->playOnce();
    
    if (!_isLeftPressed && !_isRightPressed)
    {
        _animationExultRebounce->startAnimation(5, false);
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

PlayerState* PlayerExultingState::handleDeathInput(Player *player, eAction input)
{
    _animationExultFall->stopAnimation();
    PlayerState* newState = new PlayerDeathState(player, input);
    
    return newState;
}

PlayerState* PlayerExultingState::handleFlyInput(Player *player, eAction input)
{
    _animationExultFall->stopAnimation();
    PlayerState* newState = new PlayerFlyingState(player, input);
    
    return newState;
}

void PlayerExultingState::updateBothDirectionsPressed(Player *player)
{
    player->oppositeImpulse();
}

void PlayerExultingState::updateRightPressed(Player *player)
{
    player->movePlayerRight();
}

void PlayerExultingState::updateLeftPressed(Player *player)
{
    player->movePlayerLeft();
}


