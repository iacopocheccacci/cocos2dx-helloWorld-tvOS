//
//  Goatskin.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/12/14.
//
//

#include "Goatskin.h"
#include "Utility.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

using namespace gameObjectsParams;

bool Goatskin::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = InteractiveBlock::initWithDictionary(dict, prnt);
    
    if(status)
    {
        auto value = Utility::loadDataFromFile(GOATSKIN_GROUP, GOATSKIN_RANGE_NAME, GOATSKIN_RANGE_VAL);
        _range = atof(value->getCString());
        _status = eGoatskinStatus::GOATSKIN_IDLE;
        
        this->initAnimation();
        this->initAudio();
    }
    
    return status;
}

void Goatskin::initAnimation()
{
    auto sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(goatskinDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (this->containsTag(gameTags::DIRECTION_UP_TAG))
        {
            _animationOpen = AnimationComponent::create(sprite, AnimationComponent::VER_GOATSKIN_OPEN);
            _animationBlow = AnimationComponent::create(sprite, AnimationComponent::VER_GOATSKIN_BLOW);
        }
        else
        {
            _animationOpen = AnimationComponent::create(sprite, AnimationComponent::HOR_GOATSKIN_OPEN);
            _animationBlow = AnimationComponent::create(sprite, AnimationComponent::HOR_GOATSKIN_BLOW);
        }
        this->addChild(_animationOpen);
        this->addChild(_animationBlow);
    }
}

void Goatskin::initAudio()
{
    _soundPressed = SoundComponent::create(SoundComponent::SOUND_GOATSKIN_PRESSED);
    this->addChild(_soundPressed);
    _soundBlow = SoundComponent::create(SoundComponent::SOUND_GOATSKIN_BLOW);
    this->addChild(_soundBlow);
    
    // il goatskin non fa il suono dell'interactive base
}

bool Goatskin::turnOn(float impulse, bool begin)
{
    bool open = false;
    
    // TODO: Gestire caso di impatto leggero per l'effetto audio e grafico di "scombussolamento"
    
    if (begin && (impulse > goatskinDefs::THRESHOLD_TURN_ON))
    {
        if(_soundBlow)
        {
            _soundBlow->playOnce();
        }
        
        // Goatskin turned on
        open = true;
        _status = eGoatskinStatus::GOATSKIN_ACTIVE;
    }
    else if (begin && (impulse > goatskinDefs::THRESHOLD_SHAKE))
    {
        if(_soundPressed)
        {
            _soundPressed->playOnce();
        }
    }
    
    return open;
}

void Goatskin::openAction()
{
    _animationOpen->startAnimation(3, false);
}

void Goatskin::blowAction()
{
    _animationBlow->startAnimation(11, false);
}