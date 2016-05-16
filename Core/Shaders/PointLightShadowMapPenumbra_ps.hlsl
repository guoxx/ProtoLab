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
	float depth = g_DepthBuffer.Sample(g_PointSampler, input.texcoord);
	float3 posWS = ReconstructWorldSpacePosition(input.positionSS, depth, mProjInv, mViewInv);

	float3 lightVec = (posWS - g_LightPositionWS.xyz);
	float3 normalizedLightVec = normalize(lightVec);
	normalizedLightVec.z = -normalizedLightVec.z;
	float zBlocker = g_PointLightDilatedShadowMap.Sample(g_PointSampler, normalizedLightVec);

	int shadowMapFace = getFaceOfPointLightShadowMap(g_LightPositionWS.xyz, posWS);
	float4 posLS = mul(float4(posWS, 1), g_mViewProjLight[shadowMapFace]);
	float zReceiver = posLS.z / posLS.w;

	float4 outColor = float4(1, 1, 1, 1);
	outColor.r = zBlocker > zReceiver ? 0 : 1;
	outColor.g = EstimatePenumbra(zReceiver, zBlocker);

	return outColor;
}