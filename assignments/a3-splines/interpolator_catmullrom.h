#ifndef _interpolator_catmullrom_
#define _interpolator_catmullrom_

#include <string>
#include <vector>
#include "interpolator.h"

//--------------------------------
// Catmull-Rom 
//--------------------------------

class InterpolatorCatmullRom : public Interpolator
{
public:
    InterpolatorCatmullRom() : Interpolator("Catmull-Rom") {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       //this help us to find the start poistion of the controlpoints in the mCtrlPoints
       float v = (float)u;
       int start = segment * 3;
       return (1-v) * (1-v) * (1-v) * mCtrlPoints[start] + 3.0f * (1-v) * (1-v) * v * mCtrlPoints[start+1]
         + 3.0f * v * v * (1-v) * mCtrlPoints[start+2] + v * v * v * mCtrlPoints[start + 3];
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       clearControlPoints();
       float a = 1.0f/6.0f;
       for (int i = 0; i < keys.size()-1; i++){
          //check if it is the start point or end point
          if (i == 0){
             mCtrlPoints.push_back(keys[0]);
             mCtrlPoints.push_back(keys[0] + a * keys[1] - a * keys[0]);
             mCtrlPoints.push_back(keys[1] - a * keys[2] + a * keys[0]);
          } else if (i == keys.size()-2){
             mCtrlPoints.push_back(keys[i]);
             mCtrlPoints.push_back(keys[i] + a * keys[i+1] - a * keys[i-1]);
             mCtrlPoints.push_back(keys[i+1] - a * keys[i+1] + a * keys[i]);
             mCtrlPoints.push_back(keys[i+1]);
          } else {
             mCtrlPoints.push_back(keys[i]);
             mCtrlPoints.push_back(keys[i] + a * keys[i+1] - a * keys[i-1]);
             mCtrlPoints.push_back(keys[i+1] - a * keys[i+2] + a * keys[i]);
          }
       }
    }
};

#endif
