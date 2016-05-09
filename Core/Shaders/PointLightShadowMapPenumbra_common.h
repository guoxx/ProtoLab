#include "HLSLCommon.h"
#include "FilterIdentity_common.h"

ConstantsBuffer(LightData, ps, 0)
{
	float4 g_sampleStep;
	float4 g_dir;
};

#ifdef __HLSL__
#endif