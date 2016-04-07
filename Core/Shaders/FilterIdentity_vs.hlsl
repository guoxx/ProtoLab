#include "FilterIdentity_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = float4(input.position, 1);
	result.positionSS = (result.position.xy + 1) * 0.5;
	result.texcoord = input.texcoord;
	return result;
}