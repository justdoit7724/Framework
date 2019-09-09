#pragma once
#include "Resource.h"

class Buffer : public Resource 
{
public:
	Buffer(const CD3D11_BUFFER_DESC* desc);
	
	Buffer(const CD3D11_BUFFER_DESC* desc, void* _initValue);
	
	void SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	void Update(void* data, UINT byteSize);

	ID3D11Buffer* Get(){return resource.Get(); }

	const D3D11_BUFFER_DESC desc;

private:


	ComPtr<ID3D11Buffer> resource;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
};