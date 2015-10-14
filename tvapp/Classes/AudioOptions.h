//
//  AudioOptions.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 17/09/15.
//
//

#ifndef __SongOfPan__AudioOptions__
#define __SongOfPan__AudioOptions__

#include "cocos2d.h"

USING_NS_CC;

class AudioOptions : public Node
{
public:
    
    CREATE_FUNC(AudioOptions);
    bool init() override;
    
private:
    
    void initBackground();
    void initTitle();
    void initAudioMusicOnOff();
    void initAudioFXOnOff();
    
    void handleAudioMusicOnOff(Ref * pSender);
    void handleAudioFXOnOff(Ref * pSender);
    
    void updatePanel();
    void updateAudioMusicOnOff();
    void updateAudioFXOnOff();
    
    Sprite * _panelSprite;
    
    MenuItemToggle * _audioMusicOnOffToggle;
    MenuItemToggle * _audioFXOnOffToggle;
    
};


#endif /* defined(__SongOfPan__AudioOptions__) */
