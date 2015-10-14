//
//  SettingsPanel.h
//  SOP_Proto
//
//  Created by Leonardo Passeri on 05/03/15.
//
//

#ifndef __SOP_Proto__SettingsPanel__
#define __SOP_Proto__SettingsPanel__

#include "cocos2d.h"
#include "InputSelection.h"

enum class eInputType;

USING_NS_CC;

class SettingsPopUp : public LayerColor, public InputSelectionDelegate
{    
public:
    
    static SettingsPopUp * create(const bool inGame);
    static SettingsPopUp * create(const Color4B& color, const bool inGame);
    
    virtual bool initWithColor(const Color4B& color, const bool inGame);
    
    void presentPopUp(Node * parent, bool animation);
    
private:
    
    void initBlockTouch();
    void initPanel(eInputType inputType);
    void initContents(eInputType inputType);
    void initCloseButton();
    void initAudioMusicOnOff(eInputType inputType);
    void initAudioFXOnOff(eInputType inputType);
    void initAudioMusicVolume(eInputType inputType);
    void initAudioFXVolume(eInputType inputType);
    void initInputSelection();
    
    void initDelayedInteraction();
    
    void dismissPopUp();
    void enableInteraction(bool enable);
    
    void updatePanel();
    void updateAudioMusicOnOff();
    void updateAudioFXOnOff();
//    void updateInputSelection();
    
    void handleClose(Ref * pSender);
    
    void handleAudioMusicOnOff(Ref * pSender);
    void handleAudioFXOnOff(Ref * pSender);
    
    void handleInputSelection(eInputType inputType) override;
    
    Sprite * _panelSprite;
    
    MenuItemToggle * _audioMusicOnOffToggle;
    
    MenuItemToggle * _audioFXOnOffToggle;
    MenuItemImage * _itemImageClose;
    
    InputSelection * _inputSelection;
    
    bool _willAnimate;
    bool _panelInGame;
};

#endif /* defined(__SOP_Proto__SettingsPanel__) */
