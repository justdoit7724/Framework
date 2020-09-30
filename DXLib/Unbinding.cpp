
#include "pch.h"

#include "Unbinding.h"

using namespace DX;

void Unbinding::VSSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->VSSetShaderResources(startIdx, numViews, &null);
}

void Unbinding::PSSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->PSSetShaderResources(startIdx, numViews, &null);
}

void Unbinding::CSSRV(UINT startIdx, UINT numViews)
{
	ID3D11ShaderResourceView* null = nullptr;
	DX_DContext->CSSetShaderResources(startIdx, numViews, &null);
}

void Unbinding::CSUAV(UINT startIdx, UINT numViews)
{
	ID3D11UnorderedAccessView* null = nullptr;
	DX_DContext->CSSetUnorderedAccessViews(startIdx, numViews, &null, nullptr);
}
