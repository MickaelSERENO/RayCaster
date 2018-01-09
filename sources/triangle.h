#pragma once
#include "object.h"

#define SIGN(x) ((x < 0) ? -1 : 1)

class Triangle : public Object
{
public:
	Triangle();
	Triangle(const Point& a, const Point& b, const Point& c) : p1(a), p2(b), p3(c) {}
	virtual Hit intersect(const Ray& ray);
	virtual Color getTextureColor(const Point& p) const;
	~Triangle();

	Point p1;
	Point p2;
	Point p3;
};