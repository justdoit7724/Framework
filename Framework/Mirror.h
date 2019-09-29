#pragma once
#include "Object.h"

class Scene;

class Mirror :
	public Object
{
public:
	Mirror(Scene* captureScene, UINT width, UINT height);

	void Update(const Camera* camera, const XMMATRIX& texMat = XMMatrixIdentity()) override;

	Scene* const captureScene;

private:
	void UpdatePerspective(const Camera* eye);

	Camera* perspective;

	ComPtr<ID3D11RenderTargetView> rtv[6];
	ComPtr<ID3D11ShaderResourceView> srv;
};

