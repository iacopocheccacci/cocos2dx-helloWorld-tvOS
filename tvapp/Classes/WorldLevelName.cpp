//
//  WorldLevelName.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/02/15.
//
//

#include "WorldLevelName.h"
#include "VisibleRect.h"
#include "GameLogic.h"
#include "Utility.h"
#include "HUD.h"
#include "NavigationMenu.h"

namespace levelNameDefs
{
    static const char* BACKGROUND_SPRITE = "UI/hudBg_sx.png";
}

WorldLevelName::~WorldLevelName()
{
    this->removeAllChildrenWithCleanup(true);
}

bool WorldLevelName::init()
{
    bool initDone = false;
    
    if (Layer::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void WorldLevelName::initLabel(std::string levelName)
{
    Sprite * background = Sprite::create(levelNameDefs::BACKGROUND_SPRITE);
    background->setPosition(VisibleRect::leftTop() -  Point(0.0f, navigationMenuDefs::OFFSET_BACKGROUND_Y));
    background->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
    this->addChild(background);
    
    int level = Utility::getLevelFromLevelName(levelName);
    
    char levelStr[25];
    snprintf(levelStr, 25, "LEVEL %d", level);
    auto levelLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, levelStr, TextHAlignment::CENTER);
    levelLabel->setScale(0.5f);
    levelLabel->setColor(appParams::FONT_ORANGE);
    levelLabel->setOpacity(200);
    levelLabel->setNormalizedPosition(Point(0.5f, navigationMenuDefs::MENU_NORMALIZED_POSITION.y));
    background->addChild(levelLabel);
}