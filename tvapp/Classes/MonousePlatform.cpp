//
//  MonousePlatform.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 02/12/14.
//
//

#include "MonousePlatform.h"
#include "Utility.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"
#include "Drop.h"

bool MonousePlatform::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHShape::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _lifeTimer = 3;
        _animationTimer = 1;
        _timersStarted = false;
        
        Utility::applyTopGhostVertex(this);
        
        this->initAudio();
    }
    
    return status;
}

void MonousePlatform::initAudio()
{
    _soundFall = SoundComponent::create(SoundComponent::SOUND_MONOUSE_PLATFORM_FALL);
    this->addChild(_soundFall);
}

void MonousePlatform::initAnimation(int worldNumber)
{
    std::string animationName;
    Sprite * sprite = nullptr;
    
    sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(monousePlatformDefs::SPRITE_0_NAME));
    if (sprite)
    {
        animationName = AnimationComponent::MONOUSE_PLATFORM_SINGLE_FALL;
    }
    else
    {
        sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(monousePlatformDefs::SPRITE_1_NAME));
        if (sprite)
        {
            animationName = AnimationComponent::MONOUSE_PLATFORM_1_FALL;
        }
        else
        {
            sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(monousePlatformDefs::SPRITE_2_NAME));
            if (sprite)
            {
                animationName = AnimationComponent::MONOUSE_PLATFORM_2_FALL;
            }
            else
            {
                sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(monousePlatformDefs::SPRITE_3_NAME));
                if (sprite)
                {
                    animationName = AnimationComponent::MONOUSE_PLATFORM_3_FALL;
                }
                else
                {
                    sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(monousePlatformDefs::SPRITE_4_NAME));
                    if (sprite)
                    {
                        animationName = AnimationComponent::MONOUSE_PLATFORM_4_FALL;
                    }
                }
            }
        }
    }
    
    
    if (sprite)
    {
        // TODO: gestire meglio le diverse piattaforme monouso.
        // Se stiamo usando un livello di test allora settiamo di default la piattaforma del mondo 3
        if(worldNumber != 2 && worldNumber != 3 && worldNumber != 4)
        {
            if (worldNumber == 5)
            {
                worldNumber = 2;
            }
            else
            {
                worldNumber = 3;
            }
        }
        //
        
        char worldStr[4];
        snprintf(worldStr, 4, "_w%d", worldNumber);
        
        char animationNameForWorld[50];
        snprintf(animationNameForWorld, 50, animationName.c_str(), worldStr, "%04d");
        
        _animationFall = AnimationComponent::create(sprite, animationNameForWorld);
        this->addChild(_animationFall);
    }
}

void MonousePlatform::startLifeTimer()
{
    if (!_timersStarted)
    {
        _timersStarted = true;
        this->schedule(schedule_selector(MonousePlatform::lifeTimeScheduler), 0.1f);
        this->schedule(schedule_selector(MonousePlatform::animationTimeScheduler), 0.1f);
    }
}

void MonousePlatform::lifeTimeScheduler(float dt)
{
    if (_lifeTimer == 0)
    {
        this->getBox2dBody()->SetActive(false);
        this->unschedule(schedule_selector(MonousePlatform::lifeTimeScheduler));
    }
    _lifeTimer--;
}

void MonousePlatform::animationTimeScheduler(float dt)
{
    if (_animationTimer == 0)
    {
        CallFunc * removeCallback = CallFunc::create([this]()
        {
            this->removeFromParentAndCleanup(true);
            
        });
        
        this->stopDropAnimationWithDelay();
        _animationFall->startAnimationWithCallback(12, removeCallback, false);
        _soundFall->playOnce();
        this->unschedule(schedule_selector(MonousePlatform::animationTimeScheduler));
    }
    _animationTimer--;
}

void MonousePlatform::stopDropAnimation()
{
    auto childVec = this->getChildren();
    
    for (Vector<Node*>::iterator it = childVec.begin(); it != childVec.end(); ++it)
    {
        auto drop = dynamic_cast<Drop*>(*it);
        if (drop)
        {
            drop->removeFromParent();
        }
    }
}

void MonousePlatform::stopDropAnimationWithDelay()
{
    auto delay = DelayTime::create(.4f);
    
    auto stopCallback = CallFunc::create([this]()
    {
        this->stopDropAnimation();
    });
    
    this->runAction(Sequence::create(delay, stopCallback, NULL));
}