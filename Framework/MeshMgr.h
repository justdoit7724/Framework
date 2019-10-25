#pragma once
#include <string>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <unordered_map>

class Object;

class MeshLoader
{
public:
	static std::vector<Object*> Load(std::string filepath, std::string filename);
	
};

