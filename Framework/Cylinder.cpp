#include "Cylinder.h"
#include "ShaderFormat.h"

Cylinder::Cylinder(const int sliceCount)
	:Shape()
{
	assert(sliceCount >= 3);

#pragma region side

	float dTheta = XM_2PI / sliceCount;
	float hHeight = 1.0f;

	std::vector<Vertex> vertice;
	for (int i = 0; i < 2; ++i) {
		float y = -hHeight + i*2;

		for (int j = 0; j <= sliceCount; ++j) {
			Vertex vertex;
			float c = cosf(j*dTheta);
			float s = sinf(j*dTheta);
			vertex.pos = XMFLOAT3(0.5f*c, y, 0.5f*s);
			vertex.tex.x = float(j) / sliceCount;
			vertex.tex.y = 1.0f - i;

			XMFLOAT3 tangent = XMFLOAT3(-s, 0, c);
			XMFLOAT3 bitangent = -UP;
			vertex.n = Cross(tangent, bitangent);

			vertice.push_back(vertex);
		}
	}

	std::vector<UINT> indice;
	int ringVertexCount = sliceCount + 1;
	for (int i = 0; i < sliceCount; ++i)
	{
		indice.push_back(i);
		indice.push_back(ringVertexCount + i);
		indice.push_back(ringVertexCount + i + 1);
		indice.push_back(i);
		indice.push_back(ringVertexCount + i + 1);
		indice.push_back(i + 1);
	}
#pragma endregion

#pragma region cap
	// top
	int baseIdx = vertice.size();
	for (int i = 0; i < sliceCount + 1; ++i)
	{
		float x = 0.5f * cosf(i*dTheta);
		float z = 0.5f * sinf(i*dTheta);
		float u = x / 2.0f + 0.5f;
		float v = z / 2.0f + 0.5f;

		Vertex vertex;
		vertex.pos = XMFLOAT3(x, hHeight, z);
		vertex.n = UP;
		vertex.tex = XMFLOAT2(u, v);
		vertice.push_back(vertex);
	}

	Vertex vertex;
	vertex.pos = XMFLOAT3(0, hHeight, 0);
	vertex.n = UP;
	vertex.tex = XMFLOAT2(0.5f, 0.5f);
	vertice.push_back(vertex);

	int centerIdx = vertice.size()-1;
	for (int i = 0; i < sliceCount; ++i)
	{
		indice.push_back(centerIdx);
		indice.push_back(baseIdx + i + 1);
		indice.push_back(baseIdx + i);
	}

	//bottom
	baseIdx = vertice.size();
	for (int i = 0; i < sliceCount + 1; ++i)
	{
		float x = 0.5f * cosf(i*dTheta);
		float z = 0.5f * sinf(i*dTheta);
		float u = x / 2.0f + 0.5f;
		float v = z / 2.0f + 0.5f;

		Vertex vertex;
		vertex.pos = XMFLOAT3(x, -hHeight, z);
		vertex.n = -UP;
		vertex.tex = XMFLOAT2(u, v);
		vertice.push_back(vertex);
	}

	vertex.pos = XMFLOAT3(0, -hHeight, 0);
	vertex.n = -UP;
	vertex.tex = XMFLOAT2(0.5f, 0.5f);
	vertice.push_back(vertex);

	centerIdx = vertice.size() - 1;
	for (int i = 0; i < sliceCount; ++i)
	{
		indice.push_back(centerIdx);
		indice.push_back(baseIdx + i);
		indice.push_back(baseIdx + i + 1);
	}
#pragma endregion

	int polySize = indice.size()/3;
	for (int i=0; i< polySize; ++i)
	{
		XMFLOAT3 tangent;
		Vertex v0 = vertice[indice[i * 3]];
		Vertex v1 = vertice[indice[i * 3 + 1]];
		Vertex v2 = vertice[indice[i * 3 + 2]];

		CalculateTangent(
			v0.pos,
			v1.pos,
			v2.pos,
			v0.tex,
			v1.tex,
			v2.tex, &tangent);
		tangent = Normalize(tangent);

		vertice[indice[i * 3]].tangent = tangent;
		vertice[indice[i * 3+1]].tangent = tangent;
		vertice[indice[i * 3+2]].tangent = tangent;
	}

	Init(vertice.data(), sizeof(Vertex), vertice.size(), indice.data(), indice.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}



