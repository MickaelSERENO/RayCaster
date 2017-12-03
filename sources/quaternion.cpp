#include "quaternion.h"

Quaternion::Quaternion() : a(1), b(0), c(0), d(0)
{}

Quaternion::Quaternion(double q0, double q1, double q2, double q3) : a(q0), b(q1), c(q2), d(q3)
{
	double length = a*a + b*b + c*c + d*d;
	a /= length;
	b /= length;
	c /= length;
	d /= length;
}

Quaternion::Quaternion(const Triple& euler, double angle)
{
	Triple e = euler.normalized();

	a = cos(angle / 2);
	b = sin(angle / 2) * cos(e.x);
	c = sin(angle / 2) * cos(e.y);
	d = sin(angle / 2) * cos(e.z);
}

Quaternion Quaternion::inverse()
{
	return Quaternion(a, -b, -c, -d);
}