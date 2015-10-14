//
//  VideoStory.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 16/07/15.
//
//

#include "VideoStory.h"
#include "Utility.h"
#include "SoundComponent.h"
#include "SoundVideo.h"
#include "ScoreManager.h"

using namespace ui;

bool VideoStory::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _playbackAction = nullptr;
        _video = nullptr;
        _player = nullptr;
        _playButton = nullptr;
        _currentAudioFrame = 0;
        _updateSoundEffects = false;
        _callback = nullptr;
        _callbackListener = nullptr;
    }
    
    return status;
}

void VideoStory::initVideo(LHScene * scene, std::string levelName, Node * player)
{
    _player = player;
    
    this->setupSearchPaths(scene);
    
    int world = Utility::getWorldFromLevelName(levelName);
    
    char videoFile[50];
    snprintf(videoFile, 50, videoStoryDefs::VIDEO_FILE_NAME_TEMPLATE, world);
    
    _video = CSLoader::getInstance()->createNodeWithFlatBuffersFile(videoFile);
    if (_video)
    {
        _video->setPosition(Point(this->getContentSize()/2.0f));
        this->addChild(_video);
        
        _playbackAction = CSLoader::createTimeline(videoFile);
        _playbackAction->setCurrentFrame(0);
        _playbackAction->setLastFrameCallFunc(CC_CALLBACK_0(VideoStory::onVideoEnded, this));
        _video->runAction(_playbackAction);
        
        this->initSubtitles(world);
        this->initAudio(world);
        this->initPlayButton(world);
        this->initLabels(world);
        
        this->scheduleUpdate();
    }
}

void VideoStory::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void VideoStory::pauseVideo()
{
    if (_audio)
    {
        _audio->pauseSound();
    }
    if (_soundVideo)
    {
        _soundVideo->pauseAllEffects();
    }
}

void VideoStory::resumeVideo()
{
    if (_audio)
    {
        _audio->resumeSound();
    }
    if (_soundVideo)
    {
        _soundVideo->resumeAllEffects();
    }
}

#pragma mark - Private Methods

void VideoStory::setupSearchPaths(LHScene * scene)
{
    FileUtils * fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths = fileUtils->getSearchPaths();
    
    char studioResPath[50];
    snprintf(studioResPath, 50, appParams::COCOS_STUDIO_RES_PATH_TEMPLATE, scene->getCurrentDeviceSuffix().c_str());
    
    if (searchPaths.end() == std::find(searchPaths.begin(), searchPaths.end(), studioResPath))
    {
        searchPaths.push_back(studioResPath);
        fileUtils->setSearchPaths(searchPaths);
    }
}

void VideoStory::initAudio(int world)
{
    char audioTrackName[50];
    snprintf(audioTrackName, 50, SoundComponent::SOUND_VIDEO_STORY, world);
    _audio = SoundComponent::create(audioTrackName);
    this->addChild(_audio);
    
    // Carico gli effetti sonori
    _soundVideo = SoundVideo::createFromWorld(world);
    this->addChild(_soundVideo);
}

void VideoStory::initSubtitles(int world)
{
    _subIndex = 0;
    
    // Carico i sottotitoli
    FileUtils * fileUtils = FileUtils::getInstance();
    ValueMap allSubtitles = fileUtils->getValueMapFromFile(videoStoryDefs::SUBTITLES_FILE_NAME);
    
    char subtitlesKey[50];
    snprintf(subtitlesKey, 50, videoStoryDefs::SUBTITLES_KEY_TEMPLATE, world);
    
    ValueMap::const_iterator iterSubtitles;
    float time;
    std::string text;
    float duration = -1;
    
    iterSubtitles = allSubtitles.find(subtitlesKey);
    if (allSubtitles.end() != iterSubtitles)
    {
        ValueVector subtitles = (*iterSubtitles).second.asValueVector();
        for (auto iterSubtitle = subtitles.begin(); iterSubtitle != subtitles.end(); iterSubtitle++)
        {
            time = (*iterSubtitle).asValueMap().at(videoStoryDefs::SUBTITLES_TIME_KEY).asFloat();
            text = (*iterSubtitle).asValueMap().at(videoStoryDefs::SUBTITLES_TEXT_KEY).asString();
            
            auto iterDuration = (*iterSubtitle).asValueMap().find(videoStoryDefs::SUBTITLES_DURATION_KEY);
            if (iterDuration != (*iterSubtitle).asValueMap().end())
            {
                duration = (*iterDuration).second.asFloat();
            }
            
            auto subtitle = std::unique_ptr<Subtitle>(new Subtitle(time, text, duration));
            _subtitles.push_back(std::move(subtitle));
        }
    }
    
    // Preparo la label
    Node * subtitleArea = this->getChildNodeWithName(videoStoryDefs::SUBTITLES_AREA_NAME);
    if (subtitleArea)
    {
        // WORKAROUND per bug introdotto con il passaggio a Cocos2dx 3.7.1: la label è grande il doppio di prima
        _subtitleLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "", TextHAlignment::CENTER);
//        _subtitleLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "", TextHAlignment::CENTER, subtitleArea->getContentSize().width);
        
        _subtitleLabel->setScale(videoStoryDefs::SUBTITLES_SIZE);
        _subtitleLabel->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
        _subtitleLabel->setPosition(Point(subtitleArea->getContentSize().width/2.0f, subtitleArea->getContentSize().height));
        _subtitleLabel->setOpacity(0);
        _subtitleLabel->setLineHeight(_subtitleLabel->getLineHeight() + 10.0f);
        subtitleArea->addChild(_subtitleLabel);
    }
}

void VideoStory::initPlayButton(int world)
{
    if (_video)
    {
        Node * cover = _video->getChildByName(videoStoryDefs::COVER_STUDIO_NAME);
        if (cover)
        {
            Button * playButton = cover->getChildByName<Button *>(videoStoryDefs::PLAY_BUTTON_STUDIO_NAME);
            Button * lockButton = cover->getChildByName<Button *>(videoStoryDefs::LOCK_BUTTON_STUDIO_NAME);
            if (this->isVideoUnlocked(world))
            {
                if (playButton)
                {
                    playButton->setVisible(true);
                    _playButton = playButton;
                    
                    _playButton->addTouchEventListener([this](Ref* ref, Widget::TouchEventType type){
                        if (Widget::TouchEventType::ENDED == type)
                        {
                            this->onPlayButtonPressed();
                        }
                    });
                    _playButton->addClickEventListener([this](Ref* ref){
                        this->onPlayButtonPressed();
                    });
                }
                if (lockButton)
                {
                    lockButton->setVisible(false);
                }
            }
            else
            {
                if (lockButton)
                {
                    lockButton->setVisible(true);
                    _playButton = lockButton;
                    
                    _playButton->addTouchEventListener([this](Ref* ref, Widget::TouchEventType type){
                        if (Widget::TouchEventType::ENDED == type)
                        {
                            this->onLockButtonPressed();
                        }
                    });
                    _playButton->addClickEventListener([this](Ref* ref){
                        this->onLockButtonPressed();
                    });
                }
                if (playButton)
                {
                    playButton->setVisible(false);
                }
            }
        }
    }
}

void VideoStory::initLabels(int world)
{
    Node * cover = _video->getChildByName(videoStoryDefs::COVER_STUDIO_NAME);
    if (cover)
    {
        char chapterStr[50];
        Point chapterPosition;
        if (appParams::BONUS_WORLD == world)
        {
            snprintf(chapterStr, 50, "%s", videoStoryDefs::CHAPTER_BONUS_LABEL_TEXT);
            chapterPosition = videoStoryDefs::CHAPTER_BONUS_LABEL_POSITION;
        }
        else
        {
            std::string titleStr = videoStoryDefs::TITLE_MAP.at(world);
            Label * titleLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, titleStr, TextHAlignment::CENTER);
            titleLabel->setPosition(videoStoryDefs::TITLE_LABEL_POSITION);
            titleLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
            titleLabel->setScale(videoStoryDefs::TITLE_LABEL_SCALE);
            titleLabel->setColor(videoStoryDefs::TITLE_LABEL_COLOR);
            titleLabel->setOpacity(videoStoryDefs::TITLE_LABEL_OPACITY);
            cover->addChild(titleLabel);
            
            snprintf(chapterStr, 50, videoStoryDefs::CHAPTER_LABEL_TEMPLATE, world);
            chapterPosition = videoStoryDefs::CHAPTER_LABEL_POSITION;
        }
        
        Label * chapterLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, chapterStr, TextHAlignment::CENTER);
        chapterLabel->setPosition(chapterPosition);
        chapterLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
        chapterLabel->setScale(videoStoryDefs::CHAPTER_LABEL_SCALE);
        chapterLabel->setColor(videoStoryDefs::CHAPTER_LABEL_COLOR);
        chapterLabel->setOpacity(videoStoryDefs::CHAPTER_LABEL_OPACITY);
        cover->addChild(chapterLabel);
    }
}

#pragma mark - Callbacks

void VideoStory::onPlayButtonPressed()
{
    if (true == this->isPlayerInVideoArea())
    {
        this->playVideo();
    }
}

void VideoStory::onLockButtonPressed()
{
    if (true == this->isPlayerInVideoArea())
    {
        auto node = static_cast<Node*>(this);
        node->setName(callbackDefs::VIDEO_LOCKED);
        
        if (_callback && _callbackListener)
        {
            (_callbackListener->*_callback)(node);
        }
    }
}

void VideoStory::onVideoEnded()
{
    this->stopVideo();
}

#pragma mark - Update Methods

void VideoStory::update(float dt)
{
    this->updatePlayBack(dt);
    this->updateSubtitles(dt);
    this->updatePlayButton(dt);
    this->updateSoundEffects(dt);
}

void VideoStory::updatePlayBack(float dt)
{
    if (false == this->isPlayerInVideoArea())
    {
        this->stopVideo();
    }
}

void VideoStory::updateSubtitles(float dt)
{
    if (_subtitleLabel)
    {
        if (_audio && _audio->isPlaying())
        {
            if (_subIndex < _subtitles.size())
            {
                float currentAudioTime = _audio->getTimelinePosition();
                Subtitle * subtitle = _subtitles.at(_subIndex).get();
                if (subtitle->time < currentAudioTime)
                {
                    if (_subIndex == 0)
                    {
                        this->startSubtitles();
                    }
                    
                    _subtitleLabel->setString(subtitle->text);
                    _subIndex++;
                    
                    if (_subIndex == _subtitles.size())
                    {
                        this->stopSubtitles(true);
                    }
                }
                else if (subtitle->duration != -1 && subtitle->time + subtitle->duration < currentAudioTime)
                {
                    _subtitleLabel->setString("");
                }
            }
        }
        else
        {
            _subIndex = 0;
            
            this->stopSubtitles(false);
        }
    }
}

void VideoStory::updatePlayButton(float dt)
{
    if (_playButton)
    {
        if (this->isPlayerInVideoArea())
        {
            _playButton->setTouchEnabled(true);
        }
        else
        {
            _playButton->setTouchEnabled(false);
        }
    }
}

void VideoStory::updateSoundEffects(float dt)
{
    if (_playbackAction && _playbackAction->getCurrentFrame() != _currentAudioFrame)
    {
        _currentAudioFrame = _playbackAction->getCurrentFrame();
        if (_soundVideo && _updateSoundEffects)
        {
            _soundVideo->playForFrame(_currentAudioFrame);
        }
    }
}

void VideoStory::playVideo()
{
    static const int transitionTag = static_cast<int>(eAction::FADE);
    static const GLubyte destinationCoverOpacity = 0;
    
    if (_playButton)
    {
        _playButton->setTouchEnabled(false);
    }
    
    Node * cover = _video->getChildByName(videoStoryDefs::COVER_STUDIO_NAME);
    Sequence * transitionSequence = dynamic_cast<Sequence *>(cover->getActionByTag(transitionTag));
    if (cover && nullptr == transitionSequence && _playbackAction && false == _playbackAction->isPlaying())
    {
        auto node = static_cast<Node*>(this);
        node->setName(callbackDefs::VIDEO_PLAY);
        
        if (_callback && _callbackListener)
        {
            (_callbackListener->*_callback)(node);
        }
        
        _playbackAction->setCurrentFrame(0);
        FadeTo * fadeCover = FadeTo::create(videoStoryDefs::VIDEO_FADE_TIME, destinationCoverOpacity);
        CallFunc * callback = CallFunc::create([this](){
            _playbackAction->gotoFrameAndPlay(0, false);
            if (_audio)
            {
                _audio->startPlay();
            }
            _updateSoundEffects = true;
        });
        transitionSequence = Sequence::create(fadeCover, callback, NULL);
        transitionSequence->setTag(transitionTag);
        cover->runAction(transitionSequence);
    }
}

void VideoStory::stopVideo()
{
    static const int transitionTag = static_cast<int>(eAction::FADE);
    static const GLubyte destinationCoverOpacity = 255;
    
    Node * cover = _video->getChildByName(videoStoryDefs::COVER_STUDIO_NAME);
    Sequence * transitionSequence = dynamic_cast<Sequence *>(cover->getActionByTag(transitionTag));
    if (cover && nullptr == transitionSequence && _playbackAction && true == _playbackAction->isPlaying())
    {
        _updateSoundEffects = false;
        if (_soundVideo)
        {
            _soundVideo->fadeOutAllEffects();
        }
        if (_audio)
        {
            _audio->stopPlay(true);
        }
        
        FadeTo * fadeCover = FadeTo::create(videoStoryDefs::VIDEO_FADE_TIME, destinationCoverOpacity);
        CallFunc * callback = CallFunc::create([this](){
            _playbackAction->gotoFrameAndPause(0);
            
            if (_playButton)
            {
                _playButton->setTouchEnabled(true);
            }
            
            auto node = static_cast<Node*>(this);
            node->setName(callbackDefs::VIDEO_STOP);
            
            if (_callback && _callbackListener)
            {
                (_callbackListener->*_callback)(node);
            }
        });
        transitionSequence = Sequence::create(fadeCover, callback, NULL);
        transitionSequence->setTag(transitionTag);
        cover->runAction(transitionSequence);
    }
}

void VideoStory::startSubtitles()
{
    static const int transitionTag = static_cast<int>(eAction::FADE);
    static const GLubyte destinationOpacity = 255;
    
    if (_subtitleLabel)
    {
        Sequence * transitionSequence = dynamic_cast<Sequence *>(_subtitleLabel->getActionByTag(transitionTag));
        GLubyte currentOpacity = _subtitleLabel->getOpacity();
        if ( nullptr == transitionSequence && currentOpacity < destinationOpacity )
        {
            _subtitleLabel->setString("");
            FadeTo * fadeSubtitle = FadeTo::create(videoStoryDefs::SUBTITLES_FADE_IN_TIME, destinationOpacity);
            transitionSequence = Sequence::create(fadeSubtitle, NULL);
            transitionSequence->setTag(transitionTag);
            _subtitleLabel->runAction(transitionSequence);
        }
    }
}

void VideoStory::stopSubtitles(bool needDelay)
{
    static const int transitionTag = static_cast<int>(eAction::FADE);
    static const GLubyte destinationOpacity = 0;
    
    if (_subtitleLabel)
    {
        Sequence * transitionSequence = dynamic_cast<Sequence *>(_subtitleLabel->getActionByTag(transitionTag));
        GLubyte currentOpacity = _subtitleLabel->getOpacity();
        if ( nullptr == transitionSequence && currentOpacity > destinationOpacity )
        {
            DelayTime * delay = DelayTime::create((needDelay) ? videoStoryDefs::SUBTITLES_DELAY_FADE_OUT_TIME : 0.0f);
            FadeTo * fadeSubtitle = FadeTo::create(videoStoryDefs::SUBTITLES_FADE_OUT_TIME, destinationOpacity);
            transitionSequence = Sequence::create(delay, fadeSubtitle, NULL);
            transitionSequence->setTag(transitionTag);
            _subtitleLabel->runAction(transitionSequence);
        }
    }
}

#pragma mark - Utilities

bool VideoStory::isPlayerInVideoArea()
{
    bool playerIn = false;
    Node * playbackArea = this->getChildNodeWithName(videoStoryDefs::PLAYBACK_AREA_NAME);
    if (playbackArea && _player)
    {
        Point screenPlayerPosition = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Rect screenPlaybackArea = Utility::convertRectToWorldSpace(playbackArea->getParent(), playbackArea->getBoundingBox());
        if (screenPlaybackArea.containsPoint(screenPlayerPosition))
        {
            playerIn = true;
        }
    }
    return playerIn;
}

bool VideoStory::isVideoUnlocked(int world)
{
    // Solo il filmato del mondo bonus può essere bloccato, e ciò avviene solo se non sono state prese tutte le pecore
    return ( world != appParams::BONUS_WORLD || ScoreManager::getInstance()->isAllSheepCaught() );
}
