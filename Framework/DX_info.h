#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <assert.h>
#include <DirectXMath.h>

using namespace DirectX;
using namespace Microsoft::WRL;

extern ID3D11Device* device;
extern ID3D11DeviceContext* dContext;

#define MB(s) MessageBox(0,s,0,0)

#define r_assert(r)	assert(SUCCEEDED(r))