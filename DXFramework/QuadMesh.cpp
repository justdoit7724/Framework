
#include "QuadMesh.h"
#include "ShaderFormat.h"

QuadMesh::QuadMesh()
	: Mesh()
{
	static Vertex OBJ_QUAD[4];

	OBJ_QUAD[0].n = FORWARD;
	OBJ_QUAD[1].n = FORWARD;
	OBJ_QUAD[2].n = FORWARD;
	OBJ_QUAD[3].n = FORWARD;
	OBJ_QUAD[0].tangent = -RIGHT;
	OBJ_QUAD[1].tangent = -RIGHT;
	OBJ_QUAD[2].tangent = -RIGHT;
	OBJ_QUAD[3].tangent = -RIGHT;
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



	Init(&OBJ_QUAD[0], sizeof(Vertex), ARRAYSIZE(OBJ_QUAD), OBJ_QUAD_INDICE, ARRAYSIZE(OBJ_QUAD_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

