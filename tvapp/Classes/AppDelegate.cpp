#include "AppDelegate.h"
#include "Definitions.h"
#include "FakeLoading.h"
#include <vector>
#include <string>
#include "AnalyticsManager.h"
#include "ScoreManager.h"

using namespace std;

#ifdef EDITOR_MODE
    #include "EditorTableView.h"
#endif

USING_NS_CC;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate() 
{
    
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
#if CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
#else
    GLContextAttrs glContextAttrs = {5, 6, 5, 0, 16, 8};
#endif

    
    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview)
    {
        glview = GLViewImpl::create(appParams::APP_NAME);
        auto screen = glview->getVisibleRect();
        Size frameSize = screen.size;
        if(glview->getRetinaFactor() == 2)
        {
            frameSize = Size(frameSize.width * 1.5f, frameSize.height * 1.35f);
//            glview->setFrameSize(rect.size.width * 1.5f, rect.size.height * 1.35f);
        }
#ifdef WIDE_SCREEN
        frameSize = Size(frameSize.width, frameSize.width * 9.0f / 16.0f);
#endif
        glview->setFrameSize(frameSize.width, frameSize.height);
        
        director->setOpenGLView(glview);
        
    }

    glview->setDesignResolutionSize(DESIGN_RES_H, DESIGN_RES_V, ResolutionPolicy::NO_BORDER);
    Size frameSize = glview->getFrameSize();
    
    vector<string> searchPath;
    
    if (frameSize.width > DESIGN_RES_H)
    {
//        searchPath.push_back("hd");
        searchPath.push_back(appParams::EXTENSION_HD_PATH);
        director->setContentScaleFactor(2);
    }
    else
    {
//        searchPath.push_back("sd");
        searchPath.push_back(appParams::EXTENSION_SD_PATH);
        director->setContentScaleFactor(1);
    }
    
    searchPath.push_back(appParams::PARTICLES_PATH);
    searchPath.push_back(appParams::COCOS_STUDIO_PATH);
    searchPath.push_back(appParams::PUBLISHED_RES_PATH);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);
    
    auto fileUtils = FileUtils::getInstance();
    
    fileUtils->setSearchPaths(searchPath);
    
    bool displayStats = false;

#if 1 == COCOS2D_DEBUG
    displayStats = true; // Display Stats in EDITOR_MODE
#endif
    
    // Analytics
    AnalyticsManager::initAnalytics();
    AnalyticsManager::startSession();
    
    // Inizializzo lo ScoreManager (calcolo totale delle pecore nel gioco...)
    ScoreManager::getInstance();
    
#ifdef EDITOR_MODE
    Scene *scene = Scene::create();
    EditorTableView *layer = EditorTableView::create();
    scene->addChild(layer);
#else
    auto scene = FakeLoading::scene();
#endif
    
    director->setDisplayStats(false);
    
    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
