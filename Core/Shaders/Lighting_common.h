#include "FilterIdentity_common.h"

ConstantsBuffer(View, ps, 0)
{
	float4 g_vCameraPosition;
	float4x4 g_mProjInv;
	float4x4 g_mViewInv;
};

ConstantsBuffer(PointLightParam, ps, 1)
{
	float4 g_LightPosition;
	float4 g_LightIntensity;
	float4 g_RadiusStart;
	float4 g_RadiusEnd;
	float4x4 g_mViewProjInLightSpace[6];
};