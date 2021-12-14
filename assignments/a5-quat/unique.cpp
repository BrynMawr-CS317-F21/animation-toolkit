#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkui/framework.h"
#include <iostream>
#include <math.h>

using glm::vec3;
using atkmath::Matrix3;
using atkmath::Vector3;
using atkmath::Quaternion;
using namespace glm;

class A5Unique : public atkui::Framework {
   public:
      A5Unique() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      pos = vec3(width()/2, height()/2, 0);
      axis[0] = vec3(1, 2, 0);
      axis[1] = vec3(2, 1, 0);
      axis[2] = vec3(2, -1, 0);
      axis[3] = vec3(1, -2, 0);
      axis[4] = vec3(-2, 1, 0);
      axis[5] = vec3(-1, 2, 0);
      axis[6] = vec3(-2, -1, 0);
      axis[7] = vec3(-1, -2, 0);
      size = 0.0;
      distance = 30.0;
      theta = 0.0;
      thetaRate = 0.1;
   }

   virtual void scene() {
      float  x, y;
      for(int i = 0; i < 8; i++){
         if ((i%5)==0)
            setColor(vec3(0,165,227)/255.0f);
         if ((i%5)==1)
            setColor(vec3(141,215,191)/255.0f);
         if ((i%5)==2)
            setColor(vec3(255,150,197)/255.0f);
         if ((i%5)==3)
            setColor(vec3(255,87,104)/255.0f);
         if ((i%5)==4)
            setColor(vec3(255,162,58)/255.0f);
         size =  i * distance;
         theta += thetaRate * dt();
         push();
         translate(pos);
         rotate(theta, axis[i]);
         drawTorus(vec3(0), size);
         pop();
      }
   }

 private:
   vec3 pos;
   vec3 axis[8];
   float size;
   float distance;
   float theta;
   float thetaRate;
};

int main(int argc, char** argv) {
   A5Unique viewer;
   viewer.run();
   return 0;
}

