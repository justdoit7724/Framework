#pragma once
#include "Shape.h"

class Hill :
	public Shape
{
public:
	Hill(int n, int m, ID3D11ShaderResourceView *const* heightMap);
	~Hill();

	void Apply()override;
private:
};

