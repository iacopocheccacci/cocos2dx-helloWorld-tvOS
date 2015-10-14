//
//  VideoStory.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 16/07/15.
//
//

#ifndef __SongOfPan__VideoStory__
#define __SongOfPan__VideoStory__

#include "LevelHelper2API.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Definitions.h"

class SoundComponent;
class SoundVideo;

namespace videoStoryDefs
{
    static const char* CLASS_NAME = "VideoStory";
    
    static const char* PLAYBACK_AREA_NAME = "VideoArea";
    static const char* COVER_STUDIO_NAME = "cover";
    static const char* PLAY_BUTTON_STUDIO_NAME = "playButton";
    static const char* LOCK_BUTTON_STUDIO_NAME = "lockButton";
    static const char* VIDEO_FILE_NAME_TEMPLATE = "w%d_videoStory.csb";
    static const float VIDEO_FADE_TIME = 1.0f;
    
    static const float SUBTITLES_SIZE = 0.5f;
    static const char* SUBTITLES_AREA_NAME = "SubtitlesArea";
    static const char* SUBTITLES_FILE_NAME = "subtitles.plist";
    static const char* SUBTITLES_KEY_TEMPLATE = "world_%d";
    static const char* SUBTITLES_TEXT_KEY = "text";
    static const char* SUBTITLES_TIME_KEY = "time";
    static const char* SUBTITLES_DURATION_KEY = "duration";
    static const float SUBTITLES_FADE_IN_TIME = 0.1f;
    static const float SUBTITLES_DELAY_FADE_OUT_TIME = 2.0f;
    static const float SUBTITLES_FADE_OUT_TIME = 0.5f;
    
    static const char* CHAPTER_LABEL_TEMPLATE = "CHAPTER %d";
    static const char* CHAPTER_BONUS_LABEL_TEXT = "EPILOGUE";
    static const Point CHAPTER_LABEL_POSITION = {320.0f, 310.0f};
    static const Point CHAPTER_BONUS_LABEL_POSITION = {320.0f, 285.0f};
    static const Color3B CHAPTER_LABEL_COLOR = {255, 255, 255};
    static const GLubyte CHAPTER_LABEL_OPACITY = 125;
    static const float CHAPTER_LABEL_SCALE = 0.5f;
    
    static const Point TITLE_LABEL_POSITION = {320.0f, 260.0f};
    static const Color3B TITLE_LABEL_COLOR = {255, 255, 255};
    static const GLubyte TITLE_LABEL_OPACITY = 200;
    static const float TITLE_LABEL_SCALE = 1.2f;
    static const std::unordered_map<int, std::string> TITLE_MAP = {
        {1, "THE QUEST"},
        {2, "COMMITMENT"},
        {3, "UNDERSTANDING"},
        {4, "RESPONSIBILITY"},
        {5, "TRUST"},
    };
}

class VideoStory : public LHNode
{
public:
    NODEWITHDICT_FUNC(VideoStory);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void initVideo(LHScene * scene, std::string levelName, Node * player);
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
    void pauseVideo();
    void resumeVideo();
    
private:
    
    struct Subtitle
    {
        Subtitle(float time, std::string text, float duration) : time(time), text(text), duration(duration) {};
        
        float time;
        std::string text;
        float duration;
    };
    
    // Init
    void setupSearchPaths(LHScene * scene);
    void initAudio(int world);
    void initSubtitles(int world);
    void initPlayButton(int world);
    void initLabels(int world);
    
    // Callbacks
    void onVideoEnded();
    void onPlayButtonPressed();
    void onLockButtonPressed();
    void onFrameEvent(cocostudio::timeline::Frame * frame);
    
    // Update
    void update(float dt);
    void updatePlayBack(float dt);
    void updateSubtitles(float dt);
    void updatePlayButton(float dt);
    void updateSoundEffects(float dt);
    
    void playVideo();
    void stopVideo();
    
    void startSubtitles();
    void stopSubtitles(bool needDelay);
    
    // Utilities
    bool isPlayerInVideoArea();
    bool isVideoUnlocked(int world);
    
    cocostudio::timeline::ActionTimeline * _playbackAction;
    Node * _video;
    Node * _player;
    Label * _subtitleLabel;
    ui::Button * _playButton;
    
    bool _updateSoundEffects;
    
    SoundComponent * _audio;
    SoundVideo * _soundVideo;
    
    int _currentAudioFrame;
    
    std::vector<std::unique_ptr<Subtitle>> _subtitles;
    int _subIndex;
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
};

#endif /* defined(__SongOfPan__VideoStory__) */
