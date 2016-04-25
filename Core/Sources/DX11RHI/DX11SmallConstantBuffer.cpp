#include "stdafx.h"
#include "DX11SmallConstantBuffer.h"


DX11SmallConstantBuffer::DX11SmallConstantBuffer(DX11Device* dev, void* mem, uint32_t sizeInBytes)
{
	_bufferSizeInBytes = sizeInBytes;
	_cache = (float4*)_aligned_malloc(_cacheMemAlignment, sizeInBytes);
	_cbuffer = dev->createConstantBuffer(mem, sizeInBytes);
}

DX11SmallConstantBuffer::~DX11SmallConstantBuffer()
{
	_aligned_free(_cache);
}

void DX11SmallConstantBuffer::setVectorF(uint32_t reg, DirectX::FXMVECTOR vec)
{
	assert((reg * sizeof(float4) + sizeof(float4)) <= _bufferSizeInBytes);
	float4* ptr = _cache + reg;
	DirectX::XMStoreFloat4(ptr, vec);
}

void DX11SmallConstantBuffer::setVectorI(uint32_t reg, DirectX::XMVECTORI32 vec)
{
	assert((reg * sizeof(float4) + sizeof(int4)) <= _bufferSizeInBytes);
	int4* ptr = reinterpret_cast<int4*>(_cache + reg);
	DirectX::XMStoreSInt4(ptr, vec);
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
