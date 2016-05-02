#include "PointLightShadowMapMinFilter_common.h"

Texture2DArray<float> g_shadowMap : register(t0);
SamplerState g_pointSampler : register(s0);

float main(GSOutput input) : SV_TARGET
{
	float depthVal = g_shadowMap.SampleLevel(g_pointSampler, float3(input.texcoord, float(input.RTIndex)), 0);
	return depthVal;
}