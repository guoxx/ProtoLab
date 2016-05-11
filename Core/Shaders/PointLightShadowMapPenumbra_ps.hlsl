#include "PointLightShadowMapPenumbra_common.h"

TextureCube<float> g_PointLightShadowMap : register(t0);
TextureCube<float> g_PointLightDilatedShadowMap : register(t1);
Texture2D<float> g_DepthBuffer : register(t2);

SamplerState g_PointSampler : register(s0);

float EstimatePenumbra(float zReceiver, float zBlocker)
{
	return (zReceiver - zBlocker) / zBlocker;
}

float4 main(VSOutput input) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}