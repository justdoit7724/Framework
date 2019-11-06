#pragma once
#include "Object.h"

class Scene;

class Glass :
	public Object
{
public:
	Glass(Scene* captureScene, Shape* shape);
	~Glass();

	void Render(const Camera* camera, UINT sceneDepth)const override;

private:
	Scene* captureScene;

	ComPtr<ID3D11RenderTargetView> captureRTV[6];
	ComPtr<ID3D11ShaderResourceView> captureSRV;
	ComPtr<ID3D11DepthStencilView> captureDSV;
	D3D11_VIEWPORT captureViewport;
	Camera* captureCamera[6];
};

