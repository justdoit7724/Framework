#pragma once
#include "Scene.h"
#include <vector>

class Object;
class Timer;
class DirectionalLight;
class PointLight;
class SpotLight;
class UICanvas;
class IGraphic;
class Timer;
class VShader;
class PShader;
class DepthStencilState;
class BlendState;

class TestScene :
	public Scene
{
public:
	TestScene(IGraphic* graphic);
	~TestScene();
	void Update(float elapsed, float spf)override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

private:

	IGraphic* graphic;
	Timer* timer;

	UICanvas* canvas;

	DirectionalLight* dLight = nullptr;
	PointLight* pLight = nullptr;
	PointLight* pLight2 = nullptr;

	//
	ID3D11ShaderResourceView* aoNDSRV;
	ID3D11RenderTargetView* aoNDRTV;
	ID3D11DepthStencilView* aoNDDSV;
	ID3D11ShaderResourceView* aoSRV;
	ID3D11RenderTargetView* aoRTV;
	ID3D11ShaderResourceView* aoBlurSRV;
	ID3D11RenderTargetView* aoBlurRTV;
	ID3D11ShaderResourceView* aoBlur2SRV;
	ID3D11RenderTargetView* aoBlur2RTV;
	VShader* aoMapVS;
	PShader* aoMapPS;
	VShader* aoVS;
	PShader* aoPS;
	VShader* aoBlurVS;
	PShader* aoBlurPS;
	DepthStencilState* dsState;
	DepthStencilState* noDsState;
	BlendState* blendState;
	D3D11_VIEWPORT aoVp;
	Object* flor;
};

