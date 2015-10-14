//
//  WolfWithPlayerAndBrazierState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#ifndef __SOP__WOLF_WITH_PLAYER_AND_BRAZIER_STATE__
#define __SOP__WOLF_WITH_PLAYER_AND_BRAZIER_STATE__

#include "WolfState.h"

class WolfWithPlayerAndBrazierState : public WolfState
{
public:
    WolfWithPlayerAndBrazierState(Wolf* wolf, bool fromBrazierState);
    virtual WolfState* handleCollision(Wolf* wolf, eColliderType collider);
    virtual void update(Wolf* wolf);
    virtual bool isDangerous()      { return false; }
    
private:
    virtual void setGraphics(Wolf* wolf);
};

#endif // __SOP__WOLF_WITH_PLAYER_AND_BRAZIER_STATE__
