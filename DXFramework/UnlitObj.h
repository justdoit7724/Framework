#pragma once

#include "Object.h"

class UnlitObj : public Object
{
public:
	UnlitObj(std::string name, std::shared_ptr<Mesh> shape, std::shared_ptr<Collider> collider, XMVECTOR color, bool directRender);

	void Render(const XMMATRIX& vp, const Frustum& frustum, UINT sceneDepth) const override;

private:
	UnlitObj() {};

	XMVECTOR color;

};

