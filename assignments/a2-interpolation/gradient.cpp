#include <iostream>
#include "atkui/framework.h"
using namespace glm;

class Gradient : public atkui::Framework {
 public:
  Gradient() : atkui::Framework(atkui::Orthographic) {
  }
  virtual void setup(){
    cnw = vec3(1,1,0);
    cne = vec3(0,1,1);
    csw = vec3(1,0,0);
    cse = vec3(1,0,1);
    n = 10; //number of cubes produced
    size = width()/n; //make sure the size of each cube
  }

  virtual void scene() {
    float px;
    float py;
    float tx;
    float ty;
    vec3 position;
    vec3 cx0;
    vec3 cx1;
    vec3 c;

    for (int i = 0; i<=n; i++){
      for (int j = 0; j<=n; j++){
        position = vec3(px, py, 0); //store the position of the cube

        //interpolate the color of each cube
        cx0 = cnw * (1-tx) + cne * tx;
        cx1 = csw * (1-tx) + cse * tx;
        c = cx0 * (1-ty) + cx1 * ty;

        px = size/2 + i*size;
        py = size/2 + j*size;
        //this could make sure that tx and ty are within [0,1]
        tx = px/width(); 
        ty = 1- py/width();
        setColor(c);
        drawCube(position, vec3(size, size, size));
      }
    }    
  }
  
  private:
    vec3 cnw;
    vec3 cne;
    vec3 csw;
    vec3 cse;
    float size; //to store the size of the cube
    float n;
};

int main(int argc, char **argv) {
  Gradient viewer;
  viewer.run();
}
