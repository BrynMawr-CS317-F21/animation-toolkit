#include "atkui/framework.h"

using namespace glm;
class Circles : public atkui::Framework {
   public:
      Circles() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      theta = 0.0;
      thetaRate = 0.001;
      radius = 0.0;
      distance = 30.0;
      r = 10.0;
      positionX = width()/2;
      positionY = height()/2;
      tenDegree = acos(0.0)/9;
   }

   virtual void scene() {
      for (int i = 0; i < 11; i++){
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
         radius = i * distance;
         for (int j = 0; j < 36; j++){
            float px;
            float py;
            theta += thetaRate * dt();
            if(i%2){
               px = radius * cos(theta + j * tenDegree) + positionX;
               py = radius * sin(theta + j * tenDegree) + positionY;
               drawSphere(vec3(px, py, 0), r);
            } else {
               px = radius * sin(theta + j * tenDegree) + positionX;
               py = radius * cos(theta + j * tenDegree) + positionY;
               drawSphere(vec3(px, py, 0), r);
            }            
         }
      }
   }

   private:
      float theta;
      float thetaRate;
      float radius;
      float r;
      float distance;
      float positionX;
      float positionY;
      float tenDegree;

};

int main(int argc, char** argv)  {
   Circles viewer;
   viewer.run();
   return 0;
}
