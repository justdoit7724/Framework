#include "SSAOMap.h"
#include "Game_info.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Shape.h"
#include "Camera.h"
#include "Transform.h"
#include "Object.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "ShaderReg.h"
#include "Scene.h"

#define SSAO_RES_X (SCREEN_WIDTH*0.5f)
#define SSAO_RES_Y (SCREEN_HEIGHT*0.5f)

#define SSAO_BLUR_HORIZONTAL 1
#define SSAO_BLUR_VERTICAL 0

void NormalDepthMapInit(ID3D11RenderTargetView * *RTV, ID3D11ShaderResourceView * *SRV, ID3D11DepthStencilView * *DSV)
{
	D3D11_TEXTURE2D_DESC nd_desc;
	nd_desc.ArraySize = 1;
	nd_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	nd_desc.CPUAccessFlags = 0;
	nd_desc.Format = DXGI_FORMAT_R32G32B32A32_TYPELESS;
	nd_desc.Width = SSAO_RES_X;
	nd_desc.Height = SSAO_RES_Y;
	nd_desc.MipLevels = 1;
	nd_desc.MiscFlags = 0;
	nd_desc.SampleDesc = { 1,0 };
	nd_desc.Usage = D3D11_USAGE_DEFAULT;
	ComPtr<ID3D11Texture2D> ndMap;
	r_assert(
		DX_Device->CreateTexture2D(&nd_desc, nullptr, ndMap.GetAddressOf())
	);
	D3D11_RENDER_TARGET_VIEW_DESC aoNDRTV_desc;
	aoNDRTV_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	aoNDRTV_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	aoNDRTV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(ndMap.Get(), &aoNDRTV_desc, RTV)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC aoNDSRV_desc;
	aoNDSRV_desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	aoNDSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	aoNDSRV_desc.Texture2D.MipLevels = 1;
	aoNDSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(ndMap.Get(), &aoNDSRV_desc, SRV)
	);
	D3D11_TEXTURE2D_DESC ds_desc;
	ds_desc.ArraySize = 1;
	ds_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	ds_desc.CPUAccessFlags = 0;
	ds_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	ds_desc.Width = SSAO_RES_X;
	ds_desc.Height = SSAO_RES_Y;
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
	dsv_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsv_desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsv_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateDepthStencilView(dsTex.Get(), &dsv_desc, DSV)
	);
}

void ScreenMapInit(ID3D11RenderTargetView** RTV, ID3D11ShaderResourceView** SRV)
{
	D3D11_TEXTURE2D_DESC acsTex_desc;
	acsTex_desc.ArraySize = 1;
	acsTex_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	acsTex_desc.CPUAccessFlags = 0;
	acsTex_desc.Format = DXGI_FORMAT_R8G8B8A8_TYPELESS;
	acsTex_desc.Width = SCREEN_WIDTH;
	acsTex_desc.Height = SCREEN_HEIGHT;
	acsTex_desc.MipLevels = 1;
	acsTex_desc.MiscFlags = 0;
	acsTex_desc.Usage = D3D11_USAGE_DEFAULT;
	acsTex_desc.SampleDesc = { 1,0 };
	ComPtr<ID3D11Texture2D> aoTex;
	r_assert(
		DX_Device->CreateTexture2D(&acsTex_desc, nullptr, aoTex.GetAddressOf())
	);
	D3D11_RENDER_TARGET_VIEW_DESC aoRTV_desc;
	aoRTV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	aoRTV_desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	aoRTV_desc.Texture2D.MipSlice = 0;
	r_assert(
		DX_Device->CreateRenderTargetView(aoTex.Get(), &aoRTV_desc, RTV)
	);
	D3D11_SHADER_RESOURCE_VIEW_DESC aoSRV_desc;
	aoSRV_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	aoSRV_desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	aoSRV_desc.Texture2D.MipLevels = 1;
	aoSRV_desc.Texture2D.MostDetailedMip = 0;
	r_assert(
		DX_Device->CreateShaderResourceView(aoTex.Get(), &aoSRV_desc, SRV)
	);
}


SSAOMap::SSAOMap()
{
	vp.Width = SSAO_RES_X;
	vp.Height = SSAO_RES_Y;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;

	NormalDepthMapInit(ndRTV.GetAddressOf(), ndSRV.GetAddressOf(), dsv.GetAddressOf());
	ScreenMapInit(acsRTV.GetAddressOf(), acsSRV.GetAddressOf());
	ScreenMapInit(blurRTV.GetAddressOf(), blurSRV.GetAddressOf());
	ScreenMapInit(blur2RTV.GetAddressOf(), finalSRV.GetAddressOf());

	ndVS = new VShader("SSAO_ND_VS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	ndVS->AddCB(0, 1, sizeof(XMMATRIX) * 4);
	static const D3D11_INPUT_ELEMENT_DESC iLayout[] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "FAR_PLANE_IDX", 0,DXGI_FORMAT_R32_FLOAT, 0, sizeof(XMFLOAT3),D3D11_INPUT_PER_VERTEX_DATA,0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, sizeof(XMFLOAT4), D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	acsVS = new VShader("SSAO_AcessVS.cso", iLayout, ARRAYSIZE(iLayout));
	acsVS->AddCB(0, 1, sizeof(XMFLOAT4) * 4);
	blurVS = new VShader("SSAO_BlurVS.cso", iLayout, ARRAYSIZE(iLayout));
	ndPS = new PShader("SSAO_ND_PS.cso");
	acsPS = new PShader("SSAO_AcessPS.cso");
	acsPS->AddCB(0, 1, sizeof(XMMATRIX));
	acsPS->AddCB(1, 1, sizeof(XMFLOAT4) * 14);
	acsPS->AddSRV(0, 1);
	D3D11_SAMPLER_DESC samp_desc;
	ZeroMemory(&samp_desc, sizeof(D3D11_SAMPLER_DESC));
	samp_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	samp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samp_desc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
	samp_desc.MinLOD = 0;
	samp_desc.MaxLOD = 1;
	acsPS->AddSamp(0, 1, &samp_desc);
	acsPS->WriteSRV(0, ndSRV.Get());
	blurPS = new PShader("SSAO_BlurPS.cso");
	blurPS->AddCB(0, 1, sizeof(float));
	blurPS->AddSRV(0, 1);
	blurPS->AddSRV(1, 1);
	blurPS->AddSamp(0, 1, &samp_desc);


	dsState = new DepthStencilState(nullptr);
	D3D11_DEPTH_STENCIL_DESC noDs_desc;
	ZeroMemory(&noDs_desc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	noDs_desc.DepthEnable = false;
	noDsState = new DepthStencilState(&noDs_desc);
	blendState = new BlendState(nullptr);

	Vertex_AO vertice[4] = {
		Vertex_AO(XMFLOAT3(-1, 1, 1), 0, XMFLOAT2(0, 0)),
		Vertex_AO(XMFLOAT3(1, 1, 1), 1, XMFLOAT2(1, 0)),
		Vertex_AO(XMFLOAT3(-1, -1, 1), 2, XMFLOAT2(0, 1)),
		Vertex_AO(XMFLOAT3(1,-1,1), 3, XMFLOAT2(1,1))
	};

	sample[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	sample[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);
	sample[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	sample[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);
	sample[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	sample[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);
	sample[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	sample[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);
	sample[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	sample[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);
	sample[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	sample[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);
	sample[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	sample[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);


	vFarPlane[0] = XMFLOAT4(-1000 * tan(XM_PIDIV4), 1000 * tan(XM_PIDIV4), 1000, 0);
	vFarPlane[1] = XMFLOAT4(1000 * tan(XM_PIDIV4), 1000 * tan(XM_PIDIV4), 1000, 0);
	vFarPlane[2] = XMFLOAT4(-1000 * tan(XM_PIDIV4), -1000 * tan(XM_PIDIV4), 1000, 0);
	vFarPlane[3] = XMFLOAT4(1000 * tan(XM_PIDIV4), -1000 * tan(XM_PIDIV4), 1000, 0);

	D3D11_BUFFER_DESC vb_desc;
	vb_desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vb_desc.ByteWidth = sizeof(Vertex_AO) * 4;
	vb_desc.CPUAccessFlags = 0;
	vb_desc.MiscFlags = 0;
	vb_desc.StructureByteStride = 0;
	vb_desc.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA data;
	data.pSysMem = vertice;
	r_assert(
		DX_Device->CreateBuffer(&vb_desc, &data, vb.GetAddressOf())
	);
}

SSAOMap::~SSAOMap()
{
	delete ndVS;
	delete acsVS;
	delete blurVS;
	delete ndPS;
	delete acsPS;
	delete blurPS;
}

void SSAOMap::Mapping(const Scene* scene, const Camera* camera)
{
	ID3D11RenderTargetView* oriPassRTV;
	ID3D11DepthStencilView* oriDSV;
	D3D11_VIEWPORT oriVP;
	DX_DContext->OMGetRenderTargets(1, &oriPassRTV, &oriDSV);
	UINT oriVPNum = 1;
	DX_DContext->RSGetViewports(&oriVPNum, &oriVP);

	DX_DContext->HSSetShader(nullptr, nullptr, 0);
	DX_DContext->DSSetShader(nullptr, nullptr, 0);
	DX_DContext->GSSetShader(nullptr, nullptr, 0);

	DrawNormalDepth(scene, camera);
	DrawAccess(camera->StdProjMat(), oriVP);
	BlurHorizon();
	BlurVertical();

	DX_DContext->OMSetRenderTargets(1, &oriPassRTV, oriDSV);
	DX_DContext->PSSetShaderResources(SHADER_REG_PS_SRV_SSAO, 1, finalSRV.GetAddressOf());
	DX_DContext->RSSetViewports(1, &oriVP);
}


void SSAOMap::DrawNormalDepth(const Scene* scene, const Camera* camera)
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	// unbinding srv used in blurPS shader
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->PSSetShaderResources(SHADER_REG_PS_SRV_SSAO, 1, &nullSRV);

	DX_DContext->RSSetViewports(1, &vp);
	DX_DContext->ClearRenderTargetView(ndRTV.Get(), defaultColor);
	DX_DContext->ClearDepthStencilView(dsv.Get(), D3D11_CLEAR_DEPTH, 1.0, NULL);
	DX_DContext->OMSetRenderTargets(1, ndRTV.GetAddressOf(), dsv.Get());
	ndPS->Apply();
	blendState->Apply();
	dsState->Apply();

	int i = 0;
	while(true)
	{
		const Object* curObj = scene->GetObj(i++);
		if (curObj == nullptr)
			break;

		XMMATRIX w = curObj->transform->WorldMatrix();
		XMMATRIX drawTransf[4] = {
			w,
			camera->VMat(),
			camera->StdProjMat(), // no z Priority for ssao
			XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(w), w))
		};
		ndVS->WriteCB(0, drawTransf);
		ndVS->Apply();

		// only triangle
		D3D11_PRIMITIVE_TOPOLOGY curPrimitiveType = curObj->shape->GetPrimitiveType();
		curObj->shape->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		curObj->RenderGeom();

		curObj->shape->SetPrimitiveType(curPrimitiveType);
	}
}

void SSAOMap::DrawAccess(const XMMATRIX& p, D3D11_VIEWPORT oriVP)
{
	ID3D11ShaderResourceView* nullSRV = nullptr;
	DX_DContext->PSSetShaderResources(0, 1, &nullSRV);
	DX_DContext->RSSetViewports(1, &oriVP);
	DX_DContext->ClearRenderTargetView(acsRTV.Get(), defaultColor);
	DX_DContext->OMSetRenderTargets(1, acsRTV.GetAddressOf(), nullptr);

	UINT stide = sizeof(Vertex_AO);
	UINT offset = 0;
	DX_DContext->IASetVertexBuffers(0, 1, vb.GetAddressOf(), &stide, &offset);
	DX_DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	acsVS->WriteCB(0, vFarPlane);
	XMMATRIX projUv = p * XMMATRIX(
		0.5f, 0, 0, 0,
		0, -0.5f, 0, 0,
		0, 0, 1, 0,
		0.5f, 0.5f, 0, 1);
	acsPS->WriteCB(0, &projUv);
	acsPS->WriteCB(1, sample);
	acsVS->Apply();
	acsPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
}

void SSAOMap::BlurHorizon()
{
	DX_DContext->ClearRenderTargetView(blurRTV.Get(), defaultColor);
	DX_DContext->OMSetRenderTargets(1, blurRTV.GetAddressOf(), nullptr);
	blurVS->Apply();
	float curDirection = SSAO_BLUR_HORIZONTAL;
	blurPS->WriteCB(0, &curDirection);
	blurPS->WriteSRV(0, ndSRV.Get());
	blurPS->WriteSRV(1, acsSRV.Get());
	blurPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
}

void SSAOMap::BlurVertical()
{
	DX_DContext->ClearRenderTargetView(blur2RTV.Get(), defaultColor);
	DX_DContext->OMSetRenderTargets(1, blur2RTV.GetAddressOf(), nullptr);
	blurVS->Apply();
	float curDirection = SSAO_BLUR_VERTICAL;
	blurPS->WriteCB(0, &curDirection);
	blurPS->WriteSRV(0, ndSRV.Get());
	blurPS->WriteSRV(1, blurSRV.Get());
	blurPS->Apply();
	noDsState->Apply();
	DX_DContext->Draw(4, 0);
}
