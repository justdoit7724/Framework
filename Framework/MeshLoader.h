#pragma once

#include "DX_info.h"

class Object;

class MeshLoader
{
public:
	static void Load(std::vector<Object*>& storage, std::string filepath, std::string filename);
};

