#pragma once

class DX11RenderTarget : public Noncopyable
{
public:

	ID3D11ShaderResourceView* getTextureSRV() const;
	ID3D11RenderTargetView* getRenderTarget(uint32_t mipmap = 0) const;

	uint32_t getNumOfMipmaps();

private:

	friend class DX11RHI;

	DX11RenderTarget();
	~DX11RenderTarget();

	ID3D11Texture2D*						_texture{nullptr};
	ID3D11ShaderResourceView*				_textureSRV{nullptr};	
	std::vector<ID3D11RenderTargetView*>	_renderTargets{};
};

