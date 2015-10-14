//
//  NoteMenuItem.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 13/11/14.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP_NOTE_MENU_ITEM__
#define __SOP_NOTE_MENU_ITEM__

#include "EditorMenuItem.h"
#include "extensions/cocos-ext.h"
#include "Sheep.h"
#include "Player.h"

USING_NS_CC;

enum eNoteSliderValue
{
    NOTE_DISTANCE = 0,
    PARTICLE_SCALE,
    NOTE_DURATION,
    SHEEP_PAN_DISTANCE,
    TYPE_NOTE_DISTANCE,
    TYPE_PARTICLE_SCALE,
    TYPE_NOTE_DURATION,
    TYPE_SHEEP_PAN_DISTANCE,
};

class NoteMenuItem : public EditorMenuItem
{
public:
    NoteMenuItem();
    virtual ~NoteMenuItem();
    
    CREATE_FUNC(NoteMenuItem);
    bool init();
    void initLevelVar();
    void setPlayer(Player* player)             { _player = player; }
    void setScene(GameLogic* scene)            { _scene = scene; }
    
private:
    extension::ControlSlider* noteDistanceActivation();
    extension::ControlSlider* particleScaleActivation();
    extension::ControlSlider* sheepPanDistanceActivation();
    extension::ControlSlider* noteDurationActivation();
    
    void noteDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void particleScaleSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void sheepPanDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    void noteDurationSliderAction(Ref* sender, extension::Control::EventType controlEvent);
    
    extension::ControlSlider* _noteDistanceSliderCtl;
    extension::ControlSlider* _particleScaleSliderCtl;
    extension::ControlSlider* _sheepPanDistanceSliderCtl;
    extension::ControlSlider* _noteDurationSliderCtl;
    
    Player* _player;
    GameLogic* _scene;
};


#endif // __SOP_NOTE_MENU_ITEM__

#endif // EDITOR_MODE