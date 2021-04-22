#pragma once
#include <d3d11.h>
#include <DirectXMath.h>



//struct SHADER_STD_TRANSF
//{
//	//todo
//	XMMATRIX w[128];
//	XMMATRIX v;
//	XMMATRIX p;
//	XMMATRIX n;
//
//	SHADER_STD_TRANSF(XMMATRIX world, XMMATRIX v, XMMATRIX p)
//		:v(v), p(p)
//	{
//		ZeroMemory(w, sizeof(XMMATRIX) * 128);
//		w[0] = world;
//
//		XMVECTOR det = DirectX::XMMatrixDeterminant(world);
//		n = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, world));
//	}
//	SHADER_STD_TRANSF(XMMATRIX* arWorld, int size, XMMATRIX v, XMMATRIX p)
//		:v(v), p(p)
//	{
//		ZeroMemory(w, sizeof(XMMATRIX) * 128);
//		memcpy(w, arWorld, size * sizeof(XMMATRIX));
//
//		/*XMVECTOR det = DirectX::XMMatrixDeterminant(world);
//		n = DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, world));*/
//	}
//};
//
//struct SHADER_PT_TRANSF
//{
//	XMMATRIX w;
//	XMMATRIX vp;
//	XMMATRIX n;
//	XMMATRIX ui;
//	XMMATRIX dLightVPT;
//	XMMATRIX pLightP;
//
//	SHADER_PT_TRANSF(const XMMATRIX& world, const XMMATRIX& vp, const XMMATRIX& ui, const XMMATRIX& dLightVP, const XMMATRIX& pLightP)
//		:w(world),
//		vp(vp),
//		ui(ui),
//		dLightVPT(dLightVP* XMMATRIX(
//			0.5f, 0, 0, 0,
//			0, -0.5f, 0, 0,
//			0, 0, 1, 0,
//			0.5f, 0.5f, 0, 1)),
//		pLightP(pLightP)
//	{
//		XMVECTOR detWorld = DirectX::XMMatrixDeterminant(world);
//		XMMATRIX invdetWorld = DirectX::XMMatrixInverse(&detWorld, world);
//		n = DirectX::XMMatrixTranspose(invdetWorld);
//	}
//};
//struct VS_Simple_Property
//{
//	XMMATRIX worldMat;
//	XMMATRIX vpMat;
//
//	VS_Simple_Property(const XMMATRIX& w, const XMMATRIX& vp)
//		: worldMat(w), vpMat(vp) {}
//};
//

