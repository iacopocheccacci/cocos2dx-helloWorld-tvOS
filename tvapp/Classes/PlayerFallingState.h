//
//  PlayerFallingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 10/02/15.
//
//

#ifndef __SOP__PLAYER_FALLING_STATE__
#define __SOP__PLAYER_FALLING_STATE__

#include "PlayerState.h"

class PlayerFallingState : public PlayerState
{
public:
    PlayerFallingState(Player* player, eAction input);
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);
    
private:
    void setFallingGraphics(Player* player, bool start);
    void setRightFallingGraphics(Player* player, bool start);
    void setLeftFallingGraphics(Player* player, bool start);
    
    void initFallingRightState(Player* player);
    void initFallingLeftState(Player* player);
    void initFallingState(Player* player);
    
    void handleMoveRightInput(Player* player);
    void handleMoveLeftInput(Player* player);
    void handleStopMoveRightInput(Player* player);
    void handleStopMoveLeftInput(Player* player);
    PlayerState* handleStopFallInput(Player* player);
    PlayerState* handleDeathInput(Player* player, eAction input);
    PlayerState* handleFlyInput(Player* player, eAction input);
    PlayerState* handleExultInput(Player* player, eAction input);
    
    void updateRightPressed(Player* player);
    void updateLeftPressed(Player* player);
    
    bool _isRightPressed;
    bool _isLeftPressed;
    
    SoundComponent * _soundWalk;
    AnimationComponent* _animationFall;
    AnimationComponent* _animationRebounce;
    AnimationComponent* _animationPlayingFall;
    AnimationComponent* _animationPlayingRebounce;
};


#endif // __SOP__PLAYER_FALLING_STATE__
