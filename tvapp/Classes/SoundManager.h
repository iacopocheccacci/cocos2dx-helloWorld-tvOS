//
//  SoundManager.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 26/01/15.
//
//

#ifndef __SOP_Proto__SoundManager__
#define __SOP_Proto__SoundManager__

#include "cocos2d.h"

enum class eGroundType;

class SoundManager : public cocos2d::Ref
{
    
public:
    
    virtual ~SoundManager();
    static SoundManager* getInstance();
    static void releaseSoundManager();
    
    void preloadAudio(std::vector<std::string> audioGroups);
    
    bool isAudioPreloaded();

    void updateSounds(float dt);
    
    void pauseSoundManager();
    void resumeSoundManager();
    
    // Effects
    void * playSoundOnce(const char * soundId);
    void * playSoundOnce(const char * soundId, const bool isMusic);
    void * playSoundOnce(const char * soundId, const bool isMusic, const bool update);
    void * startPlaySound(const char * soundId);
    void * startPlaySound(const char * soundId, const bool isMusic);
    void stopPlaySound(void * soundHandle);
    void stopPlaySound(void * soundHandle, const bool fadeOut);
    void pauseSound(void * soundHandle);
    void resumeSound(void * soundHandle);
    
    void updateSoundPosition(void * soundHandle, cocos2d::Point position);
    void updateSoundParameter(void * soundHandle, const char * paramName, const float paramValue, const bool disableOnMax);
    
    bool isSoundPlaying(void * soundHandle);
    float getSoundTimelinePosition(void * soundHandle);
    
    // Gestione suoni
    void setUserVolumeMusic(float volume, bool fade);
    void setUserVolumeFX(float volume, bool fade);
    
    void setSystemVolumeMusic(float volume, bool fade);
    void setSystemVolumeFX(float volume, bool fade);
    
    bool setUniqueIdForSoundHandle(void * soundHandle, const std::string uniqueId);
    void * getSoundHandleById(const std::string uniqueId);
    
    void forceUpdate();
    
    std::vector<std::string> getSoundEventsForVideo(int videoNumber);
    
private:
    
    static const char * USERDEFAULTS_MUSIC_ACTIVE;
    static const char * USERDEFAULTS_FX_ACTIVE;
    static const char * USERDEFAULTS_MUSIC_VOLUME;
    static const char * USERDEFAULTS_FX_VOLUME;
    
    static const char * FILE_BANK_MASTER_DATA_NAME;
    static const char * FILE_BANK_MASTER_META_NAME;
    
    float _systemVolumeFX;
    float _systemVolumeMusic;
    
    float _userVolumeFX;
    float _userVolumeMusic;
    
    float _expectedSystemVolumeFX;
    float _expectedSystemVolumeMusic;
    
    float _expectedUserVolumeFX;
    float _expectedUserVolumeMusic;
    
    SoundManager();
    static SoundManager* m_soundManager;
    
    void* loadAudioGroup(std::string audioGroup);

    void updateMusicVolume(float dt);
    void updateFXVolume(float dt);
    
    bool removeInstanceById(const std::string uniqueId);
    void setVolumeForSound(void * soundHandle, bool isMusic);
    
    // Debug
    void debugCPU(float dt);
    void debugMemory(float dt);
    void debugCountTrackedInstances(float dt);
    void debugCountInstances(float dt);
    void debugInstancesMap(float dt);
};

#endif /* defined(__SOP_Proto__SoundManager__) */
