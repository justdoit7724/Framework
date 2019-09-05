#pragma once
#include "Shape.h"

// not light effect because of normal of vertice
class Hill :
	public Shape
{
public:
	Hill(ID3D11Device* device, ID3D11DeviceContext* dContext, int n, int m, XMFLOAT2 heightRange, ID3D11ShaderResourceView * heightMap);
	~Hill();


private:
};

