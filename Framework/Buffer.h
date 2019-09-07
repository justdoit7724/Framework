#pragma once
#include "DX_info.h"

template<typename T>
class Buffer {
public:
	Buffer(ID3D11Device* device, const CD3D11_BUFFER_DESC desc)
	{
		resourceDesc = desc;

		r_assert(
			device->CreateBuffer(
				&desc,
				nullptr,
				resource.GetAddressOf())
		);
	}
	Buffer(ID3D11Device* device, const CD3D11_BUFFER_DESC desc, const T _initValue)
	{
		resourceDesc = desc;

		int size = desc.ByteWidth / desc.StructureByteStride;
		T* initValues = new T[size];
		for (int i = 0; i < size; ++i) {
			initValues[i] = _initValue;
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initValues;

		r_assert(
			device->CreateBuffer(
				&desc,
				data,
				resource.GetAddressOf())
		);
	}

	ID3D11ShaderResourceView*const* SRV(ID3D11Device* device) {

		if (srv == nullptr)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;

			desc.Format = DXGI_FORMAT_UNKNOWN;
			desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
			desc.Buffer.ElementOffset = 0;
			desc.Buffer.ElementWidth = resourceDesc.ByteWidth / resourceDesc.StructureByteStride;

			r_assert(
				device->CreateShaderResourceView(
					resource.Get(),
					&desc,
					srv.GetAddressOf())
			);
		}

		return srv.GetAddressOf();
	}
	void Update(ID3D11DeviceContext* dContext, T* data) {

		D3D11_MAPPED_SUBRESOURCE mappedData;

		dContext->Map(resource.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		CopyMemory(mappedData.pData, data, resourceDesc.ByteWidth);
		dContext->Unmap(resource.Get(), 0);
	}

private:

	D3D11_BUFFER_DESC resourceDesc;

	ComPtr<ID3D11Buffer> resource;
	ComPtr<ID3D11ShaderResourceView> srv;
	ComPtr<ID3D11UnorderedAccessView> uav;
};