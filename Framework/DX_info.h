#pragma once


using namespace DirectX;
using namespace Microsoft::WRL;

extern ID3D11Device* DX_Device;
extern ID3D11DeviceContext* DX_DContext;

#define MB(s) \
MessageBox(0,s,0,0);\
exit(-1);

#define r_assert(r)	assert(SUCCEEDED(r))