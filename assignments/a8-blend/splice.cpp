#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class ASplice : public atkui::Framework
{
public:
   ASplice() : atkui::Framework(atkui::Perspective) {}
   virtual ~ASplice() {}

   void setup()
   {
      _alpha = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      _splice = spliceUpperBody(_lower, _upper, _alpha);
   }

   Motion spliceUpperBody(const Motion& lower, const Motion& upper, float alpha)
   {
      Motion result;
      Pose newPose;
      Joint* spine = _skeleton.getByName("Beta:Spine1");
      result.setFramerate(lower.getFramerate());
      for(int i = 0; i < lower.getNumKeys(); i++){
         newPose = lower.getKey(i);
         for(int j = spine->getID(); j<spine->getID()+countChild(spine); j++){
            glm::quat q = slerp(lower.getKey(i).jointRots[j], upper.getKey(i+120).jointRots[j], alpha);
            newPose.jointRots[j] = q;
         }
         result.appendKey(newPose);
      }
      return result;
   }

   int countChild(Joint* a){
      if(a->getNumChildren()>0){
         int tmp = 1;
         for(int i = 0; i<a->getNumChildren();i++){
            tmp+=countChild(a->getChildAt(i));
         }
         return tmp;
      }
      return 1;
   }

   void scene()
   {  
      _splice.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: "+std::to_string(_alpha), 10, 15);
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_UP)
      {
         _alpha = std::min(_alpha+0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN)
      {
         _alpha = std::max(_alpha-0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
   }

   Skeleton _skeleton;
   Motion _upper;
   Motion _lower;
   Motion _splice;
   float _alpha;
};

int main(int argc, char** argv)
{
   ASplice viewer;
   viewer.run();
   return 0;
}

