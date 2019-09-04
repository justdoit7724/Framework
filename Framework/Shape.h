#pragma once
#include "DX_info.h"
#include <vector>

struct Vertex;

class Shape
{
public:
	void Render(ID3D11DeviceContext* dContext);

protected:
	Shape();
	void Init(ID3D11Device* device, const Vertex* vertice, const int vertexCount, const UINT* indice, const int idxCount);

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount;
};

