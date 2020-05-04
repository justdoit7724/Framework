#pragma once

#include "DX_info.h"

class Unbinding
{
public:
	static void VSSRV(UINT startIdx, UINT numViews);
	static void PSSRV(UINT startIdx, UINT numViews);
	static void CSSRV(UINT startIdx, UINT numViews);
	static void CSUAV(UINT startIdx, UINT numViews);

};
