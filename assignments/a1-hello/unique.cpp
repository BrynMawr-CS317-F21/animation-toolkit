#include "atkui/framework.h"

using namespace glm;

class Unique : public atkui::Framework {
 public:
  Unique() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    positionX = width() * 0.5;
    positionY = height() * 0.5;
    theta = 0.0;
    thetaRate = 0.01;
    radius = 0.0;
    radiusRate = 0.3;
    tenDegree = acos(0.0)/9;
  }

  virtual void scene() {
    float px;
    float py;
    for (int i = 0; i < 5; i++){
      for (int j = 0; j < 36; j++){
        setColor(agl::randomUnitVector()); //used random color
        radius += radiusRate * dt(); 
        //magnifying the circle would be increasing the radius
        theta += thetaRate * dt();
        //I still want to to be in random motion
        if (radius >= width()/2 || radius >= height()/2)
          radius = 0.0;
        px = radius * cos(theta + j * tenDegree) + positionX;
        py = radius * sin(theta + j * tenDegree) + positionY;
        drawSphere(vec3(px, py, 0), 10);
      }
    }
  }

 private:
  float theta;
  float thetaRate;
  float positionX;
  float positionY;
  float tenDegree;
  float radius;
  float radiusRate;
};

int main(int argc, char** argv) {
  Unique viewer;
  viewer.run();
  return 0;
}
