#pragma once
#include "DX_info.h"

class Resource
{
public:
	static void VSUnbindSRV(UINT startIdx, UINT numViews);
	static void PSUnbindSRV(UINT startIdx, UINT numViews);
	static void CSUnbindSRV(UINT startIdx, UINT numViews);
	static void CSUnbindUAV(UINT startIdx, UINT numViews);

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
