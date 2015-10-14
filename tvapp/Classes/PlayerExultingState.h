//
//  PlayerExultingState.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 30/04/15.
//
//

#ifndef __SOP__PLAYER_EXULTING_STATE__
#define __SOP__PLAYER_EXULTING_STATE__

#include "PlayerState.h"
#include "Definitions.h"

class AnimationComponent;
class SoundComponent;

class PlayerExultingState : public PlayerState
{
public:
    PlayerExultingState(Player* player, eAction input);
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
    
    void updateBothDirectionsPressed(Player* player);
    void updateRightPressed(Player* player);
    void updateLeftPressed(Player* player);
    
    bool _isRightPressed;
    bool _isLeftPressed;
    
    AnimationComponent* _animationExultFall;
    AnimationComponent* _animationExultRebounce;
    SoundComponent* _soundWalk;
};

#endif // __SOP__PLAYER_EXULTING_STATE__
