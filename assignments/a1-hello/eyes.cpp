#include "atkui/framework.h"

using namespace glm;

class Eyes : public atkui::Framework
{
  public:
    Eyes() : atkui::Framework(atkui::Orthographic) {
    }

    virtual void setUp() {
      theta = 0.0;
    }

    virtual void scene() {
      float y = 0.5 * height();

      // eye white
      float leftW = 0.25 * width();
      float rightW = 0.75 * width();

      setColor(vec3(1,1,1));
      drawSphere(vec3(leftW, y, -400), 170);
      drawSphere(vec3(rightW, y, -400), 170);
      
      //iris 
      theta += 1.2f * dt();
      float pxL = 50.0f * sin(theta) + 0.25 * width();
      float pxR = 50.0f * sin(theta) + 0.75 * width();
      
      setColor(vec3(0,0,0));
      drawSphere(vec3(pxL, y, 0), 50);
      drawSphere(vec3(pxR, y, 0), 50);
    }

    private:
      float theta;

};

int main(int argc, char** argv) {
  Eyes viewer;
  viewer.run();
  return 0;
}