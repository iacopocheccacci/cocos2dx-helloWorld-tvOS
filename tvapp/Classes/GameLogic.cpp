//
//  GameLogic.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 07/11/14.
//
//

#include "GameLogic.h"
#include "Sheep.h"
#include "Note.h"
#include "MonousePlatform.h"
#include "Brazier.h"
#include "Goatskin.h"
#include "Door.h"
#include "Switch.h"
#include "Wolf.h"
#include "Portal.h"
#include "Gear.h"
#include "Flute.h"
#include "EoloZone.h"
#include "Eolo.h"
#include "Tutorial.h"
#include "HUD.h"
#include "VisibleRect.h"
#include "LosePopUp.h"
#include "PausePopUp.h"
#include "Utility.h"
#include "SoundComponent.h"
#include "CameraManager.h"
#include "MyMenuItemSprite.h"
#include "ScoreManager.h"
#include "AnimationComponent.h"
#include "LoadingLayer.h"
#include "SettingsPopUp.h"
#include "SettingsManager.h"
#include "ConfirmInputPopUp.h"
#include "NotificationPop.h"
#include "EffectsManager.h"
#include "Inspiration.h"
#include "Drop.h"
#include "VideoStory.h"
#include "SheepShow.h"
#include "AnalyticsManager.h"
#include "SoundManager.h"
#include "FaithJumpBrick.h"

#ifdef EDITOR_MODE
#include "EditorTableView.h"
#include "EditorMenuManager.h"
#endif

using namespace gameObjectsParams;

enum class eGameLogicChild
{
    HUD_LAYER = 0,
    PAUSE_POP_UP,
    NOTIFICATION,
};

enum class eSceneZOrders
{
    FOREGROUND_EFFECTS = 10,
    HUD_LAYER        = 50,
    NOTIFICATION     = 75,
    PAUSE_POP_UP     = 100
};

enum class ePopUps
{
    PAUSE_POP_UP,
    CONFIRM_INPUT_POP_UP,
};

enum class eNotification
{
    WORLD_UNLOCKED,
    WORLD_LOCKED,
    GAME_OVER,
    GAME_NOT_OVER,
    EPILOGUE_LOCKED
};

enum class eEndLevelMode
{
    QUIT = 0,
    RESTART,
    WIN,
    NAVIGATE
};

GameLogic* GameLogic::create(GameContext context)
{
    GameLogic *ret = new GameLogic();
    
    if (ret && ret->initWithContentOfFile(context))
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

void GameLogic::onEnter()
{
    LHScene::onEnter();

    if (!_isCinematicScene)
    {
        this->setupInput();
    }
    
    this->startAudio();
}

void GameLogic::onEnterTransitionDidFinish()
{
    LHScene::onEnterTransitionDidFinish();
    
    _customEventLock = false;
}

void GameLogic::onExit()
{
    LHScene::onExit();
}

GameLogic::~GameLogic()
{
    this->unscheduleUpdate();
}

bool GameLogic::initWithContentOfFile(GameContext context)
{
//    std::string levelPath("publishedResources/");
//    levelPath += plistLevelFile;
    _levelName = context.getLevelName();
    
    bool retValue = LHScene::initWithContentOfFile(_levelName);
    
    if (retValue)
    {
        _isLose = false;
        _isWin = false;
        _fluteTaken = false;
        _splitEventListener = nullptr;
        _context = context;
        _inputEnabled = true;
        _exiting = false;
        _customEventLock = true;
        
        _isCinematicScene = false;
        if (eLevelType::MAIN_MENU == Utility::getLevelType(_levelName)
            && -1 != ScoreManager::getInstance()->getFeedbackFluteTaken())
        {
            _isCinematicScene = true;
        }
        
        _player = dynamic_cast<Player*>(this->getChildNodeWithName(playerDefs::PLAYER_NAME));
        
        this->initHUD();
        
        _initialSheepQuantity = _sheepCounter = this->initSheepCounter();
        _enteringSheepCounter = 0;
        _isEnteringSheepCounterActive = false;
        
        this->initWorldLevelLabel();
        _monsterInGoal = false;
        
        auto value = Utility::loadDataFromFile(SHEEP_GROUP, SHEEP_DISTANCE_NAME, SHEEP_DISTANCE_VAL);
        _distanceBetweenSheeps = atof(value->getCString());
        
        value = Utility::loadDataFromFile(NOTE_GROUP, NOTE_DURATION_NAME, NOTE_DURATION_VAL);
        _noteDuration = atof(value->getCString());
        
        this->scheduleUpdate();
        this->schedule(schedule_selector(GameLogic::updateCustomEvents), 0.016f);
        
        this->initPlayerPosition(context);
        
        this->initDoorSwitchConnections();
        this->initInteractive();
        this->initSheeps();
        this->initWolf();
        this->initPortals();
        this->initNotes();
        
        this->initMonousePlatforms();
        
        this->initParallax();
        this->initAudio();
        this->initCameraManager();
		this->initSheepCounterGlobal();
        this->initEoloZones();
        this->initFlute();
        this->initGoal();
        this->initBonusWorldPlatform();
        this->initCredits();
        
        this->initEffects();
        
        if (eInputType::SPLIT_SCREEN == this->getInputType())
        {
            this->initTutorial();
        }
        
        this->initAnalytics(context);
        
        this->initStoryVideos();

#ifdef EDITOR_MODE
        EditorMenuManager* editor = EditorMenuManager::create();
        this->addChild(editor, 0);
        editor->createItems();
#endif
    }
    else
    {
        retValue = false;
    }
    
    return retValue;
}

Player* GameLogic::getPlayerInstance()
{
    return _player;
}

Tutorial* GameLogic::getTutorialInstance()
{
    return dynamic_cast<Tutorial*>(this->getChildNodeWithName(tutorialDefs::GUIDE_NODE));
}

HUD* GameLogic::getHUDInstance()
{
    return dynamic_cast<HUD*>(this->getChildByTag(static_cast<int>(eGameLogicChild::HUD_LAYER)));
}

LevelHelperNodeTypeSubclass GameLogic::createNodeObjectForSubclassWithName(const std::string subclassTypeName, const std::string lhTypeName)
{
    if (subclassTypeName == playerDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Player::nodeWithDictionary;
    }
    else if (subclassTypeName == sheepDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Sheep::nodeWithDictionary;
    }
    else if (subclassTypeName == noteDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Note::nodeWithDictionary;
    }
    else if (subclassTypeName == monousePlatformDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&MonousePlatform::nodeWithDictionary;
    }
    else if (subclassTypeName == brazierDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Brazier::nodeWithDictionary;
    }
    else if (subclassTypeName == interactiveBlockDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&InteractiveBlock::nodeWithDictionary;
    }
    else if (subclassTypeName == goatskinDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Goatskin::nodeWithDictionary;
    }
    else if (subclassTypeName == portalDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Portal::nodeWithDictionary;
    }
    else if (subclassTypeName == doorDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Door::nodeWithDictionary;
    }
    else if (subclassTypeName == switchDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Switch::nodeWithDictionary;
    }
    else if (subclassTypeName == wolfDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Wolf::nodeWithDictionary;
    }
    else if (subclassTypeName == gearDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Gear::nodeWithDictionary;
    }
    else if (subclassTypeName == fluteDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Flute::nodeWithDictionary;
    }
    else if (subclassTypeName == tutorialDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Tutorial::nodeWithDictionary;
    }
    else if (subclassTypeName == eoloZoneDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&EoloZone::nodeWithDictionary;
    }
    else if (subclassTypeName == eoloDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Eolo::nodeWithDictionary;
    }
    else if (subclassTypeName == dropDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&Drop::nodeWithDictionary;
    }
    else if (subclassTypeName == videoStoryDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&VideoStory::nodeWithDictionary;
    }
    else if (subclassTypeName == sheepShowDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&SheepShow::nodeWithDictionary;
    }
    else if (subclassTypeName == faithJumpBrickDefs::CLASS_NAME)
    {
        return (LevelHelperNodeTypeSubclass)&FaithJumpBrick::nodeWithDictionary;
    }
    
    return nullptr;
}

#pragma mark INPUT_SECTION

eInputType GameLogic::getInputType()
{
    return SettingsManager::getInstance()->loadUserInput();
}

void GameLogic::setupInput()
{
    eInputType inputType = this->getInputType();
    
    _inputController.setInputType(inputType);
    
    if (eInputType::KEYBOARD == inputType)
    {
        this->setupKeyboardInput();
    }
    else if (eInputType::SPLIT_SCREEN == inputType || eInputType::BUTTONS == inputType)
    {
        this->setupButtonsInput();
        this->setupSplitScreenInput();
    }
    else if(eInputType::GAMEPAD == inputType)
    {
        this->setupGamePadInput();
    }
    else
    {
        CCLOG("Unkonwn input type!");
    }
    
    this->updateInput();
}

void GameLogic::setupKeyboardInput()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed  = CC_CALLBACK_2(GameLogic::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(GameLogic::onKeyReleased, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameLogic::setupSplitScreenInput()
{
    _splitEventListener = cocos2d::EventListenerTouchAllAtOnce::create();
    _splitEventListener->onTouchesBegan = CC_CALLBACK_2(GameLogic::onTouchesBegan, this);
    _splitEventListener->onTouchesMoved = CC_CALLBACK_2(GameLogic::onTouchesMoved, this);
    _splitEventListener->onTouchesEnded = CC_CALLBACK_2(GameLogic::onTouchesEnded, this);
    _splitEventListener->onTouchesCancelled = CC_CALLBACK_2(GameLogic::onTouchesCancelled, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_splitEventListener, this);
    
    // Inizialmente disattivo
    _splitEventListener->setEnabled(false);
    
    auto tutorial = this->getTutorialInstance();
    if (tutorial && tutorial->getIscontrollerGuideEnabled())
    {
        _inputController.setControllerTutorial(tutorial);
    }
}

void GameLogic::setupGamePadInput()
{
    auto _gamePadListener = EventListenerController::create();
    
    //bind onConneected event call function
    _gamePadListener->onConnected = CC_CALLBACK_2(GameLogic::onConnectController,this);
    
    //bind disconnect event call function
    _gamePadListener->onDisconnected = CC_CALLBACK_2(GameLogic::onDisconnectedController,this);
    
    //bind onKeyDown event call function
    _gamePadListener->onKeyDown = CC_CALLBACK_3(GameLogic::onKeyDown, this);
    
    //bind onKeyUp event call function
    _gamePadListener->onKeyUp = CC_CALLBACK_3(GameLogic::onKeyUp, this);
    
    _gamePadListener->onKeyRepeat = CC_CALLBACK_3(GameLogic::onKeyRepeat, this);
    
    //bind onAxis event call function, onAxis will be called when analog stick is changed
    _gamePadListener->onAxisEvent = CC_CALLBACK_3(GameLogic::onAxisEvent, this);
    
    //Activate the listener into the event dispatcher
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_gamePadListener, this);
    
    //This function should be called for iOS platform
    Controller::startDiscoveryController();
}

//Controller is the obejects of the Controller，keyCode means the keycode of the controller you click down
void GameLogic::onKeyDown(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    //    CCLOG("KeyDown:%d", keyCode);
    showButtonState(controller, keyCode, true);
}

void GameLogic::onKeyUp(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event)
{
    //You can get the controller by tag, deviceId or devicename if there are multiple controllers
    //    CCLOG("KeyUp:%d", keyCode);
    showButtonState(controller, keyCode, false);
}

void GameLogic::onKeyRepeat(cocos2d::Controller *controller, int keyCode, cocos2d::Event *event){
    showButtonState(controller, keyCode, true);
}

//The axis includes X-axis and Y-axis and its range is from -1 to 1. X-axis is start from left to right and Y-axis is bottom to top.
void GameLogic::onAxisEvent(cocos2d::Controller* controller, int keyCode, cocos2d::Event* event)
{
    const auto& keyStatus = controller->getKeyStatus(keyCode);
    CCLOG("Axis KeyCode:%d Axis Value:%f", keyCode, keyStatus.value);
}

void GameLogic::showButtonState(cocos2d::Controller *controller, int keyCode, bool isPressed)
{
    //    onConnectController(controller,nullptr);
    
    if (isPressed)
    {
        switch (keyCode)
        {
            case cocos2d::Controller::Key::BUTTON_A:
                CCLOG("Button A Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_B:
                CCLOG("Button B Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_X:
                CCLOG("Button X Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_Y:
                CCLOG("Button Y Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_UP:
                CCLOG("Button DPAD_UP Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_DOWN:
                CCLOG("Button DPAD_Down Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_LEFT:
                CCLOG("Button DPAD_Left Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_RIGHT:
                CCLOG("Button DPAD_Right Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_LEFT_SHOULDER:
                CCLOG("Button Left Shoulder Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_RIGHT_SHOULDER:
                CCLOG("Button Right Shoulder Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_LEFT_THUMBSTICK:
                CCLOG("Button Left ThumbStick Pressed");
                break;
            case cocos2d::Controller::Key::BUTTON_RIGHT_THUMBSTICK:
                CCLOG("Button Right Shoulder Pressed");
                break;
            default:
            {
                char ketStatus[30];
                sprintf(ketStatus,"Key Down:%d",keyCode);
                break;
            }
        }
    }
    else
    {
        switch (keyCode)
        {
            case cocos2d::Controller::Key::BUTTON_A:
                CCLOG("Button A");
                break;
            case cocos2d::Controller::Key::BUTTON_B:
                CCLOG("Button B");
                break;
            case cocos2d::Controller::Key::BUTTON_X:
                CCLOG("Button X");
                break;
            case cocos2d::Controller::Key::BUTTON_Y:
                CCLOG("Button Y");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_UP:
                CCLOG("Button DPAD_UP");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_DOWN:
                CCLOG("Button DPAD_Down");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_LEFT:
                CCLOG("Button DPAD_Left");
                break;
            case cocos2d::Controller::Key::BUTTON_DPAD_RIGHT:
                CCLOG("Button DPAD_Right");
                break;
            case cocos2d::Controller::Key::BUTTON_LEFT_SHOULDER:
                CCLOG("Button Left Shoulder");
                break;
            case cocos2d::Controller::Key::BUTTON_RIGHT_SHOULDER:
                CCLOG("Button Right Shoulder");
                break;
            case cocos2d::Controller::Key::BUTTON_LEFT_THUMBSTICK:
                CCLOG("Button Left ThumbStick");
                break;
            case cocos2d::Controller::Key::BUTTON_RIGHT_THUMBSTICK:
                CCLOG("Button Right Shoulder");
                break;
            default:
            {
                char ketStatus[30];
                sprintf(ketStatus,"Key Down:%d",keyCode);
                break;
                
            }
        }
    }
}

void GameLogic::setupButtonsInput()
{
    auto arrowSx = Sprite::create(appParams::LEFT_BUTTON_PATH);
    auto arrowDx = Sprite::create(appParams::RIGHT_BUTTON_PATH);
    auto arrowUp = Sprite::create(appParams::JUMP_BUTTON_PATH);
    
    float offsetY = - (appParams::CONTROLLER_BUTTON_HEIGHT / 2.0f - arrowSx->getContentSize().height / 2.0f) + 10.0f;
    auto sizeButton = Size(appParams::CONTROLLER_BUTTON_WIDTH, appParams::CONTROLLER_BUTTON_HEIGHT);
    
    auto touchSx = MyMenuItemSprite::create(arrowSx, arrowSx, arrowSx, CC_CALLBACK_1(GameLogic::buttonInputCallback, this));
    touchSx->setContentSize(sizeButton);
    touchSx->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    touchSx->setPositionY(offsetY);
    touchSx->getNormalImage()->setPosition(Point(touchSx->getContentSize() / 2.0f));
    touchSx->getNormalImage()->setAnchorPoint(Point::ANCHOR_MIDDLE);
    touchSx->setName(appParams::LEFT_BUTTON_NAME);
    
    auto touchDx = MyMenuItemSprite::create(arrowDx, arrowDx, arrowDx, CC_CALLBACK_1(GameLogic::buttonInputCallback, this));
    touchDx->setContentSize(sizeButton);
    touchDx->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    touchDx->setPosition(Vec2(touchSx->getContentSize().width, offsetY));
    touchDx->getNormalImage()->setPosition(Point(touchDx->getContentSize() / 2.0f));
    touchDx->getNormalImage()->setAnchorPoint(Point::ANCHOR_MIDDLE);
    touchDx->setName(appParams::RIGHT_BUTTON_NAME);
    
    auto touchUp = MyMenuItemSprite::create(arrowUp, arrowUp, arrowUp, CC_CALLBACK_1(GameLogic::buttonInputCallback, this));
    touchUp->setContentSize(sizeButton);
    touchUp->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);
    touchUp->setPosition( - sizeButton.width / 2.0f, offsetY);
    touchUp->getNormalImage()->setPosition(Point(touchUp->getContentSize() / 2.0f));
    touchUp->getNormalImage()->setAnchorPoint(Point::ANCHOR_MIDDLE);
    touchUp->setName(appParams::JUMP_BUTTON_NAME);
    
    auto menu1 = Menu::create(touchSx, touchDx, NULL);
    menu1->setCascadeOpacityEnabled(true);
    menu1->setPosition(VisibleRect::leftBottom());
    
    auto menu2 = Menu::create(touchUp, NULL);
    menu2->setCascadeOpacityEnabled(true);
    menu2->setPosition(VisibleRect::rightBottom());
    
    auto menuGeneral = Node::create();
    menuGeneral->addChild(menu1);
    menuGeneral->addChild(menu2);
    menuGeneral->setCascadeOpacityEnabled(true);
    this->addChild(menuGeneral, 1, appParams::MENU_BUTTONS_NAME);
    
    menuGeneral->runAction(FadeOut::create(10.0f));
    
    // Inizialmente invisibile
    menuGeneral->setVisible(false);
}

void GameLogic::updateInput()
{
    eInputType inputType = this->getInputType();
    auto menu = dynamic_cast<Node*>(this->getChildByName(appParams::MENU_BUTTONS_NAME));
    
    if (menu && _splitEventListener)
    {
        _inputController.setInputType(inputType);
        
        switch (inputType)
        {
            case eInputType::BUTTONS:
            {
                menu->setVisible(true);
                menu->stopAllActions();
                menu->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), FadeOut::create(8.0f)));
                _splitEventListener->setEnabled(false);
                
                // Interrompo il tutorial
//                SettingsManager::getInstance()->saveMoveTutorialDone(true);
                auto tutorial = this->getTutorialInstance();
                if (tutorial)
                {
                    tutorial->setIscontrollerGuideEnabled(false);
//                    tutorial->setVisible(false);
//                    _inputController.setControllerTutorial(nullptr);
                }
                break;
            }
            case eInputType::SPLIT_SCREEN:
            {
                menu->setVisible(false);
                _splitEventListener->setEnabled(true);
                
                // Riattivo il tutorial
                auto tutorial = this->getTutorialInstance();
                if (tutorial)
                {
                    //bool tutorialDone = SettingsManager::getInstance()->loadMoveTutorialDone();
                    tutorial->setIscontrollerGuideEnabled(true);
                }
                
                break;
            }   
            default:
                break;
        }
    }
    
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->updateStopBtnInputType(inputType);
    }
}

void GameLogic::buttonInputCallback(cocos2d::Ref *sender)
{
    auto button = dynamic_cast<MyMenuItemSprite*>(sender);
    if (_inputEnabled)
    {
        _cameraManager->zoomResume();
    }
    
    if (button)
    {
        bool selected = button->isSelected();
        _inputController.setButtonInput(button->getName(), selected);
        this->updateStatus();
    }
    
    auto menu = dynamic_cast<Node*>(this->getChildByName(appParams::MENU_BUTTONS_NAME));
    
    if (menu && _inputEnabled)
    {
        menu->stopAllActions();
        menu->runAction(Sequence::createWithTwoActions(FadeIn::create(0.5f), FadeOut::create(8.0f)));
    }
}

void GameLogic::setInputEnabled(bool enabled)
{
    if (enabled == false)
    {
        _inputController.resetAllCommand();
        this->updateStatus();
    }
    _inputEnabled = enabled;
    this->updateStatus();
}

void GameLogic::touchesEvent(const std::vector<Touch *> &touches, Event* event, bool isTouchBegan)
{
    if (_inputEnabled)
    {
        _cameraManager->zoomResume();
    }
    
    for (auto &item: touches )
    {
        auto touch = item;
        auto location = touch->getLocation();
        Player* player = getPlayerInstance();
        
        if (player)
        {
            _inputController.setTouchInput(location, event, isTouchBegan, player->isVerticalOnlyJump());
            this->updateStatus();
        }
    }
}

void GameLogic::onTouchesBegan(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    this->touchesEvent(touches, event, true);
}

void GameLogic::onTouchesMoved(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    for ( auto &item: touches )
    {
        auto touch = item;
        auto location = touch->getLocation();
        
        _inputController.resolveTouchMoveInput(touch, event);
        this->updateStatus();
    }
}

void GameLogic::onTouchesEnded(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    touchesEvent(touches, event, false);
}

void GameLogic::onTouchesCancelled(const std::vector<Touch *> &touches, cocos2d::Event *event)
{
    pauseLevel();
}

void GameLogic::keyboardEvent(EventKeyboard::KeyCode keyCode, bool pressed)
{
    _inputController.setKeyboardInput(keyCode, pressed);
    this->updateStatus();
}

void GameLogic::onKeyPressed(EventKeyboard::KeyCode keyCode, cocos2d::Event *unused_event)
{
    if (_inputEnabled)
    {
        _cameraManager->zoomResume();
    }
    
    this->keyboardEvent(keyCode, true);
    
//    Player* player = getPlayerInstance();
//    Vec2 location(0.0f, 0.0f);
//    
//    if (keyCode == EventKeyboard::KeyCode::KEY_L)
//    {
//        location.set(600.0f, 200.0f);
//    }
//    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
//    {
//        location.set(100.0f, 200.0f);
//    }
//    
//    if (player && location != Vec2(0.0f, 0.0f))
//    {
//        _inputController.setTouchInput(location, nullptr, true, player->isVerticalOnlyJump());
//        this->updateStatus();
//    }

}

void GameLogic::onKeyReleased(EventKeyboard::KeyCode keyCode, cocos2d::Event *unused_event)
{
    this->keyboardEvent(keyCode, false);
    
//    Player* player = getPlayerInstance();
//    Vec2 location(0.0f, 0.0f);
//    
//    if (keyCode == EventKeyboard::KeyCode::KEY_L)
//    {
//        location.set(600.0f, 200.0f);
//    }
//    else if (keyCode == EventKeyboard::KeyCode::KEY_A)
//    {
//        location.set(100.0f, 200.0f);
//    }
//    
//    if (player && location != Vec2(0.0f, 0.0f))
//    {
//        _inputController.setTouchInput(location, nullptr, false, player->isVerticalOnlyJump());
//        this->updateStatus();
//    }
}

void GameLogic::onConnectController(cocos2d::Controller* controller, Event* event)
{
    CCLOG("Game controller connected");
}

void GameLogic::onDisconnectedController(cocos2d::Controller* controller, Event* event)
{
    CCLOG("Game controller disconnected");
}

void GameLogic::updateStatus()
{
    Player* player = getPlayerInstance();
    
    if (player && _inputEnabled)
    {
        if (player->isMovingRight() != _inputController.isRightPressed())
        {
            player->handleInput(_inputController.isRightPressed() ? eAction::MOVE_RIGHT : eAction::STOP_MOVE_RIGHT);
        }
        
        if (player->isMovingLeft() != _inputController.isLeftPressed())
        {
            player->handleInput(_inputController.isLeftPressed() ? eAction::MOVE_LEFT : eAction::STOP_MOVE_LEFT);
        }
        
        if (!player->isJumping() && _inputController.isJumpPressed())
        {
            player->handleInput(eAction::JUMP);
        }
    }
}

#pragma mark COLLISION_SECTION

void GameLogic::didBeginContactBetweenNodes(Node* nodeA, Node* nodeB, Point contactPoint, float impulse)
{
    CollisionManager cm;
    eCollisionType type = cm.handleCollisionBegin(nodeA, nodeB, impulse);
    
    switch (type)
    {
        case eCollisionType::PLAYER_NOTE_COLLISION:
            this->createNoteEffect(eNoteEffect::NORMAL_NOTE);
            break;
            
        case eCollisionType::PLAYER_INVERSE_NOTE_COLLISION:
            this->createNoteEffect(eNoteEffect::INVERSE_NOTE);
            break;
            
        case eCollisionType::PLAYER_FIRE_COLLISION:
            this->burnPlayer();
        case eCollisionType::PLAYER_LOSE_COLLISION:
            this->handleDefeat();
            break;
            
        case eCollisionType::PLAYER_INSPIRATION_COLLISION:
            this->startInspiration(this->getInspirationNode(nodeA, nodeB));
            break;
            
        case eCollisionType::SHEEP_FIRE_COLLISION:
            this->handleSheepInFire(nodeA, nodeB);
            break;
            
        case eCollisionType::SHEEP_GOAL_COLLISION:
            this->handleSheepInGoal();
            break;
            
        case eCollisionType::SHEEP_WOLF_COLLISION_BEGIN:
            break;
            
        case eCollisionType::MONSTER_GOAL_COLLISION:
            this->handleMonsterInGoal(this->getGoalNode(nodeA, nodeB));
            break;
            
        case eCollisionType::MOVABLE_LEFT_COLLISION:
        case eCollisionType::MOVABLE_RIGHT_COLLISION:
        case eCollisionType::MOVABLE_UP_COLLISION:
        case eCollisionType::MOVABLE_DOWN_COLLISION:
            this->handleMovablePlatformChangeDirection(type, this->getMovablePlatform(nodeA, nodeB));
            break;
            
        case eCollisionType::IDLE_GOATSKIN_COLLISION:
            this->turnOnGoatskin(getGoatskin(nodeA, nodeB));
            // continue to next case before break!
        case eCollisionType::OFF_GOATSKIN_COLLISION:
        case eCollisionType::PLAYER_BLOCK_COLLISION_BEGIN:
        case eCollisionType::PLAYER_GROUND_COLLISION_BEGIN:
        case eCollisionType::PLAYER_SHEEP_COLLISION_BEGIN:
            this->handlePlayerJumpEnd();
            break;
        case eCollisionType::PLAYER_FAITH_COLLISION:
            this->handlePlayerCollisionWithFaith();
            this->handlePlayerJumpEnd();
            break;
        case eCollisionType::PLAYER_FLUTE_COLLISION:
            this->handleFluteTaken();
            break;
        case eCollisionType::PLAYER_SCRIPT_EVENT_COLLISION_BEGIN:
            this->handleScriptEvent(nodeA, nodeB, true);
            break;
            
        case eCollisionType::INTERACTIVE_LOSE_COLLISION:
            this->handleRespawnEvent(nodeA, nodeB);
            break;
            
        default:
            break;
    }
}

void GameLogic::didEndContactBetweenNodes(Node* nodeA, Node* nodeB)
{
    CollisionManager cm;
    eCollisionType type = cm.handleCollisionEnd(nodeA, nodeB);
//    cm.handleCollisionEnd(nodeA, nodeB);
    
    switch (type)
    {
        case eCollisionType::PLAYER_SCRIPT_EVENT_COLLISION_END:
            this->handleScriptEvent(nodeA, nodeB, false);
            break;
            
        default:
            break;
    }
}

bool GameLogic::shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB)
{
    CollisionManager cm;
    return cm.handleDisableCollision(nodeA, nodeB);
}

void GameLogic::didFinishedPlayingAnimation(LHAnimation *anim)
{
    Inspiration::handleEndInspiration(anim);
}

void GameLogic::handleSheepInGoal()
{
    if (false == _monsterInGoal)
    {
        this->decrementSheepCounter();
        this->updatePortals();
        
        this->playSoundSheepCollected();
        
        this->safeCheckForSheepInGoal();
    }
}

void GameLogic::safeCheckForSheepInGoal()
{
    if (_sheepCounter > 0)
    {
        CallFunc * callback = CallFunc::create([this]()
        {
            auto goal = dynamic_cast<LHNode*>(this->getChildrenWithTags({gameTags::GOAL_TAG})->getLastObject());
            
            const std::vector<std::string> tagValues = {gameTags::SHEEP_TAG};
            __Array* sheepArray = this->getChildrenWithTags(tagValues);
            Ref* ref = nullptr;
            Sheep* sheep = nullptr;
            
            CCARRAY_FOREACH(sheepArray, ref)
            {
                sheep = dynamic_cast<Sheep*>(ref);
                
                bool sheepInGoal = false;
                
                b2Body * sheepBody = sheep->getBox2dBody();
                b2Body * goalBody = goal->getBox2dBody();
                for (b2ContactEdge * contact = sheepBody->GetContactList(); contact != nullptr; contact = contact->next)
                {
                    if (contact->other == goalBody)
                    {
                        sheepInGoal = true;
                    }
                }
                // TODO: controllare se la pecora è nella capanna
                if (sheepInGoal)
                {
                    CCLOG("Recuperata pecora con controllo di sicurezza!");
                    sheep->removeFromParentAndCleanup(true);
                    this->handleSheepInGoal();
                }
            }
        });
        Sequence * safeCheckSequence = Sequence::create(DelayTime::create(0.2f), callback, NULL);
        this->runAction(safeCheckSequence);
    }
}

void GameLogic::handleMonsterInGoal(LHNodeProtocol * goalNodeProt)
{
    _monsterInGoal = true;
    
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portalArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    CCARRAY_FOREACH(portalArray, ref)
    {
        portal = dynamic_cast<Portal*>(ref);
        if (portal && ePortalType::NORMAL == portal->getPortalType())
        {
            portal->closeWithTransition(0.5f);
        }
    }
    
    this->burnGoal(goalNodeProt);
    
    this->handleDefeat();
    
//    if (_initialSheepQuantity != _sheepCounter)
//    {
//        this->handleDefeat();
//    }
}

void GameLogic::handleMovablePlatformChangeDirection(eCollisionType type, LHNodeProtocol* platform)
{
    b2Vec2 velocity;
    velocity.Set(0.0f, 0.0f);
    
    switch (type)
    {
        case eCollisionType::MOVABLE_LEFT_COLLISION:
            velocity.Set(-5.f, 0.0f);
            break;
        case eCollisionType::MOVABLE_RIGHT_COLLISION:
            velocity.Set(5.f, 0.0f);
            break;
        case eCollisionType::MOVABLE_UP_COLLISION:
            velocity.Set(0.0f, 5.f);
            break;
        case eCollisionType::MOVABLE_DOWN_COLLISION:
            velocity.Set(0.0f, -5.f);
            break;
            
        default:
            break;
    }
    
    this->setPlayerOnMovableVelocity(platform, velocity);
    this->setSheepOnMovableVelocity(platform, velocity);
    this->setInteractiveOnMovableVelocity(platform, velocity);
}

void GameLogic::handlePlayerJumpEnd()
{
    getPlayerInstance()->handleJumpingStateEnd(_inputController.isJumpPressed());
}

void GameLogic::handlePlayerCollisionWithFaith()
{
    bool isFirst = true;
    const std::vector<std::string> tagValues = {gameTags::FAITH_TAG};
    __Array* faithNodeArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    FaithJumpBrick* faith = nullptr;
    
    CCARRAY_FOREACH(faithNodeArray, ref)
    {
        faith = dynamic_cast<FaithJumpBrick*>(ref);
        if (faith && !faith->isAlreadyActive())
        {
            faith->buildPlatform();
            if (_soundFall && isFirst)
            {
                isFirst = false;
                _soundFall->playOnce();
            }
        }
    }
}

void GameLogic::handleSheepExiting(Node * sheepNode)
{
    Sheep * sheep = dynamic_cast<Sheep *>(sheepNode);
    if (sheep)
    {
        _sheeps.erase(std::remove(_sheeps.begin(), _sheeps.end(), sheep), _sheeps.end());
        CCLOG("Pecora uscita di scena");
    }
}

void GameLogic::setPlayerOnMovableVelocity(LHNodeProtocol *platform, b2Vec2 velocity)
{
    auto player = getPlayerInstance();
    LHNodeProtocol* playerPlatform = player->getMovablePlatform();
    
    if (playerPlatform && (playerPlatform == platform))
    {
        player->getBox2dBody()->SetLinearVelocity(velocity);
    }
}

void GameLogic::setSheepOnMovableVelocity(LHNodeProtocol* platform, b2Vec2 velocity)
{
    const std::vector<std::string> tagValues = {gameTags::SHEEP_TAG};
    __Array* sheepArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Sheep* sheep = nullptr;
    LHNodeProtocol* sheepPlatform = nullptr;
    
    CCARRAY_FOREACH(sheepArray, ref)
    {
        sheep = dynamic_cast<Sheep*>(ref);
        if (sheep)
        {
            sheepPlatform = sheep->getMovablePlatform();
            if (sheepPlatform && (sheepPlatform == platform))
            {
                sheep->getBox2dBody()->SetLinearVelocity(velocity);
            }
        }
    }
}

void GameLogic::setInteractiveOnMovableVelocity(LHNodeProtocol *platform, b2Vec2 velocity)
{
    const std::vector<std::string> tagValues = {gameTags::INTERACTIVE_TAG};
    __Array* interactiveArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    InteractiveBlock* interactive = nullptr;
    LHNodeProtocol* interactivePlatform = nullptr;
    
    CCARRAY_FOREACH(interactiveArray, ref)
    {
        interactive = dynamic_cast<InteractiveBlock*>(ref);
        if (interactive)
        {
            interactivePlatform = interactive->getMovablePlatform();
            if (interactivePlatform && (interactivePlatform == platform))
            {
                interactive->getBox2dBody()->SetLinearVelocity(velocity);
            }
        }
    }
}

void GameLogic::handleDefeat()
{
    auto player = this->getPlayerInstance();
    auto respawnNode = dynamic_cast<LHNode *>(this->getChildNodeWithName(appParams::RESPAWN_NODE_NAME));
    
    if (respawnNode)
    {
        if (player)
        {
            this->relocatePlayer(respawnNode->getPosition());
            CCLOG("Respawn!");
        }
    }
    else
    {
        CCLOG("Sconfitta!");
        
        if (_cameraManager)
        {
            _cameraManager->handleDefeat();
        }
        
        this->timerEnded(false);
//        if (player)
//        {
//            player->setNoteEffectType(eNoteEffect::NO_EFFECT);
//            player->disableNoteParticles();
//            player->stopTimer();
//        }
        
        this->setInputEnabled(false);
        
        if (_monsterInGoal)
        {
            DelayTime * delay = DelayTime::create(2.0f);
            CallFunc * loseScreenCallback = CallFunc::create([this](){
                this->createLoseScreen();
            });
            this->runAction(Sequence::create(delay, loseScreenCallback, NULL));
        }
        else
        {
            this->createLoseScreen();
        }
    }
}

void GameLogic::handleSheepInFire(Node* nodeA, Node* nodeB)
{
    auto sheep = dynamic_cast<Sheep*>(nodeA);
    if (!sheep)
    {
        sheep = dynamic_cast<Sheep*>(nodeB);
    }
    
    if (sheep)
    {
        const std::vector<std::string> tagValues = {gameTags::WOLF_TAG};
        __Array* wolfArray = this->getChildrenWithTags(tagValues);
        Ref* ref = nullptr;
        Wolf* wolf = nullptr;
        
        CCARRAY_FOREACH(wolfArray, ref)
        {
            wolf = dynamic_cast<Wolf*>(ref);
            if (wolf)
            {
                wolf->deleteSheepFromArrayIfExist(sheep);
            }
        }
    }
}

void GameLogic::handleFluteTaken()
{
    _fluteTaken = true;
    
    HUD * hud = this->getHUDInstance();
    if (hud)
    {
        hud->activeFlute();
    }
    
    if (_cameraManager)
    {
        _cameraManager->handleFluteTaken();
    }
}

void GameLogic::handleScriptEvent(Node* nodeA, Node* nodeB, bool begin)
{
    LHNode * scriptNode;
    auto nodelhA = dynamic_cast<LHNodeProtocol *>(nodeA);
    if (nodelhA->containsTag(gameTags::SCRIPT_EVENT_TAG))
    {
        scriptNode = dynamic_cast<LHNode *>(nodeA);
    }
    else
    {
        scriptNode = dynamic_cast<LHNode *>(nodeB);
    }
    
    if (scriptNode->getName().compare(scriptNodeNameDefs::TUTORIAL_JUMP_AHEAD) == 0)
    {
        auto tutorial = this->getTutorialInstance();
        bool tutorialDone = SettingsManager::getInstance()->loadMoveTutorialDone();
        if (tutorial && tutorial->getIscontrollerGuideEnabled() && !tutorialDone)
        {
            if (begin)
            {
                tutorial->setTutorialState(eTutorialState::TUTORIAL_CONTROLLER_JUMP_RIGHT);
                CCLOG("Tutorial Jump Right Begin!");
            }
            else
            {
                tutorial->setTutorialState(eTutorialState::TUTORIAL_NONE);
                CCLOG("Tutorial Jump Right End!");
            }
        }
    }
    
    if (scriptNode->getName().compare(scriptNodeNameDefs::TUTORIAL_JUMP_VERTICAL) == 0)
    {
        auto tutorial = this->getTutorialInstance();
        bool tutorialDone = SettingsManager::getInstance()->loadMoveTutorialDone();
        if (tutorial && tutorial->getIscontrollerGuideEnabled() && !tutorialDone)
        {
            if (begin)
            {
                tutorial->setTutorialState(eTutorialState::TUTORIAL_CONTROLLER_JUMP_VERTICAL);
                CCLOG("Tutorial Jump Vertical Begin!");
            }
            else
            {
                tutorial->setTutorialState(eTutorialState::TUTORIAL_NONE);
                CCLOG("Tutorial Jump Vertical End!");
            }
        }
    }
    
    if (scriptNode->getName().compare(scriptNodeNameDefs::TUTORIAL_VIDEO) == 0)
    {
        auto tutorial = this->getTutorialInstance();
        if (tutorial)
        {
            auto hud = this->getHUDInstance();
            if (hud)
            {
                if (begin)
                {
                    hud->hideElements();
                }
                else
                {
                    hud->showElements();
                }
            }
            
            if (tutorial->getIscontrollerGuideEnabled())
            {
                if (begin)
                {
                    tutorial->setTutorialState(eTutorialState::TUTORIAL_CONTROLLER_VIDEO);
                }
                else
                {
                    tutorial->setTutorialState(eTutorialState::TUTORIAL_NONE);
                    
                    bool tutorialDone = SettingsManager::getInstance()->loadMoveTutorialDone();
                    SettingsManager::getInstance()->saveMoveTutorialDone(true);
                    if (eInputType::SPLIT_SCREEN == this->getInputType() && false == tutorialDone)
                    {
                        this->pauseLevelWithPopUp(ePopUps::CONFIRM_INPUT_POP_UP);
                    }
                }
            }
        }
    }
    
    if (scriptNode->getName().compare(scriptNodeNameDefs::INPUT_CONFIRM) == 0)
    {
        bool tutorialDone = SettingsManager::getInstance()->loadMoveTutorialDone();
        eInputType inputType = this->getInputType();
        if (begin)
        {
            SettingsManager::getInstance()->saveMoveTutorialDone(true);
            
            auto tutorial = this->getTutorialInstance();
            if (tutorial)
            {
                auto tutorialExpirationTimer = tutorial->getActionByTag(static_cast<int>(eTutorialAction::EXPIRATION));
                
                if (false == tutorialDone && eInputType::KEYBOARD != inputType && tutorialExpirationTimer)
                {
                    tutorial->stopAction(tutorialExpirationTimer);
                    this->pauseLevelWithPopUp(ePopUps::CONFIRM_INPUT_POP_UP);
                    CCLOG("Mantenere i comandi sperimentali?");
                }
            }
        }
    }
    
    if (scriptNode->getName().compare(scriptNodeNameDefs::SHEEP_SHOW_ZONE) == 0)
    {
        Player * player = this->getPlayerInstance();
        player->setSheepShowState(begin);
        
        HUD * hud = this->getHUDInstance();
        if (hud)
        {
            if (begin)
            {
                hud->hideGlobalCounter();
            }
            else
            {
                hud->showGlobalCounter();
            }
        }
        
        SheepShow * sheepShow = dynamic_cast<SheepShow*>(this->getChildNodeWithName(appParams::SHEEP_SHOW_NODE_NAME));
        if (sheepShow)
        {
            sheepShow->setPlayState(begin);
            
            float musicVolume = (begin) ? 0.0f : 1.0f;
            SoundManager::getInstance()->setSystemVolumeMusic(musicVolume, true);
        }
    }
    
    if (scriptNode->containsTag(gameTags::CAMERA_EVENT_TAG))
    {
        if (_cameraManager)
        {
            _cameraManager->handleCameraEvent(scriptNode);
        }
    }
}

void GameLogic::handleRespawnEvent(Node* nodeA, Node* nodeB)
{
    InteractiveBlock* interactive = dynamic_cast<InteractiveBlock*>(nodeA);
    if (!interactive)
    {
        interactive =dynamic_cast<InteractiveBlock*>(nodeB);
    }
    
    if (interactive && interactive->getParent())
    {
        // Controlla se esiste un nodo di respawn customizzato, se non esiste usa posizione di default.
        std::stringstream respawnName;
        respawnName << appParams::RESPAWN_NODE_NAME << "_" << interactive->getParent()->getName();
        auto respawnNode = dynamic_cast<LHNode *>(this->getChildNodeWithName(respawnName.str()));
        
        if (respawnNode)
        {
            this->relocateElement(interactive, respawnNode->getPosition());
            CCLOG("Respawn!");
        }
        else
        {
            auto newPoint = Vec2(interactive->getParent()->getPositionX(), interactive->getParent()->getPositionY() + 200.0f);
            this->relocateElement(interactive, newPoint);
        }
    }
}

#pragma mark PARTICLES_SECTION

void GameLogic::createNoteEffect(eNoteEffect effect)
{
    Player* player = getPlayerInstance();
    bool isInactive = true;
    
    if (player)
    {
        auto callback = CallFunc::create( [this]()
        {
            this->timerEnded();
        } );
        
        float duration = _noteDuration;
        
        // Suono di catch della nota
        if (player->getSoundCatchNote())
        {
            player->getSoundCatchNote()->playOnce();
        }
        
        if (player->getIsPlaying())
        {
            if (player->getNoteEffectType() == effect)
            {
                isInactive = false;
            }
            else
            {
                player->stopTimer();
                player->setNoteEffectType(effect);
//                player->setPlayingState(true);
            }
            player->enableNoteParticles();
            player->startTimer(callback, duration, isInactive);
        }
        else
        {
            player->setNoteEffectType(effect);
            player->enableNoteParticles();
            player->startTimer(callback, duration, isInactive);
//            player->setPlayingState(true);
            this->schedule(schedule_selector(GameLogic::checkForParticleCollision), .01);
            
            auto hud = this->getHUDInstance();
            if (hud)
            {
                
                hud->showStopBtn();
            }
        }
        player->setPlayingState(true);
    }
    
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->updateStopBtnEffect(effect);
    }
}

void GameLogic::timerEnded()
{
    this->timerEnded(true);
}

void GameLogic::timerEnded(bool endSound)
{
    auto player = getPlayerInstance();
    
    if (player)
    {
        player->disableNoteParticles();
        player->setPlayingState(false, endSound);
        player->setNoteEffectType(eNoteEffect::NO_EFFECT);
        player->stopTimer();
    }
    
    this->unschedule(schedule_selector(GameLogic::checkForParticleCollision));
    this->removeNoteEffectFromSheeps();
    
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->hideStopBtn();
    }
    
    this->regenerateNotes();
}

void GameLogic::checkForParticleCollision(float dt)
{
    auto player = getPlayerInstance();
    
    if (player)
    {
        bool isInverseEffect = (player->getNoteEffectType() == eNoteEffect::INVERSE_NOTE) ? true : false;
        b2Vec2 playerPosition = player->getBox2dBody()->GetPosition();
        int count = 1;
        
        for (Sheep * sheep : _sheeps)
        {
            this->handleMonsterPosition(sheep);
            sheep->updateSheepStatus(playerPosition, isInverseEffect, count);
        }
    }
}

void GameLogic::removeNoteEffectFromSheeps()
{
    const std::vector<std::string> tagValues = {gameTags::SHEEP_TAG};
    __Array* sheepArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Sheep* sheep = nullptr;
    
    CCARRAY_FOREACH(sheepArray, ref)
    {
        sheep = dynamic_cast<Sheep*>(ref);
        if (sheep)
        {
            sheep->setIsExcited(false);
            sheep->handleInput(eAction::STOP_MOVE_RIGHT);
        }
    }
}

#pragma mark -------------------

void GameLogic::initHUD()
{
    eLevelType levelType = Utility::getLevelType(_levelName);
    Node * player = this->getPlayerInstance();
    auto hud = HUD::create(levelType, player);
    hud->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
    this->addChild(hud, static_cast<int>(eSceneZOrders::HUD_LAYER), static_cast<int>(eGameLogicChild::HUD_LAYER));
}

void GameLogic::initPlayerPosition(GameContext context)
{
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portals = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    ScoreManager * score = ScoreManager::getInstance();
    
    eLevelType levelType = Utility::getLevelType(context.getLevelName());
    if (eLevelType::LEVEL_SELECTION == levelType)
    {
        // Posiziono il player accanto al portale di provenienza nel LevelSelection
        int level = Utility::getLevelFromLevelName(context.getPreviousLevelName());
        if(level != -1)
        {
            CCARRAY_FOREACH(portals, ref)
            {
                portal = dynamic_cast<Portal*>(ref);
                if (portal && portal->getDestinationLevelNumber() == level)
                {
                    Node * gameWorld = this->getChildNodeWithName(sceneChilds::GAME_WORLD);
                    Point position = portal->getParent()->convertToWorldSpace(portal->getPosition());
                    
                    Point offset;
                    if (score->isLevelWon(context.getPreviousLevelName()))
                    {
                        offset = Point( + 100.0f, 0.0f);
                    }
                    else
                    {
                        offset = Point( - 100.0f, 0.0f);
                    }
                    
                    position = gameWorld->convertToNodeSpace(position) + offset;
                    this->relocatePlayer(position);
                }
            }
        }
    }
    else if (eLevelType::MAIN_MENU == levelType)
    {
        if (!_isCinematicScene)
        {
            // Posiziono il player accanto al portale di provenienza nel MainMenu
            int world = Utility::getWorldFromLevelName(context.getPreviousLevelName());
            if(world == -1)
            {
                // Ho appena aperto il gioco
#ifdef TEST_PLAYER_INIT_POSITION
                // Solo in caso di feedback di mondo sbloccato mi pongo accanto al portale del mondo precedente.
                // Altrimenti riparto sempre dall'inizio
                if (score->getFeedbackWorldUnlock() != -1)
                {
                    world = score->getFeedbackWorldUnlock() - 1;
                }
#else
                // Se non ho un portale di provenienza, mi pongo accanto al portale dell'ultimo mondo
                // sbloccato
                if (score->hasPlayerWonFirstLevel())
                {
                    world = score->getLastWorldUnlocked();
                }

                // Se è previsto un feedback di mondo sbloccato, mi pongo accanto al mondo precedente.
                if (score->getFeedbackWorldUnlock() == world && score->isWorldUnLocked(world))
                {
                    world--;
                }
#endif
            }
            
            Node * worldPortal = this->getWorldPortal(world);
            if (worldPortal)
            {
                Node * gameWorld = this->getChildNodeWithName(sceneChilds::GAME_WORLD);
                Point position = worldPortal->getParent()->convertToWorldSpace(worldPortal->getPosition());
                position = gameWorld->convertToNodeSpace(position) - Point(100.0f, 0.0f);
                this->relocatePlayer(position);
            }
        }
    }
}

void GameLogic::initTutorial()
{
    auto tutorial = this->getTutorialInstance();
    if (tutorial && tutorial->containsTag(gameTags::CONTROLLER_TAG))
    {
        if (_isCinematicScene)
        {
            tutorial->removeFromParentAndCleanup(true);
        }
        else
        {
            tutorial->initVideoGuide();
        }
    }
}

int GameLogic::initSheepCounter()
{
    int counter = 0;
    const std::vector<std::string> tagValues = {gameTags::SHEEP_TAG};
    __Array* sheepArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Sheep* sheep = nullptr;
    
    CCARRAY_FOREACH(sheepArray, ref)
    {
        sheep = dynamic_cast<Sheep*>(ref);
        if (sheep && !sheep->containsTag(gameTags::MONSTER_TAG))
        {
            counter++;
        }
    }

    auto hud = this->getHUDInstance();
    if (hud)
    {
        bool fluteExist = ScoreManager::getInstance()->isFluteExistInLevel(_levelName);
        
        hud->initSheepCounter(counter, fluteExist);
        if (fluteExist && ScoreManager::getInstance()->loadTakenFluteForLevel(_levelName))
        {
            hud->activeFlute();
        }
    }
    
    return counter;
}

void GameLogic::decrementSheepCounter()
{
    _sheepCounter--;
    notifySheepDecrement();
}

void GameLogic::notifySheepDecrement()
{
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->decrementSheepCounter();
    }

}

bool GameLogic::checkIfVictory()
{
    return (_sheepCounter == 0);
}

void GameLogic::createLoseScreen()
{
    if (!_isLose && !_isWin)
    {
        this->resetAudio();
        
        Player * player = this->getPlayerInstance();
        if (player)
        {
            if(player->getSoundDeath()) player->getSoundDeath()->playOnce();
        }
        
        _isLose = true;
        
        
        // stop input
        auto hud = this->getHUDInstance();
        
        if (hud)
        {
            hud->pauseNavigationMenuListener();
        }
        
        _inputController.resetAllCommand();
        this->setInputEnabled(false);
        
        this->losePopUpSchedule();
    }
}

void GameLogic::initWorldLevelLabel()
{
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->initWorldLevelLabel(_levelName);
    }
}

void GameLogic::initSheeps()
{
    auto sheeps = (this->getChildrenWithTags({gameTags::SHEEP_TAG}));
    Ref * ref = nullptr;
    Sheep * sheep = nullptr;
    
    auto player = this->getPlayerInstance();
    if (player)
    {
        CCARRAY_FOREACH(sheeps, ref)
        {
            sheep = dynamic_cast<Sheep*>(ref);
            if (sheep)
            {
                sheep->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
                _sheeps.push_back(sheep);
            }
        }
    }
}

void GameLogic::initWolf()
{
    const std::vector<std::string> tagValues = {gameTags::WOLF_TAG};
    __Array* wolfArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Wolf* wolf = nullptr;
    
    CCARRAY_FOREACH(wolfArray, ref)
    {
        wolf = dynamic_cast<Wolf*>(ref);
        if (wolf)
        {
            wolf->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
        }
    }
}

void GameLogic::initPortals()
{
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portalArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    int currentWorld = Utility::getWorldFromLevelName(_levelName);
    
    ScoreManager * score = ScoreManager::getInstance();
    Node * gameWorld = this->getChildNodeWithName(sceneChilds::GAME_WORLD);
    
    CCARRAY_FOREACH(portalArray, ref)
    {
        portal = dynamic_cast<Portal*>(ref);
        if (portal && gameWorld)
        {
            portal->initDecorations(gameWorld);
            portal->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
            
            if (ePortalType::WORLD == portal->getPortalType())
            {
                int destinationWorld = portal->getDestinationWorldNumber();
                if (destinationWorld == appParams::BONUS_WORLD)
                {
                    if (score->getFluteTakenCount() > 1
                        || ( score->getFluteTakenCount() == 1  && !_isCinematicScene ) )
                    {
                        portal->open();
                    }
                }
                else if (score->isWorldUnLocked(destinationWorld) && score->getFeedbackWorldUnlock() != destinationWorld)
                {
                    portal->open();
                    if (score->isWorldCompleted(destinationWorld))
                    {
                        portal->setCompleteScoreSheep();
                    }
                    if (score->isWorldFluteTaken(destinationWorld))
                    {
                        portal->setCompleteScoreFlute();
                    }
                }
            }
            else if (ePortalType::LEVEL == portal->getPortalType())
            {
                char levelName[50] = "";
                Utility::getLevelName(currentWorld, portal->getDestinationLevelNumber(), levelName);
                char prevLevel[50] = "";
                Utility::getPrevLevelName(levelName, prevLevel);
                if (score->isLevelWon(prevLevel))
                {
                    portal->open();
                    if (score->isLevelCompleted(levelName))
                    {
                        portal->setCompleteScoreSheep();
                    }
                    if (score->loadTakenFluteForLevel(levelName))
                    {
                        portal->setCompleteScoreFlute();
                    }
                }
                if (!score->isFluteExistInLevel(levelName))
                {
                    portal->removeScoreFluteIcon();
                }
            }
            else if (ePortalType::EXIT == portal->getPortalType())
            {
                portal->open();
            }
        }
    }
}

void GameLogic::initMonousePlatforms()
{
    const std::vector<std::string> tagValues = {gameTags::MONOUSE_TAG};
    __Array* monouseArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    MonousePlatform* monousePlatform = nullptr;
    
    int worldNumber = Utility::getWorldFromLevelName(_levelName);
    
    CCARRAY_FOREACH(monouseArray, ref)
    {
        monousePlatform = dynamic_cast<MonousePlatform *>(ref);
        if (monousePlatform)
        {
            monousePlatform->initAnimation(worldNumber);
        }
    }
}

void GameLogic::initNotes()
{
    const std::vector<std::string> tagValues = {gameTags::NOTE_TAG};
    __Array* noteArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Note* note = nullptr;
    
    CCARRAY_FOREACH(noteArray, ref)
    {
        note = dynamic_cast<Note *>(ref);
        if (note)
        {
            note->setRegenerationTime(_noteDuration);
        }
    }
}

void GameLogic::initFlute()
{
    _fluteExists = false;
    
    const std::vector<std::string> tagValues = {gameTags::FLUTE_TAG};
    __Array* noteArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Flute* flute = nullptr;
    
    CCARRAY_FOREACH(noteArray, ref)
    {
        flute = dynamic_cast<Flute *>(ref);
        if (flute)
        {
            _fluteExists = true;
#ifndef EDITOR_MODE
            bool fluteAlreadyTaken = ScoreManager::getInstance()->loadTakenFluteForLevel(_levelName);
            if (fluteAlreadyTaken)
            {
                flute->removeFromParent();
                if (_cameraManager)
                {
                    _cameraManager->handleFluteTaken();
                }
            }
#endif
        }
    }
}

void GameLogic::updatePortals()
{
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portalArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    CCARRAY_FOREACH(portalArray, ref)
    {
        portal = dynamic_cast<Portal*>(ref);
        if (portal && (portal->getState() == ePortalState::CLOSED_PORTAL))
        {
            portal->open();
        }
    }
}

void GameLogic::burnPlayer()
{
    auto player = this->getPlayerInstance();
    if (player)
    {
        player->burn();
    }
}

void GameLogic::callbackFunc(Node* sender)
{
    std::string senderName = sender->getName();
    
    if (senderName == callbackDefs::RESTART)
    {
        this->restartLevel();
    }
    else if (senderName == callbackDefs::ZOOM)
    {
        _cameraManager->zoomToggle();
    }
    else if (senderName == callbackDefs::PAUSE)
    {
        this->pauseLevel();
    }
    else if (senderName == callbackDefs::RESUME)
    {
        this->resumeLevel();
    }
    else if (senderName == callbackDefs::NOTIFICATION_RESUME)
    {
        this->handleDismissNotification();
    }
    else if (senderName == callbackDefs::HOME)
    {
        this->backToMenu();
    }
    else if (senderName == callbackDefs::NEXT)
    {
        this->nextLevel();
    }
    else if (senderName == callbackDefs::BACK)
    {
        this->previousLevel();
    }
    else if (senderName == callbackDefs::STOP_NOTE)
    {
        this->timerEnded();
    }
    else if (senderName == callbackDefs::WOLF)
    {
//        this->createLoseScreen();
    }
    else if (senderName == callbackDefs::PORTAL)
    {
        this->enterPortalNormal();
    }
    else if (senderName.find(callbackDefs::PORTAL_WORLD_ROOT) != std::string::npos)
    {
        this->enterPortalWorld(senderName);
    }
    else if (senderName.find(callbackDefs::PORTAL_LEVEL_ROOT) != std::string::npos)
    {
        this->enterPortalLevel(senderName);
    }
    else if (senderName.find(callbackDefs::PORTAL_MAIN_MENU) != std::string::npos)
    {
        this->enterPortalMainMenu();
    }
    else if (senderName.find(callbackDefs::VIDEO_PLAY) != std::string::npos)
    {
        this->handleStoryVideoPlay();
    }
    else if (senderName.find(callbackDefs::VIDEO_STOP) != std::string::npos)
    {
        this->handleStoryVideoStop();
    }
    else if (senderName.find(callbackDefs::VIDEO_LOCKED) != std::string::npos)
    {
        this->handleStoryVideoLockedPressed();
    }
    else if (senderName == callbackDefs::SHEEP_EXITING)
    {
        this->handleSheepExiting(sender);
    }
}

void GameLogic::restartLevel()
{
    this->goToLevel(eEndLevelMode::RESTART, this->getLevelName());
}

void GameLogic::backToMenu()
{
    int destinationWorld = Utility::getWorldFromLevelName(_levelName);
    this->goToLevelSelection(eEndLevelMode::QUIT, destinationWorld);
}

void GameLogic::pauseLevel()
{
    this->pauseLevelWithPopUp(ePopUps::PAUSE_POP_UP);
}

void GameLogic::pauseLevelWithPopUp(ePopUps popUpType)
{
    Utility::pauseAllChildrenRecursive(this);
    
    auto hud = this->getHUDInstance();
    
    PopUp * popUp;
    switch(popUpType)
    {
        case ePopUps::PAUSE_POP_UP:
        {
            eLevelType levelType = Utility::getLevelType(_levelName);
            popUp = PausePopUp::create(levelType);
            break;
        }
        case ePopUps::CONFIRM_INPUT_POP_UP:
            popUp = ConfirmInputPopUp::create();
            break;
        default:
        {
            eLevelType levelType = Utility::getLevelType(_levelName);
            popUp = PausePopUp::create(levelType);
            break;
        }
    }
    
    popUp->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
    this->addChild(popUp, static_cast<int>(eSceneZOrders::PAUSE_POP_UP), static_cast<int>(eGameLogicChild::PAUSE_POP_UP));
    
    if (hud)
    {
        hud->pauseNavigationMenuListener();
    }
    
    _inputController.resetAllCommand();
    this->updateStatus();
    
    this->pauseAudio();
}

void GameLogic::showNotification(eNotification notificationType)
{
    if (nullptr == this->getChildByTag(static_cast<int>(eGameLogicChild::NOTIFICATION)))
    {
        NotificationPopUp * notification = nullptr;
        switch(notificationType)
        {
            case eNotification::WORLD_UNLOCKED:
                notification = NotificationWorldUnlockPopUp::create();
                break;
            case eNotification::WORLD_LOCKED:
                notification = NotificationWorldLockedPopUp::create();
                break;
            case eNotification::GAME_OVER:
                notification = NotificationGameOverPopUp::create();
                break;
            case eNotification::GAME_NOT_OVER:
                notification = NotificationGameNotOverPopUp::create();
                break;
            case eNotification::EPILOGUE_LOCKED:
                notification = NotificationEpilogueLockedPopUp::create();
                break;
            default:
            {
                CCLOG("Tipo di notifica sconosciuto!!");
                break;
            }
        }
        
        if (notification)
        {
            notification->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
            
            int zOrder = static_cast<int>(eSceneZOrders::PAUSE_POP_UP);
            int tag = static_cast<int>(eGameLogicChild::NOTIFICATION);
            this->addChild(notification, zOrder, tag);
        }
    }
}

void GameLogic::handleDismissNotification()
{
    NotificationPopUp * notification = dynamic_cast<NotificationPopUp*>(this->getChildByTag(static_cast<int>(eGameLogicChild::NOTIFICATION)));
    if (notification)
    {
        notification->removeFromParentAndCleanup(true);
    }
    _customEventLock = false;
}

void GameLogic::resumeLevel()
{
    Utility::resumeAllChildrenRecursive(this);
    
    auto hud = this->getHUDInstance();
    auto node = this->getChildByTag(static_cast<int>(eGameLogicChild::PAUSE_POP_UP));
    
    if (node)
    {
        this->removeChildByTag(static_cast<int>(eGameLogicChild::PAUSE_POP_UP));
    }
    
    if (hud)
    {
        hud->resumeNavigationMenuListenser();
    }
    
    this->resumeAudio();
    
    this->updateInput();
}

void GameLogic::nextLevel()
{
    std::string levelName = this->getLevelName();
    char nextLevelName[50];
    snprintf(nextLevelName, 50, "%s", appParams::PUBLISHED_MAIN_MENU_LEVEL_NAME);
    Utility::getNextLevelName(levelName, nextLevelName);
    
    auto levelArray = Utility::loadLevelsFromFile();
    Ref* ref = nullptr;
    
    if (levelArray)
    {
        CCARRAY_FOREACH(levelArray, ref)
        {
            auto tmp = dynamic_cast<__String*>(ref);
            if (strcmp(tmp->getCString() , nextLevelName) == 0)
            {
                this->goToLevel(eEndLevelMode::WIN, tmp->getCString());
                break;
            }
        }
    }
}

void GameLogic::previousLevel()
{
    std::string levelName = this->getLevelName();
    char prevLevelName[50];
    Utility::getPrevLevelName(levelName, prevLevelName);
    
    auto levelArray = Utility::loadLevelsFromFile();
    Ref* ref = nullptr;
    
    if (levelArray)
    {
        CCARRAY_FOREACH(levelArray, ref)
        {
            auto tmp = dynamic_cast<__String*>(ref);
            if (strcmp(tmp->getCString(), prevLevelName) == 0)
            {
                this->goToLevel(eEndLevelMode::WIN, tmp->getCString());
                break;
            }
        }
    }
}

void GameLogic::enterPortalNormal()
{
    int currentWorld = Utility::getWorldFromLevelName(_levelName);
    
    char nextLevelName[50] = "";
    Utility::getNextLevelName(_levelName, nextLevelName);
    int nextWorld = Utility::getWorldFromLevelName(nextLevelName);
    
    // Feedback di flauto preso
    if (_fluteTaken)
    {
        ScoreManager::getInstance()->setFeedbackFluteTaken(currentWorld);
    }
    
    // Notification di mondo sbloccato
    if (false == ScoreManager::getInstance()->isLevelWon(_levelName) && currentWorld != nextWorld)
    {
        if (-1 == nextWorld)
        {
            // Ultimo livello del gioco
            ScoreManager::getInstance()->setNotificationGameOver();
        }
        else
        {
            // Non ultimo livello del gioco
            ScoreManager::getInstance()->setNotificationUnlockWorld(nextWorld);
        }
    }
    
    // Navigazione
    if (false == ScoreManager::getInstance()->isLevelWon(_levelName) && currentWorld == nextWorld)
    {
        // Livello NON ultimo e giocato per la prima volta
        if (_fluteTaken)
        {
            this->goToMainMenu(eEndLevelMode::WIN, nextLevelName);
        }
        else
        {
            this->nextLevel();
        }
    }
    else
    {
        // Livello ultimo o rigiocato
        if (_fluteTaken)
        {
            char levelSelectionName[50] = "";
            Utility::getLevelSelectionName(currentWorld, levelSelectionName);
            
            this->goToMainMenu(eEndLevelMode::WIN, levelSelectionName);
        }
        else
        {
            this->goToLevelSelection(eEndLevelMode::WIN, currentWorld);
        }
    }
    
//    if (_fluteTaken)
//    {
//        // Flauto preso
//        ScoreManager::getInstance()->setFeedbackFluteTaken(currentWorld);
//        
//        char levelSelectionName[50] = "";
//        Utility::getLevelSelectionName(currentWorld, levelSelectionName);
//        
//        if (ScoreManager::getInstance()->isLevelWon(_levelName))
//        {
//            // Il livello è stato rigiocato dopo averlo già completato
//            this->goToMainMenu(eEndLevelMode::WIN, levelSelectionName);
//        }
//        else
//        {
//            // Il livello è stato completato per la prima volta
//            if (currentWorld == nextWorld)
//            {
//                // Il livello NON è l'ultimo del mondo
//                this->goToMainMenu(eEndLevelMode::WIN, nextLevelName);
//            }
//            else
//            {
//                // Il livello è l'ultimo del mondo
//                ScoreManager::getInstance()->setNotificationUnlockWorld(nextWorld);
//                this->goToMainMenu(eEndLevelMode::WIN, levelSelectionName);
//            }
//        }
//    }
//    else
//    {
//        // Flauto non preso
//        if (ScoreManager::getInstance()->isLevelWon(_levelName))
//        {
//            // Il livello è stato rigiocato dopo averlo già completato
//            this->goToLevelSelection(eEndLevelMode::WIN, currentWorld);
//        }
//        else
//        {
//            // Il livello è stato completato per la prima volta
//            if (currentWorld == nextWorld)
//            {
//                // Il livello NON è l'ultimo del mondo
//                this->nextLevel();
//            }
//            else
//            {
//                // Il livello è l'ultimo del mondo
//                ScoreManager::getInstance()->setNotificationUnlockWorld(nextWorld);
//                this->goToLevelSelection(eEndLevelMode::WIN, currentWorld);
//            }
//        }
//    }
}

void GameLogic::enterPortalMainMenu()
{
    this->goToMainMenu(eEndLevelMode::NAVIGATE);
}

void GameLogic::enterPortalWorld(std::string portalCallbackName)
{
    if (portalCallbackName.find(callbackDefs::PORTAL_WORLD_ROOT) != std::string::npos)
    {
        std::string portalWorldRoot = callbackDefs::PORTAL_WORLD_ROOT;
        std::string worldNumberStr = portalCallbackName.substr(portalWorldRoot.size(), portalWorldRoot.size() + 2);
        int worldId = atoi(worldNumberStr.c_str());
        
        this->goToLevelSelection(eEndLevelMode::NAVIGATE, worldId);
    }
    else
    {
        CCLOG("CallbackName Portale errato: %s", portalCallbackName.c_str());
    }
}

void GameLogic::enterPortalLevel(std::string portalCallbackName)
{
    if (portalCallbackName.find(callbackDefs::PORTAL_LEVEL_ROOT) != std::string::npos)
    {
        std::string portalLevelRoot = callbackDefs::PORTAL_LEVEL_ROOT;
        std::string levelNumberStr = portalCallbackName.substr(portalLevelRoot.size(), portalLevelRoot.size() + 2);
        int levelId = atoi(levelNumberStr.c_str());
        
        int world = Utility::getWorldFromLevelName(_levelName);
        
        char levelName[50];
        Utility::getLevelName(world, levelId, levelName);
        
        this->goToLevel(eEndLevelMode::NAVIGATE, levelName);
    }
    else
    {
        CCLOG("CallbackName Portale errato: %s", portalCallbackName.c_str());
    }
}

void GameLogic::goToLevel(eEndLevelMode endMode, std::string level)
{
    if (false == FileUtils::getInstance()->isFileExist(level) || _exiting)
    {
        return;
    }
    _exiting = true;
    
    GameContext context(level, _context.getLevelName());
    
    this->handleEndLevel(endMode, context);
    
    eLevelType levelType = Utility::getLevelType(_levelName);
    
    int currentWorld = Utility::getWorldFromLevelName(_levelName);
    int newWorld = Utility::getWorldFromLevelName(level);
    int newLevel = Utility::getLevelFromLevelName(level);
    
    if (currentWorld != newWorld || eLevelType::NORMAL != levelType || (newWorld == 5 && newLevel == 7))
    {
        if (_soundMusic)
        {
            _soundMusic->stopPlay(true);
        }
    }
    if (currentWorld != newWorld || (newWorld == 5 && newLevel == 7))
    {
        if (_soundAmbience)
        {
            _soundAmbience->stopPlay(true);
        }
    }
    this->resetAudio();
    
    this->transitionToScene(context);
}

void GameLogic::goToLevelSelection(eEndLevelMode endMode, int worldId)
{
    if (_exiting)
    {
        return;
    }
    _exiting = true;
    
    eLevelType levelType = Utility::getLevelType(_levelName);
    if (eLevelType::NORMAL == levelType)
    {
        if (_soundMusic)
        {
            _soundMusic->stopPlay(true);
        }
        if (_soundAmbience)
        {
            _soundAmbience->stopPlay(true);
        }
    }
    
    this->resetAudio();
    
#ifdef EDITOR_MODE
    auto scene = Scene::create();
    EditorTableView *layer = EditorTableView::create();
    scene->addChild(layer);
    
    if (scene)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    }
#endif
    
    char levelSelectionName[50];
    Utility::getLevelSelectionName(worldId, levelSelectionName);
    GameContext context(levelSelectionName, _context.getLevelName());
    
    this->handleEndLevel(endMode, context);
    
    this->transitionToScene(context);
}

void GameLogic::goToMainMenu(eEndLevelMode endMode)
{
    this->goToMainMenu(endMode, "");
}

void GameLogic::goToMainMenu(eEndLevelMode endMode, std::string afterFeedbackLevel)
{
    if (_exiting)
    {
        return;
    }
    _exiting = true;
    
    eLevelType levelType = Utility::getLevelType(_levelName);
    if (eLevelType::NORMAL == levelType)
    {
        if (_soundMusic)
        {
            _soundMusic->stopPlay(true);
        }
        if (_soundAmbience)
        {
            _soundAmbience->stopPlay(true);
        }
    }
    
    this->resetAudio();
    
    GameContext context(appParams::PUBLISHED_MAIN_MENU_LEVEL_NAME, _context.getLevelName());
    
    this->handleEndLevel(endMode, context);
    
    if (!afterFeedbackLevel.empty())
    {
        context.setAfterFeedbackLevelName(afterFeedbackLevel);
        context.setAfterFeedbackPreviousLevelName(_levelName);
    }
    
    this->transitionToScene(context);
}

void GameLogic::transitionToScene(GameContext nextLevelContext)
{
    Scene * scene = LoadingLayer::scene(nextLevelContext);
    
    if (scene)
    {
        Director::getInstance()->replaceScene(TransitionFade::create(1.0, scene, Color3B::WHITE));
    }
}

void GameLogic::handleEndLevel(eEndLevelMode endMode, GameContext& nextLevelContext)
{
    auto hud = this->getHUDInstance();
    if (hud)
    {
        hud->pauseNavigationMenuListener();
    }
    this->setInputEnabled(false);
    
    // Gestione feedback di flauto preso
    if (!_context.getAfterFeedbackPreviousLevelName().empty())
    {
        nextLevelContext.setPreviousLevelName(_context.getAfterFeedbackPreviousLevelName());
    }
    
    eLevelType levelType = Utility::getLevelType(_levelName);
    
    switch(endMode)
    {
        case eEndLevelMode::WIN:
        {
            // Salvo l'ultimo livello completato
            ScoreManager::getInstance()->saveBestCompletedLevel(_levelName);
            
            int worldJustUnlocked = -1;
            bool isGameJustFinished = false;
            this->updateSheepCounterGlobal(&worldJustUnlocked, &isGameJustFinished);
            
            // Preparo le notifiche in caso di sblocco di un nuovo mondo
            if (-1 != worldJustUnlocked)
            {
                ScoreManager::getInstance()->setFeedbackWorldUnlock(worldJustUnlocked);
                ScoreManager::getInstance()->setNotificationUnlockWorld(worldJustUnlocked);
            }
            
            // Preparo la notifica in caso di fine gioco
            if (isGameJustFinished)
            {
                ScoreManager::getInstance()->setNotificationGameOver();
            }
            
            // Se ho preso il flauto, salvo l'informazione
            if (_fluteTaken)
            {
                CCLOG("Preso primo pezzo di flauto");
#ifndef EDITOR_MODE
                ScoreManager::getInstance()->saveTakenFluteForLevel(_levelName);
#endif
            }
            
            int caughtSheeps = _initialSheepQuantity - _sheepCounter;
            eAnalyticsOptionFlute fluteOption = eAnalyticsOptionFlute::NOT_AVAILABLE;
            if (true == _fluteExists)
            {
                if (true == _fluteTaken)
                {
                    fluteOption = eAnalyticsOptionFlute::TAKEN;
                }
                else
                {
                    fluteOption = eAnalyticsOptionFlute::MISSED;
                }
            }
            AnalyticsManager::sendEventEndLevel(_levelName, caughtSheeps, _context.getNumberOfRestarts(), fluteOption, true);
            
            // Feedback di tutte le pecore prese
            if (_sheepCounter == 0 && _soundAllSheepFeedback)
            {
                _soundAllSheepFeedback->playOnce(true);
                _soundAllSheepFeedback->setDestroySoundOnExit(false);
            }
            
            CCLOG("Victory");
            break;
        }
        case eEndLevelMode::RESTART:
        {
            nextLevelContext.setNumberOfRestarts(_context.getNumberOfRestarts() + 1);
            
            CCLOG("Restart");
            break;
        }
        case eEndLevelMode::QUIT:
        {
            // Segnalo la "Resa" agli Analytics
            if (eLevelType::NORMAL == levelType)
            {
                int caughtSheeps = _initialSheepQuantity - _sheepCounter;
                eAnalyticsOptionFlute fluteOption = eAnalyticsOptionFlute::NOT_AVAILABLE;
                if (true == _fluteExists)
                {
                    fluteOption = eAnalyticsOptionFlute::MISSED;
                }
                AnalyticsManager::sendEventEndLevel(_levelName, caughtSheeps, _context.getNumberOfRestarts(), fluteOption, false);
            }
            
            CCLOG("Quit");
            break;
        }
        case eEndLevelMode::NAVIGATE:
        {
            CCLOG("Navigation");
            break;
        }
        default:
            break;
    }
}

void GameLogic::initParallax()
{
    LHParallax * parallax = dynamic_cast<LHParallax *>(this->getChildNodeWithName(sceneChilds::PARALLAX));
    if (parallax)
    {
        parallax->followNode(this);
    }
}

void GameLogic::initCameraManager()
{
    Player * player = this->getPlayerInstance();
    Node * cameraTarget = this->getChildNodeWithName("GAME WORLD");
    _cameraManager = CameraManager::create(this, cameraTarget, player, _levelName);
    this->addChild(_cameraManager);
    
    // Inizializzazione particolare per il feedback di flauto preso
    if (eLevelType::MAIN_MENU == Utility::getLevelType(_levelName))
    {
        int worldOrigin = ScoreManager::getInstance()->getFeedbackFluteTaken();
        if (-1 != worldOrigin)
        {
            Portal * worldPortal = this->getWorldPortal(worldOrigin);
            if (worldPortal)
            {
                _cameraManager->cameraInstantFocusOnNode(worldPortal);
            }
        }
    }
}

LHNodeProtocol* GameLogic::getMovablePlatform(Node *nodeA, Node *nodeB)
{
    LHNodeProtocol* block = dynamic_cast<LHNodeProtocol*>(nodeA);
    
    if ((block == nullptr) || !this->isMovablePlatform(block))
    {
        block = dynamic_cast<LHNodeProtocol*>(nodeB);
        if (block && !isMovablePlatform(block))
        {
            block = nullptr;
        }
    }
    
    return block;
}

bool GameLogic::isMovablePlatform(LHNodeProtocol* block)
{
    bool res = false;
    
    if (block && block->containsTag(gameTags::BLOCK_TAG) && block->containsTag(gameTags::MOVABLE_TAG))
    {
        res = true;
    }
   
    return res;
}

LHNodeProtocol* GameLogic::getInspirationNode(Node *nodeA, Node *nodeB)
{
    LHNodeProtocol* node = dynamic_cast<LHNodeProtocol*>(nodeA);
    
    if ((node == nullptr) || !node->containsTag(gameTags::INSPIRATION_TAG))
    {
        node = dynamic_cast<LHNodeProtocol*>(nodeB);
        if (node && !node->containsTag(gameTags::INSPIRATION_TAG))
        {
            node = nullptr;
        }
    }
    
    return node;
}

LHNodeProtocol* GameLogic::getGoalNode(Node *nodeA, Node *nodeB)
{
    LHNodeProtocol* node = dynamic_cast<LHNodeProtocol*>(nodeA);
    
    if ((node == nullptr) || !node->containsTag(gameTags::GOAL_TAG))
    {
        node = dynamic_cast<LHNodeProtocol*>(nodeB);
        if (node && !node->containsTag(gameTags::GOAL_TAG))
        {
            node = nullptr;
        }
    }
    
    return node;
}

void GameLogic::turnOnGoatskin(Goatskin *goatskin)
{
    if (goatskin)
    {
        auto sprite = dynamic_cast<LHSprite*>(goatskin->getChildNodeWithName(goatskinDefs::SPRITE_NAME));
        
        if (sprite)
        {
            DelayTime* delay1 = DelayTime::create(0.2f);
            CallFunc * openCallback = CallFunc::create([goatskin]()
            {
                goatskin->openAction();
            });
            
            CallFunc* blowCallback = CallFunc::create([this, goatskin]()
            {
                goatskin->blowAction();
                this->checkForNotesToMove(goatskin);
                this->checkForBrazierToTurnOff(goatskin);
            });
            
            DelayTime* delay2 = DelayTime::create(0.8f);
            
            CallFunc* closeCallback = CallFunc::create([goatskin]()
            {
                goatskin->closeAction();
            });
            
            auto _action = Sequence::create(openCallback, delay1, blowCallback, delay2, closeCallback, NULL);
            sprite->runAction(_action);
        }
    }
}

Goatskin* GameLogic::getGoatskin(Node *nodeA, Node *nodeB)
{
    auto goatskinA = dynamic_cast<Goatskin*>(nodeA);
    auto goatskinB = dynamic_cast<Goatskin*>(nodeB);
    
    if (goatskinA && goatskinB)
    {
        if (goatskinA->getBox2dBody()->GetPosition().y < goatskinB->getBox2dBody()->GetPosition().y)
        {
            return goatskinA;
        }
    }
    else if (goatskinA)
    {
        return goatskinA;
    }
    
    return goatskinB;
}

void GameLogic::checkForNotesToMove(Goatskin* goatskin)
{
    const std::vector<std::string> tagValues = {gameTags::NOTE_TAG};
    __Array* noteArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Note* note = nullptr;
    
    CCARRAY_FOREACH(noteArray, ref)
    {
        note = dynamic_cast<Note*>(ref);
        if (note)
        {
            note->moveIfInWindRange(goatskin);
        }
    }
}

void GameLogic::checkForBrazierToTurnOff(Goatskin *goatskin)
{
    const std::vector<std::string> tagValues = {gameTags::BRAZIER_TAG};
    __Array* brazierArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Brazier* brazier = nullptr;
    
    CCARRAY_FOREACH(brazierArray, ref)
    {
        brazier = dynamic_cast<Brazier*>(ref);
        if (brazier && (brazier->getBrazierStatus() == eBrazierStatus::BRAZIER_ON))
        {
            brazier->TurnOffIfInWindRange(goatskin);
        }
    }
}

void GameLogic::initGoal()
{
    const std::vector<std::string> tagValues = {gameTags::GOAL_TAG};
    __Array* goalArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    LHNodeProtocol* goal = nullptr;
    
    CCARRAY_FOREACH(goalArray, ref)
    {
        goal = dynamic_cast<LHNodeProtocol*>(ref);
        if (goal)
        {
            LHSprite * goalBurnIntro = dynamic_cast<LHSprite*>(goal->getChildNodeWithName("CapannaBruciaIntro"));
            if (goalBurnIntro)
            {
                goalBurnIntro->setVisible(false);
            }
            LHSprite * goalBurnLoop = dynamic_cast<LHSprite*>(goal->getChildNodeWithName("CapannaBruciaLoop"));
            if (goalBurnLoop)
            {
                goalBurnLoop->setVisible(false);
            }
        }
    }
}

void GameLogic::burnGoal(LHNodeProtocol * goalNodeProt)
{
    const std::vector<std::string> tagValues = {gameTags::GOAL_TAG};
    __Array* goalArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    LHNodeProtocol* goal = nullptr;
    
    CCARRAY_FOREACH(goalArray, ref)
    {
        goal = dynamic_cast<LHNodeProtocol*>(ref);
        if (goal)
        {
            if (_soundGoalBurn)
            {
                _soundGoalBurn->startPlay();
            }
            
            float introTime = 0.0f;
            LHSprite * goalBurnIntro = dynamic_cast<LHSprite*>(goal->getChildNodeWithName("CapannaBruciaIntro"));
            if (goalBurnIntro)
            {
                goalBurnIntro->setVisible(true);
                LHAnimation * burnIntroAnimation = goalBurnIntro->getAnimationWithName("burnIntro");
                if (burnIntroAnimation)
                {
                    introTime = burnIntroAnimation->totalTime();
                    burnIntroAnimation->setActive(true);
                    burnIntroAnimation->restart();
                }
            }
            LHSprite * goalBurnLoop = dynamic_cast<LHSprite*>(goal->getChildNodeWithName("CapannaBruciaLoop"));
            if (goalBurnLoop)
            {
                LHAnimation * burnLoopAnimation = goalBurnLoop->getAnimationWithName("burnLoop");
                if (burnLoopAnimation)
                {
                    DelayTime * delayIntro = DelayTime::create(introTime);
                    CallFunc * loopCallback = CallFunc::create([goalBurnLoop, burnLoopAnimation]() {
                        goalBurnLoop->setVisible(true);
                        burnLoopAnimation->setActive(true);
                        burnLoopAnimation->restart();
                    });
                    this->runAction(Sequence::createWithTwoActions(delayIntro, loopCallback));
                }
            }
        }
    }
}

void GameLogic::initDoorSwitchConnections()
{
    const std::vector<std::string> tagValues = {gameTags::DOOR_TAG};
    __Array* doorArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Door* door = nullptr;
    LHAsset* doorAsset = nullptr;
    std::string doorName;
    
    CCARRAY_FOREACH(doorArray, ref)
    {
        door = dynamic_cast<Door*>(ref);
        if (door)
        {
            doorAsset = dynamic_cast<LHAsset*>(door->getParent());
            if (doorAsset)
            {
                doorName = doorAsset->getName();
                setSwitchObserverIfExist(door, doorName);
            }
        }
    }
}

void GameLogic::setSwitchObserverIfExist(Door* door, std::string doorName)
{
    const std::vector<std::string> assetTagValues = {doorName};
    const std::vector<std::string> objTagValues = {gameTags::SWITCH_TAG, gameTags::GEAR_TAG};
    __Array* objAssetArray = this->getChildrenWithTags(assetTagValues);
    __Array* doorObjArray;
    Ref* assetRef = nullptr;
    Ref* switchRef = nullptr;
    Switch* doorSwitch = nullptr;
    Gear* doorGear = nullptr;

    CCARRAY_FOREACH(objAssetArray, assetRef)
    {
        LHAsset* doorObjAsset = dynamic_cast<LHAsset*>(assetRef);
        if (doorObjAsset)
        {
            doorObjArray = doorObjAsset->getChildrenWithTags(objTagValues);
            CCARRAY_FOREACH(doorObjArray, switchRef)
            {
                doorSwitch = dynamic_cast<Switch*>(switchRef);
                if (doorSwitch)
                {
                    doorSwitch->addObserver(door);
                    door->updateSwitchCounter();
                }
                
                doorGear = dynamic_cast<Gear*>(switchRef);
                if (doorGear)
                {
                    door->addObserver(doorGear);
                }
            }
        }
    }
}

void GameLogic::initEffects()
{
    EffectsManager * effectsManager = EffectsManager::createWithLogic(this);
    effectsManager->setName(sceneChilds::EFFECTS);
    this->addChild(effectsManager);
}

void GameLogic::initAnalytics(GameContext& context)
{
    // se non sto facendo restart e sono su un livello normale, lancio l'evento di inizio livello
    if (context.getLevelName().compare(context.getPreviousLevelName()) != 0
        && eLevelType::NORMAL == Utility::getLevelType(context.getLevelName()))
    {
        AnalyticsManager::sendEventStartLevel(context.getLevelName());
    }
}

void GameLogic::initInteractive()
{
    auto interactive = (this->getChildrenWithTags({gameTags::INTERACTIVE_TAG}));
    Ref* ref = nullptr;
    InteractiveBlock* block = nullptr;
    
    auto player = this->getPlayerInstance();
    CCARRAY_FOREACH(interactive, ref)
    {
        block = dynamic_cast<InteractiveBlock*>(ref);
        if (block)
        {
            block->setPlayer(player);
        }
    }
}

void GameLogic::initEoloZones()
{
    auto eoloZone = (this->getChildrenWithTags({gameTags::START_EOLO_TAG}));
    Ref* ref = nullptr;
    LHNode* node = nullptr;
    
    CCARRAY_FOREACH(eoloZone, ref)
    {
        node = dynamic_cast<LHNode*>(ref);
        if (node)
        {
            auto eoloZone = dynamic_cast<EoloZone*>(node);
            auto nodeAsset = dynamic_cast<LHAsset*>(node->getParent());
            
            if (eoloZone && nodeAsset)
            {
                auto eoloStuff = (this->getChildrenWithTags({nodeAsset->getName()}));
                
                Ref* ref_eoloStuff = nullptr;
                LHNode* eoloBezierNode = nullptr;
                LHAsset* eoloAsset = nullptr;
                Eolo* eoloNode = nullptr;
                
                CCARRAY_FOREACH(eoloStuff, ref_eoloStuff)
                {
                    eoloAsset = dynamic_cast<LHAsset*>(ref_eoloStuff);
                    if (eoloAsset)
                    {
                        eoloNode = dynamic_cast<Eolo*>(eoloAsset->getChildNodeWithName(eoloDefs::NODE_NAME));
                        eoloZone->addObserver(eoloNode);
                    }
                    else
                    {
                        eoloBezierNode = dynamic_cast<LHNode*>(ref_eoloStuff);
                        if (eoloBezierNode)
                        {
                            eoloZone->setBezierNode(eoloBezierNode);
                        }
                    }
                }
            }
        }
    }
}

void GameLogic::initBonusWorldPlatform()
{
    if (eLevelType::MAIN_MENU == Utility::getLevelType(_levelName) && ScoreManager::getInstance()->getFluteTakenCount() == 0)
    {
        this->setBonusPlatformEnabled(false, 0.0f);
    }
}

void GameLogic::initCredits()
{
    SheepShow * sheepShow = dynamic_cast<SheepShow*>(this->getChildNodeWithName(appParams::SHEEP_SHOW_NODE_NAME));
    if (sheepShow)
    {
        sheepShow->initCredits(this);
    }
}

void GameLogic::initAudio()
{
    _soundSheepCollected = SoundComponent::create(SoundComponent::SOUND_SHEEP_COLLECTED);
    this->addChild(_soundSheepCollected);
    
    this->initAudioBraziers();
    this->initAudioSheeps();
    
    int worldNumber = Utility::getWorldFromLevelName(_levelName);
    int levelNumber = Utility::getLevelFromLevelName(_levelName);
    
    _soundMusic = nullptr;
    _soundAmbience = nullptr;
    _soundAllSheepFeedback = nullptr;
    _soundPortalActive = nullptr;
    _soundPortalNotActive = nullptr;
    _soundGoalBurn = nullptr;
    _soundFall = nullptr;
    
    this->initAudioMusic(worldNumber, levelNumber);
    
    eLevelType levelType = Utility::getLevelType(_levelName);
    if (worldNumber != -1)
    {
        if (eLevelType::NORMAL == levelType)
        {
            this->initAudioAmbience(worldNumber, levelNumber);
            this->initAudioAllSheepFeedback();
        }
        this->initAudioFlute(worldNumber);
    }
    
    this->initAudioPortalFeedback();
    this->initAudioGoalBurn();
    this->initAudioFaithJumpBridge();
}

void GameLogic::initAudioBraziers()
{
    auto braziers = (this->getChildrenWithTags({gameTags::BRAZIER_TAG}));
    Ref* ref = nullptr;
    Brazier* brazier = nullptr;
    
    auto player = this->getPlayerInstance();
    if (player)
    {
        CCARRAY_FOREACH(braziers, ref)
        {
            brazier = dynamic_cast<Brazier*>(ref);
            if (brazier)
            {
                brazier->initAudio(player);
            }
        }
    }
}

void GameLogic::initAudioSheeps()
{
    auto sheeps = (this->getChildrenWithTags({gameTags::SHEEP_TAG}));
    Ref * ref = nullptr;
    Sheep * sheep = nullptr;
    
    auto player = this->getPlayerInstance();
    if (player)
    {
        CCARRAY_FOREACH(sheeps, ref)
        {
            sheep = dynamic_cast<Sheep*>(ref);
            if (sheep)
            {
                sheep->initAudio(player);
            }
        }
    }
}

void GameLogic::initAudioMusic(int worldNumber, int levelNumber)
{
    eLevelType levelType = Utility::getLevelType(_levelName);
    if (eLevelType::MAIN_MENU == levelType || eLevelType::LEVEL_SELECTION == levelType)
    {
        _soundMusic = SoundComponent::create(SoundComponent::SOUND_MUSIC_MENU, true);
        this->addChild(_soundMusic);
        _soundMusic->startPlayCrossScene();
    }
    else
    {
        if (-1 != worldNumber && (worldNumber != 5 || levelNumber != 7))
        {
            char musicName[50];
            snprintf(musicName, 50, SoundComponent::SOUND_MUSIC, worldNumber);
            _soundMusic = SoundComponent::create(musicName, true);
            this->addChild(_soundMusic);
        }
    }
}

void GameLogic::initAudioAmbience(int worldNumber, int levelNumber)
{
    char ambienceFXName[50];
    if (worldNumber == 5 && levelNumber == 7)
    {
        snprintf(ambienceFXName, 50, "%s", SoundComponent::SOUND_AMBIENCE_LAST_LEVEL);
    }
    else
    {
        snprintf(ambienceFXName, 50, SoundComponent::SOUND_AMBIENCE, worldNumber);
    }
    
    _soundAmbience = SoundComponent::create(ambienceFXName, false);
    this->addChild(_soundAmbience);
}

void GameLogic::initAudioFlute(int worldNumber)
{
    auto player = this->getPlayerInstance();
    if (player)
    {
        {
            char fluteName[50];
            snprintf(fluteName, 50, SoundComponent::SOUND_FLUTE, worldNumber);
            SoundComponent* soundFluteStart = SoundComponent::create(fluteName);
            player->addChild(soundFluteStart);
            player->setSoundFluteStart(soundFluteStart);
        }
        
        {
            SoundComponent* soundFluteRedStart = SoundComponent::create(SoundComponent::SOUND_FLUTE_RED);
            player->addChild(soundFluteRedStart);
            player->setSoundFluteRedStart(soundFluteRedStart);
        }
    }
}

void GameLogic::initAudioAllSheepFeedback()
{
    _soundAllSheepFeedback = SoundComponent::create(SoundComponent::SOUND_ALL_SHEEP_FEEDBACK);
    this->addChild(_soundAllSheepFeedback);
}

void GameLogic::initAudioPortalFeedback()
{
    _soundPortalActive = SoundComponent::create(SoundComponent::SOUND_PORTAL_ACTIVE);
    this->addChild(_soundPortalActive);
    
    _soundPortalNotActive = SoundComponent::create(SoundComponent::SOUND_PORTAL_NOT_ACTIVE);
    this->addChild(_soundPortalNotActive);
}

void GameLogic::initAudioGoalBurn()
{
    _soundGoalBurn = SoundComponent::create(SoundComponent::SOUND_GOAL_BURN);
    this->addChild(_soundGoalBurn);
}

void GameLogic::initAudioFaithJumpBridge()
{
    _soundFall = SoundComponent::create(SoundComponent::SOUND_MONOUSE_REVERSE_PLATFORM_FALL);
    this->addChild(_soundFall);
}

void GameLogic::startAudio()
{
    if (_soundMusic)
    {
        _soundMusic->startPlayCrossScene();
    }
    if (_soundAmbience)
    {
        _soundAmbience->startPlayCrossScene();
    }
}

void GameLogic::resetAudio()
{
    bool fadeOut = false;
    Player * player = this->getPlayerInstance();
    if (player)
    {
        if (player->getSoundWalk())
        {
            player->getSoundWalk()->stopPlay(fadeOut);
        }
        
        if (player->getSoundFluteStart())
        {
            player->getSoundFluteStart()->stopPlay(fadeOut);
        }
        
        if (player->getSoundFluteRedStart())
        {
            player->getSoundFluteRedStart()->stopPlay(fadeOut);
        }
        
        if (player->getSoundFluteEnd())
        {
            player->getSoundFluteEnd()->stopPlay(fadeOut);
        }
        
        if (player->getSoundPush())
        {
            player->getSoundPush()->stopPlay(fadeOut);
        }
        
        if (player->getSoundCatchNote())
        {
            player->getSoundCatchNote()->stopPlay(fadeOut);
        }
    }
}

void GameLogic::pauseAudio()
{
    Player * player = this->getPlayerInstance();
    if (player)
    {
        if (player->getSoundWalk())
        {
            player->getSoundWalk()->pauseSound();
        }
        
        if (player->getSoundFluteStart())
        {
            player->getSoundFluteStart()->pauseSound();
        }
        
        if (player->getSoundFluteRedStart())
        {
            player->getSoundFluteRedStart()->pauseSound();
        }
    }
    this->setPauseStoryVideos(true);
    
    SheepShow * sheepShow = dynamic_cast<SheepShow*>(this->getChildNodeWithName(appParams::SHEEP_SHOW_NODE_NAME));
    if (sheepShow)
    {
        sheepShow->setPauseState(true);
    }
    
}

void GameLogic::resumeAudio()
{
    Player * player = this->getPlayerInstance();
    if (player)
    {
        if (player->getSoundWalk())
        {
            player->getSoundWalk()->resumeSound();
        }
        
        if (player->getSoundFluteStart())
        {
            player->getSoundFluteStart()->resumeSound();
        }
        
        if (player->getSoundFluteRedStart())
        {
            player->getSoundFluteRedStart()->resumeSound();
        }
    }
    this->setPauseStoryVideos(false);
    
    SheepShow * sheepShow = dynamic_cast<SheepShow*>(this->getChildNodeWithName(appParams::SHEEP_SHOW_NODE_NAME));
    if (sheepShow)
    {
        sheepShow->setPauseState(false);
    }
}

#pragma mark - Story Videos

void GameLogic::initStoryVideos()
{
    __Array* videoStoryArray = this->getChildrenOfType<VideoStory *>();
    Ref* ref = nullptr;
    VideoStory* videoStory = nullptr;
    
    CCARRAY_FOREACH(videoStoryArray, ref)
    {
        videoStory = dynamic_cast<VideoStory*>(ref);
        if (videoStory)
        {
            videoStory->initVideo(this, _levelName, this->getPlayerInstance());
            videoStory->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
        }
    }
}

void GameLogic::handleStoryVideoPlay()
{
    SoundManager::getInstance()->setSystemVolumeMusic(0.0f, true);
}

void GameLogic::handleStoryVideoStop()
{
    SoundManager::getInstance()->setSystemVolumeMusic(1.0f, true);
}

void GameLogic::handleStoryVideoLockedPressed()
{
    if (appParams::BONUS_WORLD == Utility::getWorldFromLevelName(_levelName))
    {
        this->showNotification(eNotification::EPILOGUE_LOCKED);
    }
}

void GameLogic::setPauseStoryVideos(bool paused)
{
    __Array* videoStoryArray = this->getChildrenOfType<VideoStory *>();
    Ref* ref = nullptr;
    VideoStory* videoStory = nullptr;
    
    CCARRAY_FOREACH(videoStoryArray, ref)
    {
        videoStory = dynamic_cast<VideoStory*>(ref);
        if (videoStory)
        {
            if (paused)
            {
                videoStory->pauseVideo();
            }
            else
            {
                videoStory->resumeVideo();
            }
        }
    }
}

#pragma mark - Update

void GameLogic::update(float dt)
{
    this->updateAudio(dt);
    
    auto player = this->getPlayerInstance();
    
    if (player->getIsPlaying() && (_inputController.isStopNotePressed() || player->isUnderEoloEffect()))
    {
        this->timerEnded();
        _inputController.resetStopNotePressed();
    }
    
    if (_inputController.isZoomPressed())
    {
        _cameraManager->zoomToggle();
    }
    
    if (_inputController.isPausePressed())
    {
        this->pauseLevel();
        _inputController.resetPausePressed();
    }
}

void GameLogic::updateAudio(float dt)
{
    this->updateAudioAmbience(dt);
    this->updateAudioMusic(dt);
}

void GameLogic::updateAudioAmbience(float dt)
{
    if (_soundAmbience)
    {
        auto player = this->getPlayerInstance();
        if (player)
        {
            float playerBodyY = player->getBox2dBody()->GetPosition().y;
            
            _soundAmbience->updateParameter(soundComponentParams::Y_POS, playerBodyY);
        }
    }
}

void GameLogic::updateAudioMusic(float dt)
{
    if (_soundMusic)
    {
        // Aggiorno la musica in base alla posizione di Pan rispetto alla capanna
        auto player = this->getPlayerInstance();
        if (player)
        {
            _soundMusic->updateParameterDistanceFromNode(player);
        }
    }
}

#pragma mark - Custom Events

void GameLogic::updateCustomEvents(float dt)
{
    if (_customEventLock)
    {
        return;
    }
    
    eLevelType levelType = Utility::getLevelType(_context.getLevelName());
    
    if (eLevelType::MAIN_MENU == levelType)
    {
        // Eseguo l'eventuale feedback di flauto preso
        this->resolveFeedbackFluteTaken();
        if (_customEventLock)
        {
            return;
        }
        // Eseguo l'eventuale feedback di sblocco mondi
        this->resolveFeedbackUnlockWorld();
        if (_customEventLock)
        {
            return;
        }
    }
    
    // Eseguo l'eventuale notification di sblocco mondo
    this->resolveNotificationUnlockWorld();
    if (_customEventLock)
    {
        return;
    }
    
    // Eseguo l'eventuale notification di fine gioco
    this->resolveNotificationGameOver();
    if (_customEventLock)
    {
        return;
    }
    
    if (!_customEventLock)
    {
        this->unschedule(schedule_selector(GameLogic::updateCustomEvents));
    }
}

void GameLogic::resolveFeedbackUnlockWorld()
{
    ScoreManager * score = ScoreManager::getInstance();
    int worldToCheck = score->getFeedbackWorldUnlock();
    if (-1 != worldToCheck)
    {
        if (score->isWorldUnLocked(worldToCheck))
        {
            if (_cameraManager)
            {
                Portal * worldPortal = this->getWorldPortal(worldToCheck);
                if (worldPortal)
                {
                    CCLOG("Feedback: Mondo %d sbloccato!!", worldToCheck);
                    
                    _customEventLock = true;
                    
                    this->setInputEnabled(false);
                    
                    DelayTime * delay = DelayTime::create(1.5f);
                    CallFunc * startFeedback = CallFunc::create([this, worldPortal]() {
                        Point startingPosition = _cameraManager->getCurrentPosition();
                        
                        _cameraManager->portalFocusWithCallback(worldPortal, [this, worldPortal, startingPosition](){
                            DelayTime * firstDelay = DelayTime::create(0.5f);
                            float transitionTime = 1.0f;
                            CallFunc * openPortal = CallFunc::create([this, worldPortal, transitionTime]() {
                                if (_soundPortalActive)
                                {
                                    _soundPortalActive->playOnce();
                                }
                                worldPortal->openWithTransition(transitionTime);
                                
                            });
                            DelayTime * secondDelay = DelayTime::create(transitionTime + 0.5f);
                            CallFunc * backToPlayer = CallFunc::create([this, startingPosition]() {
                                _cameraManager->portalFocusWithCallback(startingPosition, [this](){
                                    this->setInputEnabled(true);
                                    _cameraManager->resetCamera();
                                    _customEventLock = false;
                                });
                            });
                            Sequence * seq = Sequence::create(firstDelay, openPortal, secondDelay, backToPlayer, NULL);
                            this->runAction(seq);
                        });
                    });
                    Sequence * seq = Sequence::create(delay, startFeedback, NULL);
                    this->runAction(seq);
                }
            }
        }
    }
    score->resetNotificationUnlockWorld();
    score->resetFeedbackWorldUnlock();
}

void GameLogic::resolveFeedbackFluteTaken()
{
    ScoreManager * score = ScoreManager::getInstance();
    int worldOrigin = score->getFeedbackFluteTaken();
    if (-1 != worldOrigin)
    {
        if (_cameraManager)
        {
            Portal * worldPortal = this->getWorldPortal(appParams::BONUS_WORLD);
            if (worldPortal)
            {
                this->setInputEnabled(false);
                
                bool firstFlute = (score->getFluteTakenCount() == 1);
                
                _customEventLock = true;
                
                if (firstFlute)
                {
                    CCLOG("Feedback: Primo pezzo di flauto preso!!");
                    worldPortal->close();
                    this->setBonusPlatformEnabled(false, 0.0f);
                }
                else
                {
                    CCLOG("Feedback: Ulteriore pezzo di flauto preso!!");
                }
                
                DelayTime * delayFlute = DelayTime::create(1.5f);
                CallFunc * showFlute = CallFunc::create([this]() {
                    HUD * hud = this->getHUDInstance();
                    if (hud)
                    {
                        hud->showFluteFeedback();
                    }
                });
                
                DelayTime * delayCamera = DelayTime::create(2.5f);
                CallFunc * startCamera = CallFunc::create([this, worldPortal, firstFlute]() {
                    Point startingPosition = _cameraManager->getCurrentPosition();
                    
                    _cameraManager->portalFocusWithCallback(worldPortal, [this, worldPortal, firstFlute, startingPosition](){
                        DelayTime * firstDelay = DelayTime::create(0.5f);
                        float transitionTime = 1.0f;
                        CallFunc * openPortal = CallFunc::create([this, worldPortal, firstFlute, transitionTime]() {
                            if (_soundPortalActive)
                            {
                                _soundPortalActive->playOnce();
                            }
                            if (firstFlute)
                            {
                                worldPortal->openWithTransition(transitionTime);
                                this->setBonusPlatformEnabled(true, transitionTime);
                            }
                            HUD * hud = this->getHUDInstance();
                            if (hud)
                            {
                                hud->hideFluteFeedback();
                            }
                        });
                        DelayTime * secondDelay = DelayTime::create(transitionTime + 1.5f);
                        CallFunc * backToPlayer = CallFunc::create([this, startingPosition]() {
                            if (!_context.getAfterFeedbackLevelName().empty())
                            {
                                eLevelType levelType = Utility::getLevelType(_context.getAfterFeedbackLevelName());
                                int world = Utility::getWorldFromLevelName(_context.getAfterFeedbackLevelName());
                                switch (levelType)
                                {
                                    case eLevelType::NORMAL:
                                        this->goToLevel(eEndLevelMode::NAVIGATE, _context.getAfterFeedbackLevelName());
                                        break;
                                    case eLevelType::LEVEL_SELECTION:
                                        this->goToLevelSelection(eEndLevelMode::NAVIGATE, world);
                                        break;
                                    case eLevelType::MAIN_MENU:
                                    default:
                                        this->goToMainMenu(eEndLevelMode::NAVIGATE);
                                        break;
                                }
                            }
                            else
                            {
                                this->goToMainMenu(eEndLevelMode::NAVIGATE);
                            }
                        });
                        Sequence * seq = Sequence::create(firstDelay, openPortal, secondDelay, backToPlayer, NULL);
                        this->runAction(seq);
                    });
                });
                this->runAction(Sequence::create(delayFlute, showFlute, NULL));
                this->runAction(Sequence::create(delayCamera, startCamera, NULL));
            }
        }
    }
    score->resetFeedbackFluteTaken();
}

void GameLogic::resolveNotificationUnlockWorld()
{
    ScoreManager * score = ScoreManager::getInstance();
    int notificationWorld = score->getNotificationUnlockWorld();
    if (-1 != notificationWorld)
    {
        _customEventLock = true;
        if (score->isWorldUnLocked(notificationWorld))
        {
            CCLOG("Notification: Mondo %d sbloccato!!", notificationWorld);
            this->showNotification(eNotification::WORLD_UNLOCKED);
//            this->pauseLevelWithPopUp(ePopUps::NOTIFICATION_WORLD_UNLOCKED);
        }
        else
        {
            CCLOG("Notification: Mondo %d non ancora sbloccato...", notificationWorld);
            this->showNotification(eNotification::WORLD_LOCKED);
//            this->pauseLevelWithPopUp(ePopUps::NOTIFICATION_WORLD_LOCKED);
        }
    }
    score->resetNotificationUnlockWorld();
}

void GameLogic::resolveNotificationGameOver()
{
    ScoreManager * score = ScoreManager::getInstance();
    bool gameOverNotification = score->getNotificationGameOver();
    if (gameOverNotification)
    {
        _customEventLock = true;
        if (score->isAllSheepCaught())
        {
            CCLOG("Notification: Gioco finito!!");
            this->showNotification(eNotification::GAME_OVER);
//            this->pauseLevelWithPopUp(ePopUps::NOTIFICATION_GAME_OVER);
        }
        else
        {
            CCLOG("Notification: Gioco non ancora finito...");
            this->showNotification(eNotification::GAME_NOT_OVER);
//            this->pauseLevelWithPopUp(ePopUps::NOTIFICATION_GAME_NOT_OVER);
        }
    }
    score->resetNotificationGameOver();
}

void GameLogic::setBonusPlatformEnabled(bool enabled, float transitionTime)
{
    if (eLevelType::MAIN_MENU == Utility::getLevelType(_levelName))
    {
        auto blocks = (this->getChildrenWithTags({gameTags::BLOCK_TAG}));
        Ref* ref = nullptr;
        LHAsset* blockAsset = nullptr;
        Node * blockNode = nullptr;
        LHPhysicsProtocol * blockPhysicsProtocol = nullptr;
        
        CCARRAY_FOREACH(blocks, ref)
        {
            blockPhysicsProtocol = dynamic_cast<LHPhysicsProtocol*>(ref);
            blockNode = dynamic_cast<Node*>(ref);
            
            if (blockPhysicsProtocol && blockNode && blockNode->getParent())
            {
                blockAsset = dynamic_cast<LHAsset*>(blockNode->getParent());
                if (blockAsset && blockAsset->containsTag(gameTags::UNLOCK_PLATFORM_TAG))
                {
                    b2Body * body = blockPhysicsProtocol->getBox2dBody();
                    blockNode->setCascadeOpacityEnabled(true);
                    if (body)
                    {
                        body->SetActive(enabled);
                    }
                    if (enabled)
                    {
                        Utility::fadeInNodeWithTime(blockNode, transitionTime);
                    }
                    else
                    {
                        Utility::fadeOutNodeWithTime(blockNode, transitionTime);
                    }
                }
            }
        }
    }
}

#pragma mark - Sheep Counter Global

void GameLogic::initSheepCounterGlobal()
{
    auto hud = this->getHUDInstance();
    if (hud)
    {
        int world = Utility::getWorldFromLevelName(_levelName);
        hud->initSheepCounterGlobal(world);
    }
}

void GameLogic::updateSheepCounterGlobal(int* worldUnlocked, bool *isGameFinished)
{
    ScoreManager * score = ScoreManager::getInstance();
    int currentLastWorldUnLocked = score->getLastWorldUnlocked();
    bool currentGameFinishedStatus = score->isAllSheepCaught();
    
    int level = Utility::getLevelFromLevelName(_levelName);
    int world = Utility::getWorldFromLevelName(_levelName);
    score->setBestScore(world, level, _initialSheepQuantity - _sheepCounter);
    
    int updatedLastWorldUnlocked = score->getLastWorldUnlocked();
    bool updatedGameFinishedStatus = score->isAllSheepCaught();
    
    if (currentLastWorldUnLocked != updatedLastWorldUnlocked)
    {
        (*worldUnlocked) = updatedLastWorldUnlocked;
    }
    if (currentGameFinishedStatus != updatedGameFinishedStatus)
    {
        (*isGameFinished) = updatedGameFinishedStatus;
    }
}

#pragma mark - Utilities

void GameLogic::relocatePlayer(Point newGameWorldPosition)
{
    Player * player = this->getPlayerInstance();
    if (player)
    {
        Node * gameWorld = this->getChildNodeWithName(sceneChilds::GAME_WORLD);
        Point screenCoord = player->getParent()->convertToWorldSpace(player->getPosition());
        Point gameWorldPosition = gameWorld->convertToNodeSpace(screenCoord);
        
        Point diffGameWorldPosition = newGameWorldPosition - gameWorldPosition;
        player->setPosition(player->getPosition() + diffGameWorldPosition);
    }
}

void GameLogic::relocateElement(LHNodeProtocol *nodeProtocol, Point newPosition)
{
    auto node = LHNodeProtocol::LHGetNode(nodeProtocol);
    if (node && node->getParent())
    {
        Node* gameWorld = this->getChildNodeWithName(sceneChilds::GAME_WORLD);
        auto screenCoord = node->getParent()->convertToWorldSpace(node->getPosition());
        auto gameWorldPosition = gameWorld->convertToNodeSpace(screenCoord);
        
        auto diffGameWorldPosition = newPosition - gameWorldPosition;
        node->setPosition(node->getPosition() + diffGameWorldPosition);
    }
}

Portal * GameLogic::getWorldPortal(int world)
{
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portals = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    Portal * searchedPortal = nullptr;
    if(world != -1)
    {
        CCARRAY_FOREACH(portals, ref)
        {
            portal = dynamic_cast<Portal*>(ref);
            if (portal && portal->getDestinationWorldNumber() == world)
            {
                searchedPortal = portal;
                break;
            }
        }
    }
    return searchedPortal;
}

Portal * GameLogic::getLevelPortal(int level)
{
    const std::vector<std::string> tagValues = {gameTags::PORTAL_TAG};
    __Array* portals = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Portal* portal = nullptr;
    
    Portal * searchedPortal = nullptr;
    if(level != -1)
    {
        CCARRAY_FOREACH(portals, ref)
        {
            portal = dynamic_cast<Portal*>(ref);
            if (portal && portal->getDestinationLevelNumber() == level)
            {
                searchedPortal = portal;
                break;
            }
        }
    }
    return searchedPortal;
}

void GameLogic::regenerateNotes()
{
    const std::vector<std::string> tagValues = {gameTags::NOTE_TAG};
    __Array* noteArray = this->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Note* note = nullptr;
    
    CCARRAY_FOREACH(noteArray, ref)
    {
        note = dynamic_cast<Note*>(ref);
        if (note)
        {
            note->regenerateNote(true);
        }
    }
}

void GameLogic::playSoundSheepCollected()
{
    if (_soundSheepCollected)
    {
        // Questa sezione attiva un delay usato per scandire il suono della pecora che entra
        // nella capanna nel caso che siano due o più ravvicinate.
        if (!_isEnteringSheepCounterActive)
        {
            _isEnteringSheepCounterActive = true;
            auto delay = DelayTime::create(1.f);
            
            auto resetCounterCallback = CallFunc::create([this]()
            {
                _enteringSheepCounter = 0;
                _isEnteringSheepCounterActive = false;
            });
            
            this->runAction(Sequence::create(delay, resetCounterCallback, NULL));
        }
        
        _enteringSheepCounter++;
        
        float interval = _enteringSheepCounter * .1f;
        _soundSheepCollected->playOnceWithDelay(interval);
    }
}

void GameLogic::startInspiration(LHNodeProtocol *nodeProt)
{
    if (nodeProt)
    {
        auto node = dynamic_cast<LHPhysicsProtocol*>(nodeProt);
        if (node)
        {
            node->getBox2dBody()->SetActive(false);
        }
        
        Inspiration::handleStartInspiration(nodeProt);
    }
}

void GameLogic::handleMonsterPosition(Sheep* sheep)
{
    if (sheep->containsTag(gameTags::MONSTER_TAG))
    {
        auto goal = dynamic_cast<LHPhysicsProtocol*>(this->getChildNodeWithName(appParams::GOAL_NODE_NAME));
        if (goal)
        {
            auto distance = goal->getBox2dBody()->GetPosition() - sheep->getBox2dBody()->GetPosition();
            if (distance.x < 0)
            {
                sheep->setIsOnGoalRight(true);
            }
            else
            {
                sheep->setIsOnGoalRight(false);
            }
        }
    }
}

void GameLogic::losePopUpSchedule()
{
    this->scheduleOnce([this](float dt)
    {
        auto lose = LosePopUp::create();
        lose->setCallbackFunc(this, callfuncN_selector(GameLogic::callbackFunc));
       
        this->addChild(lose, static_cast<int>(eSceneZOrders::HUD_LAYER));
    }, 2.f, "lose");
}

#pragma mark - Editor Mode Utilities

#ifdef EDITOR_MODE

__Array* GameLogic::getSheep()
{
    const std::vector<std::string> tagValues = {gameTags::SHEEP_TAG};
    return this->getChildrenWithTags(tagValues);
}

__Array* GameLogic::getGoatskins()
{
    const std::vector<std::string> tagValues = {gameTags::GOATSKIN_TAG};
    return this->getChildrenWithTags(tagValues);
}

void GameLogic::resetForEditor()
{
    this->resetAudio();
    
    if (_soundMusic)
    {
        _soundMusic->stopPlay(true);
    }
    if (_soundAmbience)
    {
        _soundAmbience->stopPlay(true);
    }
}

#endif
