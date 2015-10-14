//
//  AudioOptions.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 17/09/15.
//
//

#include "AudioOptions.h"
#include "Definitions.h"
#include "SoundManager.h"
#include "SettingsManager.h"

namespace audioOptionsDefs
{
    static const char* BG_SPRITE_NAME = "UI/optionsAudioBg.png";
    static const char* CHECK_SPRITE_NAME = "UI/optionsAudio_check.png";
    
    static const char* TITLE = "AUDIO OPTIONS";
}

bool AudioOptions::init()
{
    bool initDone = false;
    
    if (Node::init())
    {
        _panelSprite = nullptr;
        _audioMusicOnOffToggle = nullptr;
        _audioFXOnOffToggle = nullptr;
        
        this->initBackground();
        this->initTitle();
        this->initAudioMusicOnOff();
        this->initAudioFXOnOff();
        
        this->updatePanel();
        
        initDone = true;
    }
    return initDone;
}

void AudioOptions::initBackground()
{
    _panelSprite = Sprite::create(audioOptionsDefs::BG_SPRITE_NAME);
    this->addChild(_panelSprite);
}

void AudioOptions::initTitle()
{
    Label * label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, audioOptionsDefs::TITLE);
    label->setPosition(Point(_panelSprite->getContentSize().width / 2.0f, 125.0f));
    label->setScale(0.5f);
    label->setColor(appParams::FONT_ORANGE);
    _panelSprite->addChild(label);
}

void AudioOptions::initAudioMusicOnOff()
{
    // Etichetta Audio Music
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "MUSIC");
    label->setPosition(Point(94.0f, 79.0f));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    _panelSprite->addChild(label);
    
    // Pulsante Audio On/Off
    auto emptyCheckbox = Sprite::create(audioOptionsDefs::CHECK_SPRITE_NAME);
    emptyCheckbox->setOpacity(0.0f);
    auto itemNormal = MenuItemSprite::create(emptyCheckbox, emptyCheckbox);
    auto itemActive = MenuItemImage::create(audioOptionsDefs::CHECK_SPRITE_NAME,
                                            audioOptionsDefs::CHECK_SPRITE_NAME);
    _audioMusicOnOffToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(AudioOptions::handleAudioMusicOnOff, this),
                                                                itemNormal,
                                                                itemActive,
                                                                nullptr);
    _audioMusicOnOffToggle->setAnchorPoint(Point(0.135f, 0.5f));
    
    // Menu Audio On/Off
    Menu * menu = Menu::create(_audioMusicOnOffToggle, NULL);
    menu->setPosition(Point(64.0f, 79.0f));
    _panelSprite->addChild(menu);
}

void AudioOptions::initAudioFXOnOff()
{
    // Etichetta Audio FX
    auto label = Label::createWithBMFont(appParams::CHAPTERS_FONT_PATH, "FX");
    label->setPosition(Point(94.0f, 39.0f));
    label->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label->setScale(0.5f);
    _panelSprite->addChild(label);
    
    // Pulsante Audio FX On/Off
    auto emptyCheckbox = Sprite::create(audioOptionsDefs::CHECK_SPRITE_NAME);
    emptyCheckbox->setOpacity(0);
    auto itemNormal = MenuItemSprite::create(emptyCheckbox, emptyCheckbox);
    auto itemActive = MenuItemImage::create(audioOptionsDefs::CHECK_SPRITE_NAME,
                                            audioOptionsDefs::CHECK_SPRITE_NAME);
    _audioFXOnOffToggle = MenuItemToggle::createWithCallback(CC_CALLBACK_1(AudioOptions::handleAudioFXOnOff, this),
                                                             itemNormal,
                                                             itemActive,
                                                             nullptr);
    _audioFXOnOffToggle->setAnchorPoint(Point(0.135f, 0.5f));
    
    // Menu Audio FX On/Off
    Menu * menu = Menu::create(_audioFXOnOffToggle, NULL);
    menu->setPosition(Point(64.0f, 39.0f));
    _panelSprite->addChild(menu);
}

void AudioOptions::handleAudioMusicOnOff(Ref * pSender)
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveMusic();
    bool newStatus = !currentStatus;
    
    SettingsManager::getInstance()->saveUserActiveMusic(newStatus);
    
    // Il Director non viene più messo in pause, quindi posso fare sempre fade
    bool fadeEffect = true;
    float newVolume = (newStatus) ? SettingsManager::getInstance()->loadUserVolumeMusic() : 0.0f;
    SoundManager::getInstance()->setUserVolumeMusic(newVolume, fadeEffect);
    
    this->updatePanel();
}

void AudioOptions::handleAudioFXOnOff(Ref * pSender)
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveFX();
    bool newStatus = !currentStatus;
    
    SettingsManager::getInstance()->saveUserActiveFX(newStatus);
    
    // Il Director non viene più messo in pause, quindi posso fare sempre fade
    bool fadeEffect = true;//(_panelInGame) ? false : true;
    float newVolume = (newStatus) ? SettingsManager::getInstance()->loadUserVolumeFX() : 0.0f;
    SoundManager::getInstance()->setUserVolumeFX(newVolume, fadeEffect);
    
    this->updatePanel();
}

#pragma mark - Aggiornamento del pannello

void AudioOptions::updatePanel()
{
    this->updateAudioMusicOnOff();
    this->updateAudioFXOnOff();
}

void AudioOptions::updateAudioMusicOnOff()
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveMusic();
    if (currentStatus)
    {
        _audioMusicOnOffToggle->setSelectedIndex(1);
    }
    else
    {
        _audioMusicOnOffToggle->setSelectedIndex(0);
    }
}

void AudioOptions::updateAudioFXOnOff()
{
    bool currentStatus = SettingsManager::getInstance()->loadUserActiveFX();
    if (currentStatus)
    {
        _audioFXOnOffToggle->setSelectedIndex(1);
    }
    else
    {
        _audioFXOnOffToggle->setSelectedIndex(0);
    }
}
