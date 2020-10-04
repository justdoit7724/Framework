#pragma once


#ifdef DXLIB_EXPORTS
#define DXLIB_DLL __declspec(dllexport)
#else
#define DXLIB_DLL __declspec(dllimport)
#endif


#include <d3d11.h>

#include "Graphic.h"
#include "Math.h"
#include "Light.h"
#include "Buffer.h"
#include "Camera.h"
#include "Transform.h"
#include "ShaderReg.h"
#include "Object.h"
#include "Mesh.h"
#include "CylinderMesh.h"
#include "LineMesh.h"
#include "QuadMesh.h"
#include "SphereMesh.h"
#include "TextureLoad.h"