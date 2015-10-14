//
//  EditorMenuManager.h
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 06/11/14.
//
//

#ifdef EDITOR_MODE

#ifndef __SOP_EDITOR_MENU_MANAGER__
#define __SOP_EDITOR_MENU_MANAGER__

#include "cocos2d.h"

class InputMenuItem;
class SheepMenuItem;
class NoteMenuItem;

USING_NS_CC;

enum eItems
{
    INPUT_MENU = 0,
    SHEEP_MENU,
    NOTE_MENU,
    GOATSKIN_MENU,
    BACK,
    SAVE,
};

class EditorMenuManager : public Layer
{
public:
    EditorMenuManager();
    virtual ~EditorMenuManager();
    CREATE_FUNC(EditorMenuManager);
    
    bool init();
    void createItems();
    void inputTabPressed(Ref* sender);
    void sheepTabPressed(Ref* sender);
    void noteTabPressed(Ref* sender);
    void goatskinTabPressed(Ref* sender);
    void saveTabPressed(Ref* sender);
    void backTabPressed(Ref* sender);
    
private:
    void moveTab(eItems item);
    void saveData();
};


#endif // __SOP_EDITOR_MENU_MANAGER__

#endif // EDITOR_MODE