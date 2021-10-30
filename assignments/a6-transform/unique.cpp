#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using atk::Transform;
using glm::quat;
using glm::vec3;

class A6Unique : public atkui::Framework {
   public:
      A6Unique() : atkui::Framework(atkui::Orthographic) {
   }

   virtual void setup() {
      Joint* root  = new Joint("");
      root->setLocalTranslation(vec3(width()/2,height()/2,0));
      _tentacle.addJoint(root);
   }

   virtual void scene() {
      
   }

 private:

};

int main(int argc, char** argv) {
   A6Unique viewer;
   viewer.run();
   return 0;
}

