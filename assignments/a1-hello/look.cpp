#include "atkui/framework.h"

using namespace glm;

class Look : public atkui::Framework {
 public:
  Look() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    _mouseX = width() * 0.5;
    _mouseY = height() * 0.5;
    thetaL = 0.0;
    thetaR = 0.0;
    r = 50.0;
  }

  virtual void scene() {
    vec3 target = vec3(_mouseX, _mouseY, 0);
    setColor(vec3(1,0,0));
    drawSphere(target, 5); //red target

    //eye white
    float y = 0.5 * height();
    float leftW = 0.25 * width();
    float rightW = 0.75 * width();
 
    setColor(vec3(1,1,1));
    drawSphere(vec3(leftW, y, -400), 170);
    drawSphere(vec3(rightW, y, -400), 170);
    
    //iris (black)
    thetaL = atan2((_mouseX - leftW), (_mouseY - y));
    thetaR = atan2((_mouseX - rightW), (_mouseY - y));
    float pxL = r * sin(thetaL) + leftW;
    float pyL = r * cos(thetaL) + y;
    float pxR = r * sin(thetaR) + rightW;
    float pyR = r * cos(thetaR) + y;

    setColor(vec3(0,0,0));
    drawSphere(vec3(pxL, pyL, 0), 50);
    drawSphere(vec3(pxR, pyR, 0), 50);
  }

  void mouseMotion(int x, int y, int dx, int dy) {
    _mouseX = x;
    _mouseY = height() - y;
  }

 private:
  int _mouseX;
  int _mouseY;
  float thetaL;
  float thetaR;
  float r;
};

int main(int argc, char** argv) {
  Look viewer;
  viewer.run();
  return 0;
}