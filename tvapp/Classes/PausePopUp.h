//
//  PausePopUp.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#ifndef __SOP_PAUSE_POP_UP__
#define __SOP_PAUSE_POP_UP__

#include "NavigationPopUp.h"

enum class eLevelType;

class PausePopUp : public NavigationPopUp
{
public:
    virtual ~PausePopUp();
    static PausePopUp * create(eLevelType levelType);
    bool init(eLevelType levelType);
    
private:
    void initLogo();
    
//    MenuItem* setupSettingsButton();
//    
//    void settingsPressed(Ref* sender);
};


#endif // __SOP_PAUSE_POP_UP__
