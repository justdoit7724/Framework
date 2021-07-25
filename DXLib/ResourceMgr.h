#pragma once

#define BONE_MAX 128

class VShader;
class PShader;
class VertexBuffer;
class IndexBuffer;

class ResourceMgr
{

public:
	ResourceMgr(ID3D11Device* device, ID3D11DeviceContext* dContext);
	~ResourceMgr();
	
	//TEXTURE
public:
	BOOL LoadTexture(std::string fileName);
	BOOL LoadTextureArray(std::wstring folderName, std::vector<std::string> fileNames, std::string name);
	BOOL LoadTextureCM(const std::vector<std::string>& fileNames, std::string key);
	BOOL GetTexture(std::string name, ID3D11ShaderResourceView** srv);
	BOOL HasTexture(std::string name);
private:
	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_mSRV;

	//VERTEX SHADER
public:
	BOOL LoadVShader(std::string csoPath);
	BOOL GetVShader(std::string title, VShader** shader);
	BOOL HasVShader(std::string key);
private:
	std::unordered_map<std::string, VShader*> m_mVShader;

	//HULL SHADER
	//DOMAIN SHADER
	//GEOMETRY SHADER
	//PIXEL SHADER
public:
	BOOL LoadPShader(std::string csoPath);
	BOOL GetPShader(std::string title, PShader** shader);
	BOOL HasPShader(std::string key);
private:
	std::unordered_map<std::string, PShader*> m_mPShader;

	//MESH
public:
	BOOL LoadMesh(const DX::Vertex* vertice, int verticeCount, const UINT* indice, int indiceCount, std::string key);
	BOOL GetMesh(std::string title, VertexBuffer** vb, IndexBuffer** ib);
	BOOL HasMesh(std::string key);
private:
	struct MeshInfo
	{
		VertexBuffer* vb;
		IndexBuffer* ib;
	};
	std::unordered_map<std::string, MeshInfo> m_mMeshBuffer;

private:
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_dContext;

};


