#include "CustomFormat.h"
#include "Geometrics.h"
#include "Transform.h"

Vertex::Vertex(const Vertex & v)
{
	pos = v.pos;
	n = v.n;
	tex = v.tex;
}

Vertex::Vertex(Vector3f _pos)
{
	pos = _pos;
	n = 0;
	tex = 0;
}

ShaderMatrix::ShaderMatrix(const DirectX::XMMATRIX & vp)
	:w(DirectX::XMMatrixIdentity()), vp(vp), n(DirectX::XMMatrixIdentity())
{
}

ShaderMatrix::ShaderMatrix(Transform * transform, const DirectX::XMMATRIX & vp)
	: vp(vp)
{
	const XMMATRIX s = transform->S();
	const XMMATRIX r = transform->R();
	const XMMATRIX t = transform->T();

	w = s * r * t;
	n = XMMatrixTranspose(XMMatrixInverse(&XMMatrixDeterminant(r), r));
}

