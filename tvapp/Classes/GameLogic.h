//
//  GameLogic.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 07/11/14.
//
//

#ifndef __SOP_GAME_LOGIC__
#define __SOP_GAME_LOGIC__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Controller.h"
#include "CollisionManager.h"
#include "Player.h"
#include "GameContext.h"

namespace sceneChilds
{
    static const char* GAME_WORLD = "GAME WORLD";
    static const char* PARALLAX = "parallax";
    static const char* EFFECTS = "effectsManager";
}

enum class eZOrderGameWorld
{
    STARS =     -10,
    RAIN =       50,
};

enum class eGameLogicChild;
enum class ePopUps;
enum class eNotification;
enum class eEndLevelMode;

class Sheep;
class NoteProgressBar;
class Brazier;
class Door;
class Portal;

class SoundComponent;
class CameraManager;
class Tutorial;
class HUD;

class GameLogic : public LHScene
{
public:
    static GameLogic* create(GameContext context);
    
    virtual ~GameLogic();
    void onEnter();
    void onEnterTransitionDidFinish();
    void onExit();
    virtual bool initWithContentOfFile(GameContext context);
    
    // using touch screen
    void touchesEvent(const std::vector<Touch*>& touches, Event* event, bool isTouchBegan);
    void onTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
    void onTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
    void onTouchesEnded(const std::vector<Touch*>& touches, Event  *event);
    void onTouchesCancelled(const std::vector<Touch*>& touches, Event *event);
    
    // using keyboard
    virtual void keyboardEvent(EventKeyboard::KeyCode keyCode, bool pressed);
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* unused_event);
    virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* unused_event);
    
    virtual void didBeginContactBetweenNodes(Node* nodeA, Node* nodeB, Point contactPoint, float impulse);
    virtual void didEndContactBetweenNodes(Node* nodeA, Node* nodeB);
    virtual bool shouldDisableContactBetweenNodes(Node* nodeA, Node* nodeB);
    
    virtual void didFinishedPlayingAnimation(LHAnimation* anim);
    
    eInputType getInputType();
    std::string getLevelName()                  { return _levelName; }
    
    void setSheepDistance(float distance)       { _distanceBetweenSheeps = distance; }
    float getSheepDistance()                    { return _distanceBetweenSheeps; }
    
    void setNoteDuration(float duration)        { _noteDuration = duration; }
    float getNoteDuration()                     { return _noteDuration; }
    
    CameraManager * getCameraManager()          { return _cameraManager; }
    
#ifdef EDITOR_MODE
    Player* getPlayer()                         { return getPlayerInstance(); }
    __Array* getSheep();
    __Array* getGoatskins();
    
    void resetForEditor();
#endif
    
private:
    Player* getPlayerInstance();
    Tutorial* getTutorialInstance();
    HUD* getHUDInstance();
    
    void updateStatus();
    LevelHelperNodeTypeSubclass createNodeObjectForSubclassWithName(const std::string subclassTypeName, const std::string lhTypeName);
    
    void setupInput();
    void setupKeyboardInput();
    void setupSplitScreenInput();
    void setupButtonsInput();
    void updateInput();
    void buttonInputCallback(Ref* sender);
    void setInputEnabled(bool enabled);
    
    void handleSheepInGoal();
    void handleMonsterInGoal(LHNodeProtocol * goalNodeProt);
    void handleMovablePlatformChangeDirection(eCollisionType type, LHNodeProtocol* platform);
    void handlePlayerJumpEnd();
    void handleDefeat();
    void handleSheepInFire(Node* nodeA, Node* nodeB);
    void handleFluteTaken();
    void handleScriptEvent(Node* nodeA, Node* nodeB, bool begin);
    void handleRespawnEvent(Node* nodeA, Node* nodeB);
    void handlePlayerCollisionWithFaith();
    void handleSheepExiting(Node * sheepNode);
    
    void initHUD();
    void initTutorial();
    
    void createNoteEffect(eNoteEffect effect);
    void timerEnded();
    void timerEnded(bool endSound);
    void checkForParticleCollision(float dt);
    void removeNoteEffectFromSheeps();
    
    void initPlayerPosition(GameContext context);
    int initSheepCounter();
    void fillSheepCounter();
    void decrementSheepCounter();
    void notifySheepDecrement();
    bool checkIfVictory();
    void createLoseScreen();
    void initWorldLevelLabel();
    void initSheeps();
    void initWolf();
    void initPortals();
    void initNotes();
    void initFlute();
    void initGoal();
    void updateCustomEvents(float dt);
    void updatePortals();
    void burnPlayer();
    
    void initMonousePlatforms();
    
    void callbackFunc(Node* sender);
    void restartLevel();
    void backToMenu();
    void pauseLevel();
    void pauseLevelWithPopUp(ePopUps popUpType);
    void showNotification(eNotification notificationType);
    void resumeLevel();
    void handleDismissNotification();
    void nextLevel();
    void previousLevel();
    void enterPortalNormal();
    void enterPortalMainMenu();
    void enterPortalWorld(std::string portalCallbackName);
    void enterPortalLevel(std::string portalCallbackName);
    void goToLevel(eEndLevelMode endMode, std::string level);
    void goToLevelSelection(eEndLevelMode endMode, int worldId);
    void goToMainMenu(eEndLevelMode endMode);
    void goToMainMenu(eEndLevelMode endMode, std::string afterFeedbackLevel);
    void transitionToScene(GameContext nextLevelContext);
    void handleEndLevel(eEndLevelMode endMode, GameContext& nextLevelContext);
    
    void initParallax();
    
    void initCameraManager();
    
    void setPlayerOnMovableVelocity(LHNodeProtocol* platform, b2Vec2 velocity);
    void setSheepOnMovableVelocity(LHNodeProtocol* platform, b2Vec2 velocity);
    void setInteractiveOnMovableVelocity(LHNodeProtocol* platform, b2Vec2 velocity);
    
    LHNodeProtocol* getMovablePlatform(Node* nodeA, Node* nodeB);
    bool isMovablePlatform(LHNodeProtocol* block);
    
    LHNodeProtocol* getInspirationNode(Node* nodeA, Node* nodeB);
    LHNodeProtocol* getGoalNode(Node *nodeA, Node *nodeB);
    
    void turnOnGoatskin(Goatskin* goatskin);
    void shakeGoatskin(Goatskin* goatskin);
    Goatskin* getGoatskin(Node* nodeA, Node* nodeB);
    void burnGoal(LHNodeProtocol * goalNodeProt);
    void checkForNotesToMove(Goatskin* goatskin);
    void checkForBrazierToTurnOff(Goatskin* goatskin);
    void safeCheckForSheepInGoal();
    
    void initDoorSwitchConnections();
    void setSwitchObserverIfExist(Door* door, std::string doorName);
    
    void initInteractive();
    void initEoloZones();
    void initBonusWorldPlatform();
    void initCredits();
    
    void initEffects();
    
    void initAnalytics(GameContext & context);
    
    void initAudio();
    void initAudioBraziers();
    void initAudioSheeps();
    void initAudioMusic(int worldNumber, int levelNumber);
    void initAudioAmbience(int worldNumber, int levelNumber);
    void initAudioFlute(int worldNumber);
    void initAudioAllSheepFeedback();
    void initAudioPortalFeedback();
    void initAudioGoalBurn();
    void initAudioFaithJumpBridge();
    
    void startAudio();
    void resetAudio();
    void pauseAudio();
    void resumeAudio();
    
    // Story Video
    void initStoryVideos();
    void handleStoryVideoPlay();
    void handleStoryVideoStop();
    void handleStoryVideoLockedPressed();
    void setPauseStoryVideos(bool paused);
    
    // Update
    void update(float dt);
    void updateAudio(float dt);
    void updateAudioAmbience(float dt);
    void updateAudioMusic(float dt);
    
    void initSheepCounterGlobal();
    void updateSheepCounterGlobal(int* worldUnlocked, bool *isGameFinished);
    
    // Custom Events
    void resolveFeedbackUnlockWorld();
    void resolveFeedbackFluteTaken();
    void resolveNotificationUnlockWorld();
    void resolveNotificationGameOver();
    void setBonusPlatformEnabled(bool enabled, float transitionTime);
    
    // Utilities
    void relocatePlayer(Point newGamintPosition);
    void relocateElement(LHNodeProtocol* node, Point newPosition);
    Portal * getWorldPortal(int world);
    Portal * getLevelPortal(int level);
    
    void regenerateNotes();
    void playSoundSheepCollected();
    
    void startInspiration(LHNodeProtocol* node);
    void handleMonsterPosition(Sheep* sheep);
    
    void losePopUpSchedule();
    
    Controller _inputController;
    int _initialSheepQuantity;
    int _sheepCounter;
    int _enteringSheepCounter;                  // pecore che stanno entrando contemporaneamente nella capanna
    bool _isEnteringSheepCounterActive;
    
    bool _monsterInGoal;
//    eInputType _inputType;
    std::string _levelName;
    float _distanceBetweenSheeps;
    float _noteDuration;
    
    bool _isLose;
    bool _isWin;
    bool _fluteExists;
    bool _fluteTaken;
    
    bool _inputEnabled;
    
    bool _exiting;
    bool _customEventLock;
    
    bool _isCinematicScene;
    
    GameContext _context;
    
    EventListenerTouchAllAtOnce * _splitEventListener;
    
    SoundComponent * _soundAmbience;
    SoundComponent * _soundMusic;
    SoundComponent * _soundSheepCollected;
    SoundComponent * _soundAllSheepFeedback;
    SoundComponent * _soundPortalActive;
    SoundComponent * _soundPortalNotActive;
    SoundComponent * _soundGoalBurn;
    SoundComponent * _soundFall;
    
    ParticleSystemQuad * _foregroundParticle;
    
    CameraManager * _cameraManager;
    
    Player * _player;
    
    std::vector<Sheep *> _sheeps;
};


#endif // __SOP_GAME_LOGIC__
