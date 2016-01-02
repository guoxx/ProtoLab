#pragma once

class DX11DepthStencilRenderTarget : public Noncopyable
{
public:

	ID3D11DepthStencilView* getRenderTarget(uint32_t mipmap = 0);

	uint32_t getNumOfMipmaps();

private:

	friend class DX11RHI;

	DX11DepthStencilRenderTarget();
	~DX11DepthStencilRenderTarget();

	ID3D11Texture2D*						_texture{nullptr};
	std::vector<ID3D11DepthStencilView*>	_depthStencilRenderTargets{};
};

