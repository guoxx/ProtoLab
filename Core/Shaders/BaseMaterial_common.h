#include "HLSLCommon.h"

ConstantsBuffer(View, vs, 0)
{
	float4x4 mModel;
	float4x4 mModelView;
	float4x4 mModelViewProj;
	float4x4 mModelViewInvTrans;
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

ConstantsBuffer(PointLight, ps, 1)
{
	float4x4 mViewProjInLightSpace[6];
	float4 lightPositionInCameraSpace;
	float4 lightPositionInWorldSpace;
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

struct VSOutput
{
	float4 position : SV_POSITION;
	float3 normalCS : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 positionCS : POSITION1;
	float3 positionWS : POSITION2;
};

struct GSOutput
{
	float4 position : SV_POSITION;
	float3 normalCS : NORMAL;
	float2 texcoord : TEXCOORD;
	float3 positionCS : POSITION1;
	float3 positionWS : POSITION2;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
};

#endif