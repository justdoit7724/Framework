#pragma once
#include <string>
#include "Transform.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "CustomFormat.h"
#include "Shader.h"
#include "Shape.h"

class Camera;

class Object
{
public:
	Object();
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, int zOrder);
	~Object();

	void EnableShadow(XMFLOAT3 shadowPlaneN, float shadowPlaneDist, float shadowTransparency);

	Transform* GetTransform(){return transform;}
	void Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat = XMMatrixIdentity());
	virtual void Render();

	//TODO
	Transform* transform;
	Shape* shape;
	VShader* vs;
	GShader* gs;
	PShader* ps;
	BlendState * blendState;
	DepthStencilState * dsState;


protected:

	XMMATRIX worldMat;
	XMMATRIX nMat;
	int zOrder;

	XMMATRIX dir_light_shadowMats[LIGHT_MAX_EACH];
	XMMATRIX pt_light_shadowMats[LIGHT_MAX_EACH];
	bool isShadow;
	XMFLOAT3 shadowPlaneNormal;
	float shadowPlaneDist;
	VShader* shadow_vs;
	PShader* shadow_ps;
	XMMATRIX vpMat;
};

