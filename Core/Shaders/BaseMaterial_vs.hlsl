#include "BaseMaterial_common.h"

VSOutput main(VSInput input)
{
	VSOutput result;
	result.position = mul(float4(input.position, 1), mModelViewProj);
	result.normal = mul(float4(input.normal, 1), mModelInvTrans).xyz;
	result.texcoord = input.texcoord;
	return result;
}