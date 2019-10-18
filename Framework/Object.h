#pragma once
#include <string>
#include "DX_info.h"

class Camera;
class Transform;
class Shape;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class BlendState;
class DepthStencilState;
class RasterizerState;

class Object
{
public:
	Object(Shape* shape, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS, int zOrder);
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, ID3D11ShaderResourceView* normalSRV, ID3D11ShaderResourceView* cm, int zOrder);
	~Object();

	virtual void Update(const Camera* camera, float elapsed, const XMMATRIX& texMat = XMMatrixIdentity());
	virtual void Render() const;
	void RenderGeom() const;

	//TODO
	Transform* transform;
	Shape* shape;
	VShader* vs;
	HShader* hs;
	DShader* ds;
	GShader* gs;
	PShader* ps;
	BlendState * blendState = nullptr;
	DepthStencilState * dsState = nullptr;
	RasterizerState* rsState = nullptr;

protected:

	XMMATRIX worldMat;
	XMMATRIX nMat;
	int zOrder;
};

