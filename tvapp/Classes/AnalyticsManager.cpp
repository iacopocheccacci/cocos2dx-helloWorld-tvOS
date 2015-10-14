//
//  AnalyticsManager.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 29/06/15.
//
//

#include "AnalyticsManager.h"

#ifndef EDITOR_MODE
#if (CC_TARGET_PLATFORM==CC_PLATFORM_IOS)// || CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID)
#define FLURRY_ACTIVE
#endif
#endif

#ifdef FLURRY_ACTIVE
#include "PluginFlurryAnalytics/PluginFlurryAnalytics.h"
#endif

namespace AnalyticsManagerDefs
{
#ifdef FLURRY_ACTIVE
    static const char* SHEEPS_CAUGHT_KEY = "Sheeps";
    static const char* RESTARTS_KEY = "Restarts";
    static const char* WIN_KEY = "Win";
    static const char* FLUTE_TAKEN_KEY = "Flute";
#endif
}

void AnalyticsManager::initAnalytics()
{
#ifdef FLURRY_ACTIVE
    sdkbox::PluginFlurryAnalytics::init();
#endif
}

void AnalyticsManager::startSession()
{
#ifdef FLURRY_ACTIVE
    sdkbox::PluginFlurryAnalytics::startSession();
#endif
}

void AnalyticsManager::endSession()
{
#ifdef FLURRY_ACTIVE
    sdkbox::PluginFlurryAnalytics::endSession();
#endif
}

void AnalyticsManager::sendEventStartLevel(std::string levelName)
{
#ifdef FLURRY_ACTIVE
    std::string eventName = levelName;
    
    int sheeps = 0;
    int restarts = 0;
    
    std::map<std::string, std::string> params;
    
    std::stringstream sheepsStringStream;
    sheepsStringStream << sheeps;
    params[AnalyticsManagerDefs::SHEEPS_CAUGHT_KEY] = sheepsStringStream.str();
    
    std::stringstream restartsStringStream;
    restartsStringStream << restarts;
    params[AnalyticsManagerDefs::RESTARTS_KEY] = restartsStringStream.str();
    
    sdkbox::PluginFlurryAnalytics::logEvent(eventName, params, true);
    
#endif
}

void AnalyticsManager::sendEventEndLevel(std::string levelName, int sheepCaughtCount, int restartsNum, eAnalyticsOptionFlute flute, bool win)
{
#ifdef FLURRY_ACTIVE
    std::string eventName = levelName;
    
    int sheeps = sheepCaughtCount;
    int restarts = restartsNum;
    std::string fluteStr = "";
    AnalyticsManager::fluteOptionToString(flute, fluteStr);
    
    std::map<std::string, std::string> params;
    
    std::stringstream sheepsStringStream;
    sheepsStringStream << sheeps;
    params[AnalyticsManagerDefs::SHEEPS_CAUGHT_KEY] = sheepsStringStream.str();
    
    std::stringstream restartsStringStream;
    restartsStringStream << restarts;
    params[AnalyticsManagerDefs::RESTARTS_KEY] = restartsStringStream.str();
    
    params[AnalyticsManagerDefs::FLUTE_TAKEN_KEY] = fluteStr;
    
    params[AnalyticsManagerDefs::WIN_KEY] = (win) ? "YES" : "NO";
    
    sdkbox::PluginFlurryAnalytics::endTimedEvent(eventName, params);
#endif
}

#pragma mark - Utility

bool AnalyticsManager::fluteOptionToString(eAnalyticsOptionFlute fluteOption, std::string& str)
{
    switch(fluteOption)
    {
        case eAnalyticsOptionFlute::NOT_AVAILABLE:
            str = "NOT_AVAILABLE";
            break;
        case eAnalyticsOptionFlute::MISSED:
            str = "MISSED";
            break;
        case eAnalyticsOptionFlute::TAKEN:
            str = "TAKEN";
            break;
        default:
            break;
    }
    return true;
}
