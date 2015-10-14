//
//  WolfState.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 28/01/15.
//
//

#ifndef SOP__WOLF_STATE_H
#define SOP__WOLF_STATE_H

enum class eColliderType : int
{
    UNUSED_WOLF = 0,
    WOLF_PLAYER_BEGIN,
    WOLF_PLAYER_END,
    WOLF_BRAZIER_BEGIN,
    WOLF_BRAZIER_END,
    WOLF_SHEEP_BEGIN,
    WOLF_SHEEP_END,
};

class Wolf;

class WolfState
{
public:
    virtual ~WolfState()  {}
    virtual WolfState* handleCollision(Wolf* wolf, eColliderType collider) { return nullptr; }
    virtual void update(Wolf* wolf)  {}
    virtual bool isDangerous()  { return false; }
    
private:
    virtual void setGraphics(Wolf* wolf) {}
};


#endif // SOP__WOLF_STATE_H
