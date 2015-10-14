//
//  PlayerStandingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#ifndef __SOP_PLAYER_STANDING_STATE__
#define __SOP_PLAYER_STANDING_STATE__

#include "PlayerState.h"

class PlayerStandingState : public PlayerState
{
public:
    PlayerStandingState(Player* player, bool fromJump);
    virtual ~PlayerStandingState();
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);

private:
    void setGraphics(Player* player);
    void stopAnimation(Player* player);
    
    PlayerState* handleMove(Player* player, eAction input);
    PlayerState* handleJump(Player* player, eAction input);
    PlayerState* handleFall(Player* player, eAction input);
    PlayerState* handleDeath(Player* player, eAction input);
    PlayerState* handleFly(Player* player, eAction input);
    
    AnimationComponent* _animationIdle;
    AnimationComponent* _animationPlayingIdle;
    Action* _action;
    bool _isFromJump;
};

#endif // __SOP_PLAYER_STANDING_STATE__
