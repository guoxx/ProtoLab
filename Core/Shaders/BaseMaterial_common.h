#include "HLSLCommon.h"

ConstantsBuffer(View, vs, 0)
{
	float4x4 mModelInvTrans;
	float4x4 mModelViewProj;
};

ConstantsBuffer(Material, ps, 0)
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
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normal	: NORMAL;
	float2 texcoord : TEXCOORD;
};

#endif