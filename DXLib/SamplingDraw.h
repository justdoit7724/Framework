#pragma once

#include "Object.h"

namespace DX
{
	class DXLIB_DLL SamplingDraw : public Object
	{
	public:
		SamplingDraw(ID3D11Device* device, ID3D11DeviceContext* dContext);
		~SamplingDraw();

		void SetResolution(int res);

		void Render(ID3D11DeviceContext* dContext, const XMMATRIX& v, const XMMATRIX& p, const Frustum& frustum, UINT sceneDepth)const override;

	private:
		XMFLOAT4 m_sampleInfo;
	};
}