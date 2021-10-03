#ifndef _interpolator_hermite_
#define _interpolator_hermite_

#include <string>
#include <vector>
#include <Eigen/Dense>
#include "interpolator.h"

//--------------------------------
// Hermite 
//--------------------------------

class InterpolatorHermite : public Interpolator
{
public:
    InterpolatorHermite() : Interpolator("Hermite"), mClampDir(glm::vec3(1,0,0)) {}

    virtual glm::vec3 interpolate(int segment, double u) const {
       glm::vec3 temp;
       glm::vec3 returnVec;
       float v = (float) u;
       float H0 = 1.0f - 3.0f * v * v + 2.0f * v * v * v;
       float H1 = v * (1.0f - v) * (1.0f - v);
       float H2 = - v * v + v * v * v;
       float H3 = 3.0f * v * v - 2.0f * v * v * v;
       if(isClamped()){
           if(segment == 0){
               returnVec = mCtrlPoints[0] * H0 + mClampDir * H1 + mCtrlPoints[3] * H2 + mCtrlPoints[2] * H3;
           } else if (segment == mCtrlPoints.size()-1){
               returnVec = mCtrlPoints[segment * 2] * H0 + mCtrlPoints[segment*2 + 1] * H1 
                + mCtrlPoints[segment*2 + 2] * H3 + mClampDir * H2;
           } else {
               returnVec = mCtrlPoints[segment * 2] * H0 + mCtrlPoints[segment*2 + 1] * H1 
                + mCtrlPoints[segment*2 + 3] * H2 + mCtrlPoints[segment*2+2] * H3;
           }
       } else {
           returnVec = mCtrlPoints[segment * 2] * H0 + mCtrlPoints[segment*2 + 1] * H1 
            + mCtrlPoints[segment*2 + 3] * H2 + mCtrlPoints[segment*2+2] * H3;
       }
       return returnVec;
    }

    virtual void computeControlPoints(const std::vector<glm::vec3>& keys) {
       clearControlPoints();
       int d = keys.size(); // we need to calculate for the control points
       Eigen::MatrixXd A(d, d);
       Eigen::MatrixXd p(d, 3);
       Eigen::MatrixXd pPrime(d, 3); // slopes for each control point
       //natural endpoints
       for (int i = 0; i < keys.size(); i++){
            for (int j= 0; j < keys.size(); j++){
                if ( i == j-1 || i == j+1 ){
                    A(i,j) = 1;
                } else if (i == j && i!=0 && i != keys.size()-1){
                    A(i,j) = 4;
                } else if (i == j && (i == 0 || i == keys.size()-1)){ 
                    A(i,j) = 2;
                } else {
                    A(i,j) = 0;
                }
            }
       }
       glm::vec3 temp;
       for (int i = 0; i < keys.size(); i++){
           if(i == 0){
               temp = keys[i+1] - keys[i];
           } else if (i == keys.size()-1){
               temp = keys[i] - keys[i-1];
           } else {
               temp = keys[i+1] - keys[i-1];
           }
           p(i,0) = 3.0f * temp[0];
           p(i,1) = 3.0f * temp[1];
           p(i,2) = 3.0f * temp[2];
       }
       pPrime = A.inverse() * p;
       for (int i = 0; i < keys.size(); i++){
           mCtrlPoints.push_back(keys[i]);
           temp = glm::vec3(pPrime(i,0),pPrime(i,1),pPrime(i,2));
           mCtrlPoints.push_back(temp);
       }
    }

    void setClamped(bool c) { mIsClamped = c; }
    bool isClamped() const { return mIsClamped; }

    void setClampedDirection(const glm::vec3& v) { mClampDir = v; }
    glm::vec3 getClampDirection() const { return mClampDir; }

private:
    bool mIsClamped = true;
    glm::vec3 mClampDir;

};

#endif
