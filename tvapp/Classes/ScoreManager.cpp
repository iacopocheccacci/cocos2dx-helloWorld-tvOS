//
//  ScoreManager.cpp
//  SOP_Proto
//
//  Created by Leonardo Passeri on 13/02/15.
//
//

#include "ScoreManager.h"
#include "Definitions.h"
#include "Utility.h"

namespace ScoreManagerTestCases
{
    // Test per feedback e di notification di apertura portale del mondo (livello 1-7, prendere tutte le pecore),
    // oppure di notification di MANCATO sblocco mondo successivo (livello 1-7, NON prendere tutte le pecore)
    static const ValueMap FEEDBACK_PORTAL = {
        {"world01level01", Value(1)}, // max 1
        {"world01level02", Value(3)}, // max 3
        {"world01level03", Value(3)}, // max 3
        {"world01level04", Value(3)}, // max 3
        {"world01level05", Value(4)}, // max 4
        {"world01level06", Value(4)}, // max 4
        {"world01level07", Value(0)}, // max 5
        {"feedbackWorldUnlock", Value(-1)}, // no feedback = -1
        {"feedbackFluteTaken", Value(-1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(1)}, // default 1
        {"mostOnwardLevelKey", Value(6)}, // default 0
    };
    // Test per feedback di primo pezzo di flauto preso (livello 1-7, prendere il pezzo di flauto)
    static const ValueMap FEEDBACK_FLUTE_ON_LAUNCH = {
        {"world01level01", Value(1)}, // max 1
        {"world01level02", Value(3)}, // max 3
        {"world01level03", Value(3)}, // max 3
        {"world01level04", Value(3)}, // max 3
        {"world01level05", Value(4)}, // max 4
        {"world01level06", Value(4)}, // max 4
        {"world01level07", Value(2)}, // max 5
        {"feedbackWorldUnlock", Value(-1)}, // no feedback = -1
        {"feedbackFluteTaken", Value(1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(1)}, // default 1
        {"mostOnwardLevelKey", Value(7)}, // default 0
        {"fluteTaken_world01level07", Value(true)}, // default false
    };
    // Test per feedback di apertura portale del mondo (2) da lancio applicazione
    static const ValueMap FEEDBACK_PORTAL_OPEN_ON_LAUNCH = {
        {"world01level01", Value(1)}, // max 1
        {"world01level02", Value(3)}, // max 3
        {"world01level03", Value(3)}, // max 3
        {"world01level04", Value(3)}, // max 3
        {"world01level05", Value(4)}, // max 4
        {"world01level06", Value(4)}, // max 4
        {"world01level07", Value(5)}, // max 5
        {"feedbackWorldUnlock", Value(2)}, // no feedback = -1
        {"feedbackFluteTaken", Value(-1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(1)}, // default 1
        {"mostOnwardLevelKey", Value(7)}, // default 0
    };
    // Test per feedback di flauto seguita dal passaggio al livello successivo (prendere il flauto in 3-6)
    static const ValueMap FEEDBACK_FLUTE_WITH_NEXT_LEVEL = {
        {"world01level01", Value(1)}, {"world01level02", Value(3)}, {"world01level03", Value(3)},
        {"world01level04", Value(3)}, {"world01level05", Value(4)}, {"world01level06", Value(4)},
        {"world01level07", Value(5)}, {"world02level01", Value(4)}, {"world02level02", Value(3)},
        {"world02level03", Value(2)}, {"world02level04", Value(4)}, {"world02level05", Value(3)},
        {"world02level06", Value(3)}, {"world02level07", Value(5)}, {"world02level08", Value(5)},
        {"world03level01", Value(5)}, {"world03level02", Value(2)}, {"world03level03", Value(4)},
        {"world03level04", Value(3)}, {"world03level05", Value(4)},
        {"feedbackWorldUnlock", Value(-1)}, // no feedback = -1
        {"feedbackFluteTaken", Value(-1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(3)}, // default 1
        {"mostOnwardLevelKey", Value(5)}, // default 0
    };
    // Test per notifica di terminazione gioco - tutte le pecore prese (prendere la pecora in 5-7)
    static const ValueMap FEEDBACK_GAME_OVER = {
        {"world01level01", Value(1)}, {"world01level02", Value(3)}, {"world01level03", Value(3)},
        {"world01level04", Value(3)}, {"world01level05", Value(4)}, {"world01level06", Value(4)},
        {"world01level07", Value(5)}, {"world02level01", Value(4)}, {"world02level02", Value(3)},
        {"world02level03", Value(2)}, {"world02level04", Value(4)}, {"world02level05", Value(3)},
        {"world02level06", Value(3)}, {"world02level07", Value(5)}, {"world02level08", Value(5)},
        {"world03level01", Value(5)}, {"world03level02", Value(2)}, {"world03level03", Value(4)},
        {"world03level04", Value(3)}, {"world03level05", Value(4)}, {"world03level06", Value(3)},
        {"world03level07", Value(3)}, {"world03level08", Value(4)}, {"world04level01", Value(4)},
        {"world04level02", Value(4)}, {"world04level03", Value(3)}, {"world04level04", Value(3)},
        {"world04level05", Value(3)}, {"world04level06", Value(3)}, {"world04level07", Value(3)},
        {"world05level01", Value(2)}, {"world05level02", Value(2)}, {"world05level03", Value(2)},
        {"world05level04", Value(4)}, {"world05level05", Value(3)}, {"world05level06", Value(3)},
        {"feedbackWorldUnlock", Value(-1)}, // no feedback = -1
        {"feedbackFluteTaken", Value(-1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(5)}, // default 1
        {"mostOnwardLevelKey", Value(6)}, // default 0
    };
    // Test per notifica mancata terminazione gioco - (prendere la pecora in 5-7)
    static const ValueMap FEEDBACK_GAME_NOT_OVER = {
        {"world01level01", Value(1)}, {"world01level02", Value(3)}, {"world01level03", Value(3)},
        {"world01level04", Value(3)}, {"world01level05", Value(4)}, {"world01level06", Value(4)},
        {"world01level07", Value(5)}, {"world02level01", Value(4)}, {"world02level02", Value(3)},
        {"world02level03", Value(2)}, {"world02level04", Value(4)}, {"world02level05", Value(3)},
        {"world02level06", Value(3)}, {"world02level07", Value(5)}, {"world02level08", Value(5)},
        {"world03level01", Value(5)}, {"world03level02", Value(2)}, {"world03level03", Value(4)},
        {"world03level04", Value(3)}, {"world03level05", Value(4)}, {"world03level06", Value(3)},
        {"world03level07", Value(3)}, {"world03level08", Value(4)}, {"world04level01", Value(4)},
        {"world04level02", Value(4)}, {"world04level03", Value(3)}, {"world04level04", Value(3)},
        {"world04level05", Value(3)}, {"world04level06", Value(3)}, {"world04level07", Value(3)},
        {"world05level01", Value(1)}, {"world05level02", Value(2)}, {"world05level03", Value(2)},
        {"world05level04", Value(4)}, {"world05level05", Value(3)}, {"world05level06", Value(3)},
        {"feedbackWorldUnlock", Value(-1)}, // no feedback = -1
        {"feedbackFluteTaken", Value(-1)}, // no feedback = -1
        {"notificationWorldUnlock", Value(-1)}, // no notification = -1
        {"notificationGameOver", Value(false)}, // no notification = false
        {"mostOnwardWorldKey", Value(5)}, // default 1
        {"mostOnwardLevelKey", Value(6)}, // default 0
    };
}

namespace ScoreManagerDefs
{
    static const char* BEST_COMPLETED_LEVEL_WORLD_KEY = "mostOnwardWorldKey";
    static const char* BEST_COMPLETED_LEVEL_KEY = "mostOnwardLevelKey";
    static const char* FLUTE_TAKEN_KEY_PART = "fluteTaken_";
    static const char* FEEDBACK_WORLD_UNLOCK_KEY = "feedbackWorldUnlock";
    static const char* FEEDBACK_FLUTE_TAKEN_KEY = "feedbackFluteTaken";
    static const char* NOTIFICATION_WORLD_UNLOCK_KEY = "notificationWorldUnlock";
    static const char* NOTIFICATION_GAME_OVER_KEY = "notificationGameOver";
    static const int DEFAULT_WORLD_VALUE = 1;
    static const int DEFAULT_LEVEL_VALUE = 0;
    static const int DEFAULT_FEEDBACK_WORLD_UNLOCK = -1;
    static const int DEFAULT_FEEDBACK_FLUTE_TAKEN = -1;
    static const int DEFAULT_NOTIFICATION_WORLD_UNLOCK = -1;
    static const bool DEFAULT_NOTIFICATION_GAME_OVER = false;
};

ScoreManager* ScoreManager::_scoreManager = 0;

ScoreManager::~ScoreManager()
{
    CC_SAFE_DELETE(_scoreManager);
    _scoreManager = NULL;
}

ScoreManager::ScoreManager()
{
    this->initLevels();
    this->initTotalSheepCount();
    this->initLevelsWithFlute();
    
#ifdef DEBUG_UNLOCK_ALL_LEVELS
    this->testUnlockAllLevels();
#endif
    
#ifdef DEBUG_UNLOCK_ALL_FLUTES
    this->testUnlockAllFlutes();
#endif
    // TEST - Decommentare lo specifico test per usarlo ;)
//    this->testScore(ScoreManagerTestCases::FEEDBACK_PORTAL);
//    this->testScore(ScoreManagerTestCases::FEEDBACK_FLUTE_ON_LAUNCH);
//    this->testScore(ScoreManagerTestCases::FEEDBACK_PORTAL_OPEN_ON_LAUNCH);
//    this->testScore(ScoreManagerTestCases::FEEDBACK_FLUTE_WITH_NEXT_LEVEL);
//    this->testScore(ScoreManagerTestCases::FEEDBACK_GAME_OVER);
//    this->testScore(ScoreManagerTestCases::FEEDBACK_GAME_NOT_OVER);
}

ScoreManager* ScoreManager::getInstance()
{
    if (!_scoreManager)
    {
        _scoreManager = new ScoreManager();
    }
    return _scoreManager;
}

#pragma mark - Score

void ScoreManager::setBestScore(int world, int level, int score)
{
    int oldScore = this->getBestScore(world, level);
    if (score > oldScore)
    {
        this->setScore(world, level, score);
        this->notifyScoreUpdate();
    }
}

int ScoreManager::getBestScore(int world, int level)
{
    string levelKey;
    this->getKeyFromLevel(world, level, levelKey);
    return UserDefault::getInstance()->getIntegerForKey(levelKey.c_str(),0);
}

int ScoreManager::getScoreForWorld(int world)
{
    int total = 0;
    for (int level = 1; level <= appParams::MAX_LEVELS_PER_WORLD; level++)
    {
        total += this->getBestScore(world, level);
    }
    return total;
}

int ScoreManager::getTotalScore()
{
    int total = 0;
    for (int world = 1; world <= appParams::NUMBER_OF_WORLDS; world++)
    {
        total += this->getScoreForWorld(world);
    }
    return total;
}

void ScoreManager::resetScoreForAllWorlds()
{
    for (int world = 1; world <= appParams::NUMBER_OF_WORLDS; world++)
    {
        this->resetScoreForWorld(world);
    }
}

void ScoreManager::resetScoreForWorld(int world)
{
    for (int level = 1; level <= appParams::MAX_LEVELS_PER_WORLD; level++)
    {
        this->setScore(world, level, 0);
    }
    this->notifyScoreUpdate();
}

#pragma mark - ScoreManagerObserver

void ScoreManager::addObserver(ScoreManagerObserver * observer)
{
    if (observer)
    {
        _observers.push_back(observer);
    }
    else
    {
        CCLOG("Hai provato ad aggiungere un observer nullo a ScoreManager!!");
    }
}

void ScoreManager::removeObserver(ScoreManagerObserver * observer)
{
    if (observer)
    {
        _observers.erase(std::remove(_observers.begin(), _observers.end(), observer), _observers.end());
    }
    else
    {
        CCLOG("Hai provato ad rimuovere un observer nullo da ScoreManager!!");
    }
}

void ScoreManager::notifyScoreUpdate()
{
    auto observersCopy = _observers;
    for(ScoreManagerObserver * observer : observersCopy)
    {
        observer->onScoreUpdated();
    }
}

#pragma mark - Game Progress

void ScoreManager::saveBestCompletedLevel(std::string level)
{
    int worldId = Utility::getWorldFromLevelName(level);
    int levelId = Utility::getLevelFromLevelName(level);
    
    if (false == this->isLevelWon(level))
    {
        UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_WORLD_KEY, worldId);
        UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_KEY, levelId);
        UserDefault::getInstance()->flush();
    }
}

std::string ScoreManager::loadBestCompletedLevel()
{
    int world = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_WORLD_KEY,ScoreManagerDefs::DEFAULT_WORLD_VALUE);
    int level = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_KEY,ScoreManagerDefs::DEFAULT_LEVEL_VALUE);
    char levelName[50];
    Utility::getLevelName(world, level, levelName);
    return levelName;
}

void ScoreManager::resetBestCompletedLevel()
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_WORLD_KEY, ScoreManagerDefs::DEFAULT_WORLD_VALUE);
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_KEY, ScoreManagerDefs::DEFAULT_LEVEL_VALUE);
    UserDefault::getInstance()->flush();
}

bool ScoreManager::hasPlayerWonFirstLevel()
{
    int world = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_WORLD_KEY,ScoreManagerDefs::DEFAULT_WORLD_VALUE);
    int level = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::BEST_COMPLETED_LEVEL_KEY,ScoreManagerDefs::DEFAULT_LEVEL_VALUE);
    return (ScoreManagerDefs::DEFAULT_WORLD_VALUE != world || ScoreManagerDefs::DEFAULT_LEVEL_VALUE != level);
}

bool ScoreManager::isLevelWon(std::string level)
{
    std::string currentBestCompletedLevel = this->loadBestCompletedLevel();
    
    int worldAId = Utility::getWorldFromLevelName(level);
    int worldBId = Utility::getWorldFromLevelName(currentBestCompletedLevel);
    
    int levelAId = Utility::getLevelFromLevelName(level);
    int levelBId = Utility::getLevelFromLevelName(currentBestCompletedLevel);
    
    if ( ( worldAId > worldBId ) || ( worldAId == worldBId && levelAId > levelBId ) )
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool ScoreManager::isLevelCompleted(std::string level)
{
    bool res = false;
    int worldId = Utility::getWorldFromLevelName(level);
    int levelId = Utility::getLevelFromLevelName(level);
    if (-1 != worldId && this->getBestScore(worldId, levelId) >= this->getLevelSheepCount(worldId, levelId))
    {
        res = true;
    }
    return res;
}

bool ScoreManager::isWorldUnLocked(int world)
{
    bool res = false;
    if (0 < world && world <= appParams::NUMBER_OF_WORLDS)
    {
        res = true;
        // Se manca una pecora nei mondi precedenti, allora il mondo è bloccato
        for (int worldKey = 1; worldKey < world; worldKey++)
        {
            if (_totalSheepsByLevel.end() != _totalSheepsByLevel.find(worldKey))
            {
                for (auto iterLevel = _totalSheepsByLevel[worldKey].begin(); iterLevel != _totalSheepsByLevel[worldKey].end(); iterLevel++)
                {
                    int levelKey = (*iterLevel).first;
                    int totalSheepsForCurrentLevel = (*iterLevel).second;
                    if ( totalSheepsForCurrentLevel != this->getBestScore(worldKey, levelKey) )
                    {
                        res = false;
                    }
                }
            }
        }
    }
    return res;
}

bool ScoreManager::isWorldCompleted(int world)
{
    bool res = false;
    if (0 < world && world <= appParams::NUMBER_OF_WORLDS)
    {
        if (this->getScoreForWorld(world) >= this->getWorldSheepCount(world))
        {
            res = true;
        }
    }
    return res;
}

int ScoreManager::getLastWorldUnlocked()
{
    int lastWorldUnLocked = 1;
    bool found = false;
    while (false == found && lastWorldUnLocked <= appParams::NUMBER_OF_WORLDS)
    {
        if (false == this->isWorldUnLocked(lastWorldUnLocked + 1))
        {
            found = true;;
        }
        else
        {
            lastWorldUnLocked++;
        }
    }
    return fmin(lastWorldUnLocked, appParams::NUMBER_OF_WORLDS);
}

int ScoreManager::getLevelSheepCount(int world, int level)
{
    int sheepCount = 0;
    if (_totalSheepsByLevel.end() != _totalSheepsByLevel.find(world))
    {
        if (_totalSheepsByLevel[world].end() != _totalSheepsByLevel[world].find(level))
        {
            sheepCount = _totalSheepsByLevel[world][level];
        }
    }
    return sheepCount;
}

int ScoreManager::getWorldSheepCount(int world)
{
    int sheepCount = 0;
    if (_totalSheepsByLevel.end() != _totalSheepsByLevel.find(world))
    {
        for (auto iterLevel = _totalSheepsByLevel[world].begin(); iterLevel != _totalSheepsByLevel[world].end(); iterLevel++)
        {
            sheepCount += (*iterLevel).second;
        }
    }
    return sheepCount;
}

int ScoreManager::getTotalSheepCount()
{
    int totalSheeps = 0;
    for (auto iterWorld = _totalSheepsByLevel.begin(); iterWorld != _totalSheepsByLevel.end(); iterWorld++)
    {
        for (auto iterLevel = (*iterWorld).second.begin(); iterLevel != (*iterWorld).second.end(); iterLevel++)
        {
            totalSheeps += (*iterLevel).second;
        }
    }
    return totalSheeps;
}

bool ScoreManager::isAllSheepCaught()
{
    return (this->getTotalSheepCount() == this->getTotalScore());
}

#pragma mark - Flauto

void ScoreManager::saveTakenFluteForLevel(const std::string& levelName)
{
    std::string levelKey;
    int world = Utility::getWorldFromLevelName(levelName);
    int level = Utility::getLevelFromLevelName(levelName);
    this->getKeyFromLevel(world, level, levelKey);
    
    std::string key;
    this->getKeyFluteFromLevel(levelKey, key);
    UserDefault::getInstance()->setBoolForKey(key.c_str(), true);
    CCLOG("Salvato flauto. Key: %s", key.c_str());
    UserDefault::getInstance()->flush();
}

bool ScoreManager::loadTakenFluteForLevel(const std::string& levelName)
{
    std::string levelKey;
    int world = Utility::getWorldFromLevelName(levelName);
    int level = Utility::getLevelFromLevelName(levelName);
    this->getKeyFromLevel(world, level, levelKey);
    
    std::string key;
    this->getKeyFluteFromLevel(levelKey, key);
    bool fluteTaken = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
    return fluteTaken;
}

int ScoreManager::getFluteTakenCount()
{
    std::string levelName;
    std::string key;
    bool fluteTaken;
    int fluteTakenCount = 0;
    for (int level = 1; level <= appParams::MAX_LEVELS_PER_WORLD; level++)
    {
        for (int world = 1; world <= appParams::NUMBER_OF_WORLDS; world++)
        {
            this->getKeyFromLevel(world, level, levelName);
            this->getKeyFluteFromLevel(levelName, key);
            fluteTaken = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
            if (fluteTaken)
            {
                fluteTakenCount++;
            }
        }
    }
    return fluteTakenCount;
}

bool ScoreManager::isWorldFluteTaken(int world)
{
    std::string levelName;
    std::string key;
    bool fluteTaken = false;
    for (int level = 1; level <= appParams::MAX_LEVELS_PER_WORLD; level++)
    {
        this->getKeyFromLevel(world, level, levelName);
        this->getKeyFluteFromLevel(levelName, key);
        if (UserDefault::getInstance()->getBoolForKey(key.c_str(), false))
        {
            fluteTaken = true;
        }
    }
    return fluteTaken;
}

bool ScoreManager::isFluteExistInLevel(std::string levelname)
{
    bool res = false;
    if (std::find(_levelsWithFlute.begin(), _levelsWithFlute.end(), levelname) != _levelsWithFlute.end())
    {
        res = true;
    }
    return res;
}

void ScoreManager::resetTakenFlutes()
{
    std::string levelName;
    std::string key;
    bool fluteTaken;
    for (int level = 1; level <= appParams::MAX_LEVELS_PER_WORLD; level++)
    {
        for (int world = 1; world <= appParams::NUMBER_OF_WORLDS; world++)
        {
            this->getKeyFromLevel(world, level, levelName);
            this->getKeyFluteFromLevel(levelName, key);
            fluteTaken = UserDefault::getInstance()->getBoolForKey(key.c_str(), false);
            if (fluteTaken)
            {
                UserDefault::getInstance()->setBoolForKey(key.c_str(), false);
                CCLOG("Resettato flauto. Key: %s", key.c_str());
            }
        }
    }
    UserDefault::getInstance()->flush();
}

#pragma mark - Feedback registration methods

void ScoreManager::setFeedbackWorldUnlock(int world)
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::FEEDBACK_WORLD_UNLOCK_KEY, world);
    UserDefault::getInstance()->flush();
}

int ScoreManager::getFeedbackWorldUnlock()
{
    int res = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::FEEDBACK_WORLD_UNLOCK_KEY, ScoreManagerDefs::DEFAULT_FEEDBACK_WORLD_UNLOCK);
    return res;
}

void ScoreManager::resetFeedbackWorldUnlock()
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::FEEDBACK_WORLD_UNLOCK_KEY, ScoreManagerDefs::DEFAULT_FEEDBACK_WORLD_UNLOCK);
    UserDefault::getInstance()->flush();
}

void ScoreManager::setFeedbackFluteTaken(int world)
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::FEEDBACK_FLUTE_TAKEN_KEY, world);
    UserDefault::getInstance()->flush();
}

int ScoreManager::getFeedbackFluteTaken()
{
    int res = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::FEEDBACK_FLUTE_TAKEN_KEY, ScoreManagerDefs::DEFAULT_FEEDBACK_FLUTE_TAKEN);
    return res;
}

void ScoreManager::resetFeedbackFluteTaken()
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::FEEDBACK_FLUTE_TAKEN_KEY, ScoreManagerDefs::DEFAULT_FEEDBACK_FLUTE_TAKEN);
    UserDefault::getInstance()->flush();
}

#pragma mark - Notifications registration methods

void ScoreManager::setNotificationUnlockWorld(int world)
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::NOTIFICATION_WORLD_UNLOCK_KEY, world);
    UserDefault::getInstance()->flush();
}

int ScoreManager::getNotificationUnlockWorld()
{
    int res = UserDefault::getInstance()->getIntegerForKey(ScoreManagerDefs::NOTIFICATION_WORLD_UNLOCK_KEY, ScoreManagerDefs::DEFAULT_NOTIFICATION_WORLD_UNLOCK);
    return res;
}

void ScoreManager::resetNotificationUnlockWorld()
{
    UserDefault::getInstance()->setIntegerForKey(ScoreManagerDefs::NOTIFICATION_WORLD_UNLOCK_KEY, ScoreManagerDefs::DEFAULT_NOTIFICATION_WORLD_UNLOCK);
    UserDefault::getInstance()->flush();
}

void ScoreManager::setNotificationGameOver()
{
    UserDefault::getInstance()->setBoolForKey(ScoreManagerDefs::NOTIFICATION_GAME_OVER_KEY, true);
    UserDefault::getInstance()->flush();
}

bool ScoreManager::getNotificationGameOver()
{
    bool res = UserDefault::getInstance()->getBoolForKey(ScoreManagerDefs::NOTIFICATION_GAME_OVER_KEY, ScoreManagerDefs::DEFAULT_NOTIFICATION_GAME_OVER);
    return res;
}

void ScoreManager::resetNotificationGameOver()
{
    UserDefault::getInstance()->setBoolForKey(ScoreManagerDefs::NOTIFICATION_GAME_OVER_KEY, ScoreManagerDefs::DEFAULT_NOTIFICATION_GAME_OVER);
    UserDefault::getInstance()->flush();
}

#pragma mark - Metodi privati

void ScoreManager::initLevels()
{
    auto levelArray = Utility::loadLevelsFromFile();
    Ref* ref = nullptr;
    
    if (levelArray)
    {
        CCARRAY_FOREACH(levelArray, ref)
        {
            auto levelName = dynamic_cast<__String*>(ref)->getCString();
            _levels.push_back(levelName);
        }
    }
}

void ScoreManager::initTotalSheepCount()
{
    FileUtils * fileUtils = FileUtils::getInstance();
    
    // Rimuovo temporaneamente l'eventuale log di file mancante
    bool notifyMissingFile = fileUtils->isPopupNotify();
    fileUtils->setPopupNotify(false);
    
    for (std::string levelName : _levels)
    {
        if (fileUtils->isFileExist(levelName))
        {
            int world = Utility::getWorldFromLevelName(levelName);
            int level = Utility::getLevelFromLevelName(levelName);
            
            _totalSheepsByLevel[world][level] = 0;
            
            std::string levelStr = fileUtils->getStringFromFile(levelName);
            
            size_t start = 0;
            while ((start = levelStr.find(appParams::SHEEP_ASSET_NAME, start)) != std::string::npos)
            {
                _totalSheepsByLevel[world][level]++;
                start += appParams::SHEEP_ASSET_NAME.length();
            }
        }
    }
    fileUtils->setPopupNotify(notifyMissingFile);
}

void ScoreManager::initLevelsWithFlute()
{
    FileUtils * fileUtils = FileUtils::getInstance();
    
    // Rimuovo temporaneamente l'eventuale log di file mancante
    bool notifyMissingFile = fileUtils->isPopupNotify();
    fileUtils->setPopupNotify(false);
    
    for (std::string levelName : _levels)
    {
        if (fileUtils->isFileExist(levelName))
        {
            std::string levelStr = fileUtils->getStringFromFile(levelName);
            
            if ((levelStr.find(appParams::FLUTE_ASSET_NAME, 0)) != std::string::npos)
            {
                _levelsWithFlute.push_back(levelName);
            }
        }
    }
    fileUtils->setPopupNotify(notifyMissingFile);
}

void ScoreManager::setScore(int world, int level, int score)
{
    string levelKey;
    this->getKeyFromLevel(world, level, levelKey);
    int currentScore = UserDefault::getInstance()->getIntegerForKey(levelKey.c_str(), 0);
    if (currentScore != score)
    {
        UserDefault::getInstance()->setIntegerForKey(levelKey.c_str(), score);
        UserDefault::getInstance()->flush();
    }
}

bool ScoreManager::getKeyFromLevel(int world, int level, std::string& key)
{
    char keyChar[50];
    snprintf(keyChar, 50, appParams::LEVEL_NAME_TEMPLATE, world, level);
    key = keyChar;
    return true;
}

bool ScoreManager::getKeyFluteFromLevel(const std::string& level, std::string& key)
{
    key = ScoreManagerDefs::FLUTE_TAKEN_KEY_PART + level;
    return true;
}

#pragma mark - DEBUG

void ScoreManager::testUnlockAllLevels()
{
    this->resetScoreForAllWorlds();
    
    int world = 0;
    int level = 0;
    int score = 0;
    
    for (auto iterWorld = _totalSheepsByLevel.begin(); iterWorld != _totalSheepsByLevel.end(); iterWorld++)
    {
        world = (*iterWorld).first;
        
        auto totalSheepForWorld = (*iterWorld).second;
        for (auto iterLevel = totalSheepForWorld.begin(); iterLevel != totalSheepForWorld.end(); iterLevel++)
        {
            level = (*iterLevel).first;
            score = (*iterLevel).second;
            
            this->setBestScore(world, level, score);
            
            CCLOG("World %d, Level %d, sheeps %d", world, level, score);
        }
    }
    
    int lastWorld = (int)_totalSheepsByLevel.size();
    int lastLevel = (int)_totalSheepsByLevel[lastWorld].size();
    char lastLevelName[50];
    Utility::getLevelName(lastWorld, lastLevel, lastLevelName);
    
    this->saveBestCompletedLevel(lastLevelName);
}

void ScoreManager::testUnlockAllFlutes()
{
    this->resetTakenFlutes();
    
    this->saveTakenFluteForLevel("world01_lev07.lhplist");
    this->saveTakenFluteForLevel("world02_lev08.lhplist");
    this->saveTakenFluteForLevel("world03_lev07.lhplist");
    this->saveTakenFluteForLevel("world04_lev07.lhplist");
    this->saveTakenFluteForLevel("world05_lev05.lhplist");
}

void ScoreManager::testScore(ValueMap testCase)
{
    this->resetScoreForAllWorlds();
    this->resetTakenFlutes();
    
    for(auto iter = testCase.begin(); iter != testCase.end(); iter++)
    {
        std::string key = (*iter).first;
        Value value = (*iter).second;
        switch (value.getType())
        {
            case Value::Type::INTEGER:
                UserDefault::getInstance()->setIntegerForKey(key.c_str(), value.asInt());
                break;
            case Value::Type::BOOLEAN:
                UserDefault::getInstance()->setBoolForKey(key.c_str(), value.asBool());
                break;
            default:
                break;
        }
    }
    UserDefault::getInstance()->flush();
}
