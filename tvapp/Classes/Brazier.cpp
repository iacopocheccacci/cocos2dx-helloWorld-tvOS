//
//  Brazier.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 03/12/14.
//
//

#include "Brazier.h"
#include "Goatskin.h"
#include "Utility.h"

#include "Player.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"
#include "GameLogic.h"

#if CC_TARGET_PLATFORM!=CC_PLATFORM_ANDROID
	#define BRAZIER_PARTICLE_ENABLED
#endif

enum class eBrazierType
{
    STATIC_BRAZIER = 0,
    NONSTATIC_BRAZIER,
};

Brazier::~Brazier()
{
    this->unscheduleUpdate();
}

bool Brazier::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHShape::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _brazierStatus = eBrazierStatus::BRAZIER_ON;
        this->initAnimation();
#ifdef BRAZIER_PARTICLE_ENABLED
        this->initParticleSmoke(prnt);
#endif
    	this->scheduleUpdate();
	}
    
    return status;
}

void Brazier::initAudio(LHNode * player)
{
    _soundFire = SoundComponent::create(SoundComponent::SOUND_BRAZIER);
    this->addChild(_soundFire);
    _soundFire->startPlay();
    
    _soundFire->setPositionRefNode(player);
    this->updateAudio(0.0f);
    
    _soundFireTurnOff = SoundComponent::create(SoundComponent::SOUND_BRAZIER_TURN_OFF);
    this->addChild(_soundFireTurnOff);
}

void Brazier::initAnimation()
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(brazierDefs::FIRE_SPRITE));
    _onAnimation = nullptr;
    
    if (sprite)
    {
        switch (this->getType())
        {
            case eBrazierType::STATIC_BRAZIER:
                _onAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_GROUND_ON);
                _turnOffByWindAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_GROUND_OFF);
                _smokeAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_GROUND_SMOKE);
                break;
                
            case eBrazierType::NONSTATIC_BRAZIER:
                _onAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_INTERACTIVE_ON);
                _turnOffByWindAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_INTERACTIVE_OFF);
                _smokeAnimation = AnimationComponent::create(sprite, AnimationComponent::BRAZIER_INTERACTIVE_SMOKE);
                break;
            default:
                break;
        }
        
        this->addChild(_onAnimation);
        this->addChild(_turnOffByWindAnimation);
        this->addChild(_smokeAnimation);
        
        _onAnimation->startAnimationFromRandomFrame(6, true);
    }
}

void Brazier::initParticleSmoke(Node * parentNode)
{
    Node * parentAsset = dynamic_cast<LHAsset *>(parentNode);
    if (parentAsset)
    {
        _particleSmoke = ParticleSystemQuad::create(brazierDefs::PARTICLE_SMOKE);
        _particleSmoke->setPositionType(ParticleSystem::PositionType::RELATIVE);
        _particleSmoke->setScale(0.5f);
        parentAsset->addChild(_particleSmoke, -10);
        
        this->updateParticleSmoke(0);
    }
    else
    {
        CCLOG("Attenzione: Asset non trovato per add di particle smoke");
    }
}
    
void Brazier::setUnderEoloEffect()
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(brazierDefs::FIRE_SPRITE));
    if (sprite)
    {
        this->handleTurnOffByWind(sprite);
    }
}

void Brazier::blockEffectOnFire()
{
    if (this->getBrazierStatus() == eBrazierStatus::BRAZIER_ON)
    {
        auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(brazierDefs::FIRE_SPRITE));
        if (sprite)
        {
            this->handleTurnOffByBlock(sprite);
        }
    }
}

void Brazier::TurnOffIfInWindRange(Goatskin* source)
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(brazierDefs::FIRE_SPRITE));
    if (sprite)
    {
        if (source->containsTag("LEFT"))
        {
            //sprite->setScaleX((-1) * sprite->getScaleX());
            sprite->setFlippedX(true);
        }
        else
        {
            sprite->setFlippedX(false);
        }
        
        if (Utility::isInXrange(source, this) && Utility::isInYrange(source, this))
        {
            this->handleTurnOffByWind(sprite);
        }
    }
}

void Brazier::turnOffFire()
{
    auto fire = dynamic_cast<LHNode*>(this->getChildNodeWithName(brazierDefs::FIRE_NODE));
    if (fire)
    {
        this->removeChild(fire);
        this->removeAllObservers();
    }
}

void Brazier::update(float dt)
{
    this->updateAudio(dt);
#ifdef BRAZIER_PARTICLE_ENABLED
    this->updateParticleSmoke(dt);
#endif
}

void Brazier::updateAudio(float dt)
{
    if (_soundFire)
    {
        _soundFire->updateParameterDistanceFromRefNode(true);
        _soundFire->updateParameterDirectionTowardsRefNode();
    }
}

void Brazier::updateParticleSmoke(float dt)
{
    if (_particleSmoke && _particleSmoke->isActive() && _particleSmoke->getParent())
    {
        auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(brazierDefs::FIRE_SPRITE));
        if (sprite)
        {
            Point localPosition = Point(sprite->getContentSize().width/2.0f, sprite->getContentSize().height/3.0f);
            Point screenPosition = sprite->convertToWorldSpace(localPosition);
            Point gameWorldPosition = _particleSmoke->getParent()->convertToNodeSpace(screenPosition);
            _particleSmoke->setPosition(gameWorldPosition);
        }
    }
}

void Brazier::addObserver(Observer *observer)
{
    _observers.push_back(observer);
}

void Brazier::removeObserver(Observer *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void Brazier::removeAllObservers()
{
    _observers.clear();
}

void Brazier::notify(eEventType event)
{
    auto observersCopy = _observers;
    for (std::vector<Observer*>::const_iterator iterator = observersCopy.begin(); iterator != observersCopy.end(); ++iterator)
    {
        if (*iterator != NULL)
        {
            (*iterator)->update(event, this);
        }
    }
}

eBrazierType Brazier::getType()
{
	eBrazierType type = eBrazierType::STATIC_BRAZIER; // default
    
    if (this->containsTag(gameTags::STATIC_BRAZIER_TAG))
    {
        type = eBrazierType::STATIC_BRAZIER;
    }
    else if (this->containsTag(gameTags::NON_STATIC_BRAZIER_TAG))
    {
        type = eBrazierType::NONSTATIC_BRAZIER;
    }
    
    return type;
}

void Brazier::handleTurnOffByWind(LHSprite* sprite)
{
    float turnOffFrameNr;
    
    switch (this->getType())
    {
        case eBrazierType::STATIC_BRAZIER:
            turnOffFrameNr = 13;
            break;
            
        case eBrazierType::NONSTATIC_BRAZIER:
            turnOffFrameNr = 7;
            break;
            
        default:
            break;
    }
    
    _onAnimation->stopAnimation();
    _particleSmoke->stopSystem();
    
    if (_soundFire)
    {
        _soundFire->stopPlay();
    }
    
    if (_soundFireTurnOff)
    {
        _soundFireTurnOff->playOnce();
    }
    
    CallFunc* turnOffCallback = CallFunc::create([this, turnOffFrameNr]()
    {
        _turnOffByWindAnimation->startAnimation(turnOffFrameNr, false);
        _brazierStatus = eBrazierStatus::BRAZIER_OFF;
    });
    
    DelayTime* delay1 = DelayTime::create(0.3f);
    
    CallFunc* notifyWolfCallback = CallFunc::create([this]
    {
        this->notify(eEventType::BRAZIER_TURNED_OFF);
    });
    
    DelayTime* delay2 = DelayTime::create(0.7f);
    
    CallFunc* smokeCallback = CallFunc::create([this]()
    {
        _smokeAnimation->startAnimation(9, false);
    });
    
    
    CallFunc* removeCallback = CallFunc::create([this]()
    {
        this->turnOffFire();
    });
    
    DelayTime* delay3 = DelayTime::create(1.0f);
    
    auto action = Sequence::create(turnOffCallback, delay1, notifyWolfCallback, delay2, smokeCallback, delay3, removeCallback, NULL);
    sprite->runAction(action);
}

void Brazier::handleTurnOffByBlock(LHSprite *sprite)
{
    _onAnimation->stopAnimation();
    _particleSmoke->stopSystem();
    
    if (_soundFire)
    {
        _soundFire->stopPlay();
    }
    
    if (_soundFireTurnOff)
    {
        _soundFireTurnOff->playOnce();
    }
    
    CallFunc* smokeCallback = CallFunc::create([this]()
    {
        _smokeAnimation->startAnimation(9, false);
        _brazierStatus = eBrazierStatus::BRAZIER_OFF;
        this->notify(eEventType::BRAZIER_TURNED_OFF);
    });
    
    CallFunc* removeCallback = CallFunc::create([this]()
    {
        this->turnOffFire();
    });
    
    DelayTime* delay2 = DelayTime::create(1.0f);
    
    auto _action = Sequence::create(smokeCallback, delay2, removeCallback, NULL);
    sprite->runAction(_action);
}
