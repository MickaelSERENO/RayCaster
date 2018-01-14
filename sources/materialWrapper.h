#pragma once

#include "material.h"

#include <string>
#include <fstream>
#include <regex>
#include <map>

class MaterialWrapper
{
public:
	MaterialWrapper(const std::string& filePath);
	~MaterialWrapper();
	Material* getMaterial(const std::string& name) { return m_mtl[name]; }
private:
	std::map<std::string, Material*> m_mtl;
};

