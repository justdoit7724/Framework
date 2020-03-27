#include "stdafx.h"
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
#include "Shape.h"
#include "CameraMgr.h"


//fundamental elements
Object::Object(std::string name, std::shared_ptr < Shape> shape, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS)
	:name(name), shape(shape)
{
	transform = new Transform();
	vs = new VShader(sVS, iLayouts, layoutCount);
	hs = new HShader(sHS);
	ds = new DShader(sDS);
	gs = new GShader(sGS);
	ps = new PShader(sPS);

	blendState = new BlendState(nullptr);
	dsState = new DepthStencilState(nullptr);
	rsState = new RasterizerState(nullptr);
}

//standard elements
Object::Object(std::string name, std::shared_ptr < Shape> shape, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV)
	:name(name), shape(shape)
{
	transform = new Transform();
	vs = new VShader("StdVS.cso", Adv_ILayouts, ARRAYSIZE(Adv_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("StdPS.cso");

	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddCB(SHADER_REG_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	ps->WriteCB(SHADER_REG_CB_MATERIAL,&SHADER_MATERIAL(XMFLOAT3(0.7,0.7,0.7), 0.2, XMFLOAT3(0.5, 0.5, 0.5), XMFLOAT3(0.8, 0.8, 0.8)));
	
	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	ps->AddSRV(SHADER_REG_SRV_NORMAL, 1);
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, diffSRV);
	ps->WriteSRV(SHADER_REG_SRV_NORMAL, normalSRV);

	blendState = new BlendState(nullptr);
	dsState = new DepthStencilState(nullptr);
	rsState = new RasterizerState(nullptr);
}

Object::~Object()
{
	delete transform;
	delete vs;
	delete hs;
	delete ds;
	delete gs;
	delete ps;

	delete dsState;
	delete blendState;
	delete rsState;
}

void Object::Update()
{
	if (!enabled)
		return;

	UpdateBound();
}

void Object::UpdateBound()
{
	XMFLOAT3 boundlMinPt;
	XMFLOAT3 boundlMaxPt;
	shape->GetLBound(&boundlMinPt, &boundlMaxPt);
	XMMATRIX world = transform->WorldMatrix();
	XMFLOAT3 wMinPt = Multiply(boundlMinPt, world);
	XMFLOAT3 wMaxPt = Multiply(boundlMaxPt, world);
	bound.p = transform->GetPos();
	bound.rad = Length(wMinPt - wMaxPt) * 0.5f;
}

Object::Object()
{
}

void Object::Render()const
{
	vs->Apply();
	hs->Apply();
	ds->Apply();
	gs->Apply();
	ps->Apply();

	dsState->Apply();
	blendState->Apply();
	rsState->Apply();

	shape->Apply();
}
void Object::Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const
{
	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		const SHADER_STD_TRANSF STransformation(transform->WorldMatrix(), vp);

		vs->WriteCB(0, &STransformation);

		Render();
	}
}

void Object::RenderGeom() const
{
	if (!enabled || !show)
		return;

	shape->Apply();
}

bool Object::IsInsideFrustum(const Frustum& frustum) const
{
	if (frustum.skip)
		return true;

	return (
		IntersectInPlaneSphere(frustum.right, bound) &&
		IntersectInPlaneSphere(frustum.left, bound) &&
		IntersectInPlaneSphere(frustum.top, bound) &&
		IntersectInPlaneSphere(frustum.bottom, bound) &&
		IntersectInPlaneSphere(frustum.front, bound) &&
		IntersectInPlaneSphere(frustum.back, bound));
}

bool Object::IsPicking(const Math::Ray ray) const
{
	return Math::IntersectRaySphere(ray, bound);
}