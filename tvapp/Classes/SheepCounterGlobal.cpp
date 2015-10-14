//
//  SheepCounter.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 16/02/15.
//
//

#include "SheepCounterGlobal.h"
#include "VisibleRect.h"

const float SheepCounterGlobal::PADDING_TOP = 20.0f;
const float SheepCounterGlobal::PADDING_RIGHT = 20.0f;

SheepCounterGlobal::SheepCounterGlobal()
{
    
}

SheepCounterGlobal::~SheepCounterGlobal()
{
    this->undoScoreManagerRegistration();
    this->removeAllChildrenWithCleanup(true);
}

bool SheepCounterGlobal::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    _world = -1;
    
    this->initSprite();
    this->initCounter();
    this->alignElements();
    this->initScoreManagerRegistration();
    this->setCascadeOpacityEnabled(true);
    
    return true;
}

void SheepCounterGlobal::initSprite()
{
    _iconSprite = Sprite::create(sheepCounterGlobalDefs::ICON_SPRITE_PATH);
    _iconSprite->setAnchorPoint(Point::ANCHOR_MIDDLE);
    this->addChild(_iconSprite);
}

void SheepCounterGlobal::initCounter()
{
    _counterLabel = Label::createWithBMFont(sheepCounterGlobalDefs::COUNTER_FONT_PATH, "00");
    _counterLabel->setAnchorPoint(Point::ANCHOR_MIDDLE);
    _counterLabel->setScale(0.5f);
    this->addChild(_counterLabel);
    
    this->refreshSheepCounter();
}

void SheepCounterGlobal::alignElements()
{
    _counterLabel->setPosition(VisibleRect::top() - Point(-20.0f, 45));
    _iconSprite->setPosition(_counterLabel->getPosition() - Point(10.f + _counterLabel->getContentSize().width * _counterLabel->getScaleX(), -5));
}

#pragma mark - Observer Pattern

void SheepCounterGlobal::initScoreManagerRegistration()
{
    ScoreManager::getInstance()->addObserver(this);
}

void SheepCounterGlobal::undoScoreManagerRegistration()
{
    ScoreManager::getInstance()->removeObserver(this);
}

void SheepCounterGlobal::onScoreUpdated()
{
    this->refreshSheepCounter();
}

void SheepCounterGlobal::refreshSheepCounter()
{
    this->refreshSheepCounterForAllWorlds();
}

void SheepCounterGlobal::refreshSheepCounterForAllWorlds()
{
    int totalWorldsScore = 0;
    for (int world = 1; world < 6; world++)
    {
        totalWorldsScore += ScoreManager::getInstance()->getScoreForWorld(world);
    }
    char totalWorldsScoreStr[10];
    snprintf(totalWorldsScoreStr, 10, "%3d", totalWorldsScore);
    _counterLabel->setString(totalWorldsScoreStr);
    this->alignElements();
}

void SheepCounterGlobal::refreshSheepCounterForCurrentWorld()
{
    if (_world != -1)
    {
        int worldScore = ScoreManager::getInstance()->getScoreForWorld(_world);
        char worldScoreStr[10];
        snprintf(worldScoreStr, 10, "%3d", worldScore);
        _counterLabel->setString(worldScoreStr);
        this->alignElements();
    }
}

#pragma mark - Metodi Pubblici

void SheepCounterGlobal::initWorld(int world)
{
    _world = world;
    this->refreshSheepCounter();
}

//void SheepCounterGlobal::setDefaultPosition()
//{
////    float paddingRight = SheepCounterGlobal::PADDING_RIGHT + _iconSprite->getContentSize().width * 3.0f / 2.0f;
//    float paddingTop = SheepCounterGlobal::PADDING_TOP + _iconSprite->getContentSize().height / 2.0f;
//    
//    Point defaultPosition = Point(VisibleRect::top() - Point(0.0f, paddingTop));
////    Point defaultPosition = Point(VisibleRect::rightTop() - Point(paddingRight, paddingTop));
//    
////    this->setPosition(defaultPosition);
//}