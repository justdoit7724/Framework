#pragma once
#include "Object.h"

class IGraphic;

class DynamicCubeMap :
	public Object
{
public:
	DynamicCubeMap(IGraphic* graphic, Shape* shape);

	void Update(Camera* camera, const XMMATRIX& texMat = XMMatrixIdentity())override;

private:
	IGraphic* graphic;

	ID3D11Texture2D* captureTex;
	ID3D11RenderTargetView* captureRTV[6];
	ID3D11ShaderResourceView* captureSRV;
	ID3D11DepthStencilView* captureDSV;
	D3D11_VIEWPORT captureViewport;
	Camera* captureCamera[6];
};

