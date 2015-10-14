//
//  Drop.h
//  SongOfPan
//
//  Created by Iacopo Checcacci on 22/06/15.
//
//

#ifndef __SOP__DROP__
#define __SOP__DROP__

#include "LevelHelper2API.h"
#include "Definitions.h"

class AnimationComponent;

namespace dropDefs
{
    static const char* CLASS_NAME = "Drop";
}

class Drop : public LHSprite
{
public:
    NODEWITHDICT_FUNC(Drop);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
private:
    void loadAnimation();
    AnimationComponent* _animation;
};

#endif // __SOP__DROP__
