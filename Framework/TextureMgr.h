#pragma once
#include "DX_info.h"
#include <map>
#include "Singleton.h"

#define KEY_TEXTURE_NORMAL_DEFAULT "blue"
#define FN_TEXTURE_NORMAL_DEFAULT "blue.png"

class TextureMgr : public Singleton<TextureMgr>
{
public:
	
	~TextureMgr();
	void Load(std::string key, std::string fileName, UINT miplevel);
	void Load(std::string key, std::vector<std::string> fileNames, UINT miplevel);
	void LoadCM(std::string key, std::vector<std::string> fileNames);
	ID3D11Texture2D* GetTexture(std::string fileName);

	void Apply();

private:

	std::map<UINT, std::string> stdTextures;
	std::map<UINT, std::string> normalTextures;
};

