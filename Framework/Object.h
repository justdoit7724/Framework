#pragma once
#include <string>
#include "CustomFormat.h"


class Transform;
class Shape;
class VShader;
class HShader;
class DShader;
class GShader;
class PShader;
class BlendState;
class DepthStencilState;
class RasterizerState;
class Camera;

class Object
{
public:
	Object();
	Object(Shape* shape, XMFLOAT3 mDiffuse, XMFLOAT3 mAmbient, XMFLOAT3 mSpec, float sP, XMFLOAT3 r, ID3D11ShaderResourceView* srv, int zOrder);
	~Object();

	virtual void Update(Camera* camera, const XMMATRIX& texMat = XMMatrixIdentity());
	virtual void Render();



protected:
	Transform* transform;
	Shape* shape;
	VShader* vs;
	HShader* hs;
	DShader* ds;
	GShader* gs;
	PShader* ps;
	BlendState * blendState = nullptr;
	DepthStencilState * dsState = nullptr;
	RasterizerState* rsState = nullptr;

	XMMATRIX worldMat;
	XMMATRIX nMat;
	int zOrder;

	XMMATRIX vpMat;
};

