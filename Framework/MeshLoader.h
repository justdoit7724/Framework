#pragma once

#include "DX_info.h"

class Object;
class Shape;

class MeshLoader
{
public:
	static void Load(std::vector<Object*>& storage, std::string filepath, std::string filename);
	static void LoadToken(Shape** storage, std::string path);
};

