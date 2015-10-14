//
//  Definitions.h
//  SOP_Proto
//
//  Created by Giuseppe Lapenta on 17/11/14.
//
//

#ifndef SOP_Proto_Definitions_h
#define SOP_Proto_Definitions_h

#include "ccTypes.h"

namespace appParams
{
    static const char* APP_NAME = "Song of Pan";
    static const char* EXTENSION_HD_PATH = "extension/hd";
    static const char* EXTENSION_SD_PATH = "extension/sd";
    static const char* PUBLISHED_RES_PATH = "publishedResources";
    static const char* PARTICLES_PATH = "particles";
    static const char* COCOS_STUDIO_PATH = "CocosStudio";
    static const char* COCOS_STUDIO_RES_PATH_TEMPLATE = "CocosStudio/%s";
    static const char* CHAPTERS_FONT_PATH = "UI/capitoli.fnt";
    static const char* DESCRIPTION_FONT_PATH = "UI/futura.fnt";
    
    static const char* PARAMS_FILE_NAME = "SOP.plist";
    static const char* LEVELS_LIST_FILE_NAME = "SOP_levels.plist";
    static const char* LEVELS_ARRAY_NAME = "Levels";
    
    static const char* LEVEL_NAME_TEMPLATE = "world%02dlevel%02d";
    static const char* PUBLISHED_LEVEL_NAME_TEMPLATE = "world%02d_lev%02d.lhplist";
    static const char* PUBLISHED_MAIN_MENU_LEVEL_NAME = "MainMenu.lhplist";
    static const char* PUBLISHED_LEVEL_SELECTION_NAME_TEMPLATE = "LevelSelectionWorld%02d.lhplist";
    
    static const char* LEFT_BUTTON_PATH = "UI/sxBtn.png";
    static const char* RIGHT_BUTTON_PATH = "UI/dxBtn.png";
    static const char* JUMP_BUTTON_PATH = "UI/jumpBtn.png";
    
    static const char* BLANK_PIXEL_FILE_NAME = "blankPixel.png";
    
    static const char* MENU_BUTTONS_NAME = "menuButtons";
    static const char* LEFT_BUTTON_NAME = "LeftButton";
    static const char* RIGHT_BUTTON_NAME = "RightButton";
    static const char* JUMP_BUTTON_NAME = "JumpButton";
    static const float CONTROLLER_BUTTON_WIDTH = 120;
    static const float CONTROLLER_BUTTON_HEIGHT = 150;
    
    static const int NUMBER_OF_WORLDS = 5;
    static const int BONUS_WORLD = NUMBER_OF_WORLDS + 1;
    static const int MAX_LEVELS_PER_WORLD = 10;
    
    static const char* RESPAWN_NODE_NAME = "Respawn";
    
    static const char* SKY_NODE_NAME = "sky";
    static const char* BACK_MOUNTAIN_NODE_NAME = "backMountain";
    static const char* FRONT_MOUNTAIN_NODE_NAME = "frontMountain";
    static const char* LIGHTNING_ROOT_NODE_NAME = "LightningRootNode";
    static const char* SHEEP_SHOW_NODE_NAME = "SheepShowNode";
    static const std::string SHEEP_ASSET_NAME = "Sheep.lhasset";
    static const std::string FLUTE_ASSET_NAME = "FluteAsset.lhasset";
    
    static const char* GOAL_NODE_NAME = "CapannaNode";
    
    static const cocos2d::Color3B FONT_ORANGE = {206, 89, 62};
}

namespace gameTags
{
    static const char* PLAYER_TAG = "PLAYER";
    static const char* BOUND_TAG = "BOUND";
    static const char* GROUND_TAG = "GROUND";
    static const char* SIDE_TAG = "SIDE";
    static const char* LOSE_TAG = "LOSE";
    static const char* BLOCK_TAG = "BLOCK";
    static const char* NOTE_TAG = "NOTE";
    static const char* INVERSE_NOTE_TAG = "INVERSE";
    static const char* ONESIDE_TAG = "ONESIDE";
    static const char* MOVABLE_TAG = "MOVABLE";
    static const char* INTERACTIVE_TAG = "INTERACTIVE";
    static const char* GOAL_TAG = "GOAL";
    static const char* SHEEP_TAG = "SHEEP";
    static const char* FLUTE_TAG = "FLUTE";
    static const char* GOATSKIN_TAG = "GOATSKIN";
    static const char* FIRE_TAG = "FIRE";
    static const char* BRAZIER_TAG = "BRAZIER";
    static const char* STATIC_BRAZIER_TAG = "STATIC";
    static const char* NON_STATIC_BRAZIER_TAG = "NON-STATIC";
    static const char* DOOR_TAG = "DOOR";
    static const char* AND_TAG = "AND";
    static const char* OR_TAG = "OR";
    static const char* SWITCH_TAG = "SWITCH";
    static const char* GEAR_TAG = "GEAR";
    static const char* WOLF_TAG = "WOLF";
    static const char* WOLF_PLAYER_AREA_TAG = "WOLF_PLAYER_AREA";
    static const char* CONTROLLER_TAG = "CONTROLLER";
    static const char* MONSTER_TAG = "MONSTER";
    static const char* PORTAL_TAG = "PORTAL";
    static const char* MONOUSE_TAG = "MONOUSE";
    static const char* MOVE_TAG = "MOVE";
    static const char* DIRECTION_LEFT_TAG = "LEFT";
    static const char* DIRECTION_RIGHT_TAG = "RIGHT";
    static const char* DIRECTION_UP_TAG = "UP";
    static const char* DIRECTION_DOWN_TAG = "DOWN";
    static const char* PORTAL_WORLD_TAG_ROOT = "WORLD_";
    static const char* PORTAL_LEVEL_TAG_ROOT = "LEVEL_";
    static const char* PORTAL_MAIN_MENU_TAG = "MAIN_MENU";
    static const char* SCRIPT_EVENT_TAG = "SCRIPT_EVENT";
    static const char* CAMERA_EVENT_TAG = "CAMERA_EVENT";
    static const char* START_EOLO_TAG = "START_EOLO";
    static const char* CLOUD_THUNDER_TAG = "CLOUD_THUNDER";
    static const char* LIGHTNING_SEGMENT_TAG = "LIGHTNING_SEGMENT";
    static const char* INSPIRATION_TAG = "INSPIRATION";
    static const char* MUSIC_INSPIRATION_TAG = "MUSIC";
    static const char* PARTICLE_FOG_TAG = "PARTICLE_FOG";
    static const char* PARTICLE_EOLO_ZONE_TAG = "PARTICLE_EOLO_ZONE";
    static const char* GRASS_TAG = "GRASS";
    static const char* TEMPLE_ROCK_TAG = "TEMPLE_ROCK";
    static const char* RAIN_TAG = "RAIN";
    static const char* UNLOCK_PLATFORM_TAG = "UNLOCK_PLATFORM";
    static const char* FAITH_TAG = "FAITH";
    static const char* BRIDGE_TRESHOLD_TAG = "BRIDGE_TRESHOLD";
}

namespace callbackDefs
{
    static const char* RESTART = "restart";
    static const char* ZOOM = "zoom";
    static const char* PAUSE = "pause";
    static const char* RESUME = "resume";
    static const char* HOME = "home";
    static const char* NEXT = "next";
    static const char* BACK = "back";
    static const char* STOP_NOTE = "stopNote";
    static const char* WOLF = "wolf";
    static const char* PORTAL = "portal";
    static const char* PORTAL_WORLD_ROOT = "portalWorld";
    static const char* PORTAL_LEVEL_ROOT = "portalLevel";
    static const char* PORTAL_MAIN_MENU = "portalMainMenu";
    static const char* VIDEO_PLAY = "videoPlay";
    static const char* VIDEO_STOP = "videoStop";
    static const char* VIDEO_LOCKED = "videoLocked";
    static const char* NOTIFICATION_RESUME = "resumeNotification";
    static const char* SHEEP_EXITING = "sheepExiting";
}

namespace scriptNodeNameDefs
{
    static const char* TUTORIAL_JUMP_AHEAD = "TutorialJumpAhead";
    static const char* TUTORIAL_JUMP_VERTICAL = "TutorialJumpVertical";
    static const char* INPUT_CONFIRM = "InputConfirm";
    static const char* TUTORIAL_VIDEO = "TutorialVideo";
    static const char* SHEEP_SHOW_ZONE = "SheepShowZone";
}

#define DESIGN_RES_H 960.0f
#define DESIGN_RES_V 640.0f
#define PTM_RATIO 32

#define DEBUG_UNLOCK_ALL_LEVELS

#define DEBUG_UNLOCK_ALL_FLUTES

//#define TEST_PLAYER_INIT_POSITION // se definita, Pan apparirà nel MainMenu sempre in basso a sinistra.

enum class eAction
{
    NONE = 0,
    MOVE_RIGHT,
    STOP_MOVE_RIGHT,
    MOVE_LEFT,
    STOP_MOVE_LEFT,
    JUMP,
    STOP_JUMP,
    FALL,
    STOP_FALL,
    BURN,
    JUMP_ON,
    FADE,
    ANIMATION,
    EXULT,
    SHAKE,
    FLY,
    STOP_FLY,
    SWITCH_OFF
};

enum class eGroundType
{
    GRASS = 0,
    STONE,
    TEMPLE,
    NONE
};

enum class eLevelType
{
    NORMAL = 0,
    MAIN_MENU,
    LEVEL_SELECTION
};

/**
 * define a nodeWithDictionary function for a specific type
 */
#define NODEWITHDICT_FUNC(__TYPE__) \
static __TYPE__* nodeWithDictionary(LHDictionary* dict, Node* prnt) \
{ \
    __TYPE__ *pRet = new __TYPE__(); \
    if (pRet && pRet->initWithDictionary(dict, prnt)) \
    { \
        pRet->autorelease(); \
        return pRet; \
    } \
    else \
    { \
        delete pRet; \
        pRet = NULL; \
        return NULL; \
    } \
}

#if defined(COCOS2D_DEBUG) && COCOS2D_DEBUG == 1
#define UPDATE_LOG(__CCLOG_FUNC__, __TIMER_NAME__, __UPDATE_TIME__, __LOG_INTERVAL__) \
static float __TIMER_NAME__ = 0; \
__TIMER_NAME__ += __UPDATE_TIME__; \
if (__TIMER_NAME__ > __LOG_INTERVAL__) \
{ \
__TIMER_NAME__ = 0; \
__CCLOG_FUNC__; \
}
#else
#define UPDATE_LOG(...)       do {} while (0)
#endif

#endif
