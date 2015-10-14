//
//  PausePopUp.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#include "PausePopUp.h"
#include "GameLogic.h"
#include "SettingsPopUp.h"
#include "VisibleRect.h"
#include "Definitions.h"

namespace PausePopUpDefs
{
    static const char* MIDDLE_GROUND_SPRITE_NAME = "UI/popupBg.png";
    static const char* LOGO_SPRITE_NAME = "UI/popupLogo.png";
}

PausePopUp::~PausePopUp()
{
    removeAllChildrenWithCleanup(true);
}

PausePopUp * PausePopUp::create(eLevelType levelType)
{
    PausePopUp * layer = new (std::nothrow) PausePopUp();
    if(layer && layer->init(levelType))
    {
        layer->autorelease();
        return layer;
    }
    CC_SAFE_DELETE(layer);
    return nullptr;
}

bool PausePopUp::init(eLevelType levelType)
{
    bool initDone = false;
    
    if (NavigationPopUp::initWithSpriteName(PausePopUpDefs::MIDDLE_GROUND_SPRITE_NAME))
    {
        this->initLogo();
        if (eLevelType::NORMAL == levelType)
        {
            this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_RESTART);
            this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_HOME);
        }
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_IAP);
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_OPTIONS);
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_RESUME);
        
        initDone = true;
    }
    return initDone;
}

void PausePopUp::initLogo()
{
    Sprite * logoSprite = Sprite::create(PausePopUpDefs::LOGO_SPRITE_NAME);
    logoSprite->setPosition(_middleGround->getContentSize().width / 2.0f, 450.0f);
    _middleGround->addChild(logoSprite);
}