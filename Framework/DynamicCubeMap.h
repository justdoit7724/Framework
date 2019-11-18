#pragma once
#include "Object.h"

class Scene;
class Camera;

class DynamicCubeMap :
	public Object
{
public:
	DynamicCubeMap(Scene* captureScene, Shape* shape);
	~DynamicCubeMap();

	void Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const override;

private:
	Scene* captureScene;

	ComPtr<ID3D11RenderTargetView> captureRTV[6];
	ComPtr<ID3D11ShaderResourceView> captureSRV;
	ComPtr<ID3D11DepthStencilView> captureDSV;
	D3D11_VIEWPORT captureViewport;
	Camera* captureCamera[6];
};

