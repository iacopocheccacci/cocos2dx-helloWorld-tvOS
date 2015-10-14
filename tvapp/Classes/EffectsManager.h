//
//  EffectsManager.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 09/06/15.
//
//

#ifndef __SongOfPan__ForegroundLayer__
#define __SongOfPan__ForegroundLayer__

#include "cocos2d.h"
#include "LevelHelper2API.h"

class GameLogic;
class SoundComponent;

class EffectsManager : public Node
{
public:
    
    virtual ~EffectsManager();
    static EffectsManager * createWithLogic(GameLogic * logic);
    bool initWithLogic(GameLogic * logic);
    
private:
    
    void initEffects();
    
    void initMainMenuEffects();
    void initWorld1Effects();
    void initWorld2Effects();
    void initWorld3Effects();
    void initWorld4Effects();
    void initWorld5Effects();
    
    void initMainMenuFog();
    
    void initParallaxBlur();
    void initParallaxBlurOnRoot(LHNodeProtocol * root, bool alphaBorders);
    
    void initWorld1Glow();
    void initRain();
    void initBigBolt();
    void initCloudThunder();
    void initWorld3Glow();
    void initEoloZoneGlow();
    
    void update(float dt);
    
    void updateParallaxBlur(float dt);
    
    void updateWorld2Effects(float dt);
    void updateRain(float dt);
    
    void showBigBolt(float dt);
    void showCloudThunder(float dt);
    
    void startBranchLightining(Node * rootNode);
    
    // Utilites
    void scheduleRandom(SEL_SCHEDULE selector, const float min, const float max);
    
    GameLogic * _gameLogic;
    std::string _levelName;
    
    Vector<Sprite *> _blurredParallaxes;
    Vector<Node *> _cloudsThunder;
    Node * _bolt;
    
    SoundComponent * _soundBolt;
    
    float _currentParallaxOpacity;
};

#endif /* defined(__SongOfPan__ForegroundLayer__) */
