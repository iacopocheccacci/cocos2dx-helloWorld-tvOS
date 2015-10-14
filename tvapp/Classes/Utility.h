//
//  Utility.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/11/14.
//
//

#ifndef __SOP_UTILITY__
#define __SOP_UTILITY__

#include "cocos2d.h"

USING_NS_CC;

class LHShape;
class LHPhysicsProtocol;
class LHNodeProtocol;
class LHNode;
class Goatskin;
enum class eGroundType;
enum class eLevelType;

namespace gameObjectsParams
{
    static const char* PLAYER_GROUP = "Input";
    static const char* PLAYER_HOR_IMP_NAME = "horizontalImpulse";
    static const char* PLAYER_HOR_IMP_VAL = "0.3";
    static const char* PLAYER_VER_IMP_NAME = "verticalImpulse";
    static const char* PLAYER_VER_IMP_VAL = "12.35";
    static const char* PLAYER_HOR_LIMIT_NAME = "horizontalLimit";
    static const char* PLAYER_HOR_LIMIT_VAL = "8.0";
    
    static const char* SHEEP_GROUP = "Sheep";
    static const char* SHEEP_DISTANCE_NAME = "sheepDistance";
    static const char* SHEEP_DISTANCE_VAL = "0.2";
    static const char* SHEEP_HOR_IMP_NAME = "horizontalImpulse";
    static const char* SHEEP_HOR_IMP_VAL = "0.54";
    static const char* SHEEP_VER_IMP_NAME = "verticalImpulse";
    static const char* SHEEP_VER_IMP_VAL = "8.4";
    static const char* SHEEP_HOR_LIMIT_NAME = "horizontalLimit";
    static const char* SHEEP_HOR_LIMIT_VAL = "6.0";
       
    static const char* NOTE_GROUP = "Note";
    static const char* NOTE_DURATION_NAME = "noteDuration";
    static const char* NOTE_DURATION_VAL = "10.0";
    static const char* NOTE_PARTICLE_SCALE_NAME = "particleScale";
    static const char* NOTE_PARTICLE_SCALE_VAL = "1.25";
    static const char* NOTE_DISTANCE_NAME = "noteDistance";
    static const char* NOTE_DISTANCE_VAL = "9.5";
    static const char* NOTE_SHEEP_PAN_DIST_NAME = "sheepPanDistance";
    static const char* NOTE_SHEEP_PAN_DIST_VAL = "1.5";
    
    
    static const char* GOATSKIN_GROUP = "Goatskin";
    static const char* GOATSKIN_RANGE_NAME = "range";
    static const char* GOATSKIN_RANGE_VAL = "200,0";
}

class Utility
{
public:
    
    // file utilities
    static __String* loadDataFromFile(std::string group, std::string key, std::string defaultValue);
    static __Array* loadLevelsFromFile();
    
    // object interactions utilities
    static bool isInXrange(Goatskin *source, LHPhysicsProtocol* object);
    static bool isInYrange(Goatskin *source, LHPhysicsProtocol* object);
    
    static void applyBottomGhostVertex(LHNode* node);
    static void applyBottomGhostVertex(LHShape* shape);
    static void applyTopGhostVertex(LHNode* node);
    static void applyTopGhostVertex(LHShape* shape);

    /** Estrae il numero del mondo dal nome di un livello. Se non esiste corrispondenza, ritorna -1. */
    static int getWorldFromLevelName(std::string levelName);
    static int getLevelFromLevelName(std::string levelName);
    static void getPrevLevelName(std::string levelName, char* prevLevelName);
    static void getNextLevelName(std::string levelName, char* nextLevelName);
    static void getLevelSelectionName(int world, char* levelSelectionName);
    static void getLevelName(int world, int level, char* levelName);
    
    static eLevelType getLevelType(std::string levelName);
    
    static eGroundType getGroundTypeFromNode(LHNodeProtocol * node);
    
    static float convertValueBetweenRanges(float value, float oldMin,  float oldMax,  float newMin,  float newMax);
    
    static Rect convertRectToWorldSpace(Node * node, Rect rect);
    static Rect convertRectToNodeSpace(Node * node, Rect rect);
    
    static void setDensity(LHPhysicsProtocol* node, float density);
    
    static bool convertIntToRoman(int number, std::string& romanString);
    
    static void fadeInNodeWithTime(Node * target, float time);
    static void fadeOutNodeWithTime(Node * target, float time);
    
    static void pauseAllChildrenRecursive(Node * root);
    static void resumeAllChildrenRecursive(Node * root);
    
    static void setCascadeOpacityRecursive(Node * root, bool enabled);
    
private:
    static bool checkIfAffected(Goatskin *source, LHPhysicsProtocol *object);
    static bool checkDistance(float from, float to);
    static bool checkRange(float source, float radius, float object);
};

#endif // __SOP_UTILITY__