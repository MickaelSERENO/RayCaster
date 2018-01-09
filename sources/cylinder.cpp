#include "cylinder.h"

Cylinder::Cylinder() : Object()
{}

//We used this to help us: https://www.gamedev.net/forums/topic/467789-raycylinder-intersection/
Hit Cylinder::intersect(const Ray& ray)
{
	double tCylinder = -1;
	double tCircle1  = -1;
	double tCircle2  = -1;

	//Get the cylinder direction
	Triple cylinderDir = (rotation * Triple(1.0, 0.0, 0.0)).normalized();
	Triple leftCenter  = -length / 2.0 * cylinderDir + position;
	Triple rightCenter = +length / 2.0 * cylinderDir + position;

	Triple leftRight = rightCenter - leftCenter;
	Triple X         = (ray.O - leftCenter).cross(leftRight);
	Triple Y         = ray.D.cross(leftRight);
	//Use the quadratic equation : a*t^2 + b*t + c = 0; for knowing if we touched the outer part of the cylinder
	double a = Y.dot(Y);
	double b = 2 * X.dot(Y);
	double c = X.dot(X) - radius*radius * leftRight.dot(leftRight);

	double delta = b*b - 4*a*c;
	if (delta >= 0 && a != 0)
	{
		double tCylinder1 = (-b - sqrt(delta)) / (2 * a);
		double tCylinder2 = (-b + sqrt(delta)) / (2 * a);

		if (tCylinder1 > 0)
		{
			Triple p = ray.O + tCylinder1*ray.D;
			if (abs((p - position).dot(cylinderDir)) >= length / 2.0)
				tCylinder = -1;
			else
				tCylinder = tCylinder1;
		}

		if (tCylinder < 0 || tCylinder2 < tCylinder)
		{
			if (tCylinder2 >= 0)
			{
				Triple p = ray.O + tCylinder2*ray.D;
				if (abs((p - position).dot(cylinderDir)) < length / 2.0)
					tCylinder = tCylinder2;
			}
		}
	}

	//Look if we touched the circle parts (end part)
	//Use line - place intersection
	double DdotCylinderDir = ray.D.dot(cylinderDir);
	if (DdotCylinderDir != 0)
	{
		tCircle1 = (leftCenter - ray.O).dot(cylinderDir) / DdotCylinderDir;
		if (tCircle1 > 0) //Test if we are in the circle
		{
			Triple p = ray.O + tCircle1*ray.D;
			if ((p - leftCenter).length() >= radius)
				tCircle1 = -1;
		}

		tCircle2 = (rightCenter - ray.O).dot(cylinderDir) / DdotCylinderDir;
		if (tCircle2 > 0) //Test if we are in the circle
		{
			Triple p = ray.O + tCircle2*ray.D;
			if ((p - rightCenter).length() >= radius)
				tCircle2 = -1;
		}
	}

	//Check what intersection is the closest
	if (tCylinder <= 0 && tCircle1 <= 0 && tCircle2 <= 0)
		return Hit::NO_HIT();

	double t = std::numeric_limits<double>::max();
	Triple N;
	if (tCylinder > 0)
	{
		t = tCylinder;
		Triple p = ray.O + t*ray.D;
		Triple positionP = p - position;
		N = ((positionP) - cylinderDir*((positionP).dot(cylinderDir))).normalized();
	}

	if (tCircle1 < t && tCircle1 > 0)
	{
		t = tCircle1;
		N = -cylinderDir;
	}

	if(tCircle2 < t && tCircle2 > 0)
	{
		t = tCircle2;
		N = cylinderDir;
	}

	return Hit(t, N);
}

Color Cylinder::getTextureColor(const Point& p) const
{
	return Color(0, 0, 0);
}