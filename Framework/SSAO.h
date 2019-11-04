#pragma once
#include "DX_info.h"

class VShader;
class PShader;
class Shape;
class DepthStencilState;
class BlendState;
class Camera;
class Object;

class SSAO
{
public:
	SSAO();
	~SSAO();

	void Update(const Camera* camera, const std::vector<Object*>& objs);
	
private:
	void DrawNormalDepth(const Camera* camera, const std::vector<Object*>& objs);
	void DrawAccess(const XMMATRIX& p);
	void BlurHorizon();
	void BlurVertical();
	bool isFinishDrawing = true;

	ComPtr<ID3D11RenderTargetView> ndRTV;
	ComPtr<ID3D11ShaderResourceView> ndSRV;
	ComPtr<ID3D11DepthStencilView> dsv;
	D3D11_VIEWPORT vp;

	ComPtr<ID3D11RenderTargetView> acsRTV;
	ComPtr<ID3D11ShaderResourceView> acsSRV;

	ComPtr<ID3D11RenderTargetView> blurRTV;
	ComPtr<ID3D11ShaderResourceView> blurSRV;

	ComPtr<ID3D11RenderTargetView> blur2RTV;
	ComPtr<ID3D11ShaderResourceView> finalSRV;

	ComPtr<ID3D11Buffer> vb;

	VShader *ndVS, *acsVS, *blurVS;
	PShader* ndPS, * acsPS, * blurPS;

	DepthStencilState* dsState;
	DepthStencilState* noDsState;
	BlendState* blendState;

	ID3D11RenderTargetView* oriPassRTV;
	ID3D11DepthStencilView* oriDSV;
	D3D11_VIEWPORT oriVP;

	struct Vertex_AO
	{
		XMFLOAT3 pPos;
		float farPlaneIdx;
		XMFLOAT2 tex;
		Vertex_AO(XMFLOAT3 p, float i, XMFLOAT2 t) :pPos(p), farPlaneIdx(i), tex(t) {}
	};

	XMFLOAT4 sample[14];
	XMFLOAT4 vFarPlane[4];

	const float defaultColor[4] = { 1,1,1,10000 };
};

