#pragma once

class Shader
{
public:
	virtual BOOL Apply(ID3D11DeviceContext* dContext)const =0;
};

class VShader : public Shader
{
public:
	VShader();
	~VShader();

	BOOL Modify(ID3D11Device* device, std::string fileName);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;

private:
	ID3D11InputLayout* iLayout = nullptr;
	ID3D11VertexShader* vs = nullptr;
};

class GShader : public Shader
{
public:
	GShader();
	~GShader();

	BOOL Modify(ID3D11Device* device, std::string csoPath);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;
private:
	ID3D11GeometryShader* gs = nullptr;
};

class HShader : public Shader
{
public:
	HShader();
	~HShader();

	BOOL Modify(ID3D11Device* device, std::string csoPath);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;
private:
	ID3D11HullShader* hs = nullptr;
};
class DShader : public Shader
{
public:
	DShader();
	~DShader();

	BOOL Modify(ID3D11Device* device, std::string csoPath);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;
private:
	ID3D11DomainShader* ds = nullptr;
};


class PShader : public Shader
{
public:
	PShader();
	~PShader();

	BOOL Modify(ID3D11Device* device, std::string csoPath);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;

private:
	ID3D11PixelShader* ps = nullptr;
};


class CShader : public Shader
{
public:
	CShader();
	~CShader();

	BOOL Modify(ID3D11Device* device, std::string csoPath);
	BOOL Apply(ID3D11DeviceContext* dContext)const override;

private:
	ID3D11ComputeShader* cs = nullptr;
};

