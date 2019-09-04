#pragma once
#include "DX_info.h"
#include <unordered_map>
#include "Singleton.h"

class IGraphic;

class TextureMgr : public Singleton<TextureMgr>
{
public:
	

	~TextureMgr();
	void Load(IGraphic* graphic, std::string fileName);
	void Load(IGraphic* graphic, std::string fileName, UINT spriteX);
	ID3D11ShaderResourceView* Get(std::string fileName);
	void Get(std::string fileName, OUT ID3D11ShaderResourceView** srv, OUT UINT* count);
	ID3D11Texture2D* GetTexture(std::string fileName);

private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> SRVs;

	struct AnimSRV
	{
		ID3D11ShaderResourceView* srv;
		UINT countX;

		AnimSRV(ID3D11ShaderResourceView* srv, UINT countX)
			:srv(srv), countX(countX){}
	};
	std::unordered_map<std::string, AnimSRV> animSRVs;
};

