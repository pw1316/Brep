#ifndef _QUAT_H_
#define _QUAT_H_

#include "Vec3.h"

class Quat {
public:
	Quat();
	Quat(float _x, float _y, float _z, float _w);
	Quat(const Vec3& v, float s);

	void fromAxisAngle(const Vec3& v, float angle);
	void toMatrix(float* m);
	float length() const;
    
    float x, y, z, w;
};

Quat operator*(const Quat& q2, const Quat& q1);
Quat normalize(const Quat& q);
Quat conjugate(const Quat& q);
Vec3 rotateVec(Vec3& v, const Quat& q);

#endif
