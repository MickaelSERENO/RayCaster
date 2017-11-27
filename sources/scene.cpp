//
//  Framework for a raytracer
//  File: scene.cpp
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

#include "scene.h"
#include "material.h"

Color Scene::trace(const Ray &ray)
{
    // Find hit object and distance
    Hit min_hit(std::numeric_limits<double>::infinity(),Vector());
    Object *obj = NULL;
    for (unsigned int i = 0; i < objects.size(); ++i) {
        Hit hit(objects[i]->intersect(ray));
        if (hit.t<min_hit.t) {
            min_hit = hit;
            obj = objects[i];
        }
    }

    // No hit? Return background color.
    if (!obj) return Color(0.0, 0.0, 0.0);

    Material *material = obj->material;            //the hit objects material
    Point hit = ray.at(min_hit.t);                 //the hit point
    Vector N = min_hit.N;                          //the normal at hit point
    Vector V = -ray.D;                             //the view vector

	Color color = Color(0.0, 0.0, 0.0);

	if (mode == PHONG) {
		/****************************************************
		* This is where you should insert the color
		* calculation (Phong model).
		*
		* Given: material, hit, N, V, lights[]
		* Sought: color
		*
		* Hints: (see triple.h)
		*        Triple.dot(Vector) dot product
		*        Vector+Vector      vector sum
		*        Vector-Vector      vector difference
		*        Point-Point        yields vector
		*        Vector.normalize() normalizes vector, returns length
		*        double*Color        scales each color component (r,g,b)
		*        Color*Color        dito
		*        pow(a,b)           a to the power of b
		****************************************************/

		
		for (Light* l : lights)
		{
			Vector L = (l->position - hit).normalized();
			Vector R = (2 * L.dot(N)*N - L).normalized();

			double distance = (l->position - hit).length();
			double intensity = 1.0f / (4 * M_PI*distance*distance);
			intensity = 1.0f;

			color += intensity * (
				material->ka * material->color +
				material->kd * fmax(0.0f, L.dot(N))*material->color +
				material->ks * pow(fmax(0.0f, R.dot(V)), material->n)* l->color);
		}
	}
	else if (mode == ZBUFFER) {
		double distance = (eye - hit).length();
		color = Color(distance, distance, distance);
	}
	else if (mode == NORMAL) {

	}

    return color;
}

void Scene::render(Image &img)
{
	double zMax = std::numeric_limits<double>::min();
	double zMin = std::numeric_limits<double>::max();
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < h; y++) {
        for (int x = 0; x < w; x++) {
            Point pixel(x+0.5, h-1-y+0.5, 0);
            Ray ray(eye, (pixel-eye).normalized());
            Color col = trace(ray);

			if (mode == PHONG) {
				col.clamp();
			}
			else if (mode == ZBUFFER) {

				if (col.x > zMax) {
					zMax = col.x;
				}
				if (col.x < zMin && zMin != 0.0) {
					zMin = col.x;
				}
			}

            img(x,y) = col;
        }
    }
	if (mode == ZBUFFER && zMax != zMin) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if(img(x, y).x == 0.0)
					continue;
				double colorDist = 1.0 - (img(x, y).x - zMin) / (zMax - zMin); //set in range and invert so the closer is the brighter
				img(x, y) = Color(colorDist, colorDist, colorDist);
			}
		}
	}
}

void Scene::addObject(Object *o)
{
    objects.push_back(o);
}

void Scene::addLight(Light *l)
{
    lights.push_back(l);
}

void Scene::setEye(Triple e)
{
    eye = e;
}

void Scene::setRenderMode(renderMode rm)
{
	mode = rm;
}
