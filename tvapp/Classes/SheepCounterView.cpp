//
//  SheepCounterView.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/11/14.
//
//

#include "SheepCounterView.h"
#include "VisibleRect.h"

SheepCounterView::~SheepCounterView()
{
    this->removeAllChildrenWithCleanup(true);
}

bool SheepCounterView::init()
{
    bool initDone = false;
    
    if (Layer::init())
    {
        _sheepCount = 0;
        _killIndex = 0;
        initDone = true;
    }
    
    return initDone;
}

void SheepCounterView::initCounter(int count, bool flute)
{
    if (flute)
    {
        auto fluteSprite = Sprite::create(sheepCounterViewDefs::FLUTE_DEACT_SPRITE);
        fluteSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        this->addChild(fluteSprite, 1, 0);
    }
    
    for (int i = 1; i <= count; i++)
    {
        auto sheepSprite = Sprite::create(sheepCounterViewDefs::SHEEP_DEACT_SPRITE);
        sheepSprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
        this->addChild(sheepSprite, 1, ++_sheepCount);
    }
    
    this->alignItemsWithPadding(-20);
}

void SheepCounterView::decrementCounter()
{
    auto sheepSprite = dynamic_cast<Sprite*>(this->getChildByTag(_sheepCount));
    
    if (sheepSprite)
    {
        _sheepCount--;
        sheepSprite->setTexture(sheepCounterViewDefs::SHEEP_ACTIVE_SPRITE);
    }
}

void SheepCounterView::killSheep()
{
    auto sheepSprite = dynamic_cast<Sprite*>(this->getChildByTag(_killIndex));
    
    if (sheepSprite)
    {
        _killIndex++;
        sheepSprite->setTexture(sheepCounterViewDefs::SHEEP_DEAD_SPRITE);
    }
}

void SheepCounterView::activeFlute()
{
    auto fluteSprite = dynamic_cast<Sprite*>(this->getChildByTag(0));
    
    if (fluteSprite)
    {
        fluteSprite->setTexture(sheepCounterViewDefs::FLUTE_ACTIVE_SPRITE);
    }
}

#pragma mark - Private Methods

void SheepCounterView::alignItemsWithPadding(float padding)
{
    Vec2 currentPosition;

#ifdef EDITOR_MODE
    
    currentPosition = Vec2(VisibleRect::rightTop().x, VisibleRect::rightTop().y - 100);
    
#elif CC_TARGET_PLATFORM==CC_PLATFORM_TVOS
    currentPosition = Vec2(VisibleRect::rightTop().x - 50, VisibleRect::rightTop().y - 10);
#else
    currentPosition = Vec2(VisibleRect::center().x, VisibleRect::rightTop().y - 10);
#endif
    
    this->setPosition(currentPosition);
    
    float width = -padding;
    for(const auto &child : _children)
        width += child->getContentSize().width * child->getScaleX() + padding;

#if CC_TARGET_PLATFORM==CC_PLATFORM_TVOS
    float x = -width;
    
    for(const auto &child : _children)
    {
        child->setPosition(x + child->getContentSize().width * child->getScaleX(), 0);
        child->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
        x += child->getContentSize().width * child->getScaleX() + padding;
    }

#else
    float x = -width / 2.0f;
    
    for(const auto &child : _children)
    {
        child->setPosition(x + child->getContentSize().width * child->getScaleX() / 2.0f, 0);
        child->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
        x += child->getContentSize().width * child->getScaleX() + padding;
    }

#endif
    
}

