#pragma once
#include "Object.h"
class PlaneShadow :
	public Object
{
public:
	PlaneShadow(Shape* shape, XMFLOAT3 planeN, float planeD);

	void Update(Camera* camera, const XMMATRIX& texMat)override;
	void Render() const override;
private:

	XMMATRIX vpMat;

	XMMATRIX dir_light_shadowMats[LIGHT_MAX_EACH];
	XMMATRIX pt_light_shadowMats[LIGHT_MAX_EACH];
	XMFLOAT3 planeNormal;
	float planeDist;
};

