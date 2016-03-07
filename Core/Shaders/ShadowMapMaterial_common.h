#include "HLSLCommon.h"

ConstantsBuffer(View, vs, 0)
{
	float4x4 mModel;
};

ConstantsBuffer(ViewGS, gs, 0)
{
	float4x4 mViewProj[6];
};

#ifdef __HLSL__
struct VSInput
{
	float3 position : SV_POSITION;
};

struct VSOutput
{
	float4 positionWS : SV_POSITION;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	uint RTIndex	: SV_RenderTargetArrayIndex;
};

#endif