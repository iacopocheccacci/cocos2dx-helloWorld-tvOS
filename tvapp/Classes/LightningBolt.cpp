//
//  LightningBolt.cpp
//  TestBed
//
//  Created by Leonardo Passeri on 26/06/15.
//
//

#include "LightningBolt.h"


LightningBolt * LightningBolt::create(Point source, Point dest)
{
    return LightningBolt::create(source, dest, Color3B(Color4F(0.9f, 0.8f, 1.0f, 1.0f)));
}

LightningBolt * LightningBolt::create(Point source, Point dest, Color3B color)
{
    return LightningBolt::create(source, dest, color, 2.0f);
}

LightningBolt * LightningBolt::create(Point source, Point dest, Color3B color, float thickness)
{
    LightningBolt * ret = new (std::nothrow) LightningBolt();
    if (ret && ret->init(source, dest, color, thickness))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LightningBolt::init(Point source, Point dest, Color3B color, float thickness)
{
    bool res = Node::init();
    if (res)
    {
        _segments = this->createBolt(source, dest, color, thickness);
        
        _alpha = 1.0f;
        _fadeOutRate = 0.03f;
        
        this->scheduleUpdate();
    }
    return res;
}

void LightningBolt::update(float dt)
{
    _alpha -= _fadeOutRate;
    
    if (!this->isComplete())
    {
        for (auto iter = _segments.begin(); iter != _segments.end(); iter++)
        {
            (*iter)->setLineOpacity(_alpha * 255);
        }
    }
    else
    {
        this->removeFromParentAndCleanup(true);
    }
}

Vector<LightningLine *> LightningBolt::createBolt(Point source, Point dest, Color3B color, float thickness)
{
    return LightningLine::createBolt(this, source, dest, color, thickness);
}

Point LightningBolt::getIntermediatePoint(float percentage)
{
    // Versione spartana, con punti intermedi discretizzati ad una giunzione tra un segmento e l'altro
    size_t index = (size_t)_segments.size() * percentage;
    if (index == _segments.size())
    {
        index--;
    }
    return _segments.at(index)->getStartPoint();
}