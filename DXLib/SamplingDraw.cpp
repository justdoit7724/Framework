#include "pch.h"
#include "SamplingDraw.h"
#include "ShaderFormat.h"
#include "Transform.h"
#include "QuadMesh.h"
#include "Shader.h"
#include "DepthStencilState.h"

using namespace DX;

SamplingDraw::SamplingDraw(ID3D11Device* device, ID3D11DeviceContext* dContext)
	:Object(device, dContext, "sampling", nullptr, nullptr, "EF_V_Sampling.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts), "", "", "", "EF_P_Sampling.cso")
{
	mesh = std::make_shared<QuadMesh>(device);
	transform->SetScale(2, 2, 1);
	transform->SetRot(-FORWARD);
	vs->AddCB(device, 0, 1, sizeof(XMMATRIX));
	ps->AddCB(device, 7, 1, sizeof(XMFLOAT4));

	D3D11_DEPTH_STENCIL_DESC dsDesc;
	dsDesc.DepthEnable = false;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	dsDesc.StencilEnable = false;
	dsDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
	dsState->Modify(device, &dsDesc);
}

SamplingDraw::~SamplingDraw()
{
}

void SamplingDraw::Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum& frustum, UINT sceneDepth) const
{
	if (!enabled || !show)
		return;

	XMMATRIX wvp = transform->WorldMatrix();
	vs->WriteCB(dContext, 0, &wvp);
	XMFLOAT4 sampleInfo;
	sampleInfo.x = 16;
	sampleInfo.y = 1;
	ps->WriteCB(dContext, 7, &sampleInfo);

	Object::Render(dContext);
}
