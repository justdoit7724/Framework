#pragma once
#include "Shape.h"

// not light effect because of normal of vertice
class IGraphic;
class Hill :
	public Shape
{
public:
	Hill(int n, int m, XMFLOAT2 heightRange, ID3D11ShaderResourceView *const* heightMap);
	~Hill();

	void Apply()override;
private:
};

