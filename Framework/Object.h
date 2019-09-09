#pragma once
#include "Camera.h"
#include "DX_info.h"

class Transform;
class Shape;
class VPShader;

class DepthStencilState;
class BlendState;
class Buffer;
class RasterizerState;
struct VS_Property;
struct SHADER_DIRECTIONAL_LIGHT;
struct SHADER_POINT_LIGHT;
struct SHADER_SPOT_LIGHT;
struct ShaderMaterial;

class Object
{
public:
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string imageName, bool mipmap = true, int zOrder =Z_ORDER_STANDARD);
	~Object();

	Transform* GetTransform(){return transform;}
	void Update(Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat = XMMatrixIdentity());
	void Render();

	void SetTransparency(float t);
	DepthStencilState * GetDepthStencilState(){return dsState;}
	BlendState* GetBlendState(){return blendState;}

	const bool isMipmap;
	const int zOrder;

private:

	Transform* transform;
	Shape* shape;
	VPShader* shader;
	ShaderMaterial* material;
	Buffer* cb_vs_property;
	Buffer* cb_ps_dLights;
	Buffer* cb_ps_pLights;
	Buffer* cb_ps_sLights;
	Buffer* cb_ps_eyePos;
	Buffer* cb_ps_material;
	ComPtr<ID3D11ShaderResourceView> bodySRV;
	ComPtr<ID3D11SamplerState> bodySameplerState;
	BlendState* blendState;
	DepthStencilState * dsState;
	RasterizerState* rasterizerState;
};

