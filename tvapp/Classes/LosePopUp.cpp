//
//  LosePopUp.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#include "LosePopUp.h"
#include "Definitions.h"
#include "VisibleRect.h"

namespace LosePopUpDefs
{
    static const char* MIDDLE_GROUND_SPRITE_NAME = "UI/popupBg.png";
    static const char* TITLE_TEXT = "GAME OVER";
}

bool LosePopUp::init()
{
    bool initDone = false;
    
    if (NavigationPopUp::initWithSpriteName(LosePopUpDefs::MIDDLE_GROUND_SPRITE_NAME))
    {
        this->initTitle();
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_RESTART);
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_HOME);
        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_IAP);
//        this->addPredefinedItemToMenu(ePopUpItem::BUTTON_ITEM_OPTIONS);
        
        initDone = true;
    }
    return initDone;
}

void LosePopUp::initTitle()
{
    Label * label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, LosePopUpDefs::TITLE_TEXT);
    label->setPosition(Point(_middleGround->getContentSize().width / 2.0f, 450.0f));
    label->setColor(appParams::FONT_ORANGE);
    _middleGround->addChild(label);
}
