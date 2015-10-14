#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "base/CCGameController.h"

USING_NS_CC;

class HelloWorld : public cocos2d::Layer
{
public:
    
    void onKeyDown(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    void onKeyUp(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    void onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event);
    void onKeyRepeat(Controller* controller, int keyCode, Event* event);

    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
private:
    
    cocos2d::EventListenerController* _listener;

    void registerControllerListener();
    void showButtonState(cocos2d::Controller *controller, int keyCode, bool isPressed);
    
    void onConnectController(Controller* controller, Event* event);
    void onDisconnectedController(Controller* controller, Event* event);
};


#endif // __HELLOWORLD_SCENE_H__
