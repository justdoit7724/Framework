#pragma once
#include <string>
#include "DX_info.h"
#include "Network.h"
#include "Geometrics.h"
#include <unordered_set>

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
	Object(Shape* shape, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV);
	~Object();

	virtual void Update();
	virtual void Render(const XMMATRIX& parentWorld, const XMMATRIX& vp, UINT sceneDepth) const;
	virtual void RenderGeom() const;

	virtual bool IsInsideFrustum(const Frustum& frustum) const;
	virtual bool IsPicking(const Geometrics::Ray ray)const;
	virtual void UpdateBound();

	void Visualize() override;
	void SetEnabled(bool e) { enabled = e; }
	void SetShow(bool s) { show = s; }

	void AddChildren(Object* obj);

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

	const int zOrder;
	Geometrics::Sphere Bound() { return bound; }


protected:
	Object();
	void Render()const;

	bool enabled = true;
	bool show = true;

	XMMATRIX worldMat;
	XMMATRIX nMat;

	Geometrics::Sphere bound;

	std::unordered_set<Object*> children;
private:
};

