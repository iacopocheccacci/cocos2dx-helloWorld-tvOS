//
//  ScoreManager.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 13/02/15.
//
//

#ifndef __SOP_Proto__ScoreManager__
#define __SOP_Proto__ScoreManager__

#include "cocos2d.h"

USING_NS_CC;

using namespace std;

class ScoreManagerObserver
{
public:    
    virtual void onScoreUpdated() {};
    
};

class ScoreManager
{
public:
    
    virtual ~ScoreManager();
    static ScoreManager* getInstance();
    
    // Score
    int     getBestScore(int world, int level);
    void    setBestScore(int world, int level, int score);
    int     getScoreForWorld(int world);
    int     getTotalScore();
    
    void resetScoreForAllWorlds();
    void resetScoreForWorld(int world);
    
    void addObserver(ScoreManagerObserver * observer);
    void removeObserver(ScoreManagerObserver * observer);
    
    // Game Progress
    void saveBestCompletedLevel(std::string level);
    std::string loadBestCompletedLevel();
    void resetBestCompletedLevel();
    bool hasPlayerWonFirstLevel();
    bool isLevelWon(std::string level);
    bool isLevelCompleted(std::string level);
    bool isWorldUnLocked(int world);
    bool isWorldCompleted(int world);
    int getLastWorldUnlocked();
    int getLevelSheepCount(int world, int level);
    int getWorldSheepCount(int world);
    int getTotalSheepCount();
    bool isAllSheepCaught();
    
    // Flauto
    void saveTakenFluteForLevel(const std::string& levelName);
    bool loadTakenFluteForLevel(const std::string& levelName);
    int getFluteTakenCount();
    bool isWorldFluteTaken(int world);
    bool isFluteExistInLevel(std::string levelname);
    void resetTakenFlutes();
    
    // Feedbacks
    void setFeedbackWorldUnlock(int world);
    int getFeedbackWorldUnlock();
    void resetFeedbackWorldUnlock();
    void setFeedbackFluteTaken(int world);
    int getFeedbackFluteTaken();
    void resetFeedbackFluteTaken();
    
    // Notifications
    void setNotificationUnlockWorld(int world);
    int getNotificationUnlockWorld();
    void resetNotificationUnlockWorld();
    void setNotificationGameOver();
    bool getNotificationGameOver();
    void resetNotificationGameOver();
    
private:
    
    ScoreManager();
    static ScoreManager* _scoreManager;
    
    vector<ScoreManagerObserver *> _observers;
    
    std::unordered_map<int, std::unordered_map<int, int>> _totalSheepsByLevel;
    std::vector<std::string> _levels;
    std::vector<std::string> _levelsWithFlute;
    
    // Init
    void initLevels();
    void initTotalSheepCount();
    void initLevelsWithFlute();
    
    void setScore(int world, int level, int score);
    bool getKeyFromLevel(int world, int level, std::string& key);
    bool getKeyFluteFromLevel(const std::string& level, std::string& key);
    bool isLevelExist(const std::string& level);
    void notifyScoreUpdate();
    
    // DEBUG
    void testUnlockAllLevels();
    void testUnlockAllFlutes();
    void testScore(ValueMap testCase);
};

#endif /* defined(__SOP_Proto__ScoreManager__) */
