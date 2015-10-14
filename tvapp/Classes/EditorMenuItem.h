//
//  EditorMenuItem.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifndef __SOP_EDITOR_MENU_ITEM__
#define __SOP_EDITOR_MENU_ITEM__

#include "cocos2d.h"
#include "GameLogic.h"

USING_NS_CC;

class EditorMenuItem : public LayerColor
{
public:
    virtual bool init();
    virtual void initLevelVar() = 0;
    void setTabEnabled(bool enabled)    { _tabEnabled = enabled; }
    bool getTabEnabled()                { return _tabEnabled; }
    
protected:
    bool _tabEnabled;
};

#endif // __SOP_EDITOR_MENU_ITEM__
