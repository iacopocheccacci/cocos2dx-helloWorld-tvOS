//
//  SheepShow.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 03/09/15.
//
//

#include "SheepShow.h"
#include "SoundComponent.h"
#include "ScoreManager.h"

bool SheepShow::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _soundShow = nullptr;
        _creditsBodyNode = nullptr;
        _creditsFooterNode = nullptr;
        _creditsBodyStartPosition = Point::ZERO;
        _creditsFooterStartPosition = Point::ZERO;
        
        this->initAudio();
        this->initComponents();
    }
    
    return status;
}

void SheepShow::initAudio()
{
    _soundShow = SoundComponent::create(SoundComponent::SOUND_SHEEP_SHOW);
    this->addChild(_soundShow);
}

void SheepShow::initComponents()
{
    int flutes = ScoreManager::getInstance()->getFluteTakenCount();
    Node * component = nullptr;
    auto componentNameIter = sheepShowDefs::COMPONENT_ORDER.begin();
    
    while (sheepShowDefs::COMPONENT_ORDER.end() != componentNameIter)
    {
        component = this->getChildNodeWithName(*componentNameIter);
        if (component)
        {
            component->setVisible(false);
        }
        componentNameIter++;
    }
    
    componentNameIter = sheepShowDefs::COMPONENT_ORDER.begin();
    
    for (int i = 0; i < flutes; i++)
    {
        if (sheepShowDefs::COMPONENT_ORDER.end() != componentNameIter)
        {
            component = this->getChildNodeWithName(*componentNameIter);
            if (component)
            {
                component->setVisible(true);
            }
        }
        componentNameIter++;
    }
}

void SheepShow::initCredits(LHScene * rootScene)
{
    Node * creditsBodyNode = rootScene->getChildNodeWithName(sheepShowDefs::CREDITS_BODY_NODE_NAME);
    if (creditsBodyNode)
    {
        _creditsBodyNode = creditsBodyNode;
        _creditsBodyStartPosition = creditsBodyNode->getPosition();
        _creditsBodyNode->setCascadeOpacityEnabled(true);
    }
    Node * creditsFooterNode = rootScene->getChildNodeWithName(sheepShowDefs::CREDITS_FOOTER_NODE_NAME);
    if (creditsFooterNode)
    {
        _creditsFooterNode = creditsFooterNode;
        _creditsFooterStartPosition = creditsFooterNode->getPosition();
        _creditsFooterNode->setCascadeOpacityEnabled(true);
    }
}

#pragma mark - Play Methods

void SheepShow::setPlayState(bool playing)
{
    if (playing)
    {
        this->schedule(schedule_selector(SheepShow::startPlay), sheepShowDefs::MUSIC_DURATION, CC_REPEAT_FOREVER, sheepShowDefs::DELAY_TO_PLAY);
        this->scheduleOnce(schedule_selector(SheepShow::startCredits), sheepShowDefs::DELAY_TO_PLAY);
    }
    else
    {
        this->unschedule(schedule_selector(SheepShow::startPlay));
        this->unschedule(schedule_selector(SheepShow::startCredits));
        
        this->stopPlay();
        this->stopCredits();
    }
}

void SheepShow::setPauseState(bool pause)
{
    if (_soundShow)
    {
        if (pause)
        {
            _soundShow->pauseSound();
        }
        else
        {
            _soundShow->resumeSound();
        }
    }
}

void SheepShow::startPlay(float dt)
{
    if (_soundShow)
    {
        _soundShow->stopPlay();
        _soundShow->startPlay();
    }
    this->playFlutesLeft();
    this->playFlutesRight();
    this->playChorus();
    this->playSoloSheeps();
    this->playSoloWolf();
}

void SheepShow::stopPlay()
{
    if (_soundShow)
    {
        _soundShow->stopPlay(true);
    }
    Node * component = nullptr;
    for (auto iter = sheepShowDefs::COMPONENT_ORDER.begin(); iter != sheepShowDefs::COMPONENT_ORDER.end(); iter++)
    {
        component = this->getChildNodeWithName(*iter);
        for (Node * member : component->getChildren())
        {
            this->stopPlayMemberAnimation(member);
        }
    }
}

void SheepShow::playFlutesLeft()
{
    Node * component = this->getChildNodeWithName(sheepShowDefs::SHOW_FLUTES_LEFT_NODE_NAME);
    if (component)
    {
        for (Node * member : component->getChildren())
        {
            this->startSchedulePlayForMember(member, sheepShowDefs::SCHEDULE_FLUTE_LEFT);
        }
        this->setSoundForComponent(component);
    }
}

void SheepShow::playFlutesRight()
{
    Node * component = this->getChildNodeWithName(sheepShowDefs::SHOW_FLUTES_RIGHT_NODE_NAME);
    if (component)
    {
        for (Node * member : component->getChildren())
        {
            this->startSchedulePlayForMember(member, sheepShowDefs::SCHEDULE_FLUTE_RIGHT);
        }
        this->setSoundForComponent(component);
    }
}


void SheepShow::playChorus()
{
    Node * component = this->getChildNodeWithName(sheepShowDefs::SHOW_CHORUS_NODE_NAME);
    if (component)
    {
        for (Node * member : component->getChildren())
        {
            this->startSchedulePlayForMember(member, sheepShowDefs::SCHEDULE_CHORUS);
        }
        this->setSoundForComponent(component);
    }
}

void SheepShow::playSoloWolf()
{
    Node * component = this->getChildNodeWithName(sheepShowDefs::SHOW_SOLO_WOLF_NODE_NAME);
    if (component)
    {
        for (Node * member : component->getChildren())
        {
            this->startSchedulePlayForMember(member, sheepShowDefs::SCHEDULE_SOLO_WOLF);
        }
        this->setSoundForComponent(component);
    }
}


void SheepShow::playSoloSheeps()
{
    Node * component = this->getChildNodeWithName(sheepShowDefs::SHOW_SOLO_SHEEP_NODE_NAME);
    if (component)
    {
        for (Node * member : component->getChildren())
        {
            this->startSchedulePlayForMember(member, sheepShowDefs::SCHEDULE_SOLO_SHEEPS);
        }
        this->setSoundForComponent(component);
    }
}

#pragma mark - Credits

void SheepShow::startCredits(float dt)
{
    if (_creditsBodyNode)
    {
        _creditsBodyNode->stopAllActions();
        _creditsBodyNode->setOpacity(255);
        _creditsBodyNode->setPosition(_creditsBodyStartPosition);
        _creditsBodyNode->runAction(MoveBy::create(sheepShowDefs::MUSIC_DURATION * 2.0f, Point(0.0f, sheepShowDefs::CREDITS_MOVE_Y * 2.0f)));
    }
    if (_creditsFooterNode)
    {
        _creditsFooterNode->stopAllActions();
        _creditsFooterNode->setOpacity(255);
        _creditsFooterNode->setPosition(_creditsFooterStartPosition);
        _creditsFooterNode->runAction(MoveBy::create(sheepShowDefs::MUSIC_DURATION, Point(0.0f, sheepShowDefs::CREDITS_MOVE_Y)));
    }
}

void SheepShow::stopCredits()
{
    if (_creditsBodyNode)
    {
        _creditsBodyNode->runAction(FadeOut::create(0.5f));
    }
    if (_creditsFooterNode)
    {
        _creditsFooterNode->runAction(FadeOut::create(0.5f));
    }
}

#pragma mark - Utilities

void SheepShow::startSchedulePlayForMember(Node * member, std::map<float, std::string> schedulePattern)
{
    DelayTime * delayAction = nullptr;
    CallFunc * callback = nullptr;
    float delay = 0.0f;
    std::string animName = "";
    for(auto iter = schedulePattern.begin(); iter != schedulePattern.end(); iter++)
    {
        delay = iter->first;
        animName = iter->second;
        delayAction = DelayTime::create(delay);
        callback = CallFunc::create([this, member, animName](){
            this->startPlayMemberAnimation(member, animName);
        });
        member->runAction(Sequence::create(delayAction, callback, NULL));
    }
}

void SheepShow::startPlayMemberAnimation(Node * member, std::string animName)
{
    LHAnimation * animation = nullptr;
    LHSprite * memberSprite = dynamic_cast<LHSprite*>(member);
    if (memberSprite)
    {
        animation = memberSprite->getAnimationWithName(animName);
        if (animation)
        {
            animation->setActive(true);
            animation->restart();
        }
    }
}

void SheepShow::stopPlayMemberAnimation(Node * member)
{
    LHSprite * memberSprite = dynamic_cast<LHSprite*>(member);
    if (memberSprite)
    {
        memberSprite->stopAllActions();
        
        auto animation = memberSprite->getAnimationWithName(sheepShowDefs::SHOW_IDLE_ANIMATION_NAME);
        if (animation)
        {
            animation->setActive(true);
            animation->restart();
        }
    }
}

void SheepShow::setSoundForComponent(Node * component)
{
    if (_soundShow)
    {
        LHNode * node = dynamic_cast<LHNode*>(component);
        if (node)
        {
            std::string paramName = sheepShowDefs::TRACK_PARAMS.at(node->getName());
            bool value = (component->isVisible()) ? sheepShowDefs::PARAMS_ON_VALUE : sheepShowDefs::PARAMS_OFF_VALUE;
            _soundShow->updateParameter(paramName.c_str(), value);
        }
    }
}
