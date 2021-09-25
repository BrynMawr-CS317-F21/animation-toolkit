#include "atkui/framework.h"
using namespace glm;

class Screensaver : public atkui::Framework {
 public:
  Screensaver() : atkui::Framework(atkui::Orthographic) {
  }

  void setup() {
    a0 = agl::randomUnitVector() * width();
    a1 = agl::randomUnitVector() * width();
    a2 = agl::randomUnitVector() * width();
    a3 = agl::randomUnitVector() * width();
    b0 = agl::randomUnitVector() * width();
    b1 = agl::randomUnitVector() * width();
    b2 = agl::randomUnitVector() * width();
    b3 = agl::randomUnitVector() * width();
    c0 = vec3(0);
    c1 = vec3(0);
    c2 = vec3(0);
    c3 = vec3(0);
    t = 0.0;
  }

  void scene() {
    drawBernstein(a0, a1, a2, a3);
    drawBernstein(b0, b1, b2, b3);
    float duration = 20.0f;
    t = elapsedTime()/duration;
    t = glm::clamp(t, 0.0f, 1.0f);
    c0 = (1-t)*a0 + t*b0;
    c1 = (1-t)*a1 + t*b1;
    c2 = (1-t)*a2 + t*b2;
    c3 = (1-t)*a3 + t*b3;
    drawBernstein(c0, c1, c2, c3);
    if(t==1){
      t = 0;
      produceCurve();
    }
  }

  void produceCurve(){
    a0 = b0;
    a1 = b1;
    a2 = b2;
    a3 = b3;
    b0 = agl::randomUnitVector() * width();
    b1 = agl::randomUnitVector() * width();
    b2 = agl::randomUnitVector() * width();
    b3 = agl::randomUnitVector() * width();
  }

  void drawBernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3){
    vec3 a;
    vec3 b;
    float size = 0.01;
    for (float t = 0; t < 1; t+=size){
      setColor(vec3(1));
      a = bernstein(B0, B1, B2, B3, t);
      b = bernstein(B0, B1, B2, B3, t + size);
      drawLine(a, b);
    }
  }

  vec3 bernstein(vec3 B0, vec3 B1, vec3 B2, vec3 B3, float t){
    return (1-t)*(1-t)*(1-t)*B0 + 3.0f*(1-t)*(1-t)*t* B1 + 3.0f*t*t*(1-t)* B2 + t*t*t* B3;
  }

  private:
    struct curve{
      vec3 color;
      vec3 _b0;
      vec3 _b1;
      vec3 _b2;
      vec3 _b3;
    };
    vec3 a0;
    vec3 a1;
    vec3 a2;
    vec3 a3;
    vec3 b0;
    vec3 b1;
    vec3 b2;
    vec3 b3;
    vec3 c0;
    vec3 c1;
    vec3 c2;
    vec3 c3;
    float t;
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

