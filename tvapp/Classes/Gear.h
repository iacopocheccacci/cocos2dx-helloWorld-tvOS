//
//  Gear.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 27/02/15.
//
//

#ifndef __SOP__GEAR__
#define __SOP__GEAR__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Observer.h"
#include "Definitions.h"

USING_NS_CC;

namespace gearDefs
{
    static const char* CLASS_NAME = "Gear";
}

class Gear : public LHSprite, public Observer
{
public:
    NODEWITHDICT_FUNC(Gear);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    // Observer method
    virtual void update(eEventType event, Ref * sender, void * options);
    
private:
    bool _isOpen;
};

#endif // __SOP__GEAR__
