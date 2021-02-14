#pragma once

#include "Component.h"

#define MESH_MAX 64

namespace DX {
	struct Vertex;

	class DXLIB_DLL Mesh : public Component
	{
	public:
		Mesh(ID3D11Device* device, Vertex* vertice, UINT vertByteSize, UINT vertexCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);
		~Mesh();
		virtual void Apply(ID3D11DeviceContext* dContext)const;

		void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent);
		void GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt);
		void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY p) { primitiveType = p; }
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() { return primitiveType; }
	protected:
		Mesh() {}
		void Init(ID3D11Device* device, Vertex* vertice, UINT vertByteSize, UINT vertexCount, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);

		UINT idxCount;

	private:

		XMFLOAT3 lMinPt, lMaxPt;
		ID3D11Buffer* vertexBuffer = nullptr;
		ID3D11Buffer* indexBuffer = nullptr;
		UINT vertByteSize;
		D3D11_PRIMITIVE_TOPOLOGY primitiveType;
	};
}
