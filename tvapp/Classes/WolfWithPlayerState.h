//
//  WolfWithPlayerState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#ifndef __SOP__WOLF_WITH_PLAYER_STATE__
#define __SOP__WOLF_WITH_PLAYER_STATE__

#include "WolfState.h"

class WolfWithPlayerState : public WolfState
{
public:
    WolfWithPlayerState(Wolf* wolf);
    virtual WolfState* handleCollision(Wolf* wolf, eColliderType collider);
    virtual void update(Wolf* wolf);
    virtual bool isDangerous()      { return false; }
    
private:
    virtual void setGraphics(Wolf* wolf);
};

#endif // __SOP__WOLF_WITH_PLAYER_STATE__
