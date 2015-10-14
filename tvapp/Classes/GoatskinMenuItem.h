//
//  GoatskinMenuItem.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 22/01/15.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP__GOATSKIN_MENU_ITEM__
#define __SOP__GOATSKIN_MENU_ITEM__

#include "EditorMenuItem.h"
#include "extensions/cocos-ext.h"
#include "Goatskin.h"

USING_NS_CC;

enum eGoatskinSliderValue
{
    GOATSKIN_RANGE = 0,
    TYPE_GOATSKIN_RANGE,
};

class GoatskinMenuItem : public EditorMenuItem
{
public:
    GoatskinMenuItem();
    virtual ~GoatskinMenuItem();
    
    CREATE_FUNC(GoatskinMenuItem);
    bool init();
    void initLevelVar();
    void setScene(GameLogic* scene)                     { _scene = scene; }
    
private:
    extension::ControlSlider* goatskinRangeActivation();
    void goatskinRangeSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    extension::ControlSlider* _goatskinRangeSliderCtl;
    
    GameLogic* _scene;
};

#endif // __SOP__GOATSKIN_MENU_ITEM__
#endif // EDITOR_MODE
