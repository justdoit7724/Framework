#include "Light.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "Game_info.h"
#include "Transform.h"
#include "Scene.h"

SHADER_DIRECTIONAL_LIGHT DirectionalLight::data;
SHADER_POINT_LIGHT PointLight::data;
SHADER_SPOT_LIGHT SpotLight::data;
ID3D11Buffer* DirectionalLight::cb;
ID3D11Buffer* PointLight::cb;
ID3D11Buffer* SpotLight::cb;
Camera* DirectionalLight::view=nullptr;
Camera* PointLight::view[6] = { nullptr };
Camera* SpotLight::view=nullptr;

DirectionalLight::DirectionalLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, XMFLOAT3 dir)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.enabled[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

#pragma region ShadowMap

	D3D11_TEXTURE2D_DESC sdTex_desc;
	sdTex_desc.ArraySize = 1;
	sdTex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	sdTex_desc.CPUAccessFlags = 0;
	sdTex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	sdTex_desc.Width = 1024;
	sdTex_desc.Height = 1024;
	sdTex_desc.MipLevels = 1;
	sdTex_desc.MiscFlags = 0;
	sdTex_desc.SampleDesc = { 1,0 };
	sdTex_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> sdTex;
	r_assert(
		DX_Device->CreateTexture2D(&sdTex_desc, nullptr, sdTex.GetAddressOf())
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC sdDSV_desc;
	sdDSV_desc.Flags = 0;
	sdDSV_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	sdDSV_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	sdDSV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(sdTex.Get(), &sdDSV_desc, shadowMapDSV.GetAddressOf())
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC sdSRV_desc;
	sdSRV_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sdSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sdSRV_desc.Texture2D.MipLevels = 1;
	sdSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(sdTex.Get(), &sdSRV_desc, shadowMapSRV.GetAddressOf())
	);

	shadowMapVP.TopLeftX = 0;
	shadowMapVP.TopLeftY = 0;
	shadowMapVP.Width = sdTex_desc.Width;
	shadowMapVP.Height = sdTex_desc.Height;
	shadowMapVP.MinDepth = 0;
	shadowMapVP.MaxDepth = 1;
#pragma endregion


	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_DIRECTIONAL_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	r_assert(
		DX_Device->CreateBuffer(&cb_desc, nullptr, &cb)
	);

	if(view==nullptr)
		view= new Camera("DirectionalLight",FRAME_KIND_ORTHOGONAL, 1024, 1024, 0.1f, 100.0f, XM_PIDIV2, 1.0f);

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetDir(dir);
	Enable(ENABLED);

}

void DirectionalLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void DirectionalLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void DirectionalLight::SetDir(XMFLOAT3 d)
{
	view->transform->SetRot(d);
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void DirectionalLight::Enable(STATE enable)
{
	data.enabled[id] = XMFLOAT4(enable, enable, enable, enable);
}

void DirectionalLight::ShadowCapture(Scene* scene) const
{
	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	DX_DContext->PSSetShader(nullptr, nullptr, 0);

	D3D11_VIEWPORT oriVP;
	UINT VPCount = 1;
	DX_DContext->RSGetViewports(&VPCount, &oriVP);
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);

	DX_DContext->RSSetViewports(1, &shadowMapVP);
	ID3D11RenderTargetView* nullRTV = nullptr;
	DX_DContext->OMSetRenderTargets(1, &nullRTV, shadowMapDSV.Get());
	DX_DContext->ClearDepthStencilView(shadowMapDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	scene->ShadowCapture(view);

	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
}

void DirectionalLight::Apply()
{
	D3D11_MAPPED_SUBRESOURCE mappedData;

	r_assert(DX_DContext->Map(cb, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedData));
	CopyMemory(mappedData.pData, &data, sizeof(SHADER_DIRECTIONAL_LIGHT));
	DX_DContext->Unmap(cb, 0);

	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_DIRECTIONAL_LIGHT, 1, &cb);
}



PointLight::PointLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float range, XMFLOAT3 att, XMFLOAT3 pos)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.info[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

#pragma region ShadowMap

	D3D11_TEXTURE2D_DESC sdTex_desc;
	sdTex_desc.ArraySize = 6;
	sdTex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	sdTex_desc.CPUAccessFlags = 0;
	sdTex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	sdTex_desc.Width = 1024;
	sdTex_desc.Height = 1024;
	sdTex_desc.MipLevels = 1;
	sdTex_desc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	sdTex_desc.SampleDesc = { 1,0 };
	sdTex_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> sdTex;
	r_assert(
		DX_Device->CreateTexture2D(&sdTex_desc, nullptr, sdTex.GetAddressOf())
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC sdDSV_desc;
	sdDSV_desc.Flags = 0;
	sdDSV_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	sdDSV_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
	sdDSV_desc.Texture2DArray.ArraySize = 1;
	sdDSV_desc.Texture2DArray.MipSlice = 0;
	for (int i = 0; i < 6; ++i)
	{
		sdDSV_desc.Texture2DArray.FirstArraySlice = i;
		r_assert(
			DX_Device->CreateDepthStencilView(sdTex.Get(), &sdDSV_desc, shadowMapDSV[i].GetAddressOf())
		);
	}
	D3D11_SHADER_RESOURCE_VIEW_DESC sdSRV_desc;
	sdSRV_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sdSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	sdSRV_desc.TextureCube.MipLevels = 1;
	sdSRV_desc.TextureCube.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(sdTex.Get(), &sdSRV_desc, shadowMapSRV.GetAddressOf())
	);
	shadowMapVP.TopLeftX = 0;
	shadowMapVP.TopLeftY = 0;
	shadowMapVP.Width = sdTex_desc.Width;
	shadowMapVP.Height = sdTex_desc.Height;
	shadowMapVP.MinDepth = 0;
	shadowMapVP.MaxDepth = 1;
#pragma endregion


	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_POINT_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	r_assert(
		DX_Device->CreateBuffer(&cb_desc, nullptr, &cb)
	);

	if(view[0]==nullptr)
		for (int i = 0; i < 6; ++i)
		{
			view[i] = new Camera("PointLight", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, XM_PIDIV2, 1.0f);
		}

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetRange(range);
	SetAtt(att);
	Enable(ENABLED);
}

void PointLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(a.x, a.y, a.z, 0);
}

void PointLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void PointLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(s.x, s.y, s.z, 0);
}

void PointLight::SetPos(XMFLOAT3 p)
{
	for (int i = 0; i < 6; ++i)
	{
		view[i]->transform->SetTranslation(p);
	}
	data.pos[id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void PointLight::SetRange(float r)
{
	range = r;
	data.info[id].y = r;
}

void PointLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(at.x, at.y, at.z, 0);
}

void PointLight::Enable(STATE enable)
{
	data.info[id].x = enable;
}

void PointLight::ShadowCapture(Scene* scene) const
{
	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);
	DX_DContext->PSSetShader(nullptr, nullptr, 0);

	D3D11_VIEWPORT oriVP;
	UINT VPCount = 1;
	DX_DContext->RSGetViewports(&VPCount, &oriVP);
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	DX_DContext->OMGetRenderTargets(1, &oriRTV, &oriDSV);

	DX_DContext->RSSetViewports(1, &shadowMapVP);

	for (int i = 0; i < 6; ++i)
	{
		ID3D11RenderTargetView* nullRTV = nullptr;
		DX_DContext->OMSetRenderTargets(1, &nullRTV, shadowMapDSV[i].Get());
		DX_DContext->ClearDepthStencilView(shadowMapDSV[i].Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		scene->ShadowCapture(view[i]);
	}

	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->OMSetRenderTargets(1, &oriRTV, oriDSV);
}

void PointLight::Apply()
{
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_POINT_LIGHT, 1, &cb);
}

SpotLight::SpotLight(XMFLOAT3 a, XMFLOAT3 d, XMFLOAT3 s, float r, float spot, float rad, XMFLOAT3 att, XMFLOAT3 pos, XMFLOAT3 dir)
{
	for (int i = 0; i < LIGHT_MAX_EACH; ++i)
	{
		if (data.info[i].x == LIGHT_DISABLED)
		{
			id = i;
			break;
		}
	}

#pragma region ShadowMap

	D3D11_TEXTURE2D_DESC sdTex_desc;
	sdTex_desc.ArraySize = 1;
	sdTex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	sdTex_desc.CPUAccessFlags = 0;
	sdTex_desc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	sdTex_desc.Width = 1024;
	sdTex_desc.Height = 1024;
	sdTex_desc.MipLevels = 1;
	sdTex_desc.MiscFlags = 0;
	sdTex_desc.SampleDesc = { 1,0 };
	sdTex_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> sdTex;
	r_assert(
		DX_Device->CreateTexture2D(&sdTex_desc, nullptr, sdTex.GetAddressOf())
	);
	D3D11_DEPTH_STENCIL_VIEW_DESC sdDSV_desc;
	sdDSV_desc.Flags = 0;
	sdDSV_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	sdDSV_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	sdDSV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(sdTex.Get(), &sdDSV_desc, shadowMapDSV.GetAddressOf())
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC sdSRV_desc;
	sdSRV_desc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	sdSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	sdSRV_desc.Texture2D.MipLevels = 1;
	sdSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(sdTex.Get(), &sdSRV_desc, shadowMapSRV.GetAddressOf())
	);
	shadowMapVP.TopLeftX = 0;
	shadowMapVP.TopLeftY = 0;
	shadowMapVP.Width = sdTex_desc.Width;
	shadowMapVP.Height = sdTex_desc.Height;
	shadowMapVP.MinDepth = 0;
	shadowMapVP.MaxDepth = 1;
#pragma endregion


	D3D11_BUFFER_DESC cb_desc;
	cb_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cb_desc.ByteWidth = sizeof(SHADER_SPOT_LIGHT);
	cb_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cb_desc.MiscFlags = 0;
	cb_desc.StructureByteStride = 0;
	cb_desc.Usage = D3D11_USAGE_DYNAMIC;
	r_assert(
		DX_Device->CreateBuffer(&cb_desc, nullptr, &cb)
	);

	if(view==nullptr)
		view = new Camera("SpotLight", FRAME_KIND_PERSPECTIVE, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f, XM_PIDIV2, 1.0f);

	SetAmbient(a);
	SetDiffuse(d);
	SetSpecular(s);
	SetPos(pos);
	SetDir(dir);
	SetRange(r);
	SetRad(rad);
	SetSpot(spot);
	SetAtt(att);
	Enable(ENABLED);
}

void SpotLight::SetAmbient(const XMFLOAT3 & a)
{
	ambient = a;
	data.ambient[id] = XMFLOAT4(ambient.x, ambient.y, ambient.z, 0);
}

void SpotLight::SetDiffuse(const XMFLOAT3 & d)
{
	diffuse = d;
	data.diffuse[id] = XMFLOAT4(diffuse.x, diffuse.y, diffuse.z, 0);
}

void SpotLight::SetSpecular(const XMFLOAT3 & s)
{
	specular = s;
	data.specular[id] = XMFLOAT4(specular.x, specular.y, specular.z, 0);
}

void SpotLight::SetPos(XMFLOAT3 p)
{
	view->transform->SetTranslation(p);
	data.pos[id] = XMFLOAT4(p.x, p.y, p.z, 0);
}

void SpotLight::SetDir(XMFLOAT3 d)
{
	view->transform->SetRot(d);
	data.dir[id] = XMFLOAT4(d.x, d.y, d.z, 0);
}

void SpotLight::SetRange(float r)
{
	range = r;
	data.info[id].y = r;
}

void SpotLight::SetRad(float r)
{
	rad = r;
	data.info[id].z = r;
}


void SpotLight::SetSpot(float s)
{
	spot = s;
	data.info[id].w = spot;
}

void SpotLight::SetAtt(XMFLOAT3 at)
{
	att = at;
	data.att[id] = XMFLOAT4(att.x, att.y,att.z, 0);
}

void SpotLight::Enable(STATE enable)
{
	data.info[id].x = enable;
}

void SpotLight::ShadowCapture(Scene* scene) const
{
}

void SpotLight::Apply()
{
	DX_DContext->PSSetConstantBuffers(SHADER_REG_PS_SPOT_LIGHT, 1, &cb);
}

