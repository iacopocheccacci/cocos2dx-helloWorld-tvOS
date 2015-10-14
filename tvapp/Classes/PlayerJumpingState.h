//
//  PlayerJumpingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#ifndef __SOP__PLAYER_JUMPING_STATE__
#define __SOP__PLAYER_JUMPING_STATE__

#include "PlayerState.h"

class SoundComponent;

class PlayerJumpingState : public PlayerState
{
public:
    virtual ~PlayerJumpingState();
    PlayerJumpingState(Player* player, eAction direction);
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);
    
private:
    void setJumpingGraphics(Player* player, bool start);
    void setRightJumpingGraphics(Player* player, bool start);
    void setLeftJumpingGraphics(Player* player, bool start);

    void initJumpingRightState(Player* player);
    void initJumpingLeftState(Player* player);
    void initJumpingState(Player* player);
    
    void handleMoveRightInput(Player* player);
    void handleMoveLeftInput(Player* player);
    void handleStopMoveRightInput(Player* player);
    void handleStopMoveLeftInput(Player* player);
    PlayerState* handleJumpInput(Player* player);
    PlayerState* handleStopJumpInput(Player* player);
    PlayerState* handleDeathInput(Player* player, eAction input);
    PlayerState* handleFlyInput(Player* player, eAction input);
    PlayerState* handleExultInput(Player* player, eAction input);
    
    void updateBothDirectionsPressed(Player* player);
    void updateRightPressed(Player* player);
    void updateLeftPressed(Player* player);
    
    void createJumpAnimation(LHSprite* sprite);
    void createPlayingJumpAnimation(LHSprite* sprite);
    
    bool _isRightPressed;
    bool _isLeftPressed;
    
    SoundComponent * _soundWalk;
    AnimationComponent* _animationJump;
    AnimationComponent* _animationFall;
    AnimationComponent* _animationRebounce;
    AnimationComponent* _animationPlayingJump;
    AnimationComponent* _animationPlayingFall;
    AnimationComponent* _animationPlayingRebounce;
};


#endif // __SOP__PLAYER_JUMPING_STATE__
