#include "atkui/framework.h"
using namespace glm;

class A3Unique : public atkui::Framework {
 public:
  A3Unique() : atkui::Framework(atkui::Orthographic) {
  }

  void setup() {

  }

  /** 
   * draw a guitar using splines
   */
  void scene() {
   
  }

 private:

};

int main(int argc, char** argv) {
  A3Unique viewer;
  viewer.run();
}