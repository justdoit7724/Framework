#pragma once
#include "DX_info.h"
#include <unordered_map>
#include "Singleton.h"

class TextureMgr : public Singleton<TextureMgr>
{
public:
	
	~TextureMgr();
	void Load(std::string fileName, UINT miplevel, UINT spriteCount);
	ID3D11ShaderResourceView * Get(std::string fileName);
	ID3D11ShaderResourceView ** GetAddress(std::string fileName);
	void Get(std::string fileName, OUT ID3D11ShaderResourceView const** srv, OUT UINT* count);
	ID3D11Texture2D* GetTexture(std::string fileName);

private:
	struct SRV_Info
	{
		ID3D11ShaderResourceView* srv;
		UINT countX;

		SRV_Info(ID3D11ShaderResourceView* srv, UINT countX)
			:srv(srv), countX(countX){}
	};
	std::unordered_map<std::string, SRV_Info> SRVs;
};

