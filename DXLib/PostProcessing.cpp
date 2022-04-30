#include "pch.h"
#include "PostProcessing.h"
#include "Shader.h"
#include "Mesh.h"
#include "ShaderFormat.h"
#include "BlendState.h";
#include "Graphic.h"

using namespace DX;

PostProcessing::PostProcessing(ID3D11Device* device, std::string psPath)
{
	auto stdDescs = D3DLayout_Std().GetLayout();

	m_vs = new VShader(device, "HL_PP_StdVS.cso", stdDescs.data(), stdDescs.size());
	m_ps = new PShader(device, psPath);

}

PostProcessing::~PostProcessing()
{
}

void PostProcessing::Render(ID3D11DeviceContext* dContext, ID3D11RenderTargetView* rtv)
{
	if (!rtv)
	{
		dContext->OMGetRenderTargets(1, &rtv, nullptr);
	}

	ID3D11RenderTargetView* dxRTVTmp;
	ID3D11DepthStencilView* dxDSTemp;
	dContext->OMGetRenderTargets(1, &dxRTVTmp, &dxDSTemp);

	dContext->OMSetRenderTargets(1, &rtv, nullptr);

	m_vs->Apply(dContext);
	m_ps->Apply(dContext);
	m_screenMesh->Apply(dContext);

	dContext->OMSetRenderTargets(1, &dxRTVTmp, dxDSTemp);
}
