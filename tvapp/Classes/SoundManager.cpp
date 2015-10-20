//
//  SoundManager.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 26/01/15.
//
//


#include "SoundManager.h"

#if (CC_TARGET_PLATFORM==CC_PLATFORM_MAC || CC_TARGET_PLATFORM==CC_PLATFORM_IOS || CC_TARGET_PLATFORM==CC_PLATFORM_WINRT || CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_TVOS)
#define FMOD_ACTIVE
#endif

#ifdef FMOD_ACTIVE
#include "fmod_studio.hpp"
#include "fmod.hpp"
#include "fmod_errors.h"
#include "UtilityPlatform.h"
#endif

#include "Definitions.h"

const char * SoundManager::USERDEFAULTS_MUSIC_ACTIVE = "musicplay";
const char * SoundManager::USERDEFAULTS_FX_ACTIVE = "effectsplay";
const char * SoundManager::USERDEFAULTS_MUSIC_VOLUME = "volumeMusic";
const char * SoundManager::USERDEFAULTS_FX_VOLUME = "volumeSounds";

//const char * SoundManager::FILE_BANK_MASTER_DATA_NAME = "Master_Bank.bank";
const char * SoundManager::FILE_BANK_MASTER_META_NAME = "Common.strings.bank";

USING_NS_CC;

SoundManager* SoundManager::m_soundManager = 0;

#ifdef FMOD_ACTIVE
FMOD::Studio::System *_system = {0};

std::vector<FMOD::Studio::Bank*> _banks;
FMOD::Studio::Bank* _stringsBank;

std::vector<FMOD::Studio::EventInstance *> _instancesMusic;
std::vector<FMOD::Studio::EventInstance *> _instancesFX;

std::unordered_map<std::string, FMOD::Studio::EventInstance *> _instancesMap;

void (*Common_Private_Error)(FMOD_RESULT);

void ERRCHECK(FMOD_RESULT result)
{
    if (result != FMOD_OK)
    {
        if (Common_Private_Error)
        {
            Common_Private_Error(result);
        }
        CCLOG("FMOD error %d - %s", result, FMOD_ErrorString(result));
    }
}
#endif

SoundManager::~SoundManager()
{
#ifdef FMOD_ACTIVE
    for (auto iter = _instancesMusic.begin(); iter != _instancesMusic.end(); iter++)
    {
        ERRCHECK((*iter)->release());
    }
    _instancesMusic.clear();
    for (auto iter = _instancesFX.begin(); iter != _instancesFX.end(); iter++)
    {
        ERRCHECK((*iter)->release());
    }
    _instancesFX.clear();
    for (auto iter = _instancesMap.begin(); iter != _instancesMap.end(); iter++)
    {
        ERRCHECK((*iter).second->release());
    }
    _instancesMap.clear();
    
    for (FMOD::Studio::Bank * bank : _banks)
    {
        ERRCHECK(bank->unloadSampleData());
        ERRCHECK(bank->unload());
    }
    ERRCHECK(_stringsBank->unload());
    
    ERRCHECK(_system->release());
    
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(SoundManager::updateSounds), this);
#endif
}

SoundManager::SoundManager()
: _systemVolumeMusic(1.0f)
, _systemVolumeFX(1.0f)
, _userVolumeMusic(1.0f)
, _userVolumeFX(1.0f)
, _expectedSystemVolumeFX(1.0f)
, _expectedSystemVolumeMusic(1.0f)
, _expectedUserVolumeFX(1.0f)
, _expectedUserVolumeMusic(1.0f)
{
    // Sound Init
#ifdef FMOD_ACTIVE
    //    void *extraDriverData = NULL;
    //    utilityInit(&extraDriverData);
    
    _system = NULL;
    ERRCHECK(FMOD::Studio::System::create(&_system) );
    
#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID

    FMOD::System * lowLevelSystem = nullptr;
    ERRCHECK(_system->getLowLevelSystem(&lowLevelSystem));
    ERRCHECK(lowLevelSystem->setOutput(FMOD_OUTPUTTYPE_AUDIOTRACK));

    unsigned int bufferLength;
    int numBuffers;

    ERRCHECK(lowLevelSystem->getDSPBufferSize(&bufferLength, &numBuffers));

    CCLOG("SoundManager - Default Buffer Length %u", bufferLength);
    CCLOG("SoundManager - Default Number of Buffer %d", numBuffers);

    bufferLength = 256; // 512; // 1024;
    numBuffers = 2; // 2; // 8;

    ERRCHECK(lowLevelSystem->setDSPBufferSize(bufferLength, numBuffers));

    ERRCHECK(lowLevelSystem->getDSPBufferSize(&bufferLength, &numBuffers));

    CCLOG("SoundManager - New Buffer Length %u", bufferLength);
    CCLOG("SoundManager - New Number of Buffer %d", numBuffers);

    int sampleRate;
    FMOD_SPEAKERMODE speakerMode;
    int numRawSpeakers;

    ERRCHECK(lowLevelSystem->getSoftwareFormat(&sampleRate , &speakerMode, &numRawSpeakers));

    CCLOG("SoundManager - Default Sample Rate %d", sampleRate);
    CCLOG("SoundManager - Default Speaker mode %d", (int)speakerMode);
    CCLOG("SoundManager - Default Number of Raw Speakers %d", numRawSpeakers);

    sampleRate = 24000;

    ERRCHECK(lowLevelSystem->setSoftwareFormat(sampleRate, speakerMode, numRawSpeakers));

    ERRCHECK(lowLevelSystem->getSoftwareFormat(&sampleRate , &speakerMode, &numRawSpeakers));

    CCLOG("SoundManager - New Sample Rate %d", sampleRate);
    CCLOG("SoundManager - New Speaker mode %d", (int)speakerMode);
    CCLOG("SoundManager - New Number of Raw Speakers %d", numRawSpeakers);

#endif

    ERRCHECK(_system->initialize(64, FMOD_STUDIO_INIT_LIVEUPDATE, FMOD_INIT_NORMAL, 0) );
//    ERRCHECK(_system->initialize(64, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );
    //    ERRCHECK(_system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, 0) );
    ERRCHECK(FMOD_Debug_Initialize(FMOD_DEBUG_LEVEL_WARNING, FMOD_DEBUG_MODE_TTY, 0, nullptr));
    //    ERRCHECK(_system->initialize(32, FMOD_STUDIO_INIT_NORMAL, FMOD_INIT_NORMAL, extraDriverData) );

//    _masterBank = NULL;
//    ERRCHECK(_system->loadBankFile(getMediaPath(FILE_BANK_MASTER_DATA_NAME), FMOD_STUDIO_LOAD_BANK_NORMAL, &_masterBank));
    
    _stringsBank = NULL;
    ERRCHECK(_system->loadBankFile(getMediaPath(FILE_BANK_MASTER_META_NAME), FMOD_STUDIO_LOAD_BANK_NORMAL, &_stringsBank));
    
    Director::getInstance()->getScheduler()->schedule(schedule_selector(SoundManager::updateSounds), this, 0.05, false);
#endif
}

SoundManager* SoundManager::getInstance()
{
    if (!m_soundManager)
    {
        m_soundManager = new SoundManager();
    }
    
    return m_soundManager;
}

void SoundManager::releaseSoundManager()
{
    CC_SAFE_DELETE(m_soundManager);
    m_soundManager = NULL;
}

void SoundManager::preloadAudio(std::vector<std::string> audioGroups)
{
#ifdef FMOD_ACTIVE
    static const int bankNameMaxSize = 100;
    
    for (std::string audioGroup : audioGroups)
    {
        FMOD::Studio::Bank* bank = nullptr;
        std::string audioBankName = audioGroup + ".bank";
        
        bool bankPreloaded = false;
        for (FMOD::Studio::Bank * loadedBank : _banks)
        {
            char bankPath[bankNameMaxSize];
            ERRCHECK(loadedBank->getPath(bankPath, bankNameMaxSize, nullptr));
            if (std::string(bankPath).compare("bank:/" + audioGroup) == 0)
            {
                bankPreloaded = true;
                break;
            }
        }
        
        if (!bankPreloaded)
        {
            ERRCHECK(_system->loadBankFile(getMediaPath(audioBankName.c_str()), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank));
            if (bank)
            {
                ERRCHECK(bank->loadSampleData());
                _banks.push_back(bank);
                CCLOG("Preload banco: %s", audioBankName.c_str());
            }
        }
    }
#endif
}

bool SoundManager::isAudioPreloaded()
{
	bool res = true;
#ifdef FMOD_ACTIVE
    FMOD::Studio::Bank * bank = nullptr;
    auto iter = _banks.begin();
    while (res && iter != _banks.end())
    {
        bank = (*iter);
        FMOD_STUDIO_LOADING_STATE state;
        ERRCHECK(bank->getSampleLoadingState(&state));
        
        if (FMOD_STUDIO_LOADING_STATE::FMOD_STUDIO_LOADING_STATE_LOADED != state)
        {
            res = false;
        }
        iter++;
    }
#endif
	return res;
}

void SoundManager::updateSounds(float dt)
{
#ifdef FMOD_ACTIVE
    ERRCHECK(_system->update());
    
    this->updateMusicVolume(dt);
    this->updateFXVolume(dt);
    
//    this->debugCPU(dt);
//    this->debugMemory(dt);
//    this->debugCountTrackedInstances(dt);
//    this->debugCountInstances(dt);
//    this->debugInstancesMap(dt);
#endif
}

void SoundManager::pauseSoundManager()
{
#ifdef FMOD_ACTIVE
    FMOD::System * lowLevelSystem = nullptr;
    ERRCHECK(_system->getLowLevelSystem(&lowLevelSystem));
    lowLevelSystem->mixerSuspend();
#endif
}

void SoundManager::resumeSoundManager()
{
#ifdef FMOD_ACTIVE
    FMOD::System * lowLevelSystem = nullptr;
    ERRCHECK(_system->getLowLevelSystem(&lowLevelSystem));
    lowLevelSystem->mixerResume();
#endif
}

#pragma mark - Suoni

void * SoundManager::playSoundOnce(const char * soundId)
{
    return this->playSoundOnce(soundId, false);
}

void * SoundManager::playSoundOnce(const char * soundId, const bool isMusic)
{
    return this->playSoundOnce(soundId, isMusic, true);
}

void * SoundManager::playSoundOnce(const char * soundId, const bool isMusic, const bool update)
{
    if (0 == strcmp(soundId, "")) return nullptr;
    
#ifdef FMOD_ACTIVE
    FMOD::Studio::ID sndID = {0};
    ERRCHECK(_system->lookupID(soundId, &sndID));
    FMOD::Studio::EventDescription* sndDescription = NULL;
    ERRCHECK(_system->getEventByID(&sndID, &sndDescription) );
    
    bool isOneShot;
    sndDescription->isOneshot(&isOneShot);
    if (!isOneShot)
    {
        CCLOG("ATTENZIONE: Il suono non è fire and forget");
    }
    
    FMOD::Studio::EventInstance* instance = NULL;
    ERRCHECK(sndDescription->createInstance(&instance));
    
    this->setVolumeForSound(instance, isMusic);
    
    ERRCHECK(instance->start());
    ERRCHECK(instance->release());
    if (update)
    {
        ERRCHECK(_system->update());
    }
    
    return instance;
#else
    return nullptr;
#endif
}

void * SoundManager::startPlaySound(const char * soundId)
{
    return this->startPlaySound(soundId, false);
}

void * SoundManager::startPlaySound(const char * soundId, const bool isMusic)
{
    if (0 == strcmp(soundId, "")) return nullptr;
    
#ifdef FMOD_ACTIVE
    FMOD::Studio::ID sndID = {0};
    FMOD::Studio::EventDescription * sndDescription = NULL;
    ERRCHECK(_system->lookupID(soundId, &sndID));
    ERRCHECK(_system->getEventByID(&sndID, &sndDescription) );
    FMOD::Studio::EventInstance* instance = NULL;
    ERRCHECK( sndDescription->createInstance(&instance) );
    ERRCHECK(instance->start());
    
    this->setVolumeForSound(instance, isMusic);
    
    if (isMusic)
    {
        _instancesMusic.push_back(instance);
    }
    else
    {
        _instancesFX.push_back(instance);
    }
    
    return instance;
#else
    return nullptr;
#endif
}

void SoundManager::stopPlaySound(void * soundHandle)
{
    this->stopPlaySound(soundHandle, false);
}

void SoundManager::stopPlaySound(void * soundHandle, const bool fadeOut)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        ERRCHECK(instance->stop((fadeOut) ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE));
        ERRCHECK(_system->update());
        
        _instancesFX.erase(std::remove(_instancesFX.begin(), _instancesFX.end(), instance), _instancesFX.end());
        _instancesMusic.erase(std::remove(_instancesMusic.begin(), _instancesMusic.end(), instance), _instancesMusic.end());
        for (auto iter = _instancesMap.begin(); iter != _instancesMap.end(); iter++ )
        {
            if (iter->second == soundHandle)
            {
                _instancesMap.erase(iter);
                break;
            }
        }
        ERRCHECK(instance->release());
    }
#endif
}

void SoundManager::pauseSound(void * soundHandle)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        ERRCHECK(instance->setPaused(true));
        ERRCHECK(_system->update());
    }
#endif
}

void SoundManager::resumeSound(void * soundHandle)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        ERRCHECK(instance->setPaused(false));
        ERRCHECK(_system->update());
    }
#endif
}

void SoundManager::updateSoundPosition(void * soundHandle, cocos2d::Point position)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        FMOD_3D_ATTRIBUTES attributes = { { 0 } };
        FMOD_VECTOR fmodPosition = {position.x, position.y, 0};
        attributes.position = fmodPosition;
        ERRCHECK( instance->set3DAttributes(&attributes) );
    }
#endif
}

void SoundManager::updateSoundParameter(void * soundHandle, const char * paramName, const float paramValue, const bool disableOnMax)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid())
    {
        FMOD::Studio::ParameterInstance * parameter = NULL;
        ERRCHECK(instance->getParameter(paramName, &parameter));
        
        if (!parameter->isValid()) return;
        
        if (disableOnMax)
        {
            FMOD_STUDIO_PLAYBACK_STATE instancePlayState;
            ERRCHECK(instance->getPlaybackState(&instancePlayState));
            
            FMOD_STUDIO_PARAMETER_DESCRIPTION paramDescription;
            ERRCHECK(parameter->getDescription(&paramDescription));
            
            if ((instancePlayState == FMOD_STUDIO_PLAYBACK_PLAYING
                 || instancePlayState == FMOD_STUDIO_PLAYBACK_SUSTAINING)
                && paramValue > paramDescription.maximum)
            {
                instance->stop(FMOD_STUDIO_STOP_IMMEDIATE);
                //CCLOG("Suono disabilitato perché troppo distante");
            }
            else if (instancePlayState == FMOD_STUDIO_PLAYBACK_STOPPED && paramValue < paramDescription.maximum)
            {
                instance->start();
                //CCLOG("Suono riabilitato perché abbastanza vicino");
            }
        }
        ERRCHECK(parameter->setValue(paramValue));
//        ERRCHECK(instance->setParameterValue(paramName, paramValue));
    }
#endif
}

bool SoundManager::isSoundPlaying(void * soundHandle)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        FMOD_STUDIO_PLAYBACK_STATE instancePlaybackState;
        instance->getPlaybackState(&instancePlaybackState);
        if (instancePlaybackState == FMOD_STUDIO_PLAYBACK_PLAYING
            || instancePlaybackState == FMOD_STUDIO_PLAYBACK_STARTING
            || instancePlaybackState == FMOD_STUDIO_PLAYBACK_SUSTAINING)
        {
            return true;
        }
        else
        {
            return false;
        }
//        FMOD::ChannelGroup * channelGroup;
//        bool isPlaying;
//        ERRCHECK(instance->getChannelGroup(&channelGroup));
//        ERRCHECK(channelGroup->isPlaying(&isPlaying));
//        return isPlaying;
    }
    else
    {
        return false;
    }
#else
    return false;
#endif
}

float SoundManager::getSoundTimelinePosition(void * soundHandle)
{
    int milliseconds = 0;
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        ERRCHECK(instance->getTimelinePosition(&milliseconds));
    }
#endif
    return (float)milliseconds/1000.0f;
}

void SoundManager::forceUpdate()
{
#ifdef FMOD_ACTIVE
    ERRCHECK(_system->update());
#endif
}

std::vector<std::string> SoundManager::getSoundEventsForVideo(int videoNumber)
{
    std::vector<std::string> list;
#ifdef FMOD_ACTIVE
    static const char * fileTemplate = "event:/sndVideo%d/sndVideo%d_";
    static int nameMaxSize = 200;
    
    int eventCount = 0;
    for (FMOD::Studio::Bank * bank : _banks)
    {
        ERRCHECK(bank->getEventCount(&eventCount));
        if (eventCount > 0)
        {
            char tokenCharArray[nameMaxSize];
            snprintf(tokenCharArray, 200, fileTemplate, videoNumber, videoNumber);
            std::string tokenString = tokenCharArray;
            
            FMOD::Studio::EventDescription** eventDescriptionsArray = (FMOD::Studio::EventDescription **)malloc(sizeof(FMOD::Studio::EventDescription *) * eventCount);
            int count = 0;
            ERRCHECK(bank->getEventList(eventDescriptionsArray, eventCount, &count));
            
            for (int i = 0; i < count; i++)
            {
                char soundPath[nameMaxSize];
                FMOD::Studio::EventDescription * currentEvent = *(eventDescriptionsArray + i);
                ERRCHECK(currentEvent->getPath(soundPath, nameMaxSize, nullptr));
                
                std::string pathString = soundPath;
                if (pathString.find(tokenString) != std::string::npos)
                {
                    list.push_back(pathString);
                }
            }
            
            free(eventDescriptionsArray);
        }
    }
    
#endif
    return list;
}

#pragma mark - Gestione audio

void SoundManager::setUserVolumeMusic(float volume, bool fade)
{
	if (volume < 0.0f || volume > 1.0f) return;
    
    if (fade)
    {
        _expectedUserVolumeMusic = volume;
    }
    else
    {
        _userVolumeMusic = _expectedUserVolumeMusic = volume;
#ifdef FMOD_ACTIVE
        for (auto iter = _instancesMusic.begin(); iter != _instancesMusic.end(); iter++)
        {
            this->setVolumeForSound(*iter, true);
        }
        ERRCHECK(_system->update());
#endif
    }
    return;
}

void SoundManager::setUserVolumeFX(float volume, bool fade)
{
    if ( volume < 0.0f || volume > 1.0f ) return;
    
    if (fade)
    {
        _expectedUserVolumeFX = volume;
    }
    else
    {
        _userVolumeFX = _expectedUserVolumeFX = volume;
#ifdef FMOD_ACTIVE
        for (auto iter = _instancesFX.begin(); iter != _instancesFX.end(); iter++)
        {
            this->setVolumeForSound(*iter, false);
        }
        ERRCHECK(_system->update());
#endif
	}
}

void SoundManager::setSystemVolumeMusic(float volume, bool fade)
{
    if (volume < 0.0f || volume > 1.0f) return;
    
    if (fade)
    {
        _expectedSystemVolumeMusic = volume;
    }
    else
    {
        _systemVolumeMusic = _expectedSystemVolumeMusic = volume;
#ifdef FMOD_ACTIVE
        for (auto iter = _instancesMusic.begin(); iter != _instancesMusic.end(); iter++)
        {
            this->setVolumeForSound(*iter, true);
        }
        ERRCHECK(_system->update());
#endif
    }
    return;
}

void SoundManager::setSystemVolumeFX(float volume, bool fade)
{
    if ( volume < 0.0f || volume > 1.0f ) return;
    
    if (fade)
    {
        _expectedSystemVolumeFX = volume;
    }
    else
    {
        _systemVolumeFX = _expectedSystemVolumeFX = volume;
#ifdef FMOD_ACTIVE
        for (auto iter = _instancesFX.begin(); iter != _instancesFX.end(); iter++)
        {
            this->setVolumeForSound(*iter, false);
        }
        ERRCHECK(_system->update());
#endif
	}
}

void SoundManager::updateMusicVolume(float dt)
{
#ifdef FMOD_ACTIVE
    bool volumeChanged = false;
    
    if (_expectedSystemVolumeMusic != _systemVolumeMusic)
    {
        volumeChanged = true;
        _systemVolumeMusic += (_expectedSystemVolumeMusic - _systemVolumeMusic) / 10.0f;
        if ( fabsf(_expectedSystemVolumeMusic - _systemVolumeMusic) < 0.05f )
        {
            _systemVolumeMusic = _expectedSystemVolumeMusic;
        }
    }
    
    if (_expectedUserVolumeMusic != _userVolumeMusic)
    {
        volumeChanged = true;
        _userVolumeMusic += (_expectedUserVolumeMusic - _userVolumeMusic) / 10.0f;
        if ( fabsf(_expectedUserVolumeMusic - _userVolumeMusic) < 0.05f )
        {
            _userVolumeMusic = _expectedUserVolumeMusic;
        }
    }
    
    if (volumeChanged)
    {
        for (auto iter = _instancesMusic.begin(); iter != _instancesMusic.end(); iter++)
        {
            this->setVolumeForSound(*iter, true);
        }
    }
    
    ERRCHECK(_system->update());
#endif
}

void SoundManager::updateFXVolume(float dt)
{
#ifdef FMOD_ACTIVE
    bool volumeChanged = false;
    
    if (_expectedSystemVolumeFX != _systemVolumeFX)
    {
        volumeChanged = true;
        _systemVolumeFX += (_expectedSystemVolumeFX - _systemVolumeFX) / 10.0f;
        if ( fabsf(_expectedSystemVolumeFX - _systemVolumeFX) < 0.05f )
        {
            _systemVolumeFX = _expectedSystemVolumeFX;
        }
    }
    
    if (_expectedUserVolumeFX != _userVolumeFX)
    {
        volumeChanged = true;
        _userVolumeFX += (_expectedUserVolumeFX - _userVolumeFX) / 10.0f;
        if ( fabsf(_expectedUserVolumeFX - _userVolumeFX) < 0.05f )
        {
            _userVolumeFX = _expectedUserVolumeFX;
        }
    }
    
    if (volumeChanged)
    {
        for (auto iter = _instancesFX.begin(); iter != _instancesFX.end(); iter++)
        {
            this->setVolumeForSound(*iter, false);
        }
    }
    
    ERRCHECK(_system->update());
#endif
}

void SoundManager::setVolumeForSound(void * soundHandle, bool isMusic)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() )
    {
        float volume;
        if (isMusic)
        {
            volume = _userVolumeMusic * _systemVolumeMusic;
        }
        else
        {
            volume = _userVolumeFX * _systemVolumeFX;
        }
        ERRCHECK(instance->setVolume(volume));
    }
#endif
}

#pragma mark - Utilities

bool SoundManager::setUniqueIdForSoundHandle(void * soundHandle, const std::string uniqueId)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * instance = static_cast<FMOD::Studio::EventInstance *>(soundHandle);
    if ( nullptr != instance && instance->isValid() && !uniqueId.empty() )
    {
        if (this->removeInstanceById(uniqueId))
        {
            CCLOG("Attenzione: UniqueId associato ad un'istanza di FMOD già esistente. La precedente istanza è stata distrutta");
        }
        _instancesMap[uniqueId] = instance;
        return true;
    }
#endif
    return false;
    
}

void * SoundManager::getSoundHandleById(const std::string uniqueId)
{
#ifdef FMOD_ACTIVE
    if ( !uniqueId.empty() )
    {
        //        CCLOG("Number of Elements in instancesMap: %d", (int)_instancesMap.size());
        return _instancesMap[uniqueId];
    }
#endif
    return nullptr;
}

bool SoundManager::removeInstanceById(const std::string uniqueId)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventInstance * previousInstance = _instancesMap[uniqueId];
    if (previousInstance)
    {
        ERRCHECK(previousInstance->stop(FMOD_STUDIO_STOP_ALLOWFADEOUT));
        ERRCHECK(previousInstance->release());
        _instancesMap.erase(uniqueId);
        return true;
    }
#endif
    return false;
}

#pragma mark - Debug Section

void SoundManager::debugMemory(float dt)
{
#ifdef FMOD_ACTIVE
    int currentAlloced;
    int maxAlloced;
    FMOD::Memory_GetStats(&currentAlloced, &maxAlloced);
    UPDATE_LOG(CCLOG("FMOD memory Info: current allocated %d, max allocated %d", currentAlloced, maxAlloced), timer, dt, 1.0f);
#endif
}

void SoundManager::debugCountTrackedInstances(float dt)
{
#ifdef FMOD_ACTIVE
    UPDATE_LOG(CCLOG("Mapped Sound Instances: %d", (int)_instancesFX.size() + (int)_instancesMusic.size()), timer, dt, 1.0f);
#endif
}

void SoundManager::debugCountInstances(float dt)
{
#ifdef FMOD_ACTIVE
    FMOD::Studio::EventDescription * eventDescriptionList[500];
    int totalInstances = 0;
    int count = 0;
    int instances = 0;
    
    for (FMOD::Studio::Bank * bank : _banks)
    {
        bank->getEventList(eventDescriptionList, 500, &count);
        for (int i = 0; i < count; i++)
        {
            eventDescriptionList[i]->getInstanceCount(&instances);
            totalInstances += instances;
        }
    }
    UPDATE_LOG(CCLOG("FMOD Instances: %d", totalInstances), timer, dt, 1.0f);
#endif
}

void SoundManager::debugInstancesMap(float dt)
{
#ifdef FMOD_ACTIVE
    std::string instancesStr = "";
    std::string stateStr = "";
    FMOD_STUDIO_PLAYBACK_STATE soundState;
    for (auto iter = _instancesMap.begin(); iter != _instancesMap.end(); iter++)
    {
        iter->second->getPlaybackState(&soundState);
        stateStr = (soundState == FMOD_STUDIO_PLAYBACK_PLAYING) ? "ON": "OFF";
        instancesStr += ( "\n" + iter->first + stateStr);
    }
    UPDATE_LOG(CCLOG("FMOD Instances MAP: %s", instancesStr.c_str()), timer, dt, 3.0f);
#endif
}

void SoundManager::debugCPU(float dt)
{
#ifdef FMOD_ACTIVE
	if (_system)
	{
		FMOD_STUDIO_CPU_USAGE cpuUsage;
		_system->getCPUUsage(&cpuUsage);
		UPDATE_LOG(CCLOG("FMOD CPU usage info:\ndspUsage %f;\nstreamUsage %f;\ngeometryUsage %f;\nupdateUsage %f;\nstudioUsage %f", cpuUsage.dspUsage, cpuUsage.streamUsage, cpuUsage.geometryUsage, cpuUsage.updateUsage, cpuUsage.studioUsage), timer, dt, 1.0f);
	}
#endif
}

#pragma mark - Android JNI

#if CC_TARGET_PLATFORM==CC_PLATFORM_ANDROID
extern "C" {

	JNIEXPORT void Java_org_cocos2dx_cpp_AppActivity_setStateStart(JNIEnv *env, jobject thiz)
	{
		SoundManager::getInstance()->resumeSoundManager();
	}

	JNIEXPORT void Java_org_cocos2dx_cpp_AppActivity_setStateStop(JNIEnv *env, jobject thiz)
	{
		SoundManager::getInstance()->pauseSoundManager();
	}
}

#endif
