//
//  Gear.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 27/02/15.
//
//

#include "Gear.h"

bool Gear::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHSprite::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _isOpen = false;
    }
    
    return status;
}

void Gear::update(eEventType event, Ref * sender, void * options)
{
    switch (event)
    {
        case eEventType::SWITCH_OFF:
            if (_isOpen)
            {
                _isOpen = false;
                this->runAction(RotateBy::create(0.5f, 180));
                CCLOG("ruota chiudi");
            }
            break;
            
        case eEventType::SWITCH_ON:
            if (!_isOpen)
            {
                _isOpen = true;
                this->runAction(RotateBy::create(0.5f, -180));
                CCLOG("ruota apri");
            }
            break;
            
        default:
            break;
    }

}