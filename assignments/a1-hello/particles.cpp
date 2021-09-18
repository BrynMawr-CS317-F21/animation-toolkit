#include "atkui/framework.h"

using namespace glm;

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    direction = vec3(0.5, 0.3, 0);
    radius = 15.0;
    for (int i = 0; i < 300; i++){
      struct sParticle someP;
      someP.color = agl::randomUnitVector();
      someP.speed = agl::random();
      someP.position = vec3(agl::random() * width(), agl::random() * height(), 0);
      storeP[i] = someP;
    }
  }

  virtual void scene() {
    for (int j = 0; j < 300; j++){
      setColor(storeP[j].color);
      storeP[j].position = storeP[j].position + storeP[j].speed * direction * elapsedTime();
      if (storeP[j].position.x > width()){
        storeP[j].position.x = 0;
      }
      if (storeP[j].position.y > height()){
        storeP[j].position.y = 0;
      }
      drawSphere (storeP[j].position, radius);
    }

  }

  private:
    struct sParticle{
      vec3 color;
      vec3 position;
      float speed;
    };

    struct sParticle storeP[300];
    vec3 direction;
    float radius;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
