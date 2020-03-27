#pragma once

#include "DX_info.h"
#include "Geometrics.h"

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


class Object
{
public:
	Object(std::string name, std::shared_ptr<Shape> shape, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS);
	Object(std::string name, std::shared_ptr<Shape> shape, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV);
	virtual ~Object();

	virtual void Update();
	virtual void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const;
	virtual void RenderGeom() const;

	virtual bool IsPicking(const Math::Ray ray)const;
	virtual void UpdateBound();

	void SetEnabled(bool e) { enabled = e; }
	void SetShow(bool s) { show = s; }

	//TODO
	Transform* transform;
	std::shared_ptr < Shape> shape;
	VShader* vs;
	HShader* hs;
	DShader* ds;
	GShader* gs;
	PShader* ps;
	BlendState * blendState = nullptr;
	DepthStencilState * dsState = nullptr;
	RasterizerState* rsState = nullptr;

	const std::string name;
	Math::Sphere Bound() { return bound; }


protected:
	Object();
	void Render()const;
	virtual bool IsInsideFrustum(const Frustum& frustum) const;

	bool enabled = true;
	bool show = true;


	Math::Sphere bound;
};

