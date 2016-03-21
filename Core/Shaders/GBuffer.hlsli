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
	output.Normal.xyz = Normal;
	return output;
}