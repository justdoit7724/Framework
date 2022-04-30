#pragma once

#include "Component.h"

namespace DX {
	class Vertice;


	class DXLIB_DLL Mesh : public Component
	{
	public:
		Mesh(ID3D11Device* device, Vertice vertice, const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);
		~Mesh();
		virtual void Apply(ID3D11DeviceContext* dContext)const;

		void CalculateTangent(XMFLOAT3 v0, XMFLOAT3 v1, XMFLOAT3 v2, XMFLOAT2 t0, XMFLOAT2 t1, XMFLOAT2 t2, OUT XMFLOAT3* tangent);
		void GetLBound(OUT XMFLOAT3* minPt, OUT XMFLOAT3* maxPt);
		void SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY p) { m_primitiveType = p; }
		D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() { return m_primitiveType; }
	protected:
		Mesh() {}
		void Init(ID3D11Device* device, Vertice vertice , const UINT* indice, UINT idxCount, D3D_PRIMITIVE_TOPOLOGY primitiveType);

		UINT m_idxCount;

	private:

		XMFLOAT3 m_lMinPt, m_lMaxPt;
		ID3D11Buffer* m_vertexBuffer = nullptr;
		ID3D11Buffer* m_indexBuffer = nullptr;
		D3D11_PRIMITIVE_TOPOLOGY m_primitiveType;

		Vertice* m_vertice;
	};
}
