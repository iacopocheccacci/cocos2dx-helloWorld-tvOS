//
//  SoundFrame.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 30/07/15.
//
//

#include "SoundVideo.h"
#include "SoundManager.h"

SoundVideo * SoundVideo::createFromWorld(int world)
{
    SoundVideo * ret = new (std::nothrow) SoundVideo();
    if (ret != nullptr && ret->init(world))
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

bool SoundVideo::init(int world)
{
    bool res = Node::init();
    if (true == res)
    {
        std::vector<std::string> audioFileNames = SoundManager::getInstance()->getSoundEventsForVideo(world);
        
        for (std::string audioFileName : audioFileNames)
        {
            std::string frameStr = audioFileName.substr(audioFileName.find_last_of("_")+1, audioFileName.length());
            int frame;
            istringstream ( frameStr ) >> frame;
            //int frame = atoi(frameStr.c_str());
            //int frame = std::stoi(frameStr);
            
            SoundComponent * soundComponent = SoundComponent::create(audioFileName);
            this->addChild(soundComponent);
            _effectsByFrame[frame] = soundComponent;
        }
    }
    
    return res;
}

void SoundVideo::playForFrame(int frame)
{
    auto iter = _effectsByFrame.find(frame);
    if ( _effectsByFrame.end() != iter )
    {
        (*iter).second->startPlay();
    }
}

void SoundVideo::fadeOutAllEffects()
{
    for (auto iter = _effectsByFrame.begin(); iter != _effectsByFrame.end(); iter++)
    {
        (*iter).second->stopPlay(true);
    }
}

void SoundVideo::pauseAllEffects()
{
    for (auto iter = _effectsByFrame.begin(); iter != _effectsByFrame.end(); iter++)
    {
        (*iter).second->pauseSound();
    }
}

void SoundVideo::resumeAllEffects()
{
    for (auto iter = _effectsByFrame.begin(); iter != _effectsByFrame.end(); iter++)
    {
        (*iter).second->resumeSound();
    }
}
