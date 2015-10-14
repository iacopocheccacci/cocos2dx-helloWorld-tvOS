//
//  SheepFlyingState.cpp
//  SongOfPan
//
//  Created by Iacopo Checcacci on 28/05/15.
//
//

#include "Sheep.h"
#include "SheepFlyingState.h"
#include "SheepFallingState.h"
#include "AnimationComponent.h"
#include "SoundComponent.h"

SheepFlyingState::SheepFlyingState(Sheep* sheep, eAction input)
{
    //TODO: add animation & sound
    //    _animationFly = sheep->getAnimationFlying();
    //    _soundFly = sheep->getSoundFlying();
    
    _actionLoop = nullptr;
    
    Node * sprite = sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME);
    if (sprite)
    {
        _originalSpritePosition = sprite->getPosition();
    }
    
    _animationFly = sheep->getAnimationFly();
    
    this->setGraphics(sheep, input);
}

SheepFlyingState::~SheepFlyingState()
{
}

SheepState* SheepFlyingState::handleInput(Sheep *sheep, eAction input)
{
    SheepState* newState = nullptr;
    
    switch (input)
    {
        case eAction::STOP_FLY:
            newState = this->handleStopFlyInput(sheep);
            break;
            
        default:
            break;
    }
    
    return newState;
    
}

void SheepFlyingState::update(Sheep *sheep, float dt)
{
    // do nothing
}

void SheepFlyingState::setGraphics(Sheep* sheep, eAction input)
{
    auto sprite = dynamic_cast<LHSprite*>(sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME));
    
    if (sprite)
    {
        switch (input)
        {
            case eAction::FLY:
                
                this->startAnimation(sheep);
//TODO: add animation & sound
//                if (_soundFly)
//                {
//                    _soundFly->playOnce();
//                }
                break;
                
            default:
                break;
        }
    }
}

SheepState* SheepFlyingState::handleStopFlyInput(Sheep *sheep)
{
    this->stopAnimation(sheep);
    
    return new SheepFallingState(sheep, eAction::FALL);
}

void SheepFlyingState::startAnimation(Sheep* sheep)
{
    static const float horizontalDistance = 80.0f;
    static const int animationFrames = 8;
    static const float easeRate = 2.0f;
    
//    static const float minHorizontalDistance = 40.0f;
//    static const float maxHorizontalDistance = 100.0f;
//    float randomFactor = CCRANDOM_0_1();
//    float horizontalDistance = randomFactor * ( maxHorizontalDistance - minHorizontalDistance ) + minHorizontalDistance;
    
    if (_animationFly)
    {
        _animationFly->startAnimation(animationFrames, true);
    }
    
    Node * sprite = sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME);
    if (sprite)
    {
        ActionInterval * start;
        ActionInterval * middle;
        ActionInterval * end;
        
        // Oscillazione orizzontale per tornado che cresce
        start = EaseOut::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(horizontalDistance / 4.0f, sprite->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point( - horizontalDistance / 2.0f, sprite->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(0.0f, sprite->getPositionY())), easeRate);
        Sequence * bounceStart = Sequence::create(start, middle, end, NULL);
        
        // Oscillazione orizzontale per tornado che loopa
        start = EaseOut::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(horizontalDistance, sprite->getPositionY())), easeRate);
        middle = EaseInOut::create(MoveTo::create(4.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point( - horizontalDistance, sprite->getPositionY())), easeRate);
        end = EaseIn::create(MoveTo::create(2.0f * animationComponentDefs::TIME_BETWEEN_FRAMES, Point(0.0f, sprite->getPositionY())), easeRate);
        Sequence * bounceLoop = Sequence::create(start, middle, end, NULL);
        
        // Uso un Repeat perché il RepeatForever non funziona all'interno di una Sequence
        _actionLoop = Sequence::create(bounceStart, Repeat::create(bounceLoop, 10), NULL);
        sprite->runAction(_actionLoop);
    }
}

void SheepFlyingState::stopAnimation(Sheep* sheep)
{
    if (_animationFly)
    {
        _animationFly->stopAnimation();
    }
    
    Node * sprite = sheep->getChildNodeWithName(sheepDefs::SPRITE_NAME);
    if (sprite)
    {
        if (_actionLoop)
        {
            sprite->stopAction(_actionLoop);
        }
        sprite->setPosition(_originalSpritePosition);
    }
}