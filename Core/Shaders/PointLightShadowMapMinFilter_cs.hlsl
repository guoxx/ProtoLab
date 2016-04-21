
Texture2DArray<float> g_shadowMap : register(t0);
RWTexture2DArray<float> g_mipmapShadowMap : register(1);
SamplerState g_pointSampler : register(s0);

[numthreads(16, 16, 1)]
void main(uint3 GroupId : SV_GroupID, uint3 GroupThreadId : SV_GroupThreadID, uint3 DispatchThreadId : SV_DispatchThreadID)
{
}