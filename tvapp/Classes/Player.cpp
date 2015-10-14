//
//  Player.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#include "Player.h"
#include "Utility.h"
#include "PlayerState.h"
#include "PlayerStandingState.h"
#include "PlayerWalkingState.h"
#include "SoundComponent.h"
#include "AnimationComponent.h"
#include "NoteProgressBar.h"

using namespace gameObjectsParams;

Player::~Player()
{
    if (_currentState)
    {
        delete _currentState;
    }
    this->unscheduleUpdate();
}

bool Player::initWithDictionary(LHDictionary* dict, Node* prnt)
{
    bool status = LHNode::initWithDictionary(dict, prnt);
    
    if(status)
    {
        _isOnMenuLevel = this->checkIfInMenu();
        
        // Animazioni
        this->initAnimation();
        
        _currentState = new PlayerStandingState(this, false);
        _playing = false;
        
        _movingRight = false;
        _movingLeft = false;
        _jumping = false;
        
        _hasContactToTheSide = false;
        _hasContactBelow = false;
        
        _pushingLeft = false;
        _pushingRight = false;
        
        _movablePlatform = nullptr;
        _noteEffect = eNoteEffect::NO_EFFECT;
        
        _groundType = eGroundType::NONE;
        _isDead = false;
        
        _currentVelocity = Point::ZERO;
        _lastVelocity = Point::ZERO;
        
        _eoloStartOffset = Point(0.0f, 0.0f);
        _isUnderEoloEffect = false;
        
        _isBurning = false;
        _collisionDisabled = false;
        
        // Suoni
        this->initAudio();
        
        this->loadPlayerData();
        this->scheduleUpdate();
        
        // Ghost vertex
        //        Utility::applyBottomGhostVertex(this);
        //
    }
    
    return status;
}

void Player::initAudio()
{
    _soundWalk = SoundComponent::create(SoundComponent::SOUND_PAN_WALKING);
    this->addChild(_soundWalk);
    
    _soundFluteEnd = SoundComponent::create(SoundComponent::SOUND_FLUTE_END);
    this->addChild(_soundFluteEnd);
    
    _soundJump = SoundComponent::create(SoundComponent::SOUND_PAN_JUMP);
    this->addChild(_soundJump);
    
    _soundPush = SoundComponent::create(SoundComponent::SOUND_PAN_PUSH);
    this->addChild(_soundPush);
    
    _soundPushPant = SoundComponent::create(SoundComponent::SOUND_PAN_PUSH_PANT);
    this->addChild(_soundPushPant);
    
    _soundCatchNote = SoundComponent::create(SoundComponent::SOUND_PAN_CATCH_NOTE);
    this->addChild(_soundCatchNote);
    
    _soundDeath = SoundComponent::create(SoundComponent::SOUND_PAN_DIE);
    this->addChild(_soundDeath);
    
    _soundBurn = SoundComponent::create(SoundComponent::SOUND_PAN_BURN);
    this->addChild(_soundBurn);
    
    _soundExult = SoundComponent::create(SoundComponent::SOUND_PAN_EXULT);
    this->addChild(_soundExult);
}

void Player::initAnimation()
{
    auto sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    
    if (sprite)
    {
        _animationIdle = AnimationComponent::create(sprite, AnimationComponent::PLAYER_IDLE);
        this->addChild(_animationIdle);
        
        if (_isOnMenuLevel)
        {
            _animationWalk = AnimationComponent::create(sprite, AnimationComponent::PLAYER_WALK_MENU);
        }
        else
        {
            _animationWalk = AnimationComponent::create(sprite, AnimationComponent::PLAYER_WALK);
        }
        
        this->addChild(_animationWalk);
        
        _animationJump = AnimationComponent::create(sprite, AnimationComponent::PLAYER_JUMP);
        this->addChild(_animationJump);
        
        _animationFall = AnimationComponent::create(sprite, AnimationComponent::PLAYER_FALL);
        this->addChild(_animationFall);
        
        _animationRebounce = AnimationComponent::create(sprite, AnimationComponent::PLAYER_REBOUNCE);
        this->addChild(_animationRebounce);
        
        _animationPlayingIdle = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PLAYING_IDLE);
        this->addChild(_animationPlayingIdle);
        
        _animationPlayingWalk = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PLAYING_WALK);
        this->addChild(_animationPlayingWalk);
        
        _animationPlayingJump = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PLAYING_JUMP);
        this->addChild(_animationPlayingJump);
        
        _animationPlayingFall = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PLAYING_FALL);
        this->addChild(_animationPlayingFall);
        
        _animationPlayingRebounce = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PLAYING_REBOUNCE);
        this->addChild(_animationPlayingRebounce);
        
        _animationPush = AnimationComponent::create(sprite, AnimationComponent::PLAYER_PUSHING);
        this->addChild(_animationPush);
        
        _animationStartPush = AnimationComponent::create(sprite, AnimationComponent::PLAYER_START_PUSHING);
        this->addChild(_animationStartPush);
        
        _animationBurn = AnimationComponent::create(sprite, AnimationComponent::PLAYER_BURNING);
        this->addChild(_animationBurn);
        
        _animationSlide = AnimationComponent::create(sprite, AnimationComponent::PLAYER_SLIDING);
        this->addChild(_animationSlide);
        
        _animationExultFall = AnimationComponent::create(sprite, AnimationComponent::PLAYER_EXULT_FALL);
        this->addChild(_animationExultFall);
        
        _animationExultRebounce = AnimationComponent::create(sprite, AnimationComponent::PLAYER_EXULT_REBOUNCE);
        this->addChild(_animationExultRebounce);
        
        _animationFly = AnimationComponent::create(sprite, AnimationComponent::PLAYER_FLY);
        _animationFly->setStartingFrame(1);
        this->addChild(_animationFly);
    }
}

void Player::handleInput(eAction input)
{
    PlayerState* state = _currentState->handleInput(this, input);
    if (state)
    {
        delete _currentState;
        _currentState = state;
    }
}

void Player::update(float dt)
{
    _currentState->update(this);
    
    this->updateLastVelocity();
}

void Player::setContactBelow(LHNodeProtocol * platform, bool begin)
{
    _hasContactBelow = begin;
    _collisionDisabled = false;
    if (begin && platform)
    {
        _groundType = Utility::getGroundTypeFromNode(platform);
        
        std::string groundStr = SoundComponent::getStringFromGround(_groundType);
        char soundName[30];
        
        if (!groundStr.empty())
        {
            snprintf(soundName, 30, SoundComponent::SOUND_PAN_WALKING, groundStr.c_str());
        }
        else
        {
            snprintf(soundName, 30, "%s", "");
        }

        _soundWalk->setSoundName(soundName);
        
        if (eGroundType::TEMPLE == _groundType)
        {
            _soundJump->setSoundName(SoundComponent::SOUND_PAN_JUMP_TEMPLE);
        }
        else
        {
            _soundJump->setSoundName(SoundComponent::SOUND_PAN_JUMP);
        }
        
        this->handleInput(eAction::STOP_FALL);
    }
    else
    {
        auto vel = this->getBox2dBody()->GetLinearVelocity().y;
        auto zero = (this->getVerticalZero() - 0.2f);
        if ( vel < zero)
        {
            this->handleInput(eAction::FALL);
        }
        
        _groundType = eGroundType::NONE;
    }
}

void Player::movePlayerRight()
{
    b2Vec2 impulse(_horizontalImpulse, 0.0f);
    b2Body* body = this->getBox2dBody();
    float horizontalVelocity = body->GetLinearVelocity().x;
    if (horizontalVelocity <= _horizontalVelLimit)
    {
        // il controllo seguente evita che il salto accanto ad un ostacolo mentre si preme una direzione sia più basso rispetto
        // al salto da fermo. Per farlo si blocca l'impulso orizzontale
        
        if (!(_jumping && _pushingRight))
        {
            body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        }
    }
}

void Player::movePlayerLeft()
{
    b2Vec2 impulse(-(_horizontalImpulse), 0.0f);
    b2Body* body = this->getBox2dBody();
    float horizontalVelocity = body->GetLinearVelocity().x;
    if (horizontalVelocity >= -(_horizontalVelLimit))
    {
        // il controllo seguente evita che il salto accanto ad un ostacolo mentre si preme una direzione sia più basso rispetto
        // al salto da fermo. Per farlo si blocca l'impulso orizzontale
        
        if (!(_jumping && _pushingLeft))
        {
            body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        }
    }
}

void Player::movePlayerJump()
{
    if (isPossibleToJump())
    {
        if (_movablePlatform)
        {
            // se Pan salta mentre è sopra una piattaforma mobile si resetta la sua velocità su y in modo da evitare
            // "super salti" dovuti dalla velocità della piattaforma.
            this->getBox2dBody()->SetLinearVelocity(b2Vec2(this->getBox2dBody()->GetLinearVelocity().x, 0.f));
        }
        
        b2Vec2 impulse(0.0f, _verticalImpulse);
        b2Body* body = this->getBox2dBody();
        
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
        
        if (!this->_playing)
        {
            _soundJump->playOnce();
        }
    }
}

void Player::littleJumpOnInteractive()
{
    if (isPossibleToJump() && !_isDead)
    {
        b2Vec2 impulse(0.0f, _verticalImpulse/1.4f);
        b2Body* body = this->getBox2dBody();
        
        body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
    }
}

void Player::littleJumpOnSheep()
{
    b2Vec2 impulse(0.0f, 0.5f);
    b2Body* body = this->getBox2dBody();
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
}

float Player::getVerticalZero()
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

float Player::getHorizontalZero()
{
    float actualHorizontalZero = 0.0f;
    
    // set relative velocity if on movable platform
    LHPhysicsProtocol * movablePhysics = dynamic_cast<LHPhysicsProtocol*>(_movablePlatform);
    if (movablePhysics)
    {
        actualHorizontalZero = (movablePhysics->getBox2dBody()->GetLinearVelocity().x);
    }
    
    return actualHorizontalZero;
}

void Player::burn()
{
    _isDead = true;
    _isBurning = true;
    this->handleInput(eAction::BURN);
}

bool Player::isPossibleToJump()
{
    bool jump = false;
    float actualVerticalZero = 0.1f;
    float verticalVelocity = this->getBox2dBody()->GetLinearVelocity().y;
    
    actualVerticalZero += this->getVerticalZero();
    
    if (_hasContactBelow && (/*fabs*/(verticalVelocity - 0.1f) <= actualVerticalZero))
    {
        jump = true;
    }
    
    return jump;
}

bool Player::isVerticalOnlyJump()
{
    bool isVerticalOnlyJump = false;
    b2Body* body = this->getBox2dBody();
    float horizontalVelocity = body->GetLinearVelocity().x;
    
    if (fabsf(horizontalVelocity) <= (_horizontalVelLimit / 3.0f))
    {
        isVerticalOnlyJump = true;
    }
    
    return isVerticalOnlyJump;
}

void Player::oppositeImpulse()
{
    float stopFactor = isPossibleToJump() ? 3.0f : 15.0f;
    
    b2Body* body = this->getBox2dBody();
    float horizontalVelocity = -(body->GetLinearVelocity().x);
    b2Vec2 impulse(horizontalVelocity/stopFactor, 0.0f);
    
    body->ApplyLinearImpulse(impulse, body->GetWorldCenter(), true);
}

void Player::enableNoteParticles()
{
    Color4F colorEffect;
    Color3B colorNote;
    
    switch (_noteEffect) {
        case eNoteEffect::NORMAL_NOTE:
            colorEffect = Color4F(Color3B(118, 223, 239));
//            colorEffect = Color4F(Color3B(51,51,204));
            colorNote = Color3B(0, 245, 255);
//            colorNote = Color3B(118, 223, 239);
            break;
        case eNoteEffect::INVERSE_NOTE:
            colorEffect = Color4F(Color3B(204,51,51));
            colorNote = Color3B(237, 19, 26);
            break;
        default:
            break;
    }
    
    Point effectPosition = Point(this->getContentSize().width/2.0f,
                                 this->getContentSize().height - this->getContentSize().width/2.0f);
    
    // Effetto particellare iniziale
    ParticleSystemQuad * emitterStart = ParticleSystemQuad::create(playerDefs::PARTICLE_START_FILE);
    emitterStart->setPosition(effectPosition);
    emitterStart->setPositionType(ParticleSystem::PositionType::RELATIVE);
    emitterStart->setAutoRemoveOnFinish(true);
    this->addChild(emitterStart,4);
    emitterStart->setStartColor(Color4F(1, 1, 1, 1));
    emitterStart->setStartColorVar(Color4F(0, 0, 0, 0));
    emitterStart->setEndColor(colorEffect);
    emitterStart->setEndColorVar(Color4F(0, 0, 0, 0));
    
    // Effetto particellare continuo
    float enteringTime = 0.5f;
    ScaleTo * scaleIn = ScaleTo::create(enteringTime, _particleScale);
    ParticleSystemQuad * emitterRadial = dynamic_cast<ParticleSystemQuad *>(this->getChildByName(playerDefs::PARTICLE_NAME));
    if (!emitterRadial)
    {
        emitterRadial = ParticleSystemQuad::create(playerDefs::PARTICLE_FILE);
        emitterRadial->setPosition(effectPosition);
        emitterRadial->setScale(0.0f);
        emitterRadial->setPositionType(ParticleSystem::PositionType::RELATIVE);
        this->addChild(emitterRadial,4,playerDefs::PARTICLE_NAME);
        emitterRadial->runAction(scaleIn);
    }
    emitterRadial->setStartColor(Color4F(1, 1, 1, 1));
    emitterRadial->setStartColorVar(Color4F(0, 0, 0, 0));
    emitterRadial->setEndColor(colorEffect);
    emitterRadial->setEndColorVar(Color4F(0, 0, 0, 0));
    
    auto progressNote = dynamic_cast<NoteProgressBar*>(this->getChildByTag(NOTE_BAR));
    
    if (!progressNote)
    {
        progressNote = NoteProgressBar::create();
        progressNote->setScale(0.87f);
        progressNote->setPosition(effectPosition);
        this->addChild(progressNote, 4, NOTE_BAR);
    }
    
    progressNote->setNoteColor(colorNote);
}

void Player::generateParticleEmitterByName(std::string particleName)
{
    string colorName = "";
    
    switch (_noteEffect) {
        case eNoteEffect::NORMAL_NOTE:
            colorName = "Blue";
            break;
        case eNoteEffect::INVERSE_NOTE:
            colorName = "Red";
            break;
        default:
            break;
    }
    
    Point emitterPosition = Vec2(this->getContentSize().width/2, this->getContentSize().height/2);
    
    float enteringTime = 0.5f;
    ScaleTo * scaleIn = ScaleTo::create(enteringTime, _particleScale);
    
    this->removeParticleEmitterByName(particleName);
    auto emitter = ParticleSystemQuad::create(particleName + colorName + ".plist");
    emitter->setPosition(emitterPosition);
    emitter->setScale(0.0f);
    emitter->setPositionType(ParticleSystem::PositionType::RELATIVE);
    this->addChild(emitter,4,particleName);
    emitter->runAction(scaleIn);
}

void Player::removeParticleEmitterByName(std::string particleName)
{
    ParticleSystemQuad * emitter = dynamic_cast<ParticleSystemQuad *>(this->getChildByName(particleName));
    if (emitter)
    {
        emitter->setAutoRemoveOnFinish(true);
        emitter->stopSystem();
    }
}

void Player::updateEmitter(float dt)
{
    // TODO
}

void Player::disableNoteParticles()
{
    // Sistema a particelle (quello pesante)
    float exitingTime = 0.5f;
    ScaleTo * scaleOut = ScaleTo::create(exitingTime, 0);
    DelayTime * delayForRemove = DelayTime::create(exitingTime + 0.1f);
    ParticleSystem * particleRadial = dynamic_cast<ParticleSystem *>(this->getChildByName(playerDefs::PARTICLE_NAME));
    if (particleRadial)
    {
        particleRadial->setName("");
        particleRadial->stopSystem();
        particleRadial->runAction(scaleOut);
        particleRadial->runAction(Sequence::create(delayForRemove, RemoveSelf::create(), NULL));
    }
}

void Player::setPushingSide(float direction, bool begin)
{
    this->setPushingSide(direction, begin, nullptr);
}

void Player::setPushingSide(float direction, bool begin, LHNodeProtocol * block)
{
    bool checkPushSound = false;
    if (direction > 0.0f)
    {
        if (_pushingRight != begin)
        {
            _pushingRight = begin;
            checkPushSound = begin;
        }
    }
    else
    {
        if (_pushingLeft != begin)
        {
            _pushingLeft = begin;
            checkPushSound = begin;
        }
    }
    
    if (checkPushSound)
    {
        if (block && block->containsTag(gameTags::INTERACTIVE_TAG) && dynamic_cast<PlayerWalkingState *>(_currentState))
        {
            _soundPushPant->playOnce();
            if (false == block->containsTag(gameTags::GOATSKIN_TAG))
            {
                _soundPush->playOnce();
            }
        }
    }
}

void Player::setIsOnMovablePlatform(LHNodeProtocol *platform, bool begin)
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

void Player::loadPlayerData()
{
    auto value = Utility::loadDataFromFile(PLAYER_GROUP, PLAYER_HOR_IMP_NAME, PLAYER_HOR_IMP_VAL);
    _horizontalImpulse = atof(value->getCString());
    
    value = Utility::loadDataFromFile(PLAYER_GROUP, PLAYER_VER_IMP_NAME, PLAYER_VER_IMP_VAL);
    _verticalImpulse = atof(value->getCString());
    
    value = Utility::loadDataFromFile(PLAYER_GROUP, PLAYER_HOR_LIMIT_NAME, PLAYER_HOR_LIMIT_VAL);
    _horizontalVelLimit = atof(value->getCString());
    
    value = Utility::loadDataFromFile(NOTE_GROUP, NOTE_PARTICLE_SCALE_NAME, NOTE_PARTICLE_SCALE_VAL);
    _particleScale = atof(value->getCString());
}

void Player::handleJumpingStateEnd(bool isJumpPressed)
{
    if (this->isPossibleToJump())
    {
        if (this->isJumping())
        {
            if (!isJumpPressed)
            {
                this->handleInput(eAction::STOP_JUMP);
            }
            else
            {
                this->handleInput(eAction::JUMP);
            }
        }
    }
}

void Player::handleOnBlockBorder(b2Vec2 distance, LHNodeProtocol *block, bool begin)
{
    // TODO: implementare l'animazione di Pan in bilico?
    //_animationSlide->startAnimation(14, false);
    
    this->setContactBelow(block, begin);
    
    if (this->checkIfIsOnMovable(block, begin))
    {
        this->setIsOnMovablePlatform(block, begin);
    }
    
    // TODO: questa sezione è una sorta di antibaco. Gestire meglio i singoli casi
    if (!begin)
    {
        this->setContactToTheSide(begin);
        //        this->setPushingSide(distance.x, begin);
        
        if (block->containsTag(gameTags::INTERACTIVE_TAG))
        {
            this->setPushingSide(distance.x, begin, block);
            this->handleCollisionWithInteractiveBlock(block, begin);
            if (_playing)
            {
                this->littleJumpOnInteractive();
            }
        }
        else
        {
            this->setPushingSide(distance.x, begin);
        }
    }
}

void Player::handleBlockSideContact(b2Vec2 distance, LHNodeProtocol *block, bool begin)
{
    //CCLOG("Player: Contact to the side %s", begin ? "started" : "ended");
    this->setContactToTheSide(begin);
    
    if (!block->containsTag(gameTags::ONESIDE_TAG) && !this->getIsPlaying())
    {
        LHPhysicsProtocol * blockPhysics = dynamic_cast<LHPhysicsProtocol*>(block);
        if (begin
            || !block->containsTag(gameTags::INTERACTIVE_TAG)
            || (blockPhysics && fabs(blockPhysics->getBox2dBody()->GetLinearVelocity().x) < 1.0f))
        {
            // Questo set viene fatto in ogni caso tranne quando si sta trascinando un interactive e begin = false,
            // perché mentre si spinge un oggetto si verifica un continuo inizio-fine collisione che non permette di
            // agire in modo deterministico sullo stato del player e dell'oggetto interactive.
            this->setPushingSide(distance.x, begin, block);
        }
    }
    
    if (block->containsTag(gameTags::INTERACTIVE_TAG))
    {
        this->handleCollisionWithInteractiveBlock(block, begin);
        
        if (_playing)
        {
            this->littleJumpOnInteractive();
        }
    }
}

void Player::handleBlockBelowContact(LHNodeProtocol *block, bool begin)
{
    //CCLOG("Player: Contact below %s", begin ? "started" : "ended");
    this->setContactBelow(block, begin);
    if (this->checkIfIsOnMovable(block, begin))
    {
        this->setIsOnMovablePlatform(block, begin);
    }
}

bool Player::checkIfIsOnMovable(LHNodeProtocol *shape, bool begin)
{
    // se Pan sale su una piattaforma mobile viene settato un flag che serve per non fargli subire il contraccolpo del cambio di direzione della piattaforma
    bool res = false;
    
    if (shape->containsTag(gameTags::MOVABLE_TAG) || shape->containsTag(gameTags::DOOR_TAG))
    {
        res = true;
    }
    
    return res;
}

void Player::handleCollisionWithInteractiveBlock(LHNodeProtocol *block, bool begin)
{
    if (block->containsTag(gameTags::INTERACTIVE_TAG))
    {
        LHPhysicsProtocol * physicsBlock = dynamic_cast<LHPhysicsProtocol*>(block);
        if (begin && !_playing && physicsBlock)
        {
            physicsBlock->getBox2dBody()->SetGravityScale(2.5f);
            Utility::setDensity(physicsBlock, 0.1f);
        }
    }
}

void Player::setPlayingState(const bool playing)
{
    this->setPlayingState(playing, true);
}

void Player::setPlayingState(const bool playing, const bool endSound)
{
    if (playing)
    {
        if (_noteEffect == eNoteEffect::NORMAL_NOTE)
        {
            if (_soundFluteRedStart && true == _soundFluteRedStart->isPlaying())
            {
                _soundFluteRedStart->stopPlay();
            }
            if (_soundFluteStart && false == _soundFluteStart->isPlaying())
            {
                _soundFluteStart->startPlay();
            }
        }
        else if (_noteEffect == eNoteEffect::INVERSE_NOTE)
        {
            if (_soundFluteStart && true == _soundFluteStart->isPlaying())
            {
                _soundFluteStart->stopPlay();
            }
            if (_soundFluteRedStart && false == _soundFluteRedStart->isPlaying())
            {
                _soundFluteRedStart->startPlay();
            }
        }
    }
    else
    {
        if (_soundFluteStart && true == _soundFluteStart->isPlaying())
        {
            _soundFluteStart->stopPlay();
        }
        if (_soundFluteRedStart && true == _soundFluteRedStart->isPlaying())
        {
            _soundFluteRedStart->stopPlay();
        }
        
        if (_soundFluteEnd && endSound)
        {
            _soundFluteEnd->playOnce();
        }
    }
    
    _playing = playing;
}

void Player::setSheepShowState(const bool enabled)
{
    static const char * sheepShowPlayingScheduleKey = "sheepShowPlayingScheduleKey";
    static const float sheepShowPlayingDelay = 17.113f;
    
    if (enabled)
    {
        this->scheduleOnce([this, enabled](float dt){
            this->setPlayingState(enabled, false);
        }, sheepShowPlayingDelay, sheepShowPlayingScheduleKey);
    }
    else
    {
        if (this->isScheduled(sheepShowPlayingScheduleKey))
        {
            this->unschedule(sheepShowPlayingScheduleKey);
        }
        this->setPlayingState(enabled, false);
    }
    
    Color3B playerColor = (enabled) ? Color3B::BLACK : Color3B::WHITE;
    
    Sprite * sprite = dynamic_cast<Sprite*>(this->getChildNodeWithName(playerDefs::PLAYER_SPRITE_NAME));
    if (sprite)
    {
        sprite->setColor(playerColor);
    }
    
    _soundJump->setEnable(!enabled);
    _soundWalk->setEnable(!enabled);
}

void Player::startTimer(CallFunc *callback, float time, bool inactive)
{
    auto bar = dynamic_cast<NoteProgressBar*>(this->getChildByTag(NOTE_BAR));
    if (bar)
    {
        bar->startTimer(callback, time, inactive);
    }
}

void Player::stopTimer()
{
    auto bar = dynamic_cast<NoteProgressBar*>(this->getChildByTag(NOTE_BAR));
    if (bar)
    {
        bar->stopTimer();
    }
}

void Player::updateLastVelocity()
{
    _lastVelocity = _currentVelocity;
    b2Body * playerBody = this->getBox2dBody();
    if (playerBody)
    {
        _currentVelocity = Point(playerBody->GetLinearVelocity().x, playerBody->GetLinearVelocity().y);
    }
}

Point Player::getLastVelocity()
{
    return _lastVelocity;
}

void Player::setUnderEoloWind(LHNode* node, bool begin)
{
    if (begin)
    {
        _isUnderEoloEffect = true;
        
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

void Player::updateUnderEoloWind(LHNode *node)
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

void Player::exitUnderEoloWind()
{
    _isUnderEoloEffect = false;
}

bool Player::checkIfInMenu()
{
    bool bRes = false;
    
    auto asset = dynamic_cast<LHAsset*>(this->getParent());
    if (asset && asset->containsTag(playerDefs::MENU_WALK_TAG))
    {
        bRes = true;
    }
    
    return bRes;
}

