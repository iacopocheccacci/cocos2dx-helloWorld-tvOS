//
//  Portal.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 13/02/15.
//
//

#include "Portal.h"
#include <vector>
#include "SoundComponent.h"
#include "Utility.h"

enum class eScoreIconState {
    ACTIVE = 0,
    DISABLED,
    COMPLETE
};

bool Portal::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _state = ePortalState::CLOSED_PORTAL;
        _callback = nullptr;
        _callbackListener = nullptr;
        _timer = portalDefs::PARTICLE_RESIZE_TIME;
        _playerIn = false;
        _isEntered = false;
        _timerDelay = portalDefs::PARTICLE_DELAY;
        _particleGlobalAlphaMultiplier = 1.0f;
        
        _emitterTop = nullptr;
        
        _originalEmitterStartColor = Color4F::BLACK;
        _originalEmitterStartColorVar = Color4F::BLACK;
        _originalEmitterEndColor = Color4F::BLACK;
        _originalEmitterEndColorVar = Color4F::BLACK;
        
        this->initAudio();
        this->initPortalDestination();
    }
    
    return status;
}

void Portal::initAudio()
{
    _soundOpen = SoundComponent::create(SoundComponent::SOUND_PORTAL);
    this->addChild(_soundOpen);
}

void Portal::initPortalDestination()
{
    _portalType = ePortalType::NORMAL;
    _worldDestination = -1;
    _levelDestination = -1;
    
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        for (std::string tag : parentAsset->getTags())
        {
            if (tag.find(gameTags::PORTAL_WORLD_TAG_ROOT) != std::string::npos)
            {
                std::string tagRootStr = gameTags::PORTAL_WORLD_TAG_ROOT;
                std::string worldNumberStr = tag.substr(tagRootStr.size(),tagRootStr.size() + 2);
                _worldDestination = atoi(worldNumberStr.c_str());
                _portalType = ePortalType::WORLD;
                break;
            }
            
            if (tag.find(gameTags::PORTAL_LEVEL_TAG_ROOT) != std::string::npos)
            {
                std::string tagRootStr = gameTags::PORTAL_LEVEL_TAG_ROOT;
                std::string levelNumberStr = tag.substr(tagRootStr.size(),tagRootStr.size() + 2);
                _levelDestination = atoi(levelNumberStr.c_str());
                _portalType = ePortalType::LEVEL;
                break;
            }
            
            if (tag.compare(gameTags::PORTAL_MAIN_MENU_TAG) == 0)
            {
                _portalType = ePortalType::EXIT;
                break;
            }
        }
    }
}

void Portal::initDecorations(Node * gameWorld)
{
    this->initParticle(gameWorld);
    this->initPortalLight();
    this->initScore();
    this->initDestinationLabel();
    this->scheduleUpdate();
}

void Portal::initParticle(Node * gameWorld)
{
    ParticleSystemQuad * emitterUnder = nullptr;
    
    emitterUnder = ParticleSystemQuad::create(portalDefs::PARTICLE_FILE);
    emitterUnder->setPosition(Vec2(this->getContentSize().width/2, this->getContentSize().height/2));
    emitterUnder->setPositionType(ParticleSystem::PositionType::RELATIVE);
    this->addChild(emitterUnder, 1, portalDefs::PARTICLE_UNDER_NAME);
    
    _originalEmitterStartColor = emitterUnder->getStartColor();
    _originalEmitterStartColorVar = emitterUnder->getStartColorVar();
    _originalEmitterEndColor = emitterUnder->getEndColor();
    _originalEmitterEndColorVar = emitterUnder->getEndColorVar();
    
    if (gameWorld)
    {
        _emitterTop = ParticleSystemQuad::create(portalDefs::PARTICLE_FILE);
        
        Point screenPosition = this->convertToWorldSpace(Point(this->getContentSize()/2.0f));
        Point localPosition = gameWorld->convertToNodeSpace(screenPosition);
        _emitterTop->setPosition(localPosition);
        
        _emitterTop->setPositionType(ParticleSystem::PositionType::RELATIVE);
        gameWorld->addChild(_emitterTop, portalDefs::PARTICLE_TOP_Z_ORDER);
    }
    
    this->setActiveParticle(false);
}

void Portal::initPortalLight()
{
    this->setActivePortalLight(false);
}

void Portal::initScore()
{
    this->setStateScoreSheep(eScoreIconState::DISABLED);
    this->setStateScoreFlute(eScoreIconState::DISABLED);
//    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
//    if (parentAsset)
//    {
//        LHSprite * sheepActiveIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_ACTIVE_SPRITE_NAME));
//        if (sheepActiveIcon)
//        {
//            Sprite * sheepDisabledIcon = Sprite::createWithSpriteFrameName(portalDefs::SHEEP_ICON_SPRITE_FRAME_NAME_DISABLED);
//            sheepDisabledIcon->setPosition(Point(sheepActiveIcon->getContentSize()/2.0f));
//            sheepDisabledIcon->setName(portalDefs::SHEEP_ICON_DISABLED_SPRITE_NAME);
//            sheepActiveIcon->addChild(sheepDisabledIcon, -1);
//            
//            Sprite * sheepCompleteIcon = Sprite::createWithSpriteFrameName(portalDefs::SHEEP_ICON_SPRITE_FRAME_NAME_COMPLETE);
//            sheepCompleteIcon->setPosition(Point(sheepActiveIcon->getContentSize()/2.0f));
//            sheepCompleteIcon->setName(portalDefs::SHEEP_ICON_COMPLETE_SPRITE_NAME);
//            sheepActiveIcon->addChild(sheepCompleteIcon, 1);
//            
//            this->setStateScoreSheep(eScoreIconState::DISABLED);
//        }
//        LHSprite * fluteActiveIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_ACTIVE_SPRITE_NAME));
//        if (fluteActiveIcon)
//        {
//            Sprite * fluteDisabledIcon = Sprite::createWithSpriteFrameName(portalDefs::FLUTE_ICON_SPRITE_FRAME_NAME_DISABLED);
//            fluteDisabledIcon->setPosition(Point(fluteActiveIcon->getContentSize()/2.0f));
//            fluteDisabledIcon->setName(portalDefs::FLUTE_ICON_DISABLED_SPRITE_NAME);
//            fluteActiveIcon->addChild(fluteDisabledIcon, -1);
//            
//            Sprite * fluteCompleteIcon = Sprite::createWithSpriteFrameName(portalDefs::FLUTE_ICON_SPRITE_FRAME_NAME_COMPLETE);
//            fluteCompleteIcon->setPosition(Point(fluteActiveIcon->getContentSize()/2.0f));
//            fluteCompleteIcon->setName(portalDefs::FLUTE_ICON_COMPLETE_SPRITE_NAME);
//            fluteActiveIcon->addChild(fluteCompleteIcon, 1);
//            
//            this->setStateScoreFlute(eScoreIconState::DISABLED);
//        }
//    }
}

void Portal::initDestinationLabel()
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        Node * destinationLabelNode = parentAsset->getChildNodeWithName(portalDefs::DESTINATION_LABEL_NODE_NAME);
        if (destinationLabelNode)
        {
            std::string destinationStr = "";
            
            if (ePortalType::LEVEL == _portalType)
            {
                int level = this->getDestinationLevelNumber();
                std::stringstream ss;
                ss << level;
                destinationStr = ss.str();
//                Utility::convertIntToRoman(level, destinationStr);
            }
            else if (ePortalType::EXIT == _portalType)
            {
                destinationStr = portalDefs::DESTINATION_LABEL_EXIT_VALUE;
            }
            
            if (false == destinationStr.empty())
            {
                Label * destinationLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, destinationStr, TextHAlignment::CENTER);
                destinationLabel->setPosition(destinationLabelNode->getContentSize() / 2.0f);
                destinationLabel->setScale(portalDefs::DESTINATION_LABEL_SCALE);
                destinationLabel->setName(portalDefs::DESTINATION_LABEL_NAME);
                destinationLabelNode->addChild(destinationLabel);
            }
        }
        
        this->setActiveDestinationLabel(false);
    }
}

void Portal::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void Portal::update(float dt)
{
    auto emitterUnder = dynamic_cast<ParticleSystemQuad*>(this->getChildByName(portalDefs::PARTICLE_UNDER_NAME));
    
    if (emitterUnder && _emitterTop)
    {
        float particleUnderAlphaMultiplier = _particleGlobalAlphaMultiplier;
        float particleTopAlphaMultiplier = _particleGlobalAlphaMultiplier;
        
        if ((_state == ePortalState::ENTERING_PORTAL) || ((_state == ePortalState::OPEN_PORTAL) && _playerIn))
        {
            if (_timerDelay != 0.0f)
            {
                if (_timer != portalDefs::PARTICLE_RESIZE_TIME)
                {
                    _timer++;
                    _emitterTop->setScale(portalDefs::PARTICLE_RESIZE_TIME/_timer);
                }
                _timerDelay--;
            }
            else
            {
                _emitterTop->setScale(portalDefs::PARTICLE_RESIZE_TIME/_timer);
                
                if (!_isEntered)
                {
                    this->playSoundOpen();
                    _isEntered = true;
                }
                
                if ((--_timer) == 30.0f)
                {
                    auto node = static_cast<Node*>(this);
                    
                    std::string nodeName = this->getPortalName();
                    
                    node->setName(nodeName);
                    
                    if (_callback && _callbackListener)
                    {
                        (_callbackListener->*_callback)(node);
                    }
                }
            }
        }
        else if (_state == ePortalState::OPEN_PORTAL)
        {
            particleUnderAlphaMultiplier *= portalDefs::PARTICLE_IDLE_OPACITY_MULTIPLIER;
            particleTopAlphaMultiplier *= portalDefs::PARTICLE_IDLE_OPACITY_MULTIPLIER;
            
            _timerDelay = portalDefs::PARTICLE_DELAY;
            
            if (_timer != portalDefs::PARTICLE_RESIZE_TIME)
            {
                _isEntered = false;
                _timer++;
                _emitterTop->setScale(portalDefs::PARTICLE_RESIZE_TIME/_timer);
            }
        }
        
        float opacityTimer = _timer - (portalDefs::PARTICLE_RESIZE_TIME - portalDefs::PARTICLE_OPACITY_TIME);
        if (opacityTimer <= portalDefs::PARTICLE_OPACITY_TIME)
        {
            particleUnderAlphaMultiplier *= opacityTimer/portalDefs::PARTICLE_OPACITY_TIME;
            particleTopAlphaMultiplier *= 1.0f - opacityTimer/portalDefs::PARTICLE_OPACITY_TIME;
        }
        
        // Update alpha particellari
        Color4F c;
        c = _originalEmitterStartColor;
        emitterUnder->setStartColor(Color4F(c.r, c.g, c.b, c.a * particleUnderAlphaMultiplier));
        _emitterTop->setStartColor(Color4F(c.r, c.g, c.b, c.a * particleTopAlphaMultiplier));
        c = _originalEmitterStartColorVar;
        emitterUnder->setStartColorVar(Color4F(c.r, c.g, c.b, c.a * particleUnderAlphaMultiplier));
        _emitterTop->setStartColorVar(Color4F(c.r, c.g, c.b, c.a * particleTopAlphaMultiplier));
        c = _originalEmitterEndColor;
        emitterUnder->setEndColor(Color4F(c.r, c.g, c.b, c.a * particleUnderAlphaMultiplier));
        _emitterTop->setEndColor(Color4F(c.r, c.g, c.b, c.a * particleTopAlphaMultiplier));
        c = _originalEmitterEndColorVar;
        emitterUnder->setEndColorVar(Color4F(c.r, c.g, c.b, c.a * particleUnderAlphaMultiplier));
        _emitterTop->setEndColorVar(Color4F(c.r, c.g, c.b, c.a * particleTopAlphaMultiplier));
        
        if (particleUnderAlphaMultiplier > 0.0f)
        {
            emitterUnder->setVisible(true);
        }
        else
        {
            emitterUnder->setVisible(false);
        }
        
        if (particleTopAlphaMultiplier > 0.0f)
        {
            _emitterTop->setVisible(true);
        }
        else
        {
            _emitterTop->setVisible(false);
        }
    }
}

void Portal::open()
{
    this->openWithTransition(0.0f);
}

void Portal::openWithTransition(float time)
{
    _state = ePortalState::OPEN_PORTAL;
    
    this->setActiveParticle(true, time);
    this->setStateScoreSheep(eScoreIconState::ACTIVE, time);
    this->setStateScoreFlute(eScoreIconState::ACTIVE, time);
    this->setActiveDestinationLabel(true, time);
    this->setActivePortalLight(true, time);
}

void Portal::close()
{
    this->closeWithTransition(0.0f);
}

void Portal::closeWithTransition(float time)
{
    _state = ePortalState::CLOSED_PORTAL;
    
    this->setActiveParticle(false, time);
    this->setStateScoreSheep(eScoreIconState::DISABLED, time);
    this->setStateScoreFlute(eScoreIconState::DISABLED, time);
    this->setActiveDestinationLabel(false, time);
    this->setActivePortalLight(false, time);
}

void Portal::openFake(float time)
{
    this->openWithTransition(time/2.0f);
    
    DelayTime * delay = DelayTime::create(time/2.0f);
    CallFunc * callback = CallFunc::create([this, time]() {
        this->closeWithTransition(time/2.0f);
    });
    this->runAction(Sequence::create(delay, callback, NULL));
}

std::string Portal::getPortalName()
{
    std::string portalName = "";
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        for (std::string tag : parentAsset->getTags())
        {
            if (tag.find(gameTags::PORTAL_WORLD_TAG_ROOT) != std::string::npos)
            {
                std::string tagRootStr = gameTags::PORTAL_WORLD_TAG_ROOT;
                std::string worldNumberStr = tag.substr(tagRootStr.size(),tagRootStr.size() + 2);
                portalName = callbackDefs::PORTAL_WORLD_ROOT + worldNumberStr;
                break;
            }
            else if (tag.find(gameTags::PORTAL_LEVEL_TAG_ROOT) != std::string::npos)
            {
                std::string tagRootStr = gameTags::PORTAL_LEVEL_TAG_ROOT;
                std::string levelNumberStr = tag.substr(tagRootStr.size(),tagRootStr.size() + 2);
                portalName = callbackDefs::PORTAL_LEVEL_ROOT + levelNumberStr;
                break;
            }
            else if (tag.find(gameTags::PORTAL_MAIN_MENU_TAG) != std::string::npos)
            {
                portalName = callbackDefs::PORTAL_MAIN_MENU;
                break;
            }
        }
        if (portalName.empty())
        {
            portalName = callbackDefs::PORTAL;
        }
    }
    return portalName;
}

void Portal::playSoundOpen()
{
    if (_soundOpen)
    {
//        _soundOpen->startPlay();
        _soundOpen->playOnce(true);   // sound with follow
        _soundOpen->setDestroySoundOnExit(false);
//        _soundOpen->stopPlay(true, 5.0f); // Stop dopo alcuni secondi (a causa del riverbero non è one-shot)
    }
}

void Portal::stopSoundOpen()
{
    if (_soundOpen)
    {
        _soundOpen->stopPlay(true);
    }
}

void Portal::setCompleteScoreSheep(float transitionTime)
{
    this->setStateScoreSheep(eScoreIconState::COMPLETE, transitionTime);
}

void Portal::setCompleteScoreFlute(float transitionTime)
{
    this->setStateScoreFlute(eScoreIconState::COMPLETE, transitionTime);
}

#pragma mark - Private Methods

void Portal::setActiveParticle(bool active, float transitionTime)
{
    auto emitter = this->getChildByName<ParticleSystemQuad *>(portalDefs::PARTICLE_UNDER_NAME);
    if (emitter)
    {
        std:: string schedulerKey = portalDefs::PARTICLE_TRANSITION_SCHEDULER_KEY;
        int frameIndex = 0;
        int totalFrames = (transitionTime * 60.0f) + 1;
        
        if (this->isScheduled(schedulerKey))
        {
            this->unschedule(schedulerKey);
        }
        
        this->schedule([this, emitter, frameIndex, totalFrames, active, schedulerKey](float dt) mutable {
            
            if (frameIndex == totalFrames)
            {
                this->unschedule(schedulerKey);
            }
            
            if (active)
            {
                _particleGlobalAlphaMultiplier = ( (float)frameIndex / (float)totalFrames );
            }
            else
            {
                _particleGlobalAlphaMultiplier = ( (float)(totalFrames - frameIndex) / (float)totalFrames );
            }
            
            frameIndex++;
        }, schedulerKey);
    }
}

void Portal::setActiveDestinationLabel(bool active, float transitionTime)
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        Node * labelDestNode = parentAsset->getChildNodeWithName(portalDefs::DESTINATION_LABEL_NODE_NAME);
        if (labelDestNode)
        {
            Label * labelDest = labelDestNode->getChildByName<Label *>(portalDefs::DESTINATION_LABEL_NAME);
            if (labelDest)
            {
                if (active)
                {
                    labelDest->setColor(Color3B(portalDefs::DESTINATION_COLOR_OFF));
                    labelDest->setOpacity(portalDefs::DESTINATION_COLOR_OFF.a);
                    labelDest->runAction(TintTo::create(transitionTime, Color3B(portalDefs::DESTINATION_COLOR_ON)));
                    labelDest->runAction(FadeTo::create(transitionTime, portalDefs::DESTINATION_COLOR_ON.a));
                }
                else
                {
                    labelDest->setColor(Color3B(portalDefs::DESTINATION_COLOR_ON));
                    labelDest->setOpacity(portalDefs::DESTINATION_COLOR_ON.a);
                    labelDest->runAction(TintTo::create(transitionTime, Color3B(portalDefs::DESTINATION_COLOR_OFF)));
                    labelDest->runAction(FadeTo::create(transitionTime, portalDefs::DESTINATION_COLOR_OFF.a));
                }
            }
        }
    }
}

void Portal::setActivePortalLight(bool active, float transitionTime)
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        LHSprite * portalLight = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::PORTAL_LIGHT_SPRITE_NAME));
        if (portalLight)
        {
            if (active)
            {
                portalLight->setOpacity(0);
                portalLight->runAction(FadeTo::create(transitionTime, 255));
            }
            else
            {
                portalLight->setOpacity(255);
                portalLight->runAction(FadeTo::create(transitionTime, 0));
            }
        }
    }
}

void Portal::setStateScoreSheep(eScoreIconState newState, float transitionTime)
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        LHSprite * activeIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_ACTIVE_SPRITE_NAME));
        if (activeIcon)
        {
            if (eScoreIconState::ACTIVE == newState)
            {
                Utility::fadeInNodeWithTime(activeIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(activeIcon, transitionTime);
            }
        }
        LHSprite * disabledIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_DISABLED_SPRITE_NAME));
        if (disabledIcon)
        {
            if (eScoreIconState::DISABLED == newState)
            {
                Utility::fadeInNodeWithTime(disabledIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(disabledIcon, transitionTime);
            }
        }
        LHSprite * completeIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_COMPLETE_SPRITE_NAME));
        if (completeIcon)
        {
            if (eScoreIconState::COMPLETE == newState)
            {
                Utility::fadeInNodeWithTime(completeIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(completeIcon, transitionTime);
            }
        }
    }
}

void Portal::setStateScoreFlute(eScoreIconState newState, float transitionTime)
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        LHSprite * activeIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_ACTIVE_SPRITE_NAME));
        if (activeIcon)
        {
            if (eScoreIconState::ACTIVE == newState)
            {
                Utility::fadeInNodeWithTime(activeIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(activeIcon, transitionTime);
            }
        }
        LHSprite * disabledIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_DISABLED_SPRITE_NAME));
        if (disabledIcon)
        {
            if (eScoreIconState::DISABLED == newState)
            {
                Utility::fadeInNodeWithTime(disabledIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(disabledIcon, transitionTime);
            }
        }
        LHSprite * completeIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_COMPLETE_SPRITE_NAME));
        if (completeIcon)
        {
            if (eScoreIconState::COMPLETE == newState)
            {
                Utility::fadeInNodeWithTime(completeIcon, transitionTime);
            }
            else
            {
                Utility::fadeOutNodeWithTime(completeIcon, transitionTime);
            }
        }
    }
}

void Portal::removeScoreFluteIcon()
{
    LHAsset * parentAsset = dynamic_cast<LHAsset *>(this->getParent());
    if (parentAsset)
    {
        LHSprite * fluteActiveIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_ACTIVE_SPRITE_NAME));
        if (fluteActiveIcon)
        {
            fluteActiveIcon->removeFromParentAndCleanup(true);
        }
        LHSprite * fluteDisabledIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_DISABLED_SPRITE_NAME));
        if (fluteDisabledIcon)
        {
            fluteDisabledIcon->removeFromParentAndCleanup(true);
        }
        LHSprite * fluteCompleteIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::FLUTE_ICON_COMPLETE_SPRITE_NAME));
        if (fluteCompleteIcon)
        {
            fluteCompleteIcon->removeFromParentAndCleanup(true);
        }
        LHSprite * sheepActiveIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_ACTIVE_SPRITE_NAME));
        if (sheepActiveIcon)
        {
            sheepActiveIcon->setPositionX(sheepActiveIcon->getParent()->getContentSize().width/2.0f);
        }
        LHSprite * sheepDisabledIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_DISABLED_SPRITE_NAME));
        if (sheepDisabledIcon)
        {
            sheepDisabledIcon->setPositionX(sheepDisabledIcon->getParent()->getContentSize().width/2.0f);
        }
        LHSprite * sheepCompleteIcon = dynamic_cast<LHSprite *>(parentAsset->getChildNodeWithName(portalDefs::SHEEP_ICON_COMPLETE_SPRITE_NAME));
        if (sheepCompleteIcon)
        {
            sheepCompleteIcon->setPositionX(sheepCompleteIcon->getParent()->getContentSize().width/2.0f);
        }
    }
}