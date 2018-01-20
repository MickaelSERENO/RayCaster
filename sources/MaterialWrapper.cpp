#include "materialWrapper.h"

MaterialWrapper::MaterialWrapper(const std::string& filePath)
{
	std::fstream file(filePath, std::fstream::in);

	float ambientColor[3];
	float diffuseColor[3];
	float specularColor[3];
	std::string name;
	ObjMaterial* currentMtl;
	float transparent = 0.0f;
	float specularHighlight = 0.0f;

	while(1)
	{
		std::string line;
		getline(file, line);

		int loc = line.find("  ");
		while(loc != std::string::npos)
		{
			line.replace(loc, 2, " "); //Single space in quotes
			loc = line.find("  ");
		}

		if (strstr(line.c_str(), "Ns ") == line.c_str())
		{
			std::vector<std::string> specularHighlightValue = split(line, ' ');
			specularHighlight = atof(specularHighlightValue[1].c_str());
			currentMtl->n = specularHighlight;
		}

		else if (strstr(line.c_str(), "Ka ") == line.c_str())
		{
			std::vector<std::string> ambientValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				ambientColor[i - 1] = atof(ambientValue[i].c_str());
			currentMtl->ambient = Color(ambientColor);
		}

		else if (strstr(line.c_str(), "Kd ") == line.c_str())
		{
			std::vector<std::string> diffuseValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				diffuseColor[i - 1] = atof(diffuseValue[i].c_str());
			currentMtl->diffuse = Color(diffuseColor);
		}

		else if (strstr(line.c_str(), "Ks ") == line.c_str())
		{
			std::vector<std::string> specularValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				specularColor[i - 1] = atof(specularValue[i].c_str());
			currentMtl->specular = Color(specularColor);
		}

		else if (strstr(line.c_str(), "d ") == line.c_str())
			transparent = atof(split(line, ' ')[1].c_str());

		else if(strstr(line.c_str(), "newmtl") == line.c_str())
		{
			name = split(line, ' ')[1];
			ObjMaterial* objM = new ObjMaterial();
			objM->kd = objM->ka = objM->ks = 1.0;
			currentMtl = objM;
			m_mtl.insert(std::pair<std::string, Material*>(name, objM));
			transparent = 0.0f;
			specularHighlight = 0.0f;
		}

		if(file.eof())
			break;
	}

	file.close();
}

MaterialWrapper::~MaterialWrapper()
{

}
