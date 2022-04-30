
#include "pch.h"

#include "QuadMesh.h"
#include "ShaderFormat.h"
#include "Math.h"
#include "Vertex.h"

using namespace DX;

QuadMesh::QuadMesh(ID3D11Device* device, const VertexLayout* layout)
	: Mesh()
{

	Vertice OBJ_QUAD(*layout);
	OBJ_QUAD.EmplaceBack();
	OBJ_QUAD.EmplaceBack();
	OBJ_QUAD.EmplaceBack();
	OBJ_QUAD.EmplaceBack();

	OBJ_QUAD[0].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, 0);
	OBJ_QUAD[1].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, 0);
	OBJ_QUAD[2].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, 0);
	OBJ_QUAD[3].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, 0);

	if (layout->Resolve<VertexLayout::ElementType::Texture2D>())
	{
		OBJ_QUAD[0].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_QUAD[1].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_QUAD[2].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_QUAD[3].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
	}
	if (layout->Resolve<VertexLayout::ElementType::Normal>())
	{
		OBJ_QUAD[0].Attr<VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_QUAD[1].Attr<VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_QUAD[2].Attr<VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_QUAD[3].Attr<VertexLayout::ElementType::Normal>() = FORWARD;
	}
	
	UINT OBJ_QUAD_INDICE[6] =
	{
		3,2,1,
		3,1,0
	};



	Init(device, OBJ_QUAD, OBJ_QUAD_INDICE, ARRAYSIZE(OBJ_QUAD_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

