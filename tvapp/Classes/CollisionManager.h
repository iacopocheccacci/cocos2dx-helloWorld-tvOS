//
//  CollisionManager.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 04/12/14.
//
//

#ifndef __SOP_COLLISION_MANAGER__
#define __SOP_COLLISION_MANAGER__

#include "cocos2d.h"
#include "LevelHelper2API.h"

USING_NS_CC;

class Player;
class Sheep;
class Brazier;
class Goatskin;

enum class eGroundType;

enum class eCollisionType : int
{
    NO_COLLISION = 0,
    PLAYER_NOTE_COLLISION,
    PLAYER_INVERSE_NOTE_COLLISION,
    PLAYER_GROUND_COLLISION_BEGIN,
    PLAYER_GROUND_COLLISION_END,
    PLAYER_SIDE_COLLISION_BEGIN,
    PLAYER_SIDE_COLLISION_END,
    PLAYER_BLOCK_COLLISION_BEGIN,
    PLAYER_BLOCK_COLLISION_END,
    PLAYER_SHEEP_COLLISION_BEGIN,
    PLAYER_SHEEP_COLLISION_END,
    PLAYER_SWITCH_COLLISION_BEGIN,
    PLAYER_SWITCH_COLLISION_END,
    PLAYER_LOSE_COLLISION,
    PLAYER_BRAZIER_COLLISION,
    PLAYER_FIRE_COLLISION,
    PLAYER_PORTAL_COLLISION,
    PLAYER_WOLF_COLLISION_BEGIN,
    PLAYER_WOLF_COLLISION_END,
    PLAYER_FLUTE_COLLISION,
    PLAYER_SCRIPT_EVENT_COLLISION_BEGIN,
    PLAYER_SCRIPT_EVENT_COLLISION_END,
    PLAYER_INSPIRATION_COLLISION,
    PLAYER_FAITH_COLLISION,
    SHEEP_GROUND_COLLISION_BEGIN,
    SHEEP_GROUND_COLLISION_END,
    SHEEP_SIDE_COLLISION_BEGIN,
    SHEEP_SIDE_COLLISION_END,
    SHEEP_OBSTACLE_COLLISION_BEGIN,
    SHEEP_OBSTACLE_COLLISION_END,
    SHEEP_FIRE_COLLISION,
    SHEEP_GOAL_COLLISION,
    SHEEP_WOLF_COLLISION_BEGIN,
    SHEEP_WOLF_COLLISION_END,
    SHEEP_LOSE_COLLISION,
    MONSTER_GOAL_COLLISION,
    MOVABLE_LEFT_COLLISION,
    MOVABLE_RIGHT_COLLISION,
    MOVABLE_UP_COLLISION,
    MOVABLE_DOWN_COLLISION,
    MOVABLE_INTERACTIVE_COLLISION,
    BLOCK_BRAZIER_COLLISION,
    IDLE_GOATSKIN_COLLISION,
    OFF_GOATSKIN_COLLISION,
    SWITCH_COLLISION,
    BRAZIER_WOLF_COLLISION_BEGIN,
    BRAZIER_WOLF_COLLISION_END,
    MONOUSE_INTERACTIVE_COLLISION,
    EOLO_AREA_COLLISION,
    INTERACTIVE_BLOCK_COLLISION,
    INTERACTIVE_LOSE_COLLISION,
};

class CollisionManager
{
public:
    eCollisionType handleCollisionBegin(Node* nodeA, Node* nodeB, float impulse);
    eCollisionType handleCollisionEnd(Node* nodeA, Node* nodeB);
    
    bool handleDisableCollision(Node* nodeA, Node* nodeB);
    
private:
    eCollisionType checkForPlayerCollisionBegin(Node* nodeA, Node* nodeB, float impulse);
    eCollisionType checkForPlayerCollisionEnd(Node* nodeA, Node* nodeB);
    eCollisionType checkForSheepCollisionBegin(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, float impulse);
    eCollisionType checkForSheepCollisionEnd(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    eCollisionType checkForMovableCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin);
    eCollisionType checkForBrazierCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin);
    eCollisionType checkForGoatskinCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin, float impulse);
    eCollisionType checkForSwitchCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin);
    eCollisionType checkForMonousePlatformCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    eCollisionType checkForEoloZoneCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin);
    eCollisionType checkForInteractiveBlockCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    
    bool checkForPlayerCollisionToDisable(Node* nodeA, Node* nodeB);
    
    template<typename T>
    void handleCharacterCollisionWithInteractive(T* character, LHNodeProtocol* block, bool begin);
    
    // player
    eCollisionType handlePlayerCollisionWithNode(Node* nodeA, Node* nodeB, float impulse);
    eCollisionType handleEndPlayerCollisionWithNode(Node* nodeB, Node* nodeA);

    eCollisionType handlePlayerCollisionWithNote(Player* player, LHNodeProtocol* node);
    eCollisionType handlePlayerCollisionWithBound(Player* player, LHNodeProtocol* node, bool begin, float impulse);
    eCollisionType handlePlayerCollisionWithBlock(Player *player, LHNodeProtocol* node, bool begin, float impulse);
    eCollisionType handlePlayerCollisionWithSheep(Player* player, LHNodeProtocol* node, bool begin, float impulse);
    eCollisionType handlePlayerCollisionWithPortal(LHNodeProtocol* node, bool begin);
    eCollisionType handlePlayerCollisionWithWolf(Player* player, LHNodeProtocol* node, bool begin);
    eCollisionType handlePlayerCollisionWithFlute(Player* player, LHNodeProtocol* node);
    eCollisionType handlePlayerCollisionWithFire(LHNodeProtocol* node);
    eCollisionType handlePlayerCollisionWithScriptEvent(LHNodeProtocol* node, bool begin);
    eCollisionType handlePlayerCollisionWithInspiration(Player* player, LHNodeProtocol* node);
        
    bool shouldDisablePlayerCollisionWithNode(Node* player, Node* other);
    bool shouldDisablePlayerCollisionWithSheep(LHNodeProtocol* player, LHNodeProtocol* sheep);
    bool shouldDisablePlayerCollisionWithPlatform(LHNodeProtocol* player, LHNodeProtocol* platform);
    bool shouldDisablePlayerCollisionWithInteractive(LHNodeProtocol* player, LHNodeProtocol* interactive);
    
    // sheep
    eCollisionType handleSheepCollisionWithNode(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, float impulse);
    eCollisionType handleEndSheepCollisionWithNode(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    
    eCollisionType handleSheepCollisionWithObstacle(LHNodeProtocol* sheep, LHNodeProtocol* shape, bool begin, float impulse);
    eCollisionType handleSheepCollisionWithWolf(LHNodeProtocol* sheep, LHNodeProtocol* node, bool begin);
    eCollisionType handleSheepCollisionWithFire(Sheep* sheep, LHNodeProtocol* brazier);
    
    bool shouldDisableSheepCollisionWithNode(LHNodeProtocol* sheep, LHNodeProtocol* other);
    bool shouldDisableSheepCollisionWithPlatform(Sheep* sheep, LHNodeProtocol* shape);
    
    // movable platform
    eCollisionType handleMovablePlatformChangeDirection(LHNodeProtocol* platform, LHNodeProtocol* direction);
    eCollisionType handleMovablePlatformInteractiveBlockCollision(LHNodeProtocol* platform, LHNodeProtocol* block, bool begin);
    eCollisionType getMovablePlatformVelocityChange(LHNodeProtocol* direction, b2Vec2 &vel);
    
    // monouse platform
    eCollisionType handleCollisionWithMonousePlatform(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    void handleMonousePlatform(LHNodeProtocol* node);
    
    // brazier
    eCollisionType handleBrazierCollisionWithBlock(Brazier* brazier, LHNodeProtocol* block);
    eCollisionType handleBrazierCollisionWithWolf(Brazier* brazier, LHNodeProtocol* node, bool begin);
    
    // goatskin
    eCollisionType handleGoatskinCollisionWithBlock(LHNodeProtocol* goatskin, LHNodeProtocol* block, bool begin, float impulse);
    eCollisionType handleGoatskinCollisionWithCharacter(LHNodeProtocol* goatskin, bool begin, float impulse);
    eCollisionType activateGoatskin(Goatskin* goatskin, bool begin, float impulse);
    
    // door switch
    eCollisionType handleCollisionWithSwitch(LHNodeProtocol* doorSwitch, LHNodeProtocol* node, bool begin);
    
    // interactive and oneside
    eCollisionType handleInteractiveCollisionWithBlock(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    eCollisionType handleInteractiveCollisionWithLose(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    bool shouldDisableInteractiveCollisionWithOneside(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB);
    
    // Eolo
    eCollisionType handleCollisionWithEoloArea(LHNodeProtocol* node, LHNodeProtocol* eoloZone, bool begin);
};

#endif // __SOP_COLLISION_MANAGER__
