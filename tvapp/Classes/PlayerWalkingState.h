//
//  PlayerWalkingState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#ifndef __SOP__PLAYER_WALKING_STATE__
#define __SOP__PLAYER_WALKING_STATE__

#include "PlayerState.h"

class SoundComponent;
class AnimationComponent;

class PlayerWalkingState : public PlayerState
{
public:
    virtual ~PlayerWalkingState();
    PlayerWalkingState(Player* player, eAction direction);
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);
    
private:
    void setWalkingRightGraphics(Player* player);
    void setWalkingLeftGraphics(Player* player);
    void setStandingGraphics(Player* player);
    
    void initMoveRightState(Player* player);
    void initMoveLeftState(Player* player);
    
    void handleMoveRightInput(Player* player);
    void handleMoveLeftInput(Player* player);
    PlayerState* handleStopMoveRightInput(Player* player);
    PlayerState* handleStopMoveLeftInput(Player* player);
    PlayerState* handleJumpInput(Player* player);
    PlayerState* handleFallInput(Player* player);
    PlayerState* handleDeathInput(Player* player, eAction input);
    PlayerState* handleFlyInput(Player* player, eAction input);
    
    void updateBothDirectionsPressed(Player* player);
    void updateRightPressed(Player* player);
    void updateLeftPressed(Player* player);
    
    void createPushingAnimation(LHSprite* sprite, Player * player);
    void createStopPushingAnimation();
    void stopAnimation(Player * player);
    CallFunc * createSoundWalkCallback(Player * player);
    
    bool _isRightPressed;
    bool _isLeftPressed;
    bool _isPushingRight;
    bool _isPushingLeft;
    
//    Player * _player;
    
    SoundComponent * _soundWalk;
    AnimationComponent* _animationWalk;
    AnimationComponent* _animationPlayingWalk;
    AnimationComponent* _animationStartPush;
    AnimationComponent* _animationPush;
};

#endif // __SOP__PLAYER_MOVING_STATE__
