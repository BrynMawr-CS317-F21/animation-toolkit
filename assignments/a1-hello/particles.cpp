#include "atkui/framework.h"

using namespace glm;

class Particles : public atkui::Framework
{
 public:
  Particles() : atkui::Framework(atkui::Orthographic) {
  }

  virtual void setup() {
    struct sParticle storeP[110];
    for (int i = 0; i < 110; i++){
      struct sParticle someP;
      someP.color = agl::randomUnitVector();
    }
    direction = vec3(2, 3, 0);
  }

  virtual void scene() {
    
  }

  private:
    struct sParticle{
      vec3 color;
      vec3 position;
      vec3 velocity;
    } Particle;

    vec3 direction;
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
