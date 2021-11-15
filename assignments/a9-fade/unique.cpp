#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <algorithm>
#include <string>

using namespace atk;
using namespace atkui;
using namespace glm;

class A9Unique : public atkui::Framework
{
public:
   A9Unique() : atkui::Framework(atkui::Perspective) {}
   virtual ~A9Unique() {}
   
   void setup(){

   }

   void scene(){

   }
};

int main(int argc, char** argv)
{
   A9Unique viewer;
   viewer.run();
   return 0;
}

