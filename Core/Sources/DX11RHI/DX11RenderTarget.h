#pragma once

class DX11RenderTarget : public Noncopyable
{
public:
	DX11RenderTarget() = default;
	DX11RenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	explicit DX11RenderTarget(ComPtr<IDXGISwapChain> swapChain);
	~DX11RenderTarget();

	bool initializeAs2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	bool initializeAs2DArray(uint32_t width, uint32_t height, uint32_t arraySize, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	bool initializeAsCube(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);

	ComPtr<ID3D11ShaderResourceView> getTextureSRV() const;
	ComPtr<ID3D11RenderTargetView> getRenderTarget(uint32_t mipmap = 0) const;

	uint32_t getNumOfMipmaps();

private:
	ComPtr<ID3D11Texture2D>						_texture{nullptr};
	ComPtr<ID3D11ShaderResourceView>			_textureSRV{nullptr};
	std::vector<ComPtr<ID3D11RenderTargetView>>	_renderTargets{};
};

