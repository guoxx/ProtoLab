#include "Lighting_common.h"

Texture2D<float4> GBuffer0 : register(t0);
Texture2D<float4> GBuffer1 : register(t1);
Texture2D<float4> GBuffer2 : register(t2);
Texture2D<float> DepthBuffer : register(t3);

SamplerState PointSampler;

PSOutput main(PSInput input)
{
	PSOutput output;

	GBuffer GData = DecodeGBuffer(GBuffer0, GBuffer1, GBuffer2, PointSampler, input.texcoord);
	float3 albedo = GBufferGetAlbedo(GData);
	float3 f0 = GBufferGetF0(GData);
	float roughness = GBufferGetRoughness(GData);
	float3 normal = GBufferGetNormal(GData);
	float depth = DepthBuffer.SampleLevel(PointSampler, input.texcoord, 0);
	float3 position = ReconstructWorldSpacePosition(input.positionSS, depth, g_mProjInv, g_mViewInv);

	// diffuse
	float D = length(g_LightPosition.xyz - position);
	float3 L = normalize(g_LightPosition.xyz - position);
	float cosThetaI = saturate(dot(normal, L));
	float3 E = pointLightIrradiance(g_LightIntensity.x, D, g_RadiusStart.x, g_RadiusEnd.x);
	float3 diffuse = Diffuse_Lambert(albedo) * E * cosThetaI;

	// specular
	float V = normalize(g_vCameraPosition.xyz - position);
	float3 specular = MicrofacetSpecular(f0, roughness, V, normal, L) * E * cosThetaI;

	output.color = float4(diffuse + specular, 1.0);

	return output;
}