//
//  Door.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/12/14.
//
//

#include "Door.h"
#include "SoundComponent.h"
#include "ShakeAction.h"
    
enum class eDoorStatus
{
    CLOSED_DOOR = 0,
    OPEN_DOOR,
};

#pragma mark - Door Class

bool Door::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHSprite::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _status = eDoorStatus::CLOSED_DOOR;
        _type = checkDoorType();
        _activeSwitch = _switchCounter = 0;
        
        this->initBounds();
        this->initAudio();
        
        this->scheduleUpdate();
    }
    
    return status;
}

void Door::initAudio()
{
    _soundOpen = SoundComponent::create(SoundComponent::SOUND_DOOR_OPEN);
    _soundOpen->setEnable(false);
    this->addChild(_soundOpen);
    
    _soundClose = SoundComponent::create(SoundComponent::SOUND_DOOR_CLOSE);
    _soundClose->setEnable(false);
    this->addChild(_soundClose);
    
    _soundShake = SoundComponent::create(SoundComponent::SOUND_DOOR_SHAKE);
    _soundShake->setEnable(false);
    this->addChild(_soundShake);
    
    // Ritardo l'attivazione delle componenti del suono nel caso in cui la porta cambi stato all'inizio del livello
    CallFunc * delayedEnable = CallFunc::create([this]()
    {
        _soundOpen->setEnable(true);
        _soundClose->setEnable(true);
        _soundShake->setEnable(true);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), delayedEnable, NULL));
}

void Door::initBounds()
{
    auto asset = dynamic_cast<LHAsset*>(this->getParent());
    
    if (asset)
    {
        float move;
        
        if (this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
        {
            move = asset->getContentSize().width + 0.5f;
            _lowerBound = Point(this->getPosition().x, this->getPosition().y);
            _upperBound = Point(this->getPosition().x - move, this->getPosition().y);
        }
        else
        {
            move = asset->getContentSize().height + 0.5f;
            _lowerBound = Point(this->getPosition().x, this->getPosition().y);
            _upperBound = Point(this->getPosition().x, this->getPosition().y - move);
        }
        
        _isInverseDirection = false;
    }
}

void Door::update(float dt)
{
    float position;
    float upperBound;
    float lowerBound;
    auto body = this->getBox2dBody();
    
    if (!this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
    {
        position = this->getPosition().y;
        upperBound = _upperBound.y;
        lowerBound = _lowerBound.y;
    }
    else
    {
        position = this->getPosition().x;
        upperBound = _upperBound.x;
        lowerBound = _lowerBound.x;
    }
    
    if (body)
    {
        if (!_isInverseDirection)
        {
            if ((_status == eDoorStatus::OPEN_DOOR) && (position < upperBound))
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                this->setPosition(_upperBound);
            }
            else if ((_status == eDoorStatus::CLOSED_DOOR) && (position > lowerBound))
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                this->setPosition(_lowerBound);
            }
        }
        else
        {
            if ((_status == eDoorStatus::OPEN_DOOR) && (position > upperBound))
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                this->setPosition(_upperBound);
            }
            else if ((_status == eDoorStatus::CLOSED_DOOR) && (position < lowerBound))
            {
                body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                this->setPosition(_lowerBound);
            }
        }

    }
}

void Door::updateSwitchCounter()
{
    if (_type == eDoorType::OR_DOOR)
    {
        _switchCounter = 1;
    }
    else if (_type == eDoorType::AND_DOOR)
    {
        _switchCounter++;
    }
}

void Door::update(eEventType event, Ref * sender, void * options)
{
    switch (event)
    {
        case eEventType::SWITCH_OFF:
            updateActiveSwitch(false);
            break;
        case eEventType::SWITCH_ON:
            updateActiveSwitch(true);
            break;
            
        default:
            break;
    }
    
    if (_status == eDoorStatus::CLOSED_DOOR)
    {
        if (checkIfUnlocked())
        {
            this->openDoor(sender);
        }
        else if (event == eEventType::SWITCH_ON)
        {
            this->shakeDoor(sender);
        }
    }
    else if (!checkIfUnlocked() && _status == eDoorStatus::OPEN_DOOR)
    {
        this->closeDoor(sender);
    }
}

void Door::addObserver(Observer *observer)
{
    _observers.push_back(observer);
}

void Door::removeObserver(Observer *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

eDoorType Door::checkDoorType()
{
    auto type = eDoorType::OR_DOOR; // OR_DOOR is the default type
    
    if (this->containsTag(gameTags::AND_TAG))
    {
        type = eDoorType::AND_DOOR;
    }
    
    return type;
}

void Door::updateActiveSwitch(bool increment)
{
    increment ? _activeSwitch++ : _activeSwitch--;
}

bool Door::checkIfUnlocked()
{
    return (_activeSwitch >= _switchCounter);
}

void Door::openDoor(Ref * doorSwitch)
{
    _status = eDoorStatus::OPEN_DOOR;

    auto asset = dynamic_cast<LHAsset*>(this->getParent());
    if (asset)
    {
        _soundOpen->playOnce();
        
        this->resetShakeEffect();
        
        auto body = this->getBox2dBody();
        
        if (!this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
        {
            float velocity = 10.0f * asset->getScaleY();
            body->SetLinearVelocity(b2Vec2(0.0f, -velocity));
        }
        else
        {
            float velocity = 10.0f * asset->getScaleX();
            body->SetLinearVelocity(b2Vec2(-velocity, 0.0f));
        }
        
        this->notify(eEventType::SWITCH_ON, doorSwitch);
        this->notify(eEventType::DOOR_OPEN, doorSwitch);

        this->playShadowEffectOpen();
    }
}

void Door::closeDoor(Ref * doorSwitch)
{
    _status = eDoorStatus::CLOSED_DOOR;
    
    auto asset = dynamic_cast<LHAsset*>(this->getParent());
    if (asset)
    {
        _soundClose->playOnce();
        
        this->resetShakeEffect();
        
        auto body = this->getBox2dBody();
        
        if (!this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
        {
            float velocity = 10.0f * asset->getScaleY();
            body->SetLinearVelocity(b2Vec2(0.0f, velocity));
        }
        else
        {
            float velocity = 10.0f * asset->getScaleX();
            body->SetLinearVelocity(b2Vec2(velocity, 0.0f));
        }
        
        this->notify(eEventType::SWITCH_OFF, doorSwitch);
        
        this->playShadowEffectClose();
    }
}
    
void Door::shakeDoor(Ref * doorSwitch)
{
    auto body = this->getBox2dBody();
    // Eseguo lo shake solo se la porta è ferma
    if (body && body->GetLinearVelocity().LengthSquared() == 0.0f)
    {
        _soundShake->playOnce();
        
        this->playShakeEffect();
    }
    this->notify(eEventType::DOOR_SHAKE, doorSwitch);
}

void Door::playShakeEffect()
{
    CCLOG("Door Positions: %f, %f", this->getPosition().x, this->getPosition().y);
    
    this->resetShakeEffect();
    
    Vec2 strengthVector;
    if (this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
    {
        strengthVector = Vec2(doorDefs::SHAKE_FORCE, 0.0f);
    }
    else
    {
        strengthVector = Vec2(0.0f, doorDefs::SHAKE_FORCE);
    }
    ShakeAction * shakeAction = ShakeAction::create(doorDefs::SHAKE_DURATION, strengthVector.x, strengthVector.y);
    shakeAction->setTag(static_cast<int>(eAction::SHAKE));
    this->runAction(shakeAction);
}

void Door::resetShakeEffect()
{
    this->stopActionByTag(static_cast<int>(eAction::SHAKE));
    
    if (_status == eDoorStatus::OPEN_DOOR)
    {
        if (this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
        {
            this->setPositionX(_upperBound.x);
        }
        else
        {
            this->setPositionY(_upperBound.y);
        }
    }
    else
    {
        if (this->containsTag(doorDefs::HORIZONTAL_DOOR_TAG))
        {
            this->setPositionX(_lowerBound.x);
        }
        else
        {
            this->setPositionY(_lowerBound.y);
        }
    }
}

void Door::playShadowEffectOpen()
{
    auto shadow = this->getChildNodeWithName(doorDefs::SHADOW_SPRITE_NAME);
    if (shadow)
    {
        shadow->runAction(ScaleBy::create(.6f, 0.5f, 1.0f));
        shadow->runAction(FadeOut::create(.6f));
    }
}

void Door::playShadowEffectClose()
{
    auto shadow = this->getChildNodeWithName(doorDefs::SHADOW_SPRITE_NAME);
    if (shadow)
    {
        shadow->runAction(ScaleBy::create(.63f, 2.0f, 1.0f));
        shadow->runAction(FadeIn::create(.7f));
    }
}

void Door::notify(eEventType event, Ref * doorSwitch)
{
    auto observersCopy = _observers;
    for (std::vector<Observer*>::const_iterator iterator = observersCopy.begin(); iterator != observersCopy.end(); ++iterator)
    {
        if (*iterator != NULL)
        {
            (*iterator)->update(event, this, doorSwitch);
        }
    }
}
