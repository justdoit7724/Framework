#pragma once
#include "Resource.h"
#include "Camera.h"

class Transform;
class Shape;
class VPShader;
class IGraphic;
struct VS_Property;
struct SHADER_DIRECTIONAL_LIGHT;
struct SHADER_POINT_LIGHT;
struct SHADER_SPOT_LIGHT;
struct ShaderMaterial;

class Object
{
public:
	Object(IGraphic* graphic, Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, std::string imageName, bool mipmap = true, int zOrder =Z_ORDER_STANDARD);
	~Object();

	Transform* GetTransform(){return transform;}
	void Update();
	void Render(IGraphic* graphic, Camera* camera, const SHADER_DIRECTIONAL_LIGHT* dLight, const SHADER_POINT_LIGHT* pLight, const SHADER_SPOT_LIGHT* sLight, const XMMATRIX& texMat);

	const bool isMipmap;
	const int zOrder;

private:

	Transform* transform;
	Shape* shape;
	VPShader* shader;
	ShaderMaterial* material;
	ConstantBuffer<VS_Property>* cb_vs_property;
	ConstantBuffer<SHADER_DIRECTIONAL_LIGHT>* cb_ps_dLights;
	ConstantBuffer<SHADER_POINT_LIGHT>* cb_ps_pLights;
	ConstantBuffer<SHADER_SPOT_LIGHT>* cb_ps_sLights;
	ConstantBuffer<XMFLOAT3>* cb_ps_eyePos;
	ConstantBuffer<ShaderMaterial>* cb_ps_material;
	ComPtr<ID3D11ShaderResourceView> bodySRV;
	ComPtr<ID3D11SamplerState> bodySameplerState;
};

