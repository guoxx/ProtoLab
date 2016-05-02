#include "FilterIdentity_common.h"

Texture2D<float4> tex;
SamplerState texSampler;

float4 main(VSOutput input) : SV_Target
{
	return tex.SampleLevel(texSampler, input.texcoord, 0);
}