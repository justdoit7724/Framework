#pragma once

#include <wrl.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>
#include <assert.h>
#include <DirectXMath.h>
#include <vector>

using namespace DirectX;
using namespace Microsoft::WRL;

extern ID3D11Device* DX_Device;
extern ID3D11DeviceContext* DX_DContext;

#define MB(s) MessageBox(0,s,0,0)

#define r_assert(r)	assert(SUCCEEDED(r))