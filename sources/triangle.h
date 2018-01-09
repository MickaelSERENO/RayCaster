#pragma once
#include "object.h"
class Triangle : public Object
{
public:
	Triangle();
	virtual Hit intersect(const Ray& ray);
	virtual Color getTextureColor(const Point& p) const;
	~Triangle();

	Point p1;
	Point p2;
	Point p3;
};

bool isCorrectOrder(Vector seg, Vector inter);

