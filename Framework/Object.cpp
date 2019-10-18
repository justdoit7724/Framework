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
Object::Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, ID3D11ShaderResourceView* normalSRV, ID3D11ShaderResourceView* cm, int zOrder)
	:zOrder(zOrder), shape(shape)
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
	ps->WriteCB(4,&SHADER_MATERIAL(mDiffuse, 1, mAmbient, mSpec, sP, r));
	

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &samplerDesc);
	ps->AddSamp(1, 1, &samplerDesc);
	ps->AddSRV(0, 1);
	ps->AddSRV(1, 1);
	ps->AddSRV(2, 1);
	ps->WriteSRV(0, cm);
	ps->WriteSRV(1, srv);
	ID3D11ShaderResourceView* modNormalSRV= normalSRV;
	if (normalSRV == nullptr)
	{
		TextureMgr::Instance()->Load(KEY_TEXTURE_NORMAL_DEFAULT, FN_TEXTURE_NORMAL_DEFAULT, 1);
		UINT count;
		TextureMgr::Instance()->Get(KEY_TEXTURE_NORMAL_DEFAULT, &modNormalSRV, &count);
	}

	ps->WriteSRV(2, modNormalSRV);

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

void Object::Update(const Camera* camera, float elapsed, const XMMATRIX& texMat)
{
	const SHADER_STD_TRANSF STransformation(transform->WorldMatrix(), camera->VPMat(zOrder), texMat);

	XMFLOAT3 eye = camera->GetPos();

	vs->WriteCB(0, (void*)(&STransformation));
	ps->WriteCB(3, &XMFLOAT4(eye.x, eye.y, eye.z,0));
	ps->WriteCB(5, &elapsed);
}

void Object::Render() const
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

void Object::RenderGeom() const
{
	dsState->Apply();
	blendState->Apply();
	rsState->Apply();

	shape->Apply();
}
