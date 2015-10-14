//
//  WolfDangerState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#ifndef __SOP__WOLF_DANGER_STATE__
#define __SOP__WOLF_DANGER_STATE__

#include "WolfState.h"

class WolfDangerState : public WolfState
{
public:
    WolfDangerState(Wolf* wolf);
    virtual ~WolfDangerState();
    virtual WolfState* handleCollision(Wolf* wolf, eColliderType collider);
    virtual void update(Wolf* wolf);
    virtual bool isDangerous()          { return true; }
    
private:
    virtual void setGraphics(Wolf* wolf);
    void checkIfSheepWithDelay();
    
    RepeatForever* _action;
    Wolf* _wolf;
};

#endif // __SOP__WOLF_DANGER_STATE__
