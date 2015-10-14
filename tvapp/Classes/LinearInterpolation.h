//
//  LinearInterpolation.h
//  SongOfPan
//
//  Created by Leonardo Passeri on 22/06/15.
//
//

#ifndef __SongOfPan__LinearInterpolation__
#define __SongOfPan__LinearInterpolation__

#include "cocos2d.h"

USING_NS_CC;

class LinearInterpolation
{
public:
    
    LinearInterpolation(std::vector<Point> points);
    
    bool getValue(const float percentage, Point& resultPoint);
    
private:
    
    class Step
    {
    public:
        
        inline Step() {};
        
        inline Step(Point start, Point end)
        {
            this->start = start;
            this->end = end;
            this->length = start.distance(end);
        }
        
        inline Step(const Step& copy)
        {
            this->start = copy.start;
            this->end = copy.end;
            this->length = copy.length;
        }
        
        Point start;
        Point end;
        float length;
    };
    
    std::vector<Step> _steps;
    float _curveLength;
};

#endif /* defined(__SongOfPan__LinearInterpolation__) */
