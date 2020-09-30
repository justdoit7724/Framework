#pragma once

#define LAYER_NONE 0
#define	LAYER_UI 1
#define	LAYER_DEBUG 2
#define	LAYER_BACKGROUND 4
#define	LAYER_STD 8

////custom layer//////
#define LAYER_INTERFACE_CAMERA 0x10
//////////////////////

#define LAYER_ALL 0xff

namespace DX
{
	inline bool LayerCheck(int bit, int layer)
	{
		return ((bit & layer) == layer);
	}
}