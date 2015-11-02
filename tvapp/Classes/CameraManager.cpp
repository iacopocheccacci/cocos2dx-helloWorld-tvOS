//
//  CameraManager.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 09/03/15.
//
//

#include "CameraManager.h"
#include "LHScene.h"
#include "VisibleRect.h"
#include "Door.h"
#include "Switch.h"
#include "InteractiveBlock.h"
#include "GameCamera.h"
#include "Utility.h"
#include "Definitions.h"
#include "Player.h"
#include "PlayerStandingState.h"
#include "LHUserPropertyProtocol.h"

enum class eFocusMode
{
    NONE = 0,
    ONE_SHOT,
    COOLDOWN
};

const float CameraManager::ZOOM_VALUE_ON_PLAYER_MAX = 0.8f;
const float CameraManager::ZOOM_VALUE_ON_PLAYER_MIN = 0.4f;
const float CameraManager::ZOOM_VALUE_FOCUS_MIN = 0.5f;
const float CameraManager::ZOOM_VALUE_ALL_SCREEN = 0.00001f;

const float CameraManager::ZOOM_SPEED_ALL_SCREEN = 1.8f;
const float CameraManager::ZOOM_SPEED_ON_PLAYER = 0.4f;//0.2f;
const float CameraManager::ZOOM_SPEED_FOCUS = 1.0f;

const float CameraManager::PANNING_SPEED_ALL_SCREEN = 1.0f;
const float CameraManager::PANNING_SPEED_FOCUS = 2.0f;//1.0f;

const float CameraManager::FOLLOW_TRAP_WIDTH = 75.0f;//50.0f;
const float CameraManager::FOLLOW_SPEED = 1.25f;
const float CameraManager::FOLLOW_OFFSET_X_MIN = - 300.0f;
const float CameraManager::FOLLOW_OFFSET_X_MAX = 300.0f;
const float CameraManager::FOLLOW_OFFSET_Y_MIN = - 250.0f;
const float CameraManager::FOLLOW_OFFSET_Y_MAX = 200.0f;//50.0f;
const float CameraManager::FOLLOW_TRAP_UPDATE_SPEED = 10.0f;
const char* CameraManager::FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY = "moveFollowOffsetScheduleLeft";
const char* CameraManager::FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY = "moveFollowOffsetScheduleRight";

// Zoom out in idle
const char* CameraManager::ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY = "zoomAllScreenIdleScheduleDelay";
const char* CameraManager::ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY = "zoomAllScreenIdleScheduleMove";
const char* CameraManager::ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY = "zoomAllScreenIdleScheduleZoom";
const float CameraManager::ZOOM_ALL_SCREEN_IDLE_DELAY = 3.0f;//2.3;//2.0f;
const float CameraManager::ZOOM_ALL_SCREEN_IDLE_UPDATE_PANNING_SPEED = 10.0f;
const float CameraManager::ZOOM_ALL_SCREEN_IDLE_UPDATE_ZOOMING_SPEED = 0.005f;
const float CameraManager::ZOOM_ALL_SCREEN_IDLE_ZOOM_SPEED = 0.3f;
const float CameraManager::ZOOM_ALL_SCREEN_IDLE_PANNING_SPEED = 0.15f;

// Focus
const eFocusMode CameraManager::FOCUS_EVENT_INTERACTIVE_MODE = eFocusMode::NONE;
const eFocusMode CameraManager::FOCUS_EVENT_DOOR_MODE = eFocusMode::NONE;//eFocusMode::COOLDOWN;

const float CameraManager::FOCUS_EVENT_INTERACTIVE_COOLDOWN = 10.0f;
const float CameraManager::FOCUS_EVENT_DOOR_COOLDOWN = 10.0f;

const char* CameraManager::FOCUS_EVENT_SCHEDULE_CHECK_KEY = "focusEventScheduleCheck";

// Zoom In base alla Y
const float CameraManager::VERTICAL_ZOOM_LOWER_BOUND = 400.0f;
const float CameraManager::VERTICAL_ZOOM_UPPER_BOUND = 800.0f;

// Follow offset in base alla Y
const float CameraManager::VERTICAL_FOLLOW_OFFSET_LOWER_BOUND = 200.0f;
const float CameraManager::VERTICAL_FOLLOW_OFFSET_UPPER_BOUND = 800.0f;

//
// Custom Camera
//

// Threshold
const char* CameraManager::CUSTOM_CAMERA_TAG_THRESHOLD = "CAMERA_THRESHOLD";

// Attractor
const char* CameraManager::CUSTOM_CAMERA_TAG_ATTRACTOR = "CAMERA_ATTRACTOR";

// Rail
const char* CameraManager::CUSTOM_CAMERA_TAG_RAIL = "CAMERA_RAIL";
const char* CameraManager::CUSTOM_CAMERA_TAG_RAIL_ZOOM = "RAIL_ZOOM";
const char* CameraManager::CUSTOM_CAMERA_TAG_RAIL_START_PROJ = "RAIL_START_PROJ";
const char* CameraManager::CUSTOM_CAMERA_TAG_RAIL_END_PROJ = "RAIL_END_PROJ";
const char* CameraManager::CUSTOM_CAMERA_TAG_RAIL_POINT_TEMPLATE = "RAIL_POINT_%d";

// Zone
const char* CameraManager::CUSTOM_CAMERA_TAG_ZONE = "CAMERA_ZONE";
const char* CameraManager::CUSTOM_CAMERA_TAG_ZONE_AREA = "CAMERA_AREA";
const char* CameraManager::CUSTOM_CAMERA_TAG_UPPER_BOUND = "CAMERA_UPPER_BOUND";
const char* CameraManager::CUSTOM_CAMERA_TAG_UPPER_SCREEN = "CAMERA_UPPER_SCREEN";
const char* CameraManager::CUSTOM_CAMERA_TAG_LOWER_BOUND = "CAMERA_LOWER_BOUND";
const char* CameraManager::CUSTOM_CAMERA_TAG_LOWER_SCREEN = "CAMERA_LOWER_SCREEN";

const bool CameraManager::DEBUG_CAMERA_MODE_ON = false;

enum class eCameraState
{
    READY = 0,
    ZOOMING_IN,
    ZOOMING_FOCUS,
    ZOOMING_OUT_FAST,
    ZOOMING_OUT_SLOW
};

enum class eZoomOutType
{
    IDLE,
    FAST,
    INSTANT
};

CameraManager::CameraManager()
{
    
}

CameraManager::~CameraManager()
{
    
}

CameraManager * CameraManager::create(LHScene * gameScene, Node * targetNode, Player * player, std::string levelName)
{
    CameraManager * ret = new (std::nothrow) CameraManager();
    if (ret != nullptr && ret->init(gameScene, targetNode, player, levelName))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
        ret = nullptr;
    }
    return ret;
}

bool CameraManager::init(LHScene * gameScene, Node * targetNode, Player * player, std::string levelName)
{
    if (!Node::init())
    {
        return false;
    }
    
    _gameScene = gameScene;
    _targetNode = targetNode;
    _player = player;
    _levelType = Utility::getLevelType(levelName);
    _isLastLevel = (5 == Utility::getWorldFromLevelName(levelName) && 7 == Utility::getLevelFromLevelName(levelName));
    
    if (player)
    {
        _trapLastPositionLeft = player->getPosition().x - FOLLOW_TRAP_WIDTH / 2.0f;
        _trapLastPositionRight = player->getPosition().x + FOLLOW_TRAP_WIDTH / 2.0f;
    }
    else
    {
        _trapLastPositionLeft = 0.0f;
        _trapLastPositionRight = 0.0f;
    }
    
    this->initCamera();
    
    this->initLevelCustom();
    
    // Initial Setup
    if (_isLastLevel)
    {
        this->zoomStandardStart();
    }
    else
    {
        switch (_levelType)
        {
            case eLevelType::NORMAL:
                this->zoomAllScreenStart();
                break;
            case eLevelType::MAIN_MENU:
            case eLevelType::LEVEL_SELECTION:
                this->zoomStandardStart();
                break;
            default:
                break;
        }
    }
    
    return true;
}

#pragma mark - Inizializzatori privati

void CameraManager::initLevelCustom()
{
    this->initLevelCustomCameraThreshold();
    this->initLevelCustomCameraAttractors();
    this->initLevelCustomCameraRails();
    this->initLevelCustomCameraZones();
}

void CameraManager::initLevelCustomCameraThreshold()
{
    _currentCameraThreshold = nullptr;
    
    if (_gameScene)
    {
        const std::vector<std::string> tagValues = {CUSTOM_CAMERA_TAG_THRESHOLD};
        __Array* cameraCustomNodesArray = _gameScene->getChildrenWithTags(tagValues);
        Ref* ref = nullptr;
        LHNode* cameraCustomNode = nullptr;
        
        CCARRAY_FOREACH(cameraCustomNodesArray, ref)
        {
            cameraCustomNode = dynamic_cast<LHNode*>(ref);
            _cameraThresholds.push_back(cameraCustomNode);
        }
        
        std::sort(_cameraThresholds.begin(), _cameraThresholds.end(), [](LHNode * i, LHNode * j) {
            return i->getPosition().x > j->getPosition().x;
        });
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            for (int i = 0; i < _cameraThresholds.size(); i++)
            {
                CCLOG("Camera Threshold X - %d - Posizione: %f", i, _cameraThresholds[i]->getPosition().x);
            }
        }
    }
}

void CameraManager::initLevelCustomCameraAttractors()
{
    if (_gameScene && _gameCamera)
    {
        const std::vector<std::string> tagValues = {CUSTOM_CAMERA_TAG_ATTRACTOR};
        __Array* cameraCustomNodesArray = _gameScene->getChildrenWithTags(tagValues);
        Ref* ref = nullptr;
        LHNode* cameraCustomNode = nullptr;
        
        CCARRAY_FOREACH(cameraCustomNodesArray, ref)
        {
            cameraCustomNode = dynamic_cast<LHNode*>(ref);
            if (cameraCustomNode)
            {
                CameraAttractor * cameraAttractor = dynamic_cast<CameraAttractor *>(cameraCustomNode->getUserProperty());
                if (cameraAttractor)
                {
                    std::string key = cameraCustomNode->getName();
                    Point screenPosition = cameraCustomNode->getParent()->convertToWorldSpace(cameraCustomNode->getPosition());
                    Point position = _gameCamera->getTarget()->convertToNodeSpace(screenPosition);
                    Size targetScreenSize = cameraCustomNode->getContentSize();
                    float innerRadius = cameraAttractor->getInnerRadius();
                    float outerRadius = cameraAttractor->getOuterRadius();
                    std::string fitModeStr = cameraAttractor->getFitMode();
                    
                    GameCamera::SizeConstraint fitMode = GameCamera::SizeConstraint::BOTH;
                    if (fitModeStr.compare("width") == 0)
                    {
                        fitMode = GameCamera::SizeConstraint::WIDTH;
                    }
                    else if (fitModeStr.compare("height") == 0)
                    {
                        fitMode = GameCamera::SizeConstraint::HEIGHT;
                    }
                    
                    _gameCamera->addAttractor(position, targetScreenSize, innerRadius, outerRadius, fitMode, key);
                }
            }
        }
    }
}

void CameraManager::initLevelCustomCameraRails()
{
    if (_gameScene && _gameCamera)
    {
        const std::vector<std::string> tagValues = {CUSTOM_CAMERA_TAG_RAIL};
        __Array* cameraCustomNodesArray = _gameScene->getChildrenWithTags(tagValues);
        Ref* ref = nullptr;
        LHNode* cameraCustomNode = nullptr;
        
        CCARRAY_FOREACH(cameraCustomNodesArray, ref)
        {
            cameraCustomNode = dynamic_cast<LHNode*>(ref);
            if (cameraCustomNode)
            {
                Point screenPosition;
                Point targetPosition;
                
                std::string key = cameraCustomNode->getName();
                std::vector<Point> points;
                std::vector<Size> targetSizes;
                Point startProjPoint = Point::ZERO;
                Point endProjPoint = Point::ZERO;
                Rect areaOfEffect = Rect::ZERO;
                Size targetScreenSize = Size::ZERO;
                Node * node = nullptr;
                __Array * array = nullptr;
                
                node = cameraCustomNode->getParent();
                if (node)
                {
                    Rect screenArea = Utility::convertRectToWorldSpace(node, cameraCustomNode->getBoundingBox());
                    areaOfEffect = Utility::convertRectToNodeSpace(_gameCamera->getTarget(), screenArea);
                }
                
                int index = 0;
                char railPointTag[15];
                do
                {
                    node = nullptr;
                    snprintf(railPointTag, 15, CUSTOM_CAMERA_TAG_RAIL_POINT_TEMPLATE, index);
                    array = cameraCustomNode->getChildrenWithTags({railPointTag});
                    if (array && array->count() > 0)
                    {
                        node = dynamic_cast<Node *>(array->getLastObject());
                        if (node)
                        {
                            Point screenPosition = cameraCustomNode->convertToWorldSpace(node->getPosition());
                            targetPosition = _gameCamera->getTarget()->convertToNodeSpace(screenPosition);
                            points.push_back(targetPosition);
                        }
                    }
                    index++;
                }
                while (nullptr != node);
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_RAIL_START_PROJ});
                if (array && array->count() > 0)
                {
                    Node * startProjNode = dynamic_cast<Node *>(array->getLastObject());
                    if (startProjNode)
                    {
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(startProjNode->getPosition());
                        startProjPoint = _gameCamera->getTarget()->convertToNodeSpace(screenPosition);
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_RAIL_END_PROJ});
                if (array && array->count() > 0)
                {
                    Node * endProjNode = dynamic_cast<Node *>(array->getLastObject());
                    if (endProjNode)
                    {
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(endProjNode->getPosition());
                        endProjPoint = _gameCamera->getTarget()->convertToNodeSpace(screenPosition);
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_RAIL_ZOOM});
                if (array && array->count() > 0)
                {
                    Node * targetScreenSizeNode = dynamic_cast<Node *>(array->getLastObject());
                    if (targetScreenSizeNode)
                    {
                        targetScreenSize = targetScreenSizeNode->getContentSize();
                    }
                }
                
                _gameCamera->addRail(areaOfEffect, startProjPoint, endProjPoint, points, targetScreenSize, key);
                
                // Punti di LHBezier non validi...
//                LHBezier * bezier = dynamic_cast<LHBezier *>(cameraCustomNode->getChildNodeWithName("RailPath"));
//                std::vector<Point> points = bezier->linePoints();
//                CCLOG("Numero punti binario telecamera: %d", (int)points.size());
//                for (auto iter = points.begin(); iter != points.end(); iter++)
//                {
//                    CCLOG("Point: %.02f, %.02f", (*iter).x, (*iter).y);
//                }
            }
        }
    }
}

void CameraManager::initLevelCustomCameraZones()
{
    _currentZone = nullptr;
    
    if (_gameScene && _gameCamera)
    {
        const std::vector<std::string> tagValues = {CUSTOM_CAMERA_TAG_ZONE};
        __Array* cameraCustomNodesArray = _gameScene->getChildrenWithTags(tagValues);
        Ref* ref = nullptr;
        LHNode* cameraCustomNode = nullptr;
        
        CCARRAY_FOREACH(cameraCustomNodesArray, ref)
        {
            cameraCustomNode = dynamic_cast<LHNode*>(ref);
            if (cameraCustomNode)
            {
                Point screenPosition;
                Point targetPosition;
                
                std::vector<Rect> areas;
                float lowerBound;
                float lowerOffset;
                float lowerZoom;
                float upperBound;
                float upperOffset;
                float upperZoom;
                
                __Array * array = nullptr;
                
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_ZONE_AREA});
                if (array && array->count() > 0)
                {
                    Ref* areaRef = nullptr;
                    LHNode* areaZoneNode = nullptr;
                    
                    CCARRAY_FOREACH(array, areaRef)
                    {
                        areaZoneNode = dynamic_cast<LHNode*>(areaRef);
                        if (areaZoneNode)
                        {
                            Rect screenArea = Utility::convertRectToWorldSpace(areaZoneNode->getParent(), areaZoneNode->getBoundingBox());
                            Rect areaOfEffect = Utility::convertRectToNodeSpace(_gameCamera->getTarget(), screenArea);
                            areas.push_back(areaOfEffect);
                        }
                        
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_UPPER_BOUND});
                if (array && array->count() > 0)
                {
                    Node * upperBoundNode = dynamic_cast<Node *>(array->getLastObject());
                    if (upperBoundNode)
                    {
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(upperBoundNode->getPosition());
                        upperBound = _gameCamera->getTarget()->convertToNodeSpace(screenPosition).y;
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_LOWER_BOUND});
                if (array && array->count() > 0)
                {
                    Node * lowerBoundNode = dynamic_cast<Node *>(array->getLastObject());
                    if (lowerBoundNode)
                    {
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(lowerBoundNode->getPosition());
                        lowerBound = _gameCamera->getTarget()->convertToNodeSpace(screenPosition).y;
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_UPPER_SCREEN});
                if (array && array->count() > 0)
                {
                    Node * upperScreenNode = dynamic_cast<Node *>(array->getLastObject());
                    if (upperScreenNode)
                    {
                        upperZoom = _gameCamera->getMinZoomScaleForSize(upperScreenNode->getContentSize());
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(upperScreenNode->getPosition());
                        upperOffset = _gameCamera->getTarget()->convertToNodeSpace(screenPosition).y;
                        
                    }
                }
                
                array = cameraCustomNode->getChildrenWithTags({CUSTOM_CAMERA_TAG_LOWER_SCREEN});
                if (array && array->count() > 0)
                {
                    Node * lowerScreenNode = dynamic_cast<Node *>(array->getLastObject());
                    if (lowerScreenNode)
                    {
                        lowerZoom = _gameCamera->getMinZoomScaleForSize(lowerScreenNode->getContentSize());
                        Point screenPosition = cameraCustomNode->convertToWorldSpace(lowerScreenNode->getPosition());
                        lowerOffset = _gameCamera->getTarget()->convertToNodeSpace(screenPosition).y;
                        
                    }
                }
                
                auto zone = std::unique_ptr<Zone>(new Zone(areas, lowerOffset, lowerZoom, lowerBound, upperOffset, upperZoom, upperBound));
                _cameraZones.push_back(std::move(zone));
            }
        }
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            this->updateDebugDrawZoneAreas();
        }
    }
}

void CameraManager::initCamera()
{
    CCASSERT(_gameScene, "Scena di gioco mancante!");
    CCASSERT(_targetNode, "Nodo target per camera mancante!");
    
    Point visibleOrigin = Director::getInstance()->getVisibleOrigin();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    
    Rect gameWorldRect = _gameScene->getGameWorldRect();
    float worldBoundOriginX = gameWorldRect.origin.x;
    float worldBoundOriginY = gameWorldRect.size.height + gameWorldRect.origin.y;
    float worldBoundWidth = gameWorldRect.size.width;
    float worldBoundHeight = fabs(gameWorldRect.size.height);
    Rect worldBoundaries = Rect(worldBoundOriginX, worldBoundOriginY, worldBoundWidth, worldBoundHeight);
    bool clampToBoundaries = true;
    
    auto camera = std::unique_ptr<GameCamera>(new GameCamera(_targetNode, worldBoundaries, clampToBoundaries));
    _gameCamera = move(camera);
    
    // Impostazioni per l'effetto follow
    if (_player)
    {
        Point screenCenter = Point(visibleOrigin.x + visibleSize.width / 2.0f, visibleOrigin.y + visibleSize.height / 2.0f);

        float followSpeed = FOLLOW_SPEED;
        bool followSmoothMovement = true;
        Point followStartingOffset = Point(0, 50.0f);// Il valore verrà subito riaggiornato dall'updateYFactors
        bool followEnabled = false;
        
        _gameCamera->setFollowNode(_player, followSpeed, followSmoothMovement);
        _gameCamera->setFollowedNodeOffset(followStartingOffset);
        _gameCamera->setFollowEnabled(followEnabled);
        
        // Follow Area
        float followAreaWidth = 0.0f;
        float followAreaHeight = 200.0f;
        float followAreaOriginX = screenCenter.x - followAreaWidth / 2.0f;
        float followAreaOriginY = screenCenter.y - followAreaHeight / 3.0f;
        Rect followArea = Rect(followAreaOriginX, followAreaOriginY, followAreaWidth, followAreaHeight);
        
        _gameCamera->setFollowedArea(followArea);
    }
        
    // Draw Debug
    _gameCamera->setDebugEnable(DEBUG_CAMERA_MODE_ON);
    
    _zoomFocusAction = nullptr;
    this->setCameraState(eCameraState::READY);
    
    CallFunc * delayedObservers = CallFunc::create([this]() {
        this->initCameraObservers();
    });
    
    this->runAction(Sequence::create(DelayTime::create(0.5f), delayedObservers, NULL));
    
    this->scheduleUpdate();
}

void CameraManager::initCameraObservers()
{
    if (eFocusMode::NONE != FOCUS_EVENT_DOOR_MODE)
    {
        this->initCameraObserversDoor();
    }
    if (eFocusMode::NONE != FOCUS_EVENT_INTERACTIVE_MODE)
    {
        this->initCameraObserversInteractive();
    }
}

void CameraManager::initCameraObserversDoor()
{
    const std::vector<std::string> tagValues = {"DOOR"};
    __Array* doorArray = _gameScene->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    Door* door = nullptr;
    
    CCARRAY_FOREACH(doorArray, ref)
    {
        door = dynamic_cast<Door*>(ref);
        if (door)
        {
            door->addObserver(this);
        }
    }
}

void CameraManager::initCameraObserversInteractive()
{
    const std::vector<std::string> tagValues = {"INTERACTIVE"};
    __Array* interactiveArray = _gameScene->getChildrenWithTags(tagValues);
    Ref* ref = nullptr;
    InteractiveBlock * interactive = nullptr;
    
    CCARRAY_FOREACH(interactiveArray, ref)
    {
        interactive = dynamic_cast<InteractiveBlock*>(ref);
        if (interactive)
        {
            interactive->addObserver(this);
        }
    }
}

#pragma mark - Metodi pubblici

void CameraManager::zoomToggle()
{
    if (_cameraState == eCameraState::ZOOMING_OUT_FAST)
    {
        this->zoomStandard();
    }
    else
    {
        this->zoomAllScreenButton();
    }
}

void CameraManager::zoomResume()
{
    this->resetSlowZoomAllScreenSchedule();
    
    if (_cameraState == eCameraState::ZOOMING_OUT_SLOW || _cameraState == eCameraState::ZOOMING_OUT_FAST)
    {
        this->zoomStandard();
    }
}

void CameraManager::handleCameraEvent(LHNode * triggerNode)
{
    CCLOG("Camera Event: %s", triggerNode->getName().c_str());
    CameraFocus * cameraFocus = dynamic_cast<CameraFocus *>(triggerNode->getUserProperty());
    if (cameraFocus)
    {
        this->cameraEventFocus(cameraFocus);
    }
}

void CameraManager::handleFluteTaken()
{
    if (_gameCamera)
    {
        _gameCamera->removeAttractor("CameraAttractorFlute");
    }
}

void CameraManager::handleDefeat()
{
    if (_gameCamera)
    {
        _gameCamera->setFollowEnabled(false);
    }
//    static const char * defeatFollowNodeName = "defeatFollowNodeName";
//    if (_gameCamera && _player)
//    {
//        Node * defeatFollowNode = _player->getParent()->getChildByName(defeatFollowNodeName);
//        if (!defeatFollowNode)
//        {
//            defeatFollowNode = Node::create();
//            defeatFollowNode->setName(defeatFollowNodeName);
//            
//            Point screenPlayerPosition = _player->getParent()->convertToWorldSpace(_player->getPosition());
//            Point parentPlayerPosition = _player->getParent()->convertToNodeSpace(screenPlayerPosition);
//            defeatFollowNode->setPosition(parentPlayerPosition);
//            
//            _player->getParent()->addChild(defeatFollowNode);
//            _gameCamera->setFollowNode(defeatFollowNode);
//        }
//    }
}

float CameraManager::getMinZoomValue()
{
    if (_gameCamera)
    {
        return _gameCamera->getMinZoomScaleForBoundaries();
    }
    else
    {
        return 0.0f;
    }
}

float CameraManager::getMaxZoomValue()
{
    return ZOOM_VALUE_ON_PLAYER_MAX;
}

float CameraManager::getCurrentZoom()
{
    if (_gameCamera)
    {
        return _gameCamera->getCurrentZoom();
    }
    return 0.0f;
}

Point CameraManager::getCurrentPosition()
{
    if (_gameCamera)
    {
        return _gameCamera->getCurrentPosition();
    }
    return Point::ZERO;
}

Rect CameraManager::getCameraWorldBounds()
{
    if (_gameCamera)
    {
        return _gameCamera->getWorldBoundaries();
    }
    return Rect::ZERO;
}

void CameraManager::cameraInstantFocusOnNode(Node * destinationNode)
{
    if (_gameCamera)
    {
        _gameCamera->setFollowEnabled(false);
        this->setCameraState(eCameraState::ZOOMING_FOCUS);
        _gameCamera->centerCameraOnNode(destinationNode, Point::ZERO);
    }
}

void CameraManager::portalFocusWithCallback(Node * portal, std::function<void()> callback)
{
    if (_gameCamera && _gameCamera->getTarget() && portal->getParent())
    {
        Node * target = _gameCamera->getTarget();
        
        // Calcolo la destinazione di panning
        Point localDestination = portal->getParent()->convertToWorldSpace(portal->getPosition());
        Point panningDestination = target->convertToNodeSpace(localDestination);
        
        this->portalFocusWithCallback(panningDestination, callback);
    }
}

void CameraManager::portalFocusWithCallback(Point destination, std::function<void()> callback)
{
    static const float accelerationFactor = 15.0f;
    
    if (_gameCamera && _gameCamera->getTarget())
    {
        float currentZoom = _gameCamera->getCurrentZoom();
        Rect boundaries = _gameCamera->getWorldBoundaries();
        
        destination = this->getPointFitOnScale(destination, currentZoom, boundaries);
        
        // Effettuo il focus (il panning lo eseguo nello schedule per fare l'effetto di accelerazione)
        _gameCamera->setFollowEnabled(false);
        this->setCameraState(eCameraState::ZOOMING_FOCUS);
        
        // Resetto lo schedule, per sicurezza
        if (this->isScheduled(FOCUS_EVENT_SCHEDULE_CHECK_KEY))
        {
            this->unschedule(FOCUS_EVENT_SCHEDULE_CHECK_KEY);
        }
        
        this->schedule([this, destination, callback](float dt) {
            
            if (destination.distanceSquared(_gameCamera->getCurrentPosition()) < 16.0f)
            {
                // Se sono entro un intorno dalla destinazione, interrompo lo schedule e chiamo la callback
                this->unschedule(FOCUS_EVENT_SCHEDULE_CHECK_KEY);
                callback();
            }
            else
            {
                // Accelero verso la destinazione
                Point currentPanningDest = _gameCamera->getPanningDestination();
                Point direction = destination - currentPanningDest;
                Point nextPanningDest = currentPanningDest + direction.getNormalized() * accelerationFactor;
                if (accelerationFactor * accelerationFactor >= direction.lengthSquared())
                {
                    // Se il passo che farei è superiore o uguale alla distanza che manca, tronco alla destinazione
                    nextPanningDest = destination;
                }
                _gameCamera->panToLocation(nextPanningDest, PANNING_SPEED_FOCUS, true);
            }
        }, FOCUS_EVENT_SCHEDULE_CHECK_KEY);
    }
}

void CameraManager::resetCamera()
{
    this->zoomStandard();
}

#pragma mark - Metodi Privati

void CameraManager::zoomAllScreenStart()
{
    this->zoomAllScreen(ZOOM_SPEED_ALL_SCREEN, PANNING_SPEED_ALL_SCREEN, true, true, eZoomOutType::INSTANT);
}

void CameraManager::zoomAllScreenButton()
{
    this->zoomAllScreen(ZOOM_SPEED_ALL_SCREEN, PANNING_SPEED_ALL_SCREEN, true, true, eZoomOutType::FAST);
}

void CameraManager::zoomAllScreenIdle()
{
    this->zoomAllScreen(ZOOM_ALL_SCREEN_IDLE_ZOOM_SPEED, ZOOM_ALL_SCREEN_IDLE_PANNING_SPEED, true, true, eZoomOutType::IDLE);
}

void CameraManager::zoomStandardStart()
{
    bool instantly = true;
    this->zoomStandard(ZOOM_SPEED_ON_PLAYER, instantly);
//    _gameCamera->centerCameraOnNode(_player, Point::ZERO);
}

void CameraManager::zoomStandard()
{
    bool instantly = false;
    this->zoomStandard(ZOOM_SPEED_ON_PLAYER, instantly);
}

void CameraManager::zoomAllScreen(float zoomSpeed, float panningSpeed, bool zoomSmooth, bool panningSmooth, eZoomOutType type)
{
    if (_gameCamera && !_isLastLevel)
    {
        if (_zoomFocusAction)
        {
            this->stopAction(_zoomFocusAction);
            _zoomFocusAction = nullptr;
        }
        
        Point levelCenter = this->getAllScreenLevelCenter();
   
        _gameCamera->setSoftBoundariesX(false);
        _gameCamera->setSoftBoundariesY(false);
        _gameCamera->setFollowEnabled(false);
        
        switch(type)
        {
            case eZoomOutType::INSTANT:
            {
                _gameCamera->forceZoomToScale(ZOOM_VALUE_ALL_SCREEN);
                _gameCamera->setZoomSpeed(zoomSpeed, zoomSmooth);
                _gameCamera->panToLocation(levelCenter, panningSpeed, panningSmooth);
                this->setCameraState(eCameraState::ZOOMING_OUT_FAST);
                break;
            }
            case eZoomOutType::FAST:
            {
                _gameCamera->panToLocation(levelCenter, panningSpeed, panningSmooth);
                _gameCamera->zoomToScale(ZOOM_VALUE_ALL_SCREEN, zoomSpeed, zoomSmooth);
                this->setCameraState(eCameraState::ZOOMING_OUT_FAST);
                break;
            }
            case eZoomOutType::IDLE:
            {
                if (false == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY))
                {
                    this->schedule([this, levelCenter, panningSpeed, panningSmooth](float dt) {
                        Point currentPanningDest = _gameCamera->getPanningDestination();
                        Point direction = levelCenter - currentPanningDest;
                        Point nextPanningDest = currentPanningDest + direction.getNormalized() * ZOOM_ALL_SCREEN_IDLE_UPDATE_PANNING_SPEED;
                        if (ZOOM_ALL_SCREEN_IDLE_UPDATE_PANNING_SPEED * ZOOM_ALL_SCREEN_IDLE_UPDATE_PANNING_SPEED >= direction.lengthSquared())
                        {
                            // Il passo che farei è superiore o uguale alla distanza che manca, tronco alla destinazione e
                            // interrompo lo schedule
                            nextPanningDest = levelCenter;
                            this->unschedule(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY);
                        }
                        _gameCamera->panToLocation(nextPanningDest, panningSpeed, panningSmooth);
                    }, ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY);
                }
                
                if (false == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY))
                {
                    float zoomDest = _gameCamera->zoomClampToSizeForScale(ZOOM_VALUE_ALL_SCREEN);
                    this->schedule([this, zoomDest, zoomSpeed, zoomSmooth](float dt) {
                        float currentZoomDest = _gameCamera->getZoomDestination();
                        float nextZoomDest = currentZoomDest - ZOOM_ALL_SCREEN_IDLE_UPDATE_ZOOMING_SPEED;
                        if (nextZoomDest < zoomDest)
                        {
                            // Il passo che farei è superiore o uguale alla distanza che manca, tronco alla destinazione e
                            // interrompo lo schedule
                            nextZoomDest = zoomDest;
                            this->unschedule(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY);
                        }
                        _gameCamera->zoomToScale(nextZoomDest, zoomSpeed, zoomSmooth);
                    }, ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY);
                }
                
                this->setCameraState(eCameraState::ZOOMING_OUT_SLOW);
                break;
            }
        }
    }
}

void CameraManager::zoomStandard(float zoomSpeed, bool instantly)
{
    if (_gameCamera)
    {
        this->setCameraState(eCameraState::ZOOMING_IN);
        
        if (_zoomFocusAction)
        {
            this->stopAction(_zoomFocusAction);
            _zoomFocusAction = nullptr;
        }
        
        bool smoothZoom = true;
        
//        this->updateFactors(0.0f);
        
        if (_player)
        {
            _gameCamera->setFollowEnabled(true);
        }
        
        if (instantly)
        {
            this->updateFactors(0.0f);
            
            float zoomValue = ZOOM_VALUE_ON_PLAYER_MAX;
            Point offset = Point::ZERO;
            if (eLevelType::MAIN_MENU != _levelType)
            {
                zoomValue = this->getCurrentZoomYValue();
                offset = Point(0.0f, this->getCurrentOffsetYValue());
            }
            _gameCamera->forceZoomToScale(zoomValue);
            _gameCamera->setZoomSpeed(zoomSpeed, smoothZoom);
            _gameCamera->centerCameraOnNode(_player, offset);
        }
        else
        {
            // Nota: il valore dato in ingresso, nei livelli verrà subito sostituito da quello calcolato in updateYFactors
            // Per cui, nei livelli normali, la seguente istruzione poteva essere sostituita da setZoomSpeed
            _gameCamera->zoomToScale(ZOOM_VALUE_ON_PLAYER_MAX, zoomSpeed, smoothZoom);
        }
        
//        if (_player)
//        {
//            _gameCamera->setFollowEnabled(true);
//        }
        
        _gameCamera->setSoftBoundariesX(true);
        _gameCamera->setSoftBoundariesY(true);
        
//        this->setCameraState(eCameraState::ZOOMING_IN);
    }
}

void CameraManager::startSlowZoomAllScreenTimer()
{
    if (false == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY))
    {
        this->scheduleOnce([this](float dt){
            this->zoomAllScreenIdle();
        }, ZOOM_ALL_SCREEN_IDLE_DELAY, ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY);
    }
}

void CameraManager::resetSlowZoomAllScreenSchedule()
{
    if (true == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY))
    {
        this->unschedule(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY);
    }
    if (true == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY))
    {
        this->unschedule(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY);
    }
    if (true == this->isScheduled(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY))
    {
        this->unschedule(ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY);
    }
}

void CameraManager::cameraEventDoor(Ref * door, void * options)
{
    if (_cameraState == eCameraState::ZOOMING_FOCUS) return;
    
    Door * doorObj = dynamic_cast<Door *>(door);
    if (_gameCamera && doorObj && _player)
    {
        // Se il focus avviene una sola volta, rimuovo il CameraManager dagli osservatori
        if (eFocusMode::ONE_SHOT == FOCUS_EVENT_DOOR_MODE)
        {
            if (doorObj->getType() == eDoorType::AND_DOOR)
            {
                doorObj->removeObserver(this);
            }
            else
            {
                Ref * doorSwitch = static_cast<Ref *>(options);
                auto iter = _pressedSwitchesOR.find(doorSwitch);
                if (iter == _pressedSwitchesOR.end())
                {
                    _pressedSwitchesOR.insert(doorSwitch);
                }
                else
                {
                    return;
                }
            }
        }
        else if (eFocusMode::COOLDOWN == FOCUS_EVENT_DOOR_MODE)
        {
            Switch * doorSwitch = static_cast<Switch *>(options);
            std::string scheduleId = "cameraFocusCooldownTimerDoor";
            if (false == doorSwitch->isScheduled(scheduleId))
            {
                doorSwitch->scheduleOnce(nullptr, FOCUS_EVENT_DOOR_COOLDOWN, scheduleId);
            }
            else
            {
                return;
            }
        }
        
        Size visibleSize = Director::getInstance()->getVisibleSize();
        
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        Point doorOnWorld = doorObj->getParent()->convertToWorldSpace(doorObj->getPosition());
        Point doorOnTarget = _gameCamera->getTarget()->convertToNodeSpace(doorOnWorld);
        
        Point panningDestination = playerOnTarget + ( doorOnTarget - playerOnTarget ) / 2.0f;
        
        float frameMarginHorizontal = 400.0f; // somma dei margini destro e sinitro tra Door / Player e bordi dello schermo
        float zoomScale = visibleSize.width / (fabsf(doorOnTarget.x - playerOnTarget.x) + frameMarginHorizontal);
        
        zoomScale = fmaxf(fminf(zoomScale, ZOOM_VALUE_ON_PLAYER_MAX - 0.1f), ZOOM_VALUE_FOCUS_MIN);
        
        _gameCamera->setFollowEnabled(false);
        _gameCamera->zoomToScale(zoomScale, ZOOM_SPEED_FOCUS, true);
        _gameCamera->panToLocation(panningDestination, PANNING_SPEED_FOCUS, true);
        
        this->setCameraState(eCameraState::ZOOMING_FOCUS);
        
        CallFunc * delayedReset = CallFunc::create([this]()
        {
            this->zoomStandard();
            _zoomFocusAction = nullptr;
        });
        
        _zoomFocusAction = Sequence::create(DelayTime::create(2.0f), delayedReset, NULL);
        this->runAction(_zoomFocusAction);
    }
}

void CameraManager::cameraEventInteractive(Ref * interactive)
{
    if (_cameraState == eCameraState::ZOOMING_FOCUS) return;
    
    InteractiveBlock * interactiveObj = dynamic_cast<InteractiveBlock *>(interactive);
    if (_gameCamera && interactiveObj && _player)
    {
        // Se il focus avviene una sola volta, rimuovo il CameraManager dagli osservatori
        if (eFocusMode::ONE_SHOT == FOCUS_EVENT_INTERACTIVE_MODE)
        {
            interactiveObj->removeObserver(this);
        }
        else if (eFocusMode::COOLDOWN == FOCUS_EVENT_INTERACTIVE_MODE)
        {
            std::string scheduleId = "cameraFocusCooldownTimerInteractive";
            if (false == interactiveObj->isScheduled(scheduleId))
            {
                interactiveObj->scheduleOnce(nullptr, FOCUS_EVENT_INTERACTIVE_COOLDOWN, scheduleId);
            }
            else
            {
                return;
            }
        }
        
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        float distanceToFollow = 200.0f;
        
        float panningDestinationX = playerOnTarget.x;
        float panningDestinationY = playerOnTarget.y - distanceToFollow;
        
        Point panningDestination = Point(panningDestinationX, panningDestinationY);
        
        float zoomScale = fmax(ZOOM_VALUE_ON_PLAYER_MAX - 0.1f, ZOOM_VALUE_FOCUS_MIN);
        
        _gameCamera->setFollowEnabled(false);
        _gameCamera->zoomToScale(zoomScale, ZOOM_SPEED_FOCUS, true);
        _gameCamera->panToLocation(panningDestination, PANNING_SPEED_FOCUS, true);
        
        this->setCameraState(eCameraState::ZOOMING_FOCUS);
        
        CallFunc * delayedReset = CallFunc::create([this]()
        {
            this->zoomStandard();
            _zoomFocusAction = nullptr;
        });
        
        _zoomFocusAction = Sequence::create(DelayTime::create(2.0f), delayedReset, NULL);
        this->runAction(_zoomFocusAction);
    }
}

void CameraManager::cameraEventFocus(CameraFocus * cameraFocus)
{
    if (_cameraState == eCameraState::ZOOMING_FOCUS) return;
    
    if (_gameCamera && _gameCamera->getTarget() && cameraFocus)
    {
        // Carico i dati da level helper
        float duration = cameraFocus->getDuration();
        float panningSpeed = cameraFocus->getPanningSpeed();
        float zoomingSpeed = cameraFocus->getZoomingSpeed();
        std::string fitMode = cameraFocus->getFitMode();
        Node * targetNode = cameraFocus->getTargetNode();
        Node * boundsNode = cameraFocus->getBoundsNode();
        bool isOneShot = cameraFocus->getIsOneShot();
        
        // Se il focus è one-shot, disabilito il nodo
        if (isOneShot)
        {
            LHPhysicsProtocol * physics = dynamic_cast<LHPhysicsProtocol *>(cameraFocus->getNode());
            physics->getBox2dBody()->SetActive(false);
        }
        
        // Calcolo la destinazione di zoom
        float zoomDestination = 0.0f;
        if (fitMode.compare("width") == 0)
        {
            zoomDestination = _gameCamera->getMinZoomScaleForSize(Size(targetNode->getContentSize().width, FLT_MAX));
        }
        else if (fitMode.compare("height") == 0)
        {
            zoomDestination = _gameCamera->getMinZoomScaleForSize(Size(FLT_MAX, targetNode->getContentSize().height));
        }
        else
        {
            zoomDestination = _gameCamera->getMinZoomScaleForSize(targetNode->getContentSize());
        }
        
        // Calcolo la destinazione di panning
        Point localDestination = targetNode->getParent()->convertToWorldSpace(targetNode->getPosition());
        Point panningDestination = _gameCamera->getTarget()->convertToNodeSpace(localDestination);
        panningDestination = this->getPointFitOnScale(panningDestination, zoomDestination, _gameCamera->getWorldBoundaries());
        
        // Effettuo il focus
        _gameCamera->setFollowEnabled(false);
        _gameCamera->zoomToScale(zoomDestination, zoomingSpeed, true);
        _gameCamera->panToLocation(panningDestination, panningSpeed, true);
        this->setCameraState(eCameraState::ZOOMING_FOCUS);
        
        // Sezione di schedule per far terminare il focus dopo la durata, o prima nel caso in cui il player esca dall'area
        // definita dal nodo bounds
        if (this->isScheduled(FOCUS_EVENT_SCHEDULE_CHECK_KEY))
        {
            this->unschedule(FOCUS_EVENT_SCHEDULE_CHECK_KEY);
        }
        
        _zoomFocusTimer = duration;
        Rect screenBounds = Utility::convertRectToWorldSpace(boundsNode->getParent(), boundsNode->getBoundingBox());
        Rect playerBounds = Utility::convertRectToNodeSpace(_player->getParent(), screenBounds);
        
        this->schedule([this, playerBounds](float dt) {
            _zoomFocusTimer -= 0.016f;
            if (_zoomFocusTimer <= 0.0f || false == playerBounds.containsPoint(_player->getPosition()))
            {
                this->unschedule(FOCUS_EVENT_SCHEDULE_CHECK_KEY);
                this->zoomStandard();
            }
        }, FOCUS_EVENT_SCHEDULE_CHECK_KEY);
    }
}

#pragma mark - Update

void CameraManager::update(float dt)
{
    if (_player)
    {
        this->updateFactors(dt);
        
        // Update Camera
        if (_gameCamera)
        {
            _gameCamera->update(dt);
        }
        
        // Update Draw Debug
        if (DEBUG_CAMERA_MODE_ON)
        {
            this->updateDebugDrawTrap();
            this->updateDebugDrawLevelCenter();
            this->updateDebugDrawCameraThresholds();
        }
    }
}

void CameraManager::updateFactors(float dt)
{
    bool dirtyFollow = false;
    switch (_levelType)
    {
        case eLevelType::NORMAL:
            dirtyFollow = this->updateCurrentCameraThreshold();
            dirtyFollow = this->updateCurrentCameraZone();
            this->updateXFactors(dt);
            this->updateYFactors(dt);
            this->updateAllScreenIdle(dt);
            break;
        case eLevelType::MAIN_MENU:
            dirtyFollow = this->updateCurrentCameraThreshold();
            dirtyFollow = this->updateCurrentCameraZone();
            this->updateXFactors(dt);
            break;
        case eLevelType::LEVEL_SELECTION:
            dirtyFollow = this->updateCurrentCameraThreshold();
            dirtyFollow = this->updateCurrentCameraZone();
            this->updateXFactors(dt);
            this->updateYFactors(dt);
            break;
        default:
            break;
    }
    
    if (dirtyFollow && _gameCamera)
    {
        _gameCamera->resetFollow();
    }
}

void CameraManager::updateAllScreenIdle(float dt)
{
    if (_player)
    {
        bool isZoomIn = (eCameraState::ZOOMING_IN == _cameraState);
        bool isPlayerStandingState = (nullptr != dynamic_cast<PlayerStandingState *>(_player->getPlayerState()));
        bool isNotOnMovable = (nullptr == _player->getMovablePlatform());
        
        if (isZoomIn && isPlayerStandingState && isNotOnMovable)
        {
            this->startSlowZoomAllScreenTimer();
        }
    }
}

void CameraManager::updateXFactors(float dt)
{
    this->updateFollowOffsetSide();
}

void CameraManager::updateYFactors(float dt)
{
    if (_cameraState == eCameraState::ZOOMING_IN)
    {
        if (_player)
        {
            float newZoomY = this->getCurrentZoomYValue();
            float newFollowY = this->getCurrentOffsetYValue();
            
            _gameCamera->setZoomScale(newZoomY);
            _gameCamera->setFollowedNodeOffset(Point(_gameCamera->getFollowedNodeOffset().x, newFollowY));
            
            if(DEBUG_CAMERA_MODE_ON)
            {
                UPDATE_LOG(CCLOG("Follow offset Y: %f", newFollowY), timerFollow, dt, 1.0f);
                
                Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
                Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
                UPDATE_LOG(CCLOG("Player position Y: %f", playerOnTarget.y), timerPlayer, dt, 1.0f);
            }
        }
    }
}

void CameraManager::updateFollowOffsetSide()
{
    if (_cameraState == eCameraState::ZOOMING_IN)
    {
        if (_player)
        {
            if (_player->getPosition().x < _trapLastPositionLeft)
            {
                _trapLastPositionLeft = _player->getPosition().x;
                _trapLastPositionRight = _trapLastPositionLeft + FOLLOW_TRAP_WIDTH;
                
                // Interrompo il movimento a destra se è in corso
                if (this->isScheduled(FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY))
                {
                    this->unschedule(FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY);
                }
                // Se non sono già all'estremo sinistro e non ho già schedulato il movimento a sinistra, lo schedulo adesso
                if (_gameCamera->getFollowedNodeOffset().x > FOLLOW_OFFSET_X_MIN && !this->isScheduled(FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY) )
                {
                    this->schedule([this](float dt) {
                        Point newOffset = _gameCamera->getFollowedNodeOffset() - Point(FOLLOW_TRAP_UPDATE_SPEED,0);
                        if (newOffset.x < FOLLOW_OFFSET_X_MIN)
                        {
                            // Se ho superato il limite, tronco al limite e interrompo lo schedule
                            newOffset = Point(FOLLOW_OFFSET_X_MIN, newOffset.y);
                            this->unschedule(FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY);
                        }
                        _gameCamera->setFollowedNodeOffset(newOffset);
                    }, FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY);
                }
            }
            else if (_player->getPosition().x > _trapLastPositionRight)
            {
                _trapLastPositionRight = _player->getPosition().x;
                _trapLastPositionLeft = _trapLastPositionRight - FOLLOW_TRAP_WIDTH;
                
                // Interrompo il movimento a sinistra se è in corso
                if (this->isScheduled(FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY))
                {
                    this->unschedule(FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY);
                }
                // Se non sono già all'estremo destro e non ho già schedulato il movimento a destra, lo schedulo adesso
                if (_gameCamera->getFollowedNodeOffset().x < FOLLOW_OFFSET_X_MAX && !this->isScheduled(FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY) )
                {
                    this->schedule([this](float dt) {
                        Point newOffset = _gameCamera->getFollowedNodeOffset() + Point(FOLLOW_TRAP_UPDATE_SPEED,0);
                        if (newOffset.x > FOLLOW_OFFSET_X_MAX)
                        {
                            // Se ho superato il limite, tronco al limite e interrompo lo schedule
                            newOffset = Point(FOLLOW_OFFSET_X_MAX, newOffset.y);
                            this->unschedule(FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY);
                        }
                        _gameCamera->setFollowedNodeOffset(newOffset);
                    }, FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY);
                }
            }
        }
    }
}

bool CameraManager::updateCurrentCameraThreshold()
{
    bool hasThresholdParamsChanged = false;
    
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        LHNode * newCameraThreshold = nullptr;
        auto iter = _cameraThresholds.begin();
        while (iter != _cameraThresholds.end() && nullptr == newCameraThreshold)
        {
            if (playerOnTarget.x > (*iter)->getPosition().x && playerOnTarget.y > (*iter)->getPosition().y)
            {
                newCameraThreshold = (*iter);
            }
            iter++;
        }
        
        if (_currentCameraThreshold != newCameraThreshold)
        {
            if (_currentCameraThreshold && newCameraThreshold)
            {
                CameraThreshold * infoOld = dynamic_cast<CameraThreshold *>(_currentCameraThreshold->getUserProperty());
                CameraThreshold * infoNew = dynamic_cast<CameraThreshold *>(newCameraThreshold->getUserProperty());
                if (infoOld && infoNew && _gameCamera)
                {
                    if (infoOld->getZoomLowerDeltaY() != infoNew->getZoomLowerDeltaY() ||
                        infoOld->getZoomUpperDeltaY() != infoNew->getZoomUpperDeltaY() ||
                        infoOld->getFollowOffsetLowerDeltaY() != infoNew->getFollowOffsetLowerDeltaY() ||
                        infoOld->getFollowOffsetUpperDeltaY() != infoNew->getFollowOffsetUpperDeltaY())
                    {
                        hasThresholdParamsChanged = true;
                    }
                }
            }
            else
            {
                hasThresholdParamsChanged = true;
            }
            
            _currentCameraThreshold = newCameraThreshold;
            
            if (DEBUG_CAMERA_MODE_ON)
            {
                CCLOG("Cambio di THRESHOLD");
            }
        }
    return hasThresholdParamsChanged;
}

bool CameraManager::updateCurrentCameraZone()
{
    bool hasZoneChanged = false;
    
    Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
    Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
    
    Zone * newCameraZone = nullptr;
    auto iter = _cameraZones.begin();
    while (iter != _cameraZones.end() && nullptr == newCameraZone)
    {
        Zone * zone = (*iter).get();
        if (zone->areas.size() == 0)
        {
            newCameraZone = zone;
        }
        else
        {
            for (Rect area : zone->areas)
            {
                if (area.containsPoint(playerOnTarget))
                {
                    newCameraZone = zone;
                }
            }
        }
        iter++;
    }
    
    if (_currentZone != newCameraZone)
    {
        hasZoneChanged = true;
        
        _currentZone = newCameraZone;
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            CCLOG("Cambio di ZONA");
            this->updateDebugDrawZoneAreas();
        }
    }
    return hasZoneChanged;
}

#pragma mark - Observer Pattern

void CameraManager::update(eEventType event, Ref * sender, void * options)
{
    switch (event) {
        case eEventType::DOOR_OPEN:
        case eEventType::DOOR_SHAKE:
            this->cameraEventDoor(sender, options);
            break;
        case eEventType::INTERACTIVE_FALL:
            this->cameraEventInteractive(sender);
            break;
        default:
            break;
    }
}

#pragma mark - Utilities

void CameraManager::setCameraState(eCameraState newState)
{
    switch (_cameraState)
    {
        case eCameraState::ZOOMING_IN:
            _gameCamera->setAttractorsEnabled(false);
            _gameCamera->setRailsEnabled(false);
        case eCameraState::READY:
        case eCameraState::ZOOMING_OUT_SLOW:
        case eCameraState::ZOOMING_OUT_FAST:
        case eCameraState::ZOOMING_FOCUS:
        default:
            this->unschedule(FOCUS_EVENT_SCHEDULE_CHECK_KEY);
            break;
    }
    
    _cameraState = newState;
    
    switch (newState)
    {
        case eCameraState::ZOOMING_OUT_SLOW:
            break;
        case eCameraState::ZOOMING_IN:
            _gameCamera->setAttractorsEnabled(true);
            _gameCamera->setRailsEnabled(true);
        case eCameraState::ZOOMING_FOCUS:
        case eCameraState::READY:
        case eCameraState::ZOOMING_OUT_FAST:
        default:
            this->resetSlowZoomAllScreenSchedule();
            break;
    }
}

float CameraManager::getCurrentZoomYValue()
{
    float currentZoomY = 0.0f;
    if (_player)
    {
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        bool hasZoneZoom = this->getCurrentZoomYValueFromZones(playerOnTarget, &currentZoomY);
        if (false == hasZoneZoom)
        {
            currentZoomY = this->getCurrentZoomYValueFromBounds(playerOnTarget);
        }
    }
    return currentZoomY;
}

bool CameraManager::getCurrentZoomYValueFromZones(Point playerOnTarget, float * zoom)
{
    bool res = false;
    if (_currentZone && _gameCamera && _gameCamera->getTarget() && _player)
    {
        float alpha = (playerOnTarget.y-_currentZone->lowerBound)/(_currentZone->upperBound-_currentZone->lowerBound);
        alpha = fminf(1.0f, fmax(0.0f, alpha));
        
        *zoom =(_currentZone->upperZoom-_currentZone->lowerZoom) * alpha + _currentZone->lowerZoom;
        res = true;
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            UPDATE_LOG(CCLOG("zoneOffsetLowerBound: %f, zoneOffsetUpperBound: %f", _currentZone->lowerBound, _currentZone->upperBound), timer, 0.1f, 1.0f);
            this->updateDebugDrawZoneBounds(Rect(0.0f, _currentZone->lowerBound, 0.0f, _currentZone->upperBound));
        }
    }
    return res;
}

float CameraManager::getCurrentZoomYValueFromBounds(Point playerOnTarget)
{
    float currentZoom = 0.0f;
    if (_gameCamera)
    {
        Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
        
        Rect delta = this->getCurrentCustomZoomYBoundsDelta();
        
        float valueYCurrent = playerOnTarget.y;
        float valueYMin = cameraWorldBoundaries.origin.y + VERTICAL_ZOOM_LOWER_BOUND + delta.origin.y;
        float valueYMax = cameraWorldBoundaries.origin.y + VERTICAL_ZOOM_UPPER_BOUND + delta.size.height;
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            UPDATE_LOG(CCLOG("zoomLowerBound: %f, zoomUpperBound: %f", valueYMin, valueYMax), timer, 0.1f, 1.0f);
            this->updateDebugDrawZoomInBounds(Rect(0.0f, valueYMin, 0.0f, valueYMax));
        }
        
        float positionYFactor = fmaxf(0.0f, fminf(1.0f, (valueYCurrent - valueYMin) / (valueYMax - valueYMin)));
        
        currentZoom = (ZOOM_VALUE_ON_PLAYER_MIN - ZOOM_VALUE_ON_PLAYER_MAX) * positionYFactor + ZOOM_VALUE_ON_PLAYER_MAX;
    }
    return currentZoom;
}

float CameraManager::getCurrentOffsetYValue()
{
    float currentOffsetY = 0.0f;
    if (_player && _gameCamera)
    {
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        bool hasZoneOffset = this->getCurrentOffsetYValueFromZones(playerOnTarget, &currentOffsetY);
        if (false == hasZoneOffset)
        {
            currentOffsetY = this->getCurrentOffsetYValueFromBounds(playerOnTarget);
        }
    }
    return currentOffsetY;
}

bool CameraManager::getCurrentOffsetYValueFromZones(Point playerOnTarget, float * offset)
{
    bool res = false;
    if (_currentZone && _gameCamera && _gameCamera->getTarget() && _player)
    {
        float alpha = (playerOnTarget.y-_currentZone->lowerBound)/(_currentZone->upperBound-_currentZone->lowerBound);
        alpha = fminf(1.0f, fmax(0.0f, alpha));
        
        float currentOffset = (_currentZone->upperOffset-_currentZone->lowerOffset)*alpha+_currentZone->lowerOffset;
        Point screenOffset = _gameCamera->getTarget()->convertToWorldSpace(Point(0.0f, currentOffset));
        *offset =_player->convertToNodeSpace(screenOffset).y - _player->getContentSize().height / 2.0f;;
        res = true;
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            UPDATE_LOG(CCLOG("zoneOffsetLowerBound: %f, zoneOffsetUpperBound: %f", _currentZone->lowerOffset, _currentZone->upperOffset), timer, 0.1f, 1.0f);
            this->updateDebugDrawFollowOffsetBounds(Rect(0.0f, _currentZone->lowerOffset, 0.0f, _currentZone->upperOffset));
        }
    }
    return res;
}

float CameraManager::getCurrentOffsetYValueFromBounds(Point playerOnTarget)
{
    float offset = 0.0f;
    if (_gameCamera)
    {
        Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
        
        Rect delta = this->getCurrentCustomOffsetYBoundsDelta();
        
        float valueYCurrent = playerOnTarget.y;
        float valueYMin = cameraWorldBoundaries.origin.y + VERTICAL_FOLLOW_OFFSET_LOWER_BOUND + delta.origin.y;
        float valueYMax = cameraWorldBoundaries.origin.y + VERTICAL_FOLLOW_OFFSET_UPPER_BOUND + delta.size.height;
        
        if (DEBUG_CAMERA_MODE_ON)
        {
            UPDATE_LOG(CCLOG("offsetLowerBound: %f, offsetUpperBound: %f", valueYMin, valueYMax), timer, 0.1f, 1.0f);
            this->updateDebugDrawFollowOffsetBounds(Rect(0.0f, valueYMin, 0.0f, valueYMax));
        }
        
        float positionYFactor = fmaxf(0.0f, fminf(1.0f, (valueYCurrent - valueYMin) / (valueYMax - valueYMin)));
        
        offset = (FOLLOW_OFFSET_Y_MIN - FOLLOW_OFFSET_Y_MAX) * positionYFactor + FOLLOW_OFFSET_Y_MAX;
    }
    return offset;
}

Rect CameraManager::getCurrentCustomZoomYBoundsDelta()
{
    if (_player && _gameCamera)
    {
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        float lowerDelta = 0;
        float upperDelta = 0;
        
        if (_currentCameraThreshold)
        {
            CameraThreshold * info = dynamic_cast<CameraThreshold *>(_currentCameraThreshold->getUserProperty());
            if (info)
            {
                lowerDelta = info->getZoomLowerDeltaY();
                upperDelta = info->getZoomUpperDeltaY();
            }

        }
        return Rect(0.0f, lowerDelta, 0.0f, upperDelta);
    }
    else
    {
        return Rect::ZERO;
    }
}

Rect CameraManager::getCurrentCustomOffsetYBoundsDelta()
{
    if (_player && _gameCamera)
    {
        Point playerOnWorld = _player->getParent()->convertToWorldSpace(_player->getPosition());
        Point playerOnTarget = _gameCamera->getTarget()->convertToNodeSpace(playerOnWorld);
        
        float lowerDelta = 0;
        float upperDelta = 0;
        
        if (_currentCameraThreshold)
        {
            CameraThreshold * info = dynamic_cast<CameraThreshold *>(_currentCameraThreshold->getUserProperty());
            if (info)
            {
                lowerDelta = info->getFollowOffsetLowerDeltaY();
                upperDelta = info->getFollowOffsetUpperDeltaY();
            }
            
        }
        
        return Rect(0.0f, lowerDelta, 0.0f, upperDelta);
    }
    else
    {
        return Rect::ZERO;
    }
}

Point CameraManager::getAllScreenLevelCenter()
{
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
    float levelToScreenHeightRatio = cameraWorldBoundaries.size.height / visibleSize.height;
    float levelToScreenWidthRatio = cameraWorldBoundaries.size.width / visibleSize.width;
    float panningDestinationX;
    float panningDestinationY;
    
    if (levelToScreenHeightRatio > levelToScreenWidthRatio)
    {
        // Se avanza spazio verticale, restituisco il punto più in basso
        panningDestinationX = cameraWorldBoundaries.origin.x + cameraWorldBoundaries.size.width / 2.0f;
        panningDestinationY = cameraWorldBoundaries.origin.y + ( ( cameraWorldBoundaries.size.width * visibleSize.height ) / visibleSize.width ) / 2.0f;
    }
    else
    {
        // Se avanza spazio orizzontale, restituisco il punto più a sinistra
        panningDestinationX = cameraWorldBoundaries.origin.x + ( ( cameraWorldBoundaries.size.height * visibleSize.width ) / visibleSize.height ) / 2.0f;
        panningDestinationY = cameraWorldBoundaries.origin.y + cameraWorldBoundaries.size.height / 2.0f;
    }
    return Point(panningDestinationX, panningDestinationY);
}

Point CameraManager::getPointFitOnScale(Point point, float scale, Rect bounds)
{
    Size screenSize = VisibleRect::getVisibleRect().size;
    
    float minX = bounds.origin.x + screenSize.width / 2.0f / scale;
    float maxX = bounds.origin.x + bounds.size.width - screenSize.width / 2.0f / scale;
    float clampedX = fminf(maxX, fmaxf(minX, point.x));
    
    float minY = bounds.origin.y + screenSize.height / 2.0f / scale;
    float maxY = bounds.origin.y + bounds.size.height - screenSize.height / 2.0f / scale;
    float clampedY = fminf(maxY, fmaxf(minY, point.y));
    
    return Point(clampedX, clampedY);
}

#pragma mark - Draw Debug

void CameraManager::updateDebugDrawTrap()
{
    if (_targetNode && _player)
    {
        std::string drawNodeName = "cameraManagerTrappingArea";
        DrawNode * drawNode = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeName));
        if (nullptr == drawNode)
        {
            drawNode = DrawNode::create();
            _targetNode->addChild(drawNode, 100, drawNodeName);
        }
        drawNode->clear();
        Point trapOrigin = Point(_trapLastPositionLeft, _player->getPosition().y);
        Point trapDestination = Point(_trapLastPositionRight, _player->getPosition().y);
        
        trapOrigin = _targetNode->convertToNodeSpace(_player->getParent()->convertToWorldSpace(trapOrigin));
        trapDestination = _targetNode->convertToNodeSpace(_player->getParent()->convertToWorldSpace(trapDestination));
        
        drawNode->drawLine(trapOrigin, trapDestination, Color4F(0, 0, 1, 1));
    }
}

void CameraManager::updateDebugDrawLevelCenter()
{
    if (_targetNode)
    {
        std::string drawNodeName = "cameraManagerLevelCenter";
        DrawNode * drawNode = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeName));
        if (nullptr == drawNode)
        {
            drawNode = DrawNode::create();
            _targetNode->addChild(drawNode, 100, drawNodeName);
        }
        drawNode->clear();
        drawNode->drawDot(this->getAllScreenLevelCenter(), 2.0f, Color4F(1, 1, 1, 1));
    }
}

void CameraManager::updateDebugDrawCameraThresholds()
{
    if (_targetNode)
    {
        Point upperRightCorner = Point(_gameCamera->getWorldBoundaries().getMaxX(),_gameCamera->getWorldBoundaries().getMaxY());
        Color4F rectColor = Color4F(1, 1, 1, 0.3f);
        
        for (int i = 0; i < _cameraThresholds.size(); i++)
        {
            char drawNodeName[70];
            snprintf(drawNodeName, 70, "cameraManagerCustomThreshold%d", i);
            DrawNode * drawNode = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeName));
            if (nullptr == drawNode)
            {
                drawNode = DrawNode::create();
                _targetNode->addChild(drawNode, 100, drawNodeName);
            }
            drawNode->clear();
            drawNode->drawRect(_cameraThresholds[i]->getPosition(), upperRightCorner, rectColor);
        }
    }
}

void CameraManager::updateDebugDrawZoomInBounds(Rect bounds)
{
    if (_targetNode)
    {
        Point lineOrigin;
        Point lineDestination;
        
        Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
        
        std::string drawNodeLowerName = "cameraManagerZoomInBoundLower";
        std::string drawNodeUpperName = "cameraManagerZoomInBoundUpper";
        Color4F drawColor = Color4F::YELLOW;
        
        DrawNode * drawNodeLower = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeLowerName));
        if (nullptr == drawNodeLower)
        {
            drawNodeLower = DrawNode::create();
            _targetNode->addChild(drawNodeLower, 100, drawNodeLowerName);
        }
        drawNodeLower->clear();
        
        DrawNode * drawNodeUpper = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeUpperName));
        if (nullptr == drawNodeUpper)
        {
            drawNodeUpper = DrawNode::create();
            _targetNode->addChild(drawNodeUpper, 100, drawNodeUpperName);
        }
        drawNodeUpper->clear();
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.origin.y);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.origin.y);
        drawNodeLower->drawLine(lineOrigin, lineDestination, drawColor);
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.size.height);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.size.height);
        drawNodeUpper->drawLine(lineOrigin, lineDestination, drawColor);
    }
}

void CameraManager::updateDebugDrawFollowOffsetBounds(Rect bounds)
{
    if (_targetNode)
    {
        Point lineOrigin;
        Point lineDestination;
        
        Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
        
        std::string drawNodeLowerName = "cameraManagerFollowOffsetBoundLower";
        std::string drawNodeUpperName = "cameraManagerFollowOffsetBoundUpper";
        Color4F drawColor = Color4F::GREEN;
        
        DrawNode * drawNodeLower = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeLowerName));
        if (nullptr == drawNodeLower)
        {
            drawNodeLower = DrawNode::create();
            _targetNode->addChild(drawNodeLower, 100, drawNodeLowerName);
        }
        drawNodeLower->clear();
        
        DrawNode * drawNodeUpper = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeUpperName));
        if (nullptr == drawNodeUpper)
        {
            drawNodeUpper = DrawNode::create();
            _targetNode->addChild(drawNodeUpper, 100, drawNodeUpperName);
        }
        drawNodeUpper->clear();
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.origin.y);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.origin.y);
        drawNodeLower->drawLine(lineOrigin, lineDestination, drawColor);
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.size.height);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.size.height);
        drawNodeUpper->drawLine(lineOrigin, lineDestination, drawColor);
    }
}

void CameraManager::updateDebugDrawZoneAreas()
{
    if (_targetNode)
    {
        for (int i = 0; i < _cameraZones.size(); i++)
        {
            Zone * zone = _cameraZones.at(i).get();
            
            if (zone->areas.size() > 0)
            {
                std::stringstream drawNodeAreaName;
                drawNodeAreaName << "cameraManagerZoneArea" << "_" << i;
                Color4F drawColor = (zone == _currentZone) ? Color4F::RED : Color4F::BLUE;
                
                DrawNode * drawNodeArea = _targetNode->getChildByName<DrawNode*>(drawNodeAreaName.str());
                if (nullptr == drawNodeArea)
                {
                    drawNodeArea = DrawNode::create();
                    _targetNode->addChild(drawNodeArea, 100, drawNodeAreaName.str());
                }
                drawNodeArea->clear();
                
                Rect area = zone->areas.at(0);
                drawNodeArea->drawRect(area.origin, area.origin + Point(area.size), drawColor);
            }
        }
    }
}

void CameraManager::updateDebugDrawZoneBounds(Rect bounds)
{
    if (_targetNode)
    {
        Point lineOrigin;
        Point lineDestination;
        
        Rect cameraWorldBoundaries = _gameCamera->getWorldBoundaries();
        
        std::string drawNodeLowerName = "cameraManagerZoneBoundLower";
        std::string drawNodeUpperName = "cameraManagerZoneBoundUpper";
        Color4F drawColor = Color4F::MAGENTA;
        
        DrawNode * drawNodeLower = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeLowerName));
        if (nullptr == drawNodeLower)
        {
            drawNodeLower = DrawNode::create();
            _targetNode->addChild(drawNodeLower, 100, drawNodeLowerName);
        }
        drawNodeLower->clear();
        
        DrawNode * drawNodeUpper = dynamic_cast<DrawNode *>(_targetNode->getChildByName(drawNodeUpperName));
        if (nullptr == drawNodeUpper)
        {
            drawNodeUpper = DrawNode::create();
            _targetNode->addChild(drawNodeUpper, 100, drawNodeUpperName);
        }
        drawNodeUpper->clear();
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.origin.y);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.origin.y);
        drawNodeLower->drawLine(lineOrigin, lineDestination, drawColor);
        
        lineOrigin = Point(cameraWorldBoundaries.getMinX(), bounds.size.height);
        lineDestination = Point(cameraWorldBoundaries.getMaxX(), bounds.size.height);
        drawNodeUpper->drawLine(lineOrigin, lineDestination, drawColor);
    }
}