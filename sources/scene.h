//
//  Framework for a raytracer
//  File: scene.h
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

#ifndef SCENE_H_KNBLQLP6
#define SCENE_H_KNBLQLP6

#include <vector>
#include "triple.h"
#include "light.h"
#include "object.h"
#include "image.h"
#include "Camera.h"
#include <omp.h>

#ifndef M_PI
	#define M_PI 3.14159265358979323846
#endif

enum renderMode {
	PHONG,
	ZBUFFER,
	NORMAL
};

class Scene
{
private:
    std::vector<Object*> objects;
    std::vector<Light*> lights;
    Triple eye;
	renderMode mode;
	bool drawShadow = false;
	int maxRecursionDepth=0;
	int superSamples = 1;
	Camera* camera = NULL;
public:
	Object* findHit(const Ray& ray, Hit& hit);
    Color trace(const Ray &ray);
	Color recursionColor(const Ray& ray, int recursion);
	Color phong(const Object* obj, const Vector& V, const Vector& N, const Point& hit);
    void render(Image &img);
    void addObject(Object *o);
    void addLight(Light *l);
    void setEye(Triple e);
	void setRenderMode(renderMode rm);
	void setDrawShadow(bool ds);
	void setMaxRecursionDepth(int d);
	void setSupersample(int s) { superSamples = s; }
	void setCamera(Camera* c) { camera = c; }
	Camera* getCamera() { return camera; }
    unsigned int getNumObjects() { return objects.size(); }
    unsigned int getNumLights() { return lights.size(); }
};

#endif /* end of include guard: SCENE_H_KNBLQLP6 */
