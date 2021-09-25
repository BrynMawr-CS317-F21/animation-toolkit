#include "atkui/framework.h"
using namespace glm;

class ParticleCubic : public atkui::Framework {
 public:
  ParticleCubic() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    time = 0.0f;
  }

  virtual void scene() {

    float duration = 5.0f;
    time = elapsedTime()/duration;
    time = (float)fmod(time, 1.0f);
    vec3 position;
    position = (1-time)*(1-time)*(1-time)*B0 + 3.0f*(1-time)*(1-time)*time* B1 + 3.0f*time*time*(1-time)* B2 + time*time*time* B3;

    setColor(vec3(1));
    drawSphere(position, 10);
    drawSCurve();
  }

  void drawSCurve(){
    vec3 a;
    vec3 b;
    float size = 0.01;
    for (float t = 0; t < 1; t+=size){
      setColor(vec3(0,1,1));
      a = bernstein(t);
      b = bernstein(t + size);
      drawLine(a, b);
    }
  }

  vec3 bernstein(float t){
    return (1-t)*(1-t)*(1-t)*B0 + 3.0f*(1-t)*(1-t)*t* B1 + 3.0f*t*t*(1-t)* B2 + t*t*t* B3;
  }

 private:
  vec3 B0 = vec3(100.0f,  50.0f, 0.0f);
  vec3 B1 = vec3(150.0f, 200.0f, 0.0f);
  vec3 B2 = vec3(250.0f, 100.0f, 0.0f);
  vec3 B3 = vec3(300.0f, 300.0f, 0.0f);
  float time;
};

int main(int argc, char** argv) {
  ParticleCubic viewer;
  viewer.run();
}

