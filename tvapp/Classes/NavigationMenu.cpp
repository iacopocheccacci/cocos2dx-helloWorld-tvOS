//
//  NavigationMenu.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 19/11/14.
//
//

#include "NavigationMenu.h"
#include "GameLogic.h"
#include "NoteProgressBar.h"
#include "PausePopUp.h"
#include "VisibleRect.h"
#include "Definitions.h"
#include "Player.h"
#include "Utility.h"

#ifdef EDITOR_MODE
#include "EditorTableView.h"
#endif

enum class eNavigationMenuElements {
    PAUSE,
    RESTART,
    ZOOM
};

NavigationMenu::~NavigationMenu()
{
    removeAllChildrenWithCleanup(true);
}

NavigationMenu * NavigationMenu::create(eLevelType levelType, Node * player)
{
    NavigationMenu * layer = new (std::nothrow) NavigationMenu();
    if(layer && layer->init(levelType, player))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool NavigationMenu::init(eLevelType levelType, Node * player)
{
    bool initDone = false;
    
    if (Layer::init())
    {
        _navigationMenu = nullptr;
        
        Vector<MenuItem *> items;
        switch(levelType)
        {
            case eLevelType::NORMAL:
                items.pushBack(setupZoomButton());
                items.pushBack(setupRestartButton());
                items.pushBack(setupPauseButton());
                break;
            case eLevelType::MAIN_MENU:
            case eLevelType::LEVEL_SELECTION:
                items.pushBack(setupPauseButton());
                break;
            default:
                break;
        }
        this->initMenu(items);
        
        // Pulsante di stop fisso dell'HUD
        _menuItemStopFixed = this->setupStopButton();
        if (_menuItemStopFixed)
        {
            _menuItemStopFixed->setPosition(VisibleRect::bottom() + Point( 190, -100));
            _menuItemStopFixed->setVisible(false);
            _menuItemStopFixed->setEnabled(false);
            Menu * menuFixed = Menu::create(_menuItemStopFixed, NULL);
            menuFixed->setPosition(Point::ZERO);
            this->addChild(menuFixed, 1);
        }
        
        _menuItemStopPlayer = nullptr;
        _player = nullptr;
        
        // Pulsante di stop che segue il player come figlio del player (in scala con esso)
        if (player)
        {
            _menuItemStopPlayer = this->setupStopButton();
            if (_menuItemStopPlayer)
            {
                auto menuStopButtonPlayer = Menu::create(_menuItemStopPlayer, NULL);
                Point playerCenter = Point(player->getContentSize().width/2.0f, player->getContentSize().height - player->getContentSize().width/2.0f);
                Point offset = playerCenter + Point(0.0f, 200.0f);
                float rotation = CC_DEGREES_TO_RADIANS(45);
                Point position = offset.rotateByAngle(playerCenter, rotation);
                menuStopButtonPlayer->setPosition(position);
                _menuItemStopPlayer->setVisible(false);
                _menuItemStopPlayer->setEnabled(false);
                _menuItemStopPlayer->setOpacity(0);
                player->addChild(menuStopButtonPlayer, 10);
            }
        }
        
        Utility::setCascadeOpacityRecursive(this, true);

        initDone = true;
    }
    
    return initDone;
}

void NavigationMenu::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void NavigationMenu::initMenu(Vector<MenuItem *> items)
{
    float itemsOffsetX = 0.0f;
    for (MenuItem * item : items)
    {
        item->setPositionX(itemsOffsetX + item->getContentSize().width / 2.0f);
        itemsOffsetX += item->getContentSize().width + navigationMenuDefs::MENU_ITEM_PADDING;
    }
    
    _navigationMenu = Menu::createWithArray(items);
    Sprite * background = Sprite::create(navigationMenuDefs::BACKGROUND_SPRITE_PATH);
    
    _navigationMenu->setNormalizedPosition(navigationMenuDefs::MENU_NORMALIZED_POSITION);
    background->addChild(_navigationMenu);
    
    background->setAnchorPoint(Point(navigationMenuDefs::MENU_NORMALIZED_POSITION.x, 1.0f));
    background->setPositionX(VisibleRect::right().x - itemsOffsetX);
    
#ifdef EDITOR_MODE
    background->setPositionY(VisibleRect::top().y - navigationMenuDefs::OFFSET_BACKGROUND_Y * 2.0f);
#else
    background->setPositionY(VisibleRect::top().y - navigationMenuDefs::OFFSET_BACKGROUND_Y);
#endif
    
    this->addChild(background);
}

MenuItemSprite* NavigationMenu::setupRestartButton()
{
    Sprite* btnSpriteNormal = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    btnSpriteNormal->setOpacity(0.0f);
    Sprite* btnSpriteActive = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    MenuItemSprite* restartItem = MenuItemSprite::create(btnSpriteNormal, btnSpriteActive, btnSpriteNormal, CC_CALLBACK_1(NavigationMenu::restartPressed, this));
    Sprite* restartSprite = Sprite::create(navigationMenuDefs::RESTART_SPRITE_PATH);
    restartSprite->setPosition(restartItem->getContentSize()/2.0f);
    restartItem->addChild(restartSprite);
    
    return restartItem;
}

MenuItemSprite* NavigationMenu::setupZoomButton()
{
    Sprite* btnSpriteNormal = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    btnSpriteNormal->setOpacity(0.0f);
    Sprite* btnSpriteActive = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    auto zoomItem = MenuItemSprite::create(btnSpriteNormal, btnSpriteActive, btnSpriteNormal, CC_CALLBACK_1(NavigationMenu::zoomPressed, this));
    auto zoomSprite = Sprite::create(navigationMenuDefs::ZOOM_SPRITE_PATH);
    zoomSprite->setPosition(zoomItem->getContentSize()/2.0f);
    zoomItem->addChild(zoomSprite);
    
    return zoomItem;
}

MenuItemSprite* NavigationMenu::setupPauseButton()
{
    Sprite* btnSpriteNormal = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    btnSpriteNormal->setOpacity(0.0f);
    Sprite* btnSpriteActive = Sprite::create(navigationMenuDefs::HUD_BUTTON_ACTIVE_SPRITE_PATH);
    auto pauseItem = MenuItemSprite::create(btnSpriteNormal, btnSpriteActive, btnSpriteNormal, CC_CALLBACK_1(NavigationMenu::pausePressed, this));
    auto pauseSprite = Sprite::create(navigationMenuDefs::PAUSE_SPRITE_PATH);
    pauseSprite->setPosition(pauseItem->getContentSize()/2.0f);
    pauseItem->addChild(pauseSprite);
    
    return pauseItem;
}

MenuItemSprite* NavigationMenu::setupStopButton()
{
    auto stopSprite = Sprite::create(navigationMenuDefs::STOP_NOTE_SPRITE_PATH);
    auto menuItemStop = MenuItemSprite::create(stopSprite, stopSprite, CC_CALLBACK_1(NavigationMenu::stopPressed, this));
    return menuItemStop;
}

void NavigationMenu::restartPressed(Ref* sender)
{
    Node* node = static_cast<Node*>(sender);
    node->setName(callbackDefs::RESTART);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}

void NavigationMenu::zoomPressed(Ref* sender)
{
    Node* node = static_cast<Node*>(sender);
    node->setName(callbackDefs::ZOOM);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}

void NavigationMenu::pausePressed(Ref* sender)
{
    Node* node = static_cast<Node*>(sender);
    node->setName(callbackDefs::PAUSE);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}

void NavigationMenu::stopPressed(cocos2d::Ref *sender)
{
    Node* node = static_cast<Node*>(sender);
    node->setName(callbackDefs::STOP_NOTE);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
}

void NavigationMenu::showStopNoteButton()
{
    if (_menuItemStopPlayer)
    {
        _menuItemStopPlayer->setEnabled(true);
        _menuItemStopPlayer->stopActionByTag(static_cast<int>(eAction::FADE));
        FadeIn * fadeIn = FadeIn::create(0.5f);
        fadeIn->setTag(static_cast<int>(eAction::FADE));
        _menuItemStopPlayer->runAction(fadeIn);
    }
    
    if (_menuItemStopFixed)
    {
        _menuItemStopFixed->runAction(MoveTo::create(0.5f, VisibleRect::bottom() + Point( 190, 42)));
    }
}

void NavigationMenu::hideStopNoteButton()
{
    if (_menuItemStopPlayer)
    {
        _menuItemStopPlayer->setEnabled(false);
        _menuItemStopPlayer->stopActionByTag(static_cast<int>(eAction::FADE));
        FadeOut * fadeOut = FadeOut::create(0.5);
        fadeOut->setTag(static_cast<int>(eAction::FADE));
        _menuItemStopPlayer->runAction(fadeOut);
    }
    
    if (_menuItemStopFixed)
    {
        _menuItemStopFixed->runAction(MoveTo::create(0.5f, VisibleRect::bottom() + Point( 190, -100)));
    }
}

void NavigationMenu::updateStopNoteButtonInputType(eInputType inputType)
{
    switch (inputType)
    {
        case eInputType::KEYBOARD:
        case eInputType::SPLIT_SCREEN:
        case eInputType::GAMEPAD:
        case eInputType::BUTTONS:
        {
            if (_menuItemStopPlayer)
            {
                _menuItemStopPlayer->setVisible(true);
            }
            if (_menuItemStopFixed)
            {
                _menuItemStopFixed->setVisible(false);
            }
            break;
        }
        default:
        {
            if (_menuItemStopPlayer)
            {
                _menuItemStopPlayer->setVisible(false);
            }
            if (_menuItemStopFixed)
            {
                _menuItemStopFixed->setVisible(true);
            }
            break;
        }
    }
}

void NavigationMenu::updateStopNoteButtonEffect(eNoteEffect noteEffect)
{
    std::string newSpriteName = "";
    switch (noteEffect) {
        case eNoteEffect::NORMAL_NOTE:
            newSpriteName = navigationMenuDefs::STOP_NOTE_SPRITE_PATH;
            break;
        case eNoteEffect::INVERSE_NOTE:
            newSpriteName = navigationMenuDefs::STOP_INVERSE_NOTE_SPRITE_PATH;
            break;
        case eNoteEffect::NO_EFFECT:
        default:
            break;
    }
    
    if (false == newSpriteName.empty())
    {
        Sprite * newSprite = Sprite::create(newSpriteName);
        
        if (_menuItemStopPlayer)
        {
            Sprite * normalSprite = dynamic_cast<Sprite *>(_menuItemStopPlayer->getNormalImage());
            if (normalSprite)
            {
                normalSprite->setSpriteFrame(newSprite->getSpriteFrame());
            }
            Sprite * selectedSprite = dynamic_cast<Sprite *>(_menuItemStopPlayer->getSelectedImage());
            if (selectedSprite)
            {
                selectedSprite->setSpriteFrame(newSprite->getSpriteFrame());
            }
        }
        
        if (_menuItemStopFixed)
        {
            Sprite * normalSprite = dynamic_cast<Sprite *>(_menuItemStopFixed->getNormalImage());
            if (normalSprite)
            {
                normalSprite->setSpriteFrame(newSprite->getSpriteFrame());
            }
            Sprite * selectedSprite = dynamic_cast<Sprite *>(_menuItemStopFixed->getSelectedImage());
            if (selectedSprite)
            {
                selectedSprite->setSpriteFrame(newSprite->getSpriteFrame());
            }
        }
    }
}

void NavigationMenu::disableMenu()
{
    if (_navigationMenu)
    {
        _navigationMenu->setEnabled(false);
    }
}

void NavigationMenu::enableMenu()
{
    if (_navigationMenu)
    {
        _navigationMenu->setEnabled(true);
    }
}

void NavigationMenu::update(float dt)
{
    if (_menuItemStopPlayer && _player)
    {
        Point position = Point(_player->getContentSize().width / 2.0f, _player->getContentSize().height / 2.0f + 200.0f);
        Point worldPlayerPosition = _player->convertToWorldSpace(position);
        _menuItemStopPlayer->getParent()->setPosition(this->convertToNodeSpace(worldPlayerPosition));
    }
}
