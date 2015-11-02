//
//  MyMenu.cpp
//  tvapp
//
//  Created by Iacopo Checcacci on 28/10/15.
//
//

#include "MyMenu.hpp"

MyMenu* MyMenu::create()
{
    return MyMenu::create(nullptr, nullptr);
}

MyMenu * MyMenu::create(MenuItem* item, ...)
{
    va_list args;
    va_start(args,item);
    
    MyMenu *ret = MyMenu::createWithItems(item, args);
    
    va_end(args);
    
    return ret;
}

MyMenu* MyMenu::createWithItems(MenuItem* item, va_list args)
{
    Vector<MenuItem*> items;
    if( item )
    {
        items.pushBack(item);
        MenuItem *i = va_arg(args, MenuItem*);
        while(i)
        {
            items.pushBack(i);
            i = va_arg(args, MenuItem*);
        }
    }
    
    return MyMenu::createWithArray(items);
}

MyMenu* MyMenu::createWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    auto ret = new (std::nothrow) MyMenu();
    if (ret && ret->initWithArray(arrayOfItems))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

bool MyMenu::initWithArray(const Vector<MenuItem*>& arrayOfItems)
{
    if (Layer::init())
    {
        _enabled = true;
        // menu in the center of the screen
        Size s = Director::getInstance()->getWinSize();
        
        this->ignoreAnchorPointForPosition(true);
        setAnchorPoint(Vec2(0.5f, 0.5f));
        this->setContentSize(s);
        
        setPosition(s.width/2, s.height/2);
        
        int z=0;
        
        for (auto& item : arrayOfItems)
        {
            this->addChild(item, z);
            z++;
        }
        
        _selectedItem = nullptr;
        _state = Menu::State::WAITING;
        
        // enable cascade color and opacity on menus
        setCascadeColorEnabled(true);
        setCascadeOpacityEnabled(true);
        
        
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->setSwallowTouches(true);
        
        touchListener->onTouchBegan = CC_CALLBACK_2(Menu::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(Menu::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(Menu::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(Menu::onTouchCancelled, this);
        
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        auto gamePadListener = EventListenerController::create();
        
        gamePadListener->onKeyDown = CC_CALLBACK_3(MyMenu::onKeyDown, this);

        //Activate the listener into the event dispatcher
        _eventDispatcher->addEventListenerWithSceneGraphPriority(gamePadListener, this);
        
        //This function should be called for iOS platform
        Controller::startDiscoveryController();
        
        return true;
    }
    return false;
}

bool MyMenu::onTouchBegan(Touch* touch, Event* event)
{
    auto camera = Camera::getVisitingCamera();
    if (_state != Menu::State::WAITING || ! _visible || !_enabled || !camera)
    {
        return false;
    }
    
    for (Node *c = this->_parent; c != nullptr; c = c->getParent())
    {
        if (c->isVisible() == false)
        {
            return false;
        }
    }
    
    _selectedItem = this->getItemForTouch(touch, camera);
    if (_selectedItem)
    {
        _state = MyMenu::State::TRACKING_TOUCH;
        _selectedWithCamera = camera;
        _selectedItem->selected();
        
        return true;
    }
    
    return false;
}

void MyMenu::onTouchEnded(Touch* touch, Event* event)
{
    CCASSERT(_state == MyMenu::State::TRACKING_TOUCH, "[Menu ccTouchEnded] -- invalid state");
    this->retain();
    if (_selectedItem)
    {
        _selectedItem->unselected();
    }
    _state = Menu::State::WAITING;
    _selectedWithCamera = nullptr;
    this->release();
}

void MyMenu::onKeyDown(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    CCLOG("KeyDown:%d", keyCode);
    
    switch (keyCode)
    {
        case cocos2d::Controller::Key::BUTTON_A:
            if (_selectedItem)
            {
                _selectedItem->unselected();
                _selectedItem->activate();
            }
            break;

        default:
            break;
    }
}

