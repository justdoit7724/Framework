
#include "pch.h"
#include "CylinderMesh.h"
#include "ShaderFormat.h"
#include "Math.h"

using namespace DX;

<<<<<<< HEAD
CylinderMesh::CylinderMesh(ID3D11Device* device, int sliceCount)
	:Mesh(1)
=======
CylinderMesh::CylinderMesh(ID3D11Device* device, int sliceCount, const VertexLayout* layout)
>>>>>>> 03_DepthPeeling
{
	assert(sliceCount >= 3);

#pragma region side

	float dTheta = XM_2PI / sliceCount;
	float hHeight = 0.5;
	float rad = 0.5;

	Vertice vertice(*layout);

	const bool isPos3 = layout->Resolve<VertexLayout::ElementType::Position3D>();
	const bool isTex = layout->Resolve<VertexLayout::ElementType::Texture2D>();
	const bool isNorm = layout->Resolve<VertexLayout::ElementType::Normal>();
	
	int vertCount = 0;
	for (int i = 0; i < 2; ++i) {
		float y = -hHeight + i;

		for (int j = 0; j <= sliceCount; ++j) {

			vertice.EmplaceBack();

			float c = cosf(j * dTheta);
			float s = sinf(j * dTheta);

			XMFLOAT3 tangent = XMFLOAT3(-s, 0, c);
<<<<<<< HEAD
			XMFLOAT3 bitangent = -DIR_UP;
			vertex.n = Cross(tangent, bitangent);
=======
			XMFLOAT3 bitangent = -UP;
>>>>>>> 03_DepthPeeling

			if(isPos3)
				vertice[vertCount].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(rad * c, y, rad * s);
			if(isTex)
				vertice[vertCount].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(float(j) / sliceCount, 1.0f - i);
			if(isNorm)
				vertice[vertCount].Attr<VertexLayout::ElementType::Normal>() = Cross(tangent, bitangent);
			
			vertCount++;
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
	int baseIdx = vertice.Count();
	for (int i = 0; i < sliceCount + 1; ++i)
	{
		vertice.EmplaceBack();

		float x = rad * cosf(i * dTheta);
		float z = rad * sinf(i * dTheta);
		float u = x / 1.5f;
		float v = z / 1.5f;

<<<<<<< HEAD
		Vertex vertex;
		vertex.pos = XMFLOAT3(x, hHeight, z);
		vertex.n = DIR_UP;
		vertex.tex = XMFLOAT2(u, v);
		vertice.push_back(vertex);
	}

	Vertex vertex;
	vertex.pos = XMFLOAT3(0, hHeight, 0);
	vertex.n = DIR_UP;
	vertex.tex = XMFLOAT2(0.5f, 0.5f);
	vertice.push_back(vertex);
=======
		if (isPos3)
			vertice[vertCount].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(x, hHeight, z);
		if (isTex)
			vertice[vertCount].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(u, v);
		if (isNorm)
			vertice[vertCount].Attr<VertexLayout::ElementType::Normal>() = UP;

		vertCount++;
	}
>>>>>>> 03_DepthPeeling

	vertice.EmplaceBack();
	if (isPos3)
		vertice[vertCount].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(0, hHeight, 0);
	if (isTex)
		vertice[vertCount].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0.5f, 0.5f);
	if (isNorm)
		vertice[vertCount].Attr<VertexLayout::ElementType::Normal>() = UP;
	vertCount++;
	
	int centerIdx = vertice.Count();
	for (int i = 0; i < sliceCount; ++i)
	{
		indice.push_back(centerIdx-1);
		indice.push_back(baseIdx + i + 1);
		indice.push_back(baseIdx + i);
	}

	//bottom
	baseIdx = vertice.Count();
	for (int i = 0; i < sliceCount + 1; ++i)
	{
		vertice.EmplaceBack();

		float x = rad * cosf(i * dTheta);
		float z = rad * sinf(i * dTheta);
		float u = x / 1.5f;
		float v = z / 1.5f;

<<<<<<< HEAD
		Vertex vertex;
		vertex.pos = XMFLOAT3(x, -hHeight, z);
		vertex.n = -DIR_UP;
		vertex.tex = XMFLOAT2(u, v);
		vertice.push_back(vertex);
	}

	vertex.pos = XMFLOAT3(0, -hHeight, 0);
	vertex.n = -DIR_UP;
	vertex.tex = XMFLOAT2(0.5f, 0.5f);
	vertice.push_back(vertex);
=======

		if (isPos3)
			vertice[vertCount].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(x, -hHeight, z);
		if (isTex)
			vertice[vertCount].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(u, v);
		if (isNorm)
			vertice[vertCount].Attr<VertexLayout::ElementType::Normal>() = -UP;
		vertCount++;

	}
>>>>>>> 03_DepthPeeling

	vertice.EmplaceBack();
	if (isPos3)
		vertice[vertCount].Attr<VertexLayout::ElementType::Position3D>() = XMFLOAT3(0, -hHeight, 0);
	if (isTex)
		vertice[vertCount].Attr<VertexLayout::ElementType::Texture2D>() = XMFLOAT2(0.5f, 0.5f);
	if (isNorm)
		vertice[vertCount].Attr<VertexLayout::ElementType::Normal>() = -UP;
	vertCount++;
	
	centerIdx = vertice.Count();
	for (int i = 0; i < sliceCount; ++i)
	{
		indice.push_back(centerIdx-1);
		indice.push_back(baseIdx + i);
		indice.push_back(baseIdx + i + 1);
	}
#pragma endregion

<<<<<<< HEAD
	SetSubMesh(device, 0,vertice.data(), sizeof(Vertex), vertice.size(), indice.data(), indice.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
=======
	Init(device,vertice, indice.data(), indice.size(), D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
>>>>>>> 03_DepthPeeling
}
