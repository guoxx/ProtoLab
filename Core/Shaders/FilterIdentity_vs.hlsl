#include "FilterIdentity_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = float4(input.position, 1);
	result.texcoord = input.texcoord;
	return result;
}