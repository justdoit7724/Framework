#pragma once
#include "DX_info.h"

class Resource
{
public:
	static void VSUnbindSRV(UINT startIdx, UINT numViews);
	static void PSUnbindSRV(UINT startIdx, UINT numViews);
	static void CSUnbindSRV(UINT startIdx, UINT numViews);
	static void CSUnbindUAV(UINT startIdx, UINT numViews);

};
