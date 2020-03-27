#include "stdafx.h"
#include "ShadowMap.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "Object.h"
#include "Shader.h"
#include "Shape.h"
#include "Transform.h"
#include "ShaderFormat.h"
#include "ShaderReg.h"
#include "Light.h"
#include "Shape.h"
#include "Camera.h"
#include "Buffer.h"
#include "TextureMgr.h"

OpaqueShadowMap::OpaqueShadowMap(UINT resX, UINT resY, UINT width, UINT height, const std::vector<Object const*>& objs)
	:depthSRV(nullptr), depthDSV(nullptr)
{
	drawObjs[0] = objs[0];
	drawObjs[1] = objs[1];
	drawObjs[2] = objs[2];

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

	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_BACK;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rs_desc.DepthBias = 0x008fff;
	rs_desc.DepthBiasClamp = 1.0f;
	rs_desc.SlopeScaledDepthBias = 1.0f;
	rsState = new RasterizerState(&rs_desc);
	dsState = new DepthStencilState(nullptr);
	blendState = new BlendState(nullptr);
	mapVS = new VShader("ShadowVS.cso", Adv_ILayouts, ARRAYSIZE(Adv_ILayouts));
	mapVS->AddCB(0, 1, sizeof(XMMATRIX));

	view = new Camera(FRAME_KIND_ORTHOGONAL, width, height, 0.1f, 400.0f, XM_PIDIV2, 1, true);
	cbVPMat = new Buffer(sizeof(XMMATRIX));
}

OpaqueShadowMap::~OpaqueShadowMap()
{
}

ID3D11ShaderResourceView* OpaqueShadowMap::Depth()
{
	return depthSRV.Get();

}

void OpaqueShadowMap::Mapping(const DirectionalLight* light)
{
	UINT noUs=1;
	D3D11_VIEWPORT oriVP;
	DX_DContext->RSGetViewports(&noUs, &oriVP);
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);
	ID3D11RasterizerState* oriRS;
	DX_DContext->RSGetState(&oriRS);

	DX_DContext->RSSetViewports(1, &vp);

	ID3D11ShaderResourceView* nullSRV=nullptr;
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_SHADOW, 1, &nullSRV);
	ID3D11RenderTargetView* nullRTV = nullptr;
	DX_DContext->OMSetRenderTargets(1, &nullRTV, depthDSV.Get());
	DX_DContext->ClearDepthStencilView(depthDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, NULL);

	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	DX_DContext->PSSetShader(nullptr, nullptr, 0);

	XMFLOAT3 lightDir = light->GetDir();
	view->transform->SetRot(lightDir);
	view->transform->SetTranslation(-lightDir * 200);
	view->Update();
	XMMATRIX lightVP = view->VMat() * view->ProjMat();
	
	for(auto t : drawObjs)
	{
		rsState->Apply();
		dsState->Apply();
		blendState->Apply();

		mapVS->WriteCB(0, &(t->transform->WorldMatrix() * lightVP));
		mapVS->Apply();

		t->RenderGeom();
	}

	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
	DX_DContext->RSSetState(oriRS);

	const XMMATRIX uvMat = XMMATRIX(
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1);
	cbVPMat->Write(&(lightVP * uvMat));
	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_LIGHTVP, 1, cbVPMat->GetAddress());
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_SHADOW, 1, depthSRV.GetAddressOf());

}

TranspShadowMap::TranspShadowMap(XMUINT2 res, XMUINT2 volume, const std::vector<Object const*>& objs)
{
	drawObjs[0] = objs[0];
	drawObjs[1] = objs[1];
	drawObjs[2] = objs[2];

	vp.Width = res.x;
	vp.Height = res.y;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	D3D11_TEXTURE2D_DESC tex_desc;
	tex_desc.ArraySize = 1;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	tex_desc.CPUAccessFlags = 0;
	tex_desc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	tex_desc.Width = res.x;
	tex_desc.Height = res.y;
	tex_desc.MipLevels = 1;
	tex_desc.MiscFlags = 0;
	tex_desc.SampleDesc = { 1,0 };
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> tex;
	r_assert(
		DX_Device->CreateTexture2D(&tex_desc, nullptr, tex.GetAddressOf())
	);
	D3D11_RENDER_TARGET_VIEW_DESC rtv_desc;
	rtv_desc.Format = tex_desc.Format;
	rtv_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(tex.Get(), &rtv_desc, rtv.GetAddressOf())
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc;
	srv_desc.Format = tex_desc.Format;
	srv_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv_desc.Texture2D.MipLevels = 1;
	srv_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(tex.Get(), &srv_desc, srv.GetAddressOf())
	);
	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.ArraySize = 1;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_desc.Width = tex_desc.Width;
	ds_desc.Height = tex_desc.Height;
	ds_desc.MipLevels = 1;
	ds_desc.MiscFlags = 0;
	ds_desc.SampleDesc = { 1,0 };
	ds_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> dsTex;
	r_assert(
		DX_Device->CreateTexture2D(&ds_desc, nullptr, dsTex.GetAddressOf())
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;
	dsv_desc.Flags = 0;
	dsv_desc.Format = ds_desc.Format;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(dsTex.Get(),&dsv_desc, dsv.GetAddressOf())
	);

	mapVS = new VShader("ShadowTranspVS.cso", Adv_ILayouts, ARRAYSIZE(Adv_ILayouts));
	mapVS->AddCB(0, 1, sizeof(XMMATRIX) * 2);
	mapPS = new PShader("ShadowTranspPS.cso");
	
	cbVPTMat = new Buffer(sizeof(XMMATRIX));

	rsState = new RasterizerState(nullptr);
	dsState = new DepthStencilState(nullptr);
	blendState = new BlendState(nullptr);

	view = new Camera(FRAME_KIND_ORTHOGONAL, volume.x, volume.y, 0.1f, 400, NULL, NULL, true);
}

TranspShadowMap::~TranspShadowMap()
{
	delete rsState;
	delete dsState;
	delete blendState;
	delete view;
	delete mapVS;
	delete mapPS;
}

void TranspShadowMap::Mapping(const DirectionalLight* dLight)
{
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);
	D3D11_VIEWPORT oriVP;
	UINT vpNum = 1;
	DX_DContext->RSGetViewports(&vpNum, &oriVP);

	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_SHADOW_TRANSP, 1, &nullSRV);
	float defaultColor[4] = { 0,0,0,1 };
	DX_DContext->ClearRenderTargetView(rtv.Get(), defaultColor);
	DX_DContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0f, NULL);
	DX_DContext->OMSetRenderTargets(1, rtv.GetAddressOf(), dsv.Get());
	DX_DContext->RSSetViewports(1, &vp);

	XMFLOAT3 lightDir = dLight->GetDir();
	view->transform->SetRot(lightDir);
	view->transform->SetTranslation(-lightDir * 200);
	view->Update();

	mapPS->Apply();
	XMMATRIX vpMat = view->VMat() * view->ProjMat();
	XMMATRIX vpt = vpMat * XMMATRIX(
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1);
	cbVPTMat->Write(&vpt);
	DX_DContext->PSSetConstantBuffers(SHADER_REG_CB_LIGHTVP, 1, cbVPTMat->GetAddress());

	for (auto curObj : drawObjs)
	{
		if ((curObj->name == "Skybox") || (curObj->name == "Tile"))
			continue;

		XMMATRIX world = curObj->transform->WorldMatrix();

		XMMATRIX mats[2] = {
			world * vpMat,
			XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(world),world)) };
		mapVS->WriteCB(0, mats);
		mapVS->Apply();
		dsState->Apply();
		blendState->Apply();
		rsState->Apply();

		curObj->RenderGeom();
	}

	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
	DX_DContext->RSSetViewports(1, &oriVP);

	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_SHADOW_TRANSP, 1, srv.GetAddressOf());
}
