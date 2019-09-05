#pragma once
#include <vector>
#include "DX_info.h"


class Camera;
class Object;
class Timer;
class IGraphic;
class DirectionalLight;
class PointLight;
class SpotLight;
class UICanvas;

//delete
#include "Resource.h"
struct SHADER_DIRECTIONAL_LIGHT;
struct SHADER_POINT_LIGHT;
struct SHADER_SPOT_LIGHT;
struct ShaderMaterial;
struct VS_Property;
class Hill;
class Sphere;
class VPShader;

class Scene
{
public:
	Scene(IGraphic* graphic);
	~Scene();

	void Update();
	void Render(IGraphic* graphic);

private:

	Camera* camera;

	UICanvas* canvas;

	std::vector<Object*> objs;

	DirectionalLight* dLight;

	//delete
	Hill* hill;
	Sphere* sphere;
	Object* temp;
	VPShader* shader;
	ShaderMaterial* material;
	ConstantBuffer<VS_Property>* cb_vs_property;
	ConstantBuffer<SHADER_DIRECTIONAL_LIGHT>* cb_ps_dLights;
	ConstantBuffer<SHADER_POINT_LIGHT>* cb_ps_pLights;
	ConstantBuffer<SHADER_SPOT_LIGHT>* cb_ps_sLights;
	ConstantBuffer<XMFLOAT3>* cb_ps_eyePos;
	ConstantBuffer<ShaderMaterial>* cb_ps_material;
	ID3D11ShaderResourceView* bodySrv;
	ID3D11SamplerState* samplerState;
};

