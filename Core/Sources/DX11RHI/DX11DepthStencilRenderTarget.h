#pragma once

class DX11DepthStencilRenderTarget : public Noncopyable
{
public:
	DX11DepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat);
	~DX11DepthStencilRenderTarget();

	ID3D11ShaderResourceView* getTextureSRV() const;
	ID3D11DepthStencilView* getRenderTarget(uint32_t mipmap = 0) const;

	uint32_t getNumOfMipmaps() const;

private:
	ID3D11Texture2D*						_texture{nullptr};
	ID3D11ShaderResourceView*				_textureSRV{nullptr};	
	std::vector<ID3D11DepthStencilView*>	_depthStencilRenderTargets{};
};

