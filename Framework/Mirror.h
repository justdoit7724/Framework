#pragma once
#include "Object.h"

class Scene;

class Mirror :
	public Object
{
public:
	Mirror(Scene* captureScene, UINT width, UINT height);

	void Update()override;
	void Render(const Camera* camera, UINT sceneDepth)const override;

	Scene* const captureScene;
	const XMINT2 resolution;

private:
	void UpdatePerspective(const Camera* eye);

	Camera* perspective;

	ComPtr<ID3D11RenderTargetView> rtv;
	ComPtr<ID3D11DepthStencilView> dsv;
	ComPtr<ID3D11ShaderResourceView> srv;
	D3D11_VIEWPORT vp;
};

