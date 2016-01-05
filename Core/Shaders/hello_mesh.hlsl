#include "PointLight.hlsl"

cbuffer View
{
	float4x4 modelViewMatrix;
	float4x4 modelViewProjMatrix;
};

cbuffer Materiel
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

cbuffer PointLight
{
	float3 lightPositionInWorldSpace;
	float3 intensity;
	float radiusStart;
	float radiusEnd;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
	float3 positionWS : POSITION1;
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
	result.positionWS = mul(float4(input.position, 1), modelViewMatrix);
	result.normal = input.normal;
	result.texCoord = input.texCoord;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	float r = distance(lightPositionInWorldSpace - input.positionWS);
	float3 E = pointLightIrradiance(intensity, r, radiusStart, radiusEnd);
	return float4(diffuse.rbg * E, 1.0);
}