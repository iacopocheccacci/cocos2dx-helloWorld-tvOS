//
//  NotificationPop.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 31/08/15.
//
//

#include "NotificationPop.h"
#include "Definitions.h"
#include "VisibleRect.h"
#include "ui/CocosGUI.h"
#include "Utility.h"

namespace NotificationPopUpDefs
{
    static const char* PANEL_SPRITE_NAME = "UI/notificaBg.png";
    static const char* DISMISS_BTN_NORMAL_SPRITE_NAME = "UI/okRed.png";
    static const char* DISMISS_BTN_PRESSED_SPRITE_NAME = "UI/okRed_tap.png";
    static const float ANIMATION_DURATION = 0.5f;
    
    static const float TITLE_POSITION_Y = 139.0f;
    static const float TITLE_SCALE = 0.5f;
    static const Color3B TITLE_COLOR_RED = appParams::FONT_ORANGE;
    static const Color3B TITLE_COLOR_GREEN = {92, 185, 134};
    static const char* TITLE_WORLD_UNLOCK = "NEXT CHAPTER UNLOCKED";
    static const char* TITLE_WORLD_LOCKED = "NEXT CHAPTER STILL LOCKED";
    static const char* TITLE_GAME_OVER = "EPILOGUE UNLOCKED";
    static const char* TITLE_NOT_GAME_OVER = "EPILOGUE STILL LOCKED";
    static const char* TITLE_EPILOGUE_LOCKED = "EPILOGUE IS LOCKED";
    
    static const float CONTENT_POSITION_Y = 96.0f;
    static const float CONTENT_SCALE = 0.5f;
    static const Color3B CONTENT_COLOR = {145, 129, 122};
    static const char* CONTENT_WORLD_UNLOCK = "CONGRATULATIONS\nYOU SAVED ALL THE SHEEP IN THIS CHAPTER";
    static const char* CONTENT_WORLD_LOCKED = "THERE ARE STILL SHEEP TO BE SAVED IN THE CHAPTER";
    static const char* CONTENT_GAME_OVER = "CONGRATULATIONS\nYOU SAVED ALL ZEUS'S SHEEP";
    static const char* CONTENT_NOT_GAME_OVER = "THERE ARE STILL SHEEP TO BE SAVED IN THE CHAPTER";
    static const char* CONTENT_EPILOGUE_LOCKED = "SAVE ALL ZEUS’ SHEEP TO SEE HOW THE STORY ENDS";
    
    // Definisce se si vuole o meno che la notifica vada via da sola dopo il tempo dato
    static const bool AUTO_DISMISS_ACTIVE = false;
    static const float TIME_TO_AUTO_DISMISS = 10.0f;
}

bool NotificationPopUp::init()
{
    bool initDone = false;
    
    if (Layer::init())
    {
        _panel = nullptr;
        _dismissMenuItem = nullptr;
        
        _callbackListener = nullptr;
        _callback = nullptr;
        
        this->initPanel();
        this->initTitle();
        this->initContent();
        
#if (CC_TARGET_PLATFORM!=CC_PLATFORM_TVOS)
        this->initDismissButton();
#endif
        
        if (NotificationPopUpDefs::AUTO_DISMISS_ACTIVE)
        {
            this->initDismissTimer();
        }
        
        this->showIn();
        initDone = true;
    }
    
    return initDone;
}

void NotificationPopUp::setCallbackFunc(Ref *target, SEL_CallFuncN callFunc)
{
    _callbackListener = target;
    _callback = callFunc;
}

void NotificationPopUp::initPanel()
{
    _panel = Sprite::create(NotificationPopUpDefs::PANEL_SPRITE_NAME);
    _panel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
    _panel->setPosition(Point(VisibleRect::center().x, VisibleRect::top().y));
    this->addChild(_panel);
}

void NotificationPopUp::initDismissButton()
{
    Menu * menu = Menu::create(this->setupButtonDismiss(), NULL);
    menu->setPosition(_panel->getContentSize().width/2, 33.0f);
    _panel->addChild(menu);
}

void NotificationPopUp::initDismissTimer()
{
    DelayTime * delay = DelayTime::create(NotificationPopUpDefs::TIME_TO_AUTO_DISMISS);
    CallFunc * dismiss = CallFunc::create([this](){
        if (_dismissMenuItem)
        {
            _dismissMenuItem->setName(callbackDefs::NOTIFICATION_RESUME);;
        }
        this->showOut(_dismissMenuItem);
    });
    this->runAction(Sequence::create(delay, dismiss, NULL));
}

MenuItem * NotificationPopUp::setupButtonDismiss()
{
    _dismissMenuItem = MenuItemImage::create(NotificationPopUpDefs::DISMISS_BTN_NORMAL_SPRITE_NAME,
                                          NotificationPopUpDefs::DISMISS_BTN_PRESSED_SPRITE_NAME,
                                          CC_CALLBACK_1(NotificationPopUp::dismissPressed, this));
    return _dismissMenuItem;
}

void NotificationPopUp::dismissPressed(Ref * sender)
{
    auto node = static_cast<Node*>(sender);
    node->setName(callbackDefs::NOTIFICATION_RESUME);
    
    this->showOut(node);
}

void NotificationPopUp::showIn()
{
    Utility::setCascadeOpacityRecursive(_panel, true);
    if (_dismissMenuItem)
    {
        _dismissMenuItem->setEnabled(false);
    }
    _panel->setOpacity(0);
//    _panel->setScale(0.5f);
    CallFunc * activeCallback = CallFunc::create([this](){
        if (_dismissMenuItem)
        {
            _dismissMenuItem->setEnabled(true);
        }
    });
//    _panel->runAction(EaseBackOut::create(ScaleTo::create(NotificationPopUpDefs::ANIMATION_DURATION, 1.0f)));
    _panel->runAction(FadeIn::create(NotificationPopUpDefs::ANIMATION_DURATION));
    this->runAction(Sequence::create(DelayTime::create(NotificationPopUpDefs::ANIMATION_DURATION), activeCallback, NULL));
}

void NotificationPopUp::showOut(Node * callbackNode)
{
    if (_dismissMenuItem)
    {
        _dismissMenuItem->setEnabled(false);
    }
    CallFunc * resumeCallback = CallFunc::create([this, callbackNode](){
        if (_callback && _callbackListener)
        {
            (_callbackListener->*_callback)(callbackNode);
        }
    });
//    _panel->runAction(EaseBackIn::create(ScaleTo::create(NotificationPopUpDefs::ANIMATION_DURATION, 0.5f)));
    _panel->runAction(FadeOut::create(NotificationPopUpDefs::ANIMATION_DURATION));
    this->runAction(Sequence::create(DelayTime::create(NotificationPopUpDefs::ANIMATION_DURATION), resumeCallback, NULL));
}

#pragma mark - Notification World Unlocked

void NotificationWorldUnlockPopUp::initTitle()
{
    std::string text = NotificationPopUpDefs::TITLE_WORLD_UNLOCK;
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::TITLE_POSITION_Y);
    label->setScale(NotificationPopUpDefs::TITLE_SCALE);
    label->setColor(NotificationPopUpDefs::TITLE_COLOR_GREEN);
    _panel->addChild(label);
}

void NotificationWorldUnlockPopUp::initContent()
{
    std::string text = NotificationPopUpDefs::CONTENT_WORLD_UNLOCK;
    auto label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::CONTENT_POSITION_Y);
    label->setScale(NotificationPopUpDefs::CONTENT_SCALE);
    label->setColor(NotificationPopUpDefs::CONTENT_COLOR);
    _panel->addChild(label);
}

#pragma mark - Notification World Not Yet Unlocked

void NotificationWorldLockedPopUp::initTitle()
{
    std::string text = NotificationPopUpDefs::TITLE_WORLD_LOCKED;
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::TITLE_POSITION_Y);
    label->setScale(NotificationPopUpDefs::TITLE_SCALE);
    label->setColor(NotificationPopUpDefs::TITLE_COLOR_RED);
    _panel->addChild(label);
}

void NotificationWorldLockedPopUp::initContent()
{
    std::string text = NotificationPopUpDefs::CONTENT_WORLD_LOCKED;
    auto label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::CONTENT_POSITION_Y);
    label->setScale(NotificationPopUpDefs::CONTENT_SCALE);
    label->setColor(NotificationPopUpDefs::CONTENT_COLOR);
    _panel->addChild(label);
}

#pragma mark - Notification Game Beaten

void NotificationGameOverPopUp::initTitle()
{
    std::string text = NotificationPopUpDefs::TITLE_GAME_OVER;
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::TITLE_POSITION_Y);
    label->setScale(NotificationPopUpDefs::TITLE_SCALE);
    label->setColor(NotificationPopUpDefs::TITLE_COLOR_GREEN);
    _panel->addChild(label);
}

void NotificationGameOverPopUp::initContent()
{
    std::string text = NotificationPopUpDefs::CONTENT_GAME_OVER;
    auto label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::CONTENT_POSITION_Y);
    label->setScale(NotificationPopUpDefs::CONTENT_SCALE);
    label->setColor(NotificationPopUpDefs::CONTENT_COLOR);
    _panel->addChild(label);
}

#pragma mark - Notification Game Not Yet Beaten

void NotificationGameNotOverPopUp::initTitle()
{
    std::string text = NotificationPopUpDefs::TITLE_NOT_GAME_OVER;
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::TITLE_POSITION_Y);
    label->setScale(NotificationPopUpDefs::TITLE_SCALE);
    label->setColor(NotificationPopUpDefs::TITLE_COLOR_RED);
    _panel->addChild(label);
}

void NotificationGameNotOverPopUp::initContent()
{
    std::string text = NotificationPopUpDefs::CONTENT_NOT_GAME_OVER;
    auto label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::CONTENT_POSITION_Y);
    label->setScale(NotificationPopUpDefs::CONTENT_SCALE);
    label->setColor(NotificationPopUpDefs::CONTENT_COLOR);
    _panel->addChild(label);
}

#pragma mark - Notification Epilogue video play when still unlocked

void NotificationEpilogueLockedPopUp::initTitle()
{
    std::string text = NotificationPopUpDefs::TITLE_EPILOGUE_LOCKED;
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::TITLE_POSITION_Y);
    label->setScale(NotificationPopUpDefs::TITLE_SCALE);
    label->setColor(NotificationPopUpDefs::TITLE_COLOR_RED);
    _panel->addChild(label);
}

void NotificationEpilogueLockedPopUp::initContent()
{
    std::string text = NotificationPopUpDefs::CONTENT_EPILOGUE_LOCKED;
    auto label = Label::createWithBMFont(appParams::DESCRIPTION_FONT_PATH, text, TextHAlignment::CENTER);
    label->setPosition(_panel->getContentSize().width / 2.0f, NotificationPopUpDefs::CONTENT_POSITION_Y);
    label->setScale(NotificationPopUpDefs::CONTENT_SCALE);
    label->setColor(NotificationPopUpDefs::CONTENT_COLOR);
    _panel->addChild(label);
}
