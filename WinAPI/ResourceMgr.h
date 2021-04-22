#pragma once
#include "Singleton.h"

#define SGL_ResMgr ResourceMgr::Instance()

class Mesh;
class Skeleton;
class Animation;
class Texture;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class Material;

class ResourceMgr
{
	SET_SINGLETON(ResourceMgr)

public:
	void Init();
	void Load(DX::Graphic* graphic);

	VShader* GetVShader(DX::Graphic* graphic, std::string key);
	HShader* GetHShader(DX::Graphic* graphic, std::string key);
	DShader* GetDShader(DX::Graphic* graphic, std::string key);
	GShader* GetGShader(DX::Graphic* graphic, std::string key);
	PShader* GetPShader(DX::Graphic* graphic, std::string key);
	Mesh* GetMesh(std::string key);
	Material* GetMaterial(DX::Graphic* graphic, std::string key);
	Texture* GetTexture(DX::Graphic* graphic, std::string key);

	BOOL AddMaterial(DX::Graphic* graphic, std::string key, Material* mat);

private:
	 BOOL LoadVShader(DX::Graphic* graphic, std::string folder);
	 BOOL LoadHShader(DX::Graphic* graphic, std::string folder);
	 BOOL LoadDShader(DX::Graphic* graphic, std::string folder);
	 BOOL LoadGShader(DX::Graphic* graphic, std::string folder);
	 BOOL LoadPShader(DX::Graphic* graphic, std::string folder);
	 BOOL LoadTex(DX::Graphic* graphic, std::string folder);
	 BOOL LoadModel(std::string folder);

	 void ReleaseModel();
	 void ReleaseShader();
	 void ReleaseTexture();
	 void ReleaseMaterial();

	std::unordered_map<int,std::unordered_map<std::string,VShader*>> m_VShader;
	std::unordered_map<int,std::unordered_map<std::string,HShader*>> m_HShader;
	std::unordered_map<int,std::unordered_map<std::string,DShader*>> m_DShader;
	std::unordered_map<int,std::unordered_map<std::string,GShader*>> m_GShader;
	std::unordered_map<int,std::unordered_map<std::string,PShader*>> m_PShader;
	std::unordered_map<int,std::unordered_map<std::string,Texture*>> m_tex;
	std::unordered_map<int, std::unordered_map<std::string, Material*>> m_mat;
	std::unordered_map<std::string, Mesh*> m_mesh;
	std::unordered_map<std::string, Skeleton*> m_skel;
	std::unordered_map<std::string, Animation*> m_anim;
};

