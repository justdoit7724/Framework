#pragma once
#include "DX_info.h"

template<typename T>
class Texture2D {
public:
	Texture2D(ID3D11Device* device, const CD3D11_TEXTURE2D_DESC desc)
		:resourceDesc(desc)
	{
		r_assert(
			device->CreateTexture2D(
				&desc,
				nullptr,
				resource.GetAddressOf())
		);
	}
	Texture2D(ID3D11Device* device, const CD3D11_TEXTURE2D_DESC desc, const T _initValue)
		:resourceDesc(desc)
	{
		int size = desc.Width * desc.Height;
		T* initValues = new T[size];
		ZeroMemory(initValues, sizeof(T)*size);
		for (int i = 0; i < size; ++i) {
			initValues[i] = _initValue;
		}

		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = initValues;
		data.SysMemPitch = sizeof(T)*desc.Width;

		r_assert(
			device->CreateTexture2D(
				&desc,
				&data,
				resource.GetAddressOf())
		);
	}

	ID3D11ShaderResourceView*const* SRV(ID3D11Device* device) {

		if (srv == nullptr)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC desc;
			desc.Format = resourceDesc.Format;
			desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipLevels = 1;
			desc.Texture2D.MostDetailedMip = 0;

			r_assert(
				device->CreateShaderResourceView(
					resource.Get(),
					&desc,
					srv.GetAddressOf())
			);
		}

		return srv.GetAddressOf();
	}
	ID3D11UnorderedAccessView*const* UAV(ID3D11Device* device)
	{
		if(uav==nullptr)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC desc;
			desc.Format = resourceDesc.Format;
			desc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
			desc.Texture2D.MipSlice = 0;

			r_assert(
				device->CreateUnorderedAccessView(
					resource.Get(),
					&desc,
					uav.GetAddressOf())
			);
		}

		return uav.GetAddressOf();
	}

	ID3D11Texture2D* Get () {return resource.Get();}

	ID3D11ShaderResourceView*const* NullSRV() {return &nullSRV;}
	ID3D11UnorderedAccessView*const* NullUAV() {return &nullUAV;}

	const CD3D11_TEXTURE2D_DESC resourceDesc;
private:
	ComPtr<ID3D11Texture2D> resource;
	ComPtr<ID3D11ShaderResourceView> srv = nullptr;
	ID3D11ShaderResourceView* nullSRV = nullptr;
	ComPtr<ID3D11UnorderedAccessView> uav = nullptr;
	ID3D11UnorderedAccessView* nullUAV = nullptr;

};

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
			desc.Buffer.ElementWidth = resourceDesc.ByteWidth/ resourceDesc.StructureByteStride;

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

// coherent code -> constant buffer
// incoherent code -> structed buffer
template<class T>
class ConstantBuffer
{
public:
	ConstantBuffer(ID3D11Device* device) {

		D3D11_BUFFER_DESC desc;
		desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE_DYNAMIC;
		desc.ByteWidth = Size();
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.StructureByteStride = 0;
		desc.MiscFlags = 0;

		r_assert(
			device->CreateBuffer(
				&desc,
				nullptr,
				buffer.GetAddressOf())
		);
	}

	void VSSetData(ID3D11DeviceContext* dContext, const T* data, int startSlot=0){

		D3D11_MAPPED_SUBRESOURCE mappedData;

		dContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		CopyMemory(mappedData.pData, data, Size());
		dContext->Unmap(buffer.Get(), 0);

		dContext->VSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
	}
	void PSSetData(ID3D11DeviceContext* dContext, const T* data, int startSlot=0){

		D3D11_MAPPED_SUBRESOURCE mappedData;

		dContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		CopyMemory(mappedData.pData, data, Size());
		dContext->Unmap(buffer.Get(), 0);

		dContext->PSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
	}
	void CSSetData(ID3D11DeviceContext* dContext, const T* data, UINT startSlot=0){

		D3D11_MAPPED_SUBRESOURCE mappedData;

		dContext->Map(buffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData);
		CopyMemory(mappedData.pData, data, Size());
		dContext->Unmap(buffer.Get(), 0);

		dContext->CSSetConstantBuffers(startSlot, 1, buffer.GetAddressOf());
	}

private:

	UINT Size() {
		return (15 + sizeof(T) - (sizeof(T) - 1) % 16);
	}

	ComPtr<ID3D11Buffer> buffer;
};
