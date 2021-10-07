#include "atkmath/vector3.h"
#include "atkmath/matrix3.h"
#include "atkmath/quaternion.h"
#include "atkui/framework.h"
#include <iostream>

using glm::vec3;
using atkmath::Matrix3;
using atkmath::Vector3;
using atkmath::Quaternion;
using namespace glm;

class A4Unique : public atkui::Framework {
   public:
      A4Unique() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      pos = vec3(0);
      positionX = width()/2; // set origin
      positionY = height()/2; // set origin
      distance = 50.0; //set distance between each circle
      radius = 0.0; //distance from the center
      theta = 0.0; // circulation movement
      degree = acos(0.0)/9.0f; //10 degree (for spacing each triangle)
      size = 20.0f; // modify the size of the cone
      rotationAngle = 0.0f; //this is the angle that the cone is going to rotate
      rotationRate = 0.01f;
      thetaRate = 0.001f; //rate the triangle is doing circular motion
  }

   virtual void scene() {
      float px; //recording the x position of the triangle(cone)
      float py; //recording the y position of the triangle
      for (int i = 0; i < 6; i++){
         //alternate color
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
         for (int j = 0; j < i * 9; j++){
            degree = (acos(0.0) * 4.0f) / (i * 9.0f);
            if(i%2){ 
               px = radius * sin(theta + j * degree) + positionX;
               py = radius * cos(theta + j * degree) + positionY;
            } else {
               px = radius * cos(theta + j * degree) + positionX;
               py = radius * sin(theta + j * degree) + positionY;
            }
            rotationAngle += rotationRate * dt();
            theta -= i * thetaRate * dt(); 
            push();
            translate(vec3(px, py, 0)); 
            // translate the position for each triangle as origin  
            rotate(acos(0.0), vec3(0,1,0)); 
            // we want the side view of the cone, since we want triangle
            rotate(rotationAngle, vec3(1,0,0)); 
            // rotating alone x-axis 
            scale(vec3(0, 2, 1));
            // size change
            drawCone(vec3(0), size);
            pop();
         }    
      }
  }

 private:
   vec3 pos;
   float radius;
   float distance;
   float positionX;
   float positionY;
   float theta;
   float degree;
   float size;
   float rotationAngle;
   float rotationRate;
   float thetaRate;
};

int main(int argc, char** argv) {
   A4Unique viewer;
   viewer.run();
   return 0;
}

