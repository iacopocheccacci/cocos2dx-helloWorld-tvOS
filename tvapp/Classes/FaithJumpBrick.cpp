//
//  FaithJumpBrick.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 08/09/15.
//
//

#include "FaithJumpBrick.h"
#include "Utility.h"

bool FaithJumpBrick::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _sprite = dynamic_cast<LHSprite*>(this->getChildNodeWithName(faithJumpBrickDefs::SPRITE_NAME));
        _isActive = false;
        
        Utility::applyTopGhostVertex(this);
    }
    
    return status;
}

void FaithJumpBrick::buildPlatform()
{
    if (_sprite)
    {
        auto newAnimation = _sprite->getAnimationWithName(faithJumpBrickDefs::ANIMATION_NAME);
        if (newAnimation)
        {
            newAnimation->setActive(true);
            newAnimation->restart();
            _isActive = true;
        }
    }
}