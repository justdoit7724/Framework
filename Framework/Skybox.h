#pragma once
#include "Object.h"

class Skybox : public Object
{
public:
	Skybox(ID3D11ShaderResourceView* srv);

	void Render(const Camera* camera, UINT sceneDepth)const override;

private:
};

