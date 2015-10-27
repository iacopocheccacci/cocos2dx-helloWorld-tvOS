//
//  Sheep.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 11/11/14.
//
//

#include "Sheep.h"
#include "Utility.h"
#include "SheepStandingState.h"

#include "SoundComponent.h"
#include "AnimationComponent.h"

#if (CC_TARGET_PLATFORM != CC_PLATFORM_TVOS)
    #define MOBILE
#endif

using namespace gameObjectsParams;

Sheep::~Sheep()
{
    if (_currentState)
    {
        delete _currentState;
    }
    
    this->unscheduleUpdate();
}

bool Sheep::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
//        this->initAudio();
        this->initAnimation();

        _currentState = new SheepStandingState(this);
        _isExcited = false;
        
        _hasContactToTheSide = false;
        _hasContactBelow = false;
        _isBehindInteractive = false;
        
        _movablePlatform = nullptr;
        _isDead = false;
        _isBurning = false;
        _isPlayerOver = false;
        
        _groundType = eGroundType::NONE;
        _eoloStartOffset = Point(0.0f, 0.0f);
        
        _isDoorInMovement = false;
        _isOnGoalRight = false;
        
        this->loadSheepData();
        this->scheduleUpdate();
        
        // Ghost Vertex
//        Utility::applyBottomGhostVertex(this);
    }
    
    return status;
}

void Sheep::initAudio(LHNode * listenerNode)
{
    if (this->containsTag(gameTags::MONSTER_TAG))
    {
        _soundExcited = SoundComponent::create(SoundComponent::SOUND_MONSTER_CATCH);
    }
    else
    {
        _soundExcited = SoundComponent::create(SoundComponent::SOUND_SHEEP_CATCH);
    }
    this->addChild(_soundExcited);
    
    _soundIdle2 = SoundComponent::create(SoundComponent::SOUND_SHEEP_IDLE_2);
    _soundIdle2->setPositionRefNode(listenerNode);
    this->addChild(_soundIdle2);
    
    if (this->containsTag(gameTags::MONSTER_TAG))
    {
        _soundIdle3 = SoundComponent::create(SoundComponent::SOUND_MONSTER_IDLE_3);
    }
    else
    {
        _soundIdle3 = SoundComponent::create(SoundComponent::SOUND_SHEEP_IDLE_3);
    }
    _soundIdle3->setPositionRefNode(listenerNode);
    this->addChild(_soundIdle3);
    
    _soundBurn = SoundComponent::create(SoundComponent::SOUND_SHEEP_BURN);
    this->addChild(_soundBurn);
    
    _soundJump = SoundComponent::create(SoundComponent::SOUND_SHEEP_JUMP);
    this->addChild(_soundJump);
    
    _soundLand = SoundComponent::create(SoundComponent::SOUND_SHEEP_LAND);
    this->addChild(_soundLand);
    
    _soundWalk = SoundComponent::create(SoundComponent::SOUND_SHEEP_WALKING);
    this->addChild(_soundWalk);
}

void Sheep::initAnimation()
{
    auto sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        if (this->containsTag(gameTags::MONSTER_TAG))
        {
            _animationIdle1 = AnimationComponent::create(sprite, AnimationComponent::MONSTER_IDLE_1);
            _animationIdle1->setFrameNumber(1);
            _animationIdle1->setStartingFrame(1);
            this->addChild(_animationIdle1);
            
            _animationIdle2 = AnimationComponent::create(sprite, AnimationComponent::MONSTER_IDLE_2);
            _animationIdle2->setFrameNumber(20);
            _animationIdle2->setStartingFrame(1);
            this->addChild(_animationIdle2);
            
            _animationIdle3 = nullptr;
            
            _animationWalk = AnimationComponent::create(sprite, AnimationComponent::MONSTER_WALK);
            _animationWalk->setFrameNumber(8);
            _animationWalk->setStartingFrame(1);
            this->addChild(_animationWalk);
            
            _animationJump = AnimationComponent::create(sprite, AnimationComponent::MONSTER_JUMP);
            _animationJump->setFrameNumber(3);
            _animationJump->setStartingFrame(1);
            this->addChild(_animationJump);
            
            _animationFall = AnimationComponent::create(sprite, AnimationComponent::MONSTER_FALL);
            _animationFall->setFrameNumber(3);
            _animationFall->setStartingFrame(1);
            this->addChild(_animationFall);
            
            _animationBurn = AnimationComponent::create(sprite, AnimationComponent::MONSTER_BURN);
            _animationBurn->setFrameNumber(24);
            _animationBurn->setStartingFrame(1);
            this->addChild(_animationBurn);
            
            _animationCatch = AnimationComponent::create(sprite, AnimationComponent::MONSTER_CATCH);
            _animationCatch->setCustomFrames({1,3,2,2,3,1});
            this->addChild(_animationCatch);
            
            _animationFly = AnimationComponent::create(sprite, AnimationComponent::MONSTER_FLY);
            _animationFly->setFrameNumber(8);
            _animationFly->setStartingFrame(1);
            this->addChild(_animationFly);
        }
        else
        {
            _animationIdle1 = AnimationComponent::create(sprite, AnimationComponent::SHEEP_IDLE_1);
            _animationIdle1->setFrameNumber(12);
            this->addChild(_animationIdle1);
            
            _animationIdle2 = AnimationComponent::create(sprite, AnimationComponent::SHEEP_IDLE_2);
            _animationIdle2->setFrameNumber(24);
            this->addChild(_animationIdle2);
            
            _animationIdle3 = AnimationComponent::create(sprite, AnimationComponent::SHEEP_IDLE_3);
            _animationIdle3->setFrameNumber(10);
            this->addChild(_animationIdle3);
            
            _animationWalk = AnimationComponent::create(sprite, AnimationComponent::SHEEP_WALK);
            _animationWalk->setFrameNumber(8);
            this->addChild(_animationWalk);
            
            _animationJump = AnimationComponent::create(sprite, AnimationComponent::SHEEP_JUMP);
            _animationJump->setFrameNumber(3);
            this->addChild(_animationJump);
            
            _animationFall = AnimationComponent::create(sprite, AnimationComponent::SHEEP_FALL);
            _animationFall->setFrameNumber(2);
            this->addChild(_animationFall);
            
            _animationBurn = AnimationComponent::create(sprite, AnimationComponent::SHEEP_BURN);
            _animationBurn->setFrameNumber(24);
            this->addChild(_animationBurn);
            
            _animationCatch = AnimationComponent::create(sprite, AnimationComponent::SHEEP_CATCH);
            _animationCatch->setFrameNumber(11);
            this->addChild(_animationCatch);
            
            _animationFly = AnimationComponent::create(sprite, AnimationComponent::SHEEP_FLY);
            _animationFly->setFrameNumber(8);
            _animationFly->setStartingFrame(1);
            this->addChild(_animationFly);
        }
    }
}

void Sheep::handleInput(eAction input)
{
    auto state = _currentState->handleInput(this, input);
    if (state)
    {
        delete _currentState;
        _currentState = state;
    }
}

void Sheep::update(float dt)
{
    _currentState->update(this, dt);
    this->updateAudio(dt);
    
    this->handleVelocityOnDoorMovement();
    
//    if (!_isExcited)
//    {
//        this->getBox2dBody()->SetLinearVelocity(b2Vec2(this->getHorizontalZero(), this->getBox2dBody()->GetLinearVelocity().y));
//    }
}

void Sheep::updateAudio(float dt)
{
    if (_soundIdle2)
    {
        _soundIdle2->updateParameterDistanceFromRefNode();
    }
    
    if (_soundIdle2)
    {
        _soundIdle2->updateParameterDirectionTowardsRefNode();
    }
    
    if (_soundIdle3)
    {
        _soundIdle3->updateParameterDistanceFromRefNode();
        _soundIdle3->updateParameterDirectionTowardsRefNode();
    }
}

void Sheep::setUnderEoloWind(LHNode* node, bool begin)
{
    //TODO: cambiare animazione pecora sotto effetto di Eolo
    if (begin)
    {
        auto gameWorld = node->getParent();
        auto objectAsset = this->getParent();
        
        if (gameWorld && objectAsset)
        {
            Point screenCoord = gameWorld->convertToWorldSpace(node->getPosition());
            Point relativePosition = objectAsset->convertToNodeSpace(screenCoord);
            
            _eoloStartOffset = this->getPosition() - relativePosition;
        }
        
        this->handleInput(eAction::FLY);
    }
    else
    {
        this->handleInput(eAction::STOP_FLY);
    }
}

void Sheep::updateUnderEoloWind(LHNode *node)
{
    auto gameWorld = node->getParent();
    auto objectAsset = this->getParent();
    
    if (gameWorld && objectAsset)
    {
        Point screenCoord = gameWorld->convertToWorldSpace(node->getPosition());
        Point relativePosition = objectAsset->convertToNodeSpace(screenCoord);
        
        this->setPosition(relativePosition + _eoloStartOffset);
    }
}

void Sheep::exitUnderEoloWind()
{
    // do nothing
}

void Sheep::updateSheepStatus(b2Vec2 playerPosition, bool isInverseEffect, int &index)
{
    if (!_isDead)
    {
        auto input = eAction::STOP_MOVE_RIGHT;  // STOP_MOVE_RIGHT o STOP_MOVE_LEFT in questo caso sono equivalenti
        auto sheepBody = this->getBox2dBody();

		if (sheepBody)
		{
			b2Vec2 sheepPosition = sheepBody->GetPosition();
            float distance;
            
            // Nel caso di pecora mostro sotto nota inversa, si sposta il calcolo della distanza
            // in modo da tenerla sempre più a destra delle altre.
            if (this->containsTag(gameTags::MONSTER_TAG) && isInverseEffect)
            {
                auto side = _isOnGoalRight ? b2Vec2(-1.5f,0.0f) : b2Vec2(1.5f,0.0f);
                distance = b2Distance(playerPosition + side, sheepPosition);
            }
            else
            {
                 distance = b2Distance(playerPosition, sheepPosition);
            }

			if (distance < _noteDistanceActivation)
			{
				float horizontalDistance = playerPosition.x - sheepPosition.x;

				if (!isInverseEffect)
				{
					input = this->handleNoteNormalEffect(horizontalDistance, sheepBody, index);
				}
				else
				{
					input = this->handleNoteInverseEffect(horizontalDistance, sheepBody);
				}

				index++;
			}
			else
			{
				_isExcited = false;
			}

			this->handleInput(input);
		}
    }
}

void Sheep::handleCollisionWithObstacle()
{
    if (isPossibleToJump() && !_isDead)
    {
        b2Body* sheepBody = this->getBox2dBody();
        float jumpImpulse;
        float limit = _horizontalVelLimit - (_horizontalVelLimit/100)*95;
        
        if (_movablePlatform)
        {
            // se la pecora salta mentre è sopra una piattaforma mobile si resetta la sua velocità su y in modo da
            // evitare "super salti" dovuti dalla velocità della piattaforma.
            this->getBox2dBody()->SetLinearVelocity(b2Vec2(this->getBox2dBody()->GetLinearVelocity().x, .0f));
        }
        
        // L'impulso verticale viene di seguito regolato in base alla velocità della pecora.
        // L'impulso verticale è inversamente propozionale alla velocità, questo per garantire che
        // salti in alto 2 da ferma e che non salti 5 in lungo.
        if (abs(sheepBody->GetLinearVelocity().x) < limit )
        {
            jumpImpulse = _verticalImpulse;
        }
        else
        {
            jumpImpulse = _verticalImpulse - (_verticalImpulse/100)*35;
        }
        
        b2Vec2 impulse(0.0f, jumpImpulse);
        
        _hasContactBelow = false;
        _hasContactToTheSide = false;
        
        sheepBody->ApplyLinearImpulse(impulse, sheepBody->GetWorldCenter(), true);
        this->handleInput(eAction::JUMP);
    }
}

void Sheep::setContactBelow(LHNodeProtocol *platform, bool begin)
{
    _hasContactBelow = begin;
    if (begin && platform)
    {
        _groundType = Utility::getGroundTypeFromNode(platform);
        
        std::string groundStr = SoundComponent::getStringFromGround(_groundType);
        char soundName[30];
       
        if (!groundStr.empty())
        {
            snprintf(soundName, 30, SoundComponent::SOUND_SHEEP_LAND, groundStr.c_str());
        }
        else
        {
            snprintf(soundName, 30, "%s", "");
        }
        _soundLand->setSoundName(soundName);
    }
    else
    {
        _groundType = eGroundType::NONE;
    }

}

void Sheep::setIsOnMovablePlatform(LHNodeProtocol* platform, bool begin)
{
    if (begin)
    {
        _movablePlatform = platform;
    }
    else
    {
        _movablePlatform = nullptr;
    }
}

float Sheep::getVerticalZero()
{
    float actualVerticalZero = 0.0f;
    
    // set relative velocity if on movable platform
    LHPhysicsProtocol * movablePhysics = dynamic_cast<LHPhysicsProtocol*>(_movablePlatform);
    if (movablePhysics)
    {
        actualVerticalZero = /*fabs*/(movablePhysics->getBox2dBody()->GetLinearVelocity().y);
    }
    
    return actualVerticalZero;
}

float Sheep::getHorizontalZero()
{
    float actualHorizontalZero = 0.0f;
    
    // set relative velocity if on movable platform
    LHPhysicsProtocol * movablePhysics = dynamic_cast<LHPhysicsProtocol*>(_movablePlatform);
    if (movablePhysics)
    {
        actualHorizontalZero = fabs(movablePhysics->getBox2dBody()->GetLinearVelocity().x);
    }
    
    return actualHorizontalZero;
}

void Sheep::burn()
{
    _isDead = true;
    _isBurning = true;
    this->handleInput(eAction::BURN);
}

void Sheep::playJumpOnSheep()
{
    if  (!_isPlayerOver)
    {
        this->handleInput(eAction::JUMP_ON);
    }
    
//    _currentState->stopAnimation();
//    this->getAnimationCatch()->startAnimation(12, false);
}

void Sheep::handleVelocityOnDoorMovement()
{
    if (_movablePlatform && _movablePlatform->containsTag(gameTags::DOOR_TAG))
    {
        auto movablePhysics = dynamic_cast<LHPhysicsProtocol*>(_movablePlatform);
        
        if (movablePhysics)
        {
            auto movableBody = movablePhysics->getBox2dBody();
            if (movableBody)
            {
                if (fabs(movableBody->GetLinearVelocity().x) > 0.1f)
                {
                    _isDoorInMovement = true;
                    this->getBox2dBody()->SetLinearVelocity(movableBody->GetLinearVelocity());
                }
                else if (_isDoorInMovement)
                {
                    _isDoorInMovement = false;
                    this->getBox2dBody()->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
                }
            }
        }
    }

}

bool Sheep::isJumping()
{
    bool jump = true;
    float actualVerticalZero = 0.1f;
    float verticalVelocity = this->getBox2dBody()->GetLinearVelocity().y;
    
    actualVerticalZero += this->getVerticalZero();
    
    if (_hasContactBelow && (/*fabs*/(verticalVelocity) <= actualVerticalZero))
    {
        jump = false;
    }
    
    return jump;
}

void Sheep::handleBlockSideContact(LHNodeProtocol *block, bool begin)
{
    //CCLOG("Sheep: Contact to the side %s", begin ? "started" : "ended");
    this->setContactToTheSide(begin);
    
    if (block->containsTag(gameTags::INTERACTIVE_TAG))
    {
        this->handleCollisionWithInteractiveBlock(block, begin);
    }
}

void Sheep::handleBlockBelowContact(LHNodeProtocol *block, bool begin)
{
    //CCLOG("Sheep: Contact below %s", begin ? "started" : "ended");
    this->setContactBelow(block, begin);

    
    auto vel = this->getBox2dBody()->GetLinearVelocity().y;
    auto zero = this->getVerticalZero() - 0.2f;
    if (!begin && (vel < zero))
    {
        this->handleInput(eAction::FALL);
    }
    else if (begin)
    {
        this->handleInput(eAction::STOP_FALL);
    }
    
    if (this->checkIfIsOnMovable(block, begin))
    {
        this->setIsOnMovablePlatform(block, begin);
    }
}

bool Sheep::isPossibleToJump()
{
    bool jump = false;
    
    if (!this->isJumping() && _hasContactToTheSide)
    {
        jump = true;
    }
    
    return jump;
}

void Sheep::loadSheepData()
{
    auto value = Utility::loadDataFromFile(SHEEP_GROUP, SHEEP_HOR_IMP_NAME, SHEEP_HOR_IMP_VAL);
    _horizontalImpulse = atof(value->getCString());
    
    value = Utility::loadDataFromFile(SHEEP_GROUP,SHEEP_VER_IMP_NAME, SHEEP_VER_IMP_VAL);
    _verticalImpulse = atof(value->getCString());
    
#ifndef MOBILE
    _verticalImpulse += .5f;
#endif
    
    value = Utility::loadDataFromFile(SHEEP_GROUP, SHEEP_HOR_LIMIT_NAME, SHEEP_HOR_LIMIT_VAL);
    _horizontalVelLimit = atof(value->getCString());
    
    value = Utility::loadDataFromFile(NOTE_GROUP, NOTE_DISTANCE_NAME, NOTE_DISTANCE_VAL);
    _noteDistanceActivation = atof(value->getCString());
    
    value = Utility::loadDataFromFile(NOTE_GROUP, NOTE_SHEEP_PAN_DIST_NAME, NOTE_SHEEP_PAN_DIST_VAL);
    _distanceFromPlayer = atof(value->getCString());
}

eAction Sheep::handleNoteNormalEffect(float horizontalDistance, b2Body* sheepBody, int index)
{
    auto input = eAction::NONE;
    
    if (!this->containsTag(gameTags::MONSTER_TAG))
    {
        input = this->handleNoteNormalEffectToSheep(horizontalDistance, sheepBody, index);
    }
    else
    {
        input = this->handleNoteNormalEffectToMonster(horizontalDistance, sheepBody);
    }
    
    return input;
}

eAction Sheep::handleNoteNormalEffectToSheep(float horizontalDistance, b2Body *sheepBody, int index)
{
    auto input = eAction::NONE;
    float random = CCRANDOM_0_1();
    float relativeDistance = (_distanceFromPlayer > 0.0f) ? (_distanceFromPlayer + (index * 0.5f) + random) : (_distanceFromPlayer - (index * 1.0f) - random);
    
    if (fabs(horizontalDistance) > relativeDistance)
    {
        float horizontalVelocity = sheepBody->GetLinearVelocity().x;
        b2Vec2 impulse;
        
        if (horizontalDistance > relativeDistance)
        {
            impulse.Set(_horizontalImpulse, 0.0f);
            input = eAction::MOVE_RIGHT;
        }
        else if (horizontalDistance < -(relativeDistance))
        {
            impulse.Set(-(_horizontalImpulse), 0.0f);
            input = eAction::MOVE_LEFT;
        }
        
        if (fabs(horizontalVelocity) <= _horizontalVelLimit)
        {
            sheepBody->ApplyLinearImpulse(impulse, sheepBody->GetWorldCenter(), true);
        }
    }
    else
    {
        input = eAction::STOP_MOVE_RIGHT;
    }
    
    return input;
}

eAction Sheep::handleNoteNormalEffectToMonster(float horizontalDistance, b2Body *monsterBody)
{
    auto input = eAction::NONE;
    
    if (!_isOnGoalRight)
    {
        input = this->handleMonsterOnGoalLeft(horizontalDistance, monsterBody);
    }
    else
    {
        input = this->handleMonsterOnGoalRight(horizontalDistance, monsterBody);
    }
    
    return input;

}

eAction Sheep::handleMonsterOnGoalLeft(float horizontalDistance, b2Body *monsterBody)
{
    auto input = eAction::NONE;
    float relativeDistance = 0.0f;
    float horizontalVelocity = monsterBody->GetLinearVelocity().x;
    b2Vec2 impulse;
    
    if (horizontalDistance > _distanceFromPlayer)
    {
        relativeDistance = _distanceFromPlayer - 0.5f;
        
        if (horizontalDistance > relativeDistance)
        {
            impulse.Set(_horizontalImpulse, 0.0f);
            input = eAction::MOVE_RIGHT;
            
            if (fabs(horizontalVelocity) <= _horizontalVelLimit)
            {
                monsterBody->ApplyLinearImpulse(impulse, monsterBody->GetWorldCenter(), true);
            }
        }
        else
        {
            input = eAction::STOP_MOVE_RIGHT;
        }
    }
    else if (horizontalDistance < -(_distanceFromPlayer))
    {
        relativeDistance = _distanceFromPlayer + 2.5f;
        
        if (horizontalDistance < -(relativeDistance))
        {
            impulse.Set(-(_horizontalImpulse), 0.0f);
            input = eAction::MOVE_LEFT;
            
            if (fabs(horizontalVelocity) <= _horizontalVelLimit)
            {
                monsterBody->ApplyLinearImpulse(impulse, monsterBody->GetWorldCenter(), true);
            }
        }
        else
        {
            input = eAction::STOP_MOVE_RIGHT;
        }
    }
    else
    {
        input = eAction::STOP_MOVE_RIGHT;
    }

    return input;
}

eAction Sheep::handleMonsterOnGoalRight(float horizontalDistance, b2Body *monsterBody)
{
    auto input = eAction::NONE;
    float relativeDistance = 0.0f;
    float horizontalVelocity = monsterBody->GetLinearVelocity().x;
    b2Vec2 impulse;
    
    if (horizontalDistance > _distanceFromPlayer)
    {
        relativeDistance = _distanceFromPlayer + 2.5f;
        
        if (horizontalDistance > relativeDistance)
        {
            impulse.Set(_horizontalImpulse, 0.0f);
            input = eAction::MOVE_RIGHT;
            
            if (fabs(horizontalVelocity) <= _horizontalVelLimit)
            {
                monsterBody->ApplyLinearImpulse(impulse, monsterBody->GetWorldCenter(), true);
            }
        }
        else
        {
            input = eAction::STOP_MOVE_RIGHT;
        }
    }
    else if (horizontalDistance < -(_distanceFromPlayer))
    {
        relativeDistance = _distanceFromPlayer - 0.5f;
        
        if (horizontalDistance < -(relativeDistance))
        {
            impulse.Set(-(_horizontalImpulse), 0.0f);
            input = eAction::MOVE_LEFT;
            
            if (fabs(horizontalVelocity) <= _horizontalVelLimit)
            {
                monsterBody->ApplyLinearImpulse(impulse, monsterBody->GetWorldCenter(), true);
            }
        }
        else
        {
            input = eAction::STOP_MOVE_LEFT;
        }
        
    }
    else
    {
        input = eAction::STOP_MOVE_RIGHT;
    }

    return input;
}

eAction Sheep::handleNoteInverseEffect(float horizontalDistance, b2Body* sheepBody)
{
    auto input = eAction::NONE;
    float horizontalVelocity = sheepBody->GetLinearVelocity().x;
    b2Vec2 impulse;
    
    if (horizontalDistance >= 0.0f)
    {
        impulse.Set(-(_horizontalImpulse), 0.0f);
        input = eAction::MOVE_LEFT;
    }
    else if (horizontalDistance < 0.0f)
    {
        impulse.Set(_horizontalImpulse, 0.0f);
        input = eAction::MOVE_RIGHT;
    }
    else
    {
        input = eAction::STOP_MOVE_RIGHT;
    }
    
    if (fabs(horizontalVelocity) <= _horizontalVelLimit)
    {
        sheepBody->ApplyLinearImpulse(impulse, sheepBody->GetWorldCenter(), true);
    }
    
    return input;
}

void Sheep::handleCollisionWithInteractiveBlock(LHNodeProtocol *block, bool begin)
{
    if (block->containsTag(gameTags::INTERACTIVE_TAG))
    {
//        block->getBox2dBody()->SetGravityScale(3.0f);
        LHPhysicsProtocol * blockPhysics = dynamic_cast<LHPhysicsProtocol*>(block);
        if (blockPhysics)
        {
            Utility::setDensity(blockPhysics, 1.0f);
            _isBehindInteractive = false;
        }
    }
}

bool Sheep::checkIfIsOnMovable(LHNodeProtocol *node, bool begin)
{
    // se una pecora sale su una piattaforma mobile viene settato un flag che serve per non farle subire il contraccolpo del cambio di direzione della piattaforma
    bool res = false;
    
    if (node->containsTag(gameTags::MOVABLE_TAG) || node->containsTag(gameTags::DOOR_TAG))
    {
        res = true;
    }
    
    return res;
}

#pragma mark - Death Notification Stuff

void Sheep::setCallbackFunc(Ref *target, SEL_CallFuncN callback)
{
    _callbackListener = target;
    _callback = callback;
}

void Sheep::onExit()
{
    auto node = static_cast<Node*>(this);
    node->setName(callbackDefs::SHEEP_EXITING);
    
    if (_callback && _callbackListener)
    {
        (_callbackListener->*_callback)(node);
    }
    LHNode::onExit();
}
