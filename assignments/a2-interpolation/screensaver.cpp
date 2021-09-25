#include "atkui/framework.h"
using namespace glm;
class CurveS{
  public:
    vec3 b0;
    vec3 b1;
    vec3 b2;
    vec3 b3;
    vec3 color;

  CurveS(vec3 _b0, vec3 _b1, vec3 _b2, vec3 _b3, vec3 _color){
    b0 = _b0;
    b1 = _b1;
    b2 = _b2;
    b3 = _b3;
    color = _color;
  }

  CurveS(){
    b0 = vec3(0);
    b1 = vec3(0);
    b2 = vec3(0);
    b3 = vec3(0);
    color = vec3(1);
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
    timer = 0.0;
    position = vec3(width()*0.5,width()*0.5,width()*0.5);
    maxTrail = 100;
    T = 0.1;
  }

  virtual void scene() {
    float duration = 2.0f;
    time += dt()/duration;
    timer += dt()/duration;
    interpolateSE();
    drawBernstein(current);
    if(time > 1){
      time = 0.0f;
      start = end;
      end = randomizedCurve();
    }
    CurveS temp;
    if(timer > T){
      timer = 0.0f;
      temp = current;
      curveList.push_back(temp);
    }
    if(curveList.size() == maxTrail){
      curveList.pop_front();
    }
    for (CurveS s: curveList){
      drawBernstein(s);
    }
  }

  void interpolateSE(){
    current.b0 = (1-time)*start.b0 + time*end.b0;
    current.b1 = (1-time)*start.b1 + time*end.b1;
    current.b2 = (1-time)*start.b2 + time*end.b2;
    current.b3 = (1-time)*start.b3 + time*end.b3;
    current.color = (1-time)*start.color + time*end.color;
  }

  CurveS randomizedCurve(){
    CurveS some = CurveS();
    some.b0 = agl::randomUnitVector() * (width() * 0.5f) + position;
    some.b1 = agl::randomUnitVector() * (width() * 0.5f) + position;
    some.b2 = agl::randomUnitVector() * (width() * 0.5f) + position;
    some.b3 = agl::randomUnitVector() * (width() * 0.5f) + position;
    some.color = agl::randomUnitVector();
    return some;
  }

  void drawBernstein(CurveS some){
    vec3 a;
    vec3 b;
    float size = 0.01;
    for (float t = 0; t < 1; t+=size){
      setColor(some.color);
      a = bernstein(some, t);
      b = bernstein(some, t + size);
      drawLine(a, b);
    }
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
    float timer;
    float T;
    vec3 position;
    int maxTrail;
    std::list<CurveS> curveList;
};

int main(int argc, char** argv) {
  Screensaver viewer;
  viewer.run();
}

