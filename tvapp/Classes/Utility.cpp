//
//  Utility.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 12/11/14.
//
//

#include "Utility.h"
#include <LevelHelper2API.h>
#include "Definitions.h"
#include "Goatskin.h"

__String* Utility::loadDataFromFile(std::string group, std::string key, std::string defaultValue)
{
#ifdef EDITOR_MODE
    std::string writablePath = FileUtils::getInstance()->getWritablePath();
    std::string fullPath = writablePath + appParams::PARAMS_FILE_NAME;
    
    auto loadDict = __Dictionary::createWithContentsOfFile(fullPath.c_str());
#else
    auto loadDict = __Dictionary::createWithContentsOfFile(appParams::PARAMS_FILE_NAME);
#endif
    
    auto loadDictInDict = (__Dictionary*)loadDict->objectForKey(group);
    
    if (loadDictInDict != nullptr)
    {
        auto dictObject = (__String*)loadDictInDict->objectForKey(key);
        
        if (dictObject != nullptr)
        {
            return dictObject;
        }
    }
    
    CC_SAFE_DELETE(loadDict);
    
    return __String::create(defaultValue);
}

__Array* Utility::loadLevelsFromFile()
{
    auto loadDict = __Dictionary::createWithContentsOfFile(appParams::LEVELS_LIST_FILE_NAME);
    auto loadArray = dynamic_cast<__Array*>(loadDict->objectForKey(appParams::LEVELS_ARRAY_NAME));
        
    return loadArray;
}

bool Utility::isInXrange(Goatskin *source, LHPhysicsProtocol *object)
{
    bool res = false;
    b2Vec2 objectPosition = object->getBox2dBody()->GetPosition();
    b2Vec2 sourcePosition = source->getBox2dBody()->GetPosition();
    
    if (source->containsTag(gameTags::DIRECTION_RIGHT_TAG))
    {
        if (checkIfAffected(source, object))
        {
            res = checkDistance(sourcePosition.x, objectPosition.x);
        }
    }
    else if (source->containsTag(gameTags::DIRECTION_LEFT_TAG))
    {
        if (checkIfAffected(source, object))
        {
            res = checkDistance(objectPosition.x, sourcePosition.x);
        }
    }
    else if (source->containsTag(gameTags::DIRECTION_UP_TAG))
    {
        auto sourceAsset = dynamic_cast<LHAsset*>(source->getParent());
        
        if (sourceAsset)
        {
            float heightFactor = ((sourceAsset->getContentSize().width) / 2) / PTM_RATIO;
            sourcePosition.x += heightFactor * sourceAsset->getScaleX();
            res = checkRange(sourcePosition.x, heightFactor, objectPosition.x);
        }
    }
    
    return res;
}

bool Utility::isInYrange(Goatskin *source, LHPhysicsProtocol *object)
{
    bool res = false;
    b2Vec2 objectPosition = object->getBox2dBody()->GetPosition();
    b2Vec2 sourcePosition = source->getBox2dBody()->GetPosition();
    
    if (source->containsTag(gameTags::DIRECTION_RIGHT_TAG) || source->containsTag(gameTags::DIRECTION_LEFT_TAG))
    {
        auto sourceAsset = dynamic_cast<LHAsset*>(source->getParent());
        
        if (sourceAsset)
        {
            float heightFactor = (sourceAsset->getContentSize().height * fabs(sourceAsset->getScaleY())) / PTM_RATIO;
            
            res = checkRange(sourcePosition.y, heightFactor, objectPosition.y);
        }
    }
    else if (source->containsTag(gameTags::DIRECTION_UP_TAG))
    {
        if(checkIfAffected(source, object))
        {
            res = checkDistance(sourcePosition.y, objectPosition.y);
        }
    }
    
    return res;
}

int Utility::getWorldFromLevelName(std::string levelName)
{
    if (levelName.find("world") != std::string::npos)
    {
        std::string worldNumberStr = levelName.substr(5,7);
        int world = atoi(worldNumberStr.c_str());
        
        return world;
    }
    else if (levelName.find("LevelSelectionWorld") != std::string::npos)
    {
        std::string worldNumberStr = levelName.substr(19,7);
        int world = atoi(worldNumberStr.c_str());
        
        return world;
    }
    return -1;
}

int Utility::getLevelFromLevelName(std::string levelName)
{
    if (levelName.find("lev") != std::string::npos)
    {
        std::string levelNumberStr = levelName.substr(11,13);
        int level = atoi(levelNumberStr.c_str());
        
        return level;
    }
    return -1;
}

void Utility::getNextLevelName(std::string levelName, char* nextLevelName)
{
    int world = Utility::getWorldFromLevelName(levelName);
    int level = Utility::getLevelFromLevelName(levelName);
    
    if (world == 3 || world == 2)
    {
        if (level < 8)
        {
            level++;
        }
        else
        {
            level = 1;
            world++;
        }
    }
    else if (level < 7)
    {
        level++;
    }
    else if (level == 7)
    {
        level = 1;
        world++;
    }
    
    if (world == -1 || world > appParams::NUMBER_OF_WORLDS || level == -1)
    {
        return;
    }
    
    snprintf(nextLevelName, 50, appParams::PUBLISHED_LEVEL_NAME_TEMPLATE, world, level);
}

void Utility::getPrevLevelName(std::string levelName, char* prevLevelName)
{
    int world = Utility::getWorldFromLevelName(levelName);
    int level = Utility::getLevelFromLevelName(levelName);
    
    if (level > 1)
    {
        level--;
    }
    else if (level == 1)
    {
        world--;
        
        if (world == 3 || world == 2)
        {
            level = 8;
        }
        else
        {
            level = 7;
        }
    }
    else if (world == -1 || world == 6 || level == -1)
    {
        return;
    }
    
    snprintf(prevLevelName, 50, appParams::PUBLISHED_LEVEL_NAME_TEMPLATE, world, level);
}

void Utility::getLevelSelectionName(int world, char* levelSelectionName)
{
    if (world == -1)
    {
        return;
    }
    snprintf(levelSelectionName, 50, appParams::PUBLISHED_LEVEL_SELECTION_NAME_TEMPLATE, world);
}

void Utility::getLevelName(int world, int level, char* levelName)
{
    if (world == -1 || level == -1)
    {
        return;
    }
    
    snprintf(levelName, 50, appParams::PUBLISHED_LEVEL_NAME_TEMPLATE, world, level);
}

eLevelType Utility::getLevelType(std::string levelFullName)
{
    if (levelFullName.compare(appParams::PUBLISHED_MAIN_MENU_LEVEL_NAME) == 0)
    {
        return eLevelType::MAIN_MENU;
    }
    else if (levelFullName.find("LevelSelection") != std::string::npos)
    {
        return eLevelType::LEVEL_SELECTION;
    }
    else if (levelFullName.compare("Splash.lhplist") == 0)
    {
        return eLevelType::LOADING;
    }
    else
    {
        return eLevelType::NORMAL;
    }
}

void Utility::applyBottomGhostVertex(LHNode* node)
{
    b2FixtureDef myFixtureDef;
    myFixtureDef.density = 0.1f;
    myFixtureDef.friction = 0.2f;
    b2PolygonShape polygonShape;
    myFixtureDef.shape = &polygonShape;
    polygonShape.SetAsBox(1, 1);
    
    b2EdgeShape edgeShape;
    myFixtureDef.shape = &edgeShape;
    
    float height = (node->getContentSize().height * fabs(node->getScaleY())) / 32;
    float width = (node->getContentSize().width * fabs(node->getScaleX())) / 32;
    
    float left = -width/2;
    float right = width/2;
    
    edgeShape.Set( b2Vec2(left,-(height/2)), b2Vec2(right,-(height/2)) );
    edgeShape.m_vertex0.Set( left+left, -(height/2) );
    edgeShape.m_vertex3.Set( right+right, -(height/2) );
    edgeShape.m_hasVertex0 = true;
    edgeShape.m_hasVertex3 = true;
    
    node->getBox2dBody()->CreateFixture(&myFixtureDef);
}

void Utility::applyBottomGhostVertex(LHShape* shape)
{
    b2FixtureDef myFixtureDef;
    myFixtureDef.density = 0.1f;
    myFixtureDef.friction = 0.2f;
    b2PolygonShape polygonShape;
    myFixtureDef.shape = &polygonShape;
    polygonShape.SetAsBox(1, 1);
    
    b2EdgeShape edgeShape;
    myFixtureDef.shape = &edgeShape;
    
    float height = (shape->getContentSize().height * fabs(shape->getScaleY())) / 32;
    float width = (shape->getContentSize().width * fabs(shape->getScaleX())) / 32;
    
    float left = -width/2;
    float right = width/2;
    
    edgeShape.Set( b2Vec2(left,-(height/2)), b2Vec2(right,-(height/2)) );
    edgeShape.m_vertex0.Set( left+left, -(height/2) );
    edgeShape.m_vertex3.Set( right+right, -(height/2) );
    edgeShape.m_hasVertex0 = true;
    edgeShape.m_hasVertex3 = true;

    
    shape->getBox2dBody()->CreateFixture(&myFixtureDef);
}

void Utility::applyTopGhostVertex(LHNode *node)
{
    b2FixtureDef myFixtureDef;
    myFixtureDef.density = 0.1f;
    myFixtureDef.friction = 0.2f;
    b2PolygonShape polygonShape;
    myFixtureDef.shape = &polygonShape;
    polygonShape.SetAsBox(1, 1);
    
    b2EdgeShape edgeShape;
    myFixtureDef.shape = &edgeShape;
    
    float height = (node->getContentSize().height * fabs(node->getScaleY())) / 32;
    float width = (node->getContentSize().width * fabs(node->getScaleX())) / 32;
    
    float left = -width/2;
    float right = width/2;
    
    edgeShape.Set( b2Vec2(left,height/2), b2Vec2(right,height/2) );
    edgeShape.m_vertex0.Set( left+left, height/2 );
    edgeShape.m_vertex3.Set( right+right, height/2 );
    edgeShape.m_hasVertex0 = true;
    edgeShape.m_hasVertex3 = true;
    
    node->getBox2dBody()->CreateFixture(&myFixtureDef);
}

void Utility::applyTopGhostVertex(LHShape *shape)
{
    b2FixtureDef myFixtureDef;
    myFixtureDef.density = 0.1f;
    myFixtureDef.friction = 0.2f;
    b2PolygonShape polygonShape;
    myFixtureDef.shape = &polygonShape;
    polygonShape.SetAsBox(1, 1);
    
    b2EdgeShape edgeShape;
    myFixtureDef.shape = &edgeShape;
    
    float height = (shape->getContentSize().height * fabs(shape->getScaleY())) / 32;
    float width = (shape->getContentSize().width * fabs(shape->getScaleX())) / 32;
    
    //    float left = -width/2;
    //    float right = width/2;
    //
    //    edgeShape.Set(b2Vec2(left,height/2.02f), b2Vec2(right,height/2.02f) );
    //    edgeShape.m_vertex0.Set( left - 1, height/2.02f );
    //    edgeShape.m_vertex3.Set( right + 1, height/2.02f );
    //    edgeShape.m_hasVertex0 = true;
    //    edgeShape.m_hasVertex3 = true;
    
    float left = -width/2;
    float right = width/2;
    
    edgeShape.Set( b2Vec2(left,height/2), b2Vec2(right,height/2) );
    edgeShape.m_vertex0.Set( left+left, height/2 );
    edgeShape.m_vertex3.Set( right+right, height/2 );
    edgeShape.m_hasVertex0 = true;
    edgeShape.m_hasVertex3 = true;
    
    shape->getBox2dBody()->CreateFixture(&myFixtureDef);
}

eGroundType Utility::getGroundTypeFromNode(LHNodeProtocol * node)
{
    if (!node) return eGroundType::NONE;
 
    eGroundType groundType = eGroundType::STONE;
    
    if (node->containsTag(gameTags::MONOUSE_TAG))
    {
        groundType = eGroundType::GRASS;
    }
    else if (node->containsTag(gameTags::GOATSKIN_TAG) || node->containsTag(gameTags::SHEEP_TAG))
    {
        groundType = eGroundType::NONE;
    }
    else
    {
        Node * nodeObj = LHNodeProtocol::LHGetNode(node);
        LHAsset * asset = dynamic_cast<LHAsset *>(nodeObj->getParent());
        if (asset)
        {
            if (asset->containsTag(gameTags::GRASS_TAG))
            {
                groundType = eGroundType::GRASS;
            }
            else if (asset->containsTag(gameTags::TEMPLE_ROCK_TAG))
            {
                groundType = eGroundType::TEMPLE;
            }
        }
    }
    return groundType;
}

float Utility::convertValueBetweenRanges(float value, float oldMin,  float oldMax,  float newMin,  float newMax)
{
    float oldRange = oldMax - oldMin;
    float newRange = newMax - newMin;
    
    if (value >= 0.0f)
    {
        value = fmax(fmin(value, + oldMax), + oldMin);
        return (((value - oldMin) * newRange) / oldRange) + newMin;
    }
    else
    {
        value = fmax(fmin(value, - oldMin), - oldMax);
        return (((value + oldMin) * newRange) / oldRange) - newMin;
    }
}

Rect Utility::convertRectToWorldSpace(Node * node, Rect rect)
{
    if (node)
    {
        Point localOrigin = rect.origin;
        Point localDest = rect.origin + Point(rect.size);
        Point worldOrigin = node->convertToWorldSpace(localOrigin);
        Point worldDest = node->convertToWorldSpace(localDest);
        Rect worldRect = Rect(worldOrigin.x, worldOrigin.y, worldDest.x - worldOrigin.x, worldDest.y - worldOrigin.y);
        
        return worldRect;
    }
    else
    {
        return Rect::ZERO;
    }
}

Rect Utility::convertRectToNodeSpace(Node * node, Rect rect)
{
    if (node)
    {
        Point worldOrigin = rect.origin;
        Point worldDest = rect.origin + Point(rect.size);
        Point localOrigin = node->convertToNodeSpace(worldOrigin);
        Point localDest = node->convertToNodeSpace(worldDest);
        Rect localRect = Rect(localOrigin.x, localOrigin.y, localDest.x - localOrigin.x, localDest.y - localOrigin.y);
        
        return localRect;
    }
    else
    {
        return Rect::ZERO;
    }
}

void Utility::setDensity(LHPhysicsProtocol* node, float density)
{
    b2Fixture* fix = node->getBox2dBody()->GetFixtureList();
    
    bool needReset = false;
    
    while (fix)
    {
        float newDensity;
        if (b2Shape::Type::e_circle == fix->GetShape()->GetType())
        {
            // Le shape circolari sono quasi sempre per metà dentro un'altra shape, per cui dovranno pesare la metà
            if (fix->GetDensity() != 0.0f)
            {
                newDensity = density / 2.0f;
            }
            else
            {
                newDensity = fix->GetDensity(); // = 0
            }
        }
        else
        {
            if (fix->GetDensity() == 0.0f)
            {
                newDensity = fix->GetDensity(); // = 0
            }
            else
            {
                newDensity = density;
            }
        }
        
        if (fix->GetDensity() != newDensity)
        {
            fix->SetDensity(newDensity);
            needReset = true;
        }
        
        fix = fix->GetNext();
    }
    
    if (needReset)
    {
        node->getBox2dBody()->ResetMassData();
    }
}

// check if object is in the source action radius
bool Utility::checkIfAffected(Goatskin *source, LHPhysicsProtocol *object)
{
    b2Vec2 objectPosition = object->getBox2dBody()->GetPosition();
    b2Vec2 sourcePosition = source->getBox2dBody()->GetPosition();
    float sourceBorder = (source->getParent()->getContentSize().width / 2) / PTM_RATIO;
    float distance = b2Distance(sourcePosition, objectPosition);
    float maxDistance = source->getRange() / PTM_RATIO;
    
    return ((distance < maxDistance) && (distance > sourceBorder));
}

// check if object is in the source orientation
bool Utility::checkDistance(float from, float to)
{
    return (to > from);
}

// check if object is in the source influence area (in width in case of vertical source, in height in case of horizontal source)
bool Utility::checkRange(float source, float radius, float object)
{
    return ((object > (source - radius)) && (object < (source + radius)));
}

bool Utility::convertIntToRoman(int number, std::string& romanString)
{
    static const std::map<int, std::string> conversionMap =
    {
        {1, "I"},
        {2, "II"},
        {3, "III"},
        {4, "IV"},
        {5, "V"},
        {6, "VI"},
        {7, "VII"},
        {8, "VIII"},
        {9, "IX"},
        {10, "X"}
    };
    
    bool res = false;
    
    if (number > 0 && number <= conversionMap.size())
    {
        res = true;
        romanString = conversionMap.at(number);
    }
    else
    {
        romanString = "";
        CCLOG("Non so contare oltre %d !! UGA!", (int)conversionMap.size());
    }
    return res;
}

void Utility::fadeInNodeWithTime(Node * target, float time)
{
    if (target)
    {
        target->stopActionByTag(static_cast<int>(eAction::FADE));
        if (time > 0.0f)
        {
            FadeIn * fadeIn = FadeIn::create(time);
            fadeIn->setTag(static_cast<int>(eAction::FADE));
            target->runAction(fadeIn);
        }
        else
        {
            target->setOpacity(255);
        }
    }
}

void Utility::fadeOutNodeWithTime(Node * target, float time)
{
    if (target)
    {
        target->stopActionByTag(static_cast<int>(eAction::FADE));
        if (time > 0.0f)
        {
            FadeOut * fadeOut = FadeOut::create(time);
            fadeOut->setTag(static_cast<int>(eAction::FADE));
            target->runAction(fadeOut);
        }
        else
        {
            target->setOpacity(0);
        }
    }
}

void Utility::pauseAllChildrenRecursive(Node * root)
{
    if (root)
    {
        root->pause();
        
        LHNodeAnimationProtocol * animProt = dynamic_cast<LHNodeAnimationProtocol*>(root);
        if (animProt)
        {
            LHAnimation * activeAnimation = animProt->getActiveAnimation();
            if (activeAnimation)
            {
                activeAnimation->setAnimating(false);
            }
        }
        
        for (Node * childNode : root->getChildren())
        {
            Utility::pauseAllChildrenRecursive(childNode);
        }
    }
}

void Utility::resumeAllChildrenRecursive(Node * root)
{
    if (root)
    {
        root->resume();
        
        LHNodeAnimationProtocol * animProt = dynamic_cast<LHNodeAnimationProtocol*>(root);
        if (animProt)
        {
            LHAnimation * activeAnimation = animProt->getActiveAnimation();
            if (activeAnimation)
            {
                activeAnimation->setAnimating(true);
            }
        }
        
        for (Node * childNode : root->getChildren())
        {
            Utility::resumeAllChildrenRecursive(childNode);
        }
    }
}

void Utility::setCascadeOpacityRecursive(Node * root, bool enabled)
{
    if (root)
    {
        root->setCascadeOpacityEnabled(enabled);
        
        for (Node * childNode : root->getChildren())
        {
            Utility::setCascadeOpacityRecursive(childNode, enabled);
        }
    }
}
