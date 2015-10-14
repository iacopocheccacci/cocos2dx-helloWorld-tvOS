//
//  SheepCounter.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 16/02/15.
//
//

#ifndef __SOP_Proto__SheepCounter__
#define __SOP_Proto__SheepCounter__

#include "cocos2d.h"
#include "ScoreManager.h"

USING_NS_CC;

namespace sheepCounterGlobalDefs
{
    static const char* ICON_SPRITE_PATH = "UI/sheepTotal.png";
    static const char* COUNTER_FONT_PATH = "UI/capitoli.fnt";
}

class SheepCounterGlobal : public Layer, ScoreManagerObserver
{
public:
    
    SheepCounterGlobal();
    virtual ~SheepCounterGlobal();
    
    CREATE_FUNC(SheepCounterGlobal);
    virtual bool init();
    
    void initWorld(int world);
    void refreshSheepCounter();
    
//    void setDefaultPosition();
    
private:
    
    static const float PADDING_TOP;
    static const float PADDING_RIGHT;
    
    void initSprite();
    void initCounter();
    void alignElements();
    
    void initScoreManagerRegistration();
    void undoScoreManagerRegistration();
    void onScoreUpdated();
    
    void refreshSheepCounterForAllWorlds();
    void refreshSheepCounterForCurrentWorld();
    
    int _world;
    
    Sprite * _iconSprite;
    Label * _counterLabel;
    
};

#endif /* defined(__SOP_Proto__SheepCounter__) */
