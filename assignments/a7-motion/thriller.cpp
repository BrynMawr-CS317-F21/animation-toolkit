#include <random>
#include <functional>
#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "agl/renderer.h"
#include "devil.h"

using namespace atk;
using namespace glm;

class Thriller : public atkui::Framework
{
public:
   Thriller() : atkui::Framework(atkui::Perspective) {}
   virtual ~Thriller() {}

   virtual void setup() {
      BVHReader reader;
      reader.load("../motions/Warrok/WarrokThriller.bvh", _skeleton, _motion);

      vec3 color = vec3(1,0,0);
      float size = 1.0f;

      positions[0] = vec3(30,0,0);
      positions[1] = vec3(90,0,0);
      positions[2] = vec3(-30,0,0);
      positions[3] = vec3(-90,0,0);
      positions[4] = vec3(30,0,80);
      positions[5] = vec3(90,0,80);
      positions[6] = vec3(-30,0,80);
      positions[7] = vec3(-90,0,80);
      positions[8] = vec3(30,0,-80);
      positions[9] = vec3(-30,0,-80);
      positions[10] = vec3(90,0,-80);
      positions[11] = vec3(-90,0,-80);


      for(int i = 0; i < 12; i++){
         color = agl::randomUnitVector();
         size = agl::random();
         devils[i] = Devil(5.0f * positions[i], color, size);
      }
   }

   virtual void scene() {
      if (!_paused) _motion.update(_skeleton, elapsedTime());
      for(int i = 0; i < 12; i++){
         devils[i].draw(_skeleton, *this);
      }
   }

   virtual void keyUp(int key, int mods) {
      if (key == 'P') _paused = !_paused;
   }

protected:
   Motion _motion;
   Skeleton _skeleton;
   bool _paused = false;
   Devil devils[12];
   vec3 positions[12];
};

int main(int argc, char** argv) {
   Thriller viewer;
   viewer.run();
}

