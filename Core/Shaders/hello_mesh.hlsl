
cbuffer TransformMatrixs
{
	float4x4 modelViewProjMatrix;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

PSInput VSMain(VSInput input)
{
	PSInput result;

	result.position = mul(float4(input.position, 1), modelViewProjMatrix);
	result.normal = input.normal;
	result.texCoord = input.texCoord;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return float4(1, 0, 0, 0.5);
}