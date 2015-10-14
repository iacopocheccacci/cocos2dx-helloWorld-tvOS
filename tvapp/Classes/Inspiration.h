//
//  Inspiration.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 09/06/15.
//
//

#ifndef __SOP__INSPIRATION__
#define __SOP__INSPIRATION__

#include "cocos2d.h"
#include "LevelHelper2API.h"

namespace inspirationDefs
{
    static const char* NODE_TAG = "ANIMATION";
    static const char* SPRITE_TAG = "INSPIRATION";
    static const char* IDLE_ANIMATION = "idle";
    static const char* START_ANIMATION = "start_move";
    static const char* END_ANIMATION = "end_move";
}

class Inspiration
{
public:
    static void handleStartInspiration(LHNodeProtocol* node);
    static void handleEndInspiration(LHAnimation* animation);
};

#endif // __SOP__INSPIRATION__
