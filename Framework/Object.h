#pragma once
#include "Camera.h"
#include <string>
#include "Transform.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "CustomFormat.h"
#include "Shader.h"
#include "Shape.h"
#include "Buffer.h"

class Shape;

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
	VPShader* shader;
	ShaderMaterial* material;
	XMMATRIX worldMat;
	XMMATRIX nMat;
	std::unique_ptr<Buffer> cb_vs_property;
	std::unique_ptr<Buffer> cb_ps_dLights;
	std::unique_ptr<Buffer> cb_ps_pLights;
	std::unique_ptr<Buffer> cb_ps_sLights;
	std::unique_ptr<Buffer> cb_ps_eyePos;
	std::unique_ptr<Buffer> cb_ps_material;
	ComPtr<ID3D11ShaderResourceView> bodySRV;
	ComPtr<ID3D11SamplerState> bodySameplerState;
	BlendState * blendState;
	DepthStencilState * dsState;

	XMMATRIX dir_light_shadowMats[LIGHT_MAX_EACH];
	XMMATRIX pt_light_shadowMats[LIGHT_MAX_EACH];
	bool isShadow;
	XMFLOAT3 shadowPlaneNormal;
	float shadowPlaneDist;
	VPShader* shadow_Shader;
	Buffer* cb_vs_shadow_property;
	Buffer* cb_ps_shadow_transparency;
	XMMATRIX vpMat;
};

