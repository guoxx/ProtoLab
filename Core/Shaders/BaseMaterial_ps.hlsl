#include "BaseMaterial_common.h"
#include "PointLight.hlsli"

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
	float cosineTheaH = max(0, dot(N, H));
	float3 diffuseLight = Diffuse_Lambert(diffuse.xyz) * E * cosineTheaI;
#if 1
	float Roughness = 0.5; // test
	float3 specularLight = MicrofacetSpecular(specular.xyz, Roughness, V, N, L) * E * cosineTheaI;
#else
	float specularPower = shininess;
	float3 specularLight = (specularPower + 8.0) / (8.0 * PI) * pow(cosineTheaH, specularPower) * E * specular.xyz;
#endif
	result.color.rgb = diffuseLight + specularLight;
	result.color.a = 1.0;
	return result;
}