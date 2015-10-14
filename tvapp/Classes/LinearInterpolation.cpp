//
//  LinearInterpolation.cpp
//  SongOfPan
//
//  Created by Leonardo Passeri on 22/06/15.
//
//

#include "LinearInterpolation.h"

LinearInterpolation::LinearInterpolation(std::vector<Point> points)
{
    _curveLength = 0;
    if (points.size() > 0)
    {
        Point prevPoint = points.at(0);
        for (int i = 1; i < points.size(); i++)
        {
            Step currentStep(prevPoint, points.at(i));
            _steps.push_back(currentStep);
            _curveLength += currentStep.length;
            prevPoint = points.at(i);
        }
    }
}

bool LinearInterpolation::getValue(const float percentage, Point& resultPoint)
{
    resultPoint = Point::ZERO;
    float distance = 0;
    bool res = false;
    if (percentage >= 0 && percentage <= 1.0f)
    {
        float position = percentage * _curveLength;
        auto iter = _steps.begin();
        while (false == res && iter != _steps.end())
        {
            Step step = (*iter);
            if (position <= distance + step.length)
            {
                float alpha = (position - distance) / step.length;
                resultPoint = step.start.lerp(step.end, alpha);
                res = true;
            }
            distance += step.length;
            iter++;
        }
    }
    return res;
}