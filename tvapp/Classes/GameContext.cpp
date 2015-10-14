//
//  GameContext.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 20/04/15.
//
//

#include "GameContext.h"

GameContext::GameContext()
: _levelName("")
, _previousLevelName("")
, _afterFeedbackLevelName("")
, _afterFeedbackPreviousLevelName("")
, _numberOfRestarts(0)
{
    
}

GameContext::GameContext(std::string levelName, std::string previousLevelName)
: _levelName(levelName)
, _previousLevelName(previousLevelName)
, _afterFeedbackLevelName("")
, _afterFeedbackPreviousLevelName("")
, _numberOfRestarts(0)
{
    
}

std::string GameContext::getLevelName()
{
    if (_levelName.empty())
    {
        CCLOG("Attenzione: livello non assegnato");
    }
    return _levelName;
}

std::string GameContext::getPreviousLevelName()
{
    return _previousLevelName;
}

std::string GameContext::getAfterFeedbackLevelName()
{
    return _afterFeedbackLevelName;
}

int GameContext::getNumberOfRestarts()
{
    return _numberOfRestarts;
}

void GameContext::setAfterFeedbackPreviousLevelName(std::string afterFeedbackPreviousLevelName)
{
    _afterFeedbackPreviousLevelName = afterFeedbackPreviousLevelName;
}


std::string GameContext::getAfterFeedbackPreviousLevelName()
{
    return _afterFeedbackPreviousLevelName;
}