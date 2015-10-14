//
//  LightningBranch.cpp
//  TestBed
//
//  Created by Leonardo Passeri on 27/06/15.
//
//

#include "LightningBranch.h"

LightningBranch * LightningBranch::create(Point start, Point end)
{
    return LightningBranch::create(start, end, Color3B(Color4F(0.9f, 0.8f, 1.0f, 1.0f)));
}

LightningBranch * LightningBranch::create(Point start, Point end, Color3B color)
{
    return LightningBranch::create(start, end, color, 2.0f);
}

LightningBranch * LightningBranch::create(Point start, Point end, Color3B color, float thickness)
{
    int numBranches = RandomHelper::random_int(2, 4);
    
    // pick a bunch of random points between 0 and 1 and sort them
    std::vector<float> branchPoints;
    for (int i = 0; i < numBranches; i++)
    {
        branchPoints.push_back(RandomHelper::random_real(0.0f, 1.0f));
    }
    
    return LightningBranch::create(start, end, color, thickness, branchPoints);
}

LightningBranch * LightningBranch::create(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints)
{
    LightningBranch * ret = new (std::nothrow) LightningBranch();
    if (ret && ret->init(start, end, color, thickness, branchPoints))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool LightningBranch::init(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints)
{
    bool res = Node::init();
    if (res)
    {
        _end = end;
        _direction = (end - start).getNormalized();
        this->createBranch(start, end, color, thickness, branchPoints);
        
        this->scheduleUpdate();
    }
    return res;
}

void LightningBranch::createBranch(Point start, Point end, Color3B color, float thickness, std::vector<float> branchPoints)
{
    auto mainBolt = LightningBolt::create(start, end, color, thickness);
    this->addChild(mainBolt);
    
    Point direction = (end - start).getNormalized();
    
    std::sort(branchPoints.begin(), branchPoints.end());
    
    for (unsigned int i = 0; i < branchPoints.size(); i++)
    {
        // Bolt::getIntermediatePoint() gets the position of the lightning bolt at specified fraction (0 = start of bolt, 1 = end)
        Point boltStart = mainBolt->getIntermediatePoint(branchPoints.at(i));
        
        // rotate 30 degrees. Alternate between rotating left and right.
        Point boltDirection = direction;
        boltDirection.rotate(Point::ZERO, CC_DEGREES_TO_RADIANS(30 * ((i & 1) == 0 ? 1 : -1)));
        float boltLength = (end - boltStart).length();
        Point boltEnd = boltStart + boltDirection * boltLength;
        
        this->addChild(LightningBolt::create(boltStart, boltEnd, color, thickness));
    }
}

void LightningBranch::update(float delta)
{
    if (this->getChildrenCount() == 0)
    {
        this->removeFromParentAndCleanup(true);
    }
}
