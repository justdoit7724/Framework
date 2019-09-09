#pragma once
#include "Component.h"
#include <vector>

class Buffer;

class Shape : public Component
{
public:
	virtual void Apply();
	UINT IndexCount() {return indexCount;}

protected:
	Shape();
	void Init(void* vertice, const int vertexCount, void* indice, UINT idxCount);

private:
	Buffer* vertexBuffer=nullptr;
	Buffer* indexBuffer=nullptr;
	UINT indexCount;
};

