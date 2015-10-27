//
//  Sheep.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 11/11/14.
//
//

#ifndef __SOP_SHEEP__
#define __SOP_SHEEP__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"
#include "SheepState.h"
#include "WindMovable.h"

class SoundComponent;
class AnimationComponent;

namespace sheepDefs
{
    static const char* CLASS_NAME = "Sheep";
    static const char* SPRITE_NAME = "SheepSprite";
}

USING_NS_CC;

class Sheep : public LHNode, public WindMovable
{
public:
    NODEWITHDICT_FUNC(Sheep);
    virtual ~Sheep();
    
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt) override;
    
    void initAudio(LHNode * listenerNode);
    void initAnimation();
    
    float getHorizontalImpulse()                                                { return _horizontalImpulse; }
    float getVerticalImpulse()                                                  { return _verticalImpulse; }
    float getHorizontalVelLimit()                                               { return _horizontalVelLimit; }
    float getNoteActivationDistance()                                           { return _noteDistanceActivation; }
    float getDistanceFromPlayer()                                               { return _distanceFromPlayer; }
    
    void setHorizontalImpulse(float value)                                      { _horizontalImpulse = value; }
    void setVerticalImpulse(float value)                                        { _verticalImpulse = value; }
    void setHorizontalVelLimit(float value)                                     { _horizontalVelLimit = value; }
    void setNoteActivationDistance(float value)                                 { _noteDistanceActivation = value; }
    void setDistanceFromPlayer(float value)                                     { _distanceFromPlayer = value; }
    
    bool getIsExcited()                                                         { return _isExcited; }
    void setIsExcited(bool value)                                               { _isExcited = value; }
    
    void handleInput(eAction input);
    void updateSheepStatus(b2Vec2 playerPosition, bool isInverseEffect, int &index);
    void handleCollisionWithObstacle();
    
    void setContactBelow(LHNodeProtocol * platform, bool value);
    void setContactToTheSide(bool value)                                        { _hasContactToTheSide = value; }
    
    void setIsOnMovablePlatform(LHNodeProtocol* platform, bool begin);
    LHNodeProtocol* getMovablePlatform()                                        { return _movablePlatform; }
    
     bool isJumping();
    
    void handleBlockSideContact(LHNodeProtocol* block, bool begin);
    void handleBlockBelowContact(LHNodeProtocol* block, bool begin);
    
    void setIsBehindInteractive(bool value)                                     { _isBehindInteractive = value; }
    bool getIsBehindInteractive()                                               { return _isBehindInteractive; }
    
    float getVerticalZero();
    float getHorizontalZero();
    
    void burn();
    void playJumpOnSheep();
    
    void handleVelocityOnDoorMovement();
    
    SoundComponent * getSoundExcited()                                          { return _soundExcited; }
    SoundComponent * getSoundIdle3()                                            { return _soundIdle2; }
    SoundComponent * getSoundIdle2()                                            { return _soundIdle3; }
    SoundComponent * getSoundBurn()                                             { return _soundBurn; }
    SoundComponent * getSoundJump()                                             { return _soundJump; }
    SoundComponent * getSoundLand()                                             { return _soundLand; }
    SoundComponent * getSoundWalk()                                             { return _soundWalk; }
    
    AnimationComponent * getAnimationIdle1()                                    { return _animationIdle1; }
    AnimationComponent * getAnimationIdle2()                                    { return _animationIdle2; }
    AnimationComponent * getAnimationIdle3()                                    { return _animationIdle3; }
    AnimationComponent * getAnimationWalk()                                     { return _animationWalk; }
    AnimationComponent * getAnimationJump()                                     { return _animationJump; }
    AnimationComponent * getAnimationFall()                                     { return _animationFall; }
    AnimationComponent * getAnimationBurn()                                     { return _animationBurn; }
    AnimationComponent * getAnimationCatch()                                    { return _animationCatch; }
    AnimationComponent * getAnimationFly()                                      { return _animationFly; }
    
    void setIsPlayerOver(bool value)                                            { _isPlayerOver = value; }
    bool getIsPlayerOver()                                                      { return _isPlayerOver; }
    
    void updateAudio(float dt);
    void setUnderEoloWind(LHNode* node, bool begin) override;
    void updateUnderEoloWind(LHNode* node) override;
    void exitUnderEoloWind() override;
    
    void setIsOnGoalRight(bool isOnGoalRight)                                   { _isOnGoalRight = isOnGoalRight; }
    
    bool getIsBurning()                                                         { return _isBurning; }
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
    void onExit() override;
    
private:
    virtual void update(float dt) override;
    
    bool isPossibleToJump();
    void loadSheepData();
    
    
    
    eAction handleNoteNormalEffect(float distance, b2Body* sheepBody, int index);
    eAction handleNoteNormalEffectToSheep(float distance, b2Body* sheepBody, int index);
    eAction handleNoteNormalEffectToMonster(float distance, b2Body* monsterBody);
    eAction handleMonsterOnGoalLeft(float distance, b2Body* monsterBody);
    eAction handleMonsterOnGoalRight(float distance, b2Body* monsterBody);
    
    eAction handleNoteInverseEffect(float distance, b2Body* sheepBody);
    
    void handleCollisionWithInteractiveBlock(LHNodeProtocol* block, bool begin);
    bool checkIfIsOnMovable(LHNodeProtocol* shape, bool begin);
    
    SheepState* _currentState;
    float _horizontalImpulse;
    float _verticalImpulse;
    float _horizontalVelLimit;
    float _noteDistanceActivation;
    float _distanceFromPlayer;
    
    bool _isExcited;
    bool _hasContactToTheSide;
    bool _hasContactBelow;
    bool _isBehindInteractive;
    bool _isDead;
    bool _isBurning;
    bool _isPlayerOver;
    bool _isDoorInMovement;
    bool _isOnGoalRight;
    
    eGroundType _groundType;
    
    LHNodeProtocol* _movablePlatform;  // Se diversa da NULL pecora su una piattaforma mobile
    
    SoundComponent * _soundExcited;
    SoundComponent * _soundIdle2;
    SoundComponent * _soundIdle3;
    SoundComponent * _soundBurn;
    SoundComponent * _soundJump;
    SoundComponent * _soundLand;
    SoundComponent * _soundWalk;
    
    AnimationComponent * _animationWalk;
    AnimationComponent * _animationJump;
    AnimationComponent * _animationIdle1;
    AnimationComponent * _animationIdle2;
    AnimationComponent * _animationIdle3;// animation 4
    AnimationComponent * _animationFall;
    AnimationComponent * _animationBurn;
    AnimationComponent * _animationCatch;
    AnimationComponent * _animationFly;
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
};

#endif // __SOP_SHEEP__
