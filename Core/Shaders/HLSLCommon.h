#ifndef __HLSL_COMMON_H__
#define __HLSL_COMMON_H__

#ifndef __cplusplus
#define __HLSL__
#endif

#define MAT_BASE			1
#define MAT_LIT				2

#define PIXELS_PER_TILE		32

#if defined(_MSC_VER)

// VC++
#define ConstantsBuffer(typename, profile, reg) \
static const uint32_t typename##Reg = reg; \
struct __declspec(align(16)) typename

typedef DirectX::XMINT4		int4;
typedef DirectX::XMINT3		int3;
typedef DirectX::XMINT2		int2;
typedef DirectX::XMUINT4	uint4;
typedef DirectX::XMUINT3	uint3;
typedef DirectX::XMUINT2	uint2;
typedef DirectX::XMFLOAT4X4 float4x4;
typedef DirectX::XMFLOAT4	float4;
typedef DirectX::XMFLOAT3	float3;
typedef DirectX::XMFLOAT2	float2;

#elif defined(__clang__)

// clang
#define cbuffer alignas(16)

#define float4x4	DirectX::XMFLOAT4X4
#define float4		DirectX::XMFLOAT4
#define float3		DirectX::XMFLOAT3
#define float2		DirectX::XMFLOAT2

#else

#define ConstantsBuffer(typename, profile, reg) \
cbuffer typename : register (b##reg)

// HLSL
const static float PI           = 3.141592654f;
const static float DIVPI		= 0.318309886f;

#include "Common/BRDF.hlsli"
#include "Common/GBuffer.hlsli"
#include "Common/PointLight.hlsli"
#include "Common/Utils.hlsli"

#endif

#endif
