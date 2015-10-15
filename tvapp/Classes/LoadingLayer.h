//
//  LoadingLayer.h
//  SongOfPan
//
//  Created by Giuseppe Lapenta on 11/03/15.
//
//

#ifndef __SongOfPan__LoadingLayer__
#define __SongOfPan__LoadingLayer__

#include "cocos2d.h"
#include "InputController.h"
#include "GameContext.h"

USING_NS_CC;

enum class eWorld;

class LoadingLayer : public Layer
{
    
    GameContext _context;
    
public:
    
    LoadingLayer();
    virtual ~LoadingLayer();
    
    virtual void onEnterTransitionDidFinish() override;
    
    static Scene* scene(GameContext context);
    static LoadingLayer* create(GameContext context);
    
    bool init(GameContext context);
    
    void update(float delta) override;
    
private:
    
    void releaseUnusedResources();
    void loadAudio();
    void goToScene();
    
    bool isLoadingEnded();

};

#endif /* defined(__SongOfPan__LoadingLayer__) */
