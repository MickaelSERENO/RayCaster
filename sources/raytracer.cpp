//
//  Framework for a raytracer
//  File: raytracer.cpp
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

#include "raytracer.h"
#include "object.h"
#include "cylinder.h"
#include "sphere.h"
#include "cube.h"
#include "material.h"
#include "light.h"
#include "image.h"
#include "yaml/yaml.h"
#include <ctype.h>
#include <fstream>
#include <assert.h>

// Functions to ease reading from YAML input
void operator >> (const YAML::Node& node, Triple& t);
Triple parseTriple(const YAML::Node& node);

void operator >> (const YAML::Node& node, Triple& t)
{
    assert(node.size()==3);
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;
}

Triple parseTriple(const YAML::Node& node)
{
    Triple t;
    node[0] >> t.x;
    node[1] >> t.y;
    node[2] >> t.z;	
    return t;
}

Material* Raytracer::parseMaterial(const YAML::Node& node)
{
    Material *m = new Material();
	
	//Test if the material contain a texture or not
	const YAML::Node*texture = node.FindValue("texture");
	if (!texture)
		node["color"] >> m->color;
	else
	{
		std::string texturePath;
		node["texture"] >> texturePath;
		std::cout << "texture found : " << texturePath << std::endl;
		m->texture = new Image(texturePath.c_str());
	}	
    node["ka"] >> m->ka;
    node["kd"] >> m->kd;
    node["ks"] >> m->ks;
    node["n"] >> m->n;
    return m;
}

Object* Raytracer::parseObject(const YAML::Node& node)
{
    Object *returnObject = NULL;
    std::string objectType;
    node["type"] >> objectType;

    if (objectType == "sphere") {
        Point pos;
        node["position"] >> pos;
        double r;
        node["radius"] >> r;
        Sphere *sphere = new Sphere(pos,r);		
        returnObject = sphere;
    }

	else if (objectType == "cylinder")
	{
		Point pos;
		node["position"] >> pos;
		double r;
		node["radius"] >> r;
		double length;
		node["length"] >> length;
		Cylinder* cylinder = new Cylinder();
		cylinder->position = pos;
		cylinder->length   = length;
		cylinder->radius   = r;
		returnObject = cylinder;
	}

	else if (objectType == "cube")
	{
		Point pos;
		double length;
		node["position"] >> pos;
		node["length"] >> length;
		Cube* cube = new Cube();
		cube->position = pos;
		cube->length = length;
		returnObject = cube;
	}

    if (returnObject) 
	{
		//Handle the rotation
		const YAML::Node* eulerAxisNode = node.FindValue("eulerAxis");
		if (eulerAxisNode)
		{
			Triple eulerAxis;
			node["eulerAxis"] >> eulerAxis;
			double eulerAngle;
			node["eulerAngle"] >> eulerAngle;
			returnObject->rotation = Quaternion(eulerAxis, eulerAngle);
		}

        // read the material and attach to object
        returnObject->material = parseMaterial(node["material"]);
    }

    return returnObject;
}

Light* Raytracer::parseLight(const YAML::Node& node)
{
    Point position;
    node["position"] >> position;
    Color color;
    node["color"] >> color;
    return new Light(position,color);
}

bool Raytracer::parseShadow(const YAML::Node& node) {
	return node[0];
}

renderMode Raytracer::parseRenderMode(const YAML::Node & node)
{
	std::string rm;
	node[0] >> rm;
	if (rm == "phong") {
		return PHONG;
	}
	else if (rm == "zbuffer") {
		return ZBUFFER;
	}
	else if (rm == "normal") {
		return NORMAL;
	}
	else {
		return PHONG; //default mode if incorrect
	}
}

Camera* Raytracer::parseCamera(const YAML::Node& node)
{
	Vector up;
	Point eye;
	Point center;
	int width;
	int height;
	double focalDistance;

	node["up"]            >> up;
	node["eye"]           >> eye;
	node["center"]        >> center;
	node["width"]         >> width;
	node["height"]        >> height;
	node["focalDistance"] >> focalDistance;

	return new Camera(eye, center, up, focalDistance, width, height);
}

/*
* Read a scene from file
*/

bool Raytracer::readScene(const std::string& inputFilename)
{
    // Initialize a new scene
    scene = new Scene();

    // Open file stream for reading and have the YAML module parse it
    std::ifstream fin(inputFilename.c_str());
    if (!fin) {
        cerr << "Error: unable to open " << inputFilename << " for reading." << endl;;
        return false;
    }
    try {
        YAML::Parser parser(fin);
        if (parser) {
            YAML::Node doc;
            parser.GetNextDocument(doc);

            // Read scene configuration options
            //scene->setEye(parseTriple(doc["Eye"]));

			//Read shadow mode
			scene->setDrawShadow(parseShadow(doc["Shadows"]));

			//Read scene Render Mode
			scene->setRenderMode(parseRenderMode(doc["RenderMode"]));

			scene->setMaxRecursionDepth(doc["MaxRecursionDepth"][0]);

			scene->setSupersample(doc["SuperSampling"][0]);

			scene->setCamera(parseCamera(doc["Camera"]));

            // Read and parse the scene objects
            const YAML::Node& sceneObjects = doc["Objects"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of objects." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneObjects.begin();it!=sceneObjects.end();++it) {
                Object *obj = parseObject(*it);
                // Only add object if it is recognized
                if (obj) {
                    scene->addObject(obj);
                } else {
                    cerr << "Warning: found object of unknown type, ignored." << endl;
                }
            }

            // Read and parse light definitions
            const YAML::Node& sceneLights = doc["Lights"];
            if (sceneObjects.GetType() != YAML::CT_SEQUENCE) {
                cerr << "Error: expected a sequence of lights." << endl;
                return false;
            }
            for(YAML::Iterator it=sceneLights.begin();it!=sceneLights.end();++it) {
                scene->addLight(parseLight(*it));
            }
        }
        if (parser) {
            cerr << "Warning: unexpected YAML document, ignored." << endl;
        }
    } catch(YAML::ParserException& e) {
        std::cerr << "Error at line " << e.mark.line + 1 << ", col " << e.mark.column + 1 << ": " << e.msg << std::endl;
        return false;
    }

    cout << "YAML parsing results: " << scene->getNumObjects() << " objects read." << endl;
    return true;
}

void Raytracer::renderToFile(const std::string& outputFilename)
{
    Image img(scene->getCamera()->getWidth(), scene->getCamera()->getHeight());
    cout << "Tracing..." << endl;
    scene->render(img);
    cout << "Writing image to " << outputFilename << "..." << endl;
    img.write_png(outputFilename.c_str());
    cout << "Done." << endl;
}
