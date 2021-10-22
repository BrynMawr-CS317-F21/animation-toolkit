#include "atkmath/quaternion.h"
#include "atkmath/matrix3.h"
#include "atkmath/vector3.h"

namespace atkmath {

Quaternion Quaternion::Slerp(const Quaternion& q0, const Quaternion& q1, double t)
{
	float dotP = (float)Dot(q0, q1);
	Quaternion tmp;
	float omega;
	if(dotP<0){
		Quaternion q3 = -q0;
		dotP = (float)Dot(q3, q1);
		omega = acos(dotP);
		tmp = (double)(sin(omega*(1-t))/sin(omega))*q3 
						+ (double)(sin(omega*t)/sin(omega))*q1;
		return tmp;
	}
	dotP = (float)Dot(q0, q1);
	omega = acos(dotP);
	tmp = (double)(sin(omega*(1-t))/sin(omega))*q0 
						+ (double)(sin(omega*t)/sin(omega))*q1;
	return tmp;
}

void Quaternion::toAxisAngle (Vector3& axis, double& angleRad) const
{
	angleRad = acos(mW) * 2;
	axis[0] = mX/sin(angleRad/2);
	axis[1] = mY/sin(angleRad/2);
	axis[2] = mZ/sin(angleRad/2);
}

void Quaternion::fromAxisAngle (const Vector3& axis, double angleRad)
{
	float theta = (float)angleRad/2;
	Vector3 tmp = axis.normalized();
	mX = tmp[0] * sin(theta);
	mY = tmp[1] * sin(theta);
	mZ = tmp[2] * sin(theta);
	mW = cos(theta);
}

Matrix3 Quaternion::toMatrix () const
{
	Matrix3 tmp;
	float a11, a12, a13, a21, a22, a23, a31, a32, a33;
	a11 = 1 - 2 * (mY * mY + mZ * mZ);
	a12 = 2 * (mX * mY - mW * mZ);
	a13 = 2 * (mX * mZ + mW * mY);
	a21 = 2 * (mX * mY + mW * mZ);
	a22 = 1 - 2 * (mX * mX + mZ * mZ);
	a23 = 2 * (mZ * mY - mW * mX);
	a31 = 2 * (mX * mZ - mW * mY);
	a32 = 2 * (mZ * mY + mW * mX);	
	a33 = 1 - 2 * (mY * mY + mX * mX);
	tmp = Matrix3(a11, a12, a13, a21, a22, a23, a31, a32, a33);
	return tmp;
}

void Quaternion::fromMatrix(const Matrix3& rot)
{
	float w2 = 0.25 * (1 + rot[0][0] + rot[1][1] + rot[2][2]);
	float x2 = 0.25 * (1 + rot[0][0] - rot[1][1] - rot[2][2]);
	float y2 = 0.25 * (1 - rot[0][0] + rot[1][1] - rot[2][2]);
	float z2 = 0.25 * (1 - rot[0][0] - rot[1][1] + rot[2][2]);
	float max = std::max({w2, x2, y2, z2});
	float w,x,y,z;
	if(max == w2){
		w = (float)sqrt(w2);
		x = 0.25 * (rot[2][1] - rot[1][2]) / w;
		y = 0.25 * (rot[0][2] - rot[2][0]) / w;
		z = 0.25 * (rot[1][0] - rot[0][1]) / w;
	}
	else if(max == x2){
		x = sqrt(x2);
		y = 0.25 * (rot[1][0] + rot[0][1]) / x;
		z = 0.25 * (rot[0][2] + rot[2][0]) / x;
		w = 0.25 * (rot[2][1] - rot[1][2]) / x;
	}
	else if(max == y2){
		y = sqrt(y2);
		x = 0.25 * (rot[1][0] + rot[0][1]) / y;
		z = 0.25 * (rot[1][2] + rot[2][1]) / y;
		w = 0.25 * (rot[0][2] - rot[2][0]) / y;
	} else {
		z = sqrt(z2);
		x = 0.25 * (rot[0][2] + rot[2][0]) / z;
		y = 0.25 * (rot[1][2] + rot[2][1]) / z;
		w = 0.25 * (rot[1][0] - rot[0][1]) / z;
	}
	mX = x;
	mY = y;
	mZ = z;
	mW = w;
}

}