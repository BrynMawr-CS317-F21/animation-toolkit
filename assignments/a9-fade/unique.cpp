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

   void loadMotion3(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion3_);
  }

   void loadMotion4(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion4_);
  }

   void loadMotion5(const std::string &filename)
  {
    BVHReader reader;
    reader.load(filename, skeleton_, motion5_);
  }

  void crossfade(int numBlendFrames, Motion a, Motion b, bool first)
  {
    assert(a.getNumKeys() > 0);
    assert(b.getNumKeys() > 0);
    assert(numBlendFrames > 0);
    assert(numBlendFrames <= a.getNumKeys());
    assert(numBlendFrames <= b.getNumKeys());

    int start1 = a.getNumKeys() - numBlendFrames -1;
    int start2 = 0;

    Pose pose1, pose2;
    vec3 pos = vec3(0);
    for(int i = 0; i < start1; i++){
      pose1 = a.getKey(i);
      if(first){
         blend_.appendKey(pose1);
      }else{
         blend_.editKey(i, pose1);
      }
    }

    for(int j = 0; j <= numBlendFrames; j++){
      pose1 = a.getKey(start1 + j);
      pose2 = b.getKey(j);
      pose2.rootPos[0] = pose1.rootPos[0];
      pose2.rootPos[2] = pose1.rootPos[2];
      Pose newPose = newPose.Lerp(pose1, pose2, (float)j/(float)numBlendFrames);
      newPose.jointRots[0] = slerp(pose1.jointRots[0], pose2.jointRots[0], (float)j/(float)numBlendFrames);
      if(first){
         blend_.appendKey(newPose);
      } else {
         blend_.editKey(j+start1, newPose);
      }
    }

    vec3 offset = a.getKey(a.getNumKeys()-1).rootPos - b.getKey(numBlendFrames).rootPos;
    for(int k = numBlendFrames+1; k < b.getNumKeys(); k++){
      pose1 = b.getKey(k);
      pose1.rootPos += offset;
      blend_.appendKey(pose1);
    }
  }

   void crossfadeAll(int numBlendFrames){
      crossfade(numBlendFrames, motion1_, motion2_, true);
      crossfade(numBlendFrames, blend_, motion3_, false);
      crossfade(numBlendFrames, blend_, motion4_, false);
      crossfade(numBlendFrames, blend_, motion5_, false);
      blend_.setFramerate(motion3_.getFramerate());
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

  Skeleton skeleton_;
  SkeletonDrawer drawer_;
  Motion motion1_;
  Motion motion2_;
  Motion motion3_;
  Motion motion4_;
  Motion motion5_;
  Motion blend_;

};

std::string PruneName(const std::string &name)
{
  int idx1 = name.find_last_of("/");
  int idx2 = name.find_last_of(".");
  return name.substr(idx1 + 1, idx2 - idx1 - 1);
}

int main(int argc, char** argv)
{
   std::string motion1 = "../motions/Beta/walking.bvh";
   std::string motion2 = "../motions/Beta/jump.bvh";
   std::string motion3 = "../motions/Beta/standard_run.bvh";
   std::string motion4 = "../motions/Beta/standard_run.bvh";
   std::string motion5 = "../motions/Beta/jump.bvh";
   int numFrames = 7;

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

   A9Unique viewer;
   viewer.loadMotion1(motion1);
   viewer.loadMotion2(motion2);
   viewer.loadMotion3(motion3);
   viewer.loadMotion4(motion4);
   viewer.loadMotion5(motion5);
   viewer.crossfadeAll(numFrames);

   viewer.run();
   return 0;
}

