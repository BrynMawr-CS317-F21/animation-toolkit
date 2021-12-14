#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace glm;
using namespace atk;
using namespace atkui;

class AZombieArms : public atkui::Framework {
public:
   AZombieArms() : atkui::Framework(atkui::Perspective) {}
   virtual ~AZombieArms() {}

   void setup() {
      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _original);

      _zombieFreeze = ComputeArmFreeze(_original);
      _zombieOffset = ComputeArmOffset(_original);
   }

   Motion ComputeArmOffset(const Motion& motion) {
      quat leftLocalRot = eulerAngleRO(XYZ, radians(vec3(-53, -88, -33)));
      quat rightLocalRot = eulerAngleRO(XYZ, radians(vec3(14, 88, -33)));
      quat elbowLocalRot = eulerAngleRO(XYZ, radians(vec3(0, 23, 0)));

      Joint* leftArm = _skeleton.getByName("Beta:LeftArm");
      Joint* rightArm = _skeleton.getByName("Beta:RightArm");

      Joint* leftElbow = _skeleton.getByName("Beta:LeftForeArm");
      Joint* rightElbow = _skeleton.getByName("Beta:RightForeArm");

      int lAid = leftArm->getID();
      int rAid = rightArm->getID();
      int lEid = leftElbow->getID();
      int rEid = rightElbow->getID();
      
      quat leftOffset = leftLocalRot * inverse(motion.getKey(0).jointRots[lAid]);
      quat rightOffset = rightLocalRot * inverse(motion.getKey(0).jointRots[rAid]);
      quat leftElbowOffset = elbowLocalRot * inverse(motion.getKey(0).jointRots[lEid]);
      quat rightElbowOffset = elbowLocalRot * inverse(motion.getKey(0).jointRots[rEid]);

      Motion result;
      result.setFramerate(motion.getFramerate());
      for(int i = 0; i < motion.getNumKeys(); i++){
         Pose tmp = motion.getKey(i);
         tmp.jointRots[lAid] = leftOffset * tmp.jointRots[lAid];
         tmp.jointRots[rAid] = rightOffset * tmp.jointRots[rAid];
         tmp.jointRots[lEid] = leftElbowOffset * tmp.jointRots[lEid];
         tmp.jointRots[rEid] = rightElbowOffset * tmp.jointRots[rEid];
         result.appendKey(tmp);
      }
      return result;
   }

   Motion ComputeArmFreeze(const Motion& motion) {
      quat leftRot = eulerAngleRO(XYZ, radians(vec3(-53, -88, -33)));
      quat rightRot = eulerAngleRO(XYZ, radians(vec3(14, 88, -33)));
      quat elbowRot = eulerAngleRO(XYZ, radians(vec3(0, 23, 0)));

      Joint* leftArm = _skeleton.getByName("Beta:LeftArm");
      Joint* rightArm = _skeleton.getByName("Beta:RightArm");

      Joint* leftElbow = _skeleton.getByName("Beta:LeftForeArm");
      Joint* rightElbow = _skeleton.getByName("Beta:RightForeArm");

      int lAid = leftArm->getID();
      int rAid = rightArm->getID();
      int lEid = leftElbow->getID();
      int rEid = rightElbow->getID();

      Motion result;
      result.setFramerate(motion.getFramerate());
      for(int i = 0; i < motion.getNumKeys(); i++){
         Pose tmp = motion.getKey(i);
         tmp.jointRots[lAid] = leftRot;
         tmp.jointRots[rAid] = rightRot;
         tmp.jointRots[lEid] = elbowRot;
         tmp.jointRots[rEid] = elbowRot;
         result.appendKey(tmp);
      }
      return result;
   }

   void update() {
      double t =  elapsedTime() * 0.5;
      if (_currentMotion == 1) {
         _zombieFreeze.update(_skeleton, t);
         drawText("Joint edit type: freeze", 10, 15);

      } else if (_currentMotion == 2) {
         _zombieOffset.update(_skeleton, t);
         drawText("Joint edit type: offset", 10, 15);

      } else {
         _original.update(_skeleton, t);
         drawText("Joint edit type: none", 10, 15);
      }
   }

   void scene() {  
      update();
      _drawer.draw(_skeleton, *this);
   }

   void keyUp(int key, int mods) {
      if (key == '1') {
          _currentMotion = 1;
      } else if (key == '2') {
          _currentMotion = 2;
      } else if (key == '0') {
          _currentMotion = 0;
      }
   }

   SkeletonDrawer _drawer;
   Skeleton _skeleton;
   Motion _original;
   Motion _zombieFreeze;
   Motion _zombieOffset;
   int _currentMotion = 0;
};

int main(int argc, char** argv) {
   AZombieArms viewer;
   viewer.run();
   return 0;
}

