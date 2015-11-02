//
//  SettingsPanel.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 05/03/15.
//
//

#include "SettingsPopUp.h"

#include "SoundManager.h"
#include "InputSelection.h"
#include "SettingsManager.h"
#include "VisibleRect.h"
#include "Definitions.h"
#include "MyMenu.hpp"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_TVOS)
    #define APPLETV
#endif

namespace SettingsPopUpDefs
{
    static const float PANEL_WIDTH = 550.0f;
    static const float PANEL_HEIGHT = 300.0f;
    static const float PANEL_NO_INPUT_WIDTH = 450.0f;
    static const float PANEL_NO_INPUT_HEIGHT =300.0f;
    static const int PANEL_Z_ORDER = 10000;
    static const float PANEL_POP_UP_ANIM_TIME = 0.2f;
    
    static const char* BTN_CLOSE_NORMAL_SPRITE_NAME = "UI/closeBtn.png";
    static const char* BTN_CLOSE_PRESSED_SPRITE_NAME = "UI/closeBtn_tap.png";
    static const char* AUDIO_CHECK_SPRITE_NAME = "UI/optionsAudio_check.png";
}

SettingsPopUp * SettingsPopUp::create(const bool inGame)
{
    return SettingsPopUp::create(Color4B(0,0,0,125), inGame);
}

SettingsPopUp * SettingsPopUp::create(const Color4B& color, const bool inGame)
{
    SettingsPopUp * layer = new (std::nothrow) SettingsPopUp();
    if(layer && layer->initWithColor(color, inGame))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool SettingsPopUp::initWithColor(const Color4B& color, const bool inGame)
{
    if ( !LayerColor::initWithColor(color) )
    {
        return false;
    }
    
    _willAnimate = false;
    _panelInGame = inGame;
    
    eInputType inputType = SettingsManager::getInstance()->loadUserInput();
    
    this->initBlockTouch();
    this->initPanel(inputType);
    this->initContents(inputType);
    this->updatePanel();
    
    return true;
}

void SettingsPopUp::initBlockTouch()
{
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch* touch, Event* event){
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void SettingsPopUp::initPanel(eInputType inputType)
{
    Size panelSize;
    if (eInputType::KEYBOARD != inputType)
    {
        panelSize = Size(SettingsPopUpDefs::PANEL_WIDTH, SettingsPopUpDefs::PANEL_HEIGHT);
    }
    else
    {
        panelSize = Size(SettingsPopUpDefs::PANEL_NO_INPUT_WIDTH, SettingsPopUpDefs::PANEL_NO_INPUT_HEIGHT);
    }
    
    _panelSprite = Sprite::create(appParams::BLANK_PIXEL_FILE_NAME, Rect(0, 0, panelSize.width, panelSize.height));
    _panelSprite->setColor(Color3B(51,51,51));
    _panelSprite->setPosition(VisibleRect::center());
    this->addChild(_panelSprite);
}

void SettingsPopUp::initContents(eInputType inputType)
{
    CCASSERT(_panelSprite, "La sprite del pannello non esiste!");
    
    this->initAudioMusicOnOff(inputType);
    this->initAudioFXOnOff(inputType);
    
    this->initAudioMusicVolume(inputType);
    this->initAudioFXVolume(inputType);
    
    if (eInputType::KEYBOARD != inputType)
    {
        this->initInputSelection();
    }
    
    this->initCloseButton();
}

void SettingsPopUp::initAudioMusicOnOff(eInputType inputType)
{
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    if (eInputType::KEYBOARD != inputType)
    {
        offsetX = _panelSprite->getContentSize().width * 1.0f / 6.0f;
        offsetY = _panelSprite->getContentSize().height * 4.0f / 5.0f;
    }
    else
    {
        offsetX = _panelSprite->getContentSize().width * 1.0f / 6.0f;
        offsetY = _panelSprite->getContentSize().height * 2.0f / 3.0f;
    }
    
    // Etichetta Audio Music
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "Music");
    label->setPosition(Point(offsetX + 45.0f, offsetY));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    _panelSprite->addChild(label);
    
    // Pulsante Audio On/Off
    std::string itemImageNormalName = SettingsPopUpDefs::AUDIO_CHECK_SPRITE_NAME;
    std::string itemImageActiveName = SettingsPopUpDefs::AUDIO_CHECK_SPRITE_NAME;
    
    auto emptyCheckbox = Sprite::create(itemImageNormalName);
    emptyCheckbox->setPosition(offsetX, offsetY);
    _panelSprite->addChild(emptyCheckbox);
    
    auto itemImageNormal = MenuItemImage::create(itemImageNormalName, itemImageNormalName);
    auto itemImageActive = MenuItemImage::create(itemImageActiveName, itemImageActiveName);
    _audioMusicOnOffToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingsPopUp::handleAudioMusicOnOff, this),
                                                                itemImageNormal,
                                                                itemImageActive,
                                                                nullptr);
    
    // Menu Audio On/Off
#ifdef APPLETV
    MyMenu * menu = MyMenu::create(_audioMusicOnOffToggle, NULL);
#else
    Menu * menu = Menu::create(_audioMusicOnOffToggle, NULL);
#endif
    
    menu->setPosition(Point(offsetX, offsetY));
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _panelSprite->addChild(menu);
}

void SettingsPopUp::initAudioFXOnOff(eInputType inputType)
{
    float offsetX = 0.0f;
    float offsetY = 0.0f;
    if (eInputType::KEYBOARD != inputType)
    {
        offsetX = _panelSprite->getContentSize().width * 1.0f / 6.0f;
        offsetY = _panelSprite->getContentSize().height * 3.0f / 5.0f;
    }
    else
    {
        offsetX = _panelSprite->getContentSize().width * 1.0f / 6.0f;
        offsetY = _panelSprite->getContentSize().height * 1.0f / 3.0f;
    }
    
    // Etichetta Audio FX
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "Sound FX");
    label->setPosition(Point(offsetX + 45.0f, offsetY));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    _panelSprite->addChild(label);
    
    // Pulsante Audio FX On/Off
    std::string itemImageNormalName = SettingsPopUpDefs::AUDIO_CHECK_SPRITE_NAME;
    std::string itemImageActiveName = SettingsPopUpDefs::AUDIO_CHECK_SPRITE_NAME;
    
    auto emptyCheckbox = Sprite::create(itemImageNormalName);
    emptyCheckbox->setPosition(offsetX, offsetY);
    _panelSprite->addChild(emptyCheckbox);
    
    auto itemImageNormal = MenuItemImage::create(itemImageNormalName, itemImageNormalName);
    auto itemImageActive = MenuItemImage::create(itemImageActiveName, itemImageActiveName);
    _audioFXOnOffToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(SettingsPopUp::handleAudioFXOnOff, this),
                                                             itemImageNormal,
                                                             itemImageActive,
                                                             nullptr);
    
    // Menu Audio FX On/Off
    Menu * menu = Menu::create(_audioFXOnOffToggle, NULL);
    menu->setPosition(Point(offsetX, offsetY));
    menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    _panelSprite->addChild(menu);
}

void SettingsPopUp::initAudioMusicVolume(eInputType inputType)
{
    // Eventualmente fare slider per il volume della musica
}

void SettingsPopUp::initAudioFXVolume(eInputType inputType)
{
    // Eventualmente fare slider per il volume degli effetti sonori
}

void SettingsPopUp::initCloseButton()
{
    // Pulsante Close
    _itemImageClose = MenuItemImage::create(SettingsPopUpDefs::BTN_CLOSE_NORMAL_SPRITE_NAME,
                                            SettingsPopUpDefs::BTN_CLOSE_PRESSED_SPRITE_NAME,
                                            CC_CALLBACK_1(SettingsPopUp::handleClose, this));
    
    // Menu Pulsante Close
#ifdef APPLETV
    MyMenu * menu = MyMenu::create(_itemImageClose, NULL);
#else
    Menu * menu = Menu::create(_itemImageClose, NULL);
#endif

    Point position = Point(_panelSprite->getContentSize().width - 20.0f, _panelSprite->getContentSize().height - 20.0f);
    menu->setPosition(position);
    _panelSprite->addChild(menu);
}

void SettingsPopUp::initInputSelection()
{
    _inputSelection = InputSelection::create();
    _inputSelection->setDelegate(this);
    _panelSprite->addChild(_inputSelection);
    _inputSelection->setNormalizedPosition(Point(0.5f, 0.25f));
}

void SettingsPopUp::initDelayedInteraction()
{
    this->enableInteraction(false);
    DelayTime * delay = DelayTime::create(SettingsPopUpDefs::PANEL_POP_UP_ANIM_TIME);
    CallFunc * activationCallback = CallFunc::create([&]()
    {
        this->enableInteraction(true);
    });
    
    this->runAction(Sequence::create(delay, activationCallback, NULL));
}

void SettingsPopUp::presentPopUp(Node *parent, bool animation)
{
    _willAnimate = animation;
    if (animation)
    {
        this->initDelayedInteraction();
        
        auto bounceAction = EaseBackOut::create(ScaleTo::create(SettingsPopUpDefs::PANEL_POP_UP_ANIM_TIME, 1.0f));
        
        _panelSprite->setScale(0);
        parent->addChild(this, SettingsPopUpDefs::PANEL_Z_ORDER);
        _panelSprite->runAction(bounceAction);
    }
    else
    {
        parent->addChild(this, SettingsPopUpDefs::PANEL_Z_ORDER);
    }
}

void SettingsPopUp::dismissPopUp()
{
    if (_willAnimate)
    {
        CallFunc * endCallback = CallFunc::create([&]()
        {
            this->removeFromParentAndCleanup(true);
        });
        
        this->enableInteraction(false);
        auto bounceAction = EaseBackIn::create(ScaleTo::create(SettingsPopUpDefs::PANEL_POP_UP_ANIM_TIME, 0));
        _panelSprite->runAction(Sequence::create(bounceAction, endCallback, NULL));
    }
    else
    {
        this->enableInteraction(false);
        this->removeFromParentAndCleanup(true);
    }
}

void SettingsPopUp::enableInteraction(bool enable)
{
    _audioMusicOnOffToggle->setEnabled(enable);
    _audioFXOnOffToggle->setEnabled(enable);
    _itemImageClose->setEnabled(enable);
}

#pragma mark - Callbacks

void SettingsPopUp::handleClose(Ref * pSender)
{
    this->dismissPopUp();
}

void SettingsPopUp::handleAudioMusicOnOff(Ref * pSender)
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveMusic();
    bool newStatus = !currentStatus;
    
    SettingsManager::getInstance()->saveUserActiveMusic(newStatus);
    
    // Il Director non viene più messo in pause, quindi posso fare sempre fade
    bool fadeEffect = true;//(_panelInGame) ? false : true;
    float newVolume = (newStatus) ? SettingsManager::getInstance()->loadUserVolumeMusic() : 0.0f;
    SoundManager::getInstance()->setUserVolumeMusic(newVolume, fadeEffect);
    
    this->updatePanel();
}

void SettingsPopUp::handleAudioFXOnOff(Ref * pSender)
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveFX();
    bool newStatus = !currentStatus;
    
    SettingsManager::getInstance()->saveUserActiveFX(newStatus);
    
    // Il Director non viene più messo in pause, quindi posso fare sempre fade
    bool fadeEffect = true;//(_panelInGame) ? false : true;
    float newVolume = (newStatus) ? SettingsManager::getInstance()->loadUserVolumeFX() : 0.0f;
    SoundManager::getInstance()->setUserVolumeFX(newVolume, fadeEffect);
    
    this->updatePanel();
}

#pragma mark - Aggiornamento del pannello

void SettingsPopUp::updatePanel()
{
    this->updateAudioMusicOnOff();
    this->updateAudioFXOnOff();
}

void SettingsPopUp::updateAudioMusicOnOff()
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveMusic();
    if (currentStatus)
    {
        _audioMusicOnOffToggle->setSelectedIndex(1);
    }
    else
    {
        _audioMusicOnOffToggle->setSelectedIndex(0);
    }
}

void SettingsPopUp::updateAudioFXOnOff()
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveFX();
    if (currentStatus)
    {
        _audioFXOnOffToggle->setSelectedIndex(1);
    }
    else
    {
        _audioFXOnOffToggle->setSelectedIndex(0);
    }
}

#pragma mark - InputSelectionDelegate

void SettingsPopUp::handleInputSelection(eInputType inputType)
{
    // Se è necessario ripropagare alla GameLogic il cambio di inputType in tempo reale, usare questo metodo
}
