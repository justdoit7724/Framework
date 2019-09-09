#include "Resource.h"

void Resource::VSUnbindSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->VSSetShaderResources(startIdx, numViews, &null);
}

void Resource::PSUnbindSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->PSSetShaderResources(startIdx, numViews, &null);
}

void Resource::CSUnbindSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->CSSetShaderResources(startIdx, numViews, &null);
}

void Resource::CSUnbindUAV(UINT startIdx, UINT numViews)
{
	OutputDebugString(std::string("\n consider pUAVInitialCounts in CSUnbindUAV() of Resource class\n").c_str());

	ID3D11UnorderedAccessView* null = nullptr;
	DX_DContext->CSSetUnorderedAccessViews(startIdx, numViews, &null, nullptr);
}
