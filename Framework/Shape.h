#pragma once
#include "Component.h"
#include <vector>

struct Vertex;

class Shape : public Component
{
public:
	virtual void Apply();

protected:
	Shape();
	void Init(const Vertex* vertice, const int vertexCount, const UINT* indice, const int idxCount);

private:
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer> indexBuffer;
	int indexCount;
};

