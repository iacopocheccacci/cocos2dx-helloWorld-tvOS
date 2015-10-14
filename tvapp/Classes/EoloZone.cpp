//
//  EoloZone.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 25/05/15.
//
//

#include "EoloZone.h"
#include "WindMovable.h"

bool EoloZone::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _observers.clear();
        _bezierNode = nullptr;
        _objectList.clear();
        _pendingObjectList.clear();
        _emitter = nullptr;
        
        this->setIdleState();
        this->scheduleUpdate();
        this->resetTornadoPosition();
    }
    
    return status;
}

void EoloZone::update(float dt)
{
    if (_state == eEoloZoneState::ACTIVE)
    {
        this->updateObjectsPosition();
        this->updateTornadoPosition();
    }
}

void EoloZone::notify(eEventType event)
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

void EoloZone::addObserver(Observer *observer)
{
    _observers.push_back(observer);
}

void EoloZone::removeObserver(Observer *observer)
{
    _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
}

void EoloZone::objectIn(WindMovable *object)
{
    _objectList.insert(object);
}

void EoloZone::objectOut(WindMovable *object)
{
    if (_objectList.find(object) != _objectList.end())
    {
        _objectList.erase(object);
    }
}

void EoloZone::pendingObjectIn(WindMovable *object)
{
    if (_pendingObjectList.find(object) == _pendingObjectList.end())
    {
        _pendingObjectList.insert(object);
    }
}

void EoloZone::pendingObjectOut(WindMovable *object)
{
    if (_pendingObjectList.find(object) != _pendingObjectList.end())
    {
        _pendingObjectList.erase(object);
    }
}

void EoloZone::handleObjectsIn(WindMovable* object)
{
    if ((_state == eEoloZoneState::IDLE) || (_state == eEoloZoneState::LOAD))
    {
        if (_state == eEoloZoneState::IDLE)
        {
            this->playWindAction();
        }
        
        this->objectIn(object);
    }
    else if (_state == eEoloZoneState::ACTIVE)
    {
        this->pendingObjectIn(object);
    }
}

void EoloZone::handleObjectsOut(WindMovable *object)
{
    if (_state == eEoloZoneState::LOAD)
    {
        this->objectOut(object);
        if (_objectList.size() == 0)
        {
            auto action = this->getActionByTag(500);
            if (action)
            {
                this->stopAction(action);
            }
            
            _state = eEoloZoneState::IDLE;
            this->notify(eEventType::EOLO_UNLOAD);
        }
    }
    else if (_state == eEoloZoneState::ACTIVE)
    {
        this->pendingObjectOut(object);
    }
}

void EoloZone::setIdleState()
{
    _state = eEoloZoneState::IDLE;
    LHPhysicsProtocol* object = nullptr;
    
    if (_bezierNode)
    {
        auto animation = dynamic_cast<LHAnimation*>(_bezierNode->getAnimationWithName(eoloZoneDefs::BEZIER_ANIMATION));
        if (animation)
        {
            animation->setActive(false);
            animation->setCurrentFrame(0);
        }
    }
    
    auto emitter = this->getEmitterInstance();
    if (emitter)
    {
        // Update alpha particellari
        Color4F c;
        c = emitter->getStartColor();
        emitter->setStartColor(Color4F(c.r, c.g, c.b, 0.6f));
        c = emitter->getStartColorVar();
        emitter->setStartColorVar(Color4F(c.r, c.g, c.b, 0.0f));
        c = emitter->getEndColor();
        emitter->setEndColor(Color4F(c.r, c.g, c.b, 0.6f));
        c = emitter->getEndColorVar();
        emitter->setEndColorVar(Color4F(c.r, c.g, c.b, 0.0f));
    }

    for (std::set<WindMovable*>::iterator it = _objectList.begin(); it!= _objectList.end(); ++it)
    {
        object = dynamic_cast<LHPhysicsProtocol*>(*it);
        if (object)
        {
            auto windMovable = dynamic_cast<WindMovable*>(object);
            if (windMovable)
            {
                windMovable->exitUnderEoloWind();
            }
            
            object->getBox2dBody()->SetActive(true);

            auto node = dynamic_cast<LHNode*>(object);
            if (node)
            {
                auto rotation = RotateTo::create(.2f, 0.0f, 0.0f);
                node->runAction(rotation);
            }
        }
    }
    
    if (this->checkIfPendingObjects())
    {
        this->playWindAction();
    }
}

void EoloZone::setLoadingState()
{
    _state = eEoloZoneState::LOAD;
}

void EoloZone::setActiveState()
{
    this->startTornadoAnimation();
    
    if (_bezierNode)
    {
        auto animation = dynamic_cast<LHAnimation*>(_bezierNode->getAnimationWithName(eoloZoneDefs::BEZIER_ANIMATION));
        if (animation)
        {
            this->setState(eEoloZoneState::ACTIVE);
            _bezierNode->setActiveAnimation(animation);
            animation->restart();
        }
        
        auto emitter = this->getEmitterInstance();
        if (emitter)
        {
            // Update alpha particellari
            Color4F c;
            c = emitter->getStartColor();
            emitter->setStartColor(Color4F(c.r, c.g, c.b, 0.0f));
            c = emitter->getStartColorVar();
            emitter->setStartColorVar(Color4F(c.r, c.g, c.b, 0.0f));
            c = emitter->getEndColor();
            emitter->setEndColor(Color4F(c.r, c.g, c.b, 0.0f));
            c = emitter->getEndColorVar();
            emitter->setEndColorVar(Color4F(c.r, c.g, c.b, 0.0f));
        }
    }
}

void EoloZone::updateObjectsPosition()
{
    LHNodeProtocol* object = nullptr;
    
    for (std::set<WindMovable*>::iterator it = _objectList.begin(); it!= _objectList.end(); ++it)
    {
        object = dynamic_cast<LHNodeProtocol*>(*it);
        if (object)
        {
            auto physicNode = dynamic_cast<LHPhysicsProtocol*>(object);
            auto objectNode = LHNodeProtocol::LHGetNode(object);
            
            if (physicNode && objectNode)
            {
                if (physicNode->getBox2dBody()->IsActive())
                {
                    physicNode->getBox2dBody()->SetActive(false);
                }
                
                auto windMovable = dynamic_cast<WindMovable*>(*it);
                if (windMovable)
                {
                    windMovable->updateUnderEoloWind(_bezierNode);
                }
            }
        }

    }
}

void EoloZone::setupObjectState(bool begin)
{
    WindMovable* object = nullptr;

    for (std::set<WindMovable*>::iterator it = _objectList.begin(); it!= _objectList.end(); ++it)
    {
        object = dynamic_cast<WindMovable*>(*it);
        if (object)
        {
            object->setUnderEoloWind(_bezierNode, begin);
        }
    }
}

void EoloZone::playWindAction()
{
    this->setLoadingState();
    
    auto loadingDelay = DelayTime::create(1.5f);
    this->notify(eEventType::EOLO_LOAD);
    
    auto activateCallback = CallFunc::create([this]()
     {
         this->setActiveState();
         this->setupObjectState(true);
         this->notify(eEventType::EOLO_BLOW);
     });
    
    auto activeDelay = DelayTime::create(2.f);
    
    auto resetCallback = CallFunc::create([this]()
    {
        this->setupObjectState(false);
        this->setIdleState();
    });
    
    auto stopDelay = DelayTime::create(1.f);
    
    auto stopCallback = CallFunc::create([this]()
    {
        this->notify(eEventType::EOLO_STOP);
    });
    
    auto action = Sequence::create(loadingDelay, activateCallback, activeDelay, resetCallback, stopDelay, stopCallback, NULL);
    action->setTag(500);
    this->runAction(action);
}

bool EoloZone::checkIfPendingObjects()
{
    bool bRes = false;
    _objectList.clear();
    
    if (!_pendingObjectList.empty())
    {
        _objectList = _pendingObjectList;
        _pendingObjectList.clear();
        bRes = true;
    }
    
    return bRes;
}

void EoloZone::startTornadoAnimation()
{
    auto tornado = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloZoneDefs::TORNADO_SPRITE_NAME));
    
    if (tornado)
    {
        tornado->setScale(2.f, 2.f);
        tornado->setVisible(true);
        
        auto startAnimation = tornado->getAnimationWithName(eoloZoneDefs::START_ANIMATION);
        auto loopAnimation = tornado->getAnimationWithName(eoloZoneDefs::LOOP_ANIMATION);
        auto stopAnimation = tornado->getAnimationWithName(eoloZoneDefs::STOP_ANIMATION);
        
        if (startAnimation && loopAnimation && stopAnimation)
        {
            auto startCallback = CallFunc::create([startAnimation]()
            {
                startAnimation->setActive(true);
                startAnimation->restart();
            });
            
            auto delay1 = DelayTime::create(1.f);
            
            auto loopCallback = CallFunc::create([loopAnimation]()
            {
                loopAnimation->setActive(true);
                loopAnimation->restart();
            });
            
            auto delay2 = DelayTime::create(1.7f);
            
            auto stopCallback = CallFunc::create([stopAnimation]()
            {
                stopAnimation->setActive(true);
                stopAnimation->restart();
            });
            
            auto resetCallback = CallFunc::create([this]()
            {
                this->resetTornadoPosition();
            });
            
            tornado->runAction(Sequence::create(startCallback, delay1, loopCallback, delay2, stopCallback, delay1,resetCallback, NULL));
        }
    }
}

void EoloZone::updateTornadoPosition()
{
    auto tornado = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloZoneDefs::TORNADO_SPRITE_NAME));
    if (tornado)
    {
        auto gameWorld = _bezierNode->getParent();
        auto tornadoNode = dynamic_cast<LHNode*>(tornado->getParent());
        
        if (gameWorld && tornadoNode)
        {
            Point screenCoord = gameWorld->convertToWorldSpace(_bezierNode->getPosition());
            Point relativePosition = tornadoNode->convertToNodeSpace(screenCoord);
            
            tornado->setPosition(Point(relativePosition.x, tornado->getPositionY()));
        }
    }
}

void EoloZone::resetTornadoPosition()
{
    auto tornado = dynamic_cast<LHSprite*>(this->getChildNodeWithName(eoloZoneDefs::TORNADO_SPRITE_NAME));
    
    if (tornado)
    {
        tornado->setPositionX(0.f);
        
        if (_state != eEoloZoneState::ACTIVE)
        {
            tornado->setVisible(false);
        }
    }
}

ParticleSystemQuad* EoloZone::getEmitterInstance()
{
    if (_emitter == nullptr)
    {
        _emitter = dynamic_cast<ParticleSystemQuad*>(this->getChildByName("Glow"));
    }

    return _emitter;
}
