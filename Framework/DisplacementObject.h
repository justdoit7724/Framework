#pragma once
#include "Object.h"
class DisplacementObject :
	public Object
{
public:
	DisplacementObject(Shape* shape, ID3D11ShaderResourceView* bodyTex, ID3D11ShaderResourceView* normalTex, ID3D11ShaderResourceView* bumpTex, float dp_scale,
		XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sp);

	void Render(const XMMATRIX& vp, XMFLOAT3 eye, UINT sceneDepth)const override;

private:
	ID3D11ShaderResourceView* body_srv;
	ID3D11ShaderResourceView* normal_srv;
	ID3D11ShaderResourceView* dpSRV;
	float dp_scale;

};

