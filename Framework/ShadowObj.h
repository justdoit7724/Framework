#pragma once
#include "Object.h"
class ShadowObj :
	public Object
{
public:
	ShadowObj(Shape* shape, ID3D11ShaderResourceView* bodySRV, ID3D11ShaderResourceView* bodyNormal, int zOrder);

	void Update(const Camera* camera, float elapsed, const XMMATRIX& dShadowVP, const XMMATRIX& pShadowV);
};

