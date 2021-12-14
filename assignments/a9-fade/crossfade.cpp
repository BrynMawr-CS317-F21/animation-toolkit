#include "atk/toolkit.h"
#include "atkui/framework.h"
#include "atkui/skeleton_drawer.h"
#include <stdio.h>
#include <string.h>

using namespace atk;
using namespace atkui;
using namespace glm;

class ACrossfadeViewer : public atkui::Framework
{
public:
  ACrossfadeViewer() : atkui::Framework(atkui::Perspective) {}
  virtual ~ACrossfadeViewer() {}

  void loadMotion1(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion1_);
  }

  void loadMotion2(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion2_);
  }

  void crossfade(int numBlendFrames)
  {
    assert(motion1_.getNumKeys() > 0);
    assert(motion2_.getNumKeys() > 0);
    assert(numBlendFrames > 0);
    assert(numBlendFrames <= motion1_.getNumKeys());
    assert(numBlendFrames <= motion2_.getNumKeys());


    int start1 = motion1_.getNumKeys() - numBlendFrames -1;
    int start2 = 0;

    blend_.setFramerate(motion2_.getFramerate());

    Pose pose1, pose2;
    vec3 pos = vec3(0);
    for(int i = 0; i < start1; i++){
      pose1 = motion1_.getKey(i);
      blend_.appendKey(pose1);
    }

    for(int j = 0; j <= numBlendFrames; j++){
      pose1 = motion1_.getKey(start1 + j);
      pose2 = motion2_.getKey(j);
      pose2.rootPos[0] = pose1.rootPos[0];
      pose2.rootPos[2] = pose1.rootPos[2];
      Pose newPose = newPose.Lerp(pose1, pose2, (float)j/(float)numBlendFrames);
      newPose.jointRots[0] = slerp(pose1.jointRots[0], pose2.jointRots[0], (float)j/(float)numBlendFrames);
      blend_.appendKey(newPose);
    }

    vec3 offset = motion1_.getKey(motion1_.getNumKeys()-1).rootPos - motion2_.getKey(numBlendFrames).rootPos;
    for(int k = numBlendFrames+1; k < motion2_.getNumKeys(); k++){
      pose1 = motion2_.getKey(k);
      pose1.rootPos += offset;
      blend_.appendKey(pose1);
    }

  }

  void save(const std::string &filename)
  {
    BVHWriter writer;
    writer.save(filename, skeleton_, blend_);
  }

  void scene()
  {
    blend_.update(skeleton_, elapsedTime());
    drawer_.draw(skeleton_, *this);
  }

private:
  Skeleton skeleton_;
  SkeletonDrawer drawer_;
  Motion motion1_;
  Motion motion2_;
  Motion blend_;
};

std::string PruneName(const std::string &name)
{
  int idx1 = name.find_last_of("/");
  int idx2 = name.find_last_of(".");
  return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char **argv)
{
  std::string motion1 = "../motions/Beta/walking.bvh";
  std::string motion2 = "../motions/Beta/jump.bvh";
  int numFrames = 10;

  try
  {
    for (int i = 0; i < argc; i++)
    {
      if (strncmp(argv[i], "-m1", 3) == 0)
        motion1 = argv[i + 1];
      else if (strncmp(argv[i], "-m2", 3) == 0)
        motion2 = argv[i + 1];
      else if (strncmp(argv[i], "-nframes", 7) == 0)
        numFrames = std::stoi(argv[i + 1]);
    }
  }
  catch (const std::exception &e)
  {
    std::cout << "usage: crossfade -m1 <path> -m2 <path> -nframes <num frames>\n";
  }

  ACrossfadeViewer viewer;
  viewer.loadMotion1(motion1);
  viewer.loadMotion2(motion2);
  viewer.crossfade(numFrames);

  std::string saveName = "blend-" + PruneName(motion1) + PruneName(motion2);
  saveName += "-" + std::to_string(numFrames) + ".bvh";
  viewer.save(saveName);

  viewer.run();
  return 0;
}
