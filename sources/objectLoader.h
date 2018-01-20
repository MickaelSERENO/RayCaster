#pragma once

/*
	This class is constructed using the code done by the past at the following git repository : https://github.com/MickaelSereno/AndroidEngine
*/

#include "object.h"
#include "stdlib.h"
#include "materialWrapper.h"
#include "triangle.h"
#include <sstream>
#include <istream>
#include <fstream>
#include <map>
#include <regex>
#include <string>


#define COORDS_PER_VERTEX 3

/** \brief the datas of one OBJ object */

class OBJWF : public Object
{
public:
	Triangle* triangles; /** <Array of triangles*/
	int nbTriangles;

	virtual Hit intersect(const Ray &ray)
	{
		Hit currentHit = Hit::NO_HIT();
		for (int i = 0; i < nbTriangles; i++)
		{
			Hit tHit(triangles[i].intersect(ray));
			if (tHit.t < currentHit.t || currentHit.no_hit)
				currentHit = tHit;
		}
		return currentHit;
	}

	virtual Color getTextureColor(const Point& p) const
	{
		return Color(0, 0, 0);
	}
};

class OBJDatas
{
public:
	MaterialWrapper* mtlWrapper;/** <The material wrapper associated with this OBJ */

	std::vector<OBJWF*> objects;
	OBJWF* lastObj = NULL;

	void insertNewObj(OBJWF* obj) 
	{
		lastObj = obj;
		objects.push_back(obj); 
	}

	std::vector<OBJWF*>& getObjects() { return objects; }
};

class ObjectLoader
{
public:
	ObjectLoader(const std::string& filePath, const Vector& defaultOrigin, double scale);
	~ObjectLoader();
	std::map<std::string, OBJDatas*>& getObjDatas() { return m_objDatas; }
private:
	std::map<std::string, MaterialWrapper*> m_mtlWrapper;
	std::map<std::string, OBJDatas*> m_objDatas;
	GeomMaterial* m_defaultMaterial;
};

