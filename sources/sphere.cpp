//
//  Framework for a raytracer
//  File: sphere.cpp
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Authors:
//    Maarten Everts
//    Jasper van de Gronde
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#include "sphere.h"
#include <iostream>
#include <math.h>

/************************** Sphere **********************************/

Hit Sphere::intersect(const Ray &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    * 
    * Insert calculation of ray/sphere intersection here. 
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return Hit::NO_HIT().
    * Otherwise, return an instance of Hit() with the distance of the
    * intersection point from the ray origin as t and the normal ad N (see example).
    ****************************************************/

    //place holder for actual intersection calculation
	//The code use mainly the concept described here : https://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-sphere-intersection

    Vector OC  = (position - ray.O);
	double tca = OC.dot(ray.D);
	if(tca < 0)
		return Hit::NO_HIT();

	double d   = sqrt(OC.length_2() - tca*tca); 
	if(d < 0 || d > r)
		return Hit::NO_HIT();

	double thc = sqrt(r*r - d*d);
	double t   = tca - thc; //The parameterized value of the ray : ray = O + t*D

    /****************************************************
    * RT1.2: NORMAL CALCULATION
    *
    * Given: t, C, r
    * Sought: N
    * 
    * Insert calculation of the sphere's normal at the intersection point.
    ****************************************************/

    Vector N = ((ray.O + t*ray.D) - position).normalized();

    return Hit(t,N);
}