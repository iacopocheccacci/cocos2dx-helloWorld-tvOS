#include "HelloWorldScene.h"


USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
//    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    registerControllerListener();

    return true;
}

void HelloWorld::registerControllerListener()
{
    //create an evnetListenerController
    _listener = EventListenerController::create();
    
    //bind onConneected event call function
    _listener->onConnected = CC_CALLBACK_2(HelloWorld::onConnectController,this);
    
    //bind disconnect event call function
    _listener->onDisconnected = CC_CALLBACK_2(HelloWorld::onDisconnectedController,this);
    
    //bind onKeyDown event call function
    _listener->onKeyDown = CC_CALLBACK_3(HelloWorld::onKeyDown, this);
    
    //bind onKeyUp event call function
    _listener->onKeyUp = CC_CALLBACK_3(HelloWorld::onKeyUp, this);
    
    _listener->onKeyRepeat = CC_CALLBACK_3(HelloWorld::onKeyRepeat, this);
    
    //bind onAxis event call function, onAxis will be called when analog stick is changed
    _listener->onAxisEvent = CC_CALLBACK_3(HelloWorld::onAxisEvent, this);
    
    //Activate the listener into the event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_listener, this);
    
    //This function should be called for iOS platform
    Controller::startDiscoveryController();
}

//Controller is the obejects of the Controller，keyCode means the keycode of the controller you click down
void HelloWorld::onKeyDown(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
//    CCLOG("KeyDown:%d", keyCode);
    showButtonState(controller, keyCode, true);
}

void HelloWorld::onKeyUp(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    //You can get the controller by tag, deviceId or devicename if there are multiple controllers
//    CCLOG("KeyUp:%d", keyCode);
    showButtonState(controller, keyCode, false);
}

void HelloWorld::onKeyRepeat(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event){
    showButtonState(controller, keyCode, true);
}

//The axis includes X-axis and Y-axis and its range is from -1 to 1. X-axis is start from left to right and Y-axis is bottom to top.
void HelloWorld::onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event)
{
    const auto& keyStatus = controller->getKeyStatus(keyCode);
    CCLOG("Axis KeyCode:%d Axis Value:%f", keyCode, keyStatus.value);
}

void HelloWorld::onConnectController(Controller* controller, Event* event)
{
    CCLOG("Game controller connected");
}

void HelloWorld::onDisconnectedController(Controller* controller, Event* event)
{
    CCLOG("Game controller disconnected");
}

void HelloWorld::showButtonState(cocos2d::Controller *controller, int keyCode, bool isPressed)
{
//    onConnectController(controller,nullptr);
    
    if (isPressed)
    {
        switch (keyCode)
        {
            case Controller::Key::BUTTON_A:
                CCLOG("Button A Pressed");
                break;
            case Controller::Key::BUTTON_B:
                CCLOG("Button B Pressed");
                break;
            case Controller::Key::BUTTON_X:
                CCLOG("Button X Pressed");
                break;
            case Controller::Key::BUTTON_Y:
                CCLOG("Button Y Pressed");
                break;
            case Controller::Key::BUTTON_DPAD_UP:
                CCLOG("Button DPAD_UP Pressed");
                break;
            case Controller::Key::BUTTON_DPAD_DOWN:
                CCLOG("Button DPAD_Down Pressed");
                break;
            case Controller::Key::BUTTON_DPAD_LEFT:
                CCLOG("Button DPAD_Left Pressed");
                break;
            case Controller::Key::BUTTON_DPAD_RIGHT:
                CCLOG("Button DPAD_Right Pressed");
                break;
            case Controller::Key::BUTTON_LEFT_SHOULDER:
                CCLOG("Button Left Shoulder Pressed");
                break;
            case Controller::Key::BUTTON_RIGHT_SHOULDER:
                CCLOG("Button Right Shoulder Pressed");
                break;
            case Controller::Key::BUTTON_LEFT_THUMBSTICK:
                CCLOG("Button Left ThumbStick Pressed");
                break;
            case Controller::Key::BUTTON_RIGHT_THUMBSTICK:
                CCLOG("Button Right Shoulder Pressed");
                break;
            default:
            {
                char ketStatus[30];
                sprintf(ketStatus,"Key Down:%d",keyCode);
                break;
            }
        }
    }
    else
    {
        switch (keyCode)
        {
            case Controller::Key::BUTTON_A:
                CCLOG("Button A");
                break;
            case Controller::Key::BUTTON_B:
                CCLOG("Button B");
                break;
            case Controller::Key::BUTTON_X:
                CCLOG("Button X");
                break;
            case Controller::Key::BUTTON_Y:
                CCLOG("Button Y");
                break;
            case Controller::Key::BUTTON_DPAD_UP:
                CCLOG("Button DPAD_UP");
                break;
            case Controller::Key::BUTTON_DPAD_DOWN:
                CCLOG("Button DPAD_Down");
                break;
            case Controller::Key::BUTTON_DPAD_LEFT:
                CCLOG("Button DPAD_Left");
                break;
            case Controller::Key::BUTTON_DPAD_RIGHT:
                CCLOG("Button DPAD_Right");
                break;
            case Controller::Key::BUTTON_LEFT_SHOULDER:
                CCLOG("Button Left Shoulder");
                break;
            case Controller::Key::BUTTON_RIGHT_SHOULDER:
                CCLOG("Button Right Shoulder");
                break;
            case Controller::Key::BUTTON_LEFT_THUMBSTICK:
                CCLOG("Button Left ThumbStick");
                break;
            case Controller::Key::BUTTON_RIGHT_THUMBSTICK:
                CCLOG("Button Right Shoulder");
                break;
            default:
            {
                char ketStatus[30];
                sprintf(ketStatus,"Key Down:%d",keyCode);
                break;

            }
        }
    }
}