//
//  WolfWithPlayerState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#include "Wolf.h"
#include "WolfDangerState.h"
#include "WolfWithPlayerState.h"
#include "WolfWithPlayerAndBrazierState.h"

WolfWithPlayerState::WolfWithPlayerState(Wolf* wolf)
{
    this->setGraphics(wolf);
}

WolfState* WolfWithPlayerState::handleCollision(Wolf *wolf, eColliderType collider)
{
    WolfState* newState = nullptr;
    
    switch (collider)
    {
        case eColliderType::WOLF_PLAYER_END:
            newState = new WolfDangerState(wolf);
            break;
            
        case eColliderType::WOLF_BRAZIER_BEGIN:
            wolf->incrementActiveBrazierCounter();
            newState = new WolfWithPlayerAndBrazierState(wolf, false);
            break;
            
//        case eColliderType::WOLF_SHEEP_BEGIN:
//            wolf->incrementSheepCounter();
//            break;
//            
//        case eColliderType::WOLF_SHEEP_END:
//            wolf->decrementSheepCounter();
//            break;
            
        default:
            break;
    }
    
    return newState;
}

void WolfWithPlayerState::update(Wolf *wolf)
{
    
}

void WolfWithPlayerState::setGraphics(Wolf* wolf)
{
    auto sprite = dynamic_cast<LHSprite*>(wolf->getChildNodeWithName(wolfDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (wolf->containsTag("WORLD_5"))
        {
            sprite->setSpriteFrame("lupo_GROTTA_w5.png");
        }
        else
        {
            sprite->setSpriteFrame("lupo_Pan.png");
        }
    }
    
}
