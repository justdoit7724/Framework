#pragma once


#ifdef DXLIB_EXPORTS
#define DXLIB_DLL __declspec(dllexport)
#else
#define DXLIB_DLL __declspec(dllimport)
#endif


#include <d3d11.h>

#include "Graphic.h"
#include "Scene.h"