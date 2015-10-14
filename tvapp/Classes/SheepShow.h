//
//  SheepShow.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 03/09/15.
//
//

#ifndef __SongOfPan__SheepShow__
#define __SongOfPan__SheepShow__

#include "LevelHelper2API.h"
#include "Definitions.h"

USING_NS_CC;

namespace sheepShowDefs
{
    static const char* CLASS_NAME = "SheepShow";
    static const float DELAY_TO_PLAY = 3.0f;
    static const float MUSIC_DURATION = 65.0f;
    static const char* SHOW_FLUTES_LEFT_NODE_NAME = "FluteLeftNode";
    static const char* SHOW_FLUTES_RIGHT_NODE_NAME = "FluteRightNode";
    static const char* SHOW_CHORUS_NODE_NAME = "ChorusNode";
    static const char* SHOW_SOLO_SHEEP_NODE_NAME = "SoloSheepsNode";
    static const char* SHOW_SOLO_WOLF_NODE_NAME = "SoloWolfNode";
    static const std::vector<std::string> COMPONENT_ORDER = {
        SHOW_FLUTES_LEFT_NODE_NAME,
        SHOW_FLUTES_RIGHT_NODE_NAME,
        SHOW_CHORUS_NODE_NAME,
        SHOW_SOLO_WOLF_NODE_NAME,
        SHOW_SOLO_SHEEP_NODE_NAME
    };
    static const std::map<std::string, std::string> TRACK_PARAMS = {
        {SHOW_FLUTES_LEFT_NODE_NAME, "volume_flauti_1"},
        {SHOW_FLUTES_RIGHT_NODE_NAME, "volume_flauti_2"},
        {SHOW_CHORUS_NODE_NAME, "volume_coro"},
        {SHOW_SOLO_WOLF_NODE_NAME, "volume_lupo"},
        {SHOW_SOLO_SHEEP_NODE_NAME, "volume_soliste"}
    };
    static const float PARAMS_OFF_VALUE = 0.0f;
    static const float PARAMS_ON_VALUE = 1.0f;
    static const char* SHOW_IDLE_ANIMATION_NAME = "idle";
    
    static const std::map<float, std::string> SCHEDULE_FLUTE_LEFT = {
        {0.0f, "playLoop"}
    };
    static const std::map<float, std::string> SCHEDULE_FLUTE_RIGHT = {
        {0.0f, "playLoop"}
    };
    static const std::map<float, std::string> SCHEDULE_CHORUS = {
        {1.821f, "playOnce"},
        {7.263f, "playOnce"},
        {12.780f,"playLoop"}
    };
    static const std::map<float, std::string> SCHEDULE_SOLO_WOLF = {
        {21.247f, "playOnce1"},
        {35.418f, "playOnce2"},
        {43.606f, "playOnce3"},
        {54.153f, "playOnce4"},
    };
    static const std::map<float, std::string> SCHEDULE_SOLO_SHEEPS = {
        {3.675f, "playOnce"},
        {8.855f, "playOnce"},
        {21.811f, "playOnce"},
        {28.410f, "playOnce"},
        {35.880f, "playOnce"},
        {39.862f, "playOnce"},
        {43.555f, "playOnce"},
        {47.281f, "playOnce"},
        {49.554f, "playOnce"},
        {52.700f, "playOnce"},
        {56.870f, "playOnce"},
        {60.905f, "playOnceLong"},
    };
    static const char* CREDITS_BODY_NODE_NAME = "CreditsBodyNode";
    static const char* CREDITS_FOOTER_NODE_NAME = "CreditsFooterNode";
    static const float CREDITS_MOVE_Y = 2300.0f;
}

class SoundComponent;

class SheepShow : public LHNode
{
public:
    NODEWITHDICT_FUNC(SheepShow);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void initCredits(LHScene * rootScene);
    
    void setPlayState(bool playing);
    void setPauseState(bool pause);
    
private:
    
    void initComponents();
    void initAudio();
    
    void startPlay(float dt);
    void stopPlay();
    
    void playFlutesLeft();
    void playFlutesRight();
    void playChorus();
    void playSoloWolf();
    void playSoloSheeps();
    
    void startCredits(float dt);
    void stopCredits();
    
    // Utilities
    void startSchedulePlayForMember(Node * member, std::map<float, std::string> schedulePattern);
    void startPlayMemberAnimation(Node * member, std::string animName);
    void stopPlayMemberAnimation(Node * member);
    void setSoundForComponent(Node * component);
    
    SoundComponent * _soundShow;
    
    Node * _creditsBodyNode;
    Node * _creditsFooterNode;
    Point _creditsBodyStartPosition;
    Point _creditsFooterStartPosition;
};


#endif /* defined(__SongOfPan__SheepShow__) */
