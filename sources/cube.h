#ifndef CUBE_H
#define CUBE_H

enum Face
{
	LEFT,
	RIGHT,
	TOP,
	BOTTOM,
	FRONT,
	BACK
};

#include "object.h"
#include "quaternion.h"

class Cube : public Object
{
	public:
		Cube() : Object() {}
		virtual Hit intersect(const Ray& ray);

		double     length = 0.0;
		Point      position;
		Quaternion rotation;
	protected:
		double planeIntersection(const Ray& ray, const Triple& p0, const Triple& N, const Triple& x0, const Triple& x1, const Triple& x2, Face f);
};

#endif

