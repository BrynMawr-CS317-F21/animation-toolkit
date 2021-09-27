<<<<<<< HEAD
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
      distance = 30.0; //set distance between each circle
      r = 10.0; // set particle radius
      positionX = width()/2; // set origin
      positionY = height()/2; // set origin
      tenDegree = acos(0.0)/9; 
      //since acos(0.0) is 90 degree, we want 10 degree thus 
      //divide it by 9
   }

   virtual void scene() {
      for (int i = 0; i < 11; i++){
         //setting each circle into different color
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
         // ensure that each particle will be in a 
         // constant distance with the other circle
         for (int j = 0; j < 36; j++){
            float px; //recording the x position of the particle
            float py; //recording the y position of the particle
            theta += thetaRate * dt(); //change the theta
            if(i%2){ 
               //i%2 to ensure neighboring circles moving in alternate direction
               //j*tenDegree to ensure each particle split by 10 degree
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
