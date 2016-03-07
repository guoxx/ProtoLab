#include "ShadowMapMaterial_common.h"

VSOutput main(VSInput input)
{
	VSOutput result;
	result.positionWS = mul(float4(input.position, 1), mModel);
	return result;
}