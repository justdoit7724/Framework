#pragma once
#include "Resource.h"

class Buffer : public Resource 
{
public:
	Buffer(D3D11_BUFFER_DESC* desc);
	Buffer(D3D11_BUFFER_DESC* desc, void* _initValue);
	//constant buffer
	Buffer(UINT byteSize);
	
	void SetSRV(D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
	void SetUAV(D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);
	ID3D11ShaderResourceView** SRV() {return srv.GetAddressOf(); }
	ID3D11UnorderedAccessView** UAV() {return uav.GetAddressOf(); }

	void Write(void* data);

	ID3D11Buffer* Get() { return resource.Get();}
	ID3D11Buffer*const* GetAddress(){ return resource.GetAddressOf(); }

	const bool isCB;
private:

	D3D11_BUFFER_DESC desc;

	ComPtr<ID3D11Buffer> resource;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
};