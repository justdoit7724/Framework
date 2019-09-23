#pragma once
#include "DX_info.h"
#include <unordered_map>
#include "Singleton.h"

class TextureMgr : public Singleton<TextureMgr>
{
public:
	
	~TextureMgr();
	void Load(std::string key, std::vector<std::string> fileNames, UINT miplevel);
	void Get(std::string key, ID3D11ShaderResourceView** srv, UINT* size);
	ID3D11Texture2D* GetTexture(std::string fileName);

private:
	struct TextureInfo
	{
		ID3D11ShaderResourceView* srv;
		UINT size;
		TextureInfo() {}
		TextureInfo(ID3D11ShaderResourceView* srv, UINT size)
			:srv(srv), size(size){}
	};
	std::unordered_map<std::string, TextureInfo> SRVs;
};

