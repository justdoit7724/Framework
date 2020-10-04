
#include "pch.h"

#include "Object.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "ShaderReg.h"
#include "TextureLoad.h"
#include "Transform.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Mesh.h"
#include "Debugging.h"
#include "Collider.h"
#include "UnlitObj.h"

using namespace DX;

UnlitObj::UnlitObj(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, XMVECTOR color, bool directRender)
	:Object(device, dContext, name, shape, collider,
		"MarkVS.cso", simple_ILayouts, ARRAYSIZE(simple_ILayouts),
		"","","",
		"MarkPS.cso",
		directRender), color(color)
{
	vs->AddCB(device, 0, 1, sizeof(XMMATRIX));
	ps->AddCB(device,SHADER_REG_CB_COLOR, 1, sizeof(XMVECTOR));
}

void UnlitObj::Render(ID3D11DeviceContext* dContext, const XMMATRIX & v, const XMMATRIX& p, const Frustum & frustum, UINT sceneDepth) const
{
	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		XMMATRIX wvp = transform->WorldMatrix() * v*p;

		vs->WriteCB(dContext, 0, &wvp);
		ps->WriteCB(dContext, SHADER_REG_CB_COLOR, &color);

		Object::Render(dContext);
	}
}
