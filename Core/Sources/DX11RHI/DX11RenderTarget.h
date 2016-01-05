#pragma once

class DX11RenderTarget : public Noncopyable
{
public:
	DX11RenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	DX11RenderTarget(IDXGISwapChain* swapChain);
	~DX11RenderTarget();

	ID3D11ShaderResourceView* getTextureSRV() const;
	ID3D11RenderTargetView* getRenderTarget(uint32_t mipmap = 0) const;

	uint32_t getNumOfMipmaps();

private:
	ID3D11Texture2D*						_texture{nullptr};
	ID3D11ShaderResourceView*				_textureSRV{nullptr};
	std::vector<ID3D11RenderTargetView*>	_renderTargets{};
};

