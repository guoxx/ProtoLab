#include "PointLightShadowMapMinFilter_common.h"

Texture2DArray<float> g_shadowMap : register(t0);
SamplerState g_pointSampler : register(s0);

float main(GSOutput input) : SV_TARGET
{
	float2 step = g_sampleStep.x * g_dir.xy;

	float2 offsets[10] = {
		-5 * step,
		-4 * step,
		-3 * step,
		-2 * step,
		-1 * step,
		 1 * step,
		 2 * step,
		 3 * step,
		 4 * step,
		 5 * step,
	};

	float minz = g_shadowMap.Sample(g_pointSampler, float3(input.texcoord, input.RTIndex)).x;
	for (int i = 0; i < 10; ++i)
	{
		minz = min(minz, g_shadowMap.Sample(g_pointSampler, float3(input.texcoord + offsets[i], input.RTIndex)).x);
	}

	return minz;
}