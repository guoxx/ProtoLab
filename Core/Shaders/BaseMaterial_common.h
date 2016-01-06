#include "HLSLCommon.h"

cbuffer View
#ifdef __HLSL__
	: register (b0)
#endif
{
	float4x4 mModelView;
	float4x4 mModelViewProj;
	float4x4 mModelViewInvTrans;
};

cbuffer Materiel
#ifdef __HLSL__
	: register (b0)
#endif
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
#ifdef __HLSL__
	: register (b1)
#endif
{
	// TODO: use float3 if possible
	float4 lightPositionInCameraSpace;
	float4 intensity;
	float radiusStart;
	float radiusEnd;
};

#ifdef __HLSL__
struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texcoord : TEXCOORD;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normalCS : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 positionCS : POSITION1;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
};

#endif