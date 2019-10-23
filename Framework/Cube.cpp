#include "Cube.h"
#include "ShaderFormat.h"

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

Cube::Cube()
	:Shape()
{
	static Vertex OBJ_CUBE[24];

	OBJ_CUBE[0].n = -FORWARD;
	OBJ_CUBE[1].n = -FORWARD;
	OBJ_CUBE[2].n = -FORWARD;
	OBJ_CUBE[3].n = -FORWARD;
	OBJ_CUBE[0].pos = XMFLOAT3(-0.5, -0.5, -0.5);
	OBJ_CUBE[0].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[1].pos = XMFLOAT3(-0.5,	 0.5,	-0.5);
	OBJ_CUBE[1].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[2].pos = XMFLOAT3(0.5,	 0.5,	-0.5);
	OBJ_CUBE[2].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[3].pos = XMFLOAT3(0.5,	-0.5,	-0.5);
	OBJ_CUBE[3].tex = XMFLOAT2(1, 1);
			
	OBJ_CUBE[4].n=RIGHT;
	OBJ_CUBE[5].n= RIGHT;
	OBJ_CUBE[6].n=RIGHT;
	OBJ_CUBE[7].n=RIGHT;
	OBJ_CUBE[4].pos = XMFLOAT3(0.5,	-0.5,	-0.5);
	OBJ_CUBE[4].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[5].pos = XMFLOAT3(0.5,	 0.5,	-0.5);
	OBJ_CUBE[5].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[6].pos = XMFLOAT3(0.5,	 0.5,	 0.5);
	OBJ_CUBE[6].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[7].pos = XMFLOAT3(0.5,	-0.5,	 0.5);
	OBJ_CUBE[7].tex = XMFLOAT2(1, 1);
		
	OBJ_CUBE[8].n = FORWARD;
	OBJ_CUBE[9].n = FORWARD;
	OBJ_CUBE[10].n = FORWARD;
	OBJ_CUBE[11].n = FORWARD;
	OBJ_CUBE[8].pos = XMFLOAT3(0.5,	-0.5, 0.5);
	OBJ_CUBE[8].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[9].pos = XMFLOAT3(0.5,	 0.5,	 0.5);
	OBJ_CUBE[9].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[10].pos = XMFLOAT3(-0.5, 0.5, 0.5);
	OBJ_CUBE[10].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[11].pos = XMFLOAT3(-0.5, -0.5, 0.5);
	OBJ_CUBE[11].tex = XMFLOAT2(1, 1);
			
	OBJ_CUBE[12].n = -RIGHT;
	OBJ_CUBE[13].n = -RIGHT;
	OBJ_CUBE[14].n = -RIGHT;
	OBJ_CUBE[15].n = -RIGHT;
	OBJ_CUBE[12].pos = XMFLOAT3(-0.5,	-0.5,	 0.5);
	OBJ_CUBE[12].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[13].pos = XMFLOAT3(-0.5,	 0.5,	 0.5);
	OBJ_CUBE[13].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[14].pos = XMFLOAT3(-0.5, 0.5, -0.5);
	OBJ_CUBE[14].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[15].pos = XMFLOAT3(-0.5, -0.5, -0.5);
	OBJ_CUBE[15].tex = XMFLOAT2(1, 1);
											  
	OBJ_CUBE[16].n = UP;
	OBJ_CUBE[17].n = UP;
	OBJ_CUBE[18].n = UP;
	OBJ_CUBE[19].n = UP;
	OBJ_CUBE[16].pos = XMFLOAT3(-0.5, 0.5, -0.5);
	OBJ_CUBE[16].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[17].pos = XMFLOAT3(-0.5, 0.5, 0.5);
	OBJ_CUBE[17].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[18].pos = XMFLOAT3(0.5, 0.5, 0.5);
	OBJ_CUBE[18].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[19].pos = XMFLOAT3(0.5, 0.5, -0.5);
	OBJ_CUBE[19].tex = XMFLOAT2(1, 1);
									
	OBJ_CUBE[20].n = -UP;
	OBJ_CUBE[21].n = -UP;
	OBJ_CUBE[22].n = -UP;
	OBJ_CUBE[23].n = -UP;
	OBJ_CUBE[20].pos = XMFLOAT3(-0.5,	-0.5,	 0.5);
	OBJ_CUBE[20].tex = XMFLOAT2(0, 1);
	OBJ_CUBE[21].pos = XMFLOAT3(-0.5,	-0.5,	-0.5);
	OBJ_CUBE[21].tex = XMFLOAT2(0, 0);
	OBJ_CUBE[22].pos = XMFLOAT3(0.5, -0.5, -0.5);
	OBJ_CUBE[22].tex = XMFLOAT2(1, 0);
	OBJ_CUBE[23].pos = XMFLOAT3(0.5, -0.5, 0.5);
	OBJ_CUBE[23].tex = XMFLOAT2(1, 1);

	CalculateTangents(OBJ_CUBE, OBJ_CUBE_INDICE, ARRAYSIZE(OBJ_CUBE_INDICE) / 3);

	XMFLOAT3 minPt = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxPt = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (int i = 0; i < ARRAYSIZE(OBJ_CUBE); ++i)
	{
		minPt.x = min(minPt.x, OBJ_CUBE[i].pos.x);
		minPt.y = min(minPt.y, OBJ_CUBE[i].pos.y);
		minPt.z = min(minPt.z, OBJ_CUBE[i].pos.z);
		maxPt.x = max(maxPt.x, OBJ_CUBE[i].pos.x);
		maxPt.y = max(maxPt.y, OBJ_CUBE[i].pos.y);
		maxPt.z = max(maxPt.z, OBJ_CUBE[i].pos.z);
	}

	Init(&OBJ_CUBE[0], sizeof(Vertex), ARRAYSIZE(OBJ_CUBE), (void*)&OBJ_CUBE_INDICE[0], ARRAYSIZE(OBJ_CUBE_INDICE), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, minPt, maxPt);
}
