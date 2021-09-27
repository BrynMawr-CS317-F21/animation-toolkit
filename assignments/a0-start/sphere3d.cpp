<<<<<<< HEAD
#include "atkui/framework.h"

using glm::vec3;

class Sphere3D : public atkui::Framework {
 public:
  Sphere3D() : atkui::Framework(atkui::Perspective) {
  }

  void keyUp(int key, int mods){
    isSpace = 0;
    isR = 0;
    if (key == GLFW_KEY_SPACE){
      //when press the spacebar, the sphere will move to a random position
      vel = agl::randomUnitVector();
      isSpace = 1;
    }
    if (key == GLFW_KEY_R){
      //when press the bottom R, the sphere will go back to the origin
      isR = 1;
    }
  }

  virtual void setUp() {
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    if (isSpace == 1){
      currentPos = currentPos + 100.0f * vel * dt();
    }

    if (isR == 1){
      currentPos = vec3(0,0,0);
    }
    // draw a sphere at center of the world
    float radius = 50.0;
    drawSphere(currentPos, radius);
  }
  bool isSpace;
  bool isR;
  vec3 currentPos;
  vec3 vel;
};

int main(int argc, char** argv)
{
  Sphere3D viewer;
  viewer.run();
  return 0;
}