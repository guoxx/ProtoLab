#include "BaseMaterial_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = mul(float4(input.position, 1), modelViewProjMatrix);
	result.positionWS = mul(float4(input.position, 1), modelViewMatrix).xyz;
	result.normal = mul(float4(input.normal, 1), modelViewProjMatrixInvTrans).xyz;
	result.texcoord = input.texcoord;
	return result;
}