#include "triangle.h"



Triangle::Triangle()
{
}

Hit Triangle::intersect(const Ray & ray)
{
	//Normal of the triangle
	Vector N = (p1 - p3).cross(p2 - p1).normalized();

	double t = -1;
	double DdotN = ray.D.dot(N);
	if (DdotN != 0)
		t = (p1 - ray.O).dot(N) / DdotN;
	
	if (t <= 0)
		return Hit::NO_HIT();

	Triple p = ray.O + t*ray.D - p1;

	//Define the three segments-normals
	Vector N1 = N.cross(p2 - p1);
	Vector N2 = N.cross(p3 - p2);
	Vector N3 = N.cross(p3 - p1);

	if (SIGN(N1.dot(p - p3)) == SIGN(N2.dot(p - p1)) && SIGN(N1.dot(p - p3)) == SIGN(N3.dot(p - p2)))
	{
		return Hit(t, N);
	}
	return Hit::NO_HIT();
}

Color Triangle::getTextureColor(const Point & p) const
{
	return Color();
}

Triangle::~Triangle()
{
}
