#include "BaseMaterial_common.h"
#include "PointLight.hlsl"

PSOutput main(PSInput input)
{
	PSOutput result;
	float d = length(input.positionCS - lightPositionInCameraSpace.xyz);
	float3 E = pointLightIrradiance(intensity.xyz, d, radiusStart, radiusEnd);
	result.color = float4(diffuse.xyz * E, 1.0f);
	return result;
}