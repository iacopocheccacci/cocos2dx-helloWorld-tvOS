//
//  Player.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifndef __SOP_PLAYER__
#define __SOP_PLAYER__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"
#include "PlayerState.h"
#include "WindMovable.h"

USING_NS_CC;

class SoundComponent;
class AnimationComponent;

namespace playerDefs
{
    static const char* CLASS_NAME = "Player";
    static const char* PLAYER_NAME = "Pan";
    static const char* PLAYER_SPRITE_NAME = "PanSprite";
    
    static const char* PARTICLE_START_FILE = "particle_note_start.plist";
    static const char* PARTICLE_FILE = "particle_note_radial.plist";
    static const char* PARTICLE_NAME = "particleRadial";
    
    static const char* MENU_WALK_TAG = "MENU_WALK";
};

enum class eNoteEffect : int
{
    NO_EFFECT = 0,
    NORMAL_NOTE,
    INVERSE_NOTE,
};

const int NOTE_BAR = 100;

class Player : public LHNode, public WindMovable
{
public:
    virtual ~Player();
    NODEWITHDICT_FUNC(Player);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    float getHorizontalImpulse()    { return _horizontalImpulse; }
    float getVerticalImpulse()      { return _verticalImpulse; }
    float getHorizontalVelLimit()   { return _horizontalVelLimit; }
    float getParticleScale()        { return _particleScale; }
    
    void setHorizontalImpulse(float value)     { _horizontalImpulse = value; }
    void setVerticalImpulse(float value)       { _verticalImpulse = value; }
    void setHorizontalVelLimit(float value)    { _horizontalVelLimit = value; }
    void setParticleScale(float value)         { _particleScale = value; }
    
    bool isVerticalOnlyJump();
    bool isMovingRight()            { return _movingRight; }
    bool isMovingLeft()             { return _movingLeft; }
    bool isJumping()                { return _jumping; }
    
    void setMovingRight(bool value) { _movingRight = value; }
    void setMovingLeft(bool value)  { _movingLeft = value; }
    void setJumping(bool value)     { _jumping = value; }
    
    void handleInput(eAction input);
    
    void oppositeImpulse();
    
    void enableNoteParticles();
    void disableNoteParticles();
    
    PlayerState * getPlayerState()                                  { return _currentState; }
    
    void setPlayingState(const bool playing);
    void setPlayingState(const bool playing, const bool endSound);
    bool getIsPlaying()                                             { return _playing; }
    
    void setSheepShowState(const bool enabled);
    
    eGroundType getGroundType()                                     { return _groundType; }
    void setContactBelow(LHNodeProtocol * platform, bool begin);
    void setContactToTheSide(bool value)                            { _hasContactToTheSide = value; }
    
    void setPushingSide(float direction, bool begin);
    void setPushingSide(float direction, bool begin, LHNodeProtocol * block);
    bool getIsPushingRight()                                        { return _pushingRight; }
    bool getIsPushingLeft()                                         { return _pushingLeft; }
    
    void resetPushing()                                             { _pushingLeft = false;
                                                                      _pushingRight = false; }
    
    void setIsOnMovablePlatform(LHNodeProtocol* platform, bool begin);
    LHNodeProtocol* getMovablePlatform()                                   { return _movablePlatform; }
    
    void setNoteEffectType(eNoteEffect effectType)                  { _noteEffect = effectType; }
    eNoteEffect getNoteEffectType()                                 { return _noteEffect; }
    
    void setSoundFluteStart(SoundComponent * soundFluteStart)       { _soundFluteStart = soundFluteStart; }
    SoundComponent * getSoundFluteStart()                           { return _soundFluteStart; }
    
    void setSoundFluteRedStart(SoundComponent * soundFluteRedStart) { _soundFluteRedStart = soundFluteRedStart; }
    SoundComponent * getSoundFluteRedStart()                        { return _soundFluteRedStart; }
    
    SoundComponent * getSoundWalk()                                 { return _soundWalk; }
    SoundComponent * getSoundFluteEnd()                             { return _soundFluteEnd; }
    SoundComponent * getSoundJump()                                 { return _soundJump; }
    SoundComponent * getSoundPush()                                 { return _soundPush; }
    SoundComponent * getSoundCatchNote()                            { return _soundCatchNote; }
    SoundComponent * getSoundDeath()                                { return _soundDeath; }
    SoundComponent * getSoundBurning()                              { return _soundBurn; }
    
    AnimationComponent * getAnimationIdle()                         { return _animationIdle; }
    AnimationComponent * getAnimationWalk()                         { return _animationWalk; }
    AnimationComponent * getAnimationJump()                         { return _animationJump; }
    AnimationComponent * getAnimationFall()                         { return _animationFall; }
    AnimationComponent * getAnimationRebounce()                     { return _animationRebounce; }
    AnimationComponent * getAnimationPlayingIdle()                  { return _animationPlayingIdle; }
    AnimationComponent * getAnimationPlayingWalk()                  { return _animationPlayingWalk; }
    AnimationComponent * getAnimationPlayingJump()                  { return _animationPlayingJump; }
    AnimationComponent * getAnimationPlayingFall()                  { return _animationPlayingFall; }
    AnimationComponent * getAnimationPushing()                      { return _animationPush; }
    AnimationComponent * getAnimationStartPushing()                 { return _animationStartPush; }
    AnimationComponent * getAnimationPlayingRebounce()              { return _animationPlayingRebounce; }
    AnimationComponent * getAnimationBurning()                      { return _animationBurn; }
    AnimationComponent * getAnimationSliding()                      { return _animationSlide; }
    AnimationComponent * getAnimationExultFall()                    { return _animationExultFall; }
    AnimationComponent * getAnimationExultRebounce()                { return _animationExultRebounce; }
    AnimationComponent * getAnimationFly()                          { return _animationFly; }
    
    void movePlayerRight();
    void movePlayerLeft();
    void movePlayerJump();
    
    void littleJumpOnInteractive();
    void littleJumpOnSheep();
    
    void handleJumpingStateEnd(bool isJumpPressed);
    
    void handleOnBlockBorder(b2Vec2 distance, LHNodeProtocol* block, bool begin);
    void handleBlockSideContact(b2Vec2 distance, LHNodeProtocol* block, bool begin);
    void handleBlockBelowContact(LHNodeProtocol* block, bool begin);
    
    float getVerticalZero();
    float getHorizontalZero();
    void burn();
    
    void startTimer(CallFunc* callback, float time, bool inactive);
    void stopTimer();
    void handleCollisionWithInteractiveBlock(LHNodeProtocol *Block, bool begin);
    
    void updateLastVelocity();
    Point getLastVelocity();
    
    void setUnderEoloWind(LHNode* node, bool begin);
    void updateUnderEoloWind(LHNode* node);
    void exitUnderEoloWind();
    
    bool isUnderEoloEffect()                { return _isUnderEoloEffect; }
    void setUnderEoloEffect(bool value)     { _isUnderEoloEffect = value; }
    
    bool getIsOnMenu()                      { return _isOnMenuLevel; }
    bool getIsBurning()                     { return _isBurning; }
    
    void setCollisionDisabled(bool value)   { _collisionDisabled = value; }
    bool getCollisionDisabled()             { return _collisionDisabled; }
    
private:
    PlayerState* _currentState;
    
    virtual void update(float dt);
    bool isPossibleToJump();
    void updateEmitter(float dt);
    void loadPlayerData();
    bool checkIfIsOnMovable(LHNodeProtocol* shape, bool begin);
    
    void initAudio();
    void initAnimation();
//    void initNoteEffect();
    
    void generateParticleEmitterByName(std::string particleName);
    void removeParticleEmitterByName(std::string particleName);
    
    bool checkIfInMenu();
    
    bool _playing;
    
    bool _movingRight;
    bool _movingLeft;
    bool _jumping;
    
    float _horizontalImpulse;
    float _verticalImpulse;
    float _horizontalVelLimit;
    float _particleScale;
    
    bool _hasContactToTheSide;
    bool _hasContactBelow;
    
    bool _pushingRight;         // Sto spingendo un blocco a destra
    bool _pushingLeft;          // Sto spingendo un blocco a sinistra
    
    LHNodeProtocol* _movablePlatform;  // Se diversa da NULL sono in piedi su una piattaforma mobile
    eNoteEffect _noteEffect;
    
    eGroundType _groundType;
    bool _isDead;
    
    Point _currentVelocity;
    Point _lastVelocity;
    
    bool _isUnderEoloEffect;
    
    SoundComponent * _soundWalk;
    SoundComponent * _soundFluteStart;
    SoundComponent * _soundFluteRedStart;
    SoundComponent * _soundFluteEnd;
    SoundComponent * _soundJump;
    SoundComponent * _soundPush;
    SoundComponent * _soundPushPant;
    SoundComponent * _soundCatchNote;
    SoundComponent * _soundDeath;
    SoundComponent * _soundBurn;
    SoundComponent * _soundExult;
    
    AnimationComponent * _animationWalk;
    AnimationComponent * _animationJump;
    AnimationComponent * _animationFall;
    AnimationComponent * _animationRebounce;
    AnimationComponent * _animationIdle;
    AnimationComponent * _animationPlayingWalk;
    AnimationComponent * _animationPlayingJump;
    AnimationComponent * _animationPlayingFall;
    AnimationComponent * _animationPlayingIdle;
    AnimationComponent * _animationPlayingRebounce;
    AnimationComponent * _animationPush;
    AnimationComponent * _animationStartPush;
    AnimationComponent * _animationBurn;
    AnimationComponent * _animationSlide;
    AnimationComponent * _animationExultFall;
    AnimationComponent * _animationExultRebounce;
    AnimationComponent * _animationFly;
    
    bool _isOnMenuLevel;
    bool _isBurning;
    bool _collisionDisabled;
};

#endif // __SOP_PLAYER__
