#pragma once
#include "Shape.h"

class Hill :
	public Shape
{
public:
	Hill(int n, int m, XMFLOAT2 heightRange, ID3D11ShaderResourceView *const* heightMap);
	~Hill();

	void Apply()const override;
private:
};

