//
//  Framework for a raytracer
//  File: material.h
//
//  Created for the Computer Science course "Introduction Computer Graphics"
//  taught at the University of Groningen by Tobias Isenberg.
//
//  Author: Maarten Everts
//
//  This framework is inspired by and uses code of the raytracer framework of 
//  Bert Freudenberg that can be found at
//  http://isgwww.cs.uni-magdeburg.de/graphik/lehre/cg2/projekt/rtprojekt.html 
//

#ifndef MATERIAL_H_TWMNT2EJ
#define MATERIAL_H_TWMNT2EJ

#include <iostream>
#include "triple.h"
#include "image.h"
#include <sstream>
#include <vector>
#include <string>
#include <istream>

inline std::vector<std::string> split(const std::string& buffer, char delim)
{
	std::vector<std::string> r;
	std::istringstream iss(buffer);
	std::string s;
	while (getline(iss, s, delim))
		r.push_back(s);
	return r;
}

class Material
{
public:
	virtual Color getBase() = 0;
	virtual Color getDiffuse() = 0;
	virtual Color getAmbient() = 0;
	virtual Color getSpecular() = 0;
	virtual Color getTextureColor() = 0;

	double n;           // exponent for specular highlight size
	double kd;          // diffuse intensity
	double ka;          // ambient intensity
	double ks;          // specular intensity 

	Image* texture = NULL; // The texture

    Material() { }
};

class GeomMaterial : public Material
{
public:
	virtual Color getBase() { return color; }
	virtual Color getDiffuse() { return Color(1.0, 1.0, 1.0); }
	virtual Color getAmbient() { return Color(1.0, 1.0, 1.0); }
	virtual Color getSpecular() { return Color(1.0, 1.0, 1.0); }
	virtual Color getTextureColor() { return Color(1.0, 1.0, 1.0); }

	Color color;        // base color
};

class ObjMaterial : public Material
{
public:
	virtual Color getBase() { return Color(1.0, 1.0, 1.0); }
	virtual Color getDiffuse() { return diffuse; }
	virtual Color getAmbient() { return ambient; }
	virtual Color getSpecular() { return specular; }
	virtual Color getTextureColor() { return diffuse; }
	Color diffuse, ambient, specular;

	ObjMaterial() : Material()
	{
		kd = 1.0;
		ka = 1.0;
		ks = 1.0;
	}
};

#endif /* end of include guard: MATERIAL_H_TWMNT2EJ */
