//
//  MyMenu.hpp
//  tvapp
//
//  Created by Iacopo Checcacci on 28/10/15.
//
//

#ifndef MY_MENU_HPP
#define MY_MENU_HPP

#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_TVOS)
    #include "base/CCGameController.h"
#endif

USING_NS_CC;

class MyMenu : public Menu
{
public:
    static MyMenu* create();
    static MyMenu* create(MenuItem* item, ...);
    static MyMenu* createWithItems(MenuItem *firstItem, va_list args);
    static MyMenu* createWithArray(const Vector<MenuItem*>& arrayOfItems);
    bool initWithArray(const Vector<MenuItem*>& arrayOfItems);
    
    bool onTouchBegan(Touch* touch, Event* event) override;
    void onTouchEnded(Touch* touch, Event* event) override;
    
    void onKeyDown(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
};

#endif // MY_MENU_HPP
