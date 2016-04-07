#include "Lighting_common.h"

Texture2D<float4> GBuffer0 : register(t0);
Texture2D<float4> GBuffer1 : register(t1);
Texture2D<float> DepthBuffer : register(t2);

SamplerState PointSampler;

PSOutput main(PSInput input)
{
	PSOutput output;

	GBuffer GData = DecodeGBuffer(GBuffer0, GBuffer1, PointSampler, input.texcoord);
	float3 albedo = GBufferGetAlbedo(GData);
	float3 normal = GBufferGetNormal(GData);
	float depth = DepthBuffer.SampleLevel(PointSampler, input.texcoord, 0);
	float3 position = ReconstructWorldSpacePosition(input.positionSS, depth, g_mProjInv, g_mViewInv);

	// diffuse
	float D = length(g_LightPosition.xyz - position);
	float3 L = normalize(g_LightPosition.xyz - position);
	float cosThetaI = saturate(dot(normal, L));
	float3 E = pointLightIrradiance(g_LightIntensity.x, D, g_RadiusStart.x, g_RadiusEnd.x);
	float3 diffuse = Diffuse_Lambert(albedo) * E * cosThetaI;

	output.color = float4(diffuse, 1.0);

	return output;
}