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

void DX11SmallConstantBuffer::setVectorF(DirectX::FXMVECTOR vec)
{
}

void DX11SmallConstantBuffer::setVectorI(DirectX::XMVECTORI32 vec)
{
}

void DX11SmallConstantBuffer::setVectorU(DirectX::XMVECTORU32 vec)
{
}

void DX11SmallConstantBuffer::setMatrix(DirectX::FXMMATRIX matrix)
{
}

void DX11SmallConstantBuffer::commit(DX11GraphicContext* ctx)
{
}
