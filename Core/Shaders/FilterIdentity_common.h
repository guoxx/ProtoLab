#include "HLSLCommon.h"

#ifdef __HLSL__
struct VSInput
{
	float3 position : SV_Position;
	float2 texcoord : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_Position;
	float2 positionSS : POSITION0;
	float2 texcoord : TEXCOORD;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
};

#endif