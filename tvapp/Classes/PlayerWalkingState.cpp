//
//  PlayerWalkingState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#include "Player.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "PlayerJumpingState.h"
#include "PlayerFallingState.h"
#include "PlayerDeathState.h"
#include "PlayerFlyingState.h"
#include "SoundComponent.h"
#include "AnimationComponent.h"
#include "Utility.h"

static const int WALKING_SOUND_ACTION_TAG = 567;

PlayerWalkingState::~PlayerWalkingState()
{
//    if (_soundWalk)             _soundWalk->stopPlay();
//    if (_animationWalk)         _animationWalk->stopAnimation();
//    if (_animationPlayingWalk) _animationPlayingWalk->stopAnimation();
    
//    CCLOG("Exit Walking State");
}

PlayerWalkingState::PlayerWalkingState(Player* player, eAction direction)
{
    _isPlaying = player->getIsPlaying();
    player->setJumping(false);
    
    _animationWalk = player->getAnimationWalk();
    _animationPlayingWalk = player->getAnimationPlayingWalk();
    _animationStartPush = player->getAnimationStartPushing();
    _animationPush = player->getAnimationPushing();
    _soundWalk = player->getSoundWalk();
    
    switch (direction)
    {
        case eAction::MOVE_RIGHT:
            this->initMoveRightState(player);
            break;
            
        case eAction::MOVE_LEFT:
            this->initMoveLeftState(player);
            break;
            
        default:
            break;
    }
}

PlayerState* PlayerWalkingState::handleInput(Player *player, eAction input)
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
            newState = this->handleStopMoveRightInput(player);
            break;
            
        case eAction::STOP_MOVE_LEFT:
            newState = this->handleStopMoveLeftInput(player);
            break;
            
        case eAction::JUMP:
            newState = this->handleJumpInput(player);
            break;
            
        case eAction::FALL:
            newState = this->handleFallInput(player);
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

void PlayerWalkingState::update(Player *player)
{
    if (_isRightPressed && _isLeftPressed)
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

void PlayerWalkingState::setWalkingRightGraphics(Player *player)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    _isPushingRight = player->getIsPushingRight();
    
    this->stopAnimation(player);
    
    if (sprite)
    {
        CallFunc * soundCallback = this->createSoundWalkCallback(player);
//        SoundComponent * soundWalk = _soundWalk;
//        CallFunc * soundCallback = CallFunc::create([soundWalk, player](){
//            DelayTime * delayA = DelayTime::create(0.05f);
//            DelayTime * delayB = DelayTime::create(0.4f);
//            CallFunc * playSound = CallFunc::create([soundWalk, player](){
//                
//                if (soundWalk)
//                {
//                    soundWalk->playOnce();
//                }
//            });
//            player->runAction(Sequence::create(delayA, playSound->clone(), delayB, playSound->clone(), NULL));
//        });
        
        if (_isPlaying)
        {
            _animationPlayingWalk->startAnimationWithCallback(11, soundCallback, true);
        }
        else if (_isPushingRight)
        {
            this->createPushingAnimation(sprite, player);
        }
        else
        {
            if (player->getIsOnMenu())
            {
                _animationWalk->startAnimationWithCallback(12, soundCallback, true);
            }
            else
            {
                _animationWalk->startAnimationWithCallback(11, soundCallback, true);
            }
        }
        
        sprite->setFlippedX(false);
    }
    
}

void PlayerWalkingState::setWalkingLeftGraphics(Player *player)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    _isPushingLeft = player->getIsPushingLeft();
    
    this->stopAnimation(player);
    
    if (sprite)
    {
        CallFunc * soundCallback = this->createSoundWalkCallback(player);
//        SoundComponent * soundWalk = _soundWalk;
//        CallFunc * soundCallback = CallFunc::create([soundWalk, player](){
//            DelayTime * delayA = DelayTime::create(0.05f);
//            DelayTime * delayB = DelayTime::create(0.4f);
//            CallFunc * playSound = CallFunc::create([soundWalk, player](){
//                
//                if (soundWalk)
//                {
//                    soundWalk->playOnce();
//                }
//            });
//            player->runAction(Sequence::create(delayA, playSound->clone(), delayB, playSound->clone(), NULL));
//        });
        
        if (_isPlaying)
        {
            _animationPlayingWalk->startAnimationWithCallback(11, soundCallback, true);
        }
        else if (_isPushingLeft)
        {
            this->createPushingAnimation(sprite, player);
        }
        else
        {
            _animationWalk->startAnimationWithCallback(11, soundCallback, true);
        }
        
        sprite->setFlippedX(true);
    }
}

void PlayerWalkingState::setStandingGraphics(Player *player)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        if (_isPlaying)
        {
            // TODO: animazione Pan che suona
            sprite->setSpriteFrame("Pan_fermoSuona_0001.png");
        }
        else
        {
            // TODO: animazione Pan normale
            sprite->setSpriteFrame("Pan_fermo_0001.png");
        }
    }
}

void PlayerWalkingState::initMoveRightState(Player* player)
{
    _isRightPressed = true;
    _isLeftPressed = false;
    _isPushingRight = player->getIsPushingRight();
    player->setMovingRight(true);
    player->setMovingLeft(false);
    this->setWalkingRightGraphics(player);
}

void PlayerWalkingState::initMoveLeftState(Player* player)
{
    _isRightPressed = false;
    _isLeftPressed = true;
    _isPushingRight = player->getIsPushingLeft();
    player->setMovingRight(false);
    player->setMovingLeft(true);
    this->setWalkingLeftGraphics(player);
}

void PlayerWalkingState::handleMoveRightInput(Player *player)
{
    _isRightPressed = true;
    player->setMovingRight(true);
    if (_isLeftPressed)
    {
        //this->setStandingGraphics(player);
    }
    else
    {
        this->setWalkingRightGraphics(player);
    }
}

void PlayerWalkingState::handleMoveLeftInput(Player *player)
{
    _isLeftPressed = true;
    player->setMovingLeft(true);
    if (_isRightPressed)
    {
        //this->setStandingGraphics(player);
    }
    else
    {
        this->setWalkingLeftGraphics(player);
    }
}

PlayerState* PlayerWalkingState::handleStopMoveRightInput(Player *player)
{
    PlayerState* newState = nullptr;
    this->stopAnimation(player);
    
    if (_isLeftPressed)
    {
        _isRightPressed = false;
        player->setMovingRight(false);
        this->setWalkingLeftGraphics(player);
    }
    else
    {
        if (_isPushingRight)
        {
            this->createStopPushingAnimation();
        }
        else
        {
            player->oppositeImpulse();
        }
        
        newState = new PlayerStandingState(player, false);
    }
    
    return newState;
}

PlayerState* PlayerWalkingState::handleStopMoveLeftInput(Player *player)
{
    PlayerState* newState = nullptr;
    this->stopAnimation(player);
    
    if (_isRightPressed)
    {
        _isLeftPressed = false;
        player->setMovingLeft(false);
        this->setWalkingRightGraphics(player);
    }
    else
    {
        if (_isPushingLeft)
        {
            this->createStopPushingAnimation();
        }
        else
        {
            player->oppositeImpulse();
        }
        
        newState = new PlayerStandingState(player, false);
    }
    
    return newState;
}

PlayerState* PlayerWalkingState::handleJumpInput(Player *player)
{
    PlayerState* newState = nullptr;
    this->stopAnimation(player);
    
    if (_isRightPressed && _isLeftPressed)
    {
        newState = new PlayerJumpingState(player, eAction::NONE);
    }
    else if (_isLeftPressed)
    {
        newState = new PlayerJumpingState(player, eAction::MOVE_LEFT);
    }
    else if (_isRightPressed)
    {
        newState = new PlayerJumpingState(player, eAction::MOVE_RIGHT);
    }
    
    return newState;
}

PlayerState* PlayerWalkingState::handleFallInput(Player *player)
{
    PlayerState* newState = nullptr;
    this->stopAnimation(player);
    
    if (_isLeftPressed)
    {
        newState = new PlayerFallingState(player, eAction::MOVE_LEFT);
    }
    else if (_isRightPressed)
    {
        newState = new PlayerFallingState(player, eAction::MOVE_RIGHT);
    }
    else
    {
        newState = new PlayerFallingState(player, eAction::NONE);
    }
    
    return newState;
}

PlayerState* PlayerWalkingState::handleDeathInput(Player *player, eAction input)
{
    _animationWalk->stopAnimation();
    PlayerState* newState = new PlayerDeathState(player, input);
    
    return newState;
}

PlayerState* PlayerWalkingState::handleFlyInput(Player *player, eAction input)
{
    _animationWalk->stopAnimation();
    PlayerState* newState = new PlayerFlyingState(player, input);
    
    return newState;
}

void PlayerWalkingState::updateBothDirectionsPressed(Player *player)
{
    player->oppositeImpulse();
    
    if (_isPlaying != player->getIsPlaying())
    {
        _isPlaying = player->getIsPlaying();
        //this->setStandingGraphics(player);
    }
}

void PlayerWalkingState::updateRightPressed(Player *player)
{
    player->movePlayerRight();
    
    if ((_isPlaying != player->getIsPlaying()) || (_isPushingRight != player->getIsPushingRight()))
    {
        _isPlaying = player->getIsPlaying();
        this->setWalkingRightGraphics(player);
    }
}

void PlayerWalkingState::updateLeftPressed(Player *player)
{
    player->movePlayerLeft();
    
    if ((_isPlaying != player->getIsPlaying()) || (_isPushingLeft != player->getIsPushingLeft()))
    {
        _isPlaying = player->getIsPlaying();
        this->setWalkingLeftGraphics(player);
    }
}

void PlayerWalkingState::createPushingAnimation(LHSprite *sprite, Player * player)
{
    DelayTime* delay = DelayTime::create(0.5f);
    CallFunc * startCallback = CallFunc::create([this]()
    {
        _animationStartPush->startAnimation(6, false);
    });
    
    CallFunc * pushCallback = CallFunc::create([this, player]()
    {
        CallFunc * soundCallback = this->createSoundWalkCallback(player);
        _animationPush->startAnimationWithCallback(6, soundCallback, true);
//        _animationPush->startAnimation(6, true);
    });
    
    // Aggiungere suono
    
    auto _action = Sequence::create(startCallback, delay, pushCallback, NULL);
    sprite->runAction(_action);
}

void PlayerWalkingState::createStopPushingAnimation()
{
    _animationStartPush->startReverseAnimation(6);
}

void PlayerWalkingState::stopAnimation(Player * player)
{
    if (_isPlaying)
    {
        _animationPlayingWalk->stopAnimation();
    }
    else
    {
        _animationWalk->stopAnimation();
    }
    // Interrompo l'azione di suono dei passi, altrimenti può capitare di sentire il secondo passo.
    player->stopActionByTag(WALKING_SOUND_ACTION_TAG);
}

CallFunc * PlayerWalkingState::createSoundWalkCallback(Player * player)
{
    SoundComponent * soundWalk = _soundWalk;
    CallFunc * soundCallback = CallFunc::create([soundWalk, player]()
    {
        DelayTime * delayA = DelayTime::create(0.05f);
        DelayTime * delayB = DelayTime::create(0.4f);
        CallFunc * playSound = CallFunc::create([soundWalk, player]()
        {
            
            if (soundWalk)
            {
                soundWalk->playOnce();
            }
        });
        Sequence * soundSequence = Sequence::create(delayA, playSound, delayB, playSound->clone(), NULL);
        soundSequence->setTag(WALKING_SOUND_ACTION_TAG);
        player->runAction(soundSequence);
    });
    return soundCallback;
}