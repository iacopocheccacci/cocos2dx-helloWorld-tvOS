//
//  PlayerJumpingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#include "Player.h"
#include "PlayerJumpingState.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerDeathState.h"
#include "PlayerFlyingState.h"
#include "PlayerExultingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

PlayerJumpingState::~PlayerJumpingState()
{
    if (_soundWalk)
    {
        _soundWalk->playOnce();
    }
}

PlayerJumpingState::PlayerJumpingState(Player* player, eAction direction)
{
    _isPlaying = player->getIsPlaying();
    player->setJumping(true);
    player->resetPushing();
    
    _animationJump = player->getAnimationJump();
    _animationFall = player->getAnimationFall();
    _animationRebounce = player->getAnimationRebounce();
    _animationPlayingJump = player->getAnimationPlayingJump();
    _animationPlayingFall = player->getAnimationPlayingFall();
    _animationPlayingRebounce = player->getAnimationPlayingRebounce();
    
    _soundWalk = player->getSoundWalk();
    
    switch (direction)
    {
        case eAction::MOVE_RIGHT:
            this->initJumpingRightState(player);
            break;
            
        case eAction::MOVE_LEFT:
            this->initJumpingLeftState(player);
            break;
            
        case eAction::NONE:
            this->initJumpingState(player);
            break;
            
        default:
            break;
    }
    
    player->movePlayerJump();
}

PlayerState* PlayerJumpingState::handleInput(Player *player, eAction input)
{
    PlayerState* newState = nullptr;
    
    switch (input)
    {
        case eAction::JUMP:
            newState = this->handleJumpInput(player);
            break;
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

        case eAction::STOP_JUMP:
            newState = this->handleStopJumpInput(player);
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

void PlayerJumpingState::update(Player *player)
{
//    player->movePlayerJump();
    
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

void PlayerJumpingState::setJumpingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                this->createPlayingJumpAnimation(sprite);
            }
            else
            {
                this->createJumpAnimation(sprite);
            }
        }
    }
}

void PlayerJumpingState::setRightJumpingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));

    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                this->createPlayingJumpAnimation(sprite);
            }
            else
            {
                this->createJumpAnimation(sprite);
            }
        }
        
        sprite->setFlippedX(false);
    }
}

void PlayerJumpingState::setLeftJumpingGraphics(Player *player, bool start)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (start)
        {
            if (_isPlaying)
            {
                this->createPlayingJumpAnimation(sprite);
            }
            else
            {
                this->createJumpAnimation(sprite);
            }
        }
        
        sprite->setFlippedX(true);
    }
}

void PlayerJumpingState::initJumpingRightState(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    _isLeftPressed = false;
    player->setMovingLeft(false);
    this->setRightJumpingGraphics(player, true);
}

void PlayerJumpingState::initJumpingLeftState(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    _isRightPressed = false;
    player->setMovingRight(false);
    this->setLeftJumpingGraphics(player, true);
}

void PlayerJumpingState::initJumpingState(Player *player)
{
    _isRightPressed = false;
    _isLeftPressed = false;
    player->setMovingRight(false);
    player->setMovingLeft(false);
    this->setJumpingGraphics(player, true);
}

void PlayerJumpingState::handleMoveRightInput(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    this->setRightJumpingGraphics(player, false);
}

void PlayerJumpingState::handleMoveLeftInput(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    this->setLeftJumpingGraphics(player, false);
}

void PlayerJumpingState::handleStopMoveRightInput(Player *player)
{
    _isRightPressed = false;
    player->oppositeImpulse();
    player->setMovingRight(false);
    if (_isLeftPressed)
    {
        this->setLeftJumpingGraphics(player, false);
    }
    else
    {
        this->setJumpingGraphics(player, false);
    }
}

void PlayerJumpingState::handleStopMoveLeftInput(Player *player)
{
    _isLeftPressed = false;
    player->oppositeImpulse();
    player->setMovingLeft(false);
    if (_isRightPressed)
    {
        this->setRightJumpingGraphics(player, false);
    }
    else
    {
        this->setJumpingGraphics(player, false);
    }
}

PlayerState* PlayerJumpingState::handleJumpInput(Player *player)
{
    PlayerState* newState = nullptr;
    _animationJump->stopAnimation();
    
    if (!_isLeftPressed && !_isRightPressed)
    {
        newState = new PlayerJumpingState(player, eAction::NONE);
    }
    else if (_isRightPressed)
    {
        newState = new PlayerJumpingState(player, eAction::MOVE_RIGHT);
    }
    else if (_isLeftPressed)
    {
        newState = new PlayerJumpingState(player, eAction::MOVE_LEFT);
    }
    
    return newState;
}

PlayerState* PlayerJumpingState::handleStopJumpInput(Player *player)
{
    PlayerState* newState = nullptr;
    _animationJump->stopAnimation();
    
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

PlayerState* PlayerJumpingState::handleDeathInput(Player *player, eAction input)
{
    _animationJump->stopAnimation();
    PlayerState* newState = new PlayerDeathState(player, input);
    
    return newState;
}

PlayerState* PlayerJumpingState::handleFlyInput(Player *player, eAction input)
{
    _animationJump->stopAnimation();
    PlayerState* newState = new PlayerFlyingState(player, input);
    
    return newState;
}

PlayerState* PlayerJumpingState::handleExultInput(Player *player, eAction input)
{
    _animationJump->stopAnimation();
    
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

void PlayerJumpingState::updateBothDirectionsPressed(Player *player)
{
    player->oppositeImpulse();
    
    if (_isPlaying != player->getIsPlaying())
    {
        if (_isPlaying)
        {
            _animationPlayingJump->stopAnimation();
        }
        else
        {
            _animationJump->stopAnimation();
        }
        
        _isPlaying = player->getIsPlaying();
        this->setJumpingGraphics(player, true);
    }
}

void PlayerJumpingState::updateRightPressed(Player *player)
{
    player->movePlayerRight();
    
    if (_isPlaying != player->getIsPlaying())
    {
        if (_isPlaying)
        {
            _animationPlayingJump->stopAnimation();
        }
        else
        {
            _animationJump->stopAnimation();
        }
        
        _isPlaying = player->getIsPlaying();
        this->setRightJumpingGraphics(player, true);
    }
}

void PlayerJumpingState::updateLeftPressed(Player *player)
{
    player->movePlayerLeft();
    
    if (_isPlaying != player->getIsPlaying())
    {
        if (_isPlaying)
        {
            _animationPlayingJump->stopAnimation();
        }
        else
        {
            _animationJump->stopAnimation();
        }
        
        _isPlaying = player->getIsPlaying();
        this->setLeftJumpingGraphics(player, true);
    }
}

void PlayerJumpingState::createJumpAnimation(LHSprite* sprite)
{
    DelayTime* delay = DelayTime::create(0.3f);
    CallFunc * jumpCallback = CallFunc::create([this]()
    {
        _animationJump->startAnimation(4, false);
    });
    
    CallFunc * fallCallback = CallFunc::create([this]()
    {
        _animationFall->startAnimation(4, false);
    });
    
    auto _action = Sequence::create(jumpCallback, delay, fallCallback, NULL);
    sprite->runAction(_action);
}

void PlayerJumpingState::createPlayingJumpAnimation(LHSprite *sprite)
{
    DelayTime* delay = DelayTime::create(0.3f);
    CallFunc * jumpCallback = CallFunc::create([this]()
    {
        _animationPlayingJump->startAnimation(4, false);
    });
    
    CallFunc * fallCallback = CallFunc::create([this]()
    {
        _animationPlayingFall->startAnimation(4, false);
    });
    
    auto _action = Sequence::create(jumpCallback, delay, fallCallback, NULL);
    sprite->runAction(_action);
}
