//
//  SoundComponent.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 04/02/15.
//
//

#ifndef __SOP_Proto__SoundComponent__
#define __SOP_Proto__SoundComponent__

#include "cocos2d.h"

USING_NS_CC;

namespace soundComponentParams
{
    static const char* Y_POS = "PositionY";
}

using namespace std;

class SoundManager;

enum class eGroundType;
class LHPhysicsProtocol;

class SoundComponent : public Node {
    
public:
    
    static const char * SOUND_MUSIC;
    static const char * SOUND_MUSIC_MENU;
    static const char * SOUND_AMBIENCE;
    static const char * SOUND_AMBIENCE_LAST_LEVEL;
    static const char * SOUND_PAN_WALKING;
    static const char * SOUND_PAN_PUSH;
    static const char * SOUND_PAN_PUSH_PANT;
    static const char * SOUND_PAN_JUMP;
    static const char * SOUND_PAN_JUMP_TEMPLE;
    static const char * SOUND_PAN_CATCH_NOTE;
    static const char * SOUND_PAN_DIE;
    static const char * SOUND_PAN_BURN;
    static const char * SOUND_PAN_EXULT;
    static const char * SOUND_FLUTE;
    static const char * SOUND_FLUTE_RED;
    static const char * SOUND_FLUTE_END;
    static const char * SOUND_SHEEP_CATCH;
    static const char * SOUND_SHEEP_IDLE_2;
    static const char * SOUND_SHEEP_IDLE_3;
    static const char * SOUND_SHEEP_WALKING;
    static const char * SOUND_SHEEP_BURN;
    static const char * SOUND_SHEEP_JUMP;
    static const char * SOUND_SHEEP_LAND;
    static const char * SOUND_SHEEP_COLLECTED;
    static const char * SOUND_MONSTER_IDLE_3;
    static const char * SOUND_MONSTER_CATCH;
    static const char * SOUND_INTERACTIVE_FALL;
    static const char * SOUND_PORTAL;
    static const char * SOUND_CATCH_FLUTE;
    static const char * SOUND_BRAZIER;
    static const char * SOUND_BRAZIER_TURN_OFF;
    static const char * SOUND_WOLF_DISCOVERED;
    static const char * SOUND_WOLF_CATCH;
    static const char * SOUND_GOATSKIN_PRESSED;
    static const char * SOUND_GOATSKIN_BLOW;
    static const char * SOUND_DOOR_OPEN;
    static const char * SOUND_DOOR_CLOSE;
    static const char * SOUND_DOOR_SHAKE;
    static const char * SOUND_SWITCH_PRESS;
    static const char * SOUND_SWITCH_RELEASE;
    static const char * SOUND_MONOUSE_PLATFORM_FALL;
    static const char * SOUND_MONOUSE_REVERSE_PLATFORM_FALL;
    static const char * SOUND_GOAL_BURN;
    static const char * SOUND_ALL_SHEEP_FEEDBACK;
    static const char * SOUND_LIGHTNING;
    static const char * SOUND_PORTAL_ACTIVE;
    static const char * SOUND_PORTAL_NOT_ACTIVE;
    static const char * SOUND_VIDEO_STORY;
    static const char * SOUND_EOLO_LOAD;
    static const char * SOUND_EOLO_TORNADO;
    static const char * SOUND_SHEEP_SHOW;
    
    static const std::unordered_map<std::string, std::vector<std::string>> SOUND_GROUPS;
    
    SoundComponent();
    virtual ~SoundComponent();
    
    static SoundComponent * create(const string soundName);
    static SoundComponent * create(const string soundName, const bool isMusic);
    bool init(const string soundName, const bool isMusic);
    
    // Accessors
    void setSoundName(string soundName)                  { _soundName = soundName; }
    string getSoundName()                                { return _soundName; }
    
    void setPositionRefNode(LHPhysicsProtocol * node)    { _positionRefNode = node; }
    LHPhysicsProtocol * getPositionRefNode()             { return _positionRefNode; }
    
    void setEnable(const bool enable)                    { _isEnabled = enable; }
    bool isEnable()                                      { return _isEnabled; }
    
    void setDestroySoundOnExit(const bool destroy)       { _destroySoundOnExit = destroy; }
    
    void playOnce();
    void playOnce(const bool follow);
    void playOnce(const bool follow, const bool update);
    void playOnceWithDelay(const float interval);
    void startPlay();
    void startPlayWithSchedule(const float interval);
    void startPlayCrossScene();
    void stopPlay();
    void stopPlay(const bool fadeOut);
    void pauseSound();
    void resumeSound();
    
    bool isPlaying();
    float getTimelinePosition();
    
    bool markSoundWithUniqueId(const string uniqueId);
    bool linkSoundByUniqueId(const string uniqueId);
    
    void forceUpdate();
    
    // Studiato per l'audio posizionale tramite i parametri built-in di FMOD
    void updatePosition(cocos2d::Point position);
    
    // Aggiornamento generico di un parametro personalizzato
    void updateParameter(const char * paramName, const float paramValue);
    void updateParameter(const char * paramName, const float paramValue, const bool disableOnMax);
    
    // Metodi di aggiornamento per parametri di uso comune
    void updateParameterDistanceFromNode(LHPhysicsProtocol * node);
    void updateParameterDistanceFromNode(LHPhysicsProtocol * node, const bool disableOnMax);
    void updateParameterDirectionTowardsNode(LHPhysicsProtocol * node);
    void updateParameterDistanceFromRefNode();
    void updateParameterDistanceFromRefNode(const bool disableOnMax);
    void updateParameterDirectionTowardsRefNode();
    
    // Utilities
    static string getStringFromGround(eGroundType groundType);
    static float getDirectionFromVector(Point vector);
    static void preloadAudioForLevelName(std::string levelName);
    
private:
    
    SoundManager * _defaultManager;
    
    string _soundName;
    void * _soundHandle;
    
    bool _isMusic;
    
    bool _isPaused;
    bool _isEnabled;
    string _uniqueId;
    bool _destroySoundOnExit;
    
    LHPhysicsProtocol * _positionRefNode;
    
    Action * _scheduledSound;
};

#endif /* defined(__SOP_Proto__SoundComponent__) */
