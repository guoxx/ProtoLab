struct VSInput
{
	float4 position : SV_POSITION;
	float3 normalCS : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 positionCS : POSITION1;
	float3 positionWS : POSITION2;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normalCS : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 positionCS : POSITION1;
	float3 positionWS : POSITION2;
};

VSOutput main(VSInput input)
{
	VSOutput result;
	result.position = input.position;
	result.positionCS = input.positionCS;
	result.positionWS = input.positionWS;
	result.normalCS = input.normalCS;
	result.texcoord = input.texcoord;
	return result;
}