//
//  PlayerFlyingState.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 26/05/15.
//
//

#include "Player.h"
#include "PlayerFlyingState.h"
#include "PlayerFallingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

PlayerFlyingState::PlayerFlyingState(Player* player, eAction input)
{
    player->setMovingLeft(false);
    player->setMovingRight(false);
    player->setJumping(false);
    
    _actionLoop = nullptr;
    
    Node * playerSprite = player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME);
    if (playerSprite)
    {
        _originalPanSpritePosition = playerSprite->getPosition();
    }
    
    _animationFly = player->getAnimationFly();
    
    //TODO: sound
//    _soundFly = player->getSoundFlying();
    
    this->setGraphics(player, input);
}

PlayerFlyingState::~PlayerFlyingState()
{
}

PlayerState* PlayerFlyingState::handleInput(Player *player, eAction input)
{
    PlayerState* newState = nullptr;
    
    switch (input)
    {
        case eAction::STOP_FLY:
            newState = this->handleStopFlyInput(player);
            break;
                    
        default:
            break;
    }
    
    return newState;

}

void PlayerFlyingState::update(Player *player)
{
    // do nothing
}

void PlayerFlyingState::setGraphics(Player* player, eAction input)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        switch (input)
        {
            case eAction::FLY:
                //TODO: add animation & sound
                this->startAnimation(player);
//                if (_soundFly)
//                {
//                    _soundFly->playOnce();
//                }
                break;
                
            default:
                break;
        }
    }
}

PlayerState* PlayerFlyingState::handleStopFlyInput(Player *player)
{
    this->stopAnimation(player);
//    _animationFly->stopAnimation();
    
    return new PlayerFallingState(player, eAction::FALL);
}

void PlayerFlyingState::startAnimation(Player * player)
{
    static const float horizontalDistance = 80.0f;
    static const int animationFrames = 8;
    static const float easeRate = 2.0f;
    
    if (_animationFly)
    {
        _animationFly->startAnimation(animationFrames, true);
    }
    
    Node * playerSprite = player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME);
    if (playerSprite)
    {
        ActionInterval * start;
        ActionInterval * middle;
        ActionInterval * end;
        
        // Oscillazione orizzontale per tornado che cresce
        start = EaseOut::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(horizontalDistance / 4.0f, playerSprite->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point( - horizontalDistance / 2.0f, playerSprite->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(0.0f, playerSprite->getPositionY())), easeRate);
        Sequence * bounceStart = Sequence::create(start, middle, end, NULL);
        
        // Oscillazione orizzontale per tornado che loopa
        start = EaseOut::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(horizontalDistance, playerSprite->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point( - horizontalDistance, playerSprite->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(0.0f, playerSprite->getPositionY())), easeRate);
        Sequence * bounceLoop = Sequence::create(start, middle, end, NULL);
        
        // Uso un Repeat perché il RepeatForever non funziona all'interno di una Sequence
        _actionLoop = Sequence::create(bounceStart, Repeat::create(bounceLoop, 10), NULL);
        playerSprite->runAction(_actionLoop);
    }
}

void PlayerFlyingState::stopAnimation(Player* player)
{
    if (_animationFly)
    {
        _animationFly->stopAnimation();
    }
    
    Node * playerSprite = player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME);
    if (playerSprite)
    {
        if (_actionLoop)
        {
                playerSprite->stopAction(_actionLoop);
        }
        playerSprite->setPosition(_originalPanSpritePosition);
    }
}