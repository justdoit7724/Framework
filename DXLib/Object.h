#pragma once


#include "LayerMask.h"
#include "Network.h"
#include "Geometrics.h"

namespace DX {

struct Frustum;
class Transform;
class Mesh;
class Collider;
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
		Object(std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, std::string sVS, const D3D11_INPUT_ELEMENT_DESC* iLayouts, UINT layoutCount, std::string sHS, std::string sDS, std::string sGS, std::string sPS, bool directRender = true);
		Object(std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, ID3D11ShaderResourceView* diffSRV, ID3D11ShaderResourceView* normalSRV = nullptr, bool directRender = true);
		virtual ~Object();

		virtual void Update();
		virtual void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const;
		virtual void RenderGeom() const;

		virtual bool IsPicking(Geometrics::Ray ray)const;
		virtual void UpdateBound();
		virtual void UpdateCollider();

		void SetEnabled(bool e) { enabled = e; }
		void SetShow(bool s) { show = s; }

		const std::string name;
		Geometrics::Sphere Bound() { return bound; }
		int Layer()const { return layer; }
		void SetLayer(int l) { layer = l; }
		Transform* transform;
		std::shared_ptr <Mesh> mesh;
		std::shared_ptr <Collider> collider;
		VShader* vs;
		HShader* hs;
		DShader* ds;
		GShader* gs;
		PShader* ps;
		BlendState* blendState = nullptr;
		DepthStencilState* dsState = nullptr;
		RasterizerState* rsState = nullptr;

		void Visualize() override;

	protected:
		Object();
		void Render()const;
		virtual bool IsInsideFrustum(const Frustum& frustum) const;


		bool enabled = true;
		bool show = true;
		int layer = LAYER_STD;

		Geometrics::Sphere bound;
	};
}

