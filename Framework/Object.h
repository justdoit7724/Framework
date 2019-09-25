#pragma once
#include <string>
#include "Transform.h"
#include "DepthStencilState.h"
#include "BlendState.h"
#include "CustomFormat.h"
#include "Shader.h"
#include "Shape.h"

class Camera;

class Object
{
public:
	Object() {}
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
	BlendState * blendState;
	DepthStencilState * dsState;

	XMMATRIX worldMat;
	XMMATRIX nMat;
	int zOrder;

	XMMATRIX vpMat;
};

