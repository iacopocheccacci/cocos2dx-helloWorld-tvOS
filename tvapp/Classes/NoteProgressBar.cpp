//
//  NoteProgressBar.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 14/11/14.
//
//

#include "NoteProgressBar.h"

namespace noteProgressBarDefs
{
    static const char* BAR_SPRITE_PATH = "UI/cerchioNota.png";
    static const int   NOTE_BLOCK_NR = 4;
    static const float PROGRESS_FIXED_UPDATE = 0.016f;
}

NoteProgressBar::~NoteProgressBar()
{
    removeAllChildrenWithCleanup(true);
    
    _callback = nullptr;
    _callbackListener = nullptr;
}

bool NoteProgressBar::init()
{
    bool initDone = false;
    
    if(Node::init())
    {
        _percentage = 0.0f;
        _currentTime = 0.0f;
        _startTime = 0.0f;
        _progressTimer = nullptr;
        _timerCallback = nullptr;
        
        this->initBar();
        
        initDone = true;
    }
    
    return initDone;
}

void NoteProgressBar::initBar()
{
    auto bar = Sprite::create(noteProgressBarDefs::BAR_SPRITE_PATH);
    _progressTimer = ProgressTimer::create(bar);
    if (_progressTimer)
    {
        _progressTimer->setReverseDirection(false);
        _progressTimer->setCascadeColorEnabled(true);
        _progressTimer->setType(ProgressTimer::Type::RADIAL);
        _progressTimer->setRotation(-45);
        
        this->addChild(_progressTimer, 0);
    }
}

void NoteProgressBar::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void NoteProgressBar::startTimer(CallFunc* callback, float time, bool inactive)
{
    float percentage = 25; // start from 75% (25% in reverse mode)
    
    if (_progressTimer)
    {
        if (!inactive)
        {
                float actualPercentage = (_progressTimer->getPercentage());
                float actualTime = (time * noteProgressBarDefs::NOTE_BLOCK_NR) * actualPercentage / 100;
                percentage += actualPercentage;
                
                if (percentage > 100)
                {
                    // Tronco al massimo la percentuale e il tempo se ho riempito il cerchio (la progress bar)
                    percentage = 100;
                    time *= noteProgressBarDefs::NOTE_BLOCK_NR;
                }
                else
                {
                    time += actualTime;
                }
        }
        else
        {
            this->showProgressBar();
        }
        
        _percentage = percentage;
        _currentTime = time;
        _startTime = time;
        _progressTimer->setPercentage(percentage);
        
        if (_timerCallback)
        {
            CC_SAFE_RELEASE_NULL(_timerCallback);
        }
        _timerCallback = callback;
        _timerCallback->retain();
        
        this->scheduleUpdate();
        
//        timer->setPercentage(percentage);
//        progressAction = ProgressTo::create(time, 0);
//        _currentSeq = Sequence::create(progressAction, callback, NULL);
//        timer->runAction(_currentSeq);
    }
}
    
void NoteProgressBar::update(float dt)
{
    if (_progressTimer)
    {
        _progressTimer->setPercentage( ( _currentTime * _percentage ) / _startTime );
        if (_currentTime == 0.0f)
        {
            if (_timerCallback)
            {
                this->runAction(_timerCallback);
                CC_SAFE_RELEASE_NULL(_timerCallback);
            }
        }
        else
        {
            _currentTime -= noteProgressBarDefs::PROGRESS_FIXED_UPDATE;
            if (_currentTime < 0.0f)
            {
                _currentTime = 0.0f;
            }
        }
    }
}

void NoteProgressBar::stopTimer()
{
    this->unscheduleUpdate();
    if (_progressTimer)
    {
        _progressTimer->setPercentage(0);
        _progressTimer->runAction(ScaleTo::create(0.2f, 0.0));
    }
}

void NoteProgressBar::setNoteColor(cocos2d::Color3B color)
{
    if (_progressTimer)
    {
        _progressTimer->setColor(color);
    }
}

void NoteProgressBar::showProgressBar()
{
    if (_progressTimer)
    {
        _progressTimer->setOpacity(0);
        _progressTimer->setScale(0);
        _progressTimer->runAction(FadeIn::create(1.0f));
        _progressTimer->runAction(ScaleTo::create(0.5f, 1.0f));
    }
}
