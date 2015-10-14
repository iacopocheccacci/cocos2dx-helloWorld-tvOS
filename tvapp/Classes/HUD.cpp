//
//  HUD.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#include "HUD.h"
#include "NavigationMenu.h"
#include "NoteProgressBar.h"
#include "SheepCounterView.h"
#include "WorldLevelName.h"
#include "SheepCounterGlobal.h"
#include "Definitions.h"
#include "VisibleRect.h"
#include "SettingsManager.h"
#include "Flute.h"
#include "Utility.h"

enum class eHudChild
{
    NAVIGATION_MENU = 0,
    SHEEP_COUNTER,
    WORLD_LEVEL_NAME,
    SHEEP_COUNTER_GLOBAL,
    FLUTE,
    FLUTE_FEEDBACK
};

HUD::~HUD()
{
    removeAllChildrenWithCleanup(true);
}

HUD * HUD::create(eLevelType levelType, Node * player)
{
    HUD * layer = new (std::nothrow) HUD();
    if(layer && layer->init(levelType, player))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;

}

bool HUD::init(eLevelType levelType, Node * player)
{
    bool initDone = false;
    
    if (Layer::init())
    {
        auto navigationMenu = NavigationMenu::create(levelType, player);
        this->addChild(navigationMenu, 1, static_cast<int>(eHudChild::NAVIGATION_MENU));
        
        auto sheepCounter = SheepCounterView::create();
        this->addChild(sheepCounter, 1, static_cast<int>(eHudChild::SHEEP_COUNTER));
        
        if (eLevelType::NORMAL == levelType)
        {
            auto worldLevelName = WorldLevelName::create();
            this->addChild(worldLevelName, 1, static_cast<int>(eHudChild::WORLD_LEVEL_NAME));
        }
        
        // Contatore di pecore prese nel gioco (totali)
        if (eLevelType::MAIN_MENU == levelType || eLevelType::LEVEL_SELECTION == levelType)
        {
//            auto sheepCounterGlobal = SheepCounterGlobal::create();
//            this->addChild(sheepCounterGlobal, 1, static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL));
        }
        
        if (eLevelType::MAIN_MENU == levelType)
        {
#if COCOS2D_DEBUG == 1
            this->initResetGlobal();
//            this->initResetButton();
//            this->initResetTutorialMoveButton();
//            this->initResetLevelsButton();
//            this->initResetFlutesButton();
#endif
        }
        
        initDone = true;
    }
    
    this->setCascadeOpacityEnabled(true);
    
    return initDone;
}


void HUD::pauseNavigationMenuListener()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    
    if (nav)
    {
    	nav->disableMenu();
    	/*
        auto array = nav->getChildren();
        for(Node* child : array)
        {
            _eventDispatcher->pauseEventListenersForTarget(child);
        }
        */
    }
}

void HUD::resumeNavigationMenuListenser()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    
    if (nav)
    {
    	nav->enableMenu();
        /*
        auto array = nav->getChildren();
        for(Node* child : array)
        {
            _eventDispatcher->resumeEventListenersForTarget(child);
        }
        */
    }
}

void HUD::initSheepCounter(int count, bool flute)
{
    auto sheepCounter = dynamic_cast<SheepCounterView*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER)));
    
    if (sheepCounter)
    {
        sheepCounter->initCounter(count, flute);
    }
}

void HUD::decrementSheepCounter()
{
    auto sheepCounter = dynamic_cast<SheepCounterView*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER)));
    
    if (sheepCounter)
    {
        sheepCounter->decrementCounter();
    }
}

void HUD::killInSheepCounter()
{
    auto sheepCounter = dynamic_cast<SheepCounterView*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER)));
    
    if (sheepCounter)
    {
        sheepCounter->killSheep();
    }
}

void HUD::activeFlute()
{
    auto sheepCounter = dynamic_cast<SheepCounterView*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER)));
    
    if (sheepCounter)
    {
        sheepCounter->activeFlute();
    }
}

void HUD::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        nav->setCallbackFunc(target, callback);
    }
}

void HUD::initWorldLevelLabel(std::string levelName)
{
    auto label = dynamic_cast<WorldLevelName *>(this->getChildByTag(static_cast<int>(eHudChild::WORLD_LEVEL_NAME)));
    if (label)
    {
        label->initLabel(levelName);
    }
}

void HUD::initSheepCounterGlobal(int world)
{
    auto sheepCounterGlobal = dynamic_cast<SheepCounterGlobal*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL)));
    if (sheepCounterGlobal)
    {
//        sheepCounterGlobal->setDefaultPosition();
        sheepCounterGlobal->initWorld(world);
    }
}

void HUD::initResetGlobal()
{
    auto resetBtn = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "RESET", TextHAlignment::CENTER);
    
    // Issue #9582 - Scale non corretto con BMFontLabel in caso di anchor point middle
    //    resetBtn->setScale(0.5f);
    
    MenuItemLabel * item = MenuItemLabel::create(resetBtn, [](Ref * sender)
    {
        ScoreManager::getInstance()->resetScoreForAllWorlds();
        SettingsManager::getInstance()->saveMoveTutorialDone(false);
        ScoreManager::getInstance()->resetBestCompletedLevel();
        ScoreManager::getInstance()->resetTakenFlutes();
    });
    
    Point position = Point(VisibleRect::leftTop()+ Point(46.0f, - resetBtn->getContentSize().height / 2.0f));
    item->setPosition(position);
    
    // Workaround - Issue #9582
    item->setScale(0.5f);
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void HUD::initResetButton()
{
    auto resetBtn = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "RESET SCORE", TextHAlignment::CENTER);
    
    // Issue #9582 - Scale non corretto con BMFontLabel in caso di anchor point middle
//    resetBtn->setScale(0.5f);
    
    MenuItemLabel * item = MenuItemLabel::create(resetBtn, [](Ref * sender) {
        ScoreManager::getInstance()->resetScoreForAllWorlds();
    });
    
    Point position = Point(VisibleRect::top() + Point(- 200.0f, - resetBtn->getContentSize().height / 2.0f));
    item->setPosition(position);
    
    // Workaround - Issue #9582
    item->setScale(0.5f);
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void HUD::initResetTutorialMoveButton()
{
    auto resetBtn = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "RESET TUTORIAL", TextHAlignment::CENTER);
    
    // Issue #9582 - Scale non corretto con BMFontLabel in caso di anchor point middle
//    resetBtn->setScale(0.5f);
    
    MenuItemLabel * item = MenuItemLabel::create(resetBtn, [](Ref * sender) {
        SettingsManager::getInstance()->saveMoveTutorialDone(false);
    });
    
    Point position = Point(VisibleRect::top() + Point(+ 100.0f, - resetBtn->getContentSize().height / 2.0f));
    item->setPosition(position);
    
    // Workaround - Issue #9582
    item->setScale(0.5f);
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void HUD::initResetLevelsButton()
{
    auto resetBtn = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "RESET LEVELS", TextHAlignment::CENTER);
    
    // Issue #9582 - Scale non corretto con BMFontLabel in caso di anchor point middle
    //    resetBtn->setScale(0.5f);
    
    MenuItemLabel * item = MenuItemLabel::create(resetBtn, [](Ref * sender) {
        ScoreManager::getInstance()->resetBestCompletedLevel();
    });
    
    Point position = Point(VisibleRect::bottom() + Point(+ 200.0f, + resetBtn->getContentSize().height / 2.0f));
    item->setPosition(position);
    
    // Workaround - Issue #9582
    item->setScale(0.5f);
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void HUD::initResetFlutesButton()
{
    auto resetBtn = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "RESET FLUTES", TextHAlignment::CENTER);
    
    // Issue #9582 - Scale non corretto con BMFontLabel in caso di anchor point middle
    //    resetBtn->setScale(0.5f);
    
    MenuItemLabel * item = MenuItemLabel::create(resetBtn, [](Ref * sender) {
        ScoreManager::getInstance()->resetTakenFlutes();
    });
    
    Point position = Point(VisibleRect::bottom() + Point(- 80.0f, + resetBtn->getContentSize().height / 2.0f));
    item->setPosition(position);
    
    // Workaround - Issue #9582
    item->setScale(0.5f);
    
    auto menu = Menu::create(item, NULL);
    menu->setPosition(Point::ZERO);
    this->addChild(menu);
}

void HUD::showStopBtn()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        nav->showStopNoteButton();
    }

}

void HUD::hideStopBtn()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        nav->hideStopNoteButton();
    }

}

void HUD::updateStopBtnInputType(eInputType inputType)
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        nav->updateStopNoteButtonInputType(inputType);
    }
}

void HUD::updateStopBtnEffect(eNoteEffect noteEffect)
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        nav->updateStopNoteButtonEffect(noteEffect);
    }
}

void HUD::showFluteFeedback()
{
    static const char* fluteSpriteFrameName = "flautoFeedback";
    
    Sprite * fluteSprite = Sprite::createWithSpriteFrameName(fluteSpriteFrameName);
    if (fluteSprite)
    {
        fluteSprite->setPosition(VisibleRect::center());
        fluteSprite->setScale(0.0f);
        fluteSprite->setOpacity(0);
        this->addChild(fluteSprite, 2, static_cast<int>(eHudChild::FLUTE_FEEDBACK));
        auto scale = EaseBackOut::create(ScaleTo::create(0.5f, 1.0f));
        auto fade = FadeIn::create(0.5f);
        fluteSprite->runAction(scale);
        fluteSprite->runAction(fade);
        
        ParticleSystemQuad * emitterBG = ParticleSystemQuad::create(fluteDefs::PARTICLE_FILE_BG);
//        emitterBG->setScale(.7f);
        emitterBG->setPosition(Point(fluteSprite->getContentSize()/2.0f));
        emitterBG->setPositionType(ParticleSystem::PositionType::GROUPED);
        emitterBG->setAutoRemoveOnFinish(false);
        fluteSprite->addChild(emitterBG,-2);
    }
}

void HUD::hideFluteFeedback()
{
    ParticleSystemQuad * emitterBG = nullptr;
    Node * fluteSprite = this->getChildByTag(static_cast<int>(eHudChild::FLUTE_FEEDBACK));
    if (fluteSprite)
    {
        fluteSprite->stopAllActions();
        auto scale = EaseBackIn::create(ScaleTo::create(0.5f, 0.0f));
        auto fade = FadeOut::create(0.5f);
        fluteSprite->runAction(scale);
        fluteSprite->runAction(fade);
        
        for (Node * child : fluteSprite->getChildren())
        {
            emitterBG = dynamic_cast<ParticleSystemQuad*>(child);
            if (emitterBG)
            {
                emitterBG->stopSystem();
            }
        }
    }
}

void HUD::hideElements()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        Utility::fadeOutNodeWithTime(nav, 1.f);
        nav->disableMenu();
    }
    auto counter = dynamic_cast<SheepCounterGlobal*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL)));
    if (counter)
    {
        Utility::fadeOutNodeWithTime(counter, 1.f);
    }
    auto levelLabel = dynamic_cast<WorldLevelName*>(this->getChildByTag(static_cast<int>(eHudChild::WORLD_LEVEL_NAME)));
    if (levelLabel)
    {
        Utility::fadeOutNodeWithTime(levelLabel, 1.f);
    }
}

void HUD::showElements()
{
    auto nav = dynamic_cast<NavigationMenu*>(this->getChildByTag(static_cast<int>(eHudChild::NAVIGATION_MENU)));
    if (nav)
    {
        Utility::fadeInNodeWithTime(nav, 1.f);
        nav->enableMenu();
    }
    auto counter = dynamic_cast<SheepCounterGlobal*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL)));
    if (counter)
    {
        Utility::fadeInNodeWithTime(counter, 1.f);
    }
    auto levelLabel = dynamic_cast<WorldLevelName*>(this->getChildByTag(static_cast<int>(eHudChild::WORLD_LEVEL_NAME)));
    if (levelLabel)
    {
        Utility::fadeInNodeWithTime(levelLabel, 1.f);
    }
}

void HUD::hideGlobalCounter()
{
    auto sheepCounterGlobal = dynamic_cast<SheepCounterGlobal*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL)));
    if (sheepCounterGlobal)
    {
        Utility::fadeOutNodeWithTime(sheepCounterGlobal, 1.0f);
    }
}

void HUD::showGlobalCounter()
{
    auto sheepCounterGlobal = dynamic_cast<SheepCounterGlobal*>(this->getChildByTag(static_cast<int>(eHudChild::SHEEP_COUNTER_GLOBAL)));
    if (sheepCounterGlobal)
    {
        Utility::fadeInNodeWithTime(sheepCounterGlobal, 1.0f);
    }
}
