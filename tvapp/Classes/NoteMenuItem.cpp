//
//  NoteMenuItem.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 13/11/14.
//
//

#ifdef EDITOR_MODE

#include "NoteMenuItem.h"

#include "VisibleRect.h"
#include "Utility.h"

NoteMenuItem::NoteMenuItem()
{
    
}

NoteMenuItem::~NoteMenuItem()
{
    removeAllChildren();
}

bool NoteMenuItem::init()
{
    bool initDone = false;
    
    if (EditorMenuItem::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void NoteMenuItem::initLevelVar()
{
    char buff[3];
    auto s = this->getContentSize();
    
    auto noteDistanceLabel = Label::createWithSystemFont("Note Dist.:", "Marker Felt", 15);
    noteDistanceLabel->setColor(Color3B::BLACK);
    noteDistanceLabel->setPosition(Point(s.width / 12.0f, s.height /  1.45f));
    addChild(noteDistanceLabel);
    
    _noteDistanceSliderCtl = noteDistanceActivation();
    _noteDistanceSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 1.45f));
    addChild(_noteDistanceSliderCtl, 0, TYPE_NOTE_DISTANCE);
    
    snprintf(buff, 3, "%.2f", _noteDistanceSliderCtl->getValue());
    auto noteDistanceValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    noteDistanceValue->setColor(Color3B::BLACK);
    noteDistanceValue->setPosition(Point(s.width / 2.5f, s.height / 1.45f));
    addChild(noteDistanceValue, 0, NOTE_DISTANCE);
    
    /*-------------------*/
    
    auto particleScaleLabel = Label::createWithSystemFont("Part. Scale:", "Marker Felt", 15);
    particleScaleLabel->setColor(Color3B::BLACK);
    particleScaleLabel->setPosition(Point(s.width / 12.0f, s.height / 3.0f));
    addChild(particleScaleLabel);
    
    _particleScaleSliderCtl = particleScaleActivation();
    _particleScaleSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 3.0f));
    addChild(_particleScaleSliderCtl, 0, TYPE_PARTICLE_SCALE);
    
    snprintf(buff, 3, "%.2f", _particleScaleSliderCtl->getValue());
    auto particleScaleValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    particleScaleValue->setColor(Color3B::BLACK);
    particleScaleValue->setPosition(Point(s.width / 2.5f, s.height / 3.0f));
    addChild(particleScaleValue, 0, PARTICLE_SCALE);
    
    /*-------------------*/
    
    auto sheepPanDistanceLabel = Label::createWithSystemFont("Pan - sheep:", "Marker Felt", 15);
    sheepPanDistanceLabel->setColor(Color3B::BLACK);
    sheepPanDistanceLabel->setPosition(Point(s.width * 0.6f, s.height / 1.45f));
    addChild(sheepPanDistanceLabel);
    
    _sheepPanDistanceSliderCtl = sheepPanDistanceActivation();
    _sheepPanDistanceSliderCtl->setPosition(Vec2(s.width * 0.8f, s.height / 1.45f));
    addChild(_sheepPanDistanceSliderCtl, 0, TYPE_SHEEP_PAN_DISTANCE);
    
    snprintf(buff, 3, "%.2f", _sheepPanDistanceSliderCtl->getValue());
    auto sheepPanDistanceValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    sheepPanDistanceValue->setColor(Color3B::BLACK);
    sheepPanDistanceValue->setPosition(Point(s.width * 0.95f, s.height / 1.45f));
    addChild(sheepPanDistanceValue, 0, SHEEP_PAN_DISTANCE);
    
    /*-------------------*/
    
    auto noteDurationLabel = Label::createWithSystemFont("Note duration:", "Marker Felt", 15);
    noteDurationLabel->setColor(Color3B::BLACK);
    noteDurationLabel->setPosition(Point(s.width * 0.6f, s.height / 3.0f));
    addChild(noteDurationLabel);
    
    _noteDurationSliderCtl = noteDurationActivation();
    _noteDurationSliderCtl->setPosition(Vec2(s.width * 0.8f, s.height / 3.0f));
    addChild(_noteDurationSliderCtl, 0, TYPE_NOTE_DURATION);
    
    snprintf(buff, 3, "%.2f", _noteDurationSliderCtl->getValue());
    auto noteDurationValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    noteDurationValue->setColor(Color3B::BLACK);
    noteDurationValue->setPosition(Point(s.width * 0.95f, s.height / 3.0f));
    addChild(noteDurationValue, 0, NOTE_DURATION);
}

extension::ControlSlider* NoteMenuItem::noteDistanceActivation()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Note", "noteDistance", "9.5");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(10.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(NoteMenuItem::noteDistanceSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void NoteMenuItem::noteDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    bool foundAtLeastOne = false;
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    Ref* ref = nullptr;
    Sheep* sheep = nullptr;
    
    CCARRAY_FOREACH(_scene->getSheep(), ref)
    {
        sheep = dynamic_cast<Sheep*>(ref);
        if (sheep)
        {
            sheep->setNoteActivationDistance(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", sheep->getNoteActivationDistance());
                auto valueLabel = (Label *)getChildByTag(NOTE_DISTANCE);
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

extension::ControlSlider* NoteMenuItem::particleScaleActivation()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Note", "particleScale", "1.25");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(1.5f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(NoteMenuItem::particleScaleSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void NoteMenuItem::particleScaleSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    _player->setParticleScale(scale);
    
    char string[12] = {0};
    snprintf(string, 12, "%2.2f", _player->getParticleScale());
    
    auto valueLabel = dynamic_cast<Label*>(getChildByTag(PARTICLE_SCALE));
    
    valueLabel->setString(string);
}

extension::ControlSlider* NoteMenuItem::sheepPanDistanceActivation()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Note", "sheepPanDistance", "1.5");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(3.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(NoteMenuItem::sheepPanDistanceSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void NoteMenuItem::sheepPanDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    bool foundAtLeastOne = false;
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    Ref* ref = nullptr;
    Sheep* sheep = nullptr;
    
    CCARRAY_FOREACH(_scene->getSheep(), ref)
    {
        sheep = dynamic_cast<Sheep*>(ref);
        if (sheep)
        {
            sheep->setDistanceFromPlayer(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", sheep->getDistanceFromPlayer());
                auto valueLabel = dynamic_cast<Label*>(getChildByTag(SHEEP_PAN_DISTANCE));
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

extension::ControlSlider* NoteMenuItem::noteDurationActivation()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Note", "noteDuration", "10.0");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(30.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(NoteMenuItem::noteDurationSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void NoteMenuItem::noteDurationSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    if (_scene)
    {
        _scene->setNoteDuration(scale);
        
        char string[12] = {0};
        snprintf(string, 12, "%2.2f", _scene->getNoteDuration());
        auto valueLabel = dynamic_cast<Label*>(getChildByTag(NOTE_DURATION));
        valueLabel->setString(string);
    }
}

#endif // EDITOR_MODE
