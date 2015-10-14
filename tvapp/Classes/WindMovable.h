//
//  WindMovable.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 26/05/15.
//
//

#ifndef SOP_WIND_MOVABLE_H
#define SOP_WIND_MOVABLE_H

//#include "cocos2d.h"
#include "LevelHelper2API.h"

class WindMovable
{
public:
    virtual void setUnderEoloWind(LHNode* node, bool begin) = 0;
    virtual void updateUnderEoloWind(LHNode* node) = 0;
    virtual void exitUnderEoloWind() = 0;
    
protected:
    Point _eoloStartOffset;
};

#endif // SOP_CHARACTER_H
