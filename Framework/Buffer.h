#pragma once
#include "Resource.h"

class Buffer : public Resource 
{
public:
	Buffer(D3D11_BUFFER_DESC* desc, void * initValue);
	Buffer(UINT byteSize);
	
	void SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	void Write(const void* data);

	ID3D11Buffer* Get(){return resource.Get(); }
	ID3D11Buffer** GetAddress() {return resource.GetAddressOf(); }

	const D3D11_BUFFER_DESC desc;

private:


	ComPtr<ID3D11Buffer> resource=nullptr;
	ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
};