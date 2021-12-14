#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class A7Unique : public atkui::Framework
{
public:
   A7Unique() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      BVHReader reader;
      reader.load("../motions/Beta/samba_dancing.bvh", skeleton, motion);
      motion.update(skeleton, 0);
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
   }

   void scene() {
      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
            if (joint->getParent() == 0) continue;

            vec3 p1 = joint->getGlobalTranslation();
            vec3 p2 = joint->getParent()->getGlobalTranslation();
            drawEllipsoid(p1, p2, 5);
      }
      motion.update(skeleton, elapsedTime());
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

private:
   struct sParticle{
      vec3 color;
      vec3 position;
      float speed;
   };

   Skeleton skeleton;
   Motion motion;
   struct sParticle storeP[1000]; // storing particles
   vec3 direction; //set the direction of particles moving 
   float radius; // set the particle radius

};

int main(int argc, char** argv) {
   A7Unique viewer;
   viewer.run();
}
