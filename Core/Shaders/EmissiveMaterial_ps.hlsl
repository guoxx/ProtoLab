#include "EmissiveMaterial_common.h"
#include "PointLight.hlsli"

PSOutput main(PSInput input)
{
	PSOutput result;
	float d = radiusStart;
	float3 E = pointLightIrradiance(intensity.xyz, d, radiusStart, radiusEnd);
	result.color.rgb = E;
	result.color.a = 1.0;
	return result;
}