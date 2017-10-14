#include "Vec3.h"
#include <cmath>

Vec3::Vec3() {

}

Vec3::Vec3(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {

}

Vec3 Vec3::operator+(const Vec3& v) const {
	return Vec3(x + v.x, y + v.y, z + v.z);
}

Vec3& Vec3::operator+=(const Vec3& v) {
	x += v.x;
	y += v.y;
	z += v.z;
	return *this;
}

Vec3 Vec3::operator-(const Vec3& v) const {
	return Vec3(x - v.x, y - v.y, z - v.z);
}

Vec3& Vec3::operator-=(const Vec3& v) {
	x -= v.x;
	y -= v.y;
	z -= v.z;
	return *this;
}

Vec3 Vec3::operator*(float s) const {
	return Vec3(x*s, y*s, z*s);
}

Vec3& Vec3::operator*=(float s) {
	x *= s;
	y *= s;
	z *= s;
        return *this;
}

Vec3 Vec3::operator/(float s) const {
	return Vec3(x/s, y/s, z/s);
}

Vec3& Vec3::operator/=(float s) {
	x /= s;
	y /= s;
	z /= s;
	return *this;
}

bool Vec3::operator==(const Vec3& v) const {
	if ((x == v.x) && (y == v.y) && (z == v.z))
		return true;
	else
		return false;
}

bool Vec3::operator!=(const Vec3& v) const {
	if (!(*this==v))
		return true;
	else
		return false;
}

float Vec3::length() const {
	return sqrt(x*x + y*y + z*z);
}

float dot(const Vec3& v1, const Vec3& v2) {
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vec3 cross(const Vec3& v1, const Vec3& v2) {
	return Vec3(v1.y*v2.z - v2.y*v1.z,
                v1.z*v2.x - v2.z*v1.x,
                v1.x*v2.y - v2.x*v1.y);
}

Vec3 normalize(const Vec3& v) {
	float length = v.length();
	return Vec3(v.x/length, v.y/length, v.z/length);
}

float angle(const Vec3& v1, const Vec3& v2) {
    return acos(dot(v1, v2)/(v1.length()*v2.length()));
}
