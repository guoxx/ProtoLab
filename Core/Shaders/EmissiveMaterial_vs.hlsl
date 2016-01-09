#include "EmissiveMaterial_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = mul(float4(input.position, 1), mModelViewProj);
	result.positionCS = mul(float4(input.position, 1), mModelView).xyz;
	return result;
}