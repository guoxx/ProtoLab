#include "HLSLCommon.h"
#include "FilterIdentity_common.h"

ConstantsBuffer(CBuffer, ps, 0)
{
	float4x4 g_mViewProjLight[6];
	float4x4 mProjInv;
	float4x4 mViewInv;
	float4 g_LightPositionWS;
	float4 g_LightNearFar;
	float4 g_LightSize;
};

#ifdef __HLSL__
#endif