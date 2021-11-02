#include <iostream>
#include "atkui/framework.h"
#include "atk/toolkit.h"

using namespace glm;
using namespace atk;

class MotionViewer : public atkui::Framework
{
public:
   MotionViewer() : atkui::Framework(atkui::Perspective) {
   }

   void setup() {
      BVHReader reader;
      reader.load("../motions/Beta/jump.bvh", skeleton, motion);
      motion.update(skeleton, 0);
   }

   void scene() {
      time += dt();
      motion.update(skeleton, time);

      setColor(vec3(0,0,0.8));
      for (int i = 0; i < skeleton.getNumJoints(); i++) {
         Joint* joint = skeleton.getByID(i);
         if (joint->getParent() == 0) continue;

         vec3 p1 = joint->getGlobalTranslation();
         vec3 p2 = joint->getParent()->getGlobalTranslation();
         drawEllipsoid(p1, p2, 5);
      }

      currentFrame = motion.getFramerate() * time;
      if(time >= motion.getDuration()){
         currentFrame = 0;
         time = 0;
         time += dt();
      }

      drawText(paused? "Paused" : "Playing", 10, 15);
      drawText("Current frame: "+std::to_string(currentFrame), 10, 35);
      drawText("Time scale: "+std::to_string(timeScale), 10, 55);

      if(isP){
         if(!paused){
            float tmp = time;
            time = tmp;
            //isP = false;
         } else {
            time += dt();
            //isP = false;
         }
      }

      if(is0){
         time = 0;
         //is0 = false;
      }

      if(paused){
         if(forward){
            time += motion.getDeltaTime();
            //forward = false;
         }
         if(backward){
            time -= motion.getDeltaTime();
            //backward = false;
         }
      }

      double delta = motion.getFramerate();

      if(increSpeed){
         timeScale = timeScale + 0.1;

         //increSpeed = false;
      }
      if(decreSpeed){
         time = time * 2.0f;
         //decreSpeed = false;
      }


   }

   virtual void keyUp(int key, int mods) {
      if (key == GLFW_KEY_P)
      {
         isP = true;
      }
      if (key == GLFW_KEY_0)
      {
         is0 = true;
      }

      if (key == GLFW_KEY_PERIOD)
      {
         forward = true;
      }

      if (key == GLFW_KEY_COMMA)
      {
         backward = true;
      }

      if (key == GLFW_KEY_RIGHT_BRACKET)
      {
         increSpeed = true;
      }

      if (key == GLFW_KEY_LEFT_BRACKET)
      {
         decreSpeed = true;
      }
      
   }

private:
   Skeleton skeleton;
   Motion motion;

   float timeScale = 1.0f;
   int currentFrame = 0; 
   bool paused = false;
   float time = 0;
   bool is0 = false;
   bool isP = false;
   bool forward = false;
   bool backward = false;
   bool increSpeed = false;
   bool decreSpeed = false;
};


int main(int argc, char** argv) {
   MotionViewer viewer;
   viewer.run();
}
