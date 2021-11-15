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
      direction = vec3(0.5, 0.3, 0);
      radius = 15.0;
      for (int i = 0; i < 1000; i++){
         struct sParticle someP;
         //setting color into random
         someP.color = agl::randomUnitVector();
         someP.speed = agl::random();
         //the reason for using spead but not velocity is that velocity have direction
         //but we dont want direction to be random, so it is not in our class
         if(i < 500)
            someP.position = vec3(agl::random() * 1000.0f, agl::random() * 1000.0f, agl::random() * 1000.0f - 50.0f);
         else 
            someP.position = vec3(-agl::random() * 1000.0f, agl::random() * 1000.0f, agl::random() * 1000.0f - 50.0f);
         //setting the start position into random
         storeP[i] = someP;
      }
      _alpha = 0;
      _beta = 0;

      BVHReader reader;
      reader.load("../motions/Beta/walking.bvh", _skeleton, _lower);
      reader.load("../motions/Beta/gangnam_style.bvh", _skeleton, _upper);
      reader.load("../motions/Beta/samba_dancing.bvh", _skeleton, _samba);
      _sambagangnan = spliceUpperBody(_lower, _samba, _beta);
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
      if(isSplice)
         _splice.update(_skeleton, elapsedTime() * 0.5);
      if(isSamba)
         _sambagangnan.update(_skeleton, elapsedTime() * 0.5);
      SkeletonDrawer drawer;
      drawer.draw(_skeleton, *this);
      drawText("alpha: "+std::to_string(_alpha), 10, 15);
      drawText("beta: "+std::to_string(_beta), 10, 35);

      for (int j = 0; j < 1000; j++){
         setColor(storeP[j].color);
         storeP[j].position = storeP[j].position + 100.0f * storeP[j].speed * direction * dt();
         //when flying out of the screen, setting the out-bounded position into 0
         if (storeP[j].position.x > 1000.0f){
            storeP[j].position.x = 0;
         }
         if (storeP[j].position.y > 1000.0f){
            storeP[j].position.y = 0;
         }
         drawSphere (storeP[j].position, radius);
      }  
   }

   void keyUp(int key, int mods) 
   {
      if (key == GLFW_KEY_1){
         isSplice = false;
         isSamba = true;
         _alpha = 0.0;
      }
      if (key == GLFW_KEY_2){
         isSplice = true;
         isSamba = false;
         _beta = 0.0;
      }
      if (key == GLFW_KEY_UP && isSplice)
      {
         _alpha = std::min(_alpha+0.05, 1.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_DOWN && isSplice)
      {
         _alpha = std::max(_alpha-0.05, 0.0);
         _splice = spliceUpperBody(_lower, _upper, _alpha);
      }
      else if (key == GLFW_KEY_UP && isSamba)
      {
         _beta = std::min(_beta+0.05, 1.0);
         _sambagangnan = spliceUpperBody(_lower, _samba, _beta);
      }
      else if (key == GLFW_KEY_DOWN && isSamba)
      {
         _beta = std::max(_beta-0.05, 0.0);
         _sambagangnan = spliceUpperBody(_lower, _samba, _beta);
      }
   }
   
   struct sParticle{
      vec3 color;
      vec3 position;
      float speed;
   };

   struct sParticle storeP[1000]; // storing particles
   vec3 direction; //set the direction of particles moving 
   float radius; // set the particle radius

   Skeleton _skeleton;
   Motion _upper;
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
   A8Unique
 viewer;
   viewer.run();
   return 0;
}

