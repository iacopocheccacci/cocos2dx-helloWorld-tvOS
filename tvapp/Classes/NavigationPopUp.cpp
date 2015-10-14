//
//  NavigationPopUp.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 17/04/15.
//
//

#include "NavigationPopUp.h"
#include "Definitions.h"
#include "VisibleRect.h"
#include "Utility.h"
#include "AudioOptions.h"
#include "InputSelection.h"

namespace navigationPopUpDefs
{
    static const float DEFAULT_ALIGN_PADDING = 8.0f;
    
    static const float DEFAULT_PRESENTATION_TIME = 0.5f;
    static const char* DEFAULT_BTN_NORMAL_SPRITE_NAME = "UI/popupBtn.png";
    static const char* DEFAULT_BTN_PRESSED_SPRITE_NAME = "UI/popupBtn_tap.png";
    static const char* DEFAULT_BTN_IAP_NORMAL_SPRITE_NAME = "UI/popupBtnIAP.png";
    static const char* DEFAULT_BTN_IAP_PRESSED_SPRITE_NAME = "UI/popupBtnIAP_tap.png";
    static const char* DEFAULT_BTN_CLOSE_NORMAL_SPRITE_NAME = "UI/closeBtn.png";
    static const char* DEFAULT_BTN_CLOSE_PRESSED_SPRITE_NAME = "UI/closeBtn_tap.png";
    static const char* DEFAULT_BTN_BACK_NORMAL_SPRITE_NAME = "UI/backBtn.png";
    static const char* DEFAULT_BTN_BACK_PRESSED_SPRITE_NAME = "UI/backBtn_tap.png";
    
    static const size_t PAGE_INDEX_MENU = 0;
    static const size_t PAGE_INDEX_OPTIONS = 1;
}

NavigationPopUp::~NavigationPopUp()
{
    removeAllChildrenWithCleanup(true);
}

NavigationPopUp * NavigationPopUp::createWithLayerColor(Color4B color, float width, float height)
{
    NavigationPopUp * layer = new (std::nothrow) NavigationPopUp();
    if(layer && layer->initWithLayerColor(color, width, height))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}
       
NavigationPopUp * NavigationPopUp::createWithSpriteName(std::string spriteName)
{
    NavigationPopUp * layer = new (std::nothrow) NavigationPopUp();
    if(layer && layer->initWithSpriteName(spriteName))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool NavigationPopUp::initWithLayerColor(Color4B color, float width, float height)
{
    bool initDone = false;
    
    if (PopUp::init())
    {
        _middleGround = LayerColor::create(color, width, height);
        _middleGround->ignoreAnchorPointForPosition(false);
        _middleGround->setPosition(VisibleRect::center());
        _middleGround->setAnchorPoint(Point::ANCHOR_MIDDLE);
        this->addChild(_middleGround);
        
        this->initCommons();
        
        initDone = true;
    }
    
    return initDone;
}
       
bool NavigationPopUp::initWithSpriteName(std::string spriteName)
{
    bool initDone = false;
    
    if (PopUp::init())
    {
        _middleGround = Sprite::create(spriteName);
        _middleGround->setPosition(VisibleRect::center());
        _middleGround->setAnchorPoint(Point::ANCHOR_MIDDLE);
        this->addChild(_middleGround);
        
        this->initCommons();
        
        initDone = true;
    }
    
    return initDone;
}

void NavigationPopUp::initCommons()
{
    _pageView = nullptr;
    _menu = nullptr;
    _resumeButton = nullptr;
    _backButton = nullptr;
    _presentationTime = navigationPopUpDefs::DEFAULT_PRESENTATION_TIME;
    _alignPadding = navigationPopUpDefs::DEFAULT_ALIGN_PADDING;
    
    this->initDefaultMenu();
}
       
void NavigationPopUp::initDefaultMenu()
{
    CCASSERT(_middleGround, "Middle Ground assente!");
    
    Size size = Size(480.0f, 330.0f);
    
    _pageView = ui::PageView::create();
    _pageView->setContentSize(size);
    _pageView->setTouchEnabled(false);
    _pageView->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _pageView->setPosition(Point(_middleGround->getContentSize().width / 2, 237.0f));
    _middleGround->addChild(_pageView);
    
    _pageView->addEventListener(CC_CALLBACK_2(NavigationPopUp::onPageViewEvent, this));
    
    ui::Widget * anchorWidget = nullptr;
    
    anchorWidget = ui::Widget::create();
    anchorWidget->addChild(this->setupMenuPanel(size));
    _pageView->addWidgetToPage(anchorWidget, navigationPopUpDefs::PAGE_INDEX_MENU, true);
    
    anchorWidget = ui::Widget::create();
    anchorWidget->addChild(this->setupOptionsPanel(size));
    _pageView->addWidgetToPage(anchorWidget, navigationPopUpDefs::PAGE_INDEX_OPTIONS, true);
    
    this->setupBackButton();
}

Node * NavigationPopUp::setupMenuPanel(Size panelSize)
{
    _menu = Menu::create();
    _menu->setPosition(Point(panelSize / 2.0f));
    return _menu;
}

Node * NavigationPopUp::setupOptionsPanel(Size panelSize)
{
    Node * rootNode = Node::create();
    rootNode->setPosition(Point(panelSize / 2.0f));
    
    Node * audioNode = AudioOptions::create();
    audioNode->setPosition(0.0f, panelSize.height * 1.0f / 4.0f);
    rootNode->addChild(audioNode);
    
    Node * inputNode = InputSelection::create();
    inputNode->setPosition(0.0f, - panelSize.height * 1.0f / 4.0f);
    rootNode->addChild(inputNode);
    
    return rootNode;
}

void NavigationPopUp::addPredefinedItemToMenu(ePopUpItem predefinedItem)
{
    MenuItem * item = nullptr;
    switch (predefinedItem)
    {
        case ePopUpItem::BUTTON_ITEM_RESTART:
            item = this->setupRestartButton();
            break;
        case ePopUpItem::BUTTON_ITEM_HOME:
            item = this->setupHomeButton();
            break;
        case ePopUpItem::BUTTON_ITEM_IAP:
            item = this->setupIAPButton();
            break;
        case ePopUpItem::BUTTON_ITEM_OPTIONS:
            item = this->setupOptionsButton();
            break;
        case ePopUpItem::BUTTON_ITEM_RESUME:
            this->setupResumeButton();
            break;
        default:
            break;
    }
    if (nullptr != item)
    {
        this->addItemToMenu(item);
    }
}

void NavigationPopUp::addCustomItemToMenu(MenuItem * item)
{
    this->addItemToMenu(item);
}

void NavigationPopUp::setAlignPadding(float padding)
{
    _alignPadding = padding;
    if (_menu)
    {
        _menu->alignItemsVerticallyWithPadding(_alignPadding);
    }
}

void NavigationPopUp::addItemToMenu(MenuItem * item)
{
    CCASSERT(_menu, "Menu mancante!");
    
    _menu->addChild(item);
    _menu->alignItemsVerticallyWithPadding(_alignPadding);
}

void NavigationPopUp::setMenuActiveRecursive(Node * root, bool enabled)
{
    if (root)
    {
        Menu * menu = dynamic_cast<Menu*>(root);
        if (menu)
        {
            menu->setEnabled(enabled);
        }
        
        for (Node * childNode : root->getChildren())
        {
            this->setMenuActiveRecursive(childNode, enabled);
        }
    }
}

MenuItem* NavigationPopUp::setupRestartButton()
{
    Label * label = nullptr;
    std::string text = "RESTART";
    
    Sprite * normalSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_NORMAL_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(normalSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    normalSprite->addChild(label);
    
    Sprite * pressedSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_PRESSED_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(pressedSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    pressedSprite->addChild(label);
    
    auto item = MenuItemSprite::create(normalSprite, pressedSprite, CC_CALLBACK_1(NavigationPopUp::restartPressed, this));
    return item;
}

MenuItem* NavigationPopUp::setupHomeButton()
{
    Label * label = nullptr;
    std::string text = "MAIN MENU";
    
    Sprite * normalSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_NORMAL_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(normalSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    normalSprite->addChild(label);
    
    Sprite * pressedSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_PRESSED_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(pressedSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    pressedSprite->addChild(label);
    
    auto item = MenuItemSprite::create(normalSprite, pressedSprite, CC_CALLBACK_1(NavigationPopUp::homePressed, this));
    return item;
}

MenuItem* NavigationPopUp::setupIAPButton()
{
    Label * label = nullptr;
    std::string text = "FULL EXPERIENCE";
    std::string price = "2.99"; // Il caratter Euro non c'è
    
    Sprite * normalSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_IAP_NORMAL_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(38.0f, normalSprite->getContentSize().height / 2.0f + 3.0f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    normalSprite->addChild(label);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, price);
    label->setPosition(314.0f, normalSprite->getContentSize().height / 2.0f + 3.0f);
    label->setScale(0.4f);
    normalSprite->addChild(label);
    
    Sprite * pressedSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_IAP_PRESSED_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(38.0f, pressedSprite->getContentSize().height / 2.0f + 3.0f);
    label->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    pressedSprite->addChild(label);
    
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, price);
    label->setPosition(314.0f, pressedSprite->getContentSize().height / 2.0f + 3.0f);
    label->setScale(0.4f);
    pressedSprite->addChild(label);
    
    auto item = MenuItemSprite::create(normalSprite, pressedSprite, CC_CALLBACK_1(NavigationPopUp::iapPressed, this));
    return item;
}

MenuItem* NavigationPopUp::setupOptionsButton()
{
    Label * label = nullptr;
    std::string text = "OPTIONS";
    
    Sprite * normalSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_NORMAL_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(normalSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    normalSprite->addChild(label);
    
    Sprite * pressedSprite = Sprite::create(navigationPopUpDefs::DEFAULT_BTN_PRESSED_SPRITE_NAME);
    label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, text);
    label->setPosition(Point(pressedSprite->getContentSize()/2.0f) + Point(0.0f, 3.0f));
    label->setScale(0.5f);
    pressedSprite->addChild(label);
    
    auto item = MenuItemSprite::create(normalSprite, pressedSprite, CC_CALLBACK_1(NavigationPopUp::optionsPressed, this));
    return item;
}

void NavigationPopUp::setupResumeButton()
{
    // Pulsante Close
    auto resumeItem = MenuItemImage::create(navigationPopUpDefs::DEFAULT_BTN_CLOSE_NORMAL_SPRITE_NAME,
                                            navigationPopUpDefs::DEFAULT_BTN_CLOSE_PRESSED_SPRITE_NAME,
                                            CC_CALLBACK_1(NavigationPopUp::resumePressed, this));
    
    // Menu Pulsante Close
    _resumeButton = Menu::create(resumeItem, NULL);
    Point position = Point(_middleGround->getContentSize().width - 96.0f, _middleGround->getContentSize().height - 43.0f);
    _resumeButton->setPosition(position);
    _middleGround->addChild(_resumeButton);
}

void NavigationPopUp::setupBackButton()
{
    // Pulsante Back
    auto backItem = MenuItemImage::create(navigationPopUpDefs::DEFAULT_BTN_BACK_NORMAL_SPRITE_NAME,
                                            navigationPopUpDefs::DEFAULT_BTN_BACK_PRESSED_SPRITE_NAME,
                                            CC_CALLBACK_1(NavigationPopUp::backPressed, this));
    
    // Menu Pulsante Back
    _backButton = Menu::create(backItem, NULL);
    Point position = Point(_middleGround->getContentSize().width - 96.0f, _middleGround->getContentSize().height - 43.0f);
    _backButton->setPosition(position);
    _middleGround->addChild(_backButton);
    
    // Inizialmente invisibile e disattivo
    _backButton->setVisible(false);
}

void NavigationPopUp::onEnter()
{
    PopUp::onEnter();
    
    this->showIn();
}

void NavigationPopUp::showIn()
{
    GLubyte currentBackLayerOpacity = this->getOpacity();
    Utility::setCascadeOpacityRecursive(_middleGround, true);
    this->setMenuActiveRecursive(_middleGround, false);
    this->setOpacity(0);
    _middleGround->setOpacity(0);
//    _middleGround->setScale(0.5f);
    CallFunc * activeCallback = CallFunc::create([this](){
        this->setMenuActiveRecursive(_middleGround, true);
        this->disableInteractionForNotSelectedPages();
    });
//    _middleGround->runAction(EaseBackOut::create(ScaleTo::create(_presentationTime, 1.0f)));
    this->runAction(FadeTo::create(_presentationTime, currentBackLayerOpacity));
    _middleGround->runAction(FadeIn::create(_presentationTime));
    this->runAction(Sequence::create(DelayTime::create(_presentationTime), activeCallback, NULL));
}

void NavigationPopUp::showOut(Node * callbackNode)
{
    this->setMenuActiveRecursive(_middleGround, false);
    CallFunc * resumeCallback = CallFunc::create([this, callbackNode](){
        if (_callback && _callbackListener)
        {
            (_callbackListener->*_callback)(callbackNode);
        }
    });
//    _middleGround->runAction(EaseBackIn::create(ScaleTo::create(_presentationTime, 0.5f)));
    this->runAction(FadeOut::create(_presentationTime));
    _middleGround->runAction(FadeOut::create(_presentationTime));
    this->runAction(Sequence::create(DelayTime::create(_presentationTime), resumeCallback, NULL));
}

void NavigationPopUp::restartPressed(Ref* sender)
{
    auto node = static_cast<Node*>(sender);
    node->setName(callbackDefs::RESTART);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
//    this->showOut(node);
}

void NavigationPopUp::homePressed(Ref* sender)
{
    auto node = static_cast<Node*>(sender);
    node->setName(callbackDefs::HOME);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
//    this->showOut(node);
}

void NavigationPopUp::iapPressed(Ref* sender)
{
    CCLOG("Full Experience requested!!!");
    // TODO
}

void NavigationPopUp::resumePressed(Ref* sender)
{
    auto node = static_cast<Node*>(sender);
    node->setName(callbackDefs::RESUME);
    
    this->showOut(node);
}

#pragma mark - Page View Stuff

void NavigationPopUp::optionsPressed(Ref* sender)
{
    if(_pageView)
    {
        _pageView->scrollToPage(navigationPopUpDefs::PAGE_INDEX_OPTIONS);
        
        this->setMenuActiveRecursive(_pageView, false);
        
        if (_resumeButton)
        {
            _resumeButton->setVisible(false);
        }
        if (_backButton)
        {
            _backButton->setVisible(true);
        }
        // Effetto audio??
    }
}

void NavigationPopUp::backPressed(Ref* sender)
{
    if(_pageView)
    {
        _pageView->scrollToPage(navigationPopUpDefs::PAGE_INDEX_MENU);
        
        this->setMenuActiveRecursive(_pageView, false);
        
        // Effetto audio??
    }
}

void NavigationPopUp::onPageViewEvent(Ref * sender, ui::PageView::EventType eventType)
{
    if (eventType == ui::PageView::EventType::TURNING)
    {
        if (_pageView)
        {
            this->setMenuActiveRecursive(_pageView, true);
            
            if (navigationPopUpDefs::PAGE_INDEX_MENU == _pageView->getCurPageIndex())
            {
                if (_resumeButton)
                {
                    _resumeButton->setVisible(true);
                }
                if (_backButton)
                {
                    _backButton->setVisible(false);
                }
            }
            this->disableInteractionForNotSelectedPages();
        }
    }
}

void NavigationPopUp::disableInteractionForNotSelectedPages()
{
    for (ui::Layout * page : _pageView->getPages())
    {
        if (_pageView->getPage(_pageView->getCurPageIndex()) != page)
        {
            this->setMenuActiveRecursive(page, false);
        }
        else
        {
            this->setMenuActiveRecursive(page, true);
        }
    }
}