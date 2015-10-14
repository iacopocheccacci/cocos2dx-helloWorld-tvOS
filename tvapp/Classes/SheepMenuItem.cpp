//
//  SheepMenuItem.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 05/11/14.
//
//

#ifdef EDITOR_MODE

#include "SheepMenuItem.h"
#include "VisibleRect.h"
#include "Utility.h"

SheepMenuItem::SheepMenuItem()
{
    
}

SheepMenuItem::~SheepMenuItem()
{
    removeAllChildren();
}

bool SheepMenuItem::init()
{
    bool initDone = false;
    
    if (EditorMenuItem::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void SheepMenuItem::initLevelVar()
{
    char buff[3];
    auto s = this->getContentSize();
    
    auto horImpLabel = Label::createWithSystemFont("Hor. Imp.:", "Marker Felt", 15);
    horImpLabel->setColor(Color3B::BLACK);
    horImpLabel->setPosition(Point(s.width / 12.0f, s.height / 1.45f));
    addChild(horImpLabel);
    
    _horImpSliderCtl = horImpSliderCtl();
    _horImpSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 1.45f));
    addChild(_horImpSliderCtl, 0, TYPE_SHEEP_HORIZONTAL_IMPULSE);
    
    snprintf(buff, 3, "%.2f", _horImpSliderCtl->getValue());
    auto horImpValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    horImpValue->setColor(Color3B::BLACK);
    horImpValue->setPosition(Point(s.width / 2.5f, s.height / 1.45f));
    addChild(horImpValue, 0, SHEEP_HORIZONTAL_IMPULSE);
    
    /*-------------------*/
    
    auto vertImpLabel = Label::createWithSystemFont("Vert. Imp.:", "Marker Felt", 15);
    vertImpLabel->setColor(Color3B::BLACK);
    vertImpLabel->setPosition(Point(s.width / 12.0f, s.height / 3.0f));
    addChild(vertImpLabel);
    
    _vertImpSliderCtl = vertImpSliderCtl();
    _vertImpSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 3.0f));
    addChild(_vertImpSliderCtl, 0, TYPE_SHEEP_VERTICAL_IMPULSE);
    
    snprintf(buff, 3, "%.2f", _vertImpSliderCtl->getValue());
    auto vertImpValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    vertImpValue->setColor(Color3B::BLACK);
    vertImpValue->setPosition(Point(s.width / 2.5f, s.height / 3.0f));
    addChild(vertImpValue, 0, SHEEP_VERTICAL_IMPULSE);
    
    /*-------------------*/
    
    auto horLimitLabel = Label::createWithSystemFont("Hor. Limit:", "Marker Felt", 15);
    horLimitLabel->setColor(Color3B::BLACK);
    horLimitLabel->setPosition(Point(s.width * 0.6f, s.height / 1.45f));
    addChild(horLimitLabel);
    
    _horLimitSliderCtl = horLimitSliderCtl();
    _horLimitSliderCtl->setPosition(Vec2(s.width * 0.8f, s.height / 1.45f));
    addChild(_horLimitSliderCtl, 0, TYPE_SHEEP_HORIZONTAL_LIMIT);
    
    snprintf(buff, 3, "%.2f", _horLimitSliderCtl->getValue());
    auto horLimitValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    horLimitValue->setColor(Color3B::BLACK);
    horLimitValue->setPosition(Point(s.width * 0.95f, s.height / 1.45f));
    addChild(horLimitValue, 0, SHEEP_HORIZONTAL_LIMIT);
    
    /*-------------------*/
    
    auto sheepDistanceLabel = Label::createWithSystemFont("Sheep Dist.:", "Marker Felt", 15);
    sheepDistanceLabel->setColor(Color3B::BLACK);
    sheepDistanceLabel->setPosition(Point(s.width * 0.6f, s.height / 3.0f));
    addChild(sheepDistanceLabel);
    
    _sheepDistanceSliderCtl = sheepDistanceSliderCtl();
    _sheepDistanceSliderCtl->setPosition(Vec2(s.width * 0.8f, s.height / 3.0f));
    addChild(_sheepDistanceSliderCtl, 0, TYPE_SHEEP_DISTANCE);
    
    snprintf(buff, 3, "%.2f", _sheepDistanceSliderCtl->getValue());
    auto sheepDistanceValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    sheepDistanceValue->setColor(Color3B::BLACK);
    sheepDistanceValue->setPosition(Point(s.width * 0.95f, s.height / 3.0f));
    addChild(sheepDistanceValue, 0, SHEEP_DISTANCE);
}

extension::ControlSlider* SheepMenuItem::horImpSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Sheep", "horizontalImpulse", "0.54");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(1.5f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SheepMenuItem::horImpSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void SheepMenuItem::horImpSliderAction(Ref* sender, extension::Control::EventType controlEvent)
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
            sheep->setHorizontalImpulse(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", sheep->getHorizontalImpulse());
                auto valueLabel = dynamic_cast<Label*>(getChildByTag(SHEEP_HORIZONTAL_IMPULSE));
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

extension::ControlSlider* SheepMenuItem::vertImpSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Sheep", "verticalImpulse", "8.4");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(16.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SheepMenuItem::vertImpSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void SheepMenuItem::vertImpSliderAction(Ref* sender, extension::Control::EventType controlEvent)
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
            sheep->setVerticalImpulse(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", sheep->getVerticalImpulse());
                auto valueLabel = (Label *)getChildByTag(SHEEP_VERTICAL_IMPULSE);
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

extension::ControlSlider* SheepMenuItem::horLimitSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Sheep", "horizontalLimit", "6.0");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(16.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SheepMenuItem::horLimitSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void SheepMenuItem::horLimitSliderAction(Ref* sender, extension::Control::EventType controlEvent)
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
            sheep->setHorizontalVelLimit(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", sheep->getHorizontalVelLimit());
                auto valueLabel = (Label *)getChildByTag(SHEEP_HORIZONTAL_LIMIT);
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

extension::ControlSlider* SheepMenuItem::sheepDistanceSliderCtl()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Sheep", "sheepDistance", "0.2");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(0.0f);
    slider->setMaximumValue(1.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(SheepMenuItem::sheepDistanceSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void SheepMenuItem::sheepDistanceSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    _scene->setSheepDistance(scale);
    
    char string[12] = {0};
    snprintf(string, 12, "%2.2f", _scene->getSheepDistance());
    
    auto valueLabel = (Label *)getChildByTag(SHEEP_DISTANCE);
    
    valueLabel->setString(string);
}

#endif // EDITOR_MODE
