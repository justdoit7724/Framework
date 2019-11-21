#include "ShadowMap.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "Object.h"
#include "Shader.h"
#include "Scene.h"
#include "Shape.h"
#include "Transform.h"
#include "ShaderFormat.h"
#include "ShaderReg.h"
#include "Light.h"
#include "Camera.h"
#include "Debugging.h"
#include "Buffer.h"

ShadowMap::ShadowMap(UINT resX, UINT resY, UINT width, UINT height)
	:depthSRV(nullptr), depthDSV(nullptr)
{
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.Width = resX;
	vp.Height = resY;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	D3D11_TEXTURE2D_DESC tex_desc;
	tex_desc.Width = resX;
	tex_desc.Height = resY;
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

	D3D11_SAMPLER_DESC samp_desc;
	ZeroMemory(&samp_desc, sizeof(D3D11_SAMPLER_DESC));
	samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samp_desc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_POINT;
	samp_desc.ComparisonFunc = D3D11_COMPARISON_LESS_EQUAL;
	samp_desc.MinLOD = 0;
	samp_desc.MaxLOD = D3D11_FLOAT32_MAX;
	r_assert(
		DX_Device->CreateSamplerState(&samp_desc, samp.GetAddressOf())
	);

	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_BACK;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rs_desc.DepthBias = 0x0008ff;
	rs_desc.DepthBiasClamp = 0.0f;
	rs_desc.SlopeScaledDepthBias = 0.0f;
	rsState = new RasterizerState(&rs_desc);
	dsState = new DepthStencilState(nullptr);
	blendState = new BlendState(nullptr);
	mapVS = new VShader("ShadowMapVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	mapVS->AddCB(0, 1, sizeof(XMMATRIX));

	view = new Camera(FRAME_KIND_ORTHOGONAL, width, height, 0.1f, 500.0f, XM_PIDIV2, 1);
	cbVPMat = new Buffer(sizeof(XMMATRIX));
}

ShadowMap::~ShadowMap()
{
}

ID3D11ShaderResourceView* ShadowMap::Depth()
{
	return depthSRV.Get();

}

void ShadowMap::Mapping(Scene* depthScene, const DirectionalLight* light)
{
	UINT noUs=1;
	D3D11_VIEWPORT oriVP;
	DX_DContext->RSGetViewports(&noUs, &oriVP);
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);

	DX_DContext->RSSetViewports(1, &vp);

	ID3D11ShaderResourceView* nullSRV=nullptr;
	DX_DContext->PSSetShaderResources(SHADER_REG_PS_SRV_SHADOW, 1, &nullSRV);
	ID3D11RenderTargetView* nullRTV = nullptr;
	DX_DContext->OMSetRenderTargets(1, &nullRTV, depthDSV.Get());
	DX_DContext->ClearDepthStencilView(depthDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, NULL);

	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	DX_DContext->PSSetShader(nullptr, nullptr, 0);

	XMFLOAT3 lightDir = light->GetDir();
	view->transform->SetRot(lightDir);
	XMFLOAT3 lightPos = -light->GetDir() * 200;
	view->transform->SetTranslation(lightPos);
	Debugging::Instance()->Mark(lightPos, 2);
	Debugging::Instance()->DirLine(lightPos, lightDir, 50);
	view->Update();
	XMMATRIX lightVP = view->VMat() * view->StdProjMat();
	int i = 0;
	const Object* curObj = depthScene->GetObj(i);
	while(curObj)
	{
		//every obj is in frustum in this project
		//if (!obj->IsInsideFrustum()) continue;

		rsState->Apply();
		dsState->Apply();
		blendState->Apply();

		mapVS->WriteCB(0,&(curObj->transform->WorldMatrix() * lightVP));
		mapVS->Apply();
		curObj->RenderGeom();

		curObj = depthScene->GetObj(++i);
	}

	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);

	const XMMATRIX uvMat = XMMATRIX(
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1);
	cbVPMat->Write(&(lightVP * uvMat));
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_CB_LIGHTVP, 1, cbVPMat->GetAddress());
	DX_DContext->PSSetShaderResources(SHADER_REG_PS_SRV_SHADOW, 1, depthSRV.GetAddressOf());
	DX_DContext->PSSetSamplers(SHADER_REG_PS_SAMP_SHADOW, 1, samp.GetAddressOf());

}
