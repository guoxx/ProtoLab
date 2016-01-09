#include "HLSLCommon.h"

ConstantsBuffer(View, vs, 0)
{
	float4x4 mModelView;
	float4x4 mModelViewProj;
};

ConstantsBuffer(PointLight, ps, 0)
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
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 positionCS : POSITION1;
};

struct PSOutput
{
	float4 color : SV_TARGET0;
};

#endif