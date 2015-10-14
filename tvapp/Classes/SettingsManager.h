//
//  SettingsManager.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 04/03/15.
//
//

#ifndef __SOP_Proto__SettingsManager__
#define __SOP_Proto__SettingsManager__

#include "cocos2d.h"

USING_NS_CC;

enum class eInputType;

class SettingsManager
{
public:
    
    virtual ~SettingsManager();
    static SettingsManager * getInstance();
    
    float loadUserVolumeMusic();
    float loadUserVolumeFX();
    void  saveUserVolumeMusic(const float volume);
    void  saveUserVolumeFX(const float volume);
    
    bool  loadUserActiveMusic();
    bool  loadUserActiveFX();
    void  saveUserActiveMusic(const bool active);
    void  saveUserActiveFX(const bool active);
    
    eInputType loadUserInput();
    void saveUserInput(const eInputType inputType);
    
    bool loadMoveTutorialDone();
    void saveMoveTutorialDone(const bool done);
    
private:
    
    SettingsManager();
    static SettingsManager* _settingsManager;
    
};

#endif /* defined(__SOP_Proto__SettingsManager__) */
