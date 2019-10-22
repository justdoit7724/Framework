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
	for (int i = 0; i < ringVertexCount; ++i)
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

	Vertex* verticeData = vertice.data();
	UINT* indiceData = indice.data();
	CalculateTangents(verticeData, indiceData, indice.size() / 3);

	XMFLOAT3 minPt = XMFLOAT3(FLT_MAX, FLT_MAX, FLT_MAX);
	XMFLOAT3 maxPt = XMFLOAT3(-FLT_MAX, -FLT_MAX, -FLT_MAX);
	for (auto vertex : vertice)
	{
		minPt.x = min(minPt.x, vertex.pos.x);
		minPt.y = min(minPt.y, vertex.pos.y);
		minPt.z = min(minPt.z, vertex.pos.z);
		maxPt.x = max(maxPt.x, vertex.pos.x);
		maxPt.y = max(maxPt.y, vertex.pos.y);
		maxPt.z = max(maxPt.z, vertex.pos.z);
	}

	Init(verticeData, sizeof(Vertex), vertice.size(), indiceData, indice.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST, minPt, maxPt);
}



