//
//  InputMenu.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 13/02/15.
//
//

#include "InputSelection.h"
#include "SettingsManager.h"
#include "MyMenuItemToggle.h"

namespace inputSelectionDefs
{
    static const char* BG_SPRITE_NAME = "UI/optionsControlBg.png";
    static const char* ICON_TRADITIONAL_SPRITE_NAME = "UI/virtualPad_icon.png";
    static const char* ICON_EXPERIMENTAL_SPRITE_NAME = "UI/doubleTap_icon.png";
    static const char* TOGGLE_DOWN_SPRITE_NAME = "UI/optionsControlToggle_down.png";
    static const char* TOGGLE_UP_SPRITE_NAME = "UI/optionsControlToggle_up.png";
    
    static const char* TITLE = "CONTROLS OPTIONS";
    
    static const char* TRADITIONAL_NAME = "VIRTUAL PAD";
    static const char* EXPERIMENTAL_NAME = "TOUCH";
}

bool InputSelection::init()
{
    if (!Node::init())
    {
        return false;
    }
    
    _panelSprite = nullptr;
    _delegate = nullptr;
    
    _toggleMenu = nullptr;
    _traditionalLabel = nullptr;
    _experimentalLabel = nullptr;
    _traditionalSprite = nullptr;
    _experimentalSprite = nullptr;
    
    this->initBackground();
    this->initTitle();
    this->initToggleSection();
    
    this->updatePanel();
    
    return true;
}

void InputSelection::initBackground()
{
    _panelSprite = Sprite::create(inputSelectionDefs::BG_SPRITE_NAME);
    this->addChild(_panelSprite);
}

void InputSelection::initTitle()
{
    Label * label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, inputSelectionDefs::TITLE);
    label->setPosition(Point(_panelSprite->getContentSize().width / 2.0f, 125.0f));
    label->setScale(0.5f);
    label->setColor(appParams::FONT_ORANGE);
    _panelSprite->addChild(label);
}

void InputSelection::initToggleSection()
{
    auto screenSize = Director::getInstance()->getWinSize();
    
    _toggleMenu = MyMenuItemToggle::createWithCallback(CC_CALLBACK_1(InputSelection::inputPressed, this),
                                                           MenuItemImage::create(inputSelectionDefs::TOGGLE_DOWN_SPRITE_NAME,
                                                                                 inputSelectionDefs::TOGGLE_DOWN_SPRITE_NAME),
                                                           MenuItemImage::create(inputSelectionDefs::TOGGLE_UP_SPRITE_NAME,
                                                                                 inputSelectionDefs::TOGGLE_UP_SPRITE_NAME),
                                                           nullptr);
    
    _toggleMenu->setPosition(Point(62.0f, 59.0f));
    
    auto menu = Menu::create(_toggleMenu , NULL);
    menu->setPosition(Point::ZERO);
    _panelSprite->addChild(menu);
//    _panelSprite->addChild(menu, 1,static_cast<int>(eInputSelectionTag::SELECTION_MENU));
    
    // Etichetta Controlli Tradizionali
    _traditionalLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, inputSelectionDefs::TRADITIONAL_NAME);
    _traditionalLabel->setPosition(Point(94.0f, 39.0f));
    _traditionalLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _traditionalLabel->setScale(0.5f);
    _panelSprite->addChild(_traditionalLabel);
    
    // Etichetta Controlli Sperimentali
    _experimentalLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, inputSelectionDefs::EXPERIMENTAL_NAME);
    _experimentalLabel->setPosition(Point(94.0f, 79.0f));
    _experimentalLabel->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _experimentalLabel->setScale(0.5f);
    _panelSprite->addChild(_experimentalLabel);
    
    // Icona Controlli Tradizionali
    _traditionalSprite = Sprite::create(inputSelectionDefs::ICON_TRADITIONAL_SPRITE_NAME);
    _traditionalSprite->setPosition(Point(349.0f, 59.0f));
    _panelSprite->addChild(_traditionalSprite);
    
    // Icona Controlli Sperimentali
    _experimentalSprite = Sprite::create(inputSelectionDefs::ICON_EXPERIMENTAL_SPRITE_NAME);
    _experimentalSprite->setPosition(Point(349.0f, 59.0f));
    _panelSprite->addChild(_experimentalSprite);
}

void InputSelection::inputPressed(cocos2d::Ref *sender)
{
    auto menuItem = dynamic_cast<MenuItemToggle *>(sender);
    eInputType inputType = eInputType::SPLIT_SCREEN;
    
    if (menuItem)
    {
        inputType = this->getInputTypeFromIndex(menuItem->getSelectedIndex());
        this->saveInputType(inputType);
    }
    this->updatePanel();
    
    if (nullptr != _delegate)
    {
        _delegate->handleInputSelection(inputType);
    }
}

eInputType InputSelection::loadInputType()
{
    return SettingsManager::getInstance()->loadUserInput();
}

void InputSelection::saveInputType(eInputType inputType)
{
    SettingsManager::getInstance()->saveUserInput(inputType);
}

void InputSelection::updatePanel()
{
    eInputType inputType = this->loadInputType();
    if (_toggleMenu)
    {
        _toggleMenu->setSelectedIndex(this->getIndexFromInputType(inputType));
        if (eInputType::SPLIT_SCREEN == static_cast<eInputType>(_toggleMenu->getSelectedIndex()))
        {
            _experimentalSprite->setVisible(true);
            _traditionalSprite->setVisible(false);
            _experimentalLabel->setColor(Color3B::WHITE);
            _experimentalLabel->setOpacity(255);
            _traditionalLabel->setColor(Color3B::GRAY);
            _traditionalLabel->setOpacity(100);
        }
        else
        {
            _experimentalSprite->setVisible(false);
            _traditionalSprite->setVisible(true);
            _experimentalLabel->setColor(Color3B::GRAY);
            _experimentalLabel->setOpacity(100);
            _traditionalLabel->setColor(Color3B::WHITE);
            _traditionalLabel->setOpacity(255);
        }
    }
}

eInputType InputSelection::getInputTypeFromIndex(int index)
{
    eInputType inputType = eInputType::SPLIT_SCREEN;
    if (0 == index)
    {
        inputType = eInputType::BUTTONS;
    }
    else
    {
        inputType = eInputType::SPLIT_SCREEN;
    }
    return inputType;
}

int InputSelection::getIndexFromInputType(eInputType inputType)
{
    int index = 0;
    if (eInputType::BUTTONS == inputType)
    {
        index = 0;
    }
    else
    {
        index = 1;
    }
    return index;
}