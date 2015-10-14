//
//  LightningBranch.h
//  TestBed
//
//  Created by Leonardo Passeri on 27/06/15.
//
//

#ifndef __TestBed__LightningBranch__
#define __TestBed__LightningBranch__

#include "cocos2d.h"
#include "LightningBolt.h"

USING_NS_CC;

class LightningBranch : public Node
{
public:
    
    static LightningBranch * create(Point start, Point end);
    static LightningBranch * create(Point start, Point end, Color3B color);
    static LightningBranch * create(Point start, Point end, Color3B color, float thickness);
    static LightningBranch * create(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints);
    
private:
    
    bool init(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints);
    
    void createBranch(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints);
    
    void update(float delta);
    
    Point _end;
    Point _direction;
};

#endif /* defined(__TestBed__LightningBranch__) */
