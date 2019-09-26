#include "Object.h"
#include "TextureMgr.h"
#include "Light.h"
#include "Transform.h"
#include "Shader.h"
#include "BlendState.h"
#include "DepthStencilState.h"
#include "RasterizerState.h"
#include "Camera.h"
#include "Shape.h"

Object::Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, int zOrder)
	:zOrder(zOrder), shape(shape)
{
	transform = new Transform();

	vs = new VShader("StandardVS.cso", Std_ILayouts, ARRAYSIZE(Std_ILayouts));
	hs = new HShader();
	ds = new DShader();
	gs = new GShader();
	ps = new PShader("StandardPS.cso");

	vs->AddCB(0, 1, sizeof(VS_Property));
	ps->AddCB(0, 1, sizeof(SHADER_DIRECTIONAL_LIGHT));
	ps->AddCB(1, 1, sizeof(SHADER_POINT_LIGHT));
	ps->AddCB(2, 1, sizeof(SHADER_SPOT_LIGHT));
	ps->AddCB(3, 1, sizeof(XMFLOAT3));
	ps->AddCB(4, 1, sizeof(ShaderMaterial));
	ps->WriteCB(4,&ShaderMaterial(mDiffuse, 1, mAmbient, mSpec, sP, r));
	

	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));
	samplerDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	ps->AddSamp(0, 1, &samplerDesc);
	ps->AddSRV(0, 1, srv);

	blendState = new BlendState();
	dsState = new DepthStencilState();
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

	if (dsState)
		delete dsState;
	if (blendState)
		delete blendState;
}

void Object::Update(Camera* camera, const XMMATRIX& texMat)
{
	vpMat = camera->ViewMat()*camera->ProjMat(zOrder);
	XMMATRIX wMat = transform->WorldMatrix();

	vs->WriteCB(0,&VS_Property(wMat, vpMat, texMat));
	ps->WriteCB(0, (void*)DirectionalLight::Data());
	ps->WriteCB(1, (void*)PointLight::Data());
	ps->WriteCB(2, (void*)SpotLight::Data());
	ps->WriteCB(3, &(camera->Pos()));

	
}

void Object::Render()
{
	vs->Apply();
	hs->Apply();
	ds->Apply();
	gs->Apply();
	ps->Apply();

	// STATE
	if(dsState)
		dsState->Apply();
	if(blendState)
		blendState->Apply();
	shape->Apply();
}