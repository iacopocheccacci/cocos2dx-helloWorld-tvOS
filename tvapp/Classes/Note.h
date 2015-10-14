//
//  Note.h
//  SOP_Proto
//
//  Created by Giuseppe Lapenta on 18/11/14.
//
//

#ifndef __SOP_NOTE__
#define __SOP_NOTE__

#include <LevelHelper2API.h>
#include "Definitions.h"

class Player;
class Goatskin;

namespace noteDefs
{
    static const char* CLASS_NAME = "Note";
}

static const int CURRENT_ACTION = 100;

class Note : public LHNode
{
public:
    NODEWITHDICT_FUNC(Note);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void moveIfInWindRange(Goatskin* source);
    virtual void startEffect(Player* player) {}
    
    void handleNotePicked();
    void regenerateNote(bool manual);
    
    void setRegenerationTime(float time)                    { _regenerationTime = time; }
    
protected:
    void moveByWind(Point destination);
    
private:
    static const bool NOTE_REGENERATION;
    float _regenerationTime;
    
};

#endif // __SOP_NOTE__
