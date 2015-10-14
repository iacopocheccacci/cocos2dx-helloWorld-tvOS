	//
//  AnimationComponent.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/02/15.
//
//

#include "AnimationComponent.h"
#include "SoundComponent.h"
#include "Definitions.h"

const char * AnimationComponent::PLAYER_IDLE = "panIdle_01_%04d.png";
const char * AnimationComponent::PLAYER_WALK_MENU = "cammina_01_%05d.png";
const char * AnimationComponent::PLAYER_WALK = "panCammina_02_%04d.png";
const char * AnimationComponent::PLAYER_JUMP = "panSalta_01_%04d.png";
const char * AnimationComponent::PLAYER_FALL = "panSalta_02_%04d.png";
const char * AnimationComponent::PLAYER_REBOUNCE = "panSalta_03_%04d.png";
const char * AnimationComponent::PLAYER_PLAYING_REBOUNCE = "panSaltaSuona_03_%04d.png";
const char * AnimationComponent::PLAYER_PLAYING_FALL = "panSaltaSuona_02_%04d.png";
const char * AnimationComponent::PLAYER_PLAYING_IDLE = "panIdleSuona_01_%04d.png";
const char * AnimationComponent::PLAYER_PLAYING_WALK = "panCamminaSuona_01_%04d.png";
const char * AnimationComponent::PLAYER_PLAYING_JUMP = "panSaltaSuona_01_%04d.png";
const char * AnimationComponent::PLAYER_START_PUSHING = "panSpinge_01_%04d.png";
const char * AnimationComponent::PLAYER_PUSHING = "panSpinge_02_%04d.png";
const char * AnimationComponent::PLAYER_BURNING = "panBrucia_01_%04d.png";
const char * AnimationComponent::PLAYER_SLIDING = "panScivola_01_%04d.png";
const char * AnimationComponent::PLAYER_EXULT_FALL = "panEsulta_02_%04d.png";
const char * AnimationComponent::PLAYER_EXULT_REBOUNCE = "panEsulta_03_%04d.png";
const char * AnimationComponent::PLAYER_FLY = "panTornado_%04d.png";

const char * AnimationComponent::SHEEP_IDLE_1 = "pecoraIdle_01_%04d.png";
const char * AnimationComponent::SHEEP_IDLE_2 = "pecoraIdle_02_%04d.png";
const char * AnimationComponent::SHEEP_IDLE_3 = "pecoraIdle_03_%04d.png";
const char * AnimationComponent::SHEEP_WALK = "pecoraCammina_01_%04d.png";
const char * AnimationComponent::SHEEP_JUMP = "pecoraSalta_01_%04d.png";
const char * AnimationComponent::SHEEP_FALL = "pecoraSalta_02_%04d.png";
const char * AnimationComponent::SHEEP_BURN = "pecoraBrucia_01_%04d.png";
const char * AnimationComponent::SHEEP_CATCH = "Pecora_schiacciata_%04d.png";
const char * AnimationComponent::SHEEP_FLY = "pecoraTornado%04d.png";

const char * AnimationComponent::MONSTER_IDLE_1 = "PecoraMostro_bela_%04d";// Sostituire con idle generico
const char * AnimationComponent::MONSTER_IDLE_2 = "PecoraMostro_bela_%04d";
const char * AnimationComponent::MONSTER_WALK = "pecoraMostro_cammina_%04d";
const char * AnimationComponent::MONSTER_JUMP = "pecoraMostro_Salta_01_%04d";
const char * AnimationComponent::MONSTER_FALL = "pecoraMostro_Salta_02_%04d";
const char * AnimationComponent::MONSTER_BURN = "pecoraMostroBrucia_%04d";
const char * AnimationComponent::MONSTER_CATCH = "PecoraMostro_schiacciata_%04d";
const char * AnimationComponent::MONSTER_FLY = "pecoraTornado%04d.png";

const char * AnimationComponent::HOR_GOATSKIN_OPEN = "Otre_orizzontale_01_%04d.png";
const char * AnimationComponent::HOR_GOATSKIN_BLOW = "Otre_orizzontale_02_%04d.png";
const char * AnimationComponent::VER_GOATSKIN_OPEN = "Otre_verticale_01_%04d.png";
const char * AnimationComponent::VER_GOATSKIN_BLOW = "Otre_verticale_02_%04d.png";

const char * AnimationComponent::MONOUSE_PLATFORM_SINGLE_FALL = "monouso%s_01_%s.png";
const char * AnimationComponent::MONOUSE_PLATFORM_1_FALL = "monouso%s_02_%s.png";
const char * AnimationComponent::MONOUSE_PLATFORM_2_FALL = "monouso%s_01_%s.png";
const char * AnimationComponent::MONOUSE_PLATFORM_3_FALL = "monouso%s_03_%s.png";
const char * AnimationComponent::MONOUSE_PLATFORM_4_FALL = "monouso%s_02_%s.png";

const char * AnimationComponent::WOLF_CATCH_FAR = "lupo_%04d.png";
const char * AnimationComponent::WOLF_CATCH_NEAR = "lupo_davanti_%04d.png";

const char * AnimationComponent::BRAZIER_GROUND_ON = "fuoco_aterra_acceso_%04d.png";
const char * AnimationComponent::BRAZIER_GROUND_OFF = "fuoco_aterra_spento_%04d.png";
const char * AnimationComponent::BRAZIER_GROUND_SMOKE = "fumo_aterra_%04d.png";
const char * AnimationComponent::BRAZIER_INTERACTIVE_ON = "fuoco_braciere_acceso_%04d.png";
const char * AnimationComponent::BRAZIER_INTERACTIVE_OFF = "fuoco_braciere_spento_%04d.png";
const char * AnimationComponent::BRAZIER_INTERACTIVE_SMOKE = "fumo_braciere__%04d.png";

const char * AnimationComponent::FLUTE = "flauto_part_%02d.png";

const char * AnimationComponent::TUTORIAL_PRESS = "thumb_%02d.png";

const char * AnimationComponent::RAIN = "pioggia_%04d";
const char * AnimationComponent::DROP = "gocce-loop_%04d.png";

AnimationComponent::AnimationComponent()
{
    
}

AnimationComponent::~AnimationComponent()
{
    
}

AnimationComponent* AnimationComponent::create(Sprite* sprite, const std::string animationName)
{
    AnimationComponent * ret = new (std::nothrow) AnimationComponent();
    if (ret != nullptr && ret->init(sprite, animationName))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    return ret;
}

bool AnimationComponent::init(Sprite* sprite, const std::string animationName)
{
    bool init = false;
    
    if (Node::init())
    {
        _sprite = sprite;
        _animationId = animationName;
        _frameNr = 0;
        _startingFrame = 0;
        init = true;
    }
    
    return init;
}

void AnimationComponent::setCustomFrames(std::vector<int> frames)
{
    _customFrames = frames;
    _frameNr = (int)_customFrames.size();
}

float AnimationComponent::getDuration()
{
    float duration = 0.0f;
    if (_frameNr > 0)
    {
        duration = ( (float)(_frameNr - 1) * animationComponentDefs::TIME_BETWEEN_FRAMES);
    }
    return duration;
}

void AnimationComponent::startAnimation(bool repeat)
{
    this->startAnimation(_frameNr, repeat);
}

void AnimationComponent::startAnimation(unsigned int frameNr, bool repeat)
{
//    this->stopAnimation();
    _frameNr = frameNr;
    auto animate = this->createAnimation(frameNr);
    
    ActionInterval * action;
    if (repeat)
    {
        action = RepeatForever::create(animate);
    }
    else
    {
        action = animate;
    }
    action->setTag(static_cast<int>(eAction::ANIMATION));
    _sprite->runAction(action);
}

void AnimationComponent::startAnimationWithSound(unsigned int frameNr, SoundComponent *sound)
{
//    this->stopAnimation();
    _frameNr = frameNr;
    ActionInterval * action = this->createAnimation(frameNr);
    if (sound)
    {
        sound->playOnce();
    }
    action->setTag(static_cast<int>(eAction::ANIMATION));
    _sprite->runAction(action);
}

void AnimationComponent::startAnimationWithCallback(unsigned int frameNr, CallFunc * callback, bool before)
{
    this->startAnimationWithCallback(frameNr, callback, before, true);
}

void AnimationComponent::startAnimationWithCallback(unsigned int frameNr, CallFunc * callback, bool before, bool repeatForever)
{
//    this->stopAnimation();
    _frameNr = frameNr;
    auto animate = this->createAnimation(frameNr);
    
    ActionInterval * action;
    if ( callback )
    {
        if ( before )
        {
            action = Sequence::create(callback, animate, NULL);
        }
        else
        {
            action = Sequence::create(animate, callback, NULL);
        }
    }
    else
    {
        action = animate;
    }
    
    if (repeatForever)
    {
        action = RepeatForever::create(action);
    }
    action->setTag(static_cast<int>(eAction::ANIMATION));
    _sprite->runAction(action);
}

void AnimationComponent::startReverseAnimation()
{
    this->startReverseAnimation(_frameNr);
}

void AnimationComponent::startReverseAnimation(unsigned int frameNr)
{
//    this->stopAnimation();
    _frameNr = frameNr;
    
    ActionInterval * action = this->createAnimation(frameNr, true);
    action->setTag(static_cast<int>(eAction::ANIMATION));
    _sprite->runAction(action);
}

void AnimationComponent::startAnimationFromRandomFrame(unsigned int frameNr, bool repeat)
{
//    this->stopAnimation();
    _frameNr = frameNr;
    
    ActionInterval * action = this->createAnimationFromRandomFrame(frameNr);
    
    if (repeat)
    {
        action = RepeatForever::create(action);
    }
    action->setTag(static_cast<int>(eAction::ANIMATION));
    _sprite->runAction(action);
}

void AnimationComponent::stopAnimation()
{
    this->stopAnimation(true);
}

void AnimationComponent::stopAnimation(bool allActions)
{
    if (allActions)
    {
        _sprite->stopAllActions();
    }
    else
    {
        _sprite->stopAllActionsByTag(static_cast<int>(eAction::ANIMATION));
    }
}

Animate * AnimationComponent::createAnimation(unsigned int frameNr)
{
    return this->createAnimation(frameNr, false);
}

Animate * AnimationComponent::createAnimation(unsigned int frameNr, bool reverse)
{
    Vector<SpriteFrame*> animFrames(frameNr);
    char str[100] = {0};
    
    if (_customFrames.size() > 0)
    {
        for(int count = 0; count < _customFrames.size(); count++)
        {
            snprintf(str, 100, _animationId.c_str(),_customFrames.at(count));
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            if (!frame)
            {
                frame = Sprite::create(str)->getSpriteFrame();
            }
            animFrames.pushBack(frame);
        }
    }
    else
    {
        for(int i = _startingFrame; i < frameNr + _startingFrame; i++)
        {
            snprintf(str, 100, _animationId.c_str(),i);
            auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
            if (!frame)
            {
                frame = Sprite::create(str)->getSpriteFrame();
            }
            animFrames.pushBack(frame);
        }
    }
    
    Animation * animation = Animation::createWithSpriteFrames(animFrames, animationComponentDefs::TIME_BETWEEN_FRAMES);
    Animate * animate = Animate::create(animation);
    
    if (reverse)
    {
        animate = animate->reverse();
    }
    
    return animate;
}

Animate * AnimationComponent::createAnimationFromRandomFrame(unsigned int frameNr)
{
    Vector<SpriteFrame*> animFrames(frameNr);
    char str[100] = {0};
    int i = CCRANDOM_0_1() * frameNr;
    
    for(int count = 0; count < frameNr; count++)
    {
        snprintf(str, 100, _animationId.c_str(),i);
        auto frame = SpriteFrameCache::getInstance()->getSpriteFrameByName(str);
        if (!frame)
        {
            frame = Sprite::create(str)->getSpriteFrame();
        }
        animFrames.pushBack(frame);
        
        i = (i + 1) % frameNr;
    }
    
    auto animation = Animation::createWithSpriteFrames(animFrames, animationComponentDefs::TIME_BETWEEN_FRAMES);
    return Animate::create(animation);
}
