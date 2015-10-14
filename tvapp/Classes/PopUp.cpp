//
//  PopUp.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/11/14.
//
//

#include "PopUp.h"
#include "VisibleRect.h"
#include "GameLogic.h"

PopUp::~PopUp()
{
    removeAllChildrenWithCleanup(true);
    
    _callbackListener = nullptr;
    _callback = nullptr;
}

bool PopUp::init()
{
    bool initDone = false;

    if (LayerColor::initWithColor(PopUpDefs::LAYER_COLOR))
    {
        initDone = true;
    }
    
    return initDone;
}

void PopUp::setCallbackFunc(Ref *target, SEL_CallFuncN callFunc)
{
    _callbackListener = target;
    _callback = callFunc;
}