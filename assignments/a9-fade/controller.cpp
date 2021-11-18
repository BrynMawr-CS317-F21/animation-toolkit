#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include "atk/toolkit.h"
#include <iostream>
#include <cmath>

using namespace glm;
using namespace std;
using namespace atk;

class WASDSimple : public atkui::Framework
{
public:
  WASDSimple() : atkui::Framework(atkui::Perspective) {}

  virtual void setup()
  {
    BVHReader reader;
    reader.load("../motions/Beta/walking.bvh", _skeleton, _walk);
    _drawer.showAxes = true;
  }

  virtual void scene()
  {
    update();
    _drawer.draw(_skeleton, *this);

    // draw heading
    vec3 p = _skeleton.getRoot()->getGlobalTranslation();
    p[1] = 10; // set height close above the floor

    setColor(vec3(0, 1, 1));
    push();
    translate(p);
    rotate(_heading, vec3(0, 1, 0));
    translate(vec3(0,0,25));
    scale(vec3(10, 10, 50));
    drawCylinder(vec3(0), 1.0f);
    pop();
  }

  virtual void update()
  {
    _walk.update(_skeleton, elapsedTime());
    //settig the root position the same as frame 0;
    float speed = 0.7f;
    quat direction = quat(cos(_heading/2.0f), 0, sin(_heading/2.0f), 0);
    quat offsetDir = direction * inverse(_walk.getKey(0).jointRots[0]);
    vec3 walkDir = vec3(sin(_heading), 0, cos(_heading));
    vec3 pos = _walk.getKey(0).rootPos;
    vec3 look = _skeleton.getByName("Beta:Head")->getGlobalTranslation();

    for(int i = 0; i < _walk.getNumKeys(); i++){
      Pose pose = _walk.getKey(i);
      pose.rootPos = pos + speed * walkDir;
      pose.jointRots[0] = offsetDir * pose.jointRots[0];
      _walk.editKey(i, pose);
      lookAt(pose.rootPos + 250.0f * normalize(vec3(0, 150, -150)), look, vec3(0,1,0));
    }
    
    
    // update heading when key is down
    if (keyIsDown('D')) _heading -= 0.05;
    if (keyIsDown('A')) _heading += 0.05;
  }

protected:
  float _heading;

  Motion _walk;
  Skeleton _skeleton;
  atkui::SkeletonDrawer _drawer;
};

int main(int argc, char **argv)
{
  WASDSimple viewer;
  viewer.run();
}
