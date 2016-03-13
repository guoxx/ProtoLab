#include "BaseMaterial_common.h"

VSOutput main(VSInput input)
{
	VSOutput result;
	result.position = mul(float4(input.position, 1), mModelViewProj);
	result.positionCS = mul(float4(input.position, 1), mModelView).xyz;
	result.positionWS = mul(float4(input.position, 1), mModel).xyz;
	result.normalCS = mul(float4(input.normal, 1), mModelViewInvTrans).xyz;
	result.texcoord = input.texcoord;
	return result;
}