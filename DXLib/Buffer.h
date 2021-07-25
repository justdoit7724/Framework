#pragma once

class Buffer
{
public:
	Buffer(ID3D11Device* device, D3D11_BUFFER_DESC desc, const void* initValue);
	virtual ~Buffer();

	void SetSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	void Write(ID3D11DeviceContext* dContext, const void* data);

	ID3D11Buffer* Get() { return m_resource; }
	ID3D11Buffer** GetAddress() { return &m_resource; }
	void GetSRV(ID3D11ShaderResourceView** srv);
	void GetUAV(ID3D11UnorderedAccessView** uav);

	const D3D11_BUFFER_DESC desc;

private:


	ID3D11Buffer* m_resource = nullptr;
	ID3D11ShaderResourceView* m_srv = nullptr;
	ID3D11UnorderedAccessView* m_uav = nullptr;
};

class ConstantBuffer : public Buffer
{
public:
	ConstantBuffer(ID3D11Device* device, int byteSize);
};

class VertexBuffer : public Buffer
{
public:
	VertexBuffer(ID3D11Device* device, const DX::Vertex* vertice, int verticeCount);

	BOOL Apply(ID3D11DeviceContext* dContext);

private:
	int m_verticeCount;
};

class IndexBuffer : public Buffer
{
public:
	IndexBuffer(ID3D11Device* device, const UINT* indice, int indexCount);

	BOOL Apply(ID3D11DeviceContext* dContext);
private:
	int m_indiceCount;
};
