#include "HLSLCommon.h"
#include "FilterIdentity_common.h"

ConstantsBuffer(LightData, ps, 0)
{
	float4 g_sampleStep;
};

#ifdef __HLSL__
struct GSOutput
{
	uint RTIndex	: SV_RenderTargetArrayIndex;
	float4 position : SV_POSITION;
	float2 texcoord : TEXCOORD;
};

struct PSOutput
{
	float minDepth : SV_Target;
};

#endif