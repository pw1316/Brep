#ifndef _VEC3_H_
#define _VEC3_H_

class Vec3 {
public:
	Vec3();
	Vec3(float _x, float _y, float _z);
	
    Vec3 operator+(const Vec3& v) const;
	Vec3& operator+=(const Vec3& v);

	Vec3 operator-(const Vec3& v) const;
	Vec3& operator-=(const Vec3& v);

	Vec3 operator*(float s) const;
	Vec3& operator*=(float s);

	Vec3 operator/(float s) const;
	Vec3& operator/=(float s);

	bool operator==(const Vec3& v) const;
	bool operator!=(const Vec3& v) const;

	float length() const;

	float x, y, z;
};

float dot(const Vec3& v1, const Vec3& v2);
Vec3 cross(const Vec3& v1, const Vec3& v2);
Vec3 normalize(const Vec3& v);
float angle(const Vec3& v1, const Vec3& v2);

#endif
