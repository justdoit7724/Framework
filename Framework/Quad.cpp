#include "Quad.h"
#include "ShaderFormat.h"

Quad::Quad()
	: Shape()
{
	static Vertex OBJ_QUAD[4];

	OBJ_QUAD[0].n = FORWARD;
	OBJ_QUAD[1].n = FORWARD;
	OBJ_QUAD[2].n = FORWARD;
	OBJ_QUAD[3].n = FORWARD;
	OBJ_QUAD[0].pos= XMFLOAT3(-0.5, -0.5, 0);
	OBJ_QUAD[0].tex = XMFLOAT2(1, 1);
	OBJ_QUAD[1].pos= XMFLOAT3(-0.5, 0.5, 0);
	OBJ_QUAD[1].tex =  XMFLOAT2(1, 0);
	OBJ_QUAD[2].pos = XMFLOAT3(0.5, 0.5, 0);
	OBJ_QUAD[2].tex=XMFLOAT2(0, 0);
	OBJ_QUAD[3].pos = XMFLOAT3(0.5, -0.5, 0);
	OBJ_QUAD[3].tex=XMFLOAT2(0, 1);
	
	UINT OBJ_QUAD_INDICE[6] =
	{
		3,2,1,
		3,1,0
	};

	CalculateTangents(OBJ_QUAD, OBJ_QUAD_INDICE, ARRAYSIZE(OBJ_QUAD_INDICE) / 3);

	XMFLOAT3 minPt = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxPt = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (auto vertex : OBJ_QUAD)
	{
		minPt.x = fminf(minPt.x, vertex.pos.x);
		minPt.y = fminf(minPt.y, vertex.pos.y);
		minPt.z = fminf(minPt.z, vertex.pos.z);
		maxPt.x = fmaxf(maxPt.x, vertex.pos.x);
		maxPt.y = fmaxf(maxPt.y, vertex.pos.y);
		maxPt.z = fmaxf(maxPt.z, vertex.pos.z);
	}

	Init(&OBJ_QUAD[0], sizeof(Vertex), ARRAYSIZE(OBJ_QUAD), OBJ_QUAD_INDICE, ARRAYSIZE(OBJ_QUAD_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, minPt, maxPt);
}

