#include "stdafx.h"
#include "DX11RenderTarget.h"


DX11RenderTarget::DX11RenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat)
{
	_texture = RHI::getInst().createTexture2D(width, height, numMipmap, texFormat, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
	_textureSRV = RHI::getInst().createShaderResourceViewTex2d(_texture, srvFormat, numMipmap);
	for (uint32_t mipSlice = 0; mipSlice < numMipmap; ++mipSlice)
	{
		ID3D11RenderTargetView* rtv = RHI::getInst().createRenderTargetViewTex2d(_texture, rtvFormat, mipSlice);
		_renderTargets.push_back(rtv);
	}
}

DX11RenderTarget::DX11RenderTarget(IDXGISwapChain* swapChain)
{
	_texture = RHI::getInst().createTexture2DFromSwapChain(swapChain);

	D3D11_TEXTURE2D_DESC texDesc;
	_texture->GetDesc(&texDesc);
	DXGI_FORMAT srvFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
	uint32_t numMipmap = texDesc.MipLevels;

	_textureSRV = nullptr;
	for (uint32_t mipSlice = 0; mipSlice < numMipmap; ++mipSlice)
	{
		ID3D11RenderTargetView* rtv = RHI::getInst().createRenderTargetViewTex2d(_texture, srvFormat, mipSlice);
		_renderTargets.push_back(rtv);
	}
}

DX11RenderTarget::~DX11RenderTarget()
{
	RHI::getInst().destroyResource(_texture);
	RHI::getInst().destroyView(_textureSRV);

	for (auto rtv : _renderTargets)
	{
		RHI::getInst().destroyView(rtv);
	}
}

ID3D11ShaderResourceView * DX11RenderTarget::getTextureSRV() const
{
	return _textureSRV;
}

ID3D11RenderTargetView* DX11RenderTarget::getRenderTarget(uint32_t mipmap) const
{
	return _renderTargets[mipmap];
}


uint32_t DX11RenderTarget::getNumOfMipmaps()
{
	return static_cast<uint32_t>(_renderTargets.size());
}
