//
//  MyMenuItemToggle.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 02/07/15.
//
//

#ifndef __SOP__MY_MENU_ITEM_TIGGLE__
#define __SOP__MY_MENU_ITEM_TIGGLE__

#include "cocos2d.h"

USING_NS_CC;

class MyMenuItemToggle : public MenuItemToggle
{
public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    static MyMenuItemToggle * createWithCallback(const ccMenuCallback& callback, MenuItem* m1, MenuItem* m2, MenuItem* m3, std::nullptr_t listEnd){ return createWithCallbackVA(callback, m1, m2, m3, NULL); }
    
    static MyMenuItemToggle* createWithCallbackVA(const ccMenuCallback& callback, M item, ...);
#else
    static MyMenuItemToggle* createWithCallback(const ccMenuCallback& callback, MenuItem* item, ...) ;
#endif
    
    virtual bool init();
    virtual void unselected() override;
};

#endif // __SOP__MY_MENU_ITEM_TIGGLE__
