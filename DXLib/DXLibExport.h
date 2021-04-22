#pragma once


#ifdef DXLIB_EXPORTS
#define DXLIB_DLL __declspec(dllexport)
#else
#define DXLIB_DLL __declspec(dllimport)
#endif

#include "PipelineRegFlag.h"
#include "DefVertex.h"
#include "Graphic.h"
#include "CustomSTL.h"