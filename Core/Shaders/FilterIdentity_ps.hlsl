#include "FilterIdentity_common.h"

Texture2D<float4> tex;
SamplerState texSampler;

PSOutput main(PSInput input)
{
	PSOutput result;
	result.color = tex.SampleLevel(texSampler, input.texcoord, 0);
	return result;
}