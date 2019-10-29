#pragma once
#include "DX_info.h"
#include "Object.h"

class nanosuit : public Object
{
public:
	nanosuit();
	void Update()override;
	void Render(const Camera* camera, UINT sceneDepth) const override;
	bool IsInsideFrustum(const Frustum* frustum) const override {
		return true;
	}

	void SetRot(XMFLOAT3 rot);
	void SetPos(XMFLOAT3 pos);
	void SetScale(XMFLOAT3 scale);
	std::vector<Object*> objs;
private:
	Object::transform;
};

