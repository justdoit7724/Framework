#include "DepthComplexity.h"


DepthComplexity::DepthComplexity(UICanvas* canvas, XMFLOAT2 pivot, float width, float height)
{

	/*
	D3D11_TEXTURE2D_DESC desc;
	desc.ArraySize = 1;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	desc.Width = SCREEN_WIDTH;
	desc.Height = SCREEN_HEIGHT;
	desc.MipLevels = 1;
	desc.MiscFlags = 0;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;



	ID3D11Texture2D* copyTex;
	
	DX_DContext->CopyResource(copyTex, graphic->DepthStencilBuffer());
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_X24_TYPELESS_G8_UINT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	r_assert(DX_Device->CreateShaderResourceView(copyTex->Get(), &srvDesc, &depthComplexSrv));

	canvas->AddDC("DC", XMFLOAT2(0, 0), 300, 300, 0, depthComplexSrv);
*/

}
