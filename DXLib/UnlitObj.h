#pragma once

#include "Object.h"


namespace DX {
	class DXLIB_DLL UnlitObj : public Object
	{
	public:
		UnlitObj(ID3D11Device* device, ID3D11DeviceContext* dContext, std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, XMVECTOR color, bool directRender=true);

		void Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum* frustum, UINT sceneDepth=NULL) const override;

	private:
		UnlitObj() = delete;

		XMVECTOR color;

	};
}

