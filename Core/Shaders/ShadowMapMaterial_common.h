#include "HLSLCommon.h"

ConstantsBuffer(View, vs, 0)
{
	float4x4 mModel;
	float4x4 mViewProj[6];
};

#ifdef __HLSL__
struct VSInput
{
	float3 position : SV_POSITION;
};

struct PSInput
{
	float4 position : SV_POSITION;
};

#endif