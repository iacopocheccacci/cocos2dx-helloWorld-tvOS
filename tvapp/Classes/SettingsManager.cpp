//
//  SettingsManager.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 04/03/15.
//
//

#include "SettingsManager.h"
#include "InputController.h"

namespace SettingsContants
{
    static const char * SOUND_VOLUME_MUSIC   = "soundVolumeMusic";
    static const char * SOUND_VOLUME_FX      = "soundVolumeFX";
    static const char * SOUND_ACTIVE_MUSIC   = "soundActiveMusic";
    static const char * SOUND_ACTIVE_FX      = "soundActiveFX";
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM== CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    static const char * INPUT_TYPE           = "inputType";
#endif
    static const char * TUTORIAL_MOVE_DONE   = "tutorialMoveDone";
}


SettingsManager * SettingsManager::_settingsManager = 0;

SettingsManager::~SettingsManager()
{
    CC_SAFE_DELETE(_settingsManager);
    _settingsManager = NULL;
}

SettingsManager::SettingsManager()
{
}

SettingsManager* SettingsManager::getInstance()
{
    if (!_settingsManager)
    {
        _settingsManager = new SettingsManager();
    }
    return _settingsManager;
}

#pragma mark - Audio

float SettingsManager::loadUserVolumeMusic()
{
    return UserDefault::getInstance()->getBoolForKey(SettingsContants::SOUND_VOLUME_MUSIC, 1.0f);
}

float SettingsManager::loadUserVolumeFX()
{
    return UserDefault::getInstance()->getBoolForKey(SettingsContants::SOUND_VOLUME_FX, 1.0f);
}

void SettingsManager::saveUserVolumeMusic(const float volume)
{
    UserDefault::getInstance()->setFloatForKey(SettingsContants::SOUND_VOLUME_MUSIC, volume);
    UserDefault::getInstance()->flush();
}

void SettingsManager::saveUserVolumeFX(const float volume)
{
    UserDefault::getInstance()->setFloatForKey(SettingsContants::SOUND_VOLUME_FX, volume);
    UserDefault::getInstance()->flush();
}

bool SettingsManager::loadUserActiveMusic()
{
    return UserDefault::getInstance()->getBoolForKey(SettingsContants::SOUND_ACTIVE_MUSIC, true);
}
bool SettingsManager::loadUserActiveFX()
{
    return UserDefault::getInstance()->getBoolForKey(SettingsContants::SOUND_ACTIVE_FX, true);
}

void SettingsManager::saveUserActiveMusic(const bool active)
{
    UserDefault::getInstance()->setBoolForKey(SettingsContants::SOUND_ACTIVE_MUSIC, active);
    UserDefault::getInstance()->flush();
}

void SettingsManager::saveUserActiveFX(const bool active)
{
    UserDefault::getInstance()->setBoolForKey(SettingsContants::SOUND_ACTIVE_FX, active);
    UserDefault::getInstance()->flush();
}

#pragma mark - Input

eInputType SettingsManager::loadUserInput()
{
    eInputType inputType;
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM== CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    inputType = static_cast<eInputType>(UserDefault::getInstance()->getIntegerForKey(SettingsContants::INPUT_TYPE, 1));
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_TVOS)
    inputType = eInputType::GAMEPAD;
#else
    inputType = eInputType::KEYBOARD;
#endif

#ifdef WINDOWS_STORE
    inputType = eInputType::KEYBOARD;
#endif
    return inputType;
}

void SettingsManager::saveUserInput(const eInputType inputType)
{
#if (CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM== CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    UserDefault::getInstance()->setIntegerForKey(SettingsContants::INPUT_TYPE, static_cast<unsigned int>(inputType));
    UserDefault::getInstance()->flush();
#endif
}

#pragma mark - Tutorial Movimento

bool SettingsManager::loadMoveTutorialDone()
{
    return UserDefault::getInstance()->getBoolForKey(SettingsContants::TUTORIAL_MOVE_DONE, false);
}

void SettingsManager::saveMoveTutorialDone(const bool done)
{
    UserDefault::getInstance()->setBoolForKey(SettingsContants::TUTORIAL_MOVE_DONE, done);
    UserDefault::getInstance()->flush();
}