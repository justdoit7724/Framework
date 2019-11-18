#pragma once
#include "Object.h"

class Skybox : public Object
{
public:
	Skybox(ID3D11ShaderResourceView* srv);

	void Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const override;

private:
};

