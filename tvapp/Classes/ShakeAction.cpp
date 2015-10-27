//
//  ShakeAction.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 14/05/15.
//
//

#include "ShakeAction.h"

ShakeAction::ShakeAction() : _strength_x(0), _strength_y(0), _initial_x(0), _initial_y(0)
{
}

ShakeAction* ShakeAction::create( float d, float strength )
{
    // call other construction method with twice the same strength
    return create( d, strength, strength );
}

ShakeAction* ShakeAction::create(float duration, float strength_x, float strength_y)
{
    ShakeAction *p_action = new ShakeAction();
    p_action->initWithDuration(duration, strength_x, strength_y);
    p_action->autorelease();
    
    return p_action;
}

bool ShakeAction::initWithDuration(float duration, float strength_x, float strength_y)
{
    if (ActionInterval::initWithDuration(duration))
    {
        _strength_x = strength_x;
        _strength_y = strength_y;
        return true;
    }
    
    return false;
}

// Helper function. I included it here so that you can compile the whole file
// it returns a random value between min and max included
float fgRangeRand( float min, float max )
{
    float rnd = ((float)rand()/(float)RAND_MAX);
    return rnd*(max-min)+min;
}

void ShakeAction::update(float time)
{
    float randx = fgRangeRand( -_strength_x, _strength_x );
    float randy = fgRangeRand( -_strength_y, _strength_y );
    
    // move the target to a shaked position
    _target->setPosition( Point(_initial_x, _initial_y) + Point(randx, randy));
    //_target->setPosition( Point( randx, randy) );
}

void ShakeAction::startWithTarget(Node *pTarget)
{
    ActionInterval::startWithTarget( pTarget );
    
    // save the initial position
    _initial_x = pTarget->getPosition().x;
    _initial_y = pTarget->getPosition().y;
}

void ShakeAction::stop(void)
{
    // Action is done, reset clip position
    _target->setPosition( Point( _initial_x, _initial_y ) );
    
    ActionInterval::stop();
}