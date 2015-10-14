//
//  ConfirmInputPopUp.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 22/04/15.
//
//

#ifndef __SongOfPan__ConfirmInputPopUp__
#define __SongOfPan__ConfirmInputPopUp__

#include "cocos2d.h"
#include "NavigationPopUp.h"
#include "InputSelection.h"

USING_NS_CC;

class ConfirmInputPopUp : public NavigationPopUp
{
public:
    
    CREATE_FUNC(ConfirmInputPopUp);
    bool init();
    
private:
    
    Menu * _controlsMenu;
    
    void initLabels();
    void initToggle();
    void initConfirmButton();
    
    MenuItem * setupButtonExperimental();
    MenuItem * setupButtonTraditional();
    
    void experimentalPressed(Ref * sender);
    void traditionalPressed(Ref * sender);
    void confirmPressed(Ref * sender);
};

#endif /* defined(__SongOfPan__ConfirmInputPopUp__) */
