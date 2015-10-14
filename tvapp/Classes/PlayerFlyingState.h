//
//  PlayerFlyingState.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 26/05/15.
//
//

#ifndef __SOP__PLAYER_FLYING_STATE__
#define __SOP__PLAYER_FLYING_STATE__

#include "PlayerState.h"
#include "Definitions.h"

class AnimationComponent;
class SoundComponent;

class PlayerFlyingState : public PlayerState
{
public:
    PlayerFlyingState(Player* player, eAction input);
    virtual ~PlayerFlyingState();
    virtual PlayerState* handleInput(Player* player, eAction input);
    virtual void update(Player* player);
    
private:
    void setGraphics(Player* player, eAction input);
    PlayerState* handleStopFlyInput(Player* player);
    
    void startAnimation(Player * player);
    void stopAnimation(Player* player);
    
    AnimationComponent* _animationFly;
    Action * _actionLoop;
    SoundComponent* _soundFly;
    
    Point _originalPanSpritePosition;
};

#endif // __SOP__PLAYER_FLYING_STATE__
