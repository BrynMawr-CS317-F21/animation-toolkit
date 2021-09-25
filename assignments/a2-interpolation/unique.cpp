#include "atkui/framework.h"
using namespace glm;

class A2Unique : public atkui::Framework {
 public:
  A2Unique() : atkui::Framework(atkui::Orthographic) {
  }

  void setup() {
   num = 10;
   distance = width()/num;
   theta = 0.0f;
   r = distance;
   for (int i = 0; i < num; i++){
      colorS[i]= agl::randomUnitVector();
   }
  }

  /** 
   * draw several curves with b1 and b2 control points changing in the direction of cos
   * color is randomized
   */
  void scene() {
   float px0;
   float px1;
   float px2;
   float py1 = height() * 0.2;
   float py2 = height() * 0.8;
   for(int i = 0; i < num; i++){
     theta += dt() * 0.5;
     px0 = i * distance + distance * 0.5;
     px1 = distance * 1.5 * cos(theta) + (i+0.5) * distance;
     px2 = (i+1) * distance - distance * 1.5 * cos(theta);
     c0 = vec3(px0, height(), 0);
     c1 = vec3(px1, py1, 0);
     c2 = vec3(px2, py2, 0);
     c3 = vec3(px0, 0, 0);
     drawBernstein(c0, c1, c2, c3, colorS[i]);
   }
  }
  //draw the curve using bernstein algorithm
  void drawBernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3, vec3 color){
    vec3 a;
    vec3 b;
    float size = 0.01;
    for (float t = 0; t < 1; t+=size){
      setColor(color);
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
  float r;
  vec3 colorS[10];
};

int main(int argc, char** argv) {
  A2Unique viewer;
  viewer.run();
}