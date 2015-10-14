//
//  InputMenu.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 13/02/15.
//
//

#ifndef __SOP_Proto__InputMenu__
#define __SOP_Proto__InputMenu__

#include "cocos2d.h"
#include "GameLogic.h"

USING_NS_CC;

class InputSelectionDelegate
{
public:
    virtual void handleInputSelection(eInputType inputType) {};
};

class InputSelection : public Node
{
public:
    
    CREATE_FUNC(InputSelection);
    bool init() override;
    
    void setDelegate(InputSelectionDelegate * delegate)                    { _delegate = delegate; }
    
private:
    
    void initBackground();
    void initTitle();
    void initToggleSection();
    
    void updatePanel();
    
    void inputPressed(Ref *sender);
    
    eInputType loadInputType();
    void saveInputType(eInputType inputType);
    
    eInputType getInputTypeFromIndex(int index);
    int getIndexFromInputType(eInputType inputType);
    
    Sprite * _panelSprite;
    
    MenuItemToggle * _toggleMenu;
    
    Label * _traditionalLabel;
    Label * _experimentalLabel;
    
    Sprite * _traditionalSprite;
    Sprite * _experimentalSprite;
    
    InputSelectionDelegate * _delegate;
};


#endif /* defined(__SOP_Proto__InputMenu__) */
