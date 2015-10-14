//
//  MyMenuItemSprite.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/02/15.
//
//

#include "MyMenuItemSprite.h"

MyMenuItemSprite* MyMenuItemSprite::create(Node *normalSprite, Node *selectedSprite, Node *disabledSprite, const ccMenuCallback &callback)
{
    MyMenuItemSprite *pRet = new MyMenuItemSprite();
    if (pRet && pRet->MenuItemSprite::initWithNormalSprite(normalSprite, selectedSprite, disabledSprite, callback))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    }
}

bool MyMenuItemSprite::init()
{
    if (!MenuItemSprite::init())
    {
        return false;
    }
    
    return true;
}

void MyMenuItemSprite::selected()
{
    MenuItemSprite::selected();
    
    this->activate();
}

void MyMenuItemSprite::unselected()
{
    MenuItemSprite::unselected();
    
    this->activate();
}