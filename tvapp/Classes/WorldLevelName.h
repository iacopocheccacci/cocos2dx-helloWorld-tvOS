//
//  WorldLevelName.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/02/15.
//
//

#ifndef __SOP_Proto__WorldLevelName__
#define __SOP_Proto__WorldLevelName__

#include "cocos2d.h"

USING_NS_CC;

class WorldLevelName : public Layer
{
public:
    virtual ~WorldLevelName();
    CREATE_FUNC(WorldLevelName);
    bool init();
    
    void initLabel(std::string levelName);
};


#endif /* defined(__SOP_Proto__WorldLevelName__) */
