//
//  LightningLine.cpp
//  TestBed
//
//  Created by Leonardo Passeri on 26/06/15.
//
//

#include "LightningLine.h"

namespace LightningLineDefs
{
    static const char* LIGHTNING_CAP_SPRITE_NAME = "LightningHalfCircle";
    static const char* LIGHTNING_SEGMENT_SPRITE_NAME = "LightningSegment";
}

LightningLine * LightningLine::create(Node * parent, Point start, Point end, Color3B color)
{
    return LightningLine::create(parent, start, end, color, 1.0f);
}

LightningLine * LightningLine::create(Node * parent, Point start, Point end, Color3B color, float thickness)
{
    LightningLine * ret = new (std::nothrow) LightningLine();
    if (ret && ret->init(parent, start, end, color, thickness))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LightningLine::init(Node * parent, Point start, Point end, Color3B color, float thickness)
{
    _start = start;
    _end = end;
    _color = color;
    _thickness = thickness;
    _parent = parent;
    
    this->createSprite();
    
    return true;
}

void LightningLine::createSprite()
{
    if (_parent)
    {
        Point tangent = _end - _start;
        float rotation = ( -1 ) * CC_RADIANS_TO_DEGREES(atan2(tangent.y, tangent.x));
        
        _startCap = Sprite::createWithSpriteFrameName(LightningLineDefs::LIGHTNING_CAP_SPRITE_NAME);
        _endCap = Sprite::createWithSpriteFrameName(LightningLineDefs::LIGHTNING_CAP_SPRITE_NAME);
        _lightningSegment = Sprite::createWithSpriteFrameName(LightningLineDefs::LIGHTNING_SEGMENT_SPRITE_NAME);
        
        const float imageThickness = 8.0f;
        float imageHeight = _lightningSegment->getContentSize().width;
        float thicknessScale = _thickness / imageThickness;
        float middleScaleX = tangent.length() / imageHeight;
        
        _startCap->setColor(_color);
        _endCap->setColor(_color);
        _lightningSegment->setColor(_color);
        
        _startCap->setAnchorPoint(Point::ANCHOR_MIDDLE_RIGHT);
        _endCap->setAnchorPoint(Point::ANCHOR_MIDDLE_LEFT);
        _lightningSegment->setAnchorPoint(Point::ANCHOR_MIDDLE);
        
        _startCap->setRotation(rotation);
        _endCap->setRotation(rotation);
        _lightningSegment->setRotation(rotation);
        
        _startCap->setPosition(_start);
        _endCap->setPosition(_end);
        _lightningSegment->setPosition( ( _end + _start ) / 2.0f);
        
        _startCap->setScale(thicknessScale);
        _endCap->setScale(thicknessScale);
        _endCap->setFlippedX(true);
        _lightningSegment->setScaleY(thicknessScale);
        _lightningSegment->setScaleX(middleScaleX);
        
        _parent->addChild(_startCap);
        _parent->addChild(_lightningSegment);
        _parent->addChild(_endCap);
    }
}

Vector<LightningLine *> LightningLine::createBolt(Node * parent, Point source, Point dest, Color3B color, float thickness)
{
    Vector<LightningLine *> results;
    Point tangent = dest - source;
    Point normal = Point(tangent.y, -tangent.x).getNormalized();
    float length = tangent.length();
    
    std::vector<float> positions;
    positions.push_back(0.0f);
    
    for (int i = 0; i < length / 20; i++)
        positions.push_back(CCRANDOM_0_1());
    
    std::sort(positions.begin(), positions.end());
    
    const float sway = 10.0f;//80.0f;
    const float jaggedness = 1 / sway;
    
    Point prevPoint = source;
    float prevDisplacement = 0;
    for (int i = 1; i < positions.size(); i++)
    {
        float pos = positions.at(i);
        
        // used to prevent sharp angles by ensuring very close positions also have small perpendicular variation.
        float scale = fminf(fmaxf((length * jaggedness) * (pos - positions[i - 1]), 0.0f), 1.0f);
        
        // defines an envelope. Points near the middle of the bolt can be further from the central line.
        float envelope = pos > 0.95f ? 20.0f * (1.0f - pos) : 1.0f;
        
        float displacement = CCRANDOM_MINUS1_1() * sway;
        displacement -= (displacement - prevDisplacement) * (1.0f - scale);
        displacement *= envelope;
        
        Point point = source + pos * tangent + displacement * normal;
        results.pushBack(LightningLine::create(parent, prevPoint, point, color, thickness));
        prevPoint = point;
        prevDisplacement = displacement;
    }
    
    results.pushBack(LightningLine::create(parent, prevPoint, dest, color, thickness));
    
    return results;
}

void LightningLine::setLineOpacity(GLubyte opacity)
{
    if (_startCap)
    {
        _startCap->setOpacity(opacity);
    }
    if (_endCap)
    {
        _endCap->setOpacity(opacity);
    }
    if (_lightningSegment)
    {
        _lightningSegment->setOpacity(opacity);
    }
}

float LightningLine::getLength()
{
    return (_end - _start).length();
}