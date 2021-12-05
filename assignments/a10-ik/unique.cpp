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
      _drawer.color = vec3(1,0,0);
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
      setColor(vec3(0, 1, 0));
      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);

      _hipTarget = vec3(0,5.0f*sin(3.0f*elapsedTime()),0);
      _hipTarget += _skeleton.getByName("Beta:Hips")->getGlobalTranslation();

      _llegTarget = vec3(40.0f*sin(3.0f*elapsedTime()),40.0f,0);
      _rlegTarget = vec3(40.0f*sin(3.0f*elapsedTime()),40.0f,0);
      _llegTarget += _skeleton.getByName("Beta:LeftLeg")->getGlobalTranslation();
      _rlegTarget += _skeleton.getByName("Beta:RightLeg")->getGlobalTranslation();
      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:LeftHand")->getID(),_lhandTarget, 0.1);
      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:RightHand")->getID(),_lhandTarget, 0.1);
   }

   void scene()
   {  
      update();
      _drawer.draw(_skeleton, *this);
      setColor(vec3(0,0,1));
      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);
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
};

int main(int argc, char** argv)
{
   A10Unique viewer;
   viewer.run();
   return 0;
}



