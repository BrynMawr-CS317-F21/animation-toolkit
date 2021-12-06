#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class A10Unique : public atkui::Framework
{
public:
   A10Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~A10Unique() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/idle.bvh", _skeleton, _motion);
      _motion.update(_skeleton, 0);
      _lfootPos = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();
      _rfootPos = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
      _lfootRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
      _rfootRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();
      _drawer.color = vec3(.1,.9,0);
   }

   void update()
   {
      _motion.update(_skeleton, elapsedTime());

      IKController ik;
      Joint* head = _skeleton.getByName("Beta:Head");
      vec3 target = head->getGlobalTranslation() + vec3(0,40.0f*sin(3.0f*elapsedTime()), 100);
      vec3 z = normalize(target - head->getGlobalTranslation());
      vec3 x = normalize(cross(vec3(0, 1, 0),z));
      vec3 y = normalize(cross(z,x));
      mat3 rotMatrix = mat3(x,y,z);
      quat rotQuat = quat_cast(rotMatrix);
      head->setLocalRotation(rotQuat);
      head->fk();

      _lhandTarget = vec3(40.0f*sin(3.0f*elapsedTime()),40.0f,0);
      _rhandTarget = vec3(40.0f*sin(3.0f*elapsedTime()),40.0f,0);
      _lhandTarget += _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();
      _rhandTarget += _skeleton.getByName("Beta:RightHand")->getGlobalTranslation();
      // setColor(vec3(0, 1, 0));
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);

      _llegTarget = vec3(50.0f*sin(3.0f*elapsedTime()),0,0);
      _rlegTarget = vec3(50.0f*sin(3.0f*elapsedTime()),0,0);
      _llegTarget += _skeleton.getByName("Beta:LeftLeg")->getGlobalTranslation();
      _rlegTarget += _skeleton.getByName("Beta:RightLeg")->getGlobalTranslation();

      std::vector<Joint*> lhandChain;
      for (Joint* current = _skeleton.getByName("Beta:LeftHand"); 
            current->getParent() && lhandChain.size() < 4; 
            current = current->getParent()) {
        lhandChain.push_back(current);
      }
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:LeftHand")->getID(),_lhandTarget,lhandChain, 0, 30, 0.1);

      std::vector<Joint*> rhandChain;
      for (Joint* current = _skeleton.getByName("Beta:RightHand"); 
            current->getParent() && rhandChain.size() < 4; 
            current = current->getParent()) {
        rhandChain.push_back(current);
      }
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:RightHand")->getID(),_rhandTarget,rhandChain, 0, 30, 0.1);

      for(int i = 0; i < _motion.getNumKeys(); i++){
         Pose pose = _motion.getKey(i);
         pose.rootPos[1] = _motion.getKey(0).rootPos[1] + 4.0f * sin(i/3.0f);
         _motion.editKey(i, pose);
      }

      std::vector<Joint*> llegChain;
      for (Joint* current = _skeleton.getByName("Beta:LeftLeg"); 
            current->getParent() && llegChain.size() < 4; 
            current = current->getParent()) {
        llegChain.push_back(current);
      }
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:LeftLeg")->getID(),_llegTarget,llegChain, 0, 30, 0.1);

      std::vector<Joint*> rlegChain;
      for (Joint* current = _skeleton.getByName("Beta:RightLeg"); 
            current->getParent() && rlegChain.size() < 4; 
            current = current->getParent()) {
        rlegChain.push_back(current);
      }
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:RightLeg")->getID(),_rlegTarget,rlegChain, 0, 30, 0.1);

      ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:LeftFoot")->getID(), _lfootPos, 0.01);
      ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:RightFoot")->getID(), _rfootPos, 0.01);
      
      quat leftFootRot = inverse( _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation()) * _lfootRot;
      quat rightFootRot = inverse(_skeleton.getByName("Beta:RightFoot")->getGlobalRotation()) * _rfootRot;
      leftFootRot = leftFootRot * _skeleton.getByName("Beta:LeftFoot")->getLocalRotation();
      rightFootRot = rightFootRot * _skeleton.getByName("Beta:RightFoot")->getLocalRotation();
      _skeleton.getByName("Beta:LeftFoot")->setLocalRotation(leftFootRot);
      _skeleton.getByName("Beta:RightFoot")->setLocalRotation(rightFootRot);
      _skeleton.fk();
   }

   void scene()
   {  
      update();
      _drawer.draw(_skeleton, *this);
      // setColor(vec3(0,0,1));
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Motion _motion;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;
   vec3 _hipTarget;
   vec3 _llegTarget;
   vec3 _rlegTarget;
   // foot position & rotation
   vec3 _lfootPos;
   vec3 _rfootPos;
   quat _lfootRot;
   quat _rfootRot;
};

int main(int argc, char** argv)
{
   A10Unique viewer;
   viewer.run();
   return 0;
}



