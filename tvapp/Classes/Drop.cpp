//
//  Drop.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 22/06/15.
//
//

#include "Drop.h"
#include "AnimationComponent.h"

bool Drop::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHSprite::initWithDictionary(dict, prnt);
    
    if(status)
    {
        this->loadAnimation();
    }
    
    return status;
}

void Drop::loadAnimation()
{
    _animation = AnimationComponent::create(this, AnimationComponent::DROP);
    
    this->addChild(_animation);
    
    _animation->startAnimationFromRandomFrame(4, true);
}