//
//  Flute.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 02/03/15.
//
//

#include "Flute.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

bool Flute::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        this->loadAnimation();
        this->initAudio();
        this->initParticle();
        
        _isCatched = false;
    }
    
    return status;
}

void Flute::handleCatch()
{
    if (!_isCatched)
    {
        _isCatched = true;
        
        this->playSound();
        
//        auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(fluteDefs::SPRITE));
//        if (sprite)
//        {
//            sprite->setVisible(false);
//        }
//        auto spriteBG = dynamic_cast<LHSprite*>(this->getChildNodeWithName(fluteDefs::SPRITE_BG));
//        if (spriteBG)
//        {
//            spriteBG->setVisible(false);
//        }
        
        ParticleSystemQuad * emitter = ParticleSystemQuad::create(fluteDefs::PARTICLE_FILE_CATCH);
        emitter->setScale(.7f);
        emitter->setPosition(Point(this->getContentSize()/2.0f));
        emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
        emitter->setAutoRemoveOnFinish(false);
        this->addChild(emitter,-2);
        
        auto fluteBody = this->getBox2dBody();
        if (fluteBody)
        {
            fluteBody->SetActive(false);
        }
        
        float delayBeforeRemove = 0.0f;
        auto catchNoteAnimation = this->getAnimationWithName(fluteDefs::ANIMATION_CATCH_NAME);
        if (catchNoteAnimation)
        {
            catchNoteAnimation->setActive(true);
            catchNoteAnimation->restart();
            delayBeforeRemove = catchNoteAnimation->totalTime();
        }
        this->runAction(Sequence::create(DelayTime::create(delayBeforeRemove), RemoveSelf::create(), NULL));
    }
}

void Flute::playSound()
{
    if (_sound)
    {
        _sound->playOnce();
    }
}

void Flute::loadAnimation()
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(fluteDefs::SPRITE));
    _animation = AnimationComponent::create(sprite, AnimationComponent::FLUTE);
    
    this->addChild(_animation);
    
    _animation->startAnimation(13, true);
}

void Flute::initAudio()
{
    _sound = SoundComponent::create(SoundComponent::SOUND_CATCH_FLUTE);
    this->addChild(_sound);
}

void Flute::initParticle()
{
    auto sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(fluteDefs::SPRITE));
    
    if (sprite)
    {
        ParticleSystemQuad * emitterBG = ParticleSystemQuad::create(fluteDefs::PARTICLE_FILE_BG);
        emitterBG->setScale(.7f);
        emitterBG->setPosition(Point(sprite->getContentSize()/2.0f));
        emitterBG->setPositionType(ParticleSystem::PositionType::RELATIVE);
        emitterBG->setAutoRemoveOnFinish(false);
        sprite->addChild(emitterBG,-2);
    }
}
