#pragma once
#include <unordered_map>
#include "Component.h"


class Buffer;

class Shader : public Component
{
public:
	~Shader();
	void AddCB(UINT slot, UINT arrayNum, UINT byteSize);
	void AddSRV(UINT slot, UINT arrayNum, ID3D11ShaderResourceView* srv);
	void AddSamp(UINT slot, UINT arrayNum, D3D11_SAMPLER_DESC* desc);
	void WriteCB(UINT slot, void* data);

protected:
	struct BindingCB 
	{
		Buffer* data;
		UINT arrayNum;
		BindingCB()
			:data(nullptr),arrayNum(0){}
		BindingCB(Buffer* data, UINT arrayNum)
			:data(data), arrayNum(arrayNum){}
	};
	struct BindingSRV
	{
		ID3D11ShaderResourceView* data;
		UINT arrayNum;
		BindingSRV() 
			:data(nullptr),arrayNum(0){}
		BindingSRV(ID3D11ShaderResourceView* data, UINT arrayNum)
			:data(data), arrayNum(arrayNum) {}
	};
	struct BindingSamp
	{
		ID3D11SamplerState* data;
		UINT arrayNum;
		BindingSamp()
			:data(nullptr), arrayNum(0){}
		BindingSamp(ID3D11SamplerState* data, UINT arrayNum)
			:data(data), arrayNum(arrayNum) {}
	};

	std::unordered_map<UINT, BindingCB> cbs;
	std::unordered_map<UINT, BindingSRV> srvs;
	std::unordered_map<UINT, BindingSamp> samps;
};

class VShader : public Shader
{
public:
	VShader(std::string fileName, const D3D11_INPUT_ELEMENT_DESC* layoutDesc, int layoutNum);

	void Apply()const override;

private:
	ComPtr<ID3D11InputLayout> iLayout = nullptr;
	ComPtr<ID3D11VertexShader> vs = nullptr;
};

class GShader : public Shader
{
public:
	GShader(std::string fileName="");

	void Apply()const override;

	const bool enabled;
private:
	ComPtr<ID3D11GeometryShader> gs = nullptr;
};

class HShader : public Shader
{
public:
	HShader(std::string fileName="");

	void Apply()const override;
private:
	ComPtr<ID3D11HullShader> hs = nullptr;
};
class DShader : public Shader
{
public:
	DShader(std::string fileName = "");

	void Apply()const override;
private:
	ComPtr<ID3D11DomainShader> ds = nullptr;
};


class PShader : public Shader
{
public:
	PShader(std::string fileName);

	void Apply()const override;

private:
	ComPtr<ID3D11PixelShader> ps = nullptr;
};


class CShader : public Shader
{
public:
	CShader(const std::string CSfileName);

	void Apply()const override;

private:
	ComPtr<ID3D11ComputeShader> cs = nullptr;
};
