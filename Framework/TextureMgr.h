#pragma once
#include "DX_info.h"
#include <unordered_map>
#include "Singleton.h"

class TextureMgr : public Singleton<TextureMgr>
{
public:
	
	~TextureMgr();
	void Load(std::string key, std::vector<std::string> fileNames, UINT miplevel);
	ID3D11ShaderResourceView* Get(std::string key);
	ID3D11Texture2D* GetTexture(std::string fileName);

private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> SRVs;
};

