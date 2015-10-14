//
//  FaithJumpBrick.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 08/09/15.
//
//

#ifndef __SOP__FAITH_JUMP_BRICK__
#define __SOP__FAITH_JUMP_BRICK__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"

namespace faithJumpBrickDefs
{
    static const char* CLASS_NAME = "FaithJumpBrick";
    static const char* SPRITE_NAME = "FaithJumpSprite";
    static const char* ANIMATION_NAME = "start";
}

USING_NS_CC;

class FaithJumpBrick : public LHNode
{
public:
    NODEWITHDICT_FUNC(FaithJumpBrick);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void buildPlatform();
    bool isAlreadyActive()          { return _isActive; }
    
private:
    bool _isActive;
    LHSprite* _sprite;
};


#endif // __SOP__FAITH_JUMP_BRICK__
