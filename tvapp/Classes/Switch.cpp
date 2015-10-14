//
//  Switch.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/12/14.
//
//

#include "Switch.h"
#include "SoundComponent.h"

bool Switch::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _observers.clear();
        _switchOn = false;
        _itemBelowCounter = 0;
        
        this->initAudio();
    }
    
    return status;
}

void Switch::initAudio()
{
    _soundPress = SoundComponent::create(SoundComponent::SOUND_SWITCH_PRESS);
    _soundPress->setEnable(false);
    this->addChild(_soundPress);
    
    _soundRelease = SoundComponent::create(SoundComponent::SOUND_SWITCH_RELEASE);
    _soundRelease->setEnable(false);
    this->addChild(_soundRelease);
    
    // Ritardo l'attivazione delle componenti del suono nel caso in cui il pulsante cambi stato all'inizio del livello
    CallFunc * delayedEnable = CallFunc::create([this]()
    {
        _soundPress->setEnable(true);
        _soundRelease->setEnable(true);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), delayedEnable, NULL));
}

void Switch::addObserver(Observer *observer)
{
    _observers.push_back(observer);
}

void Switch::removeObserver(Observer *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void Switch::updateStatus(bool begin)
{
    this->stopActionByTag(static_cast<int>(eAction::SWITCH_OFF));
    
    if (begin)
    {
        _itemBelowCounter++;
        
        if ( !_switchOn)
        {
            _switchOn = true;
            this->moveSwitch();
            this->notify(eEventType::SWITCH_ON);
        }
    }
    else
    {
        if (_itemBelowCounter >0)
        {
            _itemBelowCounter--;
        }
        
        if (_switchOn && (_itemBelowCounter == 0))
        {
            CallFunc * delayedSwitchOff = CallFunc::create([this]() {
                _switchOn = false;
                this->moveSwitch();
                this->notify(eEventType::SWITCH_OFF);
            });
            Sequence * delayedSwitchOffSeq = Sequence::create(DelayTime::create(0.1f), delayedSwitchOff, NULL);
            delayedSwitchOffSeq->setTag(static_cast<int>(eAction::SWITCH_OFF));
            this->runAction(delayedSwitchOffSeq);
            
//            _switchOn = false;
//            this->moveSwitch();
//            this->notify(eEventType::SWITCH_OFF);
        }
    }
}

void Switch::notify(eEventType event)
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

void Switch::moveSwitch()
{
    if (_switchOn)
    {
//        this->runAction(MoveTo::create(0.05f, Point(this->getPosition().x, this->getPosition().y - 30.0f)));
        _soundPress->playOnce();
        this->setPosition(Point(this->getPosition().x, this->getPosition().y - 30.0f));
    }
    else
    {
//        this->runAction(MoveTo::create(0.05f, Point(this->getPosition().x, this->getPosition().y + 30.0f)));
        _soundRelease->playOnce();
        this->setPosition(Point(this->getPosition().x, this->getPosition().y + 30.0f));
    }
}
