//
//  LosePopUp.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 20/11/14.
//
//

#ifndef __SOP_LOSE_POP_UP__
#define __SOP_LOSE_POP_UP__

#include "NavigationPopUp.h"

class LosePopUp : public NavigationPopUp
{
public:
    CREATE_FUNC(LosePopUp);
    bool init();
private:
    void initTitle();
};

#endif // __SOP_LOSE_POP_UP__
