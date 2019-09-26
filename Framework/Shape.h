#pragma once
#include "Component.h"
#include <vector>

struct Vertex;

class Shape : public Component
{
public:
	Shape();
	void Apply()const;
	void Init(void* vertice, UINT vertByteSize, UINT vertexCount, void* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);

protected:
	ComPtr<ID3D11Buffer> vertexBuffer=nullptr;
	ComPtr<ID3D11Buffer> indexBuffer=nullptr;
	UINT indexCount;
	UINT vertByteSize;
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
};

