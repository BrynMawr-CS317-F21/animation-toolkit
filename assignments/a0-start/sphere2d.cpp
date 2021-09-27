<<<<<<< HEAD
#include "atkui/framework.h"

using glm::vec3;

class Sphere2D : public atkui::Framework {
 public:
  Sphere2D() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void scene() {
    // colors are RGB triplets in range [0,1]
    setColor(vec3(0,1,0));

    // draw a sphere at center of screen
    double x = width() * 0.5;
    double y = height() * 0.5;
    double radiusS = 100; // for the sphere ahead
    drawSphere(vec3(x,y,0), radiusS);

    setColor(vec3(1,0,0));
    double radiusL = 200; // for the larger sphere behind
    drawSphere(vec3(x,y,-100), radiusL);
  }
};

int main(int argc, char** argv)
{
  Sphere2D viewer;
  viewer.run();
  return 0;
}