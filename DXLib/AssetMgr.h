#pragma once
#include "Singleton.h"
#include <string>
#include <unordered_map>

#define SGL_AssetMgr AssetMgr::Instance()

class Model;
class Texture;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class Material;

class AssetMgr
{
	SET_SINGLETON(AssetMgr)

public:
	void Init();
	void Load();

	VShader* GetVShader(std::string key);
	HShader* GetHShader(std::string key);
	DShader* GetDShader(std::string key);
	GShader* GetGShader(std::string key);
	PShader* GetPShader(std::string key);
	Model* GetModel(std::string key);
	Material* GetMaterial(std::string key);
	Texture* GetTexture(std::string key);

	BOOL LoadModel(std::string folder);

private:
	void Load(std::string subPath);

	 /*BOOL LoadVShader(std::string path);
	 BOOL LoadHShader(std::string path);
	 BOOL LoadDShader(std::string path);
	 BOOL LoadGShader(std::string path);
	 BOOL LoadPShader(std::string path);
	 BOOL LoadTex(std::string path);
	 BOOL LoadModel(std::string path);
	 BOOL LoadMaterial(std::string path);*/

	void ReleaseModel();
	 void ReleaseShader();
	 void ReleaseTexture();
	 void ReleaseMaterial();

	std::unordered_map<std::string,VShader*> m_VShader;
	std::unordered_map<std::string,HShader*> m_HShader;
	std::unordered_map<std::string,DShader*> m_DShader;
	std::unordered_map<std::string,GShader*> m_GShader;
	std::unordered_map<std::string,PShader*> m_PShader;
	std::unordered_map<std::string,Texture*> m_tex;
	std::unordered_map<std::string, Material*> m_mat;
	std::unordered_map<std::string, Model*> m_model;
};

