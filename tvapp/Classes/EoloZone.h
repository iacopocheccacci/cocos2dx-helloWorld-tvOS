//
//  EoloZone.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 25/05/15.
//
//

#ifndef __SOP__EOLO_ZONE__
#define __SOP__EOLO_ZONE__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Observer.h"
#include "Definitions.h"

USING_NS_CC;

namespace eoloZoneDefs
{
    static const char* CLASS_NAME = "EoloZone";
    static const char* BEZIER_ANIMATION = "EoloBezier";
    static const char* START_ANIMATION = "start";
    static const char* LOOP_ANIMATION = "loop";
    static const char* STOP_ANIMATION = "stop";
    static const char* TORNADO_SPRITE_NAME = "TornadoSprite";
}

enum class eEoloZoneState : int
{
    IDLE = 0,
    LOAD,
    ACTIVE
};
    
class WindMovable;

class EoloZone : public LHNode
{
public:
    NODEWITHDICT_FUNC(EoloZone);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    
    void setBezierNode(LHNode* bezierNode)              { _bezierNode = bezierNode; }
    LHNode* getBezierNode()                             { return _bezierNode; }
    void setState(eEoloZoneState state)                 { _state = state; }
    eEoloZoneState getState()                           { return _state; }
    void handleObjectsIn(WindMovable* object);
    void handleObjectsOut(WindMovable* object);
    
private:
    virtual void update(float dt);
    void notify(eEventType event);
    void objectIn(WindMovable* object);
    void objectOut(WindMovable* object);
    void pendingObjectIn(WindMovable* object);
    void pendingObjectOut(WindMovable* object);
    void setIdleState();
    void setLoadingState();
    void setActiveState();
    void updateObjectsPosition();
    void setupObjectState(bool begin);
    void playWindAction();
    bool checkIfPendingObjects();
    void startTornadoAnimation();
    void updateTornadoPosition();
    void resetTornadoPosition();
    ParticleSystemQuad* getEmitterInstance();
    
    std::vector<Observer*> _observers;
    
    LHNode* _bezierNode;
    eEoloZoneState _state;
    std::set<WindMovable*> _objectList;
    std::set<WindMovable*> _pendingObjectList;
    ParticleSystemQuad* _emitter;
};

#endif // __SOP__EOLO_ZONE__
