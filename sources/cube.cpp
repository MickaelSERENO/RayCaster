#include "cube.h"

Hit Cube::intersect(const Ray& ray)
{
	//The directions
	Triple topDir = (rotation * Triple(0.0, 1.0, 0.0)).normalized();
	Triple rightDir = (rotation * Triple(1.0, 0.0, 0.0)).normalized();
	Triple frontDir = (rotation * Triple(0.0, 0.0, 1.0)).normalized();

	//The positions
	Triple topPos   = position + topDir*length/2;
	Triple botPos   = position - topDir*length/2;
	Triple leftPos  = position - rightDir*length/2;
	Triple rightPos = position + rightDir*length/2;
	Triple frontPos = position + frontDir*length/2;
	Triple backPos  = position - frontDir*length/2;

	double tLeft, tRight, tTop, tBottom, tFront, tBack;
	double t = -1;
	Triple N;
	
	//We will use 6 planars intersections
	//Top and bottom
	tTop = planeIntersection(ray, topPos, topDir, topDir, rightDir, frontDir, TOP);
	if (tTop >= 0)
	{
		t = tTop;
		N = topDir;
	}
	tBottom = planeIntersection(ray, botPos, -topDir, topDir, rightDir, frontDir, BOTTOM);
	if (tBottom >= 0 && (t < 0 || tBottom < t))
	{
		t = tBottom;
		N = -topDir;
	}

	//Left and right
	tLeft = planeIntersection(ray, leftPos, -rightDir, topDir, rightDir, frontDir, LEFT);
	if (tLeft >= 0 && (t < 0 || tLeft < t))
	{
		t = tLeft;
		N = -rightDir;
	}
	tRight = planeIntersection(ray, rightPos, rightDir, topDir, rightDir, frontDir, RIGHT);
	if (tRight >= 0 && (t < 0 || tRight < t))
	{
		t = tRight;
		N = rightDir;
	}

	//back and front
	tBack = planeIntersection(ray, backPos, -frontDir, topDir, rightDir, frontDir, BACK);
	if (tBack >= 0 && (t < 0 || tBack < t))
	{
		t = tBack;
		N = -frontDir;
	}
	tFront = planeIntersection(ray, frontPos, frontDir, topDir, rightDir, frontDir, FRONT);
	if (tFront >= 0 && (t < 0 || tFront < t))
	{
		t = tFront;
		N = frontDir;
	}

	if (t < 0)
		return Hit::NO_HIT();
	return Hit(t, N);
}

double Cube::planeIntersection(const Ray& ray, const Triple& p0, const Triple& N, const Triple& x0, const Triple& x1, const Triple& x2, Face f)
{
	double t = -1;
	double DdotN = ray.D.dot(N);
	if (DdotN != 0)
		t = (p0 - ray.O).dot(N) / DdotN;

	if (t < 0)
		return -1;

	Triple p       = ray.O + t*ray.D - p0;
	Triple pInBase = Triple(x0.x * p.x + x1.x * p.y + x2.x * p.z,  //Change the system coordinate
							x0.y * p.x + x1.y * p.y + x2.y * p.z,
							x0.z * p.x + x1.z * p.y + x2.z * p.z);

	if (f == TOP || f == BOTTOM)
	{
		if (pInBase.z > length / 2.0 || pInBase.z < -length / 2.0 ||
			pInBase.y > length / 2.0 || pInBase.y < -length / 2.0)
			return -1;
	}

	else if (f == LEFT || f == RIGHT)
	{
		if (pInBase.z > length / 2.0 || pInBase.z < -length / 2.0 ||
			pInBase.x > length / 2.0 || pInBase.x < -length / 2.0)
			return -1;
	}

	else
	{
		if (pInBase.y > length / 2.0 || pInBase.y < -length / 2.0 ||
			pInBase.x > length / 2.0 || pInBase.x < -length / 2.0)
			return -1;
	}

	return t;
}