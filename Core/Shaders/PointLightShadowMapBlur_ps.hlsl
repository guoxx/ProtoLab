#include "PointLightShadowMapBlur_common.h"


SamplerState g_PointSampler : register(s0);

float2 get_step_size( float2 direction, float3 normal, float depth, float threshold )
{
  return direction 
         //* light_size * light_size //included in the penumbra
         * sqrt( max( dot( float3( 0, 0, 1 ), normal ), threshold ) )
         * (1 / (depth/* * depth * 100*/));
}

float4 main(VSOutput input) : SV_TARGET
{
	return float4(1, 1, 1, 1);
}