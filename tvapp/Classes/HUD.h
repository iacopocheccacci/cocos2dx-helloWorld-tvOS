//
//  HUD.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#ifndef __SOP_HUD__
#define __SOP_HUD__

#include "cocos2d.h"

USING_NS_CC;

class NoteProgressBar;

enum class eHudChild;
enum class eLevelType;
enum class eInputType;
enum class eNoteEffect;

class HUD : public Layer
{
public:
    virtual ~HUD();
    static HUD * create(eLevelType levelType, Node * player);
    bool init(eLevelType levelType, Node * player);
    
    void pauseNavigationMenuListener();
    void resumeNavigationMenuListenser();
    
    void initSheepCounter(int count, bool flute);
    void decrementSheepCounter();
    void killInSheepCounter();
    void activeFlute();
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
    void initWorldLevelLabel(std::string levelName);
    void initSheepCounterGlobal(int world);
    
    void initResetGlobal();
    void initResetButton();
    void initResetTutorialMoveButton();
    void initResetLevelsButton();
    void initResetFlutesButton();
    
    void showStopBtn();
    void hideStopBtn();
    
    void updateStopBtnInputType(eInputType inputType);
    void updateStopBtnEffect(eNoteEffect noteEffect);
    
    void showFluteFeedback();
    void hideFluteFeedback();
    
    void hideElements();
    void showElements();
    
    void hideGlobalCounter();
    void showGlobalCounter();
};

#endif // __SOP__HUD__)
