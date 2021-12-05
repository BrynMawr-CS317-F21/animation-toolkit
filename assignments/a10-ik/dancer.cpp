#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "cyclops.h"
#include "ik_controller.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace glm;
using namespace std;

class ADancer : public atkui::Framework
{
public:
   ADancer() : atkui::Framework(atkui::Perspective) {}
   virtual ~ADancer() {}

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

      _lhandTarget = vec3(0, 40.0f*(sin(3.0f*elapsedTime())+1.0f),0);
      _rhandTarget = vec3(0, 40.0f*(-sin(3.0f*elapsedTime())+1.0f),0);
      _lhandTarget += _skeleton.getByName("Beta:LeftHand")->getGlobalTranslation();
      _rhandTarget += _skeleton.getByName("Beta:RightHand")->getGlobalTranslation();
      // setColor(vec3(0, 1, 0));
      // drawSphere(_lhandTarget, 10);
      // drawSphere(_rhandTarget, 10);

      //lefthand
      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:LeftHand")->getID(),_lhandTarget, 0);
      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:RightHand")->getID(),_lhandTarget, 0);
      
      vec3 leftFoot = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();
      vec3 rightFoot = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();

      for(int i = 0; i < _motion.getNumKeys(); i++){
         Pose pose = _motion.getKey(i);
         pose.rootPos[0] = 4.0f * sin(i/3.0f);
         pose.rootPos[1] = _motion.getKey(0).rootPos[1] + 2.0f * sin(i/3.0f);
         _motion.editKey(i, pose);
      }

      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:LeftFoot")->getID(), leftFoot, 0);
      // ik.solveIKAnalytic(_skeleton,_skeleton.getByName("Beta:RightFoot")->getID(), rightFoot, 0);
      
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
};

int main(int argc, char** argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}

