#pragma once
#include "Object.h"
class PT_Obj :
	public Object
{
public:
	PT_Obj(Shape* shape, const XMMATRIX& vp_mat, XMFLOAT3 dif, float transp, XMFLOAT3 amb, XMFLOAT3 spec, float sp, XMFLOAT3 refl, ID3D11ShaderResourceView* bodySRV, ID3D11ShaderResourceView* bodyNorm, ID3D11ShaderResourceView* ptSRV, ID3D11ShaderResourceView* cmSRV, int zOrder);

	void Update(const Camera* camera, float elapsed, const XMMATRIX& texMat = XMMatrixIdentity())override;

private:
	XMMATRIX vp_mat;
};
