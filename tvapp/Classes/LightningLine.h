//
//  LightningLine.h
//  TestBed
//
//  Created by Leonardo Passeri on 26/06/15.
//
//

#ifndef __TestBed__LightningLine__
#define __TestBed__LightningLine__

#include "cocos2d.h"

USING_NS_CC;

class LightningLine : public Ref
{
public:
    
    static LightningLine * create(Node * parent, Point start, Point end, Color3B color);
    static LightningLine * create(Node * parent, Point start, Point end, Color3B color, float thickness);
    
    void update(float dt);
    
    static Vector<LightningLine *> createBolt(Node * parent, Point source, Point dest, Color3B color, float thickness);
    
    void setLineOpacity(GLubyte opacity);
    
    float getLength();
    
    Point getStartPoint()                                    { return _start; }
    
private:
    
    bool init(Node * parent, Point start, Point end, Color3B color, float thickness);
    
    void createSprite();
    
    Node * _parent;
    Point _start;
    Point _end;
    Color3B _color;
    float _thickness;
    
    Sprite * _startCap;
    Sprite * _endCap;
    Sprite * _lightningSegment;
};

#endif /* defined(__TestBed__LightningLine__) */
