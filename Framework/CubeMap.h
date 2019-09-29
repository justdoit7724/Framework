#pragma once
#include "Object.h"

class CubeMap : public Object
{
public:
	CubeMap(ID3D11ShaderResourceView* srv);

	void Update(const Camera* camera, const XMMATRIX& texMat = XMMatrixIdentity())override;

private:
};

