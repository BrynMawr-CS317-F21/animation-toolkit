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
      _motion.update(_skeleton, 0);
      _lfootPos = _skeleton.getByName("Beta:LeftFoot")->getGlobalTranslation();
      _rfootPos = _skeleton.getByName("Beta:RightFoot")->getGlobalTranslation();
      _lfootRot = _skeleton.getByName("Beta:LeftFoot")->getGlobalRotation();
      _rfootRot = _skeleton.getByName("Beta:RightFoot")->getGlobalRotation();
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
         pose.rootPos[0] = 4.0f * sin(i/3.0f);
         pose.rootPos[1] = _motion.getKey(0).rootPos[1] + 2.0f * sin(i/3.0f);
         _motion.editKey(i, pose);
      }

      std::vector<Joint*> lfootChain;
      for (Joint* current = _skeleton.getByName("Beta:LeftFoot"); 
            current->getParent() && lfootChain.size() < 3; 
            current = current->getParent()) {
         lfootChain.push_back(current);
      }
      std::vector<Joint*> rfootChain;
      for (Joint* current = _skeleton.getByName("Beta:RightFoot"); 
            current->getParent() && rfootChain.size() < 3; 
            current = current->getParent()) {
        rfootChain.push_back(current);
      }
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:LeftFoot")->getID(), _lfootPos, lfootChain, 0, 30, 0.1);
      ik.solveIKCCD(_skeleton,_skeleton.getByName("Beta:RightFoot")->getID(), _rfootPos, rfootChain, 0, 30, 0.1);
      
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
      setColor(vec3(0,0,1));
      drawSphere(_lhandTarget, 10);
      drawSphere(_rhandTarget, 10);
   }

protected:
   Cyclops _drawer;
   Skeleton _skeleton;
   Skeleton _skeleton2;
   Motion _motion;

   // Hand target positions
   vec3 _lhandTarget;
   vec3 _rhandTarget;
   vec3 _lfootPos;
   vec3 _rfootPos;
   quat _lfootRot;
   quat _rfootRot;
};

int main(int argc, char** argv)
{
   ADancer viewer;
   viewer.run();
   return 0;
}

