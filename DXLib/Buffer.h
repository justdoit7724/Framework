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
}