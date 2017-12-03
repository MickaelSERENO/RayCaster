#ifndef CYLINDER_H
#define CYLINDER_H

#include "object.h"
#include "quaternion.h"

class Cylinder : public Object
{
public:
	Cylinder();
	virtual Hit intersect(const Ray& ray);

	double     radius = 0.0;
	double     length = 0.0;
	Point      position;
	Quaternion rotation;
};

#endif