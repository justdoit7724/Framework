#pragma once

#include "Object.h"


namespace DX {
	class UnlitObj : public Object
	{
	public:
		UnlitObj(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, XMVECTOR color, bool directRender);

		void Render(ID3D11DeviceContext* dContext, const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const override;

	private:
		UnlitObj() = delete;

		XMVECTOR color;

	};
}

