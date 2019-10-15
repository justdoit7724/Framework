#include "ShadowMap.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "Object.h"
#include "Shader.h"
#include "Shape.h"
#include "Transform.h"

ShadowMap::ShadowMap(UINT width, UINT height)
	:width(width), height(height), depthSRV(nullptr), depthDSV(nullptr)
{
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = width;
	vp.Height = height;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	D3D11_TEXTURE2D_DESC tex_desc;
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1;
	tex_desc.ArraySize = 1;
	tex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	tex_desc.SampleDesc = { 1,0 };
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.MiscFlags = 0;
	ComPtr<ID3D11Texture2D> tex;
	r_assert(
		DX_Device->CreateTexture2D(&tex_desc, nullptr, tex.GetAddressOf())
	);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Flags = 0;
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(tex.Get(), &dsv_desc, depthDSV.GetAddressOf())
	);

	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(tex.Get(), &srv_desc, depthSRV.GetAddressOf())
	);

	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_BACK;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rs_desc.DepthBias = 0x1000;
	rs_desc.DepthBiasClamp = 1.0f;
	rs_desc.SlopeScaledDepthBias = 2.0f;
	
	rsState = new RasterizerState(&rs_desc);
	dsState = new DepthStencilState(nullptr);
	blendState = new BlendState(nullptr);
	mapVS = new VShader("ShadowMapVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	mapVS->AddCB(0, 1, sizeof(XMMATRIX));
}

ShadowMap::~ShadowMap()
{
}

ID3D11ShaderResourceView* ShadowMap::Depth()
{
	return depthSRV.Get();

}

void ShadowMap::Mapping(std::vector<Object*>& objs, const XMMATRIX& ptVPMat)
{
	DX_DContext->RSSetViewports(1, &vp);

	ID3D11RenderTargetView* nullRTV = nullptr;
	DX_DContext->OMSetRenderTargets(1, &nullRTV, depthDSV.Get());
	DX_DContext->ClearDepthStencilView(depthDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, NULL);

	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	DX_DContext->PSSetShader(nullptr, nullptr, 0);
	for (auto obj : objs)
	{
		rsState->Apply();
		dsState->Apply();
		blendState->Apply();

		mapVS->WriteCB(0,&(obj->transform->WorldMatrix() * ptVPMat));
		mapVS->Apply();
		obj->shape->Apply();
	}
}

//ShadowMap& ShadowMap::operator=(const ShadowMap& rhs)
//{
//	// TODO: insert return statement here
//}
