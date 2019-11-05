#include "Object.h"
#include "ShaderFormat.h"
#include "Camera.h"
#include "TextureMgr.h"
#include "Transform.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Shape.h"
#include "CameraMgr.h"

#include "Debugging.h"

//fundamental elements
Object::Object(Shape* shape, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS,int zOrder)
	:shape(shape), zOrder(zOrder)
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
Object::Object(Shape* shape, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV)
	:zOrder(Z_ORDER_STANDARD), shape(shape)
{
	transform = new Transform();
	vs = new VShader("StandardVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("StandardPS.cso");

	vs->AddCB(0, 1, sizeof(SHADER_STD_TRANSF));
	ps->AddCB(3, 1, sizeof(XMFLOAT4));
	ps->AddCB(4, 1, sizeof(SHADER_MATERIAL));
	ps->AddCB(5, 1, sizeof(float));
	ps->WriteCB(4,&SHADER_MATERIAL(XMFLOAT3(1,1,1), 1, XMFLOAT3(0.2, 0.2, 0.2), XMFLOAT3(1, 1, 1), 4, XMFLOAT3(0,0,0)));
	

	D3D11_SAMPLER_DESC cmSamp_desc;
	ZeroMemory(&cmSamp_desc, sizeof(D3D11_SAMPLER_DESC));
	cmSamp_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	cmSamp_desc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
	cmSamp_desc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
	cmSamp_desc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
	cmSamp_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	float borderC[4] = { 0,0,1,1 };
	cmSamp_desc.BorderColor[0] = 0;
	cmSamp_desc.BorderColor[1] = 1;
	cmSamp_desc.BorderColor[2] = 0;
	cmSamp_desc.BorderColor[3] = 1;
	cmSamp_desc.MinLOD = 0;
	cmSamp_desc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &cmSamp_desc);
	D3D11_SAMPLER_DESC body_desc;
	ZeroMemory(&body_desc, sizeof(D3D11_SAMPLER_DESC));
	body_desc.Filter = D3D11_FILTER_ANISOTROPIC;
	body_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	body_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	body_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	body_desc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	body_desc.MinLOD = 0;
	body_desc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(1, 1, &body_desc);
	ps->AddSRV(0, 1);
	ps->AddSRV(1, 1);
	ps->AddSRV(2, 1);
	ps->WriteSRV(0, nullptr);
	ps->WriteSRV(1, diffSRV);
	ps->WriteSRV(2, normalSRV);

	blendState = new BlendState(nullptr);
	dsState = new DepthStencilState(nullptr);
	rsState = new RasterizerState(nullptr);
}

Object::~Object()
{
	delete transform;
	delete shape;
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
	XMFLOAT3 boundlMinPt;
	XMFLOAT3 boundlMaxPt;
	shape->GetLBound(&boundlMinPt, &boundlMaxPt);
	XMFLOAT3 wMinPt = boundlMinPt * transform->GetScale();
	XMFLOAT3 wMaxPt = boundlMaxPt * transform->GetScale();
	bound.p = transform->GetPos();
	bound.rad = Length(wMinPt - wMaxPt) * 0.5f;
}

Object::Object()
	:zOrder(Z_ORDER_STANDARD)
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
void Object::Render(const Camera* camera, UINT sceneDepth) const
{
	const SHADER_STD_TRANSF STransformation(transform->WorldMatrix(), camera->VMat() * camera->ProjMat(zOrder), XMMatrixIdentity());

	XMFLOAT3 eye = camera->transform->GetPos();

	vs->WriteCB(0, (void*)(&STransformation));
	ps->WriteCB(3, &XMFLOAT4(eye.x, eye.y, eye.z, 0));

	Render();
}

void Object::RenderGeom() const
{
	shape->Apply();
}

bool Object::IsInsideFrustum(const Frustum* frustum) const
{
	return (
		IntersectInPlaneSphere(frustum->sidePt, frustum->rN, bound) &&
		IntersectInPlaneSphere(frustum->sidePt, frustum->lN, bound) &&
		IntersectInPlaneSphere(frustum->sidePt, frustum->tN, bound) &&
		IntersectInPlaneSphere(frustum->sidePt, frustum->bN, bound) &&
		IntersectInPlaneSphere(frustum->fPt, frustum->fN, bound) &&
		IntersectInPlaneSphere(frustum->nPt, frustum->nN, bound));
}

void Object::Visualize()
{
	if(IsInsideFrustum(CameraMgr::Instance()->Main()->GetFrustum()))
		Debugging::Instance()->Mark(bound.p, bound.rad, Colors::LightGreen);
}

