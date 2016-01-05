#include "FilterIdentity_common.h"

Texture2D<float4> tex;
SamplerState linearSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = Wrap;
	AddressV = Wrap;
};

PSOutput main(PSInput input)
{
	PSOutput result;
	result.color = tex.SampleLevel(linearSampler, input.texcoord, 0);
	return result;
}