#pragma once
#include "Object.h"

class Scene;
class IGraphic;

class DynamicCubeMap :
	public Object
{
public:
	DynamicCubeMap(IGraphic* graphic, Scene* captureScene, Shape* shape);
	~DynamicCubeMap();

	void Update(const Camera* camera, float elapsed, const XMMATRIX& texMat = XMMatrixIdentity())override;

private:
	Scene* captureScene;
	IGraphic* graphic;

	ComPtr<ID3D11RenderTargetView> captureRTV[6];
	ComPtr<ID3D11ShaderResourceView> captureSRV;
	ComPtr<ID3D11DepthStencilView> captureDSV;
	D3D11_VIEWPORT captureViewport;
	Camera* captureCamera[6];
};

