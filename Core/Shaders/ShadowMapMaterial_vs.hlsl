#include "ShadowMapMaterial_common.h"

PSInput main(VSInput input)
{
	PSInput result;
	result.position = mul(float4(input.position, 1), mModelViewProj);
	return result;
}