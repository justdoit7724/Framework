#pragma once

#include "Math.h"


#define LAYER_NONE 0
#define	LAYER_UI 1
#define	LAYER_DEBUG 2
#define	LAYER_BACKGROUND 4
#define	LAYER_STD 8

////custom layer//////
#define LAYER_INTERFACE_CAMERA 0x10
//////////////////////

#define LAYER_ALL 0xff

inline bool LayerCheck(int bit, int layer)
{
	return ((bit & layer) == layer);
}

enum FRAME_KIND {
	FRAME_KIND_PERSPECTIVE,
	FRAME_KIND_ORTHOGONAL
};

struct Frustum
{
	Geom::PlaneInf front;
	Geom::PlaneInf back;
	Geom::PlaneInf right;
	Geom::PlaneInf left;
	Geom::PlaneInf top;
	Geom::PlaneInf bottom;

	Frustum() {}
};

enum class CMPNT_KIND
{
	Transform = 0,
	Collider,
	Camera,
	MeshRenderer,
	Animator,
	Light,
	MeshFilter,
	Script
};

enum class SCRIPT_KIND
{
	CameraMovingCenter
};

enum class COLLIDER_KIND
{
	Sphere,
	Cube,
	Quad
};

enum class LIGHT_KIND
{
	Directional,
	Point,
	Spot
};