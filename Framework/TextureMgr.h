#pragma once
#include "DX_info.h"
#include <unordered_map>
#include "Singleton.h"

#define KEY_TEXTURE_NORMAL_DEFAULT "default_normal"
#define KEY_TEXTURE_DIFFUSE_DEFAULT "green_light"
#define FN_TEXTURE_NORMAL_DEFAULT "Data\\Texture\\default_normal.png"
#define FN_TEXTURE_DIFFUSE_DEFAULT "Data\\Texture\\green_light.png"

class TextureMgr : public Singleton<TextureMgr>
{
public:
	
	~TextureMgr();
	void Load(std::string key, std::string fileName);

	void LoadArray(std::string key, std::wstring folderName, std::vector<std::string> fileNames);
	void LoadCM(std::string key, const std::vector<std::string>& fileNames);
	ID3D11ShaderResourceView* Get(std::string key);
	//ID3D11Texture2D* GetTexture(std::string fileName);


private:

	std::unordered_map<std::string, ID3D11ShaderResourceView*> SRVs;
};

