#pragma once

#include "Object.h"

class Buffer;

namespace DX {
	class Skybox : public Object
	{
	public:
		Skybox();
		~Skybox();

		void Mapping()const;
		void Update() override {};
		void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth)const override;
		void RenderGeom() const override {};

	private:
		void Blur()const;
		bool IsInsideFrustum(const Frustum& frustum) const override { return true; }
		ID3D11SamplerState* samplerState;

		ID3D11ShaderResourceView* blur1SRV, * blur2SRV;
		ID3D11RenderTargetView* blur1RTV, * blur2RTV;
		//ID3D11DepthStencilView> 
	};
}

