//
//  ConfirmInputPopUp.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 22/04/15.
//
//

#include "ConfirmInputPopUp.h"
#include "SettingsManager.h"

namespace ConfirmInputPopUpDefs
{
    static const char* MIDDLE_GROUND_SPRITE_NAME = "UI/popupBg.png";
    static const char* CONFIRM_BTN_NORMAL_SPRITE_NAME = "UI/okGreen.png";
    static const char* CONFIRM_BTN_PRESSED_SPRITE_NAME = "UI/okGreen_tap.png";
    static const char* CONTROL_BTN_OFF_SPRITE_NAME = "UI/controlBtn_normal.png";
    static const char* CONTROL_BTN_ON_SPRITE_NAME = "UI/controlBtn_selected.png";
    static const char* CONTROL_ICON_EXPERIMENTAL_SPRITE_NAME = "UI/doubleTap_icon.png";
    static const char* CONTROL_ICON_TRADITIONAL_SPRITE_NAME = "UI/virtualPad_icon.png";
    
    static const char* MENU_TOGGLE_EXPERIMENTAL = "menuToggleExperimental";
    static const char* MENU_TOGGLE_TRADITIONAL = "menuToggleTraditional";
}

bool ConfirmInputPopUp::init()
{
    bool initDone = false;
    
    if (NavigationPopUp::initWithSpriteName(ConfirmInputPopUpDefs::MIDDLE_GROUND_SPRITE_NAME))
    {
        _controlsMenu = nullptr;
        
        this->initLabels();
        this->initToggle();
        this->initConfirmButton();
        
        initDone = true;
    }
    
    return initDone;
}

void ConfirmInputPopUp::initLabels()
{
    std::string text = "";
    Label * label = nullptr;
    
    // MainText
    text = "FEEL MIGHTY WITH THIS CONTROLS?";
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setScale(0.43f);
    label->setColor(appParams::FONT_ORANGE);
    label->setPosition(_middleGround->getContentSize().width / 2.0f, 470.0f);
    _middleGround->addChild(label);
    
    // Detail
    text = "CHOOSE THE CONTROLS WHICH BEST SUITS YOU";
    label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setScale(0.47f);
    label->setPosition(_middleGround->getContentSize().width / 2.0f, 440.0f);
    _middleGround->addChild(label);
    
    // Footer
    text = "You can change your choice via the options menu";
    label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setScale(0.5f);
    label->setPosition(_middleGround->getContentSize().width / 2.0f, 97.0f);
    _middleGround->addChild(label);
}

void ConfirmInputPopUp::initToggle()
{
    _controlsMenu = Menu::create(this->setupButtonExperimental(), this->setupButtonTraditional(), NULL);
    _controlsMenu->setPosition(Point::ZERO);
    _middleGround->addChild(_controlsMenu);
}

void ConfirmInputPopUp::initConfirmButton()
{
    Sprite * itemSpriteNormal = Sprite::create(ConfirmInputPopUpDefs::CONFIRM_BTN_NORMAL_SPRITE_NAME);
    Sprite * itemSpritePressed = Sprite::create(ConfirmInputPopUpDefs::CONFIRM_BTN_PRESSED_SPRITE_NAME);
    
    MenuItem * item = MenuItemSprite::create(itemSpriteNormal, itemSpritePressed, CC_CALLBACK_1(ConfirmInputPopUp::confirmPressed, this));
    
    Menu * menu = Menu::createWithItem(item);
    menu->setPosition(_middleGround->getContentSize().width / 2.0f, 43.0f);
    _middleGround->addChild(menu);
}

MenuItem * ConfirmInputPopUp::setupButtonExperimental()
{
    MenuItemToggle * item = nullptr;
    MenuItem * itemOff = nullptr;
    MenuItem * itemOn = nullptr;
    
    Sprite * background = nullptr;
    Label * label = nullptr;
    Sprite * icon = nullptr;
    
    background = Sprite::create(ConfirmInputPopUpDefs::CONTROL_BTN_ON_SPRITE_NAME);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "TOUCH", TextHAlignment::CENTER);
    label->setScale(0.5f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Point(81.0f, background->getContentSize().height / 2.0f));
    background->addChild(label);
    
    icon = Sprite::create(ConfirmInputPopUpDefs::CONTROL_ICON_EXPERIMENTAL_SPRITE_NAME);
    icon->setPosition(Point(353.0f, background->getContentSize().height / 2.0f));
    background->addChild(icon);
    
    itemOn = MenuItemSprite::create(background, background);
    
    background = Sprite::create(ConfirmInputPopUpDefs::CONTROL_BTN_OFF_SPRITE_NAME);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "TOUCH", TextHAlignment::CENTER);
    label->setScale(0.5f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Point(81.0f, background->getContentSize().height / 2.0f));
    background->addChild(label);
    
    icon = Sprite::create(ConfirmInputPopUpDefs::CONTROL_ICON_EXPERIMENTAL_SPRITE_NAME);
    icon->setPosition(Point(353.0f, background->getContentSize().height / 2.0f));
    background->addChild(icon);
    
    itemOff = MenuItemSprite::create(background, background);
    
    item = MenuItemToggle::createWithCallback(CC_CALLBACK_1(ConfirmInputPopUp::experimentalPressed, this), itemOn, itemOff, NULL);
    item->setPosition(Point(_middleGround->getContentSize().width / 2.0f, 332.0f));
    item->setName(ConfirmInputPopUpDefs::MENU_TOGGLE_EXPERIMENTAL);
    item->setSelectedIndex(0);
    return item;
}

MenuItem *  ConfirmInputPopUp::setupButtonTraditional()
{
    MenuItemToggle * item = nullptr;
    MenuItem * itemOff = nullptr;
    MenuItem * itemOn = nullptr;
    
    Sprite * background = nullptr;
    Label * label = nullptr;
    Sprite * icon = nullptr;
    
    background = Sprite::create(ConfirmInputPopUpDefs::CONTROL_BTN_ON_SPRITE_NAME);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "VIRTUAL PAD", TextHAlignment::CENTER);
    label->setScale(0.5f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Point(81.0f, background->getContentSize().height / 2.0f));
    background->addChild(label);
    
    icon = Sprite::create(ConfirmInputPopUpDefs::CONTROL_ICON_TRADITIONAL_SPRITE_NAME);
    icon->setPosition(Point(353.0f, background->getContentSize().height / 2.0f));
    background->addChild(icon);
    
    itemOn = MenuItemSprite::create(background, background);
    
    background = Sprite::create(ConfirmInputPopUpDefs::CONTROL_BTN_OFF_SPRITE_NAME);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "VIRTUAL PAD", TextHAlignment::CENTER);
    label->setScale(0.5f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setPosition(Point(81.0f, background->getContentSize().height / 2.0f));
    background->addChild(label);
    
    icon = Sprite::create(ConfirmInputPopUpDefs::CONTROL_ICON_TRADITIONAL_SPRITE_NAME);
    icon->setPosition(Point(353.0f, background->getContentSize().height / 2.0f));
    background->addChild(icon);
    
    itemOff = MenuItemSprite::create(background, background);
    
    item = MenuItemToggle::createWithCallback(CC_CALLBACK_1(ConfirmInputPopUp::traditionalPressed, this), itemOn, itemOff, NULL);
    item->setPosition(Point(_middleGround->getContentSize().width / 2.0f, 190.0f));
    item->setName(ConfirmInputPopUpDefs::MENU_TOGGLE_TRADITIONAL);
    item->setSelectedIndex(1);
    return item;
}

void ConfirmInputPopUp::experimentalPressed(Ref * sender)
{
    if (_controlsMenu)
    {
        MenuItemToggle * item = nullptr;
        item = _controlsMenu->getChildByName<MenuItemToggle*>(ConfirmInputPopUpDefs::MENU_TOGGLE_EXPERIMENTAL);
        if (item)
        {
            item->setSelectedIndex(0);
        }
        item = _controlsMenu->getChildByName<MenuItemToggle*>(ConfirmInputPopUpDefs::MENU_TOGGLE_TRADITIONAL);
        if (item)
        {
            item->setSelectedIndex(1);
        }
    }
    SettingsManager::getInstance()->saveUserInput(eInputType::SPLIT_SCREEN);
}

void ConfirmInputPopUp::traditionalPressed(Ref * sender)
{
    if (_controlsMenu)
    {
        MenuItemToggle * item = nullptr;
        item = _controlsMenu->getChildByName<MenuItemToggle*>(ConfirmInputPopUpDefs::MENU_TOGGLE_TRADITIONAL);
        if (item)
        {
            item->setSelectedIndex(0);
        }
        item = _controlsMenu->getChildByName<MenuItemToggle*>(ConfirmInputPopUpDefs::MENU_TOGGLE_EXPERIMENTAL);
        if (item)
        {
            item->setSelectedIndex(1);
        }
    }
    SettingsManager::getInstance()->saveUserInput(eInputType::BUTTONS);
}

void ConfirmInputPopUp::confirmPressed(Ref * sender)
{
    NavigationPopUp::resumePressed(sender);
}
