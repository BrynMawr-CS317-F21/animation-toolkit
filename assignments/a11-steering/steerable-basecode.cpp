#include "steerable.h"
#include "behaviors.h"

using namespace glm;
using namespace atk;

float ASteerable::kVelKv = 150.0; 
float ASteerable::kOriKv = 150.0;  
float ASteerable::kOriKp = 150.0;

// Given a desired velocity, veld, and dt, compute a transform holding 
// the new orientation and change in position
//
// rotation control: tau = I * (-Kv1 * thetaDot + Kp1 * (thetaD - theta))
// translation control: f = m * Kv0 * (vd - v)
void ASteerable::senseControlAct(const vec3& veld, float dt)
{
   // Compute _vd and _thetad
   _vd = length(veld);
   _thetad = atan2(veld[0], veld[2]);

   // compute _force and _torque
   _force = _mass * kVelKv * (_vd - _state[VEL]);
   _torque = _inertia * (-kOriKv * _state[AVEL] + kOriKp * (_thetad - _state[ORI]));
   
   // find derivative
   _derivative[POS] = _state[VEL];
   _derivative[ORI] = _state[AVEL];
   _derivative[VEL] = _force/_mass;
   _derivative[AVEL] = _torque/_inertia;

   // update state
   _state[POS] += dt * _derivative[POS];
   _state[ORI] += dt * _derivative[ORI];
   _state[VEL] += dt * _derivative[VEL];
   _state[AVEL] += dt * _derivative[AVEL];

   // compute global position and orientation and update _characterRoot
   quat rot = glm::angleAxis(_state[ORI], vec3(0,1,0));
   vec3 localPos(0,0,_state[POS]);

   _characterRoot.setT(rot * localPos + _characterRoot.t());
   _characterRoot.setR(rot); 
}

// randomize the colors, characters, and animation times here
void ASteerable::randomizeAppearance()
{
   // to randomize the walking animations, compute different initial values 
   // for _time
   _time = 10.0f * agl::random();

   // to randomize color, call _drawer.setColor
   _drawer.color = agl::randomUnitVector();

   // to randomize shape, compute random values for _drawer.setJointRadius
   // or randomly assign different drawers to have a mix of characters
   _drawer.jointRadius = 20.0f * agl::random();
}

