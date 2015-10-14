//
//  Flute.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 02/03/15.
//
//

#ifndef __SOP__FLUTE__
#define __SOP__FLUTE__

#include "LevelHelper2API.h"
#include "Definitions.h"

namespace fluteDefs
{
    static const char* CLASS_NAME = "Flute";
    static const char* SPRITE = "FlautoSprite";
    static const char* SPRITE_BG = "FlautoBG";
    static const char* PARTICLE_FILE = "particles/particle_flute.plist";
    static const char* PARTICLE_FILE_BG = "particles/particle_flute_bg.plist";
    static const char* PARTICLE_FILE_CATCH = "particles/particle_flute_catch.plist";
    
    static const char* ANIMATION_CATCH_NAME = "note_catch";
}

class AnimationComponent;
class SoundComponent;

class Flute : public LHNode
{
public:
    NODEWITHDICT_FUNC(Flute);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void handleCatch();
    bool getIsCatched()                     { return _isCatched; }
    
private:
    void loadAnimation();
    void initAudio();
    void initParticle();
    void playSound();
    
    AnimationComponent* _animation;
    SoundComponent* _sound;
    bool _isCatched;
};


#endif // __SOP__FLUTE__
