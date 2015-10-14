//
//  MyMenuItemToggle.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 02/07/15.
//
//

#include "MyMenuItemToggle.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
MyMenuItemToggle * MyMenuItemToggle::createWithCallbackVA(const ccMenuCallback &callback, MenuItem* item, ...)
{
    va_list args;
    va_start(args, item);
    MyMenuItemToggle *ret = new (std::nothrow) MyMenuItemToggle();
    ret->initWithCallback(callback, item, args);
    ret->autorelease();
    va_end(args);
    return ret;
}
#else
MyMenuItemToggle * MyMenuItemToggle::createWithCallback(const ccMenuCallback& callback, MenuItem* item, ...)
{
    va_list args;
    va_start(args, item);
    MyMenuItemToggle *ret = new (std::nothrow) MyMenuItemToggle();
    ret->initWithCallback(callback, item, args);
    ret->autorelease();
    va_end(args);
    return ret;
}
#endif

bool MyMenuItemToggle::init()
{
    if (!MenuItemToggle::init())
    {
        return false;
    }
    
    return true;
}

void MyMenuItemToggle::unselected()
{
    MenuItemToggle::unselected();
    
    this->activate();
}