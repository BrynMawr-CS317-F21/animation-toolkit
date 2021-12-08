#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class A8Unique : public atkui::Framework
{
public:
   A8Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~A8Unique() {}

   void setup()
   {
      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/samba_dancing.bvh", _skeleton, _samba);
      _sambagangnan = spliceUpperBody(_lower, _samba, 1.0);
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
      lookAt(vec3(0, 400, 1000), vec3(0), vec3(0, 1, 0));
      _sambagangnan.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.jointRadius = 3.0f * drawer.jointRadius;
      drawer.color = vec3(220,220,220)/256.0f;
      drawer.draw(_skeleton, *this);
      setColor(vec3(169, 169, 169)/256.0f);
      drawSphere(vec3(0,500,-300), 400);
      push();
      translate(vec3(0,450,-300));
      rotate(M_PI/2.0f, vec3(1, 0, 0));
      scale(vec3(1, 1, 0.5));
      drawTorus(vec3(0), 500);
      pop();
      push();
      setColor(vec3(255, 255, 204)/256.0f);
      translate(vec3(0,100,-300));
      rotate(3 * M_PI/2.0f, vec3(1, 0, 0));
      drawCone(vec3(0), 1200);
      pop();
   }
   
   Skeleton _skeleton;
   Motion _lower;
   Motion _samba;
   Motion _sambagangnan;
   Motion _splice;
   float _alpha;
   float _beta;
   bool isSplice;
   bool isSamba;
};

int main(int argc, char** argv)
{
   A8Unique viewer;
   viewer.run();
   return 0;
}

