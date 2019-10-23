#pragma once
#include <string>
#include "DX_info.h"
#include "Network.h"

class Camera;
struct Frustum;
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

class Object : public IDebug
{
public:
	Object(Shape* shape, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS, int zOrder);
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, ID3D11ShaderResourceView* normalSRV, ID3D11ShaderResourceView* cm, int zOrder);
	~Object();

	virtual void Update();
	virtual void Render(const Camera* camera, UINT sceneDepth) const;
	void RenderGeom() const;
	virtual bool IsInsideFrustum(const Frustum* frustum) const;
	void Visualize() override;

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

	void Render()const;

	XMMATRIX worldMat;
	XMMATRIX nMat;
	int zOrder;

	float boundRad;
	XMFLOAT3 boundlMinPt;
	XMFLOAT3 boundlMaxPt;
};

