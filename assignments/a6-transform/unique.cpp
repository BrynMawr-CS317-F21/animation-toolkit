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

        Joint *head = new Joint("head");
        head->setLocalTranslation(vec3(width()/2, height()/2, 0));
        worm.addJoint(head);

        Joint *joint1 = new Joint("joint1");
        joint1->setLocalTranslation(vec3(50,0,0));
        worm.addJoint(joint1, head);

        Joint *joint2 = new Joint("joint2");
        joint2->setLocalTranslation(vec3(50,0,0));
        worm.addJoint(joint2, joint1);

        Joint *joint3 = new Joint("joint3");
        joint3->setLocalTranslation(vec3(50,0,0));
        worm.addJoint(joint3, joint2);

        Joint *joint4 = new Joint("joint4");
        joint4->setLocalTranslation(vec3(-50,0,0));
        worm.addJoint(joint4, head);

        Joint *joint5 = new Joint("joint5");
        joint5->setLocalTranslation(vec3(-50,0,0));
        worm.addJoint(joint5, joint4);

        Joint *joint6 = new Joint("joint6");
        joint6->setLocalTranslation(vec3(-50,0,0));
        worm.addJoint(joint6, joint5);

        worm.fk(); // compute local2global transforms
    }

    virtual void scene()
    {
        quat tmp;
        Joint *someJoint;
        for (int i = 1; i < worm.getNumJoints()-1; i++)
        {
            theta = pow(-1, i)*sin(elapsedTime())*sin(elapsedTime());
            tmp = glm::angleAxis(theta, vec3(0, 0, 1));
            someJoint = worm.getByID(i);
            someJoint->setLocalRotation(tmp);
        }
        worm.fk(); 
        setColor(vec3(0, 1, 0));

        Joint *parent = NULL;
        Joint *child = NULL;
        for (int i = 0; i < 3; i++)
        {
            parent = worm.getByID(i);
            child = worm.getByID(i + 1);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }
            parent = worm.getByID(0);
            child = worm.getByID(4);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        for (int i = 4; i < worm.getNumJoints()-1; i++){
            parent = worm.getByID(i);
            child = worm.getByID(i + 1);
            vec3 globalParentPos = parent->getGlobalTranslation();
            vec3 globalPos = child->getGlobalTranslation();
            drawEllipsoid(globalParentPos, globalPos, 5);
        }
    }

protected:
    Skeleton worm;
    float theta;
    float thetaRate;
};

int main(int argc, char **argv)
{
    A6Unique viewer;
    viewer.run();
    return 0;
}
