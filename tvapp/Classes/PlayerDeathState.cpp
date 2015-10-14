//
//  PlayerDeathState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#include "Player.h"
#include "PlayerDeathState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

PlayerDeathState::PlayerDeathState(Player* player, eAction input)
{
    player->setMovingLeft(false);
    player->setMovingRight(false);
    player->setJumping(false);
    
    _animationBurn = player->getAnimationBurning();
    _soundBurn = player->getSoundBurning();
    
    _isPlaying = player->getIsPlaying();
    this->setGraphics(player, input);
}

PlayerDeathState::~PlayerDeathState()
{
}

PlayerState* PlayerDeathState::handleInput(Player *player, eAction input)
{
    // do nothing
    return nullptr;
}

void PlayerDeathState::update(Player *player)
{
    // do nothing
}

void PlayerDeathState::setGraphics(Player* player, eAction input)
{
    auto sprite = dynamic_cast<LHSprite*>(player->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        switch (input)
        {
            case eAction::BURN:
                _animationBurn->startAnimation(25, false);
                if (_soundBurn)
                {
                    _soundBurn->playOnce();
                }
                break;
                
            default:
                break;
        }
    }
}

void PlayerDeathState::stopAnimation(Player* player)
{
    // TODO: stop current animation
}