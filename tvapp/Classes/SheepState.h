//
//  SheepState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/12/14.
//
//

#ifndef SOP_SHEEP_STATE_H
#define SOP_SHEEP_STATE_H

#include "Definitions.h"

class Sheep;

class SheepState
{
public:
    virtual ~SheepState()  {}
    virtual SheepState* handleInput(Sheep* sheep, eAction input) { return nullptr; }
    virtual void update(Sheep* sheep, float dt)  {}
    
    virtual void stopAnimation()  {}
};


#endif // SOP_SHEEP_STATE_H
