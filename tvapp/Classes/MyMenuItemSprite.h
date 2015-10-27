//
//  MyMenuItemSprite.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 16/02/15.
//
//

#ifndef __SOP__MY_MENU_ITEM_SRITE__
#define __SOP__MY_MENU_ITEM_SRITE__

#include "cocos2d.h"

USING_NS_CC;

class MyMenuItemSprite : public MenuItemSprite
{
public:
    static MyMenuItemSprite * create(Node* normalSprite, Node* selectedSprite, Node* disabledSprite, const ccMenuCallback& callback);
    virtual bool init() override;
    
    virtual void selected() override;
    virtual void unselected() override;
};

#endif // __SOP__MY_MENU_ITEM_SRITE__
