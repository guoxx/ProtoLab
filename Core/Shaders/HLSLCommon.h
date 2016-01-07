
#ifndef __cplusplus
#define __HLSL__
#endif

#if defined(_MSC_VER)

// VC++
#define cbuffer struct __declspec(align(16))

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

// HLSL
const static float C_PI           = 3.141592654f;
const static float C_2PI          = 6.283185307f;
const static float C_1DIVPI       = 0.318309886f;
const static float C_1DIV2PI      = 0.159154943f;
const static float C_PIDIV2       = 1.570796327f;
const static float C_PIDIV4       = 0.785398163f;

#endif
