//
//  SheepMenuItem.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 05/11/14.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP_SHEEP_MENU_ITEM__
#define __SOP_SHEEP_MENU_ITEM__

#include "EditorMenuItem.h"
#include "extensions/cocos-ext.h"
#include "Sheep.h"

USING_NS_CC;

enum eSheepSliderValue
{
    SHEEP_HORIZONTAL_IMPULSE = 0,
    SHEEP_VERTICAL_IMPULSE,
    SHEEP_HORIZONTAL_LIMIT,
    SHEEP_DISTANCE,
    TYPE_SHEEP_HORIZONTAL_IMPULSE,
    TYPE_SHEEP_VERTICAL_IMPULSE,
    TYPE_SHEEP_HORIZONTAL_LIMIT,
    TYPE_SHEEP_DISTANCE,
};

class SheepMenuItem : public EditorMenuItem
{
public:
    SheepMenuItem();
    virtual ~SheepMenuItem();
    
    CREATE_FUNC(SheepMenuItem);
    bool init();
    void initLevelVar();
    void setScene(GameLogic* scene)            { _scene = scene; }
    
private:
    extension::ControlSlider* horImpSliderCtl();
    extension::ControlSlider* vertImpSliderCtl();
    extension::ControlSlider* horLimitSliderCtl();
    extension::ControlSlider* sheepDistanceSliderCtl();
    
    void horImpSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void vertImpSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void horLimitSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void sheepDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    
    extension::ControlSlider* _horImpSliderCtl;
    extension::ControlSlider* _vertImpSliderCtl;
    extension::ControlSlider* _horLimitSliderCtl;
    extension::ControlSlider* _sheepDistanceSliderCtl;
    
    GameLogic* _scene;
};

#endif // __SOP_SHEEP_MENU_ITEM__

#endif // EDITOR_MODE