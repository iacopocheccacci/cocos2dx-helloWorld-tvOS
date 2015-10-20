//
//  SoundComponent.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 04/02/15.
//
//

#include "SoundComponent.h"

#include "SoundManager.h"

#include "Definitions.h"
#include "Utility.h"

#include "LHNode.h"
#include <Box2D/Box2D.h>

const char * SoundComponent::SOUND_MUSIC = "event:/sndMusic/sndWorld%01dMusic";
const char * SoundComponent::SOUND_MUSIC_MENU = "event:/sndMusic/sndMenuMusic";
const char * SoundComponent::SOUND_AMBIENCE = "event:/sndAmbience/sndWorld%01dAmbience";
const char * SoundComponent::SOUND_AMBIENCE_LAST_LEVEL = "event:/sndAmbience/sndWorld5_7_Ambience";
const char * SoundComponent::SOUND_PAN_WALKING = "event:/sndFootsteps%s";
const char * SoundComponent::SOUND_PAN_PUSH = "event:/sndPush";
const char * SoundComponent::SOUND_PAN_PUSH_PANT = "";//"event:/sndPushPant";
const char * SoundComponent::SOUND_PAN_JUMP = "event:/sndJump";
const char * SoundComponent::SOUND_PAN_JUMP_TEMPLE = "event:/sndJumpTemple";
const char * SoundComponent::SOUND_PAN_CATCH_NOTE = "event:/sndCatchNote";
const char * SoundComponent::SOUND_PAN_BURN = "event:/sndPanBurns";
const char * SoundComponent::SOUND_PAN_DIE = "event:/sndFlute/sndFluteEnd";
const char * SoundComponent::SOUND_PAN_EXULT = "";
const char * SoundComponent::SOUND_SHEEP_CATCH = "event:/sndPecoraCatch";
const char * SoundComponent::SOUND_SHEEP_IDLE_2 = "event:/sndSheepIdle04";
const char * SoundComponent::SOUND_SHEEP_IDLE_3 = "event:/sndSheepIdle02";
const char * SoundComponent::SOUND_SHEEP_WALKING = "event:/sndSheepWalk";
const char * SoundComponent::SOUND_SHEEP_BURN = "event:/sndSheepBurns";
const char * SoundComponent::SOUND_SHEEP_JUMP = "event:/sndSheepJump";
const char * SoundComponent::SOUND_SHEEP_LAND = "event:/sndSheepLand%s";
const char * SoundComponent::SOUND_SHEEP_COLLECTED = "event:/sndSheepCollected";
const char * SoundComponent::SOUND_MONSTER_IDLE_3 = "event:/sndMonsterSheepIdle";
const char * SoundComponent::SOUND_MONSTER_CATCH = "event:/sndMonsterSheepIdle";
const char * SoundComponent::SOUND_FLUTE = "event:/sndFlute/sndWorld%01dFlute";
const char * SoundComponent::SOUND_FLUTE_RED = "event:/sndFlute/sndFluteRed";
const char * SoundComponent::SOUND_FLUTE_END = "event:/sndFlute/sndFluteEnd";
const char * SoundComponent::SOUND_INTERACTIVE_FALL = "event:/sndMovableFall";
const char * SoundComponent::SOUND_PORTAL = "event:/sndPortal";
const char * SoundComponent::SOUND_CATCH_FLUTE = "event:/sndCatchFlute";
const char * SoundComponent::SOUND_BRAZIER = "event:/sndBrazier";
const char * SoundComponent::SOUND_BRAZIER_TURN_OFF = "event:/sndFireOff";
const char * SoundComponent::SOUND_WOLF_DISCOVERED = "event:/sndWolfDiscovered";
const char * SoundComponent::SOUND_WOLF_CATCH = "event:/sndWolfAttack";
const char * SoundComponent::SOUND_GOATSKIN_PRESSED = "event:/sndGoatskinHit";
const char * SoundComponent::SOUND_GOATSKIN_BLOW = "event:/sndGoatskinActive";
const char * SoundComponent::SOUND_DOOR_OPEN = "event:/sndDoorOpen";
const char * SoundComponent::SOUND_DOOR_CLOSE = "event:/sndDoorClose";
const char * SoundComponent::SOUND_DOOR_SHAKE = "event:/sndDoorBlocked";
const char * SoundComponent::SOUND_SWITCH_PRESS = "event:/sndButtonPush";
const char * SoundComponent::SOUND_SWITCH_RELEASE = "event:/sndButtonRelease";
const char * SoundComponent::SOUND_MONOUSE_PLATFORM_FALL = "event:/sndMonousePlatform";
const char * SoundComponent::SOUND_MONOUSE_REVERSE_PLATFORM_FALL = "event:/sndMonouseReverse";
const char * SoundComponent::SOUND_GOAL_BURN = "event:/sndShelterBurns";
const char * SoundComponent::SOUND_ALL_SHEEP_FEEDBACK = "event:/sndZeusVoice";
const char * SoundComponent::SOUND_LIGHTNING = "event:/sndThunder";
const char * SoundComponent::SOUND_PORTAL_ACTIVE = "event:/sndPortalActivate";
const char * SoundComponent::SOUND_PORTAL_NOT_ACTIVE = "event:/sndPortalNotActivate";
const char * SoundComponent::SOUND_VIDEO_STORY = "event:/sndPoetry/sndPoetry_%02d";
const char * SoundComponent::SOUND_EOLO_LOAD = "event:/sndEolo";
const char * SoundComponent::SOUND_EOLO_TORNADO = "event:/sndTornado";
const char * SoundComponent::SOUND_SHEEP_SHOW = "event:/sndMusic/sndBonusMusic";

const std::unordered_map<std::string, std::vector<std::string>> SoundComponent::SOUND_GROUPS = {
    {"MainMenu", {"Common", "Common_2"}},
    {"LevelSelection01", {"world_1"}},
    {"LevelSelection02", {"world_2"}},
    {"LevelSelection03", {"world_3"}},
    {"LevelSelection04", {"world_4"}},
    {"LevelSelection05", {"world_5"}},
    {"LevelSelection06", {"bonus_level"}},
};

SoundComponent::SoundComponent()
: _defaultManager(nullptr)
, _soundName("")
, _soundHandle(nullptr)
, _isMusic(false)
, _scheduledSound(nullptr)
, _isPaused(false)
, _isEnabled(true)
, _uniqueId("")
, _destroySoundOnExit(true)
{
    
}

SoundComponent::~SoundComponent()
{
    if (_destroySoundOnExit)
    {
        this->stopPlay(false);
        _defaultManager = nullptr;
    }
}

SoundComponent * SoundComponent::create(const string soundName)
{
    return SoundComponent::create(soundName, false);
}

SoundComponent * SoundComponent::create(const string soundName, const bool isMusic)
{
    SoundComponent * ret = new (std::nothrow) SoundComponent();
    if (ret != nullptr && ret->init(soundName, isMusic))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    return ret;
}

bool SoundComponent::init(const string soundName, const bool isMusic)
{
    if (!Node::init())
    {
        return false;
    }
    
    _defaultManager = SoundManager::getInstance();
    
    _soundName = soundName;
    _isMusic = isMusic;
    
    return true;
}

#pragma mark -
#pragma mark - Effetti audio
#pragma mark -

#pragma mark - Generale

void SoundComponent::playOnce()
{
    this->playOnce(false);
}

void SoundComponent::playOnce(const bool follow)
{
    this->playOnce(follow, true);
}

void SoundComponent::playOnce(const bool follow, const bool update)
{
    if (_isEnabled)
    {
        void * soundHandle = _defaultManager->playSoundOnce(_soundName.c_str(), _isMusic, update);
        if (follow)
        {
            _soundHandle = soundHandle;
        }
    }
}

void SoundComponent::playOnceWithDelay(const float interval)
{
    if (_isEnabled)
    {
        DelayTime * intervalTime = DelayTime::create(interval);
        
        CallFunc * playCallback = CallFunc::create([this]()
        {
            if (_isPaused == false)
            {
                this->playOnce();
            }
        });
        
        _parent->runAction(Sequence::create(intervalTime, playCallback, NULL));
    }
}

void SoundComponent::startPlay()
{
    if (_soundHandle == nullptr && _isEnabled)
    {
        _soundHandle = _defaultManager->startPlaySound(_soundName.c_str(), _isMusic);
    }
}

void SoundComponent::startPlayWithSchedule(const float interval)
{
    if (_scheduledSound == nullptr && _isEnabled)
    {
        CallFunc * playCallback = CallFunc::create([this]()
        {
            if (_isPaused == false)
            {
                this->playOnce();
            }
        });
        
        DelayTime * intervalTime = DelayTime::create(interval);
        _scheduledSound = RepeatForever::create(Sequence::create(playCallback, intervalTime, NULL));
        _parent->runAction(_scheduledSound);
    }
}

void SoundComponent::startPlayCrossScene()
{
    bool soundAlreadyPlaying = this->linkSoundByUniqueId(_soundName);
    if(!soundAlreadyPlaying)
    {
        this->startPlay();
        this->markSoundWithUniqueId(_soundName);
    }
    this->setDestroySoundOnExit(false);
}

void SoundComponent::stopPlay()
{
    this->stopPlay(false);
}

void SoundComponent::stopPlay(const bool fadeOut)
{
    // Stop nel caso di suono lanciato con startPlay
    if (_soundHandle != nullptr)
    {
        _defaultManager->stopPlaySound(_soundHandle, fadeOut);
        _soundHandle = nullptr;
    }
    
    // Stop nel caso di suono lanciato con startPlayWithSchedule
    if (_scheduledSound != nullptr)
    {
        _parent->stopAction(_scheduledSound);
        _scheduledSound = nullptr;
    }
}

void SoundComponent::pauseSound()
{
    if (_soundHandle != nullptr)
    {
        _defaultManager->pauseSound(_soundHandle);
    }
    _isPaused = true;
}

void SoundComponent::resumeSound()
{
    if (_soundHandle != nullptr)
    {
        _defaultManager->resumeSound(_soundHandle);
    }
    _isPaused = false;
}

bool SoundComponent::isPlaying()
{
    if( !_isPaused )
    {
        if (_scheduledSound)
        {
            return true;
        }
        else
        {
            return _defaultManager->isSoundPlaying(_soundHandle);
        }
    }
    else
    {
        return false;
    }
}

float SoundComponent::getTimelinePosition()
{
    if (_soundHandle)
    {
        return _defaultManager->getSoundTimelinePosition(_soundHandle);
    }
    return 0.0f;
}

void SoundComponent::forceUpdate()
{
    _defaultManager->forceUpdate();
}

#pragma mark - Sistema di gestione suoni tra le scene

bool SoundComponent::markSoundWithUniqueId(const string uniqueId)
{
    if (_soundHandle)
    {
        return _defaultManager->setUniqueIdForSoundHandle(_soundHandle, uniqueId);
    }
    return false;
}

bool SoundComponent::linkSoundByUniqueId(const string uniqueId)
{
    if (!uniqueId.empty())
    {
        _soundHandle = _defaultManager->getSoundHandleById(uniqueId);
        return (_soundHandle) ? true : false;
    }
    return false;
}

#pragma mark - Update Methods

void SoundComponent::updateParameterDistanceFromRefNode()
{
    if(_positionRefNode)
    {
        this->updateParameterDistanceFromNode(_positionRefNode);
    }
}

void SoundComponent::updateParameterDistanceFromRefNode(const bool disableOnMax)
{
    if(_positionRefNode)
    {
        this->updateParameterDistanceFromNode(_positionRefNode, disableOnMax);
    }
}

void SoundComponent::updateParameterDirectionTowardsRefNode()
{
    if(_positionRefNode)
    {
        this->updateParameterDirectionTowardsNode(_positionRefNode);
    }
}

void SoundComponent::updatePosition(cocos2d::Point position)
{
    if (_soundHandle)
    {
        _defaultManager->updateSoundPosition(_soundHandle, position);
    }
}

void SoundComponent::updateParameter(const char * paramName, const float paramValue)
{
    this->updateParameter(paramName, paramValue, false);
}

void SoundComponent::updateParameter(const char * paramName, const float paramValue, const bool disableOnMax)
{
    if(_soundHandle)
    {
        _defaultManager->updateSoundParameter(_soundHandle, paramName, paramValue, disableOnMax);
    }
}

void SoundComponent::updateParameterDistanceFromNode(LHPhysicsProtocol * node)
{
    this->updateParameterDistanceFromNode(node, false);
}

void SoundComponent::updateParameterDistanceFromNode(LHPhysicsProtocol * node, const bool disableOnMax)
{
    if (_soundHandle)
    {
        LHPhysicsProtocol * lhOwner = dynamic_cast<LHPhysicsProtocol *>(_parent);
		
		if (lhOwner && node)
        {
			auto ownerBody = lhOwner->getBox2dBody();
			auto body = node->getBox2dBody();

			if (ownerBody && body)
			{
				b2Vec2 nodeBodyPosition = body->GetPosition();
				b2Vec2 ownerBodyPosition = ownerBody->GetPosition();
				b2Vec2 distanceBodyVector = ownerBodyPosition - nodeBodyPosition;

				float distance = distanceBodyVector.Length();
				this->updateParameter("Distance", distance, disableOnMax);
			}
        }
    }
}

void SoundComponent::updateParameterDirectionTowardsNode(LHPhysicsProtocol * node)
{
    if (_soundHandle)
    {
        LHPhysicsProtocol * lhOwner = dynamic_cast<LHPhysicsProtocol *>(_parent);
		
		if (lhOwner && node )
        {
			auto body = node->getBox2dBody();
			auto ownerBody = lhOwner->getBox2dBody();

			if (ownerBody && body)
			{
				b2Vec2 nodeBodyPosition = body->GetPosition();
				b2Vec2 ownerBodyPosition = ownerBody->GetPosition();
				b2Vec2 distanceBodyVector = ownerBodyPosition - nodeBodyPosition;

				this->updateParameter("Direction", distanceBodyVector.x);
			}
        }
    }
}

#pragma mark - Utilities

std::string SoundComponent::getStringFromGround(eGroundType groundType)
{
    std::string groundStr;
    
    switch (groundType) {
        case eGroundType::GRASS:
            groundStr = "Grass";
            break;
        case eGroundType::STONE:
            groundStr = "Stone";
            break;
        case eGroundType::TEMPLE:
            groundStr = "Temple";
            break;
        case eGroundType::NONE:
            groundStr = "";
            break;
        default:
            groundStr = "";
            break;
    }
    return groundStr;
}

float SoundComponent::getDirectionFromVector(Point vector)
{
    return CC_RADIANS_TO_DEGREES((M_PI - fabs(atan2(vector.y, vector.x))));
}

void SoundComponent::preloadAudioForLevelName(std::string levelName)
{
    int world = Utility::getWorldFromLevelName(levelName);
    
    if (1 <= world && world <= appParams::NUMBER_OF_WORLDS)
    {
        std::stringstream ss;
        
        ss << "world_" << world;
        std::string general = ss.str();
        ss.str(std::string());
        
        ss << "music_" << world;
        std::string music = ss.str();
        ss.str(std::string());
        
        SoundManager::getInstance()->preloadAudio({general, music});
    }
    else if (world == appParams::BONUS_WORLD)
    {
        SoundManager::getInstance()->preloadAudio({"bonus_level", "bonus_music"});
    }
    else
    {
        SoundManager::getInstance()->preloadAudio({"Common", "menu_music"});
    }
}
