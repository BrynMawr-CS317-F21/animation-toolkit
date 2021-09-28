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
      //setting color into random
      someP.color = agl::randomUnitVector();
      //the speed using only random would be with in range [0,1], thus to make
      //make the effect more significant, I have multiplied it with 100.0, but
      //still, the speed is random.
      someP.speed = agl::random() * 100.0f;
      //the reason for using spead but not velocity is that velocity have direction
      //but we dont want direction to be random, so it is not in our class
      someP.position = vec3(agl::random() * width(), agl::random() * height(), 0);
      //setting the start position into random
      storeP[i] = someP;
    }
  }

  virtual void scene() {
    for (int j = 0; j < 300; j++){
      setColor(storeP[j].color);
      storeP[j].position += storeP[j].speed * direction * dt();
      //when flying out of the screen, setting the out-bounded position into 0
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

    struct sParticle storeP[300]; // storing particles
    vec3 direction; //set the direction of particles moving 
    float radius; // set the particle radius
};

int main(int argc, char** argv) {
  Particles viewer;
  viewer.run();
  return 0;
}
