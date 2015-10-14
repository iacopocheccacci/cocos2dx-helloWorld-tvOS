//
//  Eolo.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 29/05/15.
//
//

#include "Eolo.h"
#include "SoundComponent.h"

bool Eolo::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        this->initAudio();
    }
    
    return status;
}

void Eolo::initAudio()
{
    _soundLoad = SoundComponent::create(SoundComponent::SOUND_EOLO_LOAD);
    this->addChild(_soundLoad);
    
    _soundBlow = SoundComponent::create(SoundComponent::SOUND_EOLO_TORNADO);
    this->addChild(_soundBlow);
}

void Eolo::setIdleState()
{
    this->startUpDownMovement();
    
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::SPRITE_NAME));
    if (sprite)
    {
        auto loadAnimation= sprite->getAnimationWithName(eoloDefs::LOAD_ANIMATION);
        if (!loadAnimation->isActive())
        {
            auto stopAnimation = sprite->getAnimationWithName(eoloDefs::STOP_ANIMATION);
            if (stopAnimation)
            {
                stopAnimation->setActive(true);
                stopAnimation->restart();
            }
        }
    }
}

void Eolo::setLoadingState()
{
    this->stopUpDownMovement();
    
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::SPRITE_NAME));
    if (sprite)
    {
        auto loadAnimation = sprite->getAnimationWithName(eoloDefs::LOAD_ANIMATION);
        if (loadAnimation)
        {
            loadAnimation->setActive(true);
            loadAnimation->restart();
            
            this->startInflate();
        }
    }
}

void Eolo::setActiveState()
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::SPRITE_NAME));
    if (sprite)
    {
        auto blowAnimation = sprite->getAnimationWithName(eoloDefs::BLOW_ANIMATION);
        if (blowAnimation)
        {
            blowAnimation->setActive(true);
            blowAnimation->restart();
            
            if (_soundBlow)
            {
                _soundBlow->playOnce();
            }
            
            this->stopInflate();
        }
    }
}

void Eolo::setUnloadState()
{
    this->startUpDownMovement();
    
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::SPRITE_NAME));
    if (sprite)
    {
        auto unloadAnimation = sprite->getAnimationWithName(eoloDefs::UNLOAD_ANIMATION);
        if (unloadAnimation)
        {
            unloadAnimation->setActive(true);
            unloadAnimation->restart();
            
            this->stopInflate();
        }
    }
}

void Eolo::startUpDownMovement()
{
    auto idleAnimation = this->getAnimationWithName(eoloDefs::IDLE_ANIMATION);
    if (idleAnimation)
    {
        idleAnimation->setActive(true);
    }
}

void Eolo::stopUpDownMovement()
{
    auto idleAnimation = this->getAnimationWithName(eoloDefs::IDLE_ANIMATION);
    if (idleAnimation)
    {
        idleAnimation->setActive(false);
    }
}

void Eolo::startInflate()
{
    auto airSprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::AIR_SPRITE_NAME));
    if (airSprite)
    {
        airSprite->setVisible(true);
        
        auto inflateAnimation = airSprite->getAnimationWithName(eoloDefs::INFLATE_ANIMATION);
        if (inflateAnimation)
        {
            inflateAnimation->setActive(true);
            inflateAnimation->restart();
            
            if (_soundLoad)
            {
                _soundLoad->startPlay();
            }
        }
    }
}

void Eolo::stopInflate()
{
    auto airSprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloDefs::AIR_SPRITE_NAME));
    if (airSprite)
    {
        airSprite->setVisible(false);
        
        auto inflateAnimation = airSprite->getAnimationWithName(eoloDefs::INFLATE_ANIMATION);
        if (inflateAnimation)
        {
            inflateAnimation->setActive(false);
            
            if (_soundLoad)
            {
                _soundLoad->stopPlay(true);
            }
        }
    }
}

void Eolo::update(eEventType event, Ref * sender, void * options)
{
    switch (event)
    {
        case eEventType::EOLO_LOAD:
            this->setLoadingState();
            break;
            
        case eEventType::EOLO_BLOW:
            this->setActiveState();
            break;
            
        case eEventType::EOLO_STOP:
            this->setIdleState();
            break;
            
        case eEventType::EOLO_UNLOAD:
            this->setUnloadState();
            break;
            
        default:
            break;
    }
}
