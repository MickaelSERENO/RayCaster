#include "triangle.h"



Triangle::Triangle()
{
}

Hit Triangle::intersect(const Ray & ray)
{
	//Normal of the triangle
	Vector N = (p2 - p1).cross(p3 - p1).normalized();

	double t = -1;
	double DdotN = ray.D.dot(N);
	if (DdotN != 0)
		t = (p1 - ray.O).dot(N) / DdotN;
	
	if (t <= 0)
		return Hit::NO_HIT();

	Triple p = ray.O + t*ray.D - p1;

	//Define the three segments-normals
	/*Vector N1;
	Vector P23 = p3 - p2;

	if(P23.x != 0)
		N1 = (-P23.y -P23.z) / P23.x
	Vector N2;
	Vector N3;

	return Hit();*/
}

Color Triangle::getTextureColor(const Point & p) const
{
	return Color();
}

bool isCorrectOrder(Vector seg, Vector inter)
{
	double angle = atan2(seg.y, seg.x) - atan2(inter.y, inter.x);
	return angle < 0;
}


Triangle::~Triangle()
{
}
