#include "triangle.h"

Triangle::Triangle()
{
}

Hit Triangle::intersect(const Ray & ray)
{
	/*//Normal of the triangle
	Vector N = (p1 - p3).cross(p1 - p2).normalized();

	double t = -1;
	double DdotN = ray.D.dot(N);
	if (DdotN != 0)
		t = (p1 - ray.O).dot(N) / DdotN;
	
	if (t <= 0)
		return Hit::NO_HIT();

	Triple p = ray.O + t*ray.D;

	//Define the three segments-normals
	Vector N1 = N.cross(p1 - p2);
	Vector N2 = N.cross(p2 - p3);
	Vector N3 = N.cross(p3 - p1);

	if (SIGN(N1.dot(p - p1)) == SIGN(N2.dot(p - p2)) && SIGN(N1.dot(p - p1)) == SIGN(N3.dot(p - p3)))
	{
		return Hit(t, N);
	}
	return Hit::NO_HIT();*/

	const float EPSILON = 0.0000001;
	Vector edge1, edge2, h, s, q;
	float a, f, u, v;
	edge1 = p2 - p1;
	edge2 = p3 - p1;
	h = ray.D.cross(edge2);
	a = edge1.dot(h);
	if (a > -EPSILON && a < EPSILON)
		return Hit::NO_HIT();
	f = 1 / a;
	s = ray.O - p1;
	u = f * (s.dot(h));
	if (u < 0.0 || u > 1.0)
		return Hit::NO_HIT();
	q = s.cross(edge1);
	v = f * ray.D.dot(q);
	if (v < 0.0 || u + v > 1.0)
		return Hit::NO_HIT();
	// At this stage we can compute t to find out where the intersection point is on the line.
	float t = f * edge2.dot(q);
	if (t > EPSILON) // ray intersection
	{
		Vector N = (p1 - p3).cross(p1 - p2).normalized();
		return Hit(t, N);
	}
	else // This means that there is a line intersection but not a ray intersection.
		return Hit::NO_HIT();
}

Color Triangle::getTextureColor(const Point & p) const
{
	return Color();
}

Triangle::~Triangle()
{
}
