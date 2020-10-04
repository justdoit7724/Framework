#pragma once

namespace DX
{
	class DXLIB_DLL Buffer
	{
	public:
		Buffer(ID3D11Device* device, D3D11_BUFFER_DESC* desc, void* initValue);
		Buffer(ID3D11Device* device, UINT byteSize);
		~Buffer();

		void SetSRV(ID3D11Device* device, D3D11_SHADER_RESOURCE_VIEW_DESC* srvDesc);
		void SetUAV(ID3D11Device* device, D3D11_UNORDERED_ACCESS_VIEW_DESC* uavDesc);

		void Write(ID3D11DeviceContext* dContext, const void* data);

		ID3D11Buffer* Get() { return resource; }
		ID3D11Buffer** GetAddress() { return &resource; }

		const D3D11_BUFFER_DESC desc;

	private:


		ID3D11Buffer* resource = nullptr;
		ID3D11ShaderResourceView* srv = nullptr;
		ID3D11UnorderedAccessView* uav = nullptr;
	};
}