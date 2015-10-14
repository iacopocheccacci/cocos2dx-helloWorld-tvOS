//
//  GameContext.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 20/04/15.
//
//

#ifndef __SongOfPan__GameContext__
#define __SongOfPan__GameContext__

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class GameContext {
    
public:
    
    GameContext();
    GameContext(std::string levelName, std::string previousLevelName);
    
    void setLevelName(std::string levelName)                           { _levelName = levelName; }
    std::string getLevelName();
    
    void setPreviousLevelName(std::string previousLevelName)           { _previousLevelName = previousLevelName; }
    std::string getPreviousLevelName();
    
    void setNumberOfRestarts(int numberOfRestarts)                     { _numberOfRestarts = numberOfRestarts; }
    int getNumberOfRestarts();
    
    void setAfterFeedbackLevelName(std::string afterFeedbackLevelName) { _afterFeedbackLevelName = afterFeedbackLevelName; }
    std::string getAfterFeedbackLevelName();
    
    void setAfterFeedbackPreviousLevelName(std::string afterFeedbackPreviousLevelName);
    std::string getAfterFeedbackPreviousLevelName();
    
private:
    
    std::string _levelName;
    std::string _previousLevelName;
    std::string _afterFeedbackLevelName;
    std::string _afterFeedbackPreviousLevelName;
    
    int _numberOfRestarts;
    
};

#endif /* defined(__SongOfPan__GameContext__) */
