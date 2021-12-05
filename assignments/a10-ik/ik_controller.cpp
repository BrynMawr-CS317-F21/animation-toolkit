#include "ik_controller.h"
#include <cmath>
#include <iostream>

using namespace atk;
using namespace glm;
using namespace std;

// solveIKAnalytic: positions the joint given by jointid so its global position
// is located at goalPos. This method computes rotations for the parent and grandparent
// of jointid.
//
// param skeleton: the character to modify
// param jointid: the ID of the end effector
// param goalPos: the target position for jointid (global pos)
//
// return true/false based on whether we could reach the goal
// side effect: skeleton should by posed such that jointid is located at goalPos (or in this direction of
// goalPos if the target is out of reach)
bool IKController::solveIKAnalytic(Skeleton& skeleton, 
    int jointid, const vec3& goalPos, float epsilon) {
  if (jointid == -1) return true;

  Joint* ankle = skeleton.getByID(jointid);
  if (!ankle->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* knee = ankle->getParent();
  if (!knee->getParent()) {
    std::cout << "Warning: solveIKAnalytic() needs joint "
      "with parent and grandparent\n";
    return false;
  }

  Joint* hip = knee->getParent();

  //solve for grandparent joint using CCD
  vec3 limbDir = ankle->getGlobalTranslation() - hip->getGlobalTranslation();
  vec3 errorDir = goalPos - ankle->getGlobalTranslation();
  if(length(errorDir)<epsilon) return true;
  vec3 axis = cross(limbDir, errorDir);
  float angleRot = atan2(length(axis), dot(limbDir,limbDir)+dot(limbDir,errorDir));
  axis = normalize(axis);
  axis = inverse(hip->getParent()->getGlobalRotation())*axis;
  quat grandPRot = angleAxis(angleRot, axis);
  skeleton.getByID(hip->getID())->setLocalRotation(grandPRot * hip->getLocalRotation());
  skeleton.fk();

  //use law of cosines to solve rot for parent node
  float r = length(goalPos - hip->getGlobalTranslation());
  float l1 = length(knee->getLocalTranslation());   
  float l2 = length(ankle->getLocalTranslation());
  float v = (r * r - l1 * l1 - l2 * l2)/((-2.0f)*l1*l2);
  float value = clamp(v, -1.0f, 1.0f);
  float theta = acos(value);
  float theta2z = theta - M_PI; //computed angle
  limbDir = normalize(knee->getLocalTranslation());
  axis = cross(limbDir, vec3(0,0,-1));
  if (limbDir[1] < 0) axis = cross(limbDir, vec3(0,0,1));
  if (length(axis) < epsilon) axis = limbDir; //when axis and limbDir are parallel
  quat parentRot = angleAxis(theta2z, axis);
  skeleton.getByID(knee->getID())->setLocalRotation(parentRot);
  skeleton.fk();

  if(length(goalPos - ankle->getGlobalTranslation())>epsilon) return false;
  return true;
}

// solveIKCCD positions the joint given by jointid so its global position
// is located at goalPos
//
// param skeleton: the character to modify
// param jointid: the ID of the joint to pose
// param goalPos: the target position for jointid (global pos)
// param chain: the list of joints to "nudge" towards the goal
// param threshold: when the given joint is within threshold of the goal, stop iterating
// param maxIters: the max number of iterations to try to reach the goal
//
// return true/false based on whether we could reach the goal
// side effect: skeleton should by posed such that jointid is located at goalPos (or in this direction of
// goalPos if the target is out of reach)
bool IKController::solveIKCCD(Skeleton& skeleton, int jointid, 
    const vec3& goalPos, const std::vector<Joint*>& chain, 
    float threshold, int maxIters, float nudgeFactor) {
  if (chain.size() == 0) return true;

  //the initial position of p
  vec3 p = vec3(0);

  for(int j = 0; j < maxIters; j++){ //count for number of iterations
    //from end effector to root
    for(int i = 0; i < chain.size(); i++){
      //compute end effectors position
      p = skeleton.getByID(jointid)->getGlobalTranslation();
      //get the position of end effector and compute limb direction in global coordinate
      vec3 e = goalPos - p; 
      if(length(e) < threshold) 
        return true;
      vec3 r = p - chain[i]->getGlobalTranslation(); //compute error direction
      if(length(r) < threshold) 
        continue;
      if(length(cross(r,e)) < threshold) continue;
      if((dot(r,r) + dot(r,e))==0) continue;
      float angleRot = nudgeFactor * atan2(length(cross(r,e)), dot(r,r)+dot(r,e)); //get the angle to rotate
      vec3 axis = normalize(cross(r,e)); // compute the axis in global coordinate
      //deal with the chance that when axis are 0
      //convert the axis into local coordinate
      if(chain[i]->getID()==0) //when root
        axis = inverse(chain[i]->getGlobalRotation()) * axis;
      else
        axis = inverse(chain[i]->getParent()->getGlobalRotation()) * axis;
      quat rot = angleAxis(angleRot, axis);
      skeleton.getByID(chain[i]->getID())->setLocalRotation(rot * chain[i]->getLocalRotation());
      skeleton.fk(); //update
    }
  }
  return false;
}
