//
//  WolfWithPlayerAndBrazierState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#include "Wolf.h"
#include "WolfWithBrazierState.h"
#include "WolfWithPlayerState.h"
#include "WolfWithPlayerAndBrazierState.h"
#include "SoundComponent.h"

WolfWithPlayerAndBrazierState::WolfWithPlayerAndBrazierState(Wolf* wolf, bool fromBrazierState)
{
    if (!fromBrazierState)
    {
        if(wolf->getSoundWolfDiscovered()) wolf->getSoundWolfDiscovered()->playOnce();
    }
    this->setGraphics(wolf);
}

WolfState* WolfWithPlayerAndBrazierState::handleCollision(Wolf *wolf, eColliderType collider)
{
    WolfState* newState = nullptr;
    
    switch (collider)
    {
        case eColliderType::WOLF_PLAYER_END:
            newState = new WolfWithBrazierState(wolf, true);
            break;
            
        case eColliderType::WOLF_BRAZIER_BEGIN:
            wolf->incrementActiveBrazierCounter();
            break;
            
        case eColliderType::WOLF_BRAZIER_END:
            wolf->decrementActiveBrazierCounter();
            if (wolf->getActiveBrazierCounter() == 0)
            {
                newState = new WolfWithPlayerState(wolf);
            }
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

void WolfWithPlayerAndBrazierState::update(Wolf *wolf)
{
    
}

void WolfWithPlayerAndBrazierState::setGraphics(Wolf* wolf)
{
    auto sprite = dynamic_cast<LHSprite*>(wolf->getChildNodeWithName(wolfDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (wolf->containsTag("WORLD_5"))
        {
            sprite->setSpriteFrame("lupo_illuminato_w5.png");
        }
        else
        {
            sprite->setSpriteFrame("lupo_illuminato.png");
        }
    }
    
}
