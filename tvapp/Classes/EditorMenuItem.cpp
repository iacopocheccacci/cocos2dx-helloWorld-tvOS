//
//  EditorMenuItem.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#include "EditorMenuItem.h"
#include "VisibleRect.h"

bool EditorMenuItem::init()
{
    bool initDone = false;
    
    if (LayerColor::init())
    {
        _tabEnabled = false;
        Rect rect = VisibleRect::getVisibleRect();
        
        setColor(Color3B(50,50,50));
        setOpacity(200);
        setContentSize(Size(rect.getMaxX(), rect.getMaxY() / 4));
        
        initDone = true;
    }
    
    return initDone;
}
