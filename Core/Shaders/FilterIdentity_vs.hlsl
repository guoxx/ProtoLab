#include "FilterIdentity_common.h"

VSOutput main(VSInput input)
{
	VSOutput result;
	result.position = float4(input.position, 1);
	result.positionSS = (result.position.xy + 1) * 0.5;
	result.texcoord = input.texcoord;
	return result;
}