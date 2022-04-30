
#include "stdafx.h"

#include "Skybox.h"
<<<<<<< HEAD
=======
#include "Shader.h"
#include "Transform.h"
#include "SphereMesh.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "DepthStencilState.h"
#include "ShaderFormat.h"
#include "ShaderReg.h"
#include "Graphic.h"

using namespace DX;
>>>>>>> 03_DepthPeeling

/*
Skybox::Skybox(ID3D11Device* device, ID3D11DeviceContext* dContext)
	:Object(device, dContext, "Skybox", nullptr, nullptr,
		"SkyboxVS.cso", D3DLayout_Std().GetLayout().data(), D3DLayout_Std().GetLayout().size(),
		"", "", "",
		"SkyboxPS.cso")
{
	VertexLayout layout = D3DLayout_Std();
	m_mesh = std::make_shared<SphereMesh>(device, 0, &layout);

	transform->SetScale(300, 300, 300);

	vs->AddCB(device, 0, 1, sizeof(XMMATRIX));
	delete rsState;
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_FRONT;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rsState = new RasterizerState(device, &rs_desc);
}

Skybox::~Skybox()
{
	blur1SRV->Release();
	blur2SRV->Release();
	blur1RTV->Release();
	blur2RTV->Release();
}

void Skybox::Mapping(ID3D11DeviceContext* dContext, ID3D11ShaderResourceView* cmSRV)const
{
	dContext->PSSetShaderResources(SHADER_REG_SRV_CM, 1, &cmSRV);
}
void Skybox::Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum* frustum, UINT sceneDepth) const
{
	if (sceneDepth != 0)
		return;

	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		XMMATRIX wvp = transform->WorldMatrix() * v * p;
		vs->WriteCB(dContext, 0, &wvp);

		Object::Render(dContext);
	}

	Blur();
}

void Skybox::Blur()const
{

}


*/