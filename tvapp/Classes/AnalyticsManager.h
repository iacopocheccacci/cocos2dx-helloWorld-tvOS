//
//  AnalyticsManager.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 29/06/15.
//
//

#ifndef __SongOfPan__AnalyticsManager__
#define __SongOfPan__AnalyticsManager__

#include "cocos2d.h"

USING_NS_CC;

enum class eAnalyticsOptionFlute
{
    NOT_AVAILABLE,
    MISSED,
    TAKEN
};

class AnalyticsManager
{
public:
    
    static void initAnalytics();
    static void startSession();
    static void endSession();
    
    static void sendEventStartLevel(std::string levelName);
    static void sendEventEndLevel(std::string levelName, int sheepCaughtCount, int restartsNum, eAnalyticsOptionFlute flute, bool win);
    
private:
    
    
    static bool fluteOptionToString(eAnalyticsOptionFlute fluteOption, std::string& str);
};

#endif /* defined(__SongOfPan__AnalyticsManager__) */
