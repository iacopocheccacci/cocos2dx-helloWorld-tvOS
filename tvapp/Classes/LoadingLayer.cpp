//
//  LoadingLayer.cpp
//  SongOfPan
//
//  Created by Giuseppe Lapenta on 11/03/15.
//
//

#include "LoadingLayer.h"
#include "GameLogic.h"
#include "Utility.h"
#include "SoundManager.h"
#include "SoundComponent.h"

LoadingLayer::LoadingLayer(){}

LoadingLayer::~LoadingLayer(){}


Scene* LoadingLayer::scene(GameContext context)
{
    auto scene = Scene::create();
    
    auto layer = LoadingLayer::create(context);
    
    scene->addChild(layer);
    
    return scene;
}

LoadingLayer* LoadingLayer::create(GameContext context)
{
    LoadingLayer *ret = new (std::nothrow) LoadingLayer();
    
    if (ret && ret->init(context))
    {
        ret->autorelease();
        return ret;
    }
    else
    {
        CC_SAFE_DELETE(ret);
        return nullptr;
    }
}


bool LoadingLayer::init(GameContext context)
{
    if (!Layer::init())
    {
        return false;
    }
    
    _context = context;

    auto winSize = Director::getInstance()->getWinSize();
    auto layerColor = LayerColor::create(Color4B(Color3B::WHITE));
    this->addChild(layerColor);
    
    auto loadingLabel = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "Loading");
    loadingLabel->setPosition(Point(winSize.width/2, winSize.height/2));
    loadingLabel->setColor(Color3B::BLACK);
    loadingLabel->setScale(0.5f);
    this->addChild(loadingLabel);
    
    return true;
}

void LoadingLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
    
    CallFunc * releaseUnusedResources = CallFunc::create(CC_CALLBACK_0(LoadingLayer::releaseUnusedResources, this));
    CallFunc * preloadAudio = CallFunc::create(CC_CALLBACK_0(LoadingLayer::loadAudio, this));
    CallFunc * waitForLoadingEnd = CallFunc::create([this](){
        this->scheduleUpdate();
    });
    
    this->runAction(Sequence::create(DelayTime::create(1.0f),
                                     releaseUnusedResources,
                                     preloadAudio,
                                     waitForLoadingEnd,
                                     NULL));
}

void LoadingLayer::releaseUnusedResources()
{
    SpriteFrameCache::getInstance()->removeUnusedSpriteFrames();
    Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

void LoadingLayer::loadAudio()
{
    SoundComponent::preloadAudioForLevelName(_context.getLevelName());
}

void LoadingLayer::goToScene()
{
    auto scene = GameLogic::create(_context);
    Director::getInstance()->replaceScene(TransitionFade::create(2.0, scene, Color3B::WHITE));
}

void LoadingLayer::update(float delta)
{
    if (this->isLoadingEnded())
    {
        this->unscheduleUpdate();
        this->goToScene();
    }
}

bool LoadingLayer::isLoadingEnded()
{
    bool res = true;
    
    if (false == SoundManager::getInstance()->isAudioPreloaded())
    {
        res = false;
    }
    
    // Aggiungere in && altri eventuali controlli di caricamento asincrono
    
    return res;
}
