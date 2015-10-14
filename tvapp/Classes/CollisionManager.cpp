//
//  CollisionManager.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 04/12/14.
//
//

#include "CollisionManager.h"
#include "Definitions.h"
#include "Player.h"
#include "Sheep.h"
#include "Note.h"
#include "MonousePlatform.h"
#include "Brazier.h"
#include "Goatskin.h"
#include "Switch.h"
#include "Door.h"
#include "Wolf.h"
#include "Portal.h"
#include "Flute.h"
#include "EoloZone.h"
#include "Utility.h"
#include "FaithJumpBrick.h"

#pragma mark PLAYER SECTION
eCollisionType CollisionManager::handleCollisionBegin(Node *nodeA, Node *nodeB, float impulse)
{
    eCollisionType type = eCollisionType::NO_COLLISION;
    LHNodeProtocol* nodeProtA = dynamic_cast<LHNodeProtocol*>(nodeA);
    LHNodeProtocol* nodeProtB = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    if (nodeProtA && nodeProtB)
    {
        type = checkForPlayerCollisionBegin(nodeA, nodeB, impulse);
        
        if (type == eCollisionType::NO_COLLISION)
        {
            type = checkForSheepCollisionBegin(nodeProtA, nodeProtB, impulse);
        
            if (type == eCollisionType::NO_COLLISION)
            {
                type = checkForMovableCollision(nodeProtA, nodeProtB, true);
            
                if (type == eCollisionType::NO_COLLISION)
                {
                    type = checkForBrazierCollision(nodeProtA, nodeProtB, true);
                
                    if (type == eCollisionType::NO_COLLISION)
                    {
                        type = checkForGoatskinCollision(nodeProtA, nodeProtB, true, impulse);
                    
                        if (type == eCollisionType::NO_COLLISION)
                        {
                            type = checkForSwitchCollision(nodeProtA, nodeProtB, true);
                            
                            if (type == eCollisionType::NO_COLLISION)
                            {
                                type = checkForMonousePlatformCollision(nodeProtA, nodeProtB);
                                
                                if (type == eCollisionType::NO_COLLISION)
                                {
                                    type = checkForEoloZoneCollision(nodeProtA, nodeProtB, true);
                                    
                                    if (type == eCollisionType::NO_COLLISION)
                                    {
                                        type = checkForInteractiveBlockCollision(nodeProtA, nodeProtB);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleCollisionEnd(Node *nodeA, Node *nodeB)
{
    eCollisionType type;
    auto nodeProtA = dynamic_cast<LHNodeProtocol*>(nodeA);
    auto nodeProtB = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    type = checkForPlayerCollisionEnd(nodeA, nodeB);
    
    if (nodeProtA && nodeProtB)
    {
        if (type == eCollisionType::NO_COLLISION)
        {
            type = checkForSheepCollisionEnd(nodeProtA, nodeProtB);
        
            if (type == eCollisionType::NO_COLLISION)
            {
                type = checkForGoatskinCollision(nodeProtA, nodeProtB, false, 0.0f/*impulse*/);
                
                if (type == eCollisionType::NO_COLLISION)
                {
                    type = checkForSwitchCollision(nodeProtA, nodeProtB, false);
                    
                    if (type == eCollisionType::NO_COLLISION)
                    {
                        type = checkForMovableCollision(nodeProtA, nodeProtB, false);
                        
                        if (type == eCollisionType::NO_COLLISION)
                        {
                            type = checkForBrazierCollision(nodeProtA, nodeProtB, false);
                            
                            if (type == eCollisionType::NO_COLLISION)
                            {
                                type = checkForEoloZoneCollision(nodeProtA, nodeProtB, false);
                            }
                        }
                    }
                }
            }
        }
    }

    return type;
}

bool CollisionManager::handleDisableCollision(Node *nodeA, Node *nodeB)
{
    LHNodeProtocol* nodeProtA = dynamic_cast<LHNodeProtocol*>(nodeA);
    LHNodeProtocol* nodeProtB = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    if (nodeProtA && nodeProtB)
    {
        // player contact
        {
            if(nodeA->getName() == playerDefs::PLAYER_NAME)
            {
                return this->shouldDisablePlayerCollisionWithNode(nodeA, nodeB);
            }
            
            if(nodeB->getName() == playerDefs::PLAYER_NAME)
            {
                return this->shouldDisablePlayerCollisionWithNode(nodeB, nodeA);
            }
        }
        
        // sheep contact
        {
            if (nodeProtA->containsTag(gameTags::SHEEP_TAG) && !nodeProtB->containsTag(gameTags::SHEEP_TAG))
            {
                return this->shouldDisableSheepCollisionWithNode(nodeProtA, nodeProtB);
            }
            else if (nodeProtB->containsTag(gameTags::SHEEP_TAG) && !nodeProtA->containsTag(gameTags::SHEEP_TAG))
            {
                return this->shouldDisableSheepCollisionWithNode(nodeProtB, nodeProtA);
            }
        }
        
        // contact between platforms and direction handler nodes
        {
            if ((nodeProtA->containsTag(gameTags::BLOCK_TAG) && nodeProtB->containsTag(gameTags::MOVE_TAG)) ||
                (nodeProtB->containsTag(gameTags::BLOCK_TAG) && nodeProtA->containsTag(gameTags::MOVE_TAG)))
            {
                return true;
            }
        }
        
        // contact between interactive and oneside
        {
            if (nodeProtA->containsTag(gameTags::INTERACTIVE_TAG) && nodeProtB->containsTag(gameTags::ONESIDE_TAG))
            {
                return this->shouldDisableInteractiveCollisionWithOneside(nodeProtA, nodeProtB);
            }
            else if (nodeProtB->containsTag(gameTags::INTERACTIVE_TAG) && nodeProtA->containsTag(gameTags::ONESIDE_TAG))
            {
                return this->shouldDisableInteractiveCollisionWithOneside(nodeProtB, nodeProtA);
            }
        }
    }
    
    return false;
}

eCollisionType CollisionManager::checkForPlayerCollisionBegin(Node *nodeA, Node *nodeB, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->getName() == playerDefs::PLAYER_NAME)
    {
        type = handlePlayerCollisionWithNode(nodeA, nodeB, impulse);
    }
    else if (nodeB->getName() == playerDefs::PLAYER_NAME)
    {
        type = handlePlayerCollisionWithNode(nodeB, nodeA, impulse);
    }
    
    return type;
}

eCollisionType CollisionManager::checkForPlayerCollisionEnd(Node *nodeA, Node *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if(nodeA->getName() == playerDefs::PLAYER_NAME)
    {
        type = handleEndPlayerCollisionWithNode(nodeA, nodeB);
    }
    else if(nodeB->getName() == playerDefs::PLAYER_NAME)
    {
        type = handleEndPlayerCollisionWithNode(nodeB, nodeA);
    }
    
    return type;
}

eCollisionType CollisionManager::checkForSheepCollisionBegin(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::SHEEP_TAG) && !nodeB->containsTag(gameTags::SHEEP_TAG))
    {
        type = handleSheepCollisionWithNode(nodeA, nodeB, impulse);
    }
    else if (nodeB->containsTag(gameTags::SHEEP_TAG) && !nodeA->containsTag(gameTags::SHEEP_TAG))
    {
        type = handleSheepCollisionWithNode(nodeB, nodeA, impulse);
    }
    
    return type;
}

eCollisionType CollisionManager::checkForSheepCollisionEnd(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::SHEEP_TAG) && !nodeB->containsTag(gameTags::SHEEP_TAG))
    {
        type = handleEndSheepCollisionWithNode(nodeA, nodeB);
    }
    else if (nodeB->containsTag(gameTags::SHEEP_TAG) && !nodeA->containsTag(gameTags::SHEEP_TAG))
    {
        type = handleEndSheepCollisionWithNode(nodeB, nodeA);
    }
    
    return type;
}

eCollisionType CollisionManager::checkForMovableCollision(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::BLOCK_TAG) && nodeA->containsTag(gameTags::MOVABLE_TAG))
    {
        if (nodeB->containsTag(gameTags::MOVE_TAG) && begin)
        {
            type = this->handleMovablePlatformChangeDirection(nodeA, nodeB);
        }
        else if (nodeB->containsTag(gameTags::INTERACTIVE_TAG))
        {
            type = this->handleMovablePlatformInteractiveBlockCollision(nodeA, nodeB, begin);
        }
    }
    else if (nodeB->containsTag(gameTags::BLOCK_TAG) && nodeB->containsTag(gameTags::MOVABLE_TAG))
    {
        if (nodeA->containsTag(gameTags::MOVE_TAG))
        {
            type = this->handleMovablePlatformChangeDirection(nodeB, nodeA);
        }
        else if (nodeA->containsTag(gameTags::INTERACTIVE_TAG))
        {
            type = this->handleMovablePlatformInteractiveBlockCollision(nodeB, nodeA, begin);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForBrazierCollision(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::FIRE_TAG))
    {
        LHNode* fire = dynamic_cast<LHNode*>(nodeA);
        Brazier* brazier = dynamic_cast<Brazier*>(fire->getParent());
        if (fire && brazier)
        {
            if (nodeB->containsTag(gameTags::BLOCK_TAG) && !nodeB->containsTag(gameTags::ONESIDE_TAG))
            {
                type = this->handleBrazierCollisionWithBlock(brazier, nodeB);
            }
            else if (nodeB->containsTag(gameTags::WOLF_TAG))
            {
                type = this->handleBrazierCollisionWithWolf(brazier, nodeB, begin);
            }
        }
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::FIRE_TAG))
        {
            LHNode* fire = dynamic_cast<LHNode*>(nodeB);
            Brazier* brazier = dynamic_cast<Brazier*>(fire->getParent());
            if (fire && brazier)
            {
                if (nodeA->containsTag(gameTags::BLOCK_TAG) && !nodeA->containsTag(gameTags::ONESIDE_TAG))
                {
                    type = this->handleBrazierCollisionWithBlock(brazier, nodeA);
                }
                else if (nodeA->containsTag(gameTags::WOLF_TAG))
                {
                    type = this->handleBrazierCollisionWithWolf(brazier, nodeA, begin);
                }
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForGoatskinCollision(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::GOATSKIN_TAG))
    {
        if (nodeB->containsTag(gameTags::BLOCK_TAG))
        {
            type = handleGoatskinCollisionWithBlock(nodeA, nodeB, begin, impulse);
        }
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::GOATSKIN_TAG))
        {
            if (nodeA->containsTag(gameTags::BLOCK_TAG))
            {
                type = handleGoatskinCollisionWithBlock(nodeB, nodeA, begin, impulse);
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForSwitchCollision(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::SWITCH_TAG))
    {
        type = handleCollisionWithSwitch(nodeA, nodeB, begin);
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::SWITCH_TAG))
        {
            type = handleCollisionWithSwitch(nodeB, nodeA, begin);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForMonousePlatformCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::MONOUSE_TAG))
    {
        type = this->handleCollisionWithMonousePlatform(nodeA, nodeB);
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::MONOUSE_TAG))
        {
            type = this->handleCollisionWithMonousePlatform(nodeB, nodeA);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForEoloZoneCollision(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::START_EOLO_TAG))
    {
        type = this->handleCollisionWithEoloArea(nodeB, nodeA, begin);
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::START_EOLO_TAG))
        {
            type = this->handleCollisionWithEoloArea(nodeA, nodeB, begin);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::checkForInteractiveBlockCollision(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (nodeA->containsTag(gameTags::INTERACTIVE_TAG))
    {
        type = this->handleInteractiveCollisionWithLose(nodeA, nodeB);
        
        if (type == eCollisionType::NO_COLLISION)
        {
            type = this->handleInteractiveCollisionWithBlock(nodeA, nodeB);
        }
    }
    
    if (type == eCollisionType::NO_COLLISION)
    {
        if (nodeB->containsTag(gameTags::INTERACTIVE_TAG))
        {
            type = this->handleInteractiveCollisionWithLose(nodeB, nodeA);
            
            if (type == eCollisionType::NO_COLLISION)
            {
                type = this->handleInteractiveCollisionWithBlock(nodeB, nodeA);
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithNode(Node *nodeA, Node *nodeB, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    LHNodeProtocol* otherProt = dynamic_cast<LHNodeProtocol*>(nodeB);
    Player* player = dynamic_cast<Player*>(nodeA);
    
    if(player && otherProt)
    {
        if(otherProt->containsTag(gameTags::NOTE_TAG))
        {
            type = this->handlePlayerCollisionWithNote(player, otherProt);
        }
        else if (otherProt->containsTag(gameTags::BOUND_TAG))
        {
            type = this->handlePlayerCollisionWithBound(player, otherProt, true, impulse);
        }
        else if (otherProt->containsTag(gameTags::BLOCK_TAG))
        {
            type = this->handlePlayerCollisionWithBlock(player, otherProt, true, impulse);
        }
        else if (otherProt->containsTag(gameTags::SHEEP_TAG))
        {
            type = this->handlePlayerCollisionWithSheep(player, otherProt, true, impulse);
        }
        else if (otherProt->containsTag(gameTags::LOSE_TAG))
        {
            type = eCollisionType::PLAYER_LOSE_COLLISION;
        }
        else if (otherProt->containsTag(gameTags::BRAZIER_TAG))
        {
            type = eCollisionType::PLAYER_BRAZIER_COLLISION;
        }
        else if (otherProt->containsTag(gameTags::FIRE_TAG))
        {
            if (!player->getCollisionDisabled())
            {
                type = this->handlePlayerCollisionWithFire(otherProt);
            }
        }
        else if (otherProt->containsTag(gameTags::SWITCH_TAG))
        {
            type = this->handleCollisionWithSwitch(otherProt, player, true);
        }
        else if (otherProt->containsTag(gameTags::PORTAL_TAG))
        {
            type = this->handlePlayerCollisionWithPortal(otherProt, true);
        }
        else if (otherProt->containsTag(gameTags::WOLF_PLAYER_AREA_TAG))
        {
            type = this->handlePlayerCollisionWithWolf(player, otherProt, true);
        }
        else if (otherProt->containsTag(gameTags::FLUTE_TAG))
        {
            type = this->handlePlayerCollisionWithFlute(player, otherProt);
        }
        else if (otherProt->containsTag(gameTags::SCRIPT_EVENT_TAG))
        {
            type = this->handlePlayerCollisionWithScriptEvent(otherProt, true);
        }
        else if (otherProt->containsTag(gameTags::START_EOLO_TAG))
        {
            type = this->handleCollisionWithEoloArea(player, otherProt, true);
        }
        else if (otherProt->containsTag(gameTags::INSPIRATION_TAG))
        {
            type = this->handlePlayerCollisionWithInspiration(player, otherProt);
        }
        else if (otherProt->containsTag(gameTags::BRIDGE_TRESHOLD_TAG))
        {
            type = eCollisionType::PLAYER_FAITH_COLLISION;
        }
    }

    return type;
}

eCollisionType CollisionManager::handleEndPlayerCollisionWithNode(Node *nodeA, Node *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    LHNodeProtocol* otherProt = dynamic_cast<LHNodeProtocol*>(nodeB);
    Player* player = dynamic_cast<Player*>(nodeA);
    
    if(player && otherProt)
    {
        if (otherProt->containsTag(gameTags::BOUND_TAG))
        {
            type = this->handlePlayerCollisionWithBound(player, otherProt, false, 0.0f /*impulse*/);
        }
        else if (otherProt->containsTag(gameTags::BLOCK_TAG))
        {
            type = this->handlePlayerCollisionWithBlock(player, otherProt, false, 0.0f /*impulse*/);
        }
        else if (otherProt->containsTag(gameTags::SHEEP_TAG))
        {
            type = this->handlePlayerCollisionWithSheep(player, otherProt, false, 0.0f /*impulse*/);
        }
        else if (otherProt->containsTag(gameTags::SWITCH_TAG))
        {
            type = this->handleCollisionWithSwitch(otherProt, player, false);
        }
        else if (otherProt->containsTag(gameTags::PORTAL_TAG))
        {
            type = this->handlePlayerCollisionWithPortal(otherProt, false);
        }
        else if (otherProt->containsTag(gameTags::WOLF_PLAYER_AREA_TAG))
        {
            type = this->handlePlayerCollisionWithWolf(player, otherProt, false);
        }
        else if (otherProt->containsTag(gameTags::SCRIPT_EVENT_TAG))
        {
            type = this->handlePlayerCollisionWithScriptEvent(otherProt, false);
        }
        else if (otherProt->containsTag(gameTags::START_EOLO_TAG))
        {
            type = this->handleCollisionWithEoloArea(player, otherProt, false);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithNote(Player *player, LHNodeProtocol *node)
{
    auto note = dynamic_cast<Note*>(node);
    auto type = eCollisionType::NO_COLLISION;
    
    if (note)
    {
        if (note->containsTag(gameTags::INVERSE_NOTE_TAG))
        {
            type = eCollisionType::PLAYER_INVERSE_NOTE_COLLISION;
        }
        else
        {
            type = eCollisionType::PLAYER_NOTE_COLLISION;
        }
        
        note->handleNotePicked();
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithBound(Player *player, LHNodeProtocol *node, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if (node)
    {
        if (node->containsTag(gameTags::GROUND_TAG))
        {
            this->handlePlayerCollisionWithBlock(player, node, begin, impulse);
            type = begin ? eCollisionType::PLAYER_GROUND_COLLISION_BEGIN : eCollisionType::PLAYER_GROUND_COLLISION_END;
        }
        else if (node->containsTag(gameTags::SIDE_TAG))
        {
            player->setContactToTheSide(begin);
            type = begin ? eCollisionType::PLAYER_SIDE_COLLISION_BEGIN : eCollisionType::PLAYER_SIDE_COLLISION_END;
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithBlock(Player *player, LHNodeProtocol *node, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    LHPhysicsProtocol* nodePhysics = dynamic_cast<LHPhysicsProtocol*>(node);
    
    if (nodePhysics)
    {
        auto blockAsset = dynamic_cast<LHAsset*>(LHNodeProtocol::LHGetNode(node)->getParent());
        
        if (blockAsset)
        {
            type = begin ? eCollisionType::PLAYER_BLOCK_COLLISION_BEGIN : eCollisionType::PLAYER_BLOCK_COLLISION_END;
            
            const float kPixelsPerMeter = PTM_RATIO;
            float heightFactor = ((player->getContentSize().height * fabs(fabs(player->getScaleY()))) / 2) + ((blockAsset->getContentSize().height * fabs(blockAsset->getScaleY())) / 2);
            float widthFactor = ((player->getContentSize().width * fabs(player->getScaleX())) / 2) + ((blockAsset->getContentSize().width * fabs(blockAsset->getScaleX())) / 2);
            heightFactor /= kPixelsPerMeter;
            widthFactor /= kPixelsPerMeter;
            
            b2Vec2 v = (nodePhysics->getBox2dBody()->GetPosition()) - (player->getBox2dBody()->GetPosition());
            
            if (fabs(v.x) > (widthFactor - 0.7f) && v.y < -(heightFactor - 0.08f))
            {
                // caso di Pan sul bordo estremo di un blocco
                player->handleOnBlockBorder(v, node, begin);
                
                if (node->containsTag(gameTags::INTERACTIVE_TAG))
                {
                    this->handleCharacterCollisionWithInteractive(player, node, begin);
                    player->handleCollisionWithInteractiveBlock(node, begin);
                }
                
                if (node->containsTag(gameTags::GOATSKIN_TAG))
                {
                    type = this->handleGoatskinCollisionWithCharacter(node, begin, impulse);
                }
            }
            else if (fabs(v.x) > widthFactor)
            {
                player->handleBlockSideContact(v, node, begin);
            }
            else if (v.y > heightFactor)
            {
                // Gestione contatto del player da sotto il blocco
                if (node->containsTag(gameTags::INTERACTIVE_TAG))
                {
                    player->handleCollisionWithInteractiveBlock(node, begin);
                }
            }
            else if (v.y < -(heightFactor - 0.08f)/* || node->containsTag(gameTags::INTERACTIVE_TAG)*/)
            {
                // Gestione contatto del player da sopra il blocco
                player->handleBlockBelowContact(node, begin);
                
                if (node->containsTag(gameTags::INTERACTIVE_TAG))
                {
                    this->handleCharacterCollisionWithInteractive(player, node, begin);
                    player->handleCollisionWithInteractiveBlock(node, begin);
                }
                
                if (node->containsTag(gameTags::MONOUSE_TAG))
                {
                    this->handleMonousePlatform(node);
                }
                
                if (node->containsTag(gameTags::GOATSKIN_TAG))
                {
                    type = this->handleGoatskinCollisionWithCharacter(node, begin, impulse);
                }
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithSheep(Player *player, LHNodeProtocol *node, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    Sheep* sheep = dynamic_cast<Sheep*>(node);
    
    if (sheep)
    {
        type = begin ? eCollisionType::PLAYER_SHEEP_COLLISION_BEGIN : eCollisionType::PLAYER_SHEEP_COLLISION_END;
        const float kPixelsPerMeter = PTM_RATIO;
        float heightFactor = ((player->getContentSize().height * fabs(player->getScaleY())) / 2) + ((sheep->getContentSize().height * fabs(sheep->getScaleY())) / 2);
        float widthFactor = ((player->getContentSize().width * fabs(player->getScaleX())) / 2) + ((sheep->getContentSize().width * fabs(sheep->getScaleX())) / 2);
        heightFactor /= kPixelsPerMeter;
        widthFactor /= kPixelsPerMeter;
        
        b2Vec2 v = (sheep->getBox2dBody()->GetPosition()) - (player->getBox2dBody()->GetPosition());
        
        if (fabs(v.x) > widthFactor && v.y < -heightFactor)
        {
            // Contact below and side with sheep
            player->setContactBelow(sheep, begin);
            player->littleJumpOnSheep();
            
            float controImpulse = (v.x < 0.0f) ? impulse : -impulse;
            
            sheep->getBox2dBody()->ApplyLinearImpulse(b2Vec2(controImpulse, 0.0f), sheep->getBox2dBody()->GetWorldCenter(), true);
        }
        else if (fabs(v.x) > widthFactor)
        {
            // Contact to the side with sheep
            player->setContactToTheSide(begin);
        }
        else if (v.y > heightFactor)
        {
            // Contact above with sheep
        }
        else if (v.y < -heightFactor)
        {
            // Contact below with sheep
            player->setContactBelow(sheep, begin);
            
            if (begin && player->getLastVelocity().y < - 10.0f)
            {
                sheep->playJumpOnSheep();
            }
            
            auto movable = sheep->getMovablePlatform();
            
            if ( movable != nullptr)
            {
                player->setIsOnMovablePlatform(movable, begin);
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithPortal(LHNodeProtocol *node, bool begin)
{
    auto portal = dynamic_cast<Portal*>(node);
    
    if (portal)
    {
        if (begin && (portal->getState() == ePortalState::OPEN_PORTAL))
        {
            portal->setState(ePortalState::ENTERING_PORTAL);
        }
        else if (!begin && (portal->getState() == ePortalState::ENTERING_PORTAL))
        {
            portal->setState(ePortalState::OPEN_PORTAL);
            portal->setPlayerIn(false);
            portal->stopSoundOpen();
        }
        else if (begin && (portal->getState() == ePortalState::CLOSED_PORTAL))
        {
            portal->setPlayerIn(true);
        }
        else if (!begin)
        {
            portal->setPlayerIn(false);
            portal->stopSoundOpen();
        }
    }
    
    return eCollisionType::PLAYER_PORTAL_COLLISION;
}

eCollisionType CollisionManager::handlePlayerCollisionWithWolf(Player* player, LHNodeProtocol* node, bool begin)
{
    auto playerArea = dynamic_cast<LHNode*>(node);
        
    if (playerArea)
    {
        auto wolf = dynamic_cast<Wolf*>(playerArea->getParent());
        
        if (wolf)
        {
            wolf->updateStatus(player, begin);
        }
    }
    
    return (begin ? eCollisionType::PLAYER_WOLF_COLLISION_BEGIN : eCollisionType::PLAYER_WOLF_COLLISION_END);
}

eCollisionType CollisionManager::handlePlayerCollisionWithFlute(Player *player, LHNodeProtocol *node)
{
    auto flute = dynamic_cast<Flute*>(node);
    auto type = eCollisionType::NO_COLLISION;
    
    if (flute && !flute->getIsCatched())
    {
        // TODO: far fare quello che il flauto dovrà far fare dO_Ob
        player->handleInput(eAction::EXULT);
        flute->handleCatch();
        type = eCollisionType::PLAYER_FLUTE_COLLISION;
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithFire(LHNodeProtocol *nodeProt)
{
    auto type = eCollisionType::NO_COLLISION;
    auto node = dynamic_cast<LHNode*>(nodeProt);
    if (node)
    {
        auto brazier = dynamic_cast<Brazier*>(node->getParent());
        if (brazier && (brazier->getBrazierStatus() == eBrazierStatus::BRAZIER_ON))
        {
            type = eCollisionType::PLAYER_FIRE_COLLISION;
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handlePlayerCollisionWithScriptEvent(LHNodeProtocol* nodeProt, bool begin)
{
    return (begin ? eCollisionType::PLAYER_SCRIPT_EVENT_COLLISION_BEGIN : eCollisionType::PLAYER_SCRIPT_EVENT_COLLISION_END);
}

eCollisionType CollisionManager::handlePlayerCollisionWithInspiration(Player *player, LHNodeProtocol *node)
{
    eCollisionType type = eCollisionType::NO_COLLISION;
    
    if (node->containsTag(gameTags::MUSIC_INSPIRATION_TAG))
    {
        if (player->getIsPlaying())
        {
            type = eCollisionType::PLAYER_INSPIRATION_COLLISION;
        }
    }
    else
    {
        type = eCollisionType::PLAYER_INSPIRATION_COLLISION;
    }

    return type;
}

//eCollisionType CollisionManager::handlePlayerCollisionWithFaith(LHNodeProtocol *node)
//{
//    auto faith = dynamic_cast<FaithJumpBrick*>(node);
//    if (faith && !faith->isAlreadyActive())
//    {
//        faith->buildPlatform();
//    }
//    
//    return eCollisionType::PLAYER_FAITH_COLLISION;
//}

template<typename T>
void CollisionManager::handleCharacterCollisionWithInteractive(T *character, LHNodeProtocol *block, bool begin)
{
    auto interactive = dynamic_cast<InteractiveBlock*>(block);
    if (interactive)
    {
        auto movable = interactive->getMovablePlatform();
        
        if ( movable != nullptr)
        {
            character->setIsOnMovablePlatform(movable, begin);
        }
    }
}

bool CollisionManager::shouldDisablePlayerCollisionWithNode(Node *nodeA, Node *nodeB)
{
    bool isDisabled = false;
    
    auto nodeProtA = dynamic_cast<LHNodeProtocol*>(nodeA);
    auto nodeProtB = dynamic_cast<LHNodeProtocol*>(nodeB);
    
    if(nodeProtA && nodeProtB)
    {
        if(nodeProtB->containsTag(gameTags::NOTE_TAG))
        {
            isDisabled = true;
        }
        else if (nodeProtB->containsTag(gameTags::SHEEP_TAG))
        {
            isDisabled = this->shouldDisablePlayerCollisionWithSheep(nodeProtA, nodeProtB);
        }
        else if (nodeProtB->containsTag(gameTags::ONESIDE_TAG))
        {
            isDisabled = this->shouldDisablePlayerCollisionWithPlatform(nodeProtA, nodeProtB);
        }
        else if (nodeProtB->containsTag(gameTags::GOAL_TAG))
        {
            isDisabled = true;
        }
        else if (nodeProtB->containsTag(gameTags::INTERACTIVE_TAG))
        {
            isDisabled = this->shouldDisablePlayerCollisionWithInteractive(nodeProtA, nodeProtB);
        }
    }
    
    return isDisabled;
}

bool CollisionManager::shouldDisablePlayerCollisionWithSheep(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto player = dynamic_cast<Player*>(nodeA);
    auto sheep = dynamic_cast<Sheep*>(nodeB);
    
    if (player && sheep)
    {
     
        b2Body* playerBody = player->getBox2dBody();
        b2Body* sheepBody = sheep->getBox2dBody();
            
        b2Vec2 playerPosition = playerBody->GetPosition();
        b2Vec2 sheepPosition = sheepBody->GetPosition();
        
        float playerRadius = player->getContentSize().height/2 * fabs(player->getScaleY()) / PTM_RATIO;
        float sheepTop = sheep->getContentSize().height/2  * fabs(sheep->getScaleY()) / PTM_RATIO;
        
        float slop = 0.2f;
        float dist = sheepPosition.y + sheepTop + playerRadius - slop;
        
        float actualVerticalZero = player->getVerticalZero();
        
        if ((playerPosition.y < dist) || (player->getBox2dBody()->GetLinearVelocity().y > actualVerticalZero))
        {
            return true;
        }
    }
    
    return false;
}

bool CollisionManager::shouldDisablePlayerCollisionWithPlatform(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto player = dynamic_cast<Player*>(nodeA);
    LHPhysicsProtocol * platformPhysics  = dynamic_cast<LHPhysicsProtocol *>(nodeB);
    
    if (player && platformPhysics)
    {
        b2Body* playerBody = player->getBox2dBody();
        b2Body* platformBody = platformPhysics->getBox2dBody();
        
        b2Vec2 playerPosition = playerBody->GetPosition();
        b2Vec2 platformPosition  = platformBody->GetPosition();
        
        float playerRadius = player->getContentSize().height/2 * fabs(player->getScaleY()) / PTM_RATIO;
        
        if (playerPosition.y < (platformPosition.y + playerRadius))
        {
            return true;
        }
    }
    
    return false;
}

bool CollisionManager::shouldDisablePlayerCollisionWithInteractive(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto player = dynamic_cast<Player*>(nodeA);
    LHPhysicsProtocol * interactivePhysics  = dynamic_cast<LHPhysicsProtocol *>(nodeB);
    
    if (player && interactivePhysics)
    {
        if (player->getIsBurning())
        {
            return true;
        }
        
        b2Body* playerBody = player->getBox2dBody();
        b2Body* interactiveBody = interactivePhysics->getBox2dBody();
        
        b2Vec2 playerPosition = playerBody->GetPosition();
        b2Vec2 interactivePosition  = interactiveBody->GetPosition();
        
        float playerRadius = player->getContentSize().height/2 * fabs(player->getScaleY()) / PTM_RATIO;
        float interactiveHeight = LHNodeProtocol::LHGetNode(nodeB)->getContentSize().height / PTM_RATIO;
        
        float interactiveThreshold = interactivePosition.y - interactiveHeight -
                                    fabs(sin(interactiveBody->GetAngle())) + playerRadius;
        
        if (player->getCollisionDisabled() || (playerPosition.y < interactiveThreshold))
        {
            player->setCollisionDisabled(true);
            return true;
        }
    }
    
    return false;
}

#pragma mark SHEEP SECTION

eCollisionType CollisionManager::handleSheepCollisionWithNode(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    Sheep* sheep = dynamic_cast<Sheep*>(nodeA);
    
    if(sheep)
    {
        if (nodeB->containsTag(gameTags::BOUND_TAG) || nodeB->containsTag(gameTags::BLOCK_TAG)/* || nodeB->containsTag("BRAZIER")*/)
        {
            type = handleSheepCollisionWithObstacle(sheep, nodeB, true, impulse);
        }
        else if (nodeB->containsTag(gameTags::FIRE_TAG))
        {
            type = this->handleSheepCollisionWithFire(sheep, nodeB);
        }
        else if (nodeB->containsTag(gameTags::GOAL_TAG))
        {
            if (sheep->containsTag(gameTags::MONSTER_TAG))
            {
                type = eCollisionType::MONSTER_GOAL_COLLISION;
            }
            else
            {
                type = eCollisionType::SHEEP_GOAL_COLLISION;
            }
            
            sheep->removeFromParent();
        }
        else if (nodeB->containsTag(gameTags::SWITCH_TAG))
        {
            type = handleCollisionWithSwitch(nodeB, nodeA, true);
        }
        else if (nodeB->containsTag(gameTags::WOLF_TAG))
        {
            type = handleSheepCollisionWithWolf(sheep, nodeB, true);
        }
        else if (nodeB->containsTag(gameTags::LOSE_TAG))
        {
            type = eCollisionType::SHEEP_LOSE_COLLISION;
        }
        else if (nodeB->containsTag(gameTags::START_EOLO_TAG))
        {
            type = this->handleCollisionWithEoloArea(sheep, nodeB, true);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleEndSheepCollisionWithNode(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    
    if(nodeB)
    {
        if (nodeB->containsTag(gameTags::BOUND_TAG) || nodeB->containsTag(gameTags::BLOCK_TAG))
        {
            type = handleSheepCollisionWithObstacle(nodeA, nodeB, false, 0.0f /*impulse*/);
        }
        else if (nodeB->containsTag(gameTags::SWITCH_TAG))
        {
            type = handleCollisionWithSwitch(nodeB, nodeA, false);
        }
        else if (nodeB->containsTag(gameTags::WOLF_TAG))
        {
            type = handleSheepCollisionWithWolf(nodeA, nodeB, false);
        }
        else if (nodeB->containsTag(gameTags::START_EOLO_TAG))
        {
            type = this->handleCollisionWithEoloArea(nodeA, nodeB, false);
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleSheepCollisionWithObstacle(LHNodeProtocol *nodeA, LHNodeProtocol *obstacle, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    auto sheep = dynamic_cast<Sheep*>(nodeA);
    LHPhysicsProtocol * obstaclePhysics = dynamic_cast<LHPhysicsProtocol*>(obstacle);
    
    if (sheep && obstaclePhysics)
    {
        type = begin ? eCollisionType::SHEEP_OBSTACLE_COLLISION_BEGIN : eCollisionType::SHEEP_OBSTACLE_COLLISION_END;
        auto blockAsset = dynamic_cast<LHAsset*>(LHNodeProtocol::LHGetNode(obstacle)->getParent());
        
        if (blockAsset)
        {
            const float kPixelsPerMeter = PTM_RATIO;
            float heightFactor = ((sheep->getContentSize().height * fabs(sheep->getScaleY())) / 2) + ((blockAsset->getContentSize().height *fabs(blockAsset->getScaleY())) / 2);
            float widthFactor = ((sheep->getContentSize().width * fabs(sheep->getScaleX())) / 2) + ((blockAsset->getContentSize().width * fabs(blockAsset->getScaleX())) / 2);
            heightFactor /= kPixelsPerMeter;
            widthFactor /= kPixelsPerMeter;
            
            b2Vec2 v = (obstaclePhysics->getBox2dBody()->GetPosition()) - (sheep->getBox2dBody()->GetPosition());
            
            if (fabs(v.x) > widthFactor - 0.25f)
            {
                sheep->handleBlockSideContact(obstacle, begin);
                if (v.y < -(heightFactor))
                {
                    sheep->handleBlockBelowContact(obstacle, begin);
                    
                    if (obstacle->containsTag(gameTags::MONOUSE_TAG))
                    {
                        this->handleMonousePlatform(obstacle);
                    }
                    
                    if (!sheep->isJumping())
                    {
                        sheep->handleInput(eAction::STOP_JUMP);
                    }
                }
                
                if (sheep->getIsExcited())
                {
                    sheep->handleCollisionWithObstacle();
                }
            }
            else if (v.y > heightFactor)
            {
                CCLOG("Sheep: Contact above %s", begin ? "started" : "ended");
            }
            else if (v.y < -(heightFactor - 0.05f)/* || obstacle->containsTag(gameTags::INTERACTIVE_TAG)*/)
            {
                sheep->handleBlockBelowContact(obstacle, begin);
                
                if (obstacle->containsTag(gameTags::INTERACTIVE_TAG))
                {
                    this->handleCharacterCollisionWithInteractive(sheep, obstacle, begin);
                }
                
                if (!sheep->isJumping())
                {
                    sheep->handleInput(eAction::STOP_JUMP);
                }
                
                if (obstacle->containsTag(gameTags::MONOUSE_TAG))
                {
                    this->handleMonousePlatform(obstacle);
                }
                
                if (obstacle->containsTag(gameTags::GOATSKIN_TAG))
                {
                    type = handleGoatskinCollisionWithCharacter(obstacle, begin, impulse);
                }
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleSheepCollisionWithWolf(LHNodeProtocol *sheep, LHNodeProtocol *node, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    auto wolf = dynamic_cast<Wolf*>(node);
    
    if (wolf)
    {
        wolf->updateStatus(sheep, begin);
        type = begin ? eCollisionType::SHEEP_WOLF_COLLISION_BEGIN : eCollisionType::SHEEP_WOLF_COLLISION_END;
    }
   
    return type;
}

eCollisionType CollisionManager::handleSheepCollisionWithFire(Sheep* sheep, LHNodeProtocol *nodeProt)
{
    auto type = eCollisionType::NO_COLLISION;
    auto node = dynamic_cast<LHNode*>(nodeProt);
    
    if (node)
    {
        auto brazier = dynamic_cast<Brazier*>(node->getParent());
        
        if (brazier && (brazier->getBrazierStatus() == eBrazierStatus::BRAZIER_ON))
        {
            type = eCollisionType::SHEEP_FIRE_COLLISION;
            sheep->burn();
        }
    }
    
    return type;
}

bool CollisionManager::shouldDisableSheepCollisionWithNode(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    bool isDisabled = false;
    Sheep* sheep = dynamic_cast<Sheep*>(nodeA);
    
    if(sheep)
    {
        if(nodeB->containsTag(gameTags::NOTE_TAG))
        {
            isDisabled = true;
        }
        else if (nodeB->containsTag(gameTags::INTERACTIVE_TAG) && sheep->getIsBurning())
        {
            isDisabled = true;
        }
        else if (nodeB->containsTag(gameTags::ONESIDE_TAG) || ((!sheep->getIsExcited() ||
                sheep->getIsBehindInteractive()) && nodeB->containsTag(gameTags::INTERACTIVE_TAG)))
        {
            isDisabled = this->shouldDisableSheepCollisionWithPlatform(sheep, nodeB);
        }
    }
    
    return isDisabled;
}

bool CollisionManager::shouldDisableSheepCollisionWithPlatform(Sheep *sheep, LHNodeProtocol *node)
{
    LHPhysicsProtocol* platformPhysics = dynamic_cast<LHPhysicsProtocol*>(node);
    b2Body* sheepBody = sheep->getBox2dBody();
    
    if (platformPhysics)
    {
        b2Body* platformBody = platformPhysics->getBox2dBody();
        
        b2Vec2 sheepPosition = sheepBody->GetPosition();
        b2Vec2 platformPosition  = platformBody->GetPosition();
        
        float sheepRadius = sheep->getContentSize().height/2 * fabs(sheep->getScaleY()) / PTM_RATIO;
        
        if (sheepPosition.y < (platformPosition.y + sheepRadius))
        {
            sheep->setIsBehindInteractive(true);    
            return true;
        }
    }
    
    return false;
}

#pragma mark MOVABLE PLATFORM SECTION

eCollisionType CollisionManager::handleMovablePlatformChangeDirection(LHNodeProtocol* nodeA, LHNodeProtocol* direction)
{
    auto type = eCollisionType::NO_COLLISION;
    auto platformPhysics = dynamic_cast<LHPhysicsProtocol*>(nodeA);
    
    if (platformPhysics && direction)
    {
        b2Vec2 vel;
        vel.Set(0.0f, 0.0f);
        type = this->getMovablePlatformVelocityChange(direction, vel);
        platformPhysics->getBox2dBody()->SetLinearVelocity(vel);
    }
    
    return type;
}

eCollisionType CollisionManager::handleMovablePlatformInteractiveBlockCollision(LHNodeProtocol *platform, LHNodeProtocol *nodeB, bool begin)
{
    auto type = eCollisionType::NO_COLLISION;
    
    auto block = dynamic_cast<InteractiveBlock*>(nodeB);
    if (block && platform)
    {
        block->setIsOnMovablePlatform(platform, begin);
        type = eCollisionType::MOVABLE_INTERACTIVE_COLLISION;
    }
    
    return type;
}

eCollisionType CollisionManager::getMovablePlatformVelocityChange(LHNodeProtocol* direction, b2Vec2 &vel)
{
    auto type = eCollisionType::NO_COLLISION;
    float velocity = 5.0f;
    
    if (direction->containsTag(gameTags::DIRECTION_LEFT_TAG))
    {
        vel.Set(-velocity, 0.0f);
        type = eCollisionType::MOVABLE_LEFT_COLLISION;
    }
    else if (direction->containsTag(gameTags::DIRECTION_RIGHT_TAG))
    {
        vel.Set(velocity, 0.0f);
        type = eCollisionType::MOVABLE_RIGHT_COLLISION;
    }
    else if (direction->containsTag(gameTags::DIRECTION_UP_TAG))
    {
        vel.Set(0.0f, velocity);
        type = eCollisionType::MOVABLE_UP_COLLISION;
    }
    else if (direction->containsTag(gameTags::DIRECTION_DOWN_TAG))
    {
        vel.Set(0.0f, -velocity);
        type = eCollisionType::MOVABLE_DOWN_COLLISION;
    }
    
    return type;
}

#pragma mark MONOUSE PLATFORM SECTION

eCollisionType CollisionManager::handleCollisionWithMonousePlatform(LHNodeProtocol* nodeA, LHNodeProtocol* block)
{
    auto type = eCollisionType::NO_COLLISION;
    
    auto monouse = dynamic_cast<MonousePlatform*>(nodeA);
    LHPhysicsProtocol* blockPhysics = dynamic_cast<LHPhysicsProtocol*>(block);
    
    if (monouse && blockPhysics)
    {
        auto blockAsset = dynamic_cast<LHAsset*>(LHNodeProtocol::LHGetNode(block)->getParent());
        if (blockAsset)
        {
            const float kPixelsPerMeter = PTM_RATIO;
            float heightFactor = ((monouse->getContentSize().height * fabs(monouse->getScaleY())) / 2) + ((blockAsset->getContentSize().height *fabs(blockAsset->getScaleY())) / 2);
            heightFactor /= kPixelsPerMeter;
            
            b2Vec2 v = monouse->getBox2dBody()->GetPosition() - blockPhysics->getBox2dBody()->GetPosition();
            
            if (v.y < -(heightFactor))
            {
                if (block->containsTag(gameTags::INTERACTIVE_TAG))
                {
                    this->handleMonousePlatform(nodeA);
                    type = eCollisionType::MONOUSE_INTERACTIVE_COLLISION;
                }
            }
        }
    }

    return type;
}

void CollisionManager::handleMonousePlatform(LHNodeProtocol *node)
{
    auto monouse = dynamic_cast<MonousePlatform*>(node);
    if (monouse)
    {
        monouse->startLifeTimer();
    }
}

#pragma mark BRAZIER SECTION

eCollisionType CollisionManager::handleBrazierCollisionWithBlock(Brazier *brazier, LHNodeProtocol *block)
{
    auto type = eCollisionType::NO_COLLISION;
    LHPhysicsProtocol* blockPhysics = dynamic_cast<LHPhysicsProtocol*>(block);
    auto blockAsset = dynamic_cast<LHAsset*>(LHNodeProtocol::LHGetNode(block)->getParent());
    auto brazierAsset = dynamic_cast<LHAsset*>(brazier->getParent());
    
    // la quarta condizione serve a verificare che la collisione non sia tra il fuoco e l'interactive dello stesso braciere
    if (blockPhysics && blockAsset && brazierAsset && (blockAsset != brazierAsset))
    {
        const float kPixelsPerMeter = PTM_RATIO;
        float heightFactor = ((brazier->getContentSize().height * fabs(brazier->getScaleY())) / 2) + ((blockAsset->getContentSize().height *fabs(blockAsset->getScaleY())) / 2);
        heightFactor /= kPixelsPerMeter;
        
        b2Vec2 v = blockPhysics->getBox2dBody()->GetPosition() - brazier->getBox2dBody()->GetPosition();
        
        if (v.y > -(heightFactor / 2))
        {
            brazier->blockEffectOnFire();
            type = eCollisionType::BLOCK_BRAZIER_COLLISION;
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleBrazierCollisionWithWolf(Brazier *brazier, LHNodeProtocol *node, bool begin)
{
    if (brazier->getBrazierStatus() == eBrazierStatus::BRAZIER_ON)
    {
        auto wolf = dynamic_cast<Wolf*>(node);
        
        if (wolf)
        {
            if (begin)
            {
                brazier->addObserver(wolf);
            }
            else
            {
                brazier->removeObserver(wolf);
            }
            
            wolf->updateStatus(brazier, begin);
        }
        
        return (begin ? eCollisionType::BRAZIER_WOLF_COLLISION_BEGIN : eCollisionType::BRAZIER_WOLF_COLLISION_END);
    }
    
    return eCollisionType::NO_COLLISION;
}

#pragma mark GOATSKIN SECTION

eCollisionType CollisionManager::handleGoatskinCollisionWithBlock(LHNodeProtocol *nodeA, LHNodeProtocol *block, bool begin, float impulse)
{
    auto type = eCollisionType::NO_COLLISION;
    
    auto goatskin = dynamic_cast<Goatskin*>(nodeA);
    LHPhysicsProtocol* blockPhysics = dynamic_cast<LHPhysicsProtocol*>(block);
    
    if (goatskin && (goatskin->getStatus() == eGoatskinStatus::GOATSKIN_IDLE) && blockPhysics)
    {
        auto blockAsset = dynamic_cast<LHAsset*>(LHNodeProtocol::LHGetNode(block)->getParent());
        if (blockAsset)
        {
            const float kPixelsPerMeter = PTM_RATIO;
            float heightFactor = ((goatskin->getContentSize().height * fabs(goatskin->getScaleY())) / 2) + ((blockAsset->getContentSize().height *fabs(blockAsset->getScaleY())) / 2);
            heightFactor /= kPixelsPerMeter;
            
            b2Vec2 v = goatskin->getBox2dBody()->GetPosition() - blockPhysics->getBox2dBody()->GetPosition();
            
            if (v.y < -(heightFactor) && (goatskin->getStatus() == eGoatskinStatus::GOATSKIN_IDLE))
            {
                type = this->activateGoatskin(goatskin, begin, impulse);
            }
            else if (v.y > heightFactor)
            {
                if (block->containsTag(gameTags::MONOUSE_TAG))
                {
                    this->handleMonousePlatform(block);
                }
            }
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleGoatskinCollisionWithCharacter(LHNodeProtocol *nodeA, bool begin, float impulse)
{
    auto goatskin = dynamic_cast<Goatskin*>(nodeA);
    eCollisionType type = eCollisionType::OFF_GOATSKIN_COLLISION;
    
    if (goatskin && (goatskin->getStatus() == eGoatskinStatus::GOATSKIN_IDLE))
    {
        type = this->activateGoatskin(goatskin, begin, impulse);
    }
    
    return type;
}

eCollisionType CollisionManager::activateGoatskin(Goatskin *goatskin, bool begin, float impulse)
{
    if (goatskin->turnOn(impulse, begin))
    {
        return eCollisionType::IDLE_GOATSKIN_COLLISION;
    }
    else
    {
        return eCollisionType::OFF_GOATSKIN_COLLISION;
    }
}

#pragma mark DOOR SWITCH SECTION

eCollisionType CollisionManager::handleCollisionWithSwitch(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB, bool begin)
{
    auto doorSwitch = dynamic_cast<Switch*>(nodeA);
    auto otherPhysicNode = dynamic_cast<LHPhysicsProtocol*>(nodeB);
    
    if (doorSwitch && otherPhysicNode)
    {
        auto switchAsset = dynamic_cast<LHAsset*>(doorSwitch->getParent());
        
        auto otherNode = dynamic_cast<Node*>(nodeB);
        if (otherNode)
        {
            auto otherAsset = dynamic_cast<LHAsset*>(otherNode->getParent());
            
            if (switchAsset && otherAsset)
            {
                const float kPixelsPerMeter = PTM_RATIO;
                float heightFactor = ((switchAsset->getContentSize().height * fabs(switchAsset->getScaleY())) / 4) + ((otherAsset->getContentSize().height *fabs(otherAsset->getScaleY())) / 4);
                heightFactor /= kPixelsPerMeter;
                
                b2Vec2 v = doorSwitch->getBox2dBody()->GetPosition() - otherPhysicNode->getBox2dBody()->GetPosition();
                
                if (v.y < -(heightFactor))
                {
                    doorSwitch->updateStatus(begin);
                }
            }
        }
    }
    
    return (begin ? eCollisionType::PLAYER_SWITCH_COLLISION_BEGIN : eCollisionType::PLAYER_SWITCH_COLLISION_END);
}

#pragma mark INTERACTIVE - ONESIDE SECTION

eCollisionType CollisionManager::handleInteractiveCollisionWithBlock(LHNodeProtocol* nodeA, LHNodeProtocol* nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    auto interactive = dynamic_cast<InteractiveBlock*>(nodeA);
    
    if (interactive)
    {
        if (nodeB->containsTag(gameTags::BLOCK_TAG))
        {
            interactive->playFallingSound();
            type = eCollisionType::INTERACTIVE_BLOCK_COLLISION;
        }
    }
    
    return type;
}

eCollisionType CollisionManager::handleInteractiveCollisionWithLose(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto type = eCollisionType::NO_COLLISION;
    auto interactive = dynamic_cast<InteractiveBlock*>(nodeA);
    
    if (interactive)
    {
        if (nodeB->containsTag(gameTags::LOSE_TAG))
        {
            type = eCollisionType::INTERACTIVE_LOSE_COLLISION;
        }
    }
    
    return type;
}

bool CollisionManager::shouldDisableInteractiveCollisionWithOneside(LHNodeProtocol *nodeA, LHNodeProtocol *nodeB)
{
    auto interactive = dynamic_cast<LHPhysicsProtocol*>(nodeA);
    LHPhysicsProtocol * oneside  = dynamic_cast<LHPhysicsProtocol *>(nodeB);
    
    if (interactive && oneside)
    {
        b2Body* interactiveBody = interactive->getBox2dBody();
        b2Body* onesidePlatform = oneside->getBox2dBody();
        
        b2Vec2 interactivePosition = interactiveBody->GetPosition();
        b2Vec2 onesidePosition  = onesidePlatform->GetPosition();
        
        Node * interactiveNode = LHPhysicsProtocol::LHGetNode(interactive);
        
        float interactiveRadius = interactiveNode->getContentSize().height/2 * fabs(interactiveNode->getScaleY()) / PTM_RATIO;
        
        if (interactivePosition.y < (onesidePosition.y + interactiveRadius))
        {
            return true;
        }
    }
    
    return false;
}

#pragma mark EOLO SECTION

eCollisionType CollisionManager::handleCollisionWithEoloArea(LHNodeProtocol *node, LHNodeProtocol *zone, bool begin)
{
    auto eoloZone = dynamic_cast<EoloZone*>(zone);
    if (eoloZone)
    {
        auto windMovable = dynamic_cast<WindMovable*>(node);
        if (windMovable)
        {
            if (begin)
            {
                eoloZone->handleObjectsIn(windMovable);
            }
            else
            {
                eoloZone->handleObjectsOut(windMovable);
            }
        }
    }
    
    return eCollisionType::EOLO_AREA_COLLISION;
}