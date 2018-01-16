#include "objectLoader.h"
#include <string>

ObjectLoader::ObjectLoader(const std::string& filePath, const Vector& defaultOrigin)
{
	m_defaultMaterial = new GeomMaterial();
	m_defaultMaterial->color = Color(0.8, 0.8, 0.8);
	m_defaultMaterial->kd = 0.8;
	m_defaultMaterial->ka = 0.2;
	m_defaultMaterial->ks = 0.4;
	m_defaultMaterial->n = 100;

	std::fstream file(filePath, std::fstream::in);

	//Information for the Transformation::defaultConf
	bool defaultPositionInit = false;

	//The vertex values for a specific OBJ
	std::vector<float> vertexPosition;
	std::vector<float> vertexNormal;
	std::vector<int>   vertexDrawOrder;
	std::vector<int>   vertexNormalOrder;

	//Associated an material (by its key str) to its number of vertices when it is being used
	std::map<std::string, int> materialSerie;

	int faceSerie = 0; //The serie of the material

	std::string currentMaterial; //the current material key

	bool currentMaterialInit = false;
	OBJDatas* currentDatas = NULL;
	bool parseObj = false;

	while (true)
	{
		std::string line;
		getline(file, line);

		if(!file.eof())
		{
			unsigned int loc;
			//Replace two spaces by one
			while ((loc = line.find("  ")) != std::string::npos) //Two spaces here
				line.replace(loc, 2, " "); //Single space in quotes

			//Comments
			if (std::regex_match(line, std::regex("^ *#.*$")))
			{
				continue;
			}
		}

		if(strstr(line.c_str(), "o ") == line.c_str())
		{
			currentDatas = new OBJDatas();
			m_objDatas.insert(std::pair<std::string, OBJDatas*>(split(line, ' ')[1], currentDatas));
			currentDatas->insertNewObj(new OBJWF());
			parseObj = false;
			currentMaterialInit = false;
		}

		//Init the material file
		else if (strstr(line.c_str(), "mtllib ") == line.c_str())
		{
			std::vector<std::string> pathSplited = split(filePath, '/');
			std::string path = "";
			for (unsigned int i = 0; i < pathSplited.size() - 1; i++)
				path += pathSplited[i];

			std::string mtlName = split(line, ' ')[1];
			path += mtlName;

			currentDatas->mtlWrapper = new MaterialWrapper(path);
			m_mtlWrapper.insert(std::pair<std::string, MaterialWrapper*>(mtlName, currentDatas->mtlWrapper));
		}

		else if (strstr(line.c_str(), "usemtl ") == line.c_str())
		{
			if (currentMaterialInit)
				parseObj = true;
			if (split(line, ' ')[1] == "Mat")
				currentDatas->lastObj->material = m_defaultMaterial;
			else
			{
				currentMaterialInit = true;
				currentMaterial = split(line, ' ')[1];
				currentDatas->lastObj->material = currentDatas->mtlWrapper->getMaterial(currentMaterial);
			}
		}

		else if (strstr(line.c_str(), "v ") == line.c_str())
		{
			std::vector<std::string> pointValues = split(line, ' ');
			for (unsigned int i = 1; i < pointValues.size(); i++)
				vertexPosition.push_back(atof(pointValues[i].c_str()));
		}

		else if (strstr(line.c_str(), "vn ") == line.c_str())
		{
			std::vector<std::string> pointValues = split(line, ' ');
			for (unsigned int i = 1; i < pointValues.size(); i++)
				vertexNormal.push_back(atof(pointValues[i].c_str()));
		}

		else if (strstr(line.c_str(), "f ") == line.c_str())
		{
			std::vector<std::string> faceValue = split(line, ' ');
			for (unsigned int i = 1; i < faceValue.size(); i++)
			{
				if (faceValue[i] != "")
				{
					std::vector<std::string> orderValue = split(faceValue[i], '/');
					vertexDrawOrder.push_back(atoi(orderValue[0].c_str()));
					vertexNormalOrder.push_back(atoi(orderValue[2].c_str()));
				}
			}
		}


		if (parseObj || file.eof())
		{
			if (currentDatas && currentDatas->lastObj)
			{
				int vertexPositionLength = COORDS_PER_VERTEX * vertexDrawOrder.size();
				int vertexNormalLength = COORDS_PER_VERTEX * vertexNormalOrder.size();

				//fill datas
				float* vertexPositionArray = (float*)malloc(sizeof(float) * vertexPositionLength);
				float* vertexNormalArray = (float*)malloc(sizeof(float) * vertexNormalLength);

				//Get the value of the position switch the order we get

				for (unsigned int i = 0; i < vertexDrawOrder.size(); i++)
				{
					//Store the vertex position get by the order in the vertexPositionArray
					int offset = (vertexDrawOrder[i] - 1) * COORDS_PER_VERTEX;
					for (uint32_t j = 0; j < COORDS_PER_VERTEX; j++)
						vertexPositionArray[COORDS_PER_VERTEX*i + j] = vertexPosition[offset + j];
				}

				for (unsigned int i = 0; i < vertexNormalOrder.size(); i++)
				{
					int offset = (vertexNormalOrder[i] - 1) * COORDS_PER_VERTEX;
					for (int j = 0; j < COORDS_PER_VERTEX; j++)
						vertexNormalArray[COORDS_PER_VERTEX*i + j] = vertexNormal[offset + j];
				}

				//Create the triangles
				OBJWF* obj = currentDatas->lastObj;
				obj->triangles = (Triangle*)malloc(sizeof(Triangle) * vertexPositionLength / 9);
				obj->nbTriangles = vertexPositionLength / 9;
				for (uint32_t i = 0; i < vertexPositionLength / 9; i++)
				{
					obj->triangles[i].p1 = Vector(vertexPositionArray[9 * i + 0], vertexPositionArray[9 * i + 1], vertexPositionArray[9 * i + 2]) + defaultOrigin;
					obj->triangles[i].p2 = Vector(vertexPositionArray[9 * i + 3], vertexPositionArray[9 * i + 4], vertexPositionArray[9 * i + 5]) + defaultOrigin;
					obj->triangles[i].p3 = Vector(vertexPositionArray[9 * i + 6], vertexPositionArray[9 * i + 7], vertexPositionArray[9 * i + 8]) + defaultOrigin;
				}
			}

			if (file.eof())
				break;

			//reinit the vertex
			{
				vertexDrawOrder = std::vector<int>();
				vertexNormalOrder = std::vector<int>();
				materialSerie = std::map<std::string, int>();
				parseObj = false;

				currentDatas->insertNewObj(new OBJWF());
			}
		}
	}

	file.close();
}

ObjectLoader::~ObjectLoader()
{
}
