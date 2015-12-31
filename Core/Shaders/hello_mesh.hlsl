
cbuffer TransformMatrixs
{
	float4x4 modelViewProjMatrix;
};

cbuffer MaterielProp
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 transmittance;
	float4 emission;
	float shininess;
	float ior;      // index of refraction
	float dissolve; // 1 == opaque; 0 == fully transparent
	// illumination model (see http://www.fileformat.info/format/material/)
	int illum;
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
	return float4(diffuse + emission);
}