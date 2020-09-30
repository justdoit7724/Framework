#pragma once

#include "Singleton.h"

namespace DX {
	class TextureMgr : public Singleton<TextureMgr>
	{
	public:

		~TextureMgr();
		void Load(std::string key, std::string fileName);

		void LoadArray(std::string key, std::wstring folderName, std::vector<std::string> fileNames);
		void LoadCM(std::string key, const std::vector<std::string>& fileNames);
		ID3D11ShaderResourceView* Get(std::string key);
		ID3D11Texture2D* GetTexture(std::string key);


	private:

		std::unordered_map<std::string, ID3D11ShaderResourceView*> SRVs;
	};
}

