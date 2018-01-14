#include "materialWrapper.h"

MaterialWrapper::MaterialWrapper(const std::string& filePath)
{
	std::fstream file(filePath, std::fstream::in);

	float ambientColor[3];
	float diffuseColor[3];
	float specularColor[3];
	std::string name;
	float transparent = 0.0f;
	float specularHighlight = 0.0f;
	bool initCurrentMaterial = false;

	while (1)
	{
		std::string line;
		getline(file, line);

		unsigned int loc;
		while ((loc = line.find("  ")) != std::string::npos)
			line.replace(loc, 2, " ");
		if(std::regex_match(line, std::regex("^ *#$")))
			continue;
	

		if (file.eof() || std::regex_match(line, std::regex("^o ")))
		{
			if (initCurrentMaterial)
			{
				ObjMaterial* objM = new ObjMaterial();
				objM->diffuse = Color(diffuseColor);
				objM->ambient = Color(ambientColor);
				objM->specular = Color(specularColor);

				m_mtl.insert(std::pair<std::string, Material*>(name, objM));
			}

			if(file.eof())
				break;

			name = split(line, ' ')[1];
			transparent = 0.0f;
			specularHighlight = 0.0f;
			initCurrentMaterial = true;
		}

		else if (std::regex_match(line, std::regex("^Ns ")))
		{
			std::vector<std::string> specularHighlightValue = split(line, ' ');
			specularHighlight = atof(specularHighlightValue[1].c_str());
		}

		else if (std::regex_match(line, std::regex("^Ka ")))
		{
			std::vector<std::string> ambientValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				ambientColor[i - 1] = atof(ambientValue[i].c_str());
		}

		else if (std::regex_match(line, std::regex("^Kd ")))
		{
			std::vector<std::string> diffuseValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				diffuseColor[i - 1] = atof(diffuseValue[i].c_str());
		}

		else if (std::regex_match(line, std::regex("^Ks ")))
		{
			std::vector<std::string> specularValue = split(line, ' ');
			for (int i = 1; i < 4; i++)
				specularColor[i - 1] = atof(specularValue[i].c_str());
		}

		else if (std::regex_match(line, std::regex("^d ")))
			transparent = atof(split(line, ' ')[1].c_str());
	}

	file.close();
}

MaterialWrapper::~MaterialWrapper()
{

}