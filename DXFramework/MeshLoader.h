#pragma once

#include "DX_info.h"

class Mesh;

class MeshLoader
{
public:
	static void Load(std::vector<Mesh*>* storage, std::string filepath, std::string filename);
};

