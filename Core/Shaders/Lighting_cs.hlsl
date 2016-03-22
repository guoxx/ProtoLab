#include "Lighting.hlsli"

Texture2D<float4> GBuffer0;
Texture2D<float4> GBuffer1;

SamplerState PointSampler;

[numthreads(1, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
}