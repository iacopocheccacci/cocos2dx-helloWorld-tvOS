//
//  GoatskinMenuItem.cpp
//  SOP_Proto
//
//  Created by Iacopo Checcacci on 22/01/15.
//
//

#ifdef EDITOR_MODE

#include "GoatskinMenuItem.h"

#include "VisibleRect.h"
#include "Utility.h"

GoatskinMenuItem::GoatskinMenuItem()
{
    
}

GoatskinMenuItem::~GoatskinMenuItem()
{
    removeAllChildren();
}

bool GoatskinMenuItem::init()
{
    bool initDone = false;
    
    if (EditorMenuItem::init())
    {
        initDone = true;
    }
    
    return initDone;
}

void GoatskinMenuItem::initLevelVar()
{
    char buff[3];
    auto s = this->getContentSize();
    
    auto goatskinRangeLabel = Label::createWithSystemFont("Range:", "Marker Felt", 15);
    goatskinRangeLabel->setColor(Color3B::BLACK);
    goatskinRangeLabel->setPosition(Point(s.width / 12.0f, s.height /  1.45f));
    addChild(goatskinRangeLabel);
    
    _goatskinRangeSliderCtl = goatskinRangeActivation();
    _goatskinRangeSliderCtl->setPosition(Vec2(s.width / 4.0f, s.height / 1.45f));
    addChild(_goatskinRangeSliderCtl, 0, TYPE_GOATSKIN_RANGE);
    
    snprintf(buff, 3, "%.2f", _goatskinRangeSliderCtl->getValue());
    auto goatskinRangeValue = Label::createWithSystemFont(buff, "Marker Felt", 15);
    goatskinRangeValue->setColor(Color3B::BLACK);
    goatskinRangeValue->setPosition(Point(s.width / 2.5f, s.height / 1.45f));
    addChild(goatskinRangeValue, 0, GOATSKIN_RANGE);
}

extension::ControlSlider* GoatskinMenuItem::goatskinRangeActivation()
{
    extension::ControlSlider * slider = extension::ControlSlider::create("sliderTrack.png","sliderProgress.png" ,"sliderThumb.png");
    
    auto value = Utility::loadDataFromFile("Goatskin", "range", "200.0");
    
    slider->setScale(0.7f);
    slider->setMinimumValue(300.0f);
    slider->setMaximumValue(800.0f);
    slider->setValue(atof(value->getCString()));
    
    slider->addTargetWithActionForControlEvents(this, cccontrol_selector(GoatskinMenuItem::goatskinRangeSliderAction), extension::Control::EventType::VALUE_CHANGED);
    
    return slider;
}

void GoatskinMenuItem::goatskinRangeSliderAction(Ref* sender, extension::Control::EventType controlEvent)
{
    bool foundAtLeastOne = false;
    extension::ControlSlider* pSliderCtl = static_cast<extension::ControlSlider*>(sender);
    float scale;
    scale = pSliderCtl->getValue();
    
    Ref* ref = nullptr;
    Goatskin* goatskin = nullptr;
    
    CCARRAY_FOREACH(_scene->getGoatskins(), ref)
    {
        goatskin = dynamic_cast<Goatskin*>(ref);
        if (goatskin)
        {
            goatskin->setRange(scale);
            if (!foundAtLeastOne)
            {
                char string[12] = {0};
                snprintf(string, 12, "%2.2f", goatskin->getRange());
                auto valueLabel = (Label *)getChildByTag(GOATSKIN_RANGE);
                valueLabel->setString(string);
                
                foundAtLeastOne = true;
            }
        }
    }
}

#endif // EDITOR_MODE
