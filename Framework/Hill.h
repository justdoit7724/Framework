#pragma once
#include "Shape.h"

// not light effect because of normal of vertice
class IGraphic;
class Hill :
	public Shape
{
public:
	Hill(IGraphic* graphic, int n, int m, XMFLOAT2 heightRange, ID3D11ShaderResourceView *const* heightMap);
	~Hill();

	void Apply(ID3D11DeviceContext* dContext)override;
private:
};

