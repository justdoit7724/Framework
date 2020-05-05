
#include "Object.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "ShaderReg.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Mesh.h"
#include "CameraMgr.h"
#include "ObjectMgr.h"
#include "Debugging.h"
#include "Collider.h"
#include "UnlitObj.h"

UnlitObj::UnlitObj(std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, XMVECTOR color, bool directRender)
	:Object(name, shape, collider,
		"MarkVS.cso", simple_ILayouts, ARRAYSIZE(simple_ILayouts),
		"","","",
		"MarkPS.cso",
		directRender), color(color)
{
	vs->AddCB(0, 1, sizeof(XMMATRIX));
	ps->AddCB(SHADER_REG_CB_COLOR, 1, sizeof(XMVECTOR));
}

void UnlitObj::Render(const XMMATRIX & vp, const Frustum & frustum, UINT sceneDepth) const
{
	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		XMMATRIX wvp = transform->WorldMatrix() * vp;

		vs->WriteCB(0, &wvp);
		ps->WriteCB(SHADER_REG_CB_COLOR, &color);

		Object::Render();
	}
}
