struct GBuffer
{
	float4 Albedo_MatId : SV_TARGET0;
	float4 Normal		: SV_TARGET1;
};

GBuffer EncodeGBuffer(int MatId, float3 Albedo, float3 Normal)
{
	GBuffer output;
	output.Albedo_MatId.a = float(MatId)/255.0f;
	output.Albedo_MatId.rgb = Albedo;
	output.Normal.xyz = (Normal + 1) * 0.5;
	return output;
}

GBuffer DecodeGBuffer(Texture2D<float4> GBuffer0, Texture2D<float4> GBuffer1, SamplerState PointSampler, float2 Texcoord)
{
	GBuffer output;
	output.Albedo_MatId = GBuffer0.SampleLevel(PointSampler, Texcoord, 0);
	output.Normal = GBuffer1.SampleLevel(PointSampler, Texcoord, 0);
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

float3 GBufferGetNormal(GBuffer buf)
{
	return (buf.Normal.xyz * 2 - 1);
}