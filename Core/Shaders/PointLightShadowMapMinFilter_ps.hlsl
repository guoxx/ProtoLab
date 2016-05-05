#include "PointLightShadowMapMinFilter_common.h"

Texture2DArray<float> g_shadowMap : register(t0);
SamplerState g_pointSampler : register(s0);

float main(GSOutput input) : SV_TARGET
{
	float minDepth = 1.0f;
	int stepx = 256/32;
	int stepy = 256/32/4;
	for (int i = 0; i < stepx; ++i)
	{
		for (int j = 0; j < stepy; j = j + 4)
		{
			float4 v = g_shadowMap.GatherRed(g_pointSampler, float3(input.texcoord, input.RTIndex), int2(i, j), int2(i, j + 1), int2(i, j + 2), int2(i, j + 3));
			minDepth = min(minDepth, min(v.x, min(v.y, min(v.z, v.w))));
		}
	}

	return minDepth;
}