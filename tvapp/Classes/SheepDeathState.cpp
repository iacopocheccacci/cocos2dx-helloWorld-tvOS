//
//  SheepDeathState.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/02/15.
//
//

#include "Sheep.h"
#include "SheepDeathState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

SheepDeathState::SheepDeathState(Sheep* sheep, eAction input)
{
    _animationBurn = sheep->getAnimationBurn();
    _soundBurn = sheep->getSoundBurn();
    this->setGraphics(sheep, input);
}

SheepState* SheepDeathState::handleInput(Sheep *sheep, eAction input)
{
    // do nothing
    return nullptr;
}

void SheepDeathState::update(Sheep *sheep)
{
    // do nothing
}

void SheepDeathState::setGraphics(Sheep* sheep, eAction input)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        switch (input)
        {
            case eAction::BURN:
                this->burnAndRemove(sheep);
                break;
                
            default:
                break;
        }
    }
}

void SheepDeathState::stopAnimation(Sheep* sheep)
{
    // TODO: stop current animation
}

void SheepDeathState::burnAndRemove(Sheep *sheep)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        DelayTime* delay0 = DelayTime::create(.25f);
        DelayTime* delay = DelayTime::create(2.0f);
        CallFunc * burnCallback = CallFunc::create([this, sheep]()
        {
            // fix: pecora su bottone non rilascia il bottone se bruciata
            sheep->getBox2dBody()->SetActive(false);
            
            _animationBurn->startAnimation(false);
//            _animationBurn->startAnimation(24, false);
            if (_soundBurn)
            {
                _soundBurn->playOnce();
            }
        });
        
        CallFunc * removeCallback = CallFunc::create([this, sheep]()
        {
            sheep->removeBody();
            sheep->removeFromParent();
        });
        
        auto _action = Sequence::create(delay0, burnCallback, delay, removeCallback, NULL);
        sprite->runAction(_action);
    }
}