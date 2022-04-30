#include "pch.h"
#include "Vertex.h"

using namespace DX;

VertexLayout::Element::Element(ElementType type, int offset)
	:m_type(type), m_offset(offset)
{
}
int VertexLayout::Element::GetOffsetAfter()const
{
	return m_offset + Size();
}
int VertexLayout::Element::GetOffset()const
{
	return m_offset;
}
int VertexLayout::Element::Size() const
{
	return SizeOf(m_type);
}

int VertexLayout::Element::SizeOf(ElementType type)
{
	switch (type)
	{
	case Position2D:
		return sizeof(Map< Position2D>::SysType);
	case Texture2D:
		return sizeof(Map<Texture2D>::SysType);
	case Position3D:
		return sizeof(Map<Position3D>::SysType);
	case Normal:
		return sizeof(Map<Normal>::SysType);
	case Float3Color:
		return sizeof(Map<Float3Color>::SysType);
	case Float4Color:
		return sizeof(Map<Float4Color>::SysType);
	case BGRAColor:
		return sizeof(Map<BGRAColor>::SysType);
	}

	assert("Wrong Element Type" && false);
}

VertexLayout::ElementType VertexLayout::Element::GetType() const
{
	return m_type;
}

D3D11_INPUT_ELEMENT_DESC VertexLayout::Element::GetDesc() const
{
	switch (m_type)
	{
	case Position2D:
		return GenerateDesc<Position2D>(m_offset);
	case Position3D:
		return GenerateDesc<Position3D>(m_offset);
	case Texture2D:
		return GenerateDesc<Texture2D>(m_offset);
	case Normal:
		return GenerateDesc<Normal>(m_offset);
	case Float3Color:
		return GenerateDesc<Float3Color>(m_offset);
	case Float4Color:
		return GenerateDesc<Float4Color>(m_offset);
	case BGRAColor:
		return GenerateDesc<BGRAColor>(m_offset);
	}

	assert("wrong Element Type" && false);
}


int VertexLayout::Count() const
{
	return m_elements.size();
}

void VertexLayout::Clear()
{
	m_elements.clear();
}

const VertexLayout::Element* VertexLayout::ResolveByIndex(unsigned int i)const
{
	return &m_elements[i];
}
VertexLayout& VertexLayout::Append(ElementType type)
{
	m_elements.emplace_back(type, Size());
	return *this;
}
int VertexLayout::Size() const
{
	if (m_elements.empty())
		return 0;

	return m_elements.back().GetOffsetAfter();
}
std::vector<D3D11_INPUT_ELEMENT_DESC> VertexLayout::GetLayout() const
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> descs;
	for (const auto& elem : m_elements)
	{
		descs.push_back(elem.GetDesc());
	}

	return descs;
}




Vertex::Vertex(char* data, const VertexLayout& layout)
	:m_data(data), m_layout(layout){}

Vertice::Vertice(VertexLayout layout)
	:m_layout(std::move(layout)){}

const VertexLayout& Vertice::GetLayout() const
{
	return m_layout;
}
const char* Vertice::GetData() const
{
	return m_buffer.data();
}
int Vertice::SizeByte() const
{
	return m_buffer.size();
}
int Vertice::Count() const
{
	return m_buffer.size() / m_layout.Size();
}
	
void DX::Vertice::EmplaceBack()
{
	if (m_layout.Count() == 0)
		assert("layout is empty" && false);

	m_buffer.resize(m_buffer.size() + m_layout.Size());
}

Vertex Vertice::Front()
{
	if (m_buffer.empty())
		assert("VertexBuffer is empty" && false);

	return Vertex(m_buffer.data(), m_layout);
}
Vertex Vertice::Back()
{
	if (m_buffer.empty())
		assert("VertexBuffer is empty" && false);

	return Vertex(m_buffer.data() + m_buffer.size() - m_layout.Size() ,m_layout);
}
Vertex Vertice::operator[](int i)
{
	assert(i < Count());
	return Vertex(m_buffer.data() + m_layout.Size() * i,m_layout );
}

void DX::Vertice::Clear()
{
	m_buffer.clear();
}
