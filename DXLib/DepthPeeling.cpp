#include "pch.h"
#include "DepthPeeling.h"
#include "Object.h"
#include "Camera.h"
#include "RasterizerState.h"
#include "DepthStencilState.h"
#include "Shader.h"
#include "ShaderFormat.h"
#include "Transform.h"
#include "Graphic.h"
#include "BlendState.h"
#include "Buffer.h"
#include "ShaderReg.h"
#include "Light.h"
#include "QuadMesh.h"
#include "UI.h"

using namespace DX;

DepthPeeling::DepthPeeling(Graphic* graphic)
	:m_dxGraphic(graphic)
{
	m_dxBlendState = new BlendState(m_dxGraphic->Device(), nullptr);
	m_dxRState = new RasterizerState(m_dxGraphic->Device(), nullptr);
	m_dxDSState = new DepthStencilState(m_dxGraphic->Device(),nullptr);

	m_dxVShader = new VShader(m_dxGraphic->Device(), "StdVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	m_dxVShader->AddCB(m_dxGraphic->Device(), 0, 1, sizeof(SHADER_STD_TRANSF));
	m_dxPShader = new PShader(m_dxGraphic->Device(), "DepthPeelingPS.cso");
	m_dxPShader->AddSRV(10, 1);
	m_dxPShader->AddCB(m_dxGraphic->Device(), SHADER_REG_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	m_dxPShader->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	m_dxPShader->AddSRV(SHADER_REG_SRV_NORMAL, 1);

	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* tex;
	m_dxGraphic->BackBuffer()->GetDesc(&texDesc);
	texDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.ArraySize = 1;
	texDesc.MipLevels = 1;
	D3D11_SUBRESOURCE_DATA data;
	int nSize = 4 * texDesc.Width * texDesc.Height;
	data.pSysMem = (void*)malloc(nSize);
	ZeroMemory((void*)data.pSysMem, nSize);
	data.SysMemPitch = 4 * texDesc.Width;
	m_dxGraphic->Device()->CreateTexture2D(&texDesc, &data, &tex);
	free((void*)data.pSysMem);
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;
	m_dxGraphic->Device()->CreateShaderResourceView(tex, &srvDesc, &m_nullSRV);
	tex->Release();

	m_cbEye = new Buffer(graphic->Device(), sizeof(XMFLOAT4));

	std::shared_ptr<QuadMesh> spMesh = std::make_shared<QuadMesh>(m_dxGraphic->Device());
	m_renderQuad = new Object(m_dxGraphic->Device(), m_dxGraphic->DContext(), "renderObj", spMesh, nullptr,"StdVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),"","","","DepthPeelingRenderPS.cso");
	m_renderQuad->transform->SetRot(-FORWARD, UP);
	m_renderQuad->transform->SetScale(2, 2, 1);
	D3D11_DEPTH_STENCIL_DESC renderDepthDesc;
	ZeroMemory(&renderDepthDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	renderDepthDesc.DepthEnable = FALSE;
	renderDepthDesc.StencilEnable = FALSE;
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_MAX;
	m_renderQuad->dsState->Modify(m_dxGraphic->Device(), &renderDepthDesc);
	m_renderQuad->blendState->Modify(m_dxGraphic->Device(), &blendDesc);
	m_renderQuad->vs->AddCB(m_dxGraphic->Device(), 0, 1, sizeof(SHADER_STD_TRANSF));
	m_renderQuad->ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1); 
}

DepthPeeling::~DepthPeeling()
{
	m_nullSRV->Release();

	for (auto it = m_vdxPic.begin(); it != m_vdxPic.end(); ++it)
	{
		it->dsv->Release();
		it->depthSrv->Release();
		it->rtv->Release();
		it->backSrv->Release();
	}
	m_vdxPic.clear();

	delete m_cbEye;
}

void DepthPeeling::Run(const Camera* cam, const std::vector<Light*>& lights, const std::vector<Object*>& objs, int nPic)
{
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	m_dxGraphic->DContext()->OMGetRenderTargets(1, &oriRTV, &oriDSV);

	PreRun(nPic);

	for (auto it = lights.begin(); it != lights.end(); ++it)
		(*it)->Apply(m_dxGraphic->Device(), m_dxGraphic->DContext());

	XMFLOAT3 eye = cam->transform->GetPos();
	m_cbEye->Write(m_dxGraphic->DContext(), &eye);
	m_dxGraphic->DContext()->PSSetConstantBuffers(SHADER_REG_CB_EYE, 1, m_cbEye->GetAddress());

	m_dxBlendState->Apply(m_dxGraphic->DContext());
	m_dxRState->Apply(m_dxGraphic->DContext());
	m_dxDSState->Apply(m_dxGraphic->DContext());

	XMMATRIX v = cam->VMat();
	XMMATRIX p = cam->ProjMat();

	for (int i = 0; i < nPic; ++i)
	{
		m_dxGraphic->DContext()->OMSetRenderTargets(1, &m_vdxPic[i].rtv, m_vdxPic[i].dsv);

		m_dxPShader->WriteSRV(10, (i==0)? m_nullSRV : m_vdxPic[i-1].depthSrv);

		for (auto it = objs.begin(); it != objs.end(); ++it)
		{
			Object* obj = *it;

			obj->Update();

			const SHADER_STD_TRANSF STransformation(obj->transform->WorldMatrix(), v, p, NULL, NULL, NULL, NULL);
			ID3D11ShaderResourceView* mainTex, *normal;
			obj->GetMainTex(&mainTex);
			obj->GetNormal(&normal);
			SHADER_MATERIAL material;
			obj->GetMaterial(&material);

			m_dxVShader->WriteCB(m_dxGraphic->DContext(), 0, &STransformation);
			m_dxVShader->Apply(m_dxGraphic->DContext());
			m_dxPShader->WriteCB(m_dxGraphic->DContext(), SHADER_REG_CB_MATERIAL, &material);
			m_dxPShader->WriteSRV(SHADER_REG_SRV_DIFFUSE, mainTex);
			m_dxPShader->WriteSRV(SHADER_REG_SRV_NORMAL, normal);
			m_dxPShader->Apply(m_dxGraphic->DContext());
			
			obj->RenderGeom(m_dxGraphic->DContext());
		}
	}
	m_dxPShader->UnboundSRVAll(m_dxGraphic->DContext());
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &oriRTV, oriDSV);

}

void DX::DepthPeeling::Render(ID3D11DeviceContext* dContext, int iIndex)
{
	Frustum nullFrustum;

	if (iIndex < 0)
	{
		for (auto it = m_vdxPic.rbegin(); it != m_vdxPic.rend(); it++)
		{
			m_renderQuad->ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, it->backSrv);
			m_renderQuad->Render(m_dxGraphic->DContext(), XMMatrixIdentity(), XMMatrixIdentity(), nullptr);
		}
	}
	else
	{
		m_renderQuad->ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, m_vdxPic[iIndex].backSrv);
		m_renderQuad->Render(m_dxGraphic->DContext(), XMMatrixIdentity(), XMMatrixIdentity(), nullptr);
	}
}

void DX::DepthPeeling::GetSRV(int index, ID3D11ShaderResourceView** ppSRV)
{
	*ppSRV = m_vdxPic[index].depthSrv;
}

void DX::DepthPeeling::GetDSV(int index, ID3D11DepthStencilView** ppDSV)
{
	*ppDSV = m_vdxPic[index].dsv;
}

void DX::DepthPeeling::GetRTV(int index, ID3D11RenderTargetView** ppRTV)
{
	*ppRTV = m_vdxPic[index].rtv;
}

void DepthPeeling::Create(PictureInfo* picInfo)
{
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC depthSrvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC backSrvDesc;
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	ID3D11Texture2D* depthTex;
	ID3D11Texture2D* renderTex;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* depthSrv;
	ID3D11RenderTargetView* rtv;
	ID3D11ShaderResourceView* backSrv;

	m_dxGraphic->DepthBuffer()->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_dxGraphic->Device()->CreateTexture2D(&texDesc, nullptr, &depthTex);

	m_dxGraphic->DSV()->GetDesc(&dsvDesc);
	m_dxGraphic->Device()->CreateDepthStencilView(depthTex, &dsvDesc, &dsv);
	m_dxGraphic->DContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	depthSrvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	depthSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	depthSrvDesc.Texture2D.MipLevels = 1;
	depthSrvDesc.Texture2D.MostDetailedMip = 0;
	m_dxGraphic->Device()->CreateShaderResourceView(depthTex, &depthSrvDesc, &depthSrv);
	depthTex->Release();

	m_dxGraphic->BackBuffer()->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_dxGraphic->Device()->CreateTexture2D(&texDesc, nullptr, &renderTex);
	m_dxGraphic->RTV()->GetDesc(&rtvDesc);
	m_dxGraphic->Device()->CreateRenderTargetView(renderTex, &rtvDesc, &rtv);
	backSrvDesc.Format = rtvDesc.Format;
	backSrvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	backSrvDesc.Texture2D.MipLevels = 1;
	backSrvDesc.Texture2D.MostDetailedMip = 0;
	m_dxGraphic->Device()->CreateShaderResourceView(renderTex, &backSrvDesc, &backSrv);
	renderTex->Release();

	picInfo->dsv = dsv;
	picInfo->depthSrv = depthSrv;
	picInfo->rtv = rtv;
	picInfo->backSrv = backSrv;
}

void DepthPeeling::PreRun(int count)
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
	m_dxPShader->UnboundSRVAll(m_dxGraphic->DContext());

	const int nAdd = count - m_vdxPic.size();
	for (int i = 0; i < nAdd; ++i)
	{
		PictureInfo pic;
		Create(&pic);
		m_vdxPic.push_back(pic);
	}
	for (int i = nAdd; i < 0; ++i)
	{
		auto it = m_vdxPic.rbegin();
		it->dsv->Release();
		it->depthSrv->Release();
		it->rtv->Release();
		m_vdxPic.pop_back();
	}

	for (auto it = m_vdxPic.begin(); it != m_vdxPic.end(); ++it)
	{
		m_dxGraphic->DContext()->ClearDepthStencilView(it->dsv, D3D11_CLEAR_DEPTH, 1.0, NULL);
		const FLOAT colBlack[4] = { 0,0,0,0 };
		m_dxGraphic->DContext()->ClearRenderTargetView(it->rtv, colBlack);
	}
}
