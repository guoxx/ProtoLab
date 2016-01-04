
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

#endif
