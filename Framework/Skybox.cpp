#include "stdafx.h"
#include "Skybox.h"
#include "Shader.h"
#include "Transform.h"
#include "SphereMesh.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "DepthStencilState.h"
#include "ShaderFormat.h"
#include "CameraMgr.h"
#include "TextureMgr.h"
#include "ShaderReg.h"

Skybox::Skybox()
	:Object("Skybox", std::make_shared<SphereMesh>(0), nullptr,
		"SkyboxVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts),
		"", "", "",
		"SkyboxPS.cso")
{
	transform->SetScale(300, 300, 300);
	
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	delete rsState;
	D3D11_RASTERIZER_DESC rs_desc;
	ZeroMemory(&rs_desc, sizeof(D3D11_RASTERIZER_DESC));
	rs_desc.CullMode = D3D11_CULL_FRONT;
	rs_desc.FillMode = D3D11_FILL_SOLID;
	rs_desc.FrontCounterClockwise = false;
	rsState = new RasterizerState(&rs_desc);
}

void Skybox::Mapping()const
{
	ID3D11ShaderResourceView* cmSRV = TextureMgr::Instance()->Get("cm");
	DX_DContext->PSSetShaderResources(SHADER_REG_SRV_CM, 1, &cmSRV);
}
void Skybox::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const
{
	if (sceneDepth != 0)
		return;

	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		XMMATRIX wvp = transform->WorldMatrix() * vp;
		vs->WriteCB(0, &wvp);

		Object::Render();
	}

	Blur();
}

void Skybox::Blur()const
{

}


