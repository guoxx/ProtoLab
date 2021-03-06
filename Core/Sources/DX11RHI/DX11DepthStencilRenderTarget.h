#pragma once

class DX11DepthStencilRenderTarget : public Noncopyable
{
public:
	DX11DepthStencilRenderTarget() = default;
	DX11DepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	~DX11DepthStencilRenderTarget();

	bool initializeAs2D(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	bool initializeAsCube(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);

	ComPtr<ID3D11ShaderResourceView> getTextureSRV() const;
	ComPtr<ID3D11DepthStencilView> getRenderTarget(uint32_t mipmap = 0) const;

	uint32_t getNumOfMipmaps() const;

private:
	ComPtr<ID3D11Texture2D>							_texture{nullptr};
	ComPtr<ID3D11ShaderResourceView>				_textureSRV{nullptr};	
	std::vector<ComPtr<ID3D11DepthStencilView>>		_depthStencilRenderTargets{};
};

