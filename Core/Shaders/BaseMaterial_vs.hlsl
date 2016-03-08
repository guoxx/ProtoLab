#include "BaseMaterial_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = mul(float4(input.position, 1), mModelViewProj);
	result.positionCS = mul(float4(input.position, 1), mModelView).xyz;
	result.positionWS = mul(float4(input.position, 1), mModel).xyz;
	result.normalCS = mul(float4(input.normal, 1), mModelViewInvTrans).xyz;
	result.texcoord = input.texcoord;
	return result;
}