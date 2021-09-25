#include "atkui/framework.h"
using namespace glm;

class A2Unique : public atkui::Framework {
 public:
  A2Unique() : atkui::Framework(atkui::Orthographic) {
  }

  void setup() {
   num = 8;
   distance = width()/num;
  }

  void scene() {
   float px0;
   float px1;
   float px2;
   float py1 = height()/4;
   float py2 = height()/4 * 3.0f;
   for(int i = 0; i < num; i++){
     theta += 1.2f * dt();
     px0 = distance/2 + i * distance;
     px1 = distance * sin(theta) + i * distance;
     px2 = (i+1) * distance - px1;
     c0 = vec3(px0, height(), 0);
     c1 = vec3(px1, py1, 0);
     c2 = vec3(px2, py2, 0);
     c3 = vec3(px0, 0, 0);
     drawBernstein(c0, c1, c2, c3);
   }
  }

  void drawBernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3){
    vec3 a;
    vec3 b;
    float size = 0.000001;
    vec3 color = agl::randomUnitVector();
    for (float t = 0; t < 1; t+=size){
      setColor(vec3(0,1,1));
      a = bernstein(B0, B1, B2, B3, t);
      b = bernstein(B0, B1, B2, B3, t + size);
      drawLine(a, b);
    }
  }

  vec3 bernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3, float t){
    return (1-t)*(1-t)*(1-t)*B0 + 3.0f*(1-t)*(1-t)*t* B1 + 3.0f*t*t*(1-t)* B2 + t*t*t* B3;
  }

 private:
  vec3 c0;
  vec3 c1;
  vec3 c2;
  vec3 c3;
  int num; //store number of curves used
  float theta;
  float distance; 
};

int main(int argc, char** argv) {
  A2Unique viewer;
  viewer.run();
}