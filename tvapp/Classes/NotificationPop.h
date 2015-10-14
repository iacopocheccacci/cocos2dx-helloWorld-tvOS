//
//  NotificationPop.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 31/08/15.
//
//

#ifndef __SongOfPan__NotificationPop__
#define __SongOfPan__NotificationPop__

#include "cocos2d.h"

USING_NS_CC;

class NotificationPopUp : public Layer
{
public:
    CREATE_FUNC(NotificationPopUp);
    bool init();
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
protected:
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
    
    Node * _panel;
    MenuItem * _dismissMenuItem;
    
    virtual void initTitle() {};
    virtual void initContent() {};
    
private:
    
    void initPanel();
    void initDismissButton();
    void initDismissTimer();
    MenuItem * setupButtonDismiss();
    void dismissPressed(Ref * sender);
    void showIn();
    void showOut(Node * callbackNode);
};

class NotificationWorldUnlockPopUp : public NotificationPopUp
{
public:
    CREATE_FUNC(NotificationWorldUnlockPopUp);
    
private:
    void initTitle() override;
    void initContent() override;
};

class NotificationWorldLockedPopUp : public NotificationPopUp
{
public:
    CREATE_FUNC(NotificationWorldLockedPopUp);
    
private:
    void initTitle() override;
    void initContent() override;
};

class NotificationGameOverPopUp : public NotificationPopUp
{
public:
    CREATE_FUNC(NotificationGameOverPopUp);
    
private:
    void initTitle() override;
    void initContent() override;
};

class NotificationGameNotOverPopUp : public NotificationPopUp
{
public:
    CREATE_FUNC(NotificationGameNotOverPopUp);
    
private:
    void initTitle() override;
    void initContent() override;
};

class NotificationEpilogueLockedPopUp : public NotificationPopUp
{
public:
    CREATE_FUNC(NotificationEpilogueLockedPopUp);
    
private:
    void initTitle() override;
    void initContent() override;
};

#endif /* defined(__SongOfPan__NotificationPop__) */
