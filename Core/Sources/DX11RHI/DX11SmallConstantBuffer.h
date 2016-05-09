#pragma once

class DX11SmallConstantBuffer
{
public:
	DX11SmallConstantBuffer(DX11Device* dev, void* mem, uint32_t sizeInBytes);
	~DX11SmallConstantBuffer();

	void setVectorF(uint32_t reg, float f0 = 0.0f, float f1 = 0.0f, float f2 = 0.0f, float f3 = 0.0f);
	void setVectorF(uint32_t reg, DirectX::FXMVECTOR vec);

	void setVectorI(uint32_t reg, int32_t i0 = 0, int32_t i1 = 0, int32_t i2 = 0, int32_t i3 = 0);
	void setVectorI(uint32_t reg, DirectX::XMVECTORI32 vec);

	void setVectorU(uint32_t reg, uint32_t u0 = 0, uint32_t u1 = 0, uint32_t u2 = 0, uint32_t u3 = 0);
	void setVectorU(uint32_t reg, DirectX::XMVECTORU32 vec);

	void setMatrix(uint32_t reg, DirectX::FXMMATRIX matrix);

	void commit(DX11GraphicContext* ctx);

	ID3D11Buffer* getBuffer()
	{
		return _cbuffer.Get();
	}

private:
	// aligned to 16 bytes boundaries for better memory copy performache
	constexpr static size_t _cacheMemAlignment = 16;

	uint32_t _bufferSizeInBytes;
	float4* _cache;	
	ComPtr<ID3D11Buffer> _cbuffer;
};

