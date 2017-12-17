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

Object* Scene::findHit(const Ray& ray, Hit& hit)
{
	// Find hit object and distance
	hit = Hit(std::numeric_limits<double>::infinity(), Vector());
	Object *obj = NULL;
	for (unsigned int i = 0; i < objects.size(); ++i) {
		Hit tempHit(objects[i]->intersect(ray));
		if (tempHit.t<hit.t && tempHit.N.dot(-ray.D) >= 0)
		{
			hit = tempHit;
			obj = objects[i];
		}
	}

	return obj;
}

Color Scene::trace(const Ray &ray)
{
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object* obj = findHit(ray, min_hit);
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
		color = phong(material, V, N, hit);
		Vector reflect = (ray.D -2 * ray.D.dot(N)*N).normalized();
		color += recursionColor(Ray(hit, reflect), maxRecursionDepth) * material->ks;
	}

	else if (mode == ZBUFFER) 
	{
		double distance = (eye - hit).length();
		color = Color(distance, distance, distance);
	}
	else if (mode == NORMAL) 
	{
		color = Color((1.0+N.x)/2.0, (1.0+N.y)/2.0, (1.0+N.z)/2.0);
	}

    return color;
}

Color Scene::recursionColor(const Ray& ray, int recursion)
{
	if (recursion <= 0)
		return Color(0.0, 0.0, 0.0);
	Hit min_hit(std::numeric_limits<double>::infinity(), Vector());
	Object* obj = findHit(ray, min_hit);

	// No hit? Return background color.
	if (!obj) return Color(0.0, 0.0, 0.0);

	Point hit = ray.at(min_hit.t);                 //the hit point
	Vector N = min_hit.N;                          //the normal at hit point
	Vector V = -ray.D;                             //the view vector

	Vector reflect = (-2 * ray.D.dot(N)*N + ray.D).normalized();
	Color color = phong(obj->material, V, N, hit);
	
	if (recursion == 1)
		return color;
    return color + obj->material->ks * recursionColor(Ray(hit, reflect), recursion - 1);
}

Color Scene::phong(const Material* material, const Vector& V, const Vector& N, const Point& hit)
{
	Color color(0.0, 0.0, 0.0);
	//Compute phong
	for (Light* l : lights)
	{

		Vector L = (l->position - hit).normalized();
		Vector R = (2 * L.dot(N)*N - L).normalized();

		double distance = (l->position - hit).length();
		double intensity = 1.0f / (4 * M_PI*distance*distance);
		intensity = 1.0f;

		if (drawShadow)
		{
			Ray rayToLight(hit, L);
			Hit hitToLight(std::numeric_limits<double>::infinity(), Vector());
			Object* objToLight = findHit(rayToLight, hitToLight);
			
			if (objToLight && (hit - rayToLight.at(hitToLight.t)).length_2() < (hit - l->position).length_2())
			{
				color += intensity * l->color * material->ka * material->color;
				continue;
			}
		}

		color += intensity * l->color * (
			material->ka * material->color +
			material->kd * fmax(0.0f, L.dot(N))*material->color +
			material->ks * pow(fmax(0.0f, R.dot(V)), material->n));
	}
	return color;
}

void Scene::render(Image &img)
{
	double zMax = 0.0;
	double zMin = std::numeric_limits<double>::max();
    int w = img.width();
    int h = img.height();
    for (int y = 0; y < camera->getHeight(); y++) {
        for (int x = 0; x < camera->getWidth(); x++) {

			Color pixColor(0, 0, 0);
			int N = superSamples;
			for (int y1 = 0; y1 < N; y1++)
			{
				for (int x1 = 0; x1 < N; x1++)
				{
					Ray ray = camera->castRay((x - 1.0 / (2.0 * N) + (x1 + 1.0) / N), (y - 1.0 / (2.0 * N) + (y1 + 1.0) / N));
					Color col = trace(ray);

					if (mode == PHONG)
					{
						col.clamp();
					}

					else if (mode == ZBUFFER) {

						if (col.x > zMax) {
							zMax = col.x;
						}
						if (col.x < zMin && col.x != 0.0) {
							zMin = col.x;
						}
					}

					pixColor = pixColor + col;
				}
			}

			pixColor = pixColor / (N*N);

            img(x,y) = pixColor;
        }
    }

	//Readapt the range of colors
	if (mode == ZBUFFER && zMax != zMin) {
		for (int y = 0; y < h; y++) {
			for (int x = 0; x < w; x++) {
				if(img(x, y).x == 0.0) //object not found
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

void Scene::setDrawShadow(bool ds)
{
	drawShadow = ds;
}

void Scene::setMaxRecursionDepth(int depth)
{
	maxRecursionDepth = depth;
}