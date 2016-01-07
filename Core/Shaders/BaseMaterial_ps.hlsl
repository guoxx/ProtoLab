#include "BaseMaterial_common.h"
#include "PointLight.hlsli"

PSOutput main(PSInput input)
{
	PSOutput result;
	float d = length(lightPositionInCameraSpace.xyz - input.positionCS);
	float3 L = normalize(lightPositionInCameraSpace.xyz - input.positionCS);
	float3 V = normalize(-input.positionCS);
	float3 H = normalize((V + L) / 2);
	float3 N = normalize(input.normalCS);
	float3 E = pointLightIrradiance(intensity.xyz, d, radiusStart, radiusEnd);
	float cosineTheaI = max(0, dot(N, L));
	float cosineTheaH = max(0, dot(N, H));
	float3 diffuseLight = (E * cosineTheaI / C_PI) * diffuse.xyz;
	float specularPower = shininess;
	float3 specularLight = (specularPower + 8.0) / (8.0 * C_PI) * pow(cosineTheaH, specularPower) * E * specular.xyz;
	result.color.rgb = diffuseLight + specularLight;
	result.color.a = 1.0;
	return result;
}