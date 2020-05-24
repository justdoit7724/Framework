#pragma once
#include "DX_info.h"

class Buffer
{
public:
	Buffer(D3D11_BUFFER_DESC* desc, void * initValue);
	Buffer(UINT byteSize);
	~Buffer();
	
	void SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	void Write(const void* data);

	ID3D11Buffer* Get(){return resource; }
	ID3D11Buffer** GetAddress() {return &resource; }

	const D3D11_BUFFER_DESC desc;

private:


	ID3D11Buffer* resource=nullptr;
	ID3D11ShaderResourceView* srv = nullptr;
	ID3D11UnorderedAccessView* uav = nullptr;
};