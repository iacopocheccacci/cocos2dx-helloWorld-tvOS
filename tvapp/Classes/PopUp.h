//
//  PopUp.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 17/11/14.
//
//

#ifndef __SOP_POP_UP__
#define __SOP_POP_UP__

#include "cocos2d.h"

USING_NS_CC;

namespace PopUpDefs
{
    static const Color4B LAYER_COLOR = {0, 0, 0, 166};
}

class PopUp : public LayerColor
{
public:
    virtual ~PopUp();
    virtual bool init();
    
    void setCallbackFunc(Ref* target, SEL_CallFuncN callback);
    
protected:
    
    Ref* _callbackListener;
    SEL_CallFuncN _callback;
    
};

#endif // __SOP_POP_UP__
