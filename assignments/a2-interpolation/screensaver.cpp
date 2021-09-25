#include "atkui/framework.h"
using namespace glm;
class CurveS{
  public:
    vec3 b0;
    vec3 b1;
    vec3 b2;
    vec3 b3;

  CurveS(vec3 _b0, vec3 _b1, vec3 _b2, vec3 _b3){
    b0 = _b0;
    b1 = _b1;
    b2 = _b2;
    b3 = _b3;
  }

  CurveS(){
    b0 = vec3(0);
    b1 = vec3(0);
    b2 = vec3(0);
    b3 = vec3(0);
  }
};
class Screensaver : public atkui::Framework {
 public:
  Screensaver() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    start = randomizedCurve();
    end = randomizedCurve();
    current = CurveS();
    time = 0.0;
  }

  virtual void scene() {
    float duration = 2.0f;
    time = elapsedTime()/duration;
    time = (float)fmod(time, 1.0f);
    if(curveEqual(current, end)){
      start = end;
      end = randomizedCurve();
    }
    drawBernstein(start);
    drawBernstein(end);
    current.b0 = (1-time)*start.b0 + time*end.b0;
    current.b1 = (1-time)*start.b1 + time*end.b1;
    current.b2 = (1-time)*start.b2 + time*end.b2;
    current.b3 = (1-time)*start.b3 + time*end.b3;
    drawBernstein(current);
  }

  CurveS randomizedCurve(){
    CurveS some = CurveS();
    some.b0 = agl::randomUnitVector() * width();
    some.b1 = agl::randomUnitVector() * width();
    some.b2 = agl::randomUnitVector() * width();
    some.b3 = agl::randomUnitVector() * width();
    return some;
  }

  void drawBernstein(CurveS some){
    vec3 a;
    vec3 b;
    float size = 0.01;
    for (float t = 0; t < 1; t+=size){
      setColor(vec3(1));
      a = bernstein(some, t);
      b = bernstein(some, t + size);
      drawLine(a, b);
    }
  }

  bool curveEqual(CurveS a, CurveS b){
    return a.b0 == b.b0 && a.b1 == b.b1 && a.b2 == b.b2 
      && a.b3 == b.b3;
  }

  vec3 bernstein(CurveS some, float t){
    return (1-t)*(1-t)*(1-t)*some.b0 + 
      3.0f*(1-t)*(1-t)*t* some.b1 + 3.0f*t*t*(1-t)* some.b2 
        + t*t*t* some.b3;
  }

  private:
    CurveS start;
    CurveS current;
    CurveS end;
    float time;
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

