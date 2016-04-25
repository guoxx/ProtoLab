#pragma once

class DX11SmallConstantBuffer
{
public:
	DX11SmallConstantBuffer(DX11Device* dev, void* mem, uint32_t sizeInBytes);
	~DX11SmallConstantBuffer();

	void setVectorF(uint32_t reg, DirectX::FXMVECTOR vec);
	void setVectorI(uint32_t reg, DirectX::XMVECTORI32 vec);
	void setVectorU(uint32_t reg, DirectX::XMVECTORU32 vec);
	void setMatrix(uint32_t reg, DirectX::FXMMATRIX matrix);

	void commit(DX11GraphicContext* ctx);

private:
	// aligned to 16 bytes boundaries for better memory copy performache
	constexpr static size_t _cacheMemAlignment = 16;

	uint32_t _bufferSizeInBytes;
	float4* _cache;	
	ComPtr<ID3D11Buffer> _cbuffer;
};

