#pragma once
#include "Component.h"
#include <vector>

struct Vertex;

class Shape : public Component
{
public:
	Shape(const Vertex* vertice, UINT vertByteSize, UINT vertexCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);
	void Apply()const;

	void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent);
	void GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt);
protected:
	Shape() {}
	void Init(const Vertex* vertice, UINT vertByteSize, UINT vertexCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);
	void CalculateTangents(Vertex* vertice, const UINT* idx, UINT polyCount);

	UINT idxCount;

private:
	XMFLOAT3 lMinPt, lMaxPt;
	ComPtr<ID3D11Buffer> vertexBuffer=nullptr;
	ComPtr<ID3D11Buffer> indexBuffer=nullptr;
	UINT vertByteSize;
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
};

