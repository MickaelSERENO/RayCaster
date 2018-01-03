#ifndef QUATERNION_H
#define QUATERNION_H

#include <cmath>
#include "triple.h"

template <typename T>
inline T min(const T& a, const T& b) { return a < b ? a : b; }

class Quaternion
{
public:
	Quaternion();
	Quaternion(double q0, double q1, double q2, double q3);
	Quaternion(const Triple& axis, double angle);

	Quaternion inverse() const;
	double a, b, c, d;
};

inline Triple operator*(const Quaternion& q, const Triple& v)
{
	Triple result;

	result.x = (q.a*q.a + q.b*q.b - q.c*q.c - q.d*q.d) * v.x +
			   2 * (-q.a*q.d + q.b*q.c) * v.y +
			   2 * (q.a*q.c + q.b*q.d) * v.z;

	result.y = 2 * (q.a*q.d + q.b*q.c) * v.x + 
			   (q.a*q.a - q.b*q.b + q.c*q.c - q.d*q.d) * v.y + 
			   2 * (-q.a*q.b + q.c*q.d) * v.z;

	result.z = 2 * (-q.a*q.c + q.b*q.d) * v.x +
		2 * (q.a*q.b + q.c*q.d)* v.y +
		(q.a*q.a - q.b*q.b - q.c*q.c + q.d*q.d) * v.z;




	return result;
}

#endif