//
//  InteractiveBlock.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 09/12/14.
//
//

#ifndef __SOP_INTERACTIVE_BLOCK__
#define __SOP_INTERACTIVE_BLOCK__

#include "cocos2d.h"
#include "LevelHelper2API.h"
#include "Definitions.h"
#include "Observer.h"
#include "WindMovable.h"

USING_NS_CC;

namespace interactiveBlockDefs
{
    static const char* CLASS_NAME = "InteractiveBlock";
}

class Player;
class SoundComponent;

class InteractiveBlock : public LHSprite, public WindMovable
{
public:
    NODEWITHDICT_FUNC(InteractiveBlock);
    virtual bool initWithDictionary(LHDictionary* dict, Node* prnt);
    
    void setIsOnMovablePlatform(LHNodeProtocol* platform, bool begin);
    LHNodeProtocol* getMovablePlatform()                                               { return _movablePlatform; }
    void setPlayer(Player* player)                                                     { _player = player; }
    
    void addObserver(Observer* observer);
    void removeObserver(Observer* observer);
    void setUnderEoloWind(LHNode* node, bool begin);
    void updateUnderEoloWind(LHNode* node);
    void exitUnderEoloWind();
    void playFallingSound();
    
private:
    void update(float dt);
    void updateDistanceFromPlayer();
    void updateFalling();
    
    bool checkIfBrazier();
    
    void notify(eEventType event);
    
    void startWindAnimation();
    void stopWindAnimation();
    
    LHNodeProtocol* _movablePlatform;  // Se diversa da NULL blocco su una piattaforma mobile
    Player* _player;
    
    bool _isFalling;
    bool _isBrazier;
    
    std::vector<Observer*> _observers;
    SoundComponent* _soundFall;
    
    Node * _windRootNode;
};

#endif // __SOP_INTERACTIVE_BLOCK__
