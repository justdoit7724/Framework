// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H


#ifdef DXLIB_EXPORTS
#define DXLIB_DLL __declspec(dllexport)
#else
#define DXLIB_DLL __declspec(dllimport)
#endif

// add headers that you want to pre-compile here
#include "framework.h"

#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXColors.h>
#pragma comment(lib,"d3d11.lib")

using namespace DirectX;



#ifndef LOAD_LIBRARY
#define LOAD_LIBRARY


//path : C:\Program Files (x86)\Windows Kits\10\Include\10.0.18362.0\um

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib, __FILE__"/../../SDK/directxtk_desktop_2015.2019.12.17.1/lib_x64/_Debug/DirectXTK.lib")
#else
#pragma comment(lib, __FILE__"/../../SDK/directxtk_desktop_2015.2019.12.17.1/lib_x64/_Release/DirectXTK.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib, __FILE__"/../../SDK/directxtk_desktop_2015.2019.12.17.1/lib_win32/_Debug/DirectXTK.lib")
#else
#pragma comment(lib, __FILE__"/../../SDK/directxtk_desktop_2015.2019.12.17.1/lib_win32/_Release/DirectXTK.lib")
#endif

#endif


#endif

#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>
#include <assert.h>
#include <string>
#include <limits>
#include <memory>
#include <queue>
#include <vector>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>
#include <fstream>

#include "PipelineRegFlag.h"

#endif //PCH_H
