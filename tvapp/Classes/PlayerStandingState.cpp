//
//  PlayerStandingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#include "Player.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerDeathState.h"
#include "PlayerFallingState.h"
#include "PlayerFlyingState.h"
#include "AnimationComponent.h"

PlayerStandingState::PlayerStandingState(Player* player, bool fromJump)
{
    player->setMovingLeft(false);
    player->setMovingRight(false);
    player->setJumping(false);
    
    _animationIdle = player->getAnimationIdle();
    _animationPlayingIdle = player->getAnimationPlayingIdle();
    _isFromJump = fromJump;
    
    _action = nullptr;
    
    _isPlaying = player->getIsPlaying();
    this->setGraphics(player);
}

PlayerStandingState::~PlayerStandingState()
{
//    if (_animationIdle)
//        _animationIdle->stopAnimation();
}

PlayerState* PlayerStandingState::handleInput(Player *player, eAction input)
{
    PlayerState* newState = nullptr;
    
    switch (input)
    {
        case eAction::MOVE_RIGHT:
        case eAction::MOVE_LEFT:
            newState = this->handleMove(player, input);
            break;

        case eAction::JUMP:
            newState = this->handleJump(player, input);
            break;
            
        case eAction::FALL:
            newState = this->handleFall(player, input);
            break;
            
        case eAction::BURN:
            newState = this->handleDeath(player, input);
            break;
            
        case eAction::FLY:
            newState = this->handleFly(player, input);
            break;
            
        default:
            break;
    }
    
    return newState;
}

void PlayerStandingState::update(Player *player)
{
    if (_isPlaying != player->getIsPlaying())
    {
        _animationPlayingIdle->stopAnimation();
        
//        if (_isPlaying)
//        {
//            _animationPlayingIdle->stopAnimation();
//        }
        
        _isPlaying = player->getIsPlaying();
        this->setGraphics(player);
    }
}

void PlayerStandingState::setGraphics(Player* player)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (_isPlaying)
        {
            if (_isFromJump)
            {
                DelayTime * delay = DelayTime::create(0.3f);
                CallFunc * idleCallback = CallFunc::create([this]()
                {
                    _animationPlayingIdle->startAnimation(10, true);
                });
                
                _action = Sequence::create(delay, idleCallback, NULL);
                sprite->runAction(_action);
            }
            else
            {
                _animationPlayingIdle->startAnimation(10, true);
            }
        }
        else
        {
            sprite->setSpriteFrame("panIdle_01_0000.png");
            DelayTime * delay = DelayTime::create(5.0f);
            CallFunc * idleCallback = CallFunc::create([this]()
            {
                _animationIdle->startAnimation(20, false);
            });
            
            _action = RepeatForever::create(Sequence::create(delay, idleCallback, NULL));
            sprite->runAction(_action);
        }
    }
}

void PlayerStandingState::stopAnimation(Player* player)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        sprite->stopAction(_action);
    }
    
    _animationPlayingIdle->stopAnimation();
}

PlayerState* PlayerStandingState::handleMove(Player *player, eAction input)
{
    this->stopAnimation(player);
    return new PlayerWalkingState(player, input);
}

PlayerState* PlayerStandingState::handleJump(Player *player, eAction input)
{
    this->stopAnimation(player);
    return new PlayerJumpingState(player, eAction::NONE);
}

PlayerState* PlayerStandingState::handleFall(Player *player, eAction input)
{
    this->stopAnimation(player);
    return new PlayerFallingState(player, input);
}

PlayerState* PlayerStandingState::handleDeath(Player *player, eAction input)
{
    this->stopAnimation(player);
    return new PlayerDeathState(player, input);
}

PlayerState* PlayerStandingState::handleFly(Player *player, eAction input)
{
    this->stopAnimation(player);
    return new PlayerFlyingState(player, input);
}
