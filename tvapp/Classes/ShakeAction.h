//
//  ShakeAction.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 14/05/15.
//
//

#ifndef __SongOfPan__ShakeAction__
#define __SongOfPan__ShakeAction__

#include "cocos2d.h"

USING_NS_CC;

class ShakeAction : public ActionInterval
{
public:
    ShakeAction();
    
    // Create the action with a time and a strength (same in x and y)
    static ShakeAction* create(float d, float strength );
    // Create the action with a time and strengths (different in x and y)
    static ShakeAction* create(float d, float strength_x, float strength_y );
    bool initWithDuration(float d, float strength_x, float strength_y );
    
    virtual void startWithTarget(Node *pTarget);
    virtual void update(float time);
    virtual void stop(void);
    
protected:
    // Initial position of the shaked node
    float _initial_x, _initial_y;
    // Strength of the action
    float _strength_x, _strength_y;
};

#endif /* defined(__SongOfPan__ShakeAction__) */
