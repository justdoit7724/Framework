#include "Resource.h"

void Resource::VSUnbindSRV(ID3D11DeviceContext * dContext, UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	dContext->VSSetShaderResources(startIdx, numViews, &null);
}

void Resource::PSUnbindSRV(ID3D11DeviceContext * dContext, UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	dContext->PSSetShaderResources(startIdx, numViews, &null);
}

void Resource::CSUnbindSRV(ID3D11DeviceContext * dContext, UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	dContext->CSSetShaderResources(startIdx, numViews, &null);
}

void Resource::CSUnbindUAV(ID3D11DeviceContext * dContext, UINT startIdx, UINT numViews)
{
	OutputDebugString(std::string("\n consider pUAVInitialCounts in CSUnbindUAV() of Resource class\n").c_str());

	ID3D11UnorderedAccessView* null = nullptr;
	dContext->CSSetUnorderedAccessViews(startIdx, numViews, &null, nullptr);
}
