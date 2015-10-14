//
//  NoteProgressBar.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 14/11/14.
//
//

#ifndef __SOP_NOTE_PROGRESS_BAR__
#define __SOP_NOTE_PROGRESS_BAR__

#include "cocos2d.h"

USING_NS_CC;

enum class eBarElementType;

class NoteProgressBar : public Node
{
public:
    virtual ~NoteProgressBar();
    CREATE_FUNC(NoteProgressBar);
    virtual bool init();
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    void startTimer(CallFunc* callback, float time, bool inactive);
    void stopTimer();
    void setNoteColor(Color3B color);

private:
    void initBar();
    void showProgressBar();
    void update(float dt);
    
    float _percentage;
    float _currentTime;
    float _startTime;
    ProgressTimer * _progressTimer;
    CallFunc* _timerCallback;
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
};

#endif // __SOP_NOTE_PROGRESS_BAR__
