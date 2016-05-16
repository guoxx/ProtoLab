#include "stdafx.h"
#include "DX11SmallConstantBuffer.h"


DX11SmallConstantBuffer::DX11SmallConstantBuffer(DX11Device* dev, void* mem, uint32_t sizeInBytes)
{
	_bufferSizeInBytes = sizeInBytes;
	_cache = (float4*)_aligned_malloc(sizeInBytes, _cacheMemAlignment);
	_cbuffer = dev->createConstantBuffer(mem, sizeInBytes);
}

DX11SmallConstantBuffer::~DX11SmallConstantBuffer()
{
	_aligned_free(_cache);
}

void DX11SmallConstantBuffer::setVectorF(uint32_t reg, float f0, float f1, float f2, float f3)
{
	DirectX::XMVECTOR vec{ f0, f1, f2, f3 };
	setVectorF(reg, vec);
}

void DX11SmallConstantBuffer::setVectorF(uint32_t reg, DirectX::FXMVECTOR vec)
{
	assert((reg * sizeof(float4) + sizeof(float4)) <= _bufferSizeInBytes);
	float4* ptr = _cache + reg;
	DirectX::XMStoreFloat4(ptr, vec);
}

void DX11SmallConstantBuffer::setVectorI(uint32_t reg, int32_t i0, int32_t i1, int32_t i2, int32_t i3)
{
	DirectX::XMVECTORI32 vec{ i0, i1, i2, i3 };
	setVectorI(reg, vec);
}

void DX11SmallConstantBuffer::setVectorI(uint32_t reg, DirectX::XMVECTORI32 vec)
{
	assert((reg * sizeof(float4) + sizeof(int4)) <= _bufferSizeInBytes);
	int4* ptr = reinterpret_cast<int4*>(_cache + reg);
	DirectX::XMStoreSInt4(ptr, vec);
}

void DX11SmallConstantBuffer::setVectorU(uint32_t reg, uint32_t u0, uint32_t u1, uint32_t u2, uint32_t u3)
{
	DirectX::XMVECTORU32 vec{ u0, u1, u2, u3 };
	setVectorU(reg, vec);
}

void DX11SmallConstantBuffer::setVectorU(uint32_t reg, DirectX::XMVECTORU32 vec)
{
	assert((reg * sizeof(float4) + sizeof(uint4)) <= _bufferSizeInBytes);
	uint4* ptr = reinterpret_cast<uint4*>(_cache + reg);
	DirectX::XMStoreUInt4(ptr, vec);
}

void DX11SmallConstantBuffer::setMatrix(uint32_t reg, DirectX::FXMMATRIX matrix)
{
	assert((reg * sizeof(float4) + sizeof(float4x4)) <= _bufferSizeInBytes);
	float4x4* ptr = reinterpret_cast<float4x4*>(_cache + reg);
	DirectX::XMStoreFloat4x4(ptr, DirectX::XMMatrixTranspose(matrix));
}

void DX11SmallConstantBuffer::commit(DX11GraphicContext* ctx)
{
	DX11ResourceMapGuard guard{ ctx, _cbuffer.Get() };
	float4* dataPtr = guard.getPtr<float4>();
	memcpy(dataPtr, _cache, _bufferSizeInBytes);
}
