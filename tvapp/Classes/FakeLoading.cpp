//
//  FakeLoading.cpp
//  SongOfPan
//
//  Created by Giuseppe Lapenta on 17/03/15.
//
//

#include "FakeLoading.h"
#include "GameLogic.h"
#include "SoundManager.h"
#include "SoundComponent.h"
#include "SettingsManager.h"
#include "GameContext.h"

namespace fakeLoadingDefs
{
    static const char* SPLASH_SCREEN_BG = "logoBG.png";
    static const char* LOGO_SPRITE = "logobalzo.png";
    static const char* LOADING_BG = "loadingBG.png";
    static const char* LOADING_TEXT = "loadingText.png";
}

FakeLoading::FakeLoading()
{
    
}

FakeLoading::~FakeLoading()
{
    
}

Scene* FakeLoading::scene()
{
    auto scene = Scene::create();
    auto layer = FakeLoading::create();
    scene->addChild(layer);
    
    return scene;
}

bool FakeLoading::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    auto size = Director::getInstance()->getWinSize();
    Node * logoRoot = Node::create();
    logoRoot->setPosition(Point(size.width/2, size.height/2));
    logoRoot->setCascadeOpacityEnabled(true);
    this->addChild(logoRoot);
    
    auto logoBG = Sprite::create(fakeLoadingDefs::SPLASH_SCREEN_BG);
    logoBG->setScaleX(size.width / logoBG->getContentSize().width);
    logoBG->setScaleY(size.height / logoBG->getContentSize().height);
    logoRoot->addChild(logoBG);
    
    auto logo = Sprite::create(fakeLoadingDefs::LOGO_SPRITE);
    logoRoot->addChild(logo);
    
    auto loadingBG = Sprite::create(fakeLoadingDefs::LOADING_BG);
    loadingBG->setPosition(Point(size.width/2, size.height/2));
    loadingBG->setCascadeOpacityEnabled(true);
    loadingBG->setOpacity(0);
    this->addChild(loadingBG);
    
    auto loadingText = Sprite::create(fakeLoadingDefs::LOADING_TEXT);
    loadingText->setPosition(Point(loadingBG->getContentSize()/2.0f) + Point(0.0f, 220.0f));
    loadingBG->addChild(loadingText);
    
    DelayTime * logoDuration = DelayTime::create(3.0f);
    FadeOut * logoFadeOut = FadeOut::create(1.0f);
    
    DelayTime * loadingDelay = DelayTime::create(4.0f);
    FadeIn * loadingFadeIn = FadeIn::create(1.0f);
    DelayTime * loadingDuration = DelayTime::create(3.0f);
    CallFunc * waitForLoadingEnd = CallFunc::create([this](){
        this->scheduleUpdate();
    });
    
    logoRoot->runAction(Sequence::create(logoDuration, logoFadeOut, NULL));
    loadingBG->runAction(Sequence::create(loadingDelay, loadingFadeIn, loadingDuration, waitForLoadingEnd, NULL));
    
    return true;
}

void FakeLoading::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    this->initAudio();
}

void FakeLoading::initAudio()
{
    // Inizializzo il SoundManager
    if (SettingsManager::getInstance()->loadUserActiveMusic())
    {
        SoundManager::getInstance()->setUserVolumeMusic(SettingsManager::getInstance()->loadUserVolumeMusic(), false);
    }
    else
    {
        SoundManager::getInstance()->setUserVolumeMusic(0.0f, false);
    }
    
    if (SettingsManager::getInstance()->loadUserActiveFX())
    {
        SoundManager::getInstance()->setUserVolumeFX(SettingsManager::getInstance()->loadUserVolumeFX(), false);
    }
    else
    {
        SoundManager::getInstance()->setUserVolumeFX(0.0f, false);
    }
    
    SoundComponent::preloadAudioForLevelName(appParams::PUBLISHED_MAIN_MENU_LEVEL_NAME);
}

void FakeLoading::update(float dt)
{
	if (this->isLoadingEnded())
	{
		this->unscheduleUpdate();

		GameContext context(appParams::PUBLISHED_MAIN_MENU_LEVEL_NAME, "");

		auto scene = GameLogic::create(context);
		Director::getInstance()->replaceScene(TransitionFade::create(2.0, scene, Color3B::WHITE));
	}
}

bool FakeLoading::isLoadingEnded()
{
	bool res = true;

	if (false == SoundManager::getInstance()->isAudioPreloaded())
	{
		res = false;
	}

	// Aggiungere in && altri eventuali controlli di caricamento asincrono

	return res;
}
