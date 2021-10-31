#include "atkui/framework.h"
#include "atk/toolkit.h"
using namespace atk;
using atk::Transform;
using glm::quat;
using glm::vec3;

class A6Unique : public atkui::Framework
{
public:
    A6Unique() : atkui::Framework(atkui::Orthographic)
    {
    }

    virtual void setup()
    {
        theta = 0.0;
        thetaRate = 0.1;
        //0
        Joint *root = new Joint("root");
        root->setLocalTranslation(vec3(width()/2, height()/2, 0));
        human.addJoint(root);
        //1
        Joint *leftShoulder = new Joint("leftShoulder");
        leftShoulder->setLocalTranslation(vec3(20,0,0));
        human.addJoint(leftShoulder, root);
        //2
        Joint *lUpperArm = new Joint("lUpperArm");
        lUpperArm->setLocalTranslation(vec3(30,0,0));
        human.addJoint(lUpperArm, leftShoulder);
        //3
        Joint *lForeArm = new Joint("lForeArm");
        lForeArm->setLocalTranslation(vec3(30,0,0));
        human.addJoint(lForeArm, lUpperArm);
        //4
        Joint *rightShoudler = new Joint("rightShoudler");
        rightShoudler->setLocalTranslation(vec3(-20,0,0));
        human.addJoint(rightShoudler, root);
        //5
        Joint *rUpperArm = new Joint("rUpperArm");
        rUpperArm->setLocalTranslation(vec3(-30,0,0));
        human.addJoint(rUpperArm, rightShoudler);
        //6
        Joint *rForeArm = new Joint("rForeArm");
        rForeArm->setLocalTranslation(vec3(-30,0,0));
        human.addJoint(rForeArm, rUpperArm);
        //7
        Joint *body = new Joint("body");
        body->setLocalTranslation(vec3(0,-50,0));
        human.addJoint(body, root);
        //8
        Joint *leftLeg = new Joint("leftLeg");
        leftLeg->setLocalTranslation(vec3(-50, -100, 0));
        human.addJoint(leftLeg, body);
        //9
        Joint *leftFoot = new Joint("leftFoot");
        leftFoot->setLocalTranslation(vec3(-20, 0, 0));
        human.addJoint(leftFoot, leftLeg);
        //10
        Joint *rightLeg = new Joint("rightLeg");
        rightLeg->setLocalTranslation(vec3(50, -100, 0));
        human.addJoint(rightLeg, body);
        //11
        Joint *rightFoot = new Joint("rightFoot");
        rightFoot->setLocalTranslation(vec3(20, 0, 0));
        human.addJoint(rightFoot, rightLeg);
        //12
        Joint *neck = new Joint("neck");
        neck->setLocalTranslation(vec3(0, 30, 0));
        human.addJoint(neck, root);
        //13
        Joint *head = new Joint("head");
        head->setLocalTranslation(vec3(0, 20, 0));
        human.addJoint(head, neck);

        human.fk(); // compute local2global transforms
    }

    virtual void scene()
    {
        quat tmp;
        Joint *someJoint;
        for (int i = 1; i < 7; i++)
        {
            theta = pow(-1, i)*sin(elapsedTime())*sin(elapsedTime());
            tmp = glm::angleAxis(theta, vec3(0, 0, 1));
            someJoint = human.getByID(i);
            someJoint->setLocalRotation(tmp);
        }
        human.fk(); 
        setColor(vec3(0, 1, 0));

        Joint *parent = NULL;
        Joint *child = NULL;
        vec3 globalParentPos;
        vec3 globalPos;
        for (int i = 0; i < 3; i++)
        {
            parent = human.getByID(i);
            child = human.getByID(i + 1);
            globalParentPos = parent->getGlobalTranslation();
            globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }
            parent = human.getByID(0);
            child = human.getByID(4);
            globalParentPos = parent->getGlobalTranslation();
            globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);

        for (int i = 4; i < 6; i++){
            parent = human.getByID(i);
            child = human.getByID(i + 1);
            globalParentPos = parent->getGlobalTranslation();
            globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }

        parent = human.getByID(0);
        child = human.getByID(7);
        globalParentPos = parent->getGlobalTranslation();
        globalPos = child->getGlobalTranslation();
        drawEllipsoid(globalParentPos, globalPos, 5);

        for (int i = 7; i < 9; i++){
            parent = human.getByID(i);
            child = human.getByID(i + 1);
            globalParentPos = parent->getGlobalTranslation();
            globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }

        parent = human.getByID(7);
        child = human.getByID(10);
        globalParentPos = parent->getGlobalTranslation();
        globalPos = child->getGlobalTranslation();
        drawEllipsoid(globalParentPos, globalPos, 5);

        for (int i = 10; i < 11; i++){
            parent = human.getByID(i);
            child = human.getByID(i + 1);
            globalParentPos = parent->getGlobalTranslation();
            globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }

        parent = human.getByID(0);
        child = human.getByID(12);
        globalParentPos = parent->getGlobalTranslation();
        globalPos = child->getGlobalTranslation();
        drawEllipsoid(globalParentPos, globalPos, 5);  

        parent = human.getByID(12);
        child = human.getByID(13);
        globalParentPos = parent->getGlobalTranslation();
        globalPos = child->getGlobalTranslation();
        drawEllipsoid(globalParentPos, globalPos, 15); 


    }

protected:
    Skeleton human;
    float theta;
    float thetaRate;
};

int main(int argc, char **argv)
{
    A6Unique viewer;
    viewer.run();
    return 0;
}
