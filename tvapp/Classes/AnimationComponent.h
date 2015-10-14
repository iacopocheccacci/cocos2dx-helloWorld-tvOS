//
//  AnimationComponent.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/02/15.
//
//

#ifndef __SOP__ANIMATION_COMPONENT__
#define __SOP__ANIMATION_COMPONENT__

#include "cocos2d.h"

USING_NS_CC;

class SoundComponent;

namespace animationComponentDefs
{
    static const float TIME_BETWEEN_FRAMES = 0.08f;
}

class AnimationComponent : public Node
{
    
public:
    static const char * PLAYER_WALK_MENU;
    static const char * PLAYER_WALK;
    static const char * PLAYER_JUMP;
    static const char * PLAYER_FALL;
    static const char * PLAYER_PLAYING_FALL;
    static const char * PLAYER_REBOUNCE;
    static const char * PLAYER_PLAYING_REBOUNCE;
    static const char * PLAYER_IDLE;
    static const char * PLAYER_PLAYING_WALK;
    static const char * PLAYER_PLAYING_JUMP;
    static const char * PLAYER_PLAYING_IDLE;
    static const char * PLAYER_START_PUSHING;
    static const char * PLAYER_PUSHING;
    static const char * PLAYER_BURNING;
    static const char * PLAYER_SLIDING;
    static const char * PLAYER_EXULT_FALL;
    static const char * PLAYER_EXULT_REBOUNCE;
    static const char * PLAYER_FLY;
    
    static const char * SHEEP_IDLE_1;
    static const char * SHEEP_IDLE_2;
    static const char * SHEEP_IDLE_3;
    static const char * SHEEP_WALK;
    static const char * SHEEP_JUMP;
    static const char * SHEEP_FALL;
    static const char * SHEEP_BURN;
    static const char * SHEEP_CATCH;
    static const char * SHEEP_FLY;
    
    static const char * MONSTER_IDLE_1;
    static const char * MONSTER_IDLE_2;
    static const char * MONSTER_WALK;
    static const char * MONSTER_JUMP;
    static const char * MONSTER_FALL;
    static const char * MONSTER_BURN;
    static const char * MONSTER_CATCH;
    static const char * MONSTER_FLY;
    
    static const char * HOR_GOATSKIN_OPEN;
    static const char * HOR_GOATSKIN_BLOW;
    static const char * VER_GOATSKIN_OPEN;
    static const char * VER_GOATSKIN_BLOW;
    
    static const char * MONOUSE_PLATFORM_SINGLE_FALL;
    static const char * MONOUSE_PLATFORM_1_FALL;
    static const char * MONOUSE_PLATFORM_2_FALL;
    static const char * MONOUSE_PLATFORM_3_FALL;
    static const char * MONOUSE_PLATFORM_4_FALL;
    
    static const char * WOLF_CATCH_FAR;
    static const char * WOLF_CATCH_NEAR;
    
    static const char * BRAZIER_GROUND_ON;
    static const char * BRAZIER_GROUND_OFF;
    static const char * BRAZIER_GROUND_SMOKE;
    static const char * BRAZIER_INTERACTIVE_ON;
    static const char * BRAZIER_INTERACTIVE_OFF;
    static const char * BRAZIER_INTERACTIVE_SMOKE;
    
    static const char * FLUTE;
    
    static const char * TUTORIAL_PRESS;
    
    static const char * RAIN;
    static const char * DROP;
    
    AnimationComponent();
    virtual ~AnimationComponent();
    
    static AnimationComponent * create(Sprite* sprite, const std::string animationName);
    bool init(Sprite* sprite, const std::string animationName);
    
    void setAnimationName(std::string animationName)                    { _animationId = animationName; }
    std::string getAnimationName()                                      { return _animationId; }
    
    Sprite * getSprite()                                                { return _sprite; }
    
    void setFrameNumber(unsigned int frameNumber)                       { _frameNr = frameNumber; }
    void setStartingFrame(int startingFrame)                            { _startingFrame = startingFrame; }
    void setCustomFrames(std::vector<int> frames);
    float getDuration();
    
    /*
     Inizia un'animazione con il numero di frame settato tramite "setFrameNumber" o tramite l'ultima
     chiamata ad un metodo "startAnimation" che richiede il numero di frame.
     Se il valore non è stato assegnato, non fa niente.
     */
    void startAnimation(bool repeat);
    void startAnimation(unsigned int frameNr, bool repeat);
    void startAnimationWithSound(unsigned int frameNr, SoundComponent* sound);
    void startAnimationWithCallback(unsigned int frameNr, CallFunc * callback, bool before);
    void startAnimationWithCallback(unsigned int frameNr, CallFunc * callback, bool before, bool repeatForever);
    void startReverseAnimation();
    void startReverseAnimation(unsigned int frameNr);
    void startAnimationFromRandomFrame(unsigned int frameNr, bool repeat);
    void stopAnimation(bool allActions);
    void stopAnimation();
    
private:
    std::string _animationId;
    Sprite* _sprite;
    unsigned int _frameNr;
    int _startingFrame;
    std::vector<int> _customFrames;
    
    Animate * createAnimation(unsigned int frameNr);
    Animate * createAnimation(unsigned int frameNr, bool reverse);
    Animate * createAnimationFromRandomFrame(unsigned int frameNr);
};

#endif // __SOP__ANIMATION_COMPONENT__
