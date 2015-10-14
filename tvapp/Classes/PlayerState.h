//
//  PlayerState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/12/14.
//
//

#ifndef SOP_PLAYER_STATE_H
#define SOP_PLAYER_STATE_H

class Player;

class PlayerState
{
public:
    virtual ~PlayerState()  {}
    virtual PlayerState* handleInput(Player* player, eAction input) { return nullptr; }
    virtual void update(Player* player)     {}
    virtual void enter(Player* player)      {}
    virtual void exit(Player* player)       {}
    
protected:
    bool _isPlaying;
};

#endif  // SOP_PLAYER_STATE_H
