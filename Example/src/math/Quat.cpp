#include "Quat.h"
#include <cmath>

#define PI 3.14159265

Quat::Quat() {

}

Quat::Quat(float _x, float _y, float _z, float _w) : x (_x), y(_y), z(_z), w(_w) {

}

Quat::Quat(const Vec3& v, float s) {
	x = v.x;
	y = v.y;
	z = v.z;
	w = s;
}

void Quat::fromAxisAngle(const Vec3& v, float theta) {
	float angle = theta * 0.5;
	w = std::cos(angle);
	x = v.x * std::sin(angle);
	y = v.y * std::sin(angle);
	z = v.z * std::sin(angle);
	*this = normalize(*this);
}

/**transforma um quaternion em uma matriz do OpenGL (row major)
 * |m[0]	m[4]	m[8]	m[12]|
 * |m[1]	m[5]	m[9]	m[13]|
 * |m[2]	m[6]	m[10]	m[14]|
 * |m[3]	m[7]	m[11]	m[15]|
 */
void Quat::toMatrix(float* m) {
	//float s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
	//s = 2.0/length();
	float xx, yy, zz, wx, wy, wz, xy, xz, yz;
	xx = 2*x*x;		yy = 2*y*y;		zz = 2*z*z;
	wx = 2*w*x;		wy = 2*w*y;		wz = 2*w*z;
	xy = 2*x*y;		xz = 2*x*z;		yz = 2*y*z;
	/*xs = s*x;	ys = s*y;	zs = s*z;
	wx = w*xs;	wy = w*ys;	wz = w*zs;
	xx = x*xs;	xy = x*ys;	xz = x*zs;
	yy = y*ys;	yz = y*zs;	zz = z*zs;*/

	m[0] = 1.0 - yy - zz;
	m[1] = xy - wz;
	m[2] = xz + wy;

	m[4] = xy + wz;
	m[5] = 1.0 - xx - zz;
	m[6] = yz - wx;

	m[8] = xz - wy;
	m[9] = yz + wx;
	m[10] = 1.0 - xx - yy;

	m[3] = m[7] = m[11] =  m[12] = m[13] = m[14] = 0.0;
	m[15] = 1.0;
}

Quat operator*(const Quat& q2, const Quat& q1){
	float _x, _y, _z, _w;

	_x = q2.y*q1.z - q2.z*q1.y + q2.w*q1.x + q2.x*q1.w;
	_y = q2.z*q1.x - q2.x*q1.z + q2.w*q1.y + q2.y*q1.w;
	_z = q2.x*q1.y - q2.y*q1.x + q2.w*q1.z + q2.z*q1.w;
	_w = q2.w*q1.w - q2.x*q1.x - q2.y*q1.y - q2.z*q1.z;

	return Quat(_x, _y, _z, _w);
}

float Quat::length() const {
	return sqrt(x*x + y*y + z*z + w*w);
}

Quat normalize(const Quat& q) {
	float length = q.length();

	return Quat(q.x/length, q.y/length, q.z/length, q.w/length);
}

Quat conjugate(const Quat& q) {
	return Quat(-q.x, -q.y, -q.z, q.w);
}

Vec3 rotateVec(Vec3& v, const Quat& q) {
    Quat q1(v.x, v.y, v.z, 0);
    Quat result = (q*q1)*conjugate(q);
    Vec3 resultVec(result.x, result.y, result.z);
    return resultVec; 
}
