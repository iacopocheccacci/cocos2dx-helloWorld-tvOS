//
//  Observer.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/12/14.
//
//

#ifndef SOP_OBSERVER_H
#define SOP_OBSERVER_H

enum class eEventType : int
{
    SWITCH_OFF = 0,
    SWITCH_ON,
    BRAZIER_TURNED_OFF,
    DOOR_OPEN,
    DOOR_SHAKE,
    INTERACTIVE_FALL,
    EOLO_LOAD,
    EOLO_BLOW,
    EOLO_STOP,
    EOLO_UNLOAD
};

class Observer
{
public:
    virtual ~Observer() {}
    virtual void update(eEventType event, cocos2d::Ref * sender, void * options = nullptr) = 0;
//    virtual void update(eEventType event, cocos2d::Ref * sender) = 0;
};

#endif // SOP_OBSERVER_H
