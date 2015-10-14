//
//  LightningBolt.h
//  TestBed
//
//  Created by Leonardo Passeri on 26/06/15.
//
//

#ifndef __TestBed__LightningBolt__
#define __TestBed__LightningBolt__

#include "cocos2d.h"
#include "LightningLine.h"

USING_NS_CC;

class LightningBolt : public Node
{
    
public:
    
    static LightningBolt * create(Point source, Point dest);
    static LightningBolt * create(Point source, Point dest, Color3B color);
    static LightningBolt * create(Point source, Point dest, Color3B color, float thickness);
    
    bool init(Point source, Point dest, Color3B color, float thickness);
    
    Point getIntermediatePoint(float percentage);
    
    // Accessors
    void setAlpha(float alpha)                     { _alpha = alpha; }
    float getAlpha()                               { return _alpha; }
    
    void setFadeOutRate(float fadeOutRate)         { _fadeOutRate = fadeOutRate; }
    float getFadeOutRate()                         { return _fadeOutRate; }
    
    bool isComplete()                              { return _alpha <= 0; };
    
private:
    
    Vector<LightningLine *> createBolt(Point source, Point dest, Color3B color, float thickness);
    
    void update(float dt);
    
    Vector<LightningLine *> _segments;
    
    float _alpha;
    float _fadeOutRate;
};

#endif /* defined(__TestBed__LightningBolt__) */
