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
using namespace DX;

DepthPeeling::DepthPeeling(Graphic* graphic)
	:m_dxGraphic(graphic)
{
	D3D11_BLEND_DESC desc;
	desc.AlphaToCoverageEnable = FALSE;
	desc.IndependentBlendEnable = FALSE;
	desc.RenderTarget[0].BlendEnable = true;
	desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
	desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	desc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	//todo --disable write
	desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	m_dxBlendState = new BlendState(m_dxGraphic->Device(), &desc);
	m_dxRState = new RasterizerState(m_dxGraphic->Device(), nullptr);
	m_dxDSState = new DepthStencilState(m_dxGraphic->Device(),nullptr);

	m_dxVShader = new VShader(m_dxGraphic->Device(), "StdVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	m_dxVShader->AddCB(m_dxGraphic->Device(), 0, 1, sizeof(SHADER_STD_TRANSF));
	m_dxPShader = new PShader(m_dxGraphic->Device(), "DepthPeelingPS.cso");
	m_dxPShader->AddSRV(10, 1);

	D3D11_TEXTURE2D_DESC texDesc;
	ID3D11Texture2D* tex;
	m_dxGraphic->BackBuffer()->GetDesc(&texDesc);
	m_dxGraphic->Device()->CreateTexture2D(&texDesc, nullptr, &tex);
	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	m_dxGraphic->RTV()->GetDesc(&rtvDesc);
	m_dxGraphic->Device()->CreateRenderTargetView(tex, &rtvDesc, &m_dxRTV);
	tex->Release();
}

DepthPeeling::~DepthPeeling()
{
	m_dxRTV->Release();
	PreRun();
}

void DepthPeeling::Run(const Camera* cam, const std::vector<Object*>& objs, int nPic)
{
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDSV;
	ID3D11RenderTargetView* nullRTV = nullptr;
	m_dxGraphic->DContext()->OMGetRenderTargets(1, &oriRTV, &oriDSV);


	PreRun();

	for (int i = 0; i < nPic; ++i)
	{
		PictureInfo pic;
		Create(&pic);
		m_vdxPic.push_back(pic);
	}

	m_dxBlendState->Apply(m_dxGraphic->DContext());
	m_dxRState->Apply(m_dxGraphic->DContext());
	m_dxDSState->Apply(m_dxGraphic->DContext());

	XMMATRIX v = cam->VMat();
	XMMATRIX p = cam->ProjMat();

	//todo --remove
	FLOAT clearCol[4] = { 1,0,1,0 };
	m_dxGraphic->DContext()->ClearRenderTargetView(m_dxRTV, clearCol);

	// very first
	m_dxGraphic->DContext()->PSSetShader(nullptr, nullptr, 0);
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &nullRTV, m_vdxPic[0].dsv);
	for (auto it = objs.begin(); it != objs.end(); ++it)
	{
		Object* obj = *it;
		XMMATRIX w = obj->transform->WorldMatrix();
		SHADER_STD_TRANSF shaderTrans(w, v, p, 0, 0, 0, 0);
		m_dxVShader->WriteCB(m_dxGraphic->DContext(), 0, &shaderTrans);
		m_dxVShader->Apply(m_dxGraphic->DContext());

		(*it)->Update();
		(*it)->RenderGeom(m_dxGraphic->DContext());
	}

	// rest
	for (int i = 1; i < nPic; ++i)
	{
		m_dxGraphic->DContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
		m_dxPShader->WriteSRV(10, m_vdxPic[size_t(i)-1].srv);
		m_dxPShader->Apply(m_dxGraphic->DContext());
		m_dxGraphic->DContext()->OMSetRenderTargets(1, &m_dxRTV, m_vdxPic[i].dsv);

		for (auto it = objs.begin(); it != objs.end(); ++it)
		{
			Object* obj = *it;
			XMMATRIX w = obj->transform->WorldMatrix();
			SHADER_STD_TRANSF shaderTrans(w, v, p, 0, 0, 0, 0);
			m_dxVShader->WriteCB(m_dxGraphic->DContext(), 0, &shaderTrans);
			m_dxVShader->Apply(m_dxGraphic->DContext());

			(*it)->Update();
			(*it)->RenderGeom(m_dxGraphic->DContext());
		}
	}
	m_dxPShader->UnboundSRVAll(m_dxGraphic->DContext());
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &oriRTV, oriDSV);

}

void DX::DepthPeeling::ApplyDSV(int index)
{
	ID3D11RenderTargetView* oriRTV;
	ID3D11DepthStencilView* oriDTV;
	m_dxGraphic->DContext()->OMGetRenderTargets(1, &oriRTV, &oriDTV);
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &oriRTV, m_vdxPic[index].dsv);
}

ID3D11ShaderResourceView* DX::DepthPeeling::GetSRV(int index)
{
	return m_vdxPic[index].srv;
}

ID3D11DepthStencilView* DX::DepthPeeling::GetDSV(int index)
{
	return m_vdxPic[index].dsv;
}

void DepthPeeling::Create(PictureInfo* picInfo)
{
	D3D11_TEXTURE2D_DESC texDesc;
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ID3D11Texture2D* tex;
	ID3D11DepthStencilView* dsv;
	ID3D11ShaderResourceView* srv;

	m_dxGraphic->DepthBuffer()->GetDesc(&texDesc);
	texDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;
	m_dxGraphic->Device()->CreateTexture2D(&texDesc, nullptr, &tex);

	m_dxGraphic->DSV()->GetDesc(&dsvDesc);
	m_dxGraphic->Device()->CreateDepthStencilView(tex, &dsvDesc, &dsv);
	m_dxGraphic->DContext()->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, NULL);

	srvDesc.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	switch (dsvDesc.ViewDimension)
	{
	case D3D11_DSV_DIMENSION_TEXTURE2D:
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = 1;
		srvDesc.Texture2D.MostDetailedMip = 0;
		break;
	case D3D11_DSV_DIMENSION_TEXTURE2DMS:
		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DMS;
		break;
	}
	m_dxGraphic->Device()->CreateShaderResourceView(tex, &srvDesc, &srv);
	tex->Release();

	picInfo->dsv = dsv;
	picInfo->srv = srv;
}

void DepthPeeling::PreRun()
{
	ID3D11RenderTargetView* nullRTV = nullptr;
	m_dxGraphic->DContext()->OMSetRenderTargets(1, &nullRTV, nullptr);
	m_dxPShader->UnboundSRVAll(m_dxGraphic->DContext());

	for (auto it = m_vdxPic.begin(); it != m_vdxPic.end(); ++it)
	{
		it->dsv->Release();
		it->srv->Release();
	}
	m_vdxPic.clear();
}
