//
//  SheepCounterView.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/11/14.
//
//

#ifndef __SOP_SHEEP_COUNTER_VIEW__
#define __SOP_SHEEP_COUNTER_VIEW__

#include "cocos2d.h"

USING_NS_CC;

namespace sheepCounterViewDefs
{
    static const char* SHEEP_DEACT_SPRITE = "UI/sheepIco.png";
    static const char* SHEEP_ACTIVE_SPRITE = "UI/sheepIco_active.png";
    static const char* SHEEP_DEAD_SPRITE = "UI/sheepIco_dead";
    static const char* FLUTE_DEACT_SPRITE = "UI/fluteIco.png";
    static const char* FLUTE_ACTIVE_SPRITE = "UI/fluteIco_active.png";
}

class SheepCounterView : public Layer
{
public:
    virtual ~SheepCounterView();
    CREATE_FUNC(SheepCounterView);
    bool init();
    
    void initCounter(int count, bool flute);
    void decrementCounter();
    void killSheep();
    void activeFlute();
    
private:
    int _sheepCount;
    int _killIndex;
    
    void alignItemsWithPadding(float padding);
};

#endif // __SOP_SHEEP_COUNTER_VIEW__
