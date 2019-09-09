#pragma once
#include "Resource.h"

class Buffer : public Resource 
{
public:
	Buffer(ID3D11Device* device, const CD3D11_BUFFER_DESC* desc);
	
	Buffer(ID3D11Device* device, const CD3D11_BUFFER_DESC* desc, void* _initValue);
	
	void SetSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

	void Update(ID3D11DeviceContext* dContext, void* data, UINT byteSize);

	ID3D11Buffer* Get(){return resource.Get(); }

	const D3D11_BUFFER_DESC desc;

private:


	ComPtr<ID3D11Buffer> resource;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
};