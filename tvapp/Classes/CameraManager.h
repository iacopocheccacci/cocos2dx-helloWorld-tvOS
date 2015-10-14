//
//  CameraManager.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 09/03/15.
//
//

#ifndef __SongOfPan__CameraManager__
#define __SongOfPan__CameraManager__

#include "cocos2d.h"
#include "Observer.h"

USING_NS_CC;

class LHScene;
class GameCamera;
class Player;
class LHNode;
class CameraFocus;

enum class eCameraState;
enum class eLevelType;
enum class eFocusMode;
enum class eZoomOutType;

class CameraManager : public Node, public Observer {
    
public:
    
    CameraManager();
    virtual ~CameraManager();
    
    static CameraManager * create(LHScene * gameScene, Node * targetNode, Player * player, std::string levelName);
    bool init(LHScene * gameScene, Node * targetNode, Player * player, std::string levelName);
    
    void zoomToggle();
    void zoomResume();
    void handleCameraEvent(LHNode * triggerNode);
    void handleFluteTaken();
    void handleDefeat();
    
    float getMinZoomValue();
    float getMaxZoomValue();
    float getCurrentZoom();
    Point getCurrentPosition();
    
    Rect getCameraWorldBounds();

    void cameraInstantFocusOnNode(Node * destinationNode);
    void portalFocusWithCallback(Node * portal, std::function<void()> callback);
    void portalFocusWithCallback(Point destination, std::function<void()> callback);
    
    void resetCamera();
    
private:
    
    struct Zone
    {
        Zone(std::vector<Rect> areas, float lowerOffset, float lowerZoom, float lowerBound, float upperOffset, float upperZoom, float upperBound)
        : areas(areas)
        , lowerOffset(lowerOffset)
        , lowerZoom(lowerZoom)
        , lowerBound(lowerBound)
        , upperOffset(upperOffset)
        , upperZoom(upperZoom)
        , upperBound(upperBound)
        {}
        
        std::vector<Rect> areas;
        float lowerOffset;
        float lowerZoom;
        float lowerBound;
        float upperOffset;
        float upperZoom;
        float upperBound;
    };
    
    static const float ZOOM_VALUE_ON_PLAYER_MAX;
    static const float ZOOM_VALUE_ON_PLAYER_MIN;
    static const float ZOOM_VALUE_FOCUS_MIN;
    static const float ZOOM_VALUE_ALL_SCREEN;
    
    static const float ZOOM_SPEED_ALL_SCREEN;
    static const float ZOOM_SPEED_ON_PLAYER;
    static const float ZOOM_SPEED_FOCUS;
    
    static const float PANNING_SPEED_ALL_SCREEN;
    static const float PANNING_SPEED_FOCUS;
    
    static const float FOLLOW_TRAP_WIDTH;
    static const float FOLLOW_SPEED;
    static const float FOLLOW_OFFSET_X_MIN;
    static const float FOLLOW_OFFSET_X_MAX;
    static const float FOLLOW_OFFSET_Y_MIN;
    static const float FOLLOW_OFFSET_Y_MAX;
    static const float FOLLOW_TRAP_UPDATE_SPEED;
    static const char* FOLLOW_TRAP_SCHEDULE_MOVE_LEFT_KEY;
    static const char* FOLLOW_TRAP_SCHEDULE_MOVE_RIGHT_KEY;
    
    static const char* ZOOM_ALL_SCREEN_IDLE_SCHEDULE_DELAY_KEY;
    static const char* ZOOM_ALL_SCREEN_IDLE_SCHEDULE_MOVE_KEY;
    static const char* ZOOM_ALL_SCREEN_IDLE_SCHEDULE_ZOOM_KEY;
    static const float ZOOM_ALL_SCREEN_IDLE_DELAY;
    static const float ZOOM_ALL_SCREEN_IDLE_UPDATE_PANNING_SPEED;
    static const float ZOOM_ALL_SCREEN_IDLE_UPDATE_ZOOMING_SPEED;
    static const float ZOOM_ALL_SCREEN_IDLE_ZOOM_SPEED;
    static const float ZOOM_ALL_SCREEN_IDLE_PANNING_SPEED;
    
    static const eFocusMode FOCUS_EVENT_INTERACTIVE_MODE;
    static const eFocusMode FOCUS_EVENT_DOOR_MODE;
    
    static const float FOCUS_EVENT_INTERACTIVE_COOLDOWN;
    static const float FOCUS_EVENT_DOOR_COOLDOWN;
    
    static const char* FOCUS_EVENT_SCHEDULE_CHECK_KEY;
    
    static const float VERTICAL_ZOOM_LOWER_BOUND;
    static const float VERTICAL_ZOOM_UPPER_BOUND;
    
    static const float VERTICAL_FOLLOW_OFFSET_LOWER_BOUND;
    static const float VERTICAL_FOLLOW_OFFSET_UPPER_BOUND;
    
    static const char* CUSTOM_CAMERA_TAG_THRESHOLD;
    static const char* CUSTOM_CAMERA_TAG_ATTRACTOR;
    static const char* CUSTOM_CAMERA_TAG_RAIL;
    static const char* CUSTOM_CAMERA_TAG_RAIL_ZOOM;
    static const char* CUSTOM_CAMERA_TAG_RAIL_START_PROJ;
    static const char* CUSTOM_CAMERA_TAG_RAIL_END_PROJ;
    static const char* CUSTOM_CAMERA_TAG_RAIL_POINT_TEMPLATE;
    
    static const char* CUSTOM_CAMERA_TAG_ZONE;
    static const char* CUSTOM_CAMERA_TAG_ZONE_AREA;
    static const char* CUSTOM_CAMERA_TAG_UPPER_BOUND;
    static const char* CUSTOM_CAMERA_TAG_UPPER_SCREEN;
    static const char* CUSTOM_CAMERA_TAG_LOWER_BOUND;
    static const char* CUSTOM_CAMERA_TAG_LOWER_SCREEN;
    
    static const bool DEBUG_CAMERA_MODE_ON;
    
    void initLevelCustom();
    void initLevelCustomCameraThreshold();
    void initLevelCustomCameraAttractors();
    void initLevelCustomCameraRails();
    void initLevelCustomCameraZones();
    
    void initCamera();
    void initCameraObservers();
    void initCameraObserversDoor();
    void initCameraObserversInteractive();
    
    void update(float dt);
    
    void updateFactors(float dt);
    
    void updateAllScreenIdle(float dt);
    void updateXFactors(float dt);
    void updateYFactors(float dt);
    
    void updateFollowOffsetSide();
    bool updateCurrentCameraThreshold();
    bool updateCurrentCameraZone();
    
    void zoomAllScreenStart();
    void zoomAllScreenButton();
    void zoomAllScreenIdle();
    void zoomStandardStart();
    void zoomStandard();
    
    void zoomAllScreen(float zoomSpeed, float panningSpeed, bool zoomSmooth, bool panningSmooth, eZoomOutType type);
    void zoomStandard(float zoomSpeed, bool instantly);
    
    void startSlowZoomAllScreenTimer();
    void resetSlowZoomAllScreenSchedule();
    
    void cameraEventDoor(Ref * door, void * options);
    void cameraEventInteractive(Ref * interacitve);
    void cameraEventFocus(CameraFocus * cameraFocus);
    
    // Observer Pattern
    void update(eEventType event, Ref * sender, void * options);
    
    // Utilities
    void setCameraState(eCameraState newState);
    float getCurrentZoomYValue();
    float getCurrentOffsetYValue();
    bool getCurrentZoomYValueFromZones(Point playerOnTarget, float * zoom);
    bool getCurrentOffsetYValueFromZones(Point playerOnTarget, float * offset);
    float getCurrentOffsetYValueFromBounds(Point playerOnTarget);
    float getCurrentZoomYValueFromBounds(Point playerOnTarget);
    Rect getCurrentCustomZoomYBoundsDelta();
    Rect getCurrentCustomOffsetYBoundsDelta();
    
    Point getAllScreenLevelCenter();
    Point getPointFitOnScale(Point point, float scale, Rect bounds);
    
    // DrawDebug
    void updateDebugDrawTrap();
    void updateDebugDrawLevelCenter();
    void updateDebugDrawCameraThresholds();
    void updateDebugDrawZoomInBounds(Rect bounds);
    void updateDebugDrawFollowOffsetBounds(Rect bounds);
    void updateDebugDrawZoneAreas();
    void updateDebugDrawZoneBounds(Rect bounds);
    
    std::unique_ptr<GameCamera> _gameCamera;
    
    LHScene * _gameScene;
    Node * _targetNode;
    Player * _player;
    eLevelType _levelType;
    
    eCameraState _cameraState;
    Action * _zoomFocusAction;
    float _zoomFocusTimer;
    
    float _trapLastPositionLeft;
    float _trapLastPositionRight;
    
    std::set<Ref *> _pressedSwitchesOR;
    
    std::vector<LHNode *> _cameraThresholds;
    LHNode * _currentCameraThreshold;
    
    std::vector<std::unique_ptr<Zone>> _cameraZones;
    Zone * _currentZone;
    
    bool _isLastLevel;
};

#endif /* defined(__SongOfPan__CameraManager__) */
