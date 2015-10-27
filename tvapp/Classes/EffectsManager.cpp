//
//  EffectsManager.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 09/06/15.
//
//

#include "EffectsManager.h"
#include "GameLogic.h"
#include "CameraManager.h"
#include "Utility.h"
#include "VisibleRect.h"
#include "AnimationComponent.h"
#include "LightningBranch.h"
#include "SoundComponent.h"

#define BLUR_PARALLAX_ENABLED

#if CC_TARGET_PLATFORM!=CC_PLATFORM_ANDROID
	#define MAIN_MENU_FOG_ENABLED
#endif

namespace EffectsManagerDefs
{
    static const char* RAIN_ANCHOR_NODE_NAME = "rainAnchor";
    static const char* WHITE_SCREEN_NODE_NAME = "whiteScreen";
}

enum class eZOrderGameScene
{
    BIG_BOLT = 10,
    WHITE_SCREEN = 10
};

EffectsManager::~EffectsManager()
{
    TextureCache * cache = Director::getInstance()->getTextureCache();
    for(Sprite * sprite : _blurredParallaxes)
    {
        sprite->removeFromParentAndCleanup(true);
        cache->removeTexture(sprite->getTexture());
    }
}

EffectsManager * EffectsManager::createWithLogic(GameLogic * logic)
{
    EffectsManager * node = new (std::nothrow) EffectsManager();
    if(node && node->initWithLogic(logic))
    {
        node->autorelease();
        return node;
    }
    CC_SAFE_DELETE(node);
    return nullptr;
}

#pragma mark - Initialization

bool EffectsManager::initWithLogic(GameLogic * logic)
{
    bool initDone = false;
    
    if (Node::init())
    {
        _gameLogic = logic;
        _levelName = logic->getLevelName();
        
        this->initEffects();
        
        this->scheduleUpdate();
        
        initDone = true;
    }
    return initDone;
}

void EffectsManager::initEffects()
{
    _bolt = nullptr;
    _soundBolt = nullptr;
    _currentParallaxOpacity = 255;
    
    auto levelType = Utility::getLevelType(_levelName);
    auto world = Utility::getWorldFromLevelName(_levelName);
    
#ifdef BLUR_PARALLAX_ENABLED
    this->initParallaxBlur();
#endif
    
    if (eLevelType::MAIN_MENU == levelType)
    {
        this->initMainMenuEffects();
    }
    else if (world == 1 && eLevelType::NORMAL == levelType)
    {
        this->initWorld1Effects();
    }
    else if (world == 2 && eLevelType::NORMAL == levelType)
    {
        this->initWorld2Effects();
    }
    else if (world == 3 && eLevelType::NORMAL == levelType)
    {
        this->initWorld3Effects();
    }
    else if (world == 4 && eLevelType::NORMAL == levelType)
    {
        this->initWorld4Effects();
    }
    else if (world == 5 && eLevelType::NORMAL == levelType)
    {
        this->initWorld5Effects();
    }
}

void EffectsManager::initMainMenuEffects()
{
    this->initMainMenuFog();
}

void EffectsManager::initWorld1Effects()
{
    this->initWorld1Glow();
}

void EffectsManager::initWorld2Effects()
{
    this->initRain();
    this->initBigBolt();
    this->initCloudThunder();
    
    // Setup iniziale
    this->update(0.0f);
}

void EffectsManager::initWorld3Effects()
{
    this->initWorld3Glow();
}

void EffectsManager::initWorld4Effects()
{
}

void EffectsManager::initWorld5Effects()
{
    this->initEoloZoneGlow();
}

void EffectsManager::initMainMenuFog()
{
    static const char * particleFogName = "particle_main_fog.plist";
    static const char * particleGlowName = "particle_main_glow.plist";
    
    const std::vector<std::string> tagValues = {gameTags::PARTICLE_FOG_TAG};
    __Array * particleFogAnchors = _gameLogic->getChildrenWithTags(tagValues);
    
    Ref* ref = nullptr;
    Node* particleFogAnchor = nullptr;
    
    CCARRAY_FOREACH(particleFogAnchors, ref)
    {
        particleFogAnchor = dynamic_cast<Node*>(ref);
        if (particleFogAnchor)
        {
            ParticleSystemQuad * emitter = nullptr;
            
#ifdef MAIN_MENU_FOG_ENABLED
            emitter = ParticleSystemQuad::create(particleFogName);
            emitter->setPosition(Point(0.0f, 80.0f));
            emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
            particleFogAnchor->addChild(emitter);
#endif
            
            emitter = ParticleSystemQuad::create(particleGlowName);
            emitter->setPosition(Point(0.0f, 100.0f));
            emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
            particleFogAnchor->addChild(emitter);
        }
    }
}

void EffectsManager::initParallaxBlur()
{
    LHNodeProtocol * sky = dynamic_cast<LHNodeProtocol *>(_gameLogic->getChildNodeWithName(appParams::SKY_NODE_NAME));
    this->initParallaxBlurOnRoot(sky, false);
    
    LHNodeProtocol * backMountains = dynamic_cast<LHNodeProtocol *>(_gameLogic->getChildNodeWithName(appParams::BACK_MOUNTAIN_NODE_NAME));
    this->initParallaxBlurOnRoot(backMountains, true);
    
    LHNodeProtocol * frontMountains = dynamic_cast<LHNodeProtocol *>(_gameLogic->getChildNodeWithName(appParams::FRONT_MOUNTAIN_NODE_NAME));
    this->initParallaxBlurOnRoot(frontMountains, true);
}

void EffectsManager::initParallaxBlurOnRoot(LHNodeProtocol * root, bool alphaBorders)
{
    static const float blurRadius = 10.0f;
//    static const float blurSampleNum = 7.0f;
    
    if (root)
    {
        __Array * parallaxSprites = root->getChildrenOfType<LHSprite *>();
        
        Ref* ref = nullptr;
        Sprite * parallaxSprite = nullptr;
        Sprite * blurredSprite = nullptr;
        RenderTexture * renderTexture = nullptr;
        
        CCARRAY_FOREACH(parallaxSprites, ref)
        {
            parallaxSprite = dynamic_cast<Sprite*>(ref);
            if (parallaxSprite)
            {
                Size size = parallaxSprite->getContentSize();
                Size sizeWithAlphaBorders = size;
                if (alphaBorders)
                {
                    sizeWithAlphaBorders = size + Size(blurRadius * 2.0f,blurRadius * 2.0f);
                }
                
                // Catturo lo spriteframe in modo da averne una versione isolata dallo
                // spritesheet di appartenenza
                renderTexture = RenderTexture::create(size.width, size.height, Texture2D::PixelFormat::RGBA8888);
                
                blurredSprite = Sprite::createWithSpriteFrame(parallaxSprite->getSpriteFrame());
                blurredSprite->setPosition(Point(size)/2);
                
                renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
                blurredSprite->visit();
                renderTexture->end();
                
                blurredSprite = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
                blurredSprite->setPosition(Point(sizeWithAlphaBorders)/2);
                blurredSprite->setFlippedY(true);
                
                // Catturo la sprite su una renderTexture più grande in modo da creare un bordo di
                // trasparenza che garantisca la corretta sfocatura del blur shader ai bordi della texture
                renderTexture = RenderTexture::create(sizeWithAlphaBorders.width, sizeWithAlphaBorders.height, Texture2D::PixelFormat::RGBA8888);
                
                renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
                blurredSprite->visit();
                renderTexture->end();
                
                Director::getInstance()->getTextureCache()->removeTexture(blurredSprite->getTexture());
                
                blurredSprite = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
                blurredSprite->setPosition(Point(sizeWithAlphaBorders)/2);
                blurredSprite->setFlippedY(true);
                
                GLchar * vertSource = (GLchar*) ccPositionTextureColor_noMVP_vert;
                GLchar * fragSource = (GLchar*) __String::createWithContentsOfFile(FileUtils::getInstance()->fullPathForFilename("Shaders/parallaxBlur.fsh").c_str())->getCString();
                auto program = GLProgram::createWithByteArrays(vertSource, fragSource);
                auto glProgramState = GLProgramState::getOrCreateWithGLProgram(program);
                blurredSprite->setGLProgramState(glProgramState);
                
                auto pixelSize = blurredSprite->getSpriteFrame()->getRectInPixels().size;
                glProgramState->setUniformVec2("resolution", pixelSize);
//                glProgramState->setUniformFloat("blurRadius", blurRadius);
//                glProgramState->setUniformFloat("sampleNum", blurSampleNum);
                
                // Catturo la sprite sottoposta a blur shader
                renderTexture = RenderTexture::create(sizeWithAlphaBorders.width, sizeWithAlphaBorders.height, Texture2D::PixelFormat::RGBA8888);
                
                renderTexture->beginWithClear(0.0f, 0.0f, 0.0f, 0.0f);
                blurredSprite->visit();
                renderTexture->end();
                
                Director::getInstance()->getTextureCache()->removeTexture(blurredSprite->getTexture());
                
                blurredSprite = Sprite::createWithTexture(renderTexture->getSprite()->getTexture());
                blurredSprite->setPosition(Point(size)/2);
                blurredSprite->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
                blurredSprite->setFlippedY(true);
                parallaxSprite->addChild(blurredSprite, -1);
                _blurredParallaxes.pushBack(blurredSprite);
            }
        }
    }
}

void EffectsManager::initWorld1Glow()
{
    static const char * particleGlowName = "particle_world1_glow.plist";
    
    const std::vector<std::string> tagValues = {gameTags::PARTICLE_FOG_TAG};
    __Array * particleAnchors = _gameLogic->getChildrenWithTags(tagValues);
    
    Ref* ref = nullptr;
    Node* particleAnchor = nullptr;
    
    CCARRAY_FOREACH(particleAnchors, ref)
    {
        particleAnchor = dynamic_cast<Node*>(ref);
        if (particleAnchor)
        {
            ParticleSystemQuad * emitter = nullptr;
            
            emitter = ParticleSystemQuad::create(particleGlowName);
            emitter->setBlendFunc(BlendFunc::ADDITIVE);
            emitter->setPosition(Point(0.0f, 100.0f));
            emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
            particleAnchor->addChild(emitter);
        }
    }
}

void EffectsManager::initRain()
{
    auto gameWorld = _gameLogic->getChildNodeWithName(sceneChilds::GAME_WORLD);
    
    const std::vector<std::string> tagValues = {gameTags::RAIN_TAG};
    __Array * rainSprites = _gameLogic->getChildrenWithTags(tagValues);
    
    if (gameWorld && rainSprites->count() > 0)
    {
        LHSprite * rainSprite = dynamic_cast<LHSprite *>(rainSprites->getLastObject());
        if (rainSprite)
        {
            int localZOrder = static_cast<int>(eZOrderGameWorld::RAIN);
            
            LHAsset * rainAsset = dynamic_cast<LHAsset *>(rainSprite->getParent());
            if (rainAsset)
            {
                localZOrder = rainAsset->getLocalZOrder();
            }
            
            // Versione con animazione tiled
            Size spriteSize = rainSprite->getContentSize();
            
            Rect cameraBounds = _gameLogic->getCameraManager()->getCameraWorldBounds();
            Point anchorPosition = Point(cameraBounds.origin.x + cameraBounds.size.width / 2.0f, cameraBounds.origin.y + cameraBounds.size.height / 2.0f);
            
            Size gridSize = cameraBounds.size;
            
            Node * anchorNode = Node::create();
            anchorNode->setName(EffectsManagerDefs::RAIN_ANCHOR_NODE_NAME);
            anchorNode->setContentSize(gridSize);
            anchorNode->setPosition(anchorPosition);
            anchorNode->setCascadeOpacityEnabled(true);
            gameWorld->addChild(anchorNode, localZOrder);
            
            int endX = ceil((gridSize.width / spriteSize.width) / 2.0f);
            int endY = ceil((gridSize.height / spriteSize.height) / 2.0f);
            int startX = - endX;
            int startY = - endY;
            for (int x = startX; x < endX; x++)
            {
                for (int y = startY; y < endY; y++)
                {
                    Sprite * sprite = Sprite::createWithSpriteFrameName(rainSprite->getSpriteFrameName());
                    sprite->setAnchorPoint(Point::ANCHOR_BOTTOM_LEFT);
                    sprite->setPosition(Point(x * spriteSize.width, y * spriteSize.height));
                    anchorNode->addChild(sprite);
                    
                    AnimationComponent * anim = AnimationComponent::create(sprite, AnimationComponent::RAIN);
                    sprite->addChild(anim);
                    anim->startAnimation(5, true);
                }
            }
        }
    }
}

void EffectsManager::initBigBolt()
{
    LayerColor * whiteScreen = LayerColor::create(Color4B(255,255,255,0));
    whiteScreen->setName(EffectsManagerDefs::WHITE_SCREEN_NODE_NAME);
    _gameLogic->addChild(whiteScreen, static_cast<int>(eZOrderGameScene::WHITE_SCREEN));
    
    _bolt = _gameLogic->getChildNodeWithName(appParams::LIGHTNING_ROOT_NODE_NAME);
    if (_bolt)
    {
        _bolt->setVisible(false);
    }
    
    _soundBolt = SoundComponent::create(SoundComponent::SOUND_LIGHTNING);
    this->addChild(_soundBolt);
    
    this->scheduleRandom(CC_SCHEDULE_SELECTOR(EffectsManager::showBigBolt), 5.0f, 7.0f);
}

void EffectsManager::initCloudThunder()
{
    const std::vector<std::string> tagValues = {gameTags::CLOUD_THUNDER_TAG};
    __Array * cloudsThunder = _gameLogic->getChildrenWithTags(tagValues);
    
    Ref* ref = nullptr;
    Node* cloudThunder = nullptr;
    
    CCARRAY_FOREACH(cloudsThunder, ref)
    {
        cloudThunder = dynamic_cast<Node*>(ref);
        if (cloudThunder)
        {
            // Uso il set visible per evitare problemi di opacità dovuti all'effetto di sfocatura
            cloudThunder->setVisible(false);
            _cloudsThunder.pushBack(cloudThunder);
        }
    }
    
    if (_cloudsThunder.size() > 0)
    {
        this->scheduleRandom(CC_SCHEDULE_SELECTOR(EffectsManager::showCloudThunder), 2.0f, 3.0f);
    }
}

void EffectsManager::initWorld3Glow()
{
    static const char * particleGlowName = "particle_world3_glow.plist";
    
    const std::vector<std::string> tagValues = {gameTags::PARTICLE_FOG_TAG};
    __Array * particleAnchors = _gameLogic->getChildrenWithTags(tagValues);
    
    Ref* ref = nullptr;
    Node* particleAnchor = nullptr;
    
    CCARRAY_FOREACH(particleAnchors, ref)
    {
        particleAnchor = dynamic_cast<Node*>(ref);
        if (particleAnchor)
        {
            ParticleSystemQuad * emitter = nullptr;
            
            emitter = ParticleSystemQuad::create(particleGlowName);
            emitter->setBlendFunc(BlendFunc::ADDITIVE);
            emitter->setPosition(Point(0.0f, 100.0f));
            emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
            particleAnchor->addChild(emitter);
        }
    }
}

void EffectsManager::initEoloZoneGlow()
{
    static const char * particleGlowName = "particle_eolo_glow.plist";
    
    const std::vector<std::string> tagValues = {gameTags::PARTICLE_EOLO_ZONE_TAG};
    __Array * particleFogAnchors = _gameLogic->getChildrenWithTags(tagValues);
    
    Ref* ref = nullptr;
    Node* particleFogAnchor = nullptr;
    
    CCARRAY_FOREACH(particleFogAnchors, ref)
    {
        particleFogAnchor = dynamic_cast<Node*>(ref);
        if (particleFogAnchor)
        {
            ParticleSystemQuad * emitter = nullptr;
            
            emitter = ParticleSystemQuad::create(particleGlowName);
            emitter->setPosition(Point(50.0f, 40.0f));
            emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
            emitter->setBlendFunc(BlendFunc::ADDITIVE);
            emitter->setCascadeOpacityEnabled(true);
            particleFogAnchor->addChild(emitter, 1, "Glow");
        }
    }
}

#pragma mark - Update

void EffectsManager::update(float dt)
{
#ifdef BLUR_PARALLAX_ENABLED
    this->updateParallaxBlur(dt);
#endif
    
    if (Utility::getWorldFromLevelName(_levelName) == 2 && eLevelType::NORMAL == Utility::getLevelType(_levelName))
    {
        this->updateWorld2Effects(dt);
    }
}

void EffectsManager::updateParallaxBlur(float dt)
{
    static const float blurOpacityMin = 0.0f;
    static const float blurOpacityMax = 255.0f;
    
    Node * gameWorld = _gameLogic->getChildNodeWithName(sceneChilds::GAME_WORLD);
    CameraManager * cameraManager = _gameLogic->getCameraManager();
    if (cameraManager && gameWorld)
    {
        for (Node * blurredSprite : _blurredParallaxes)
        {
            float gameScaleMin = cameraManager->getMinZoomValue();
            float gameScaleMax = cameraManager->getMaxZoomValue();
            float gameWorldScale = cameraManager->getCurrentZoom();
            
            float blurOpacity = Utility::convertValueBetweenRanges(gameWorldScale, gameScaleMin, gameScaleMax, blurOpacityMin, blurOpacityMax);
//            blurredSprite->setOpacity(255);
//            blurredSprite->setOpacity(blurOpacity);
            
            GLubyte newParallaxOpacity = 255 - blurOpacity;
            blurredSprite->getParent()->setOpacity(newParallaxOpacity);
            _currentParallaxOpacity = (float)newParallaxOpacity / 255.0f;
            
//            blurredSprite->getParent()->setOpacity(0);
        }
    }
}

void EffectsManager::updateWorld2Effects(float dt)
{
    this->updateRain(dt);
}

void EffectsManager::updateRain(float dt)
{
    Node * gameWorld = _gameLogic->getChildNodeWithName(sceneChilds::GAME_WORLD);
    CameraManager * cameraManager = _gameLogic->getCameraManager();
    Node * anchorNode = dynamic_cast<Node *>(gameWorld->getChildByName(EffectsManagerDefs::RAIN_ANCHOR_NODE_NAME));
    if (cameraManager && anchorNode)
    {
        float gameScaleMin = cameraManager->getMinZoomValue();
        float gameScaleMax = cameraManager->getMaxZoomValue();//0.8f;
        float gameWorldScale = cameraManager->getCurrentZoom();
        
        // trasparenza in base allo zoom
        float foregroundOpacityMin = 125.0f;
        float foregroundOpacityMax = 255.0f;
        float newForegroundOpacity = Utility::convertValueBetweenRanges(gameWorldScale, gameScaleMin, gameScaleMax - 0.2f, foregroundOpacityMin, foregroundOpacityMax);
        anchorNode->setOpacity(newForegroundOpacity);
        
        // Rescale in base allo zoom (sensato solo per pioggia a schermo)
//        float foregroundScaleMin = DESIGN_RES_H / anchorNode->getContentSize().width;
//        float foregroundScaleMax = 1.0f;
//        float newForegroundScale = Utility::convertValueBetweenRanges(gameWorldScale, gameScaleMin, gameScaleMax, foregroundScaleMin, foregroundScaleMax);
//        anchorNode->setScale(newForegroundScale);
    }

}

#pragma mark - Private Methods

void EffectsManager::showBigBolt(float dt)
{
    static const char * schedulerKey = "bigBoltSchedule";
    static const int frameRate = 24;
    static const std::vector<GLubyte> whiteScreenOpacityKeyframes = {0,255,100,0,255,100,25,5,0};
    static const std::vector<bool> bigBoltVisibilityKeyframes = {0,0,1,0,0,1,1,0,0};
    static const float minDelay = 10.0f;
    static const float maxDelay = 14.0f;
    
    if (_soundBolt)
    {
        _soundBolt->playOnce();
    }
    
    Node * whiteScreen = dynamic_cast<Node *>(_gameLogic->getChildByName(EffectsManagerDefs::WHITE_SCREEN_NODE_NAME));
    
    if (whiteScreen && _bolt)
    {
        this->startBranchLightining(_bolt);
        
        size_t frameIndex = 0;
        this->schedule([this, whiteScreen, frameIndex](float dt) mutable {
            
            if (frameIndex == whiteScreenOpacityKeyframes.size())
            {
                this->unschedule(schedulerKey);
                this->scheduleRandom(CC_SCHEDULE_SELECTOR(EffectsManager::showBigBolt), minDelay, maxDelay);
            }
            else
            {
            
                whiteScreen->setOpacity(whiteScreenOpacityKeyframes.at(frameIndex));
                _bolt->setVisible(bigBoltVisibilityKeyframes.at(frameIndex));
            }
            frameIndex++;
            
        }, 1.0f/(float)frameRate, schedulerKey);
    }
}

void EffectsManager::showCloudThunder(float dt)
{
    static const char * schedulerKey = "cloudThunderSchedule";
    static const int frameRate = 24;
    static const std::vector<GLubyte> cloudThunderOpacityKeyframes = {0,155,0,255,76,13,0};
    static const float minDelay = 2.0f;
    static const float maxDelay = 3.0f;
    
    int frameIndex = 0;
    this->schedule([this, frameIndex](float dt) mutable {
        
        if (frameIndex == cloudThunderOpacityKeyframes.size())
        {
            this->unschedule(schedulerKey);
            this->scheduleRandom(CC_SCHEDULE_SELECTOR(EffectsManager::showCloudThunder), minDelay, maxDelay);
        }
        else
        {
            GLubyte opacity = cloudThunderOpacityKeyframes.at(frameIndex);
            
            for (auto iter = _cloudsThunder.begin(); iter != _cloudsThunder.end(); iter++)
            {
//                (*iter)->setOpacity(opacity);
                Node * cloudThunder = (*iter);
                
                // Uso il set visible per evitare conflitti di opacità dovuti all'effetto di sfocatura
                if (opacity == 0)
                {
                    cloudThunder->setVisible(false);
                }
                else
                {
                    cloudThunder->setVisible(true);
                    cloudThunder->setOpacity(opacity * _currentParallaxOpacity);
                    
                    // Aggiorno anche la sprite di sfocaturra
                    if (cloudThunder->getChildrenCount() > 0.0f)
                    {
                        Node * blurredNode = cloudThunder->getChildren().at(0);
                        blurredNode->setOpacity(opacity);
                    }
                }
            }
        }
        frameIndex++;
        
    }, 1.0f/(float)frameRate, schedulerKey);
}

void EffectsManager::startBranchLightining(Node * rootNode)
{
    static const Color3B color = Color3B::WHITE;
    static const float thickness = 2.0f;
    static const std::vector<float> branchPoints = {0.25f, 0.5f, 0.75f};
    
    if (rootNode)
    {
    	// Non necessario - Il LightningBranch si rimuove da solo quando il fulmine ha terminato il suo fade out interno
    	//rootNode->removeAllChildrenWithCleanup(true);

        float minStartX = VisibleRect::getVisibleRect().getMinX() + 200.0f;
        float maxStartX = VisibleRect::getVisibleRect().getMaxX() - 200.0f;
        float boltStartPositionX = minStartX + (maxStartX - minStartX) * CCRANDOM_0_1();
        float boltStartPositionY = VisibleRect::top().y;
        Point boltStartPosition = Point(boltStartPositionX, boltStartPositionY);
        
        float displacementEndX = CCRANDOM_MINUS1_1() * 100.0f;
        float boltEndPositionX = boltStartPositionX + displacementEndX;
        float boltEndPositionY = VisibleRect::bottom().y + VisibleRect::getVisibleRect().size.height / 4.0f;
        Point boltEndPosition = Point(boltEndPositionX, boltEndPositionY);
        
        boltStartPosition = rootNode->convertToNodeSpace(boltStartPosition);
        boltEndPosition = rootNode->convertToNodeSpace(boltEndPosition);
        
        auto lightningBranch = LightningBranch::create(boltStartPosition, boltEndPosition, color, thickness, branchPoints);
        rootNode->addChild(lightningBranch);
    }
}

#pragma mark - Utilities

void EffectsManager::scheduleRandom(SEL_SCHEDULE selector, const float min, const float max)
{
    float delay = (max - min) * CCRANDOM_0_1() + min;
    this->scheduleOnce(selector, delay);
}
