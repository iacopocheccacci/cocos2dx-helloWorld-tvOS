//
//  InputMenuItem.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 05/11/14.
//
//

#ifdef EDITOR_MODE

#include "InputMenuItem.h"
#include "VisibleRect.h"
#include "Player.h"
#include "Utility.h"

InputMenuItem::InputMenuItem()
{
    _player = nullptr;
}

InputMenuItem::~InputMenuItem()
{
    removeAllChildren();
}

bool InputMenuItem::init()
{
    bool initDone = false;
    
    if (EditorMenuItem::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void InputMenuItem::initLevelVar()
{
    char buff[3];
    auto s = this->getContentSize();
    
    auto horImpLabel = Label::createWithSystemFont("Hor. Imp.:", "Marker Felt", 15);
    horImpLabel->setColor(Color3B::BLACK);
    horImpLabel->setPosition(Point(s.width / 12.0f, s.height /  1.45f));
    addChild(horImpLabel);
    
    _horImpSliderCtl = horImpSliderCtl();
    _horImpSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 1.45f));
    addChild(_horImpSliderCtl, 0, TYPE_INPUT_HORIZONTAL_IMPULSE);

    snprintf(buff, 3, "%.2f", _horImpSliderCtl->getValue());
    auto horImpValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    horImpValue->setColor(Color3B::BLACK);
    horImpValue->setPosition(Point(s.width / 2.5f, s.height /  1.45f));
    addChild(horImpValue, 0, INPUT_HORIZONTAL_IMPULSE);
    
    /*-------------------*/
    
    auto vertImpLabel = Label::createWithSystemFont("Vert. Imp.:", "Marker Felt", 15);
    vertImpLabel->setColor(Color3B::BLACK);
    vertImpLabel->setPosition(Point(s.width / 12.0f, s.height /  3.0f));
    addChild(vertImpLabel);
    
    _vertImpSliderCtl = vertImpSliderCtl();
    _vertImpSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 3.0f));
    addChild(_vertImpSliderCtl, 0, TYPE_INPUT_VERTICAL_IMPULSE);
    
    snprintf(buff, 3, "%.2f", _vertImpSliderCtl->getValue());
    auto vertImpValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    vertImpValue->setColor(Color3B::BLACK);
    vertImpValue->setPosition(Point(s.width / 2.5f, s.height /  3.0f));
    addChild(vertImpValue, 0, INPUT_VERTICAL_IMPULSE);
    
    /*-------------------*/
    
    auto horLimitLabel = Label::createWithSystemFont("Hor. Limit:", "Marker Felt", 15);
    horLimitLabel->setColor(Color3B::BLACK);
    horLimitLabel->setPosition(Point(s.width * 0.6f, s.height /  1.45f));
    addChild(horLimitLabel);
    
    _horLimitSliderCtl = horLimitSliderCtl();
    _horLimitSliderCtl->setPosition(Vec2(s.width * 0.8f, s.height / 1.45f));
    addChild(_horLimitSliderCtl, 0, TYPE_INPUT_HORIZONTAL_LIMIT);
    
    snprintf(buff, 3, "%.2f", _horLimitSliderCtl->getValue());
    auto horLimitValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    horLimitValue->setColor(Color3B::BLACK);
    horLimitValue->setPosition(Point(s.width * 0.95f, s.height / 1.45f));
    addChild(horLimitValue, 0, INPUT_HORIZONTAL_LIMIT);
}

extension::ControlSlider* InputMenuItem::horImpSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Input", "horizontalImpulse", "0.3");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(1.5f);
    slider->setValue(atof(value->getCString()));

    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(InputMenuItem::horImpSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void InputMenuItem::horImpSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    _player->setHorizontalImpulse(scale);
    
    char string[12] = {0};
    snprintf(string, 12, "%2.2f", _player->getHorizontalImpulse());
    
    auto valueLabel = dynamic_cast<Label*>(getChildByTag(INPUT_HORIZONTAL_IMPULSE));
    
    valueLabel->setString(string);
}

extension::ControlSlider* InputMenuItem::vertImpSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Input", "verticalImpulse", "12.35");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(20.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(InputMenuItem::vertImpSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void InputMenuItem::vertImpSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    _player->setVerticalImpulse(scale);
    
    char string[12] = {0};
    snprintf(string, 12, "%2.2f", _player->getVerticalImpulse());
    
    auto valueLabel = dynamic_cast<Label*>(getChildByTag(INPUT_VERTICAL_IMPULSE));
    
    valueLabel->setString(string);
}

extension::ControlSlider* InputMenuItem::horLimitSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Input", "horizontalLimit", "8.0");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(16.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(InputMenuItem::horLimitSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void InputMenuItem::horLimitSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    _player->setHorizontalVelLimit(scale);
    
    char string[12] = {0};
    snprintf(string, 12, "%2.2f", _player->getHorizontalVelLimit());
    
    auto valueLabel = dynamic_cast<Label*>(getChildByTag(INPUT_HORIZONTAL_LIMIT));
    
    valueLabel->setString(string);
}

#endif // EDITOR_MODE

