//
//  NavigationMenu.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/11/14.
//
//

#ifndef __SOP_NAVIGATION_MENU__
#define __SOP_NAVIGATION_MENU__

#include "cocos2d.h"

USING_NS_CC;

namespace navigationMenuDefs
{
    static const char* BACKGROUND_SPRITE_PATH = "UI/hudBg_dx.png";
    static const char* HUD_BUTTON_ACTIVE_SPRITE_PATH = "UI/hudBtn_active.png";
    static const char* RESTART_SPRITE_PATH = "UI/restartBtn.png";
    static const char* ZOOM_SPRITE_PATH = "UI/zoomBtn.png";
    static const char* PAUSE_SPRITE_PATH = "UI/pauseBtn.png";
    static const char* STOP_NOTE_SPRITE_PATH = "UI/pausaNotaBtn.png";
    static const char* STOP_INVERSE_NOTE_SPRITE_PATH = "UI/pausaNotaRossaBtn.png";
    
    static const float MENU_ITEM_PADDING = 1.0f;
    static const Point MENU_NORMALIZED_POSITION = {0.17948718f, 0.61111111f};
    static const float OFFSET_BACKGROUND_Y = 20.0f;
}

enum class eLevelType;
enum class eInputType;
enum class eNoteEffect;

class NavigationMenu : public Layer
{
public:
    virtual ~NavigationMenu();
    static NavigationMenu * create(eLevelType levelType, Node * player);
    bool init(eLevelType levelType, Node * player);
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
    void showStopNoteButton();
    void hideStopNoteButton();
    
    void updateStopNoteButtonInputType(eInputType inputType);
    void updateStopNoteButtonEffect(eNoteEffect noteEffect);
    
    void enableMenu();
    void disableMenu();
    
private:
    void initMenu(Vector<MenuItem *> items);
    
    MenuItemSprite* setupRestartButton();
    MenuItemSprite* setupZoomButton();
    MenuItemSprite* setupPauseButton();
    MenuItemSprite* setupStopButton();
    
    void restartPressed(Ref* sender);
    void zoomPressed(Ref* sender);
    void pausePressed(Ref* sender);
    void stopPressed(Ref* sender);
    
    void update(float dt);
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
    
    MenuItemSprite * _menuItemStopPlayer;
    MenuItemSprite * _menuItemStopFixed;
    Node * _player;
    
    Menu * _navigationMenu;
};

#endif // __SOP_NAVIGATION_MENU__
