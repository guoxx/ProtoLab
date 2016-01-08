
#ifndef __cplusplus
#define __HLSL__
#endif

#if defined(_MSC_VER)

// VC++
#define ConstantsBuffer(typename, profile, reg) \
static const uint32_t typename##Reg = reg; \
struct __declspec(align(16)) typename

#define float4x4	DirectX::XMFLOAT4X4
#define float4		DirectX::XMFLOAT4
#define float3		DirectX::XMFLOAT3
#define float2		DirectX::XMFLOAT2

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

#include "BRDF.hlsli"

#endif
