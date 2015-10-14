//
//  NavigationPopUp.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 17/04/15.
//
//

#ifndef __SongOfPan__NavigationPopUp__
#define __SongOfPan__NavigationPopUp__

#include "PopUp.h"
#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

enum class ePopUpItem
{
    BUTTON_ITEM_HOME = 0,
    BUTTON_ITEM_RESTART,
    BUTTON_ITEM_IAP,
    BUTTON_ITEM_OPTIONS,
    BUTTON_ITEM_RESUME
};

class NavigationPopUp : public PopUp
{
public:
    virtual ~NavigationPopUp();
    
    static NavigationPopUp * createWithLayerColor(Color4B color, float width, float height);
    static NavigationPopUp * createWithSpriteName(std::string spriteName);
    bool initWithLayerColor(Color4B color, float width, float height);
    bool initWithSpriteName(std::string spriteName);
    
    void setAlignPadding(float padding);
    
    void addPredefinedItemToMenu(ePopUpItem predefinedItem);
    void addCustomItemToMenu(MenuItem * item);
    
protected:
    
    Node * _middleGround;
    float _presentationTime;
    
    virtual void restartPressed(Ref* sender);
    virtual void homePressed(Ref* sender);
    virtual void iapPressed(Ref* sender);
    virtual void optionsPressed(Ref* sender);
    
    virtual void resumePressed(Ref* sender);
    virtual void backPressed(Ref* sender);
    
private:
    
    ui::PageView * _pageView;
    Menu * _menu;
    Menu * _resumeButton;
    Menu * _backButton;
    
    float _alignPadding;
    
    void initCommons();
    void initDefaultMenu();
    void addItemToMenu(MenuItem * item);
    void showIn();
    void showOut(Node * callbackNode);
    void setMenuActiveRecursive(Node * root, bool enabled);
    
    Node * setupMenuPanel(Size panelSize);
    Node * setupOptionsPanel(Size panelSize);
    
    MenuItem* setupRestartButton();
    MenuItem* setupHomeButton();
    MenuItem* setupIAPButton();
    MenuItem* setupOptionsButton();
    void setupResumeButton();
    void setupBackButton();
    
    void disableInteractionForNotSelectedPages();
    
    void onEnter();
    void onPageViewEvent(Ref * sender, ui::PageView::EventType eventType);
};

#endif /* defined(__SongOfPan__NavigationPopUp__) */
