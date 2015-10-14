//
//  Wolf.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#include "Wolf.h"
#include "WolfDangerState.h"
#include "SoundComponent.h"
#include "AnimationComponent.h"

Wolf::~Wolf()
{
    if (_currentState)
    {
        delete _currentState;
    }
}

bool Wolf::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _sheepArray.init();
        _currentState = new WolfDangerState(this);
        _activeBrazierCounter = 0;
//        _sheepCounter = 0;
        _callback = nullptr;
        _callbackListener = nullptr;
        _catching = false;
        
        this->initAudio();
        this->initAnimation();
    }
    
    return status;
}

void Wolf::initAudio()
{
    _soundWolfDiscovered = SoundComponent::create(SoundComponent::SOUND_WOLF_DISCOVERED);
    _soundWolfDiscovered->setEnable(false);
    this->addChild(_soundWolfDiscovered);
    
    // Ritardo l'attivazione delle componenti del suono nel caso in cui il lupo iniziasse il livello con un bracere davanti
    CallFunc * delayedEnable = CallFunc::create([this]()
    {
        _soundWolfDiscovered->setEnable(true);
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), delayedEnable, NULL));
    
    _soundWolfCatch = SoundComponent::create(SoundComponent::SOUND_WOLF_CATCH);
    this->addChild(_soundWolfCatch);
}

void Wolf::initAnimation()
{
    auto sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(wolfDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (this->containsTag("WORLD_5"))
        {
            _spriteCatch = Sprite::createWithSpriteFrameName(wolfDefs::SPRITE_FILE_W5);
        }
        else
        {
            _spriteCatch = Sprite::createWithSpriteFrameName(wolfDefs::SPRITE_FILE_W4);
        }
        
        sprite->addChild(_spriteCatch);
        _spriteCatch->setNormalizedPosition(Point(0.5f, 0.5f));
        
        _spriteCatchAnimation = Sprite::create();
        _spriteCatch->addChild(_spriteCatchAnimation);
        _spriteCatchAnimation->setPosition(_spriteCatch->getContentSize().width / 2.0f, 0);
        
        _animationCatchFar = AnimationComponent::create(_spriteCatchAnimation, AnimationComponent::WOLF_CATCH_FAR);
        this->addChild(_animationCatchFar);
        
        _animationCatchNear = AnimationComponent::create(_spriteCatchAnimation, AnimationComponent::WOLF_CATCH_NEAR);
        this->addChild(_animationCatchNear);
        
        _spriteCatch->setVisible(false);
    }
}

void Wolf::updateStatus(LHNodeProtocol *node, bool begin)
{
    eColliderType collider = eColliderType::UNUSED_WOLF;
    
    if (node->containsTag(gameTags::PLAYER_TAG))
    {
        collider = begin ? eColliderType::WOLF_PLAYER_BEGIN : eColliderType::WOLF_PLAYER_END;
    }
    else if (node->containsTag(gameTags::BRAZIER_TAG))
    {
        collider = begin ? eColliderType::WOLF_BRAZIER_BEGIN : eColliderType::WOLF_BRAZIER_END;
    }
    else if (node->containsTag(gameTags::SHEEP_TAG))
    {
        this->updateSheepArray(node, begin);
        collider = begin ? eColliderType::WOLF_SHEEP_BEGIN : eColliderType::WOLF_SHEEP_END;
    }
    
    WolfState* state = _currentState->handleCollision(this, collider);
    
    if (state)
    {
        delete _currentState;
        _currentState = state;
    }
}

void Wolf::update(eEventType event, Ref * sender, void * options)
{
    WolfState* state = nullptr;
    
    switch (event)
    {
        case eEventType::BRAZIER_TURNED_OFF:
            state = _currentState->handleCollision(this, eColliderType::WOLF_BRAZIER_END);
            if (state)
            {
                delete _currentState;
                _currentState = state;
            }
            break;
            
        default:
            break;
    }
}

void Wolf::catchSheep()
{
    if (_catching == false)
    {
        auto sheep = dynamic_cast<LHNode*>(_sheepArray.getRandomObject());
        
        if (sheep)
        {
            auto node = static_cast<Node*>(this);
            node->setName(callbackDefs::WOLF);
            
            if (_callback && _callbackListener)
            {
                _catching = true;
                
                this->startAnimationCatch(sheep);
                this->playCatchSound();
                this->updateSheepArray(sheep, false);
                this->safelyRemoveSheep(sheep);
                
                (_callbackListener->*_callback)(node);
            }
        }
    }
}

void Wolf::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void Wolf::deleteSheepFromArrayIfExist(LHNode *sheep)
{
    this->updateSheepArray(sheep, false);
}

void Wolf::startAnimationCatch(LHNode * catchedSheep)
{
    _spriteCatch->setVisible(true);
    
    auto wolfBodyPosition = this->getBox2dBody()->GetPosition();
    auto catchedSheepBodyPosition = catchedSheep->getBox2dBody()->GetPosition();
    float wolfSheepDistanceX = (catchedSheepBodyPosition.x - wolfBodyPosition.x) * PTM_RATIO;
    
    AnimationComponent * animationCatch;
    if ( fabsf(wolfSheepDistanceX) > 110.0f )
    {
        animationCatch = _animationCatchFar;
        _spriteCatchAnimation->setAnchorPoint(Point(0.2f,0));
    }
    else
    {
        animationCatch = _animationCatchNear;
        _spriteCatchAnimation->setAnchorPoint(Point(0.3f,0));
    }
    
    float defaultPositionX = _spriteCatchAnimation->getPositionX();
    
    _spriteCatchAnimation->setPositionX(defaultPositionX + wolfSheepDistanceX);
    
    if ( wolfSheepDistanceX >= 0 && _spriteCatchAnimation->getScaleX() >= 0 )
    {
        _spriteCatchAnimation->setScaleX(_spriteCatchAnimation->getScaleX() * -1);
    }
    else if ( wolfSheepDistanceX < 0 && _spriteCatchAnimation->getScaleX() < 0 )
    {
        _spriteCatchAnimation->setScaleX(_spriteCatchAnimation->getScaleX() * -1);
    }
    
    auto spriteCatch = _spriteCatch;
    auto spriteAnimationCatch = _spriteCatchAnimation;
    CallFunc * endAnimationBack = CallFunc::create([spriteCatch, spriteAnimationCatch, defaultPositionX, animationCatch, this]()
    {
        if (spriteCatch && spriteAnimationCatch && animationCatch)
        {
            spriteCatch->setVisible(false);
            spriteAnimationCatch->setPositionX(defaultPositionX);
            _catching = false;
        }
    });
    
    animationCatch->startAnimationWithCallback(9, endAnimationBack, false, false);
}

void Wolf::playCatchSound()
{
    SoundComponent * soundWolfCatch = _soundWolfCatch;
    CallFunc * playCallback = CallFunc::create([soundWolfCatch]()
    {
        if (soundWolfCatch)
        {
            soundWolfCatch->playOnce();
        }
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.24), playCallback, NULL));
}

void Wolf::updateSheepArray(LHNodeProtocol *node, bool begin)
{
    auto sheep = dynamic_cast<LHNode*>(node);
    
    if (begin)
    {
        if (!_sheepArray.containsObject(sheep))
        {
            _sheepArray.addObject(sheep);
        }
    }
    else
    {
        if (_sheepArray.containsObject(sheep))
        {
            _sheepArray.removeObject(sheep);
        }
    }
}

void Wolf::safelyRemoveSheep(LHNode* sheep)
{
    CallFunc *removeBodyCallback = CallFunc::create([this, sheep]()
    {
        // fix: pecora su bottone non rilascia il bottone se mangiata (Iacopo 29/04/2015)
        sheep->getBox2dBody()->SetActive(false);
    });
    
    CallFunc *removeCallback = CallFunc::create([this, sheep]()
    {
        sheep->removeBody();
        sheep->removeFromParent();
    });
    
    DelayTime * delay = DelayTime::create(0.24f);
    
    this->runAction(Sequence::create(removeBodyCallback, delay, removeCallback, NULL));
}
