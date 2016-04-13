struct GBuffer
{
	float4 Albedo_MatId			: SV_TARGET0;
	float4 F0_Roughness			: SV_TARGET1;
	float4 Normal				: SV_TARGET2;
};

GBuffer EncodeGBuffer(int MatId, float3 Albedo, float3 F0, float3 Normal, float Roughness)
{
	GBuffer output;

	output.Albedo_MatId.a = float(MatId)/255.0f;
	output.Albedo_MatId.rgb = Albedo;

	output.F0_Roughness.rgb = F0;
	output.F0_Roughness.a = Roughness;

	output.Normal.xyz = (Normal + 1) * 0.5;

	return output;
}

GBuffer DecodeGBuffer(Texture2D<float4> GBuffer0, Texture2D<float4> GBuffer1, Texture2D<float4> GBuffer2, SamplerState PointSampler, float2 Texcoord)
{
	GBuffer output;
	output.Albedo_MatId = GBuffer0.SampleLevel(PointSampler, Texcoord, 0);
	output.F0_Roughness = GBuffer1.SampleLevel(PointSampler, Texcoord, 0);
	output.Normal = GBuffer2.SampleLevel(PointSampler, Texcoord, 0);
	return output;
}

int GBufferGetMatId(GBuffer buf)
{
	return int(buf.Albedo_MatId.a * 255.0f);
}

float3 GBufferGetAlbedo(GBuffer buf)
{
	return buf.Albedo_MatId.rgb;
}

float3 GBufferGetF0(GBuffer buf)
{
	return buf.F0_Roughness.rgb;
}

float GBufferGetRoughness(GBuffer buf)
{
	return buf.F0_Roughness.a;
}

float3 GBufferGetNormal(GBuffer buf)
{
	return (buf.Normal.xyz * 2 - 1);
}