#pragma once
#include "Component.h"
#include <vector>

struct Vertex;

class Shape : public Component
{
public:
	Shape();
	void Apply()const;
	void Init(void* vertice, UINT vertByteSize, UINT vertexCount, void* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType, XMFLOAT3 minPt, XMFLOAT3 maxPt);

	void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY pt) { primitiveType = pt; }

	void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent);
	void GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt);
protected:
	void CalculateTangents(Vertex* vertice, const UINT* idx, UINT polyCount);

	UINT indexCount;

private:
	XMFLOAT3 lMinPt, lMaxPt;
	ComPtr<ID3D11Buffer> vertexBuffer=nullptr;
	ComPtr<ID3D11Buffer> indexBuffer=nullptr;
	UINT vertByteSize;
	D3D_PRIMITIVE_TOPOLOGY primitiveType;
};

