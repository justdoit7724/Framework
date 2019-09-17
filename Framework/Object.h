#pragma once
#include "Camera.h"
#include <string>
#include "Transform.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "CustomFormat.h"
#include "Shape.h"

class Shape;
class VShader;
class PShader;

class Object
{
public:
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string imageName, bool mipmap = true, int zOrder =Z_ORDER_STANDARD);
	~Object();

	void EnableShadow(XMFLOAT3 shadowPlaneN, float shadowPlaneDist, float shadowTransparency);

	Transform* GetTransform(){return transform;}
	virtual void Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat = XMMatrixIdentity());
	virtual void Render();

	void SetTransparency(float t);
	DepthStencilState * GetDepthStencilState(){return dsState;}
	BlendState* GetBlendState(){return blendState;}

	const bool isMipmap;
	const int zOrder;

protected:

	Transform* transform;
	Shape* shape;
	VShader* vs;
	PShader* ps;
	ShaderMaterial* material;
	XMMATRIX worldMat;
	XMMATRIX nMat;
	BlendState * blendState;
	DepthStencilState * dsState;

	XMMATRIX dir_light_shadowMats[LIGHT_MAX_EACH];
	XMMATRIX pt_light_shadowMats[LIGHT_MAX_EACH];
	bool isShadow;
	XMFLOAT3 shadowPlaneNormal;
	float shadowPlaneDist;
	VShader* shadow_vs;
	PShader* shadow_ps;
	XMMATRIX vpMat;
};

