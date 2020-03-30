#pragma once

#define	LAYER_UI 1
#define	LAYER_DEBUG 2
#define	LAYER_BACKGROUND 4
#define	LAYER_STD 8

inline bool LayerCheck(int bit, int layer)
{
	return ((bit & layer) == layer);
}