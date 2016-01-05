#include "BaseMaterial_common.h"
#include "PointLight.hlsl"

PSOutput main(PSInput input)
{
	PSOutput result;
	float r = length(input.positionWS - lightPositionInWorldSpace);
	float3 E = pointLightIrradiance(intensity, r, radiusStart, radiusEnd);
	result.color = float4(diffuse.xyz * E, 1.0f);
	return result;
}