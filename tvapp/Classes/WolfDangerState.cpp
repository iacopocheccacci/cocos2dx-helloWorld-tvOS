//
//  WolfDangerState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#include "Wolf.h"
#include "WolfDangerState.h"
#include "WolfWithPlayerState.h"
#include "WolfWithBrazierState.h"

WolfDangerState::WolfDangerState(Wolf* wolf)
{
    _action = nullptr;
    _wolf = wolf;
    
    this->setGraphics(wolf);
    if (_wolf->getSheepCounter() > 0)
    {
        _wolf->catchSheep();
    }
    
    this->checkIfSheepWithDelay();
}

WolfDangerState::~WolfDangerState()
{
    if(_wolf && _action)
    {
        _wolf->stopAction(_action);
    }
}

WolfState* WolfDangerState::handleCollision(Wolf *wolf, eColliderType collider)
{
    WolfState* newState = nullptr;
    
    switch (collider)
    {
        case eColliderType::WOLF_PLAYER_BEGIN:
            newState = new WolfWithPlayerState(wolf);
            break;
            
        case eColliderType::WOLF_BRAZIER_BEGIN:
            wolf->incrementActiveBrazierCounter();
            newState = new WolfWithBrazierState(wolf, false);
            break;
            
        case eColliderType::WOLF_SHEEP_BEGIN:
//            wolf->incrementSheepCounter();
//            wolf->catchSheep();
            break;
            
        default:
            break;
    }
    
    return newState;
}

void WolfDangerState::update(Wolf *wolf)
{

}

void WolfDangerState::setGraphics(Wolf* wolf)
{
    auto sprite = dynamic_cast<LHSprite*>(wolf->getChildNodeWithName(wolfDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (wolf->containsTag("WORLD_5"))
        {
            sprite->setSpriteFrame("lupo_GROTTA_occhi_w5.png");
        }
        else
        {
            sprite->setSpriteFrame("lupo_danger.png");
        }
    }
}

void WolfDangerState::checkIfSheepWithDelay()
{
    DelayTime* delay = DelayTime::create(3.0f);
    CallFunc * catchCallback = CallFunc::create([this]()
    {
        if (_wolf->getSheepCounter() > 0)
        {
            _wolf->catchSheep();
        }
    });
    
    _action = RepeatForever::create(Sequence::create(delay, catchCallback, NULL));
    _wolf->runAction(_action);
}