#include "atkui/framework.h"
#include "atk/toolkit.h"
#include "spline.h"
#include "interpolator_hermite.h"
#include "interpolator_linear.h"
#include "interpolator_catmullrom.h"

using namespace glm;

class A3Unique : public atkui::Framework {
 public:
  A3Unique() : atkui::Framework(atkui::Orthographic) {
  }

  void setup() {
      body.addInterpolationType("Catmull-Rom", &mCatmullRom);
      head.addInterpolationType("Linear", &mLinear);
      bridge.addInterpolationType("Linear", &mLinear);
      body.setInterpolationType("Catmull-Rom");
      head.setInterpolationType("Linear");
      bridge.setInterpolationType("Linear");
      bottomheight = height()/3.0f;
      a = vec3(0);
      b = vec3(0);
      
      stringH[0] = vec3(width()/2.0f - 12.5f, 22.0f * height() / 24.0f, 0);
      stringH[1] = vec3(width()/2.0f - 7.5f, 21.0f * height() / 24.0f, 0);
      stringH[2] = vec3(width()/2.0f - 2.5f, 20.0f * height() / 24.0f, 0);
      stringH[3] = vec3(width()/2.0f + 2.5f, 20.0f * height() / 24.0f, 0);
      stringH[4] = vec3(width()/2.0f + 7.5f, 21.0f * height() / 24.0f, 0);
      stringH[5] = vec3(width()/2.0f + 12.5f, 22.0f * height() / 24.0f, 0);


      stringB[0] = vec3(width()/2.0f - 12.5f, bottomheight, 0);
      stringB[1] = vec3(width()/2.0f - 7.5f, bottomheight, 0);
      stringB[2] = vec3(width()/2.0f - 2.5f, bottomheight, 0);
      stringB[3] = vec3(width()/2.0f + 2.5f, bottomheight, 0);
      stringB[4] = vec3(width()/2.0f + 7.5f, bottomheight, 0);
      stringB[5] = vec3(width()/2.0f + 12.5f, bottomheight, 0);

      headP[0] = vec3(width()/2.0f - 27.5f, 23.0f * height() / 24.0f, 0);
      headP[1] = vec3(width()/2.0f - 22.5f, 19.0f * height() / 24.0f, 0);
      headP[2] = vec3(width()/2.0f + 22.5f, 19.0f * height() / 24.0f, 0);
      headP[3] = vec3(width()/2.0f + 27.5f, 23.0f * height() / 24.0f, 0);
      headP[4] = headP[0];

      for (int i = 0; i < 5; i++){
         head.appendKey((float) i, headP[i]);
      }

      bodyP[0] = vec3(width()/2.0f, 14.0f*height()/24.0f, 0);
      bodyP[1] = vec3(width()/2.0f - 80.0f, 7.0f*height()/12.0f, 0);
      bodyP[2] = vec3(width()/2.0f - 60.0f, 11.0f*height()/24.0f, 0);
      bodyP[3] = vec3(width()/2.0f - 100.0f, 4.0f * height()/24.0f, 0);
      bodyP[4] = vec3(width()/2.0f, 1.0f*height()/12.0f, 0);
      bodyP[5] = vec3(width()/2.0f + 100.0f, 4.0f * height()/24.0f, 0);
      bodyP[6] = vec3(width()/2.0f + 60.0f, 11.0f*height()/24.0f, 0);
      bodyP[7] = vec3(width()/2.0f + 80.0f, 7.0f*height()/12.0f, 0);
      bodyP[8] = vec3(width()/2.0f, 14.0f*height()/24.0f, 0);

      for (int i = 0; i < 9; i++){
         body.appendKey((float) i, bodyP[i]);
      }

      bridgeP[0] = vec3(width()/2.0f - 25.0f, bottomheight + 1.0f * height() / 48.0f, 0);
      bridgeP[1] = vec3(width()/2.0f - 25.0f, bottomheight - 1.0f * height() / 48.0f, 0);
      bridgeP[2] = vec3(width()/2.0f + 25.0f, bottomheight - 1.0f * height() / 48.0f, 0);
      bridgeP[3] = vec3(width()/2.0f + 25.0f, bottomheight + 1.0f * height() / 48.0f, 0);
      bridgeP[4] = bridgeP[0];

      for (int i = 0; i < 5; i++){
         bridge.appendKey((float) i, bridgeP[i]);
      }

  }

  /** 
   * draw a guitar using splines
   */
  void scene() {
      setColor(vec3(1));
      //string post
      for (int i = 0; i < 6; i++){
         drawSphere(stringH[i], 3);
         drawSphere(stringB[i], 3);
         drawLine(stringH[i], stringB[i]);
      }

      float time1;
      float time2;
      float diff = 0.01;

      //head
      head.computeControlPoints();
      for (int i = 0; i < 4; i++){
         time1 = head.getTime(i);
         time2 = head.getTime(i+1);
         for (float t = time1; t <= time2; t=t+diff){
            setColor(vec3(1,1,1)); 
            a = head.getValue(t);
            b = head.getValue(t+diff);
            drawLine(a, b);
         } 
      }

      //body

      body.computeControlPoints();
      for (int i = 0; i < 8; i++){
         time1 = body.getTime(i);
         time2 = body.getTime(i+1);
         for (float t = time1; t <= time2; t=t+diff){
            setColor(vec3(1,1,1)); 
            a = body.getValue(t);
            b = body.getValue(t+diff);
            drawLine(a, b);
         } 
      }

      //bridge
      bridge.computeControlPoints();
      for (int i = 0; i < 4; i++){
         time1 = bridge.getTime(i);
         time2 = bridge.getTime(i+1);
         for (float t = time1; t <= time2; t=t+diff){
            setColor(vec3(1,1,1)); 
            a = bridge.getValue(t);
            b = bridge.getValue(t+diff);
            drawLine(a, b);
         } 
      }

      //sound hole
      float px;
      float py;
      float px1;
      float py1;
      float r = 30.0f;
      for (float t = 0; t <= 7; t=t+diff){
         px = r * cos(t) + 0.5 * width();
         py = r * sin(t) + 11.0f * height()/24.0f;
         px1 = r * cos(t+diff) + 0.5 * width();
         py1 = r * sin(t+diff) + 11.0f * height()/24.0f;
         drawLine(vec3(px, py, 0), vec3(px1, py1, 0));
      } 

      //finger pad
      a = vec3(width()/2.0f - 17.5f, 19.0f*height()/24.0 , 0);
      b = vec3(width()/2.0f - 17.5f, 7.0f*height()/12.0 , 0);
      drawLine(a, b);

      a = vec3(width()/2.0f + 17.5f, 19.0f*height()/24.0 , 0);
      b = vec3(width()/2.0f + 17.5f, 7.0f*height()/12.0 , 0);
      drawLine(a, b);

  }

 private:
   vec3 bodyP[9];
   vec3 headP[5];
   vec3 stringH[6];
   vec3 stringB[6];
   vec3 bridgeP[5];
   Spline head;
   Spline body;
   Spline bridge;
   vec3 a;
   vec3 b;
   float bottomheight;
   InterpolatorCatmullRom mCatmullRom;
   InterpolatorLinear mLinear;
};

int main(int argc, char** argv) {
  A3Unique viewer;
  viewer.run();
}
