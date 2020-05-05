
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

//fundamental elements
Object::Object(std::string name, std::shared_ptr <Mesh> mesh, std::shared_ptr<Collider> collider, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS, bool directRender)
	:name(name), mesh(mesh), collider(collider)
{
	if(directRender)
		ObjectMgr::Instance()->Register(this);

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
Object::Object(std::string name, std::shared_ptr <Mesh> mesh, std::shared_ptr<Collider> collider, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV, bool directRender)
	: name(name), mesh(mesh), collider(collider)
{
	if(directRender)
		ObjectMgr::Instance()->Register(this);

	transform = new Transform();
	vs = new VShader("StdVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("StdPS.cso");

	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddCB(SHADER_REG_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	ps->WriteCB(SHADER_REG_CB_MATERIAL,&SHADER_MATERIAL(XMFLOAT3(0.7,0.7,0.7), 0.2, XMFLOAT3(0.6, 0.6, 0.6), XMFLOAT3(0.5, 0.5, 0.5)));
	
	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	ps->AddSRV(SHADER_REG_SRV_NORMAL, 1);
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, diffSRV);
	if (!normalSRV)
	{
		TextureMgr::Instance()->Load("normal", "DXFramework\\Data\\Texture\\default_normal.png");
		ps->WriteSRV(SHADER_REG_SRV_NORMAL, TextureMgr::Instance()->Get("normal"));
	}
	else
	{
		ps->WriteSRV(SHADER_REG_SRV_NORMAL, normalSRV);
	}

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

	ObjectMgr::Instance()->Remove(this);
}

void Object::Update()
{
	if (!enabled)
		return;

	UpdateBound();
	UpdateCollider();
}

void Object::UpdateBound()
{
	if (!mesh)
		return;

	XMFLOAT3 boundlMinPt;
	XMFLOAT3 boundlMaxPt;
	mesh->GetLBound(&boundlMinPt, &boundlMaxPt);
	XMMATRIX world = transform->WorldMatrix();
	XMFLOAT3 wMinPt = Multiply(boundlMinPt, world);
	XMFLOAT3 wMaxPt = Multiply(boundlMaxPt, world);
	bound.p = transform->GetPos();
	bound.rad = Length(wMinPt - wMaxPt) * 0.5f;
}

void Object::UpdateCollider()
{
	if (!collider)
		return;

	collider->Translate(transform->GetPos());
	collider->SetRotate(transform->GetForward(), transform->GetUp());
	collider->SetScale(transform->GetScale());
}

void Object::Visualize()
{

	//if (IsInsideFrustum(CameraMgr::Instance()->Main()->GetFrustum()))
	//	Debugging::Instance()->Mark(bound.p, bound.rad, Colors::LightGreen);
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

	mesh->Apply();
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

	mesh->Apply();
}

bool Object::IsInsideFrustum(const Frustum& frustum) const
{
	if (frustum.skip || !collider)
		return true;

	return (
		IntersectInPlaneSphere(frustum.right, bound) &&
		IntersectInPlaneSphere(frustum.left, bound) &&
		IntersectInPlaneSphere(frustum.top, bound) &&
		IntersectInPlaneSphere(frustum.bottom, bound) &&
		IntersectInPlaneSphere(frustum.front, bound) &&
		IntersectInPlaneSphere(frustum.back, bound));
}

bool Object::IsPicking(Geometrics::Ray ray) const
{
	return IntersectRaySphere(ray, bound);
}