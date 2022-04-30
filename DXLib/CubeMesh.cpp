#include "pch.h"
#include "CubeMesh.h"

#include "ShaderFormat.h"
#include "Math.h"

using namespace DX;

static const UINT OBJ_CUBE_INDICE[36] =
{
	0,1,2,
	0,2,3,
	4,5,6,
	4,6,7,
	8,9,10,
	8,10,11,
	12,13,14,
	12,14,15,
	16,17,18,
	16,18,19,
	20,21,22,
	20,22,23
};

DX::CubeMesh::CubeMesh(ID3D11Device* device, const VertexLayout* layout)
{
	Vertice OBJ_CUBE(*layout);
	
	for (int i = 0; i < 30; ++i)
	{
		OBJ_CUBE.EmplaceBack();
	}
	if (layout->Resolve<VertexLayout::ElementType::Position3D>())
	{
		OBJ_CUBE[0].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, -0.5);
		OBJ_CUBE[1].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, -0.5);
		OBJ_CUBE[2].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, -0.5);
		OBJ_CUBE[3].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, -0.5);
		OBJ_CUBE[4].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, -0.5);
		OBJ_CUBE[5].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, -0.5);
		OBJ_CUBE[6].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, 0.5);
		OBJ_CUBE[7].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, 0.5);
		OBJ_CUBE[8].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, 0.5);
		OBJ_CUBE[9].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, 0.5);
		OBJ_CUBE[10].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, 0.5);
		OBJ_CUBE[11].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, 0.5);
		OBJ_CUBE[12].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, 0.5);
		OBJ_CUBE[13].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, 0.5);
		OBJ_CUBE[14].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, -0.5);
		OBJ_CUBE[15].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, -0.5);
		OBJ_CUBE[16].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, -0.5);
		OBJ_CUBE[17].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, 0.5, 0.5);
		OBJ_CUBE[18].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, 0.5);
		OBJ_CUBE[19].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, 0.5, -0.5);
		OBJ_CUBE[20].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, 0.5);
		OBJ_CUBE[21].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(-0.5, -0.5, -0.5);
		OBJ_CUBE[22].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, -0.5);
		OBJ_CUBE[23].Attr< VertexLayout::ElementType::Position3D>() = XMFLOAT3(0.5, -0.5, 0.5);
		
	}
	if (layout->Resolve<VertexLayout::ElementType::Texture2D>())
	{
		OBJ_CUBE[0].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[1].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[2].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[3].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_CUBE[4].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[5].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[6].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[7].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_CUBE[8].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[9].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[10].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[11].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_CUBE[12].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[13].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[14].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[15].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_CUBE[16].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[17].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[18].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[19].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
		OBJ_CUBE[20].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 1);
		OBJ_CUBE[21].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0, 0);
		OBJ_CUBE[22].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 0);
		OBJ_CUBE[23].Attr< VertexLayout::ElementType::Texture2D>() = XMFLOAT2(1, 1);
	}
	if (layout->Resolve<VertexLayout::ElementType::Normal>())
	{
		OBJ_CUBE[0].Attr< VertexLayout::ElementType::Normal>() = -FORWARD;
		OBJ_CUBE[1].Attr< VertexLayout::ElementType::Normal>() = -FORWARD;
		OBJ_CUBE[2].Attr< VertexLayout::ElementType::Normal>() = -FORWARD;
		OBJ_CUBE[3].Attr< VertexLayout::ElementType::Normal>() = -FORWARD;
		OBJ_CUBE[4].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[5].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[6].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[7].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[8].Attr< VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_CUBE[9].Attr< VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_CUBE[10].Attr< VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_CUBE[11].Attr< VertexLayout::ElementType::Normal>() = FORWARD;
		OBJ_CUBE[12].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[13].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[14].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[15].Attr< VertexLayout::ElementType::Normal>() = RIGHT;
		OBJ_CUBE[16].Attr< VertexLayout::ElementType::Normal>() = UP;
		OBJ_CUBE[17].Attr< VertexLayout::ElementType::Normal>() = UP;
		OBJ_CUBE[18].Attr< VertexLayout::ElementType::Normal>() = UP;
		OBJ_CUBE[19].Attr< VertexLayout::ElementType::Normal>() = UP;
		OBJ_CUBE[20].Attr< VertexLayout::ElementType::Normal>() = -UP;
		OBJ_CUBE[21].Attr< VertexLayout::ElementType::Normal>() = -UP;
		OBJ_CUBE[22].Attr< VertexLayout::ElementType::Normal>() = -UP;
		OBJ_CUBE[23].Attr< VertexLayout::ElementType::Normal>() = -UP;
	}
	
	/*int polyCount = ARRAYSIZE(OBJ_CUBE_INDICE) / 3;
	for (int i = 0; i < polyCount; ++i)
	{
		Vertex v0 = OBJ_CUBE[OBJ_CUBE_INDICE[i * 3]];
		Vertex v1 = OBJ_CUBE[OBJ_CUBE_INDICE[i * 3 + 1]];
		Vertex v2 = OBJ_CUBE[OBJ_CUBE_INDICE[i * 3 + 2]];
		XMFLOAT3 tangent = XMFLOAT3(0, 0, 0);
		CalculateTangent(v0.pos, v1.pos, v2.pos, v0.tex, v1.tex, v2.tex, &tangent);
		OBJ_CUBE[OBJ_CUBE_INDICE[i * 3]].tangent = tangent;
		OBJ_CUBE[OBJ_CUBE_INDICE[i * 3 + 1]].tangent = tangent;
		OBJ_CUBE[OBJ_CUBE_INDICE[i * 3 + 2]].tangent = tangent;
	}*/

	Init(device, OBJ_CUBE, &OBJ_CUBE_INDICE[0], ARRAYSIZE(OBJ_CUBE_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}
