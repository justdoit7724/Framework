
#include "pch.h"

#include "Object.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "ShaderReg.h"
#include "Transform.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Mesh.h"
#include "Debugging.h"
#include "LayerMask.h"
#include "Collider.h"
#include "Graphic.h"

using namespace DX;

//fundamental elements
Object::Object(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string name, std::shared_ptr <Mesh> mesh, std::shared_ptr<Collider> collider, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS, bool bDirectRender)
	:name(name), m_mesh(mesh), m_collider(collider), layer(LAYER_STD)
{
	transform = new Transform();
	vs = new VShader(device,sVS, iLayouts, layoutCount);
	hs = new HShader(device,sHS);
	ds = new DShader(device,sDS);
	gs = new GShader(device,sGS);
	ps = new PShader(device,sPS);

	blendState = new BlendState(device,nullptr);
	dsState = new DepthStencilState(device, nullptr);
	rsState = new RasterizerState(device,nullptr);

	m_material = new SHADER_MATERIAL(XMFLOAT3(0.6, 0.6, 0.6), 0.4, XMFLOAT3(0.6, 0.6, 0.6), XMFLOAT3(1.0, 1.0, 1.0));
}

//standard elements
Object::Object(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string name, std::shared_ptr <Mesh> mesh, std::shared_ptr<Collider> collider, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV, bool directRender)
	: name(name), m_mesh(mesh), m_collider(collider), layer(LAYER_STD), m_mainTex(diffSRV), m_normal(normalSRV)
{
	auto stdDescs = D3DLayout_Std().GetLayout();

	transform = new Transform();
	vs = new VShader(device,"StdVS.cso", stdDescs.data(), stdDescs.size());
	hs = new HShader(device);
	ds = new DShader(device);
	gs = new GShader(device);
	ps = new PShader(device,"StdPS.cso");

	m_material = new SHADER_MATERIAL(XMFLOAT3(0.6, 0.6, 0.6), 0.4, XMFLOAT3(0.6, 0.6, 0.6), XMFLOAT3(1.0, 1.0, 1.0));

	vs->AddCB(device, 0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddCB(device, SHADER_REG_CB_MATERIAL, 1, sizeof(SHADER_MATERIAL));
	ps->WriteCB(dContext, SHADER_REG_CB_MATERIAL,m_material);
	
	ps->AddSRV(SHADER_REG_SRV_DIFFUSE, 1);
	ps->AddSRV(SHADER_REG_SRV_NORMAL, 1);
	ps->WriteSRV(SHADER_REG_SRV_DIFFUSE, m_mainTex);
	ps->WriteSRV(SHADER_REG_SRV_NORMAL, m_normal);

	blendState = new BlendState(device, nullptr);
	dsState = new DepthStencilState(device, nullptr);
	rsState = new RasterizerState(device, nullptr);

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

	delete m_material;
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
	if (!m_mesh)
		return;

	XMFLOAT3 boundlMinPt;
	XMFLOAT3 boundlMaxPt;
	m_mesh->GetLBound(&boundlMinPt, &boundlMaxPt);
	XMMATRIX world = transform->WorldMatrix();
	XMFLOAT3 wMinPt = Multiply(boundlMinPt, world);
	XMFLOAT3 wMaxPt = Multiply(boundlMaxPt, world);
	bound.p = transform->GetPos();
	bound.rad = Length(wMinPt - wMaxPt) * 0.5f;
}

void Object::UpdateCollider()
{
	if (!m_collider)
		return;

	m_collider->Translate(transform->GetPos());
	m_collider->SetRotate(transform->GetForward(), transform->GetUp());
	m_collider->SetScale(transform->GetScale());
}

Geometrics::Sphere DX::Object::Bound()
{
	return bound;
}

int DX::Object::Layer() const
{
	return layer;
}

void DX::Object::SetLayer(int l)
{
	layer = l;
}

void DX::Object::GetMaterial(SHADER_MATERIAL* material)
{
	*material = *m_material;
}

void DX::Object::GetMainTex(ID3D11ShaderResourceView** ppSRV)
{
	*ppSRV = m_mainTex;
}

void DX::Object::GetNormal(ID3D11ShaderResourceView** ppNormal)
{
	*ppNormal = m_normal;
}

void Object::Visualize()
{

	//if (IsInsideFrustum(CameraMgr::Instance()->Main()->GetFrustum()))
	//	Debugging::Instance()->Mark(bound.p, bound.rad, Colors::LightGreen);
}

Object::Object()
{
}

void Object::Render(ID3D11DeviceContext* dContext)const
{
	vs->Apply(dContext);
	hs->Apply(dContext);
	ds->Apply(dContext);
	gs->Apply(dContext);
	ps->Apply(dContext);

	dsState->Apply(dContext);
	blendState->Apply(dContext);
	rsState->Apply(dContext);

	m_mesh->Apply(dContext);
}
void Object::Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum* frustum, UINT sceneDepth) const
{
	if (!enabled || !show)
		return;

	if (IsInsideFrustum(frustum))
	{
		const SHADER_STD_TRANSF STransformation(transform->WorldMatrix(), v, p, 1,1000,XM_PIDIV2,1);

		vs->WriteCB(dContext,0, &STransformation);

		Render(dContext);
	}
}

void Object::RenderGeom(ID3D11DeviceContext* dContext) const
{
	if (!enabled || !show)
		return;

	m_mesh->Apply(dContext);
}

bool Object::IsInsideFrustum(const Frustum* frustum) const
{
	if (!frustum || frustum->skip)
		return true;

	return (
		IntersectInPlaneSphere(frustum->right, bound) &&
		IntersectInPlaneSphere(frustum->left, bound) &&
		IntersectInPlaneSphere(frustum->top, bound) &&
		IntersectInPlaneSphere(frustum->bottom, bound) &&
		IntersectInPlaneSphere(frustum->front, bound) &&
		IntersectInPlaneSphere(frustum->back, bound));
}

bool Object::IsPicking(Geometrics::Ray ray) const
{
	if(!m_collider)
		return IntersectRaySphere(ray, bound);

	XMFLOAT3 hit;
	return m_collider->IsHit(ray, &hit);
}