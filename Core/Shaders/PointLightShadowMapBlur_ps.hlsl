#include "PointLightShadowMapBlur_common.h"

SamplerState g_PointSampler : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}