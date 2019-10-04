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

	void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent);
protected:
	void CalculateTangents(Vertex* vertice, const UINT* idx, UINT polyCount);

	ComPtr<ID3D11Buffer> vertexBuffer=nullptr;
	ComPtr<ID3D11Buffer> indexBuffer=nullptr;
	UINT indexCount;
	UINT vertByteSize;
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
};

