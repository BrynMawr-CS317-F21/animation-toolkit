#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class Butterfly : public atkui::Framework
{
public:
   Butterfly() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      Joint* body = new Joint("Body");
      body->setLocalTranslation(vec3(1,2,1)*100.0f);
      body->setLocalRotation(glm::angleAxis(glm::radians<float>(45.0f), vec3(0,1,0)));
      skeleton.addJoint(body);

      Joint* lwing = new Joint("LWing");
      lwing->setLocalTranslation(vec3(0.1,0,0)*100.0f);
      skeleton.addJoint(lwing, body);

      Joint* lswing = new Joint("LSWing");
      lswing->setLocalTranslation(vec3(0,-0.08,0)*100.0f);
      skeleton.addJoint(lswing, lwing);

      Joint* rwing = new Joint("RWing");
      rwing->setLocalTranslation(vec3(-0.1,0,0)*100.0f);
      skeleton.addJoint(rwing, body);

      Joint* rswing = new Joint("RSWing");
      rswing->setLocalTranslation(vec3(0,-0.08,0)*100.0f);
      skeleton.addJoint(rswing, rwing);

      skeleton.fk();
   }

   void scene() {
      Joint* body = skeleton.getByName("Body");
      Joint* lwing = skeleton.getByName("LWing");
      lwing->setLocalRotation(glm::angleAxis(sin(elapsedTime()), vec3(0,0,1)));
      Joint* lswing = skeleton.getByName("LSWing");
      Joint* rwing = skeleton.getByName("RWing");
      rwing->setLocalRotation(glm::angleAxis(-sin(elapsedTime()), vec3(0,0,1))); 
      Joint* rswing = skeleton.getByName("RSWing");
      skeleton.fk();

      // attach geometry to skeleton 
      Transform B = body->getLocal2Global(); 
      Transform LT = lwing->getLocal2Global(); 
      Transform RT = rwing->getLocal2Global(); 
      Transform LST = lswing->getLocal2Global(); 
      Transform RST = rswing->getLocal2Global(); 

      // draw body
      Transform bodyGeometry(
         glm::angleAxis(glm::pi<float>()*0.5f, vec3(1,0,0)), // rotation
         vec3(0), //position
         vec3(25, 200, 25)); // scale

      Transform lwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,0), 
         vec3(120,20,100));
      
      Transform lswingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(-80,0,-40), 
         vec3(20,20,50));

      Transform rwingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,0), 
         vec3(120,20,150));

      Transform rswingGeometry(
         eulerAngleRO(XYZ, vec3(0,0,0)),
         vec3(80,0,-40), 
         vec3(20,20,50));

      setColor(vec3(0.4, 0.4, 0.8));
      push();
      transform(B * bodyGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.8, 0, 0.0));
      push();
      transform(LT * lwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0.8, 0, 0.0));
      push();
      transform(LST * lswingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RT * rwingGeometry);
      drawSphere(vec3(0), 1);
      pop();

      setColor(vec3(0, 0.8, 0.0));
      push();
      transform(RST * rswingGeometry);
      drawSphere(vec3(0), 1);
      pop();
   }

private:
   Skeleton skeleton;
};


int main(int argc, char** argv) {
   Butterfly viewer;
   viewer.run();
}
