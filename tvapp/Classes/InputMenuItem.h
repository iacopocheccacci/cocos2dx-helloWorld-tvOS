//
//  InputMenuItem.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 05/11/14.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP_INPUT_MENU_ITEM__
#define __SOP_INPUT_MENU_ITEM__

#include "EditorMenuItem.h"
#include "extensions/cocos-ext.h"

USING_NS_CC;

enum eInputSliderValue
{
    INPUT_HORIZONTAL_IMPULSE = 0,
    INPUT_VERTICAL_IMPULSE,
    INPUT_HORIZONTAL_LIMIT,
    TYPE_INPUT_HORIZONTAL_IMPULSE,
    TYPE_INPUT_VERTICAL_IMPULSE,
    TYPE_INPUT_HORIZONTAL_LIMIT,
};

class InputMenuItem : public EditorMenuItem
{
public:
    InputMenuItem();
    virtual ~InputMenuItem();
    
    CREATE_FUNC(InputMenuItem);
    bool init();
    void initLevelVar();
    
    void setPlayer(Player* player)      { _player = player; }
    
private:
    extension::ControlSlider* horImpSliderCtl();
    extension::ControlSlider* vertImpSliderCtl();
    extension::ControlSlider* horLimitSliderCtl();
    
    void horImpSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void vertImpSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void horLimitSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    
    extension::ControlSlider* _horImpSliderCtl;
    extension::ControlSlider* _vertImpSliderCtl;
    extension::ControlSlider* _horLimitSliderCtl;
    
    Player* _player;
};


#endif // __SOP_INPUT_MENU_ITEM__

#endif // EDITOR_MODE