#include "HLSLCommon.h"

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

#ifdef __HLSL__
struct VSInput
{
	float3 position : POSITION;
};

struct PSInput
{
	float4 position : SV_POSITION;
};
#endif
