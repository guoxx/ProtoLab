#include "BaseMaterial_common.h"
#include "PointLight.hlsli"


TextureCube g_CubeShadowMap;

SamplerComparisonState ShadowSampler;

PSOutput main(PSInput input)
{
	PSOutput result;
	float d = length(lightPositionInCameraSpace.xyz - input.positionCS);
	float3 L = normalize(lightPositionInCameraSpace.xyz - input.positionCS);
	float3 V = normalize(-input.positionCS);
	float3 H = normalize(V + L);
	float3 N = normalize(input.normalCS);
	float3 E = pointLightIrradiance(intensity.xyz, d, radiusStart, radiusEnd);
	float cosineTheaI = max(0, dot(N, L));
	float3 diffuseLight = Diffuse_Lambert(diffuse.xyz) * E * cosineTheaI;

	float3 LightVector = input.positionWS.xyz - lightPositionInWorldSpace.xyz;
	float3 NormalizedLightVector = normalize(NormalizedLightVector);
	float3 AbsLightVector = abs(LightVector);
	float MaxCoordinate = max(AbsLightVector.x, max(AbsLightVector.y, AbsLightVector.z));
	int CubeFaceIndex = 0;
	if (MaxCoordinate == AbsLightVector.x)
	{
		CubeFaceIndex = AbsLightVector.x == LightVector.x ? 0 : 1;
	}
	else if (MaxCoordinate == AbsLightVector.y)
	{
		CubeFaceIndex = AbsLightVector.y == LightVector.y ? 2 : 3;
	}
	else
	{
		CubeFaceIndex = AbsLightVector.z == LightVector.z ? 4 : 5;
	}

	// Transform the position into light space
	float4 ShadowPosition = mul(float4(input.positionWS.xyz, 1), mViewProjInLightSpace[CubeFaceIndex]);
	float shadowDepth = ShadowPosition.z / ShadowPosition.w;
	float lightVisibility = g_CubeShadowMap.SampleCmpLevelZero(ShadowSampler, LightVector, shadowDepth);
	E = E * lightVisibility;
	diffuseLight = diffuseLight * lightVisibility;

#if 1
	float Roughness = (100 - shininess)/100; // test
	float3 specularLight = MicrofacetSpecular(specular.xyz, Roughness, V, N, L) * E * cosineTheaI;
#else
	float cosineTheaH = max(0, dot(N, H));
	float specularPower = shininess;
	float3 specularLight = (specularPower + 8.0) / (8.0 * PI) * pow(cosineTheaH, specularPower) * E * specular.xyz;
#endif
	result.color.rgb = diffuseLight + specularLight;
	result.color.a = 1.0;
	return result;
}