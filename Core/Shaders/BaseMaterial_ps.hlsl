#include "BaseMaterial_common.h"
#include "PointLight.hlsl"

PSOutput main(PSInput input)
{
	PSOutput result;
	float r = length(input.positionWS - lightPositionInWorldSpace.xyz);
	float3 E = pointLightIrradiance(intensity.xyz, r, radiusStart, radiusEnd);
	result.color = float4(diffuse.xyz * E, 1.0f);
	return result;
}