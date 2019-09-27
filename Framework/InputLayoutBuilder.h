#pragma once
#include "DX_info.h"
#include <vector>
#include <map>

class InputLayoutBuilder
{
public:
	InputLayoutBuilder() {
	}

	InputLayoutBuilder& SetInput(LPCSTR semanticName, DXGI_FORMAT format, UINT alignedByteOffset)
	{
		list.push_back({semanticName, 0, format, 0, alignedByteOffset, D3D11_INPUT_PER_VERTEX_DATA, 0});
		return *this;
	}
	D3D11_INPUT_ELEMENT_DESC* Build()
	{
		return list.data();
	}

private:
	std::vector<D3D11_INPUT_ELEMENT_DESC> list;
};

class TestBuilder
{
public:
	TestBuilder() {
	}

	TestBuilder& SetInput(int n)
	{
		list.push_back(n);
		return *this;
	}
	int* Build()
	{
		return list.data();
	}

private:
	std::vector<int> list;
};

