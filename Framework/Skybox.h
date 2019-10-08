#pragma once
#include "Object.h"

class Skybox : public Object
{
public:
	Skybox(ID3D11ShaderResourceView* srv);

	void Update(const Camera* camera, float elapsed, const XMMATRIX& texMat = XMMatrixIdentity())override;

private:
};

