#include "stdafx.h"
#include "DX11DepthStencilRenderTarget.h"


DX11DepthStencilRenderTarget::DX11DepthStencilRenderTarget(uint32_t width, uint32_t height, uint32_t numMipmap, DXGI_FORMAT texFormat, DXGI_FORMAT srvFormat, DXGI_FORMAT rtvFormat)
{
	_texture = RHI::getInst().getDevice()->createTexture2D(width, height, numMipmap, texFormat, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_DEPTH_STENCIL);
	_textureSRV = RHI::getInst().getDevice()->createShaderResourceViewTex2d(_texture, srvFormat, numMipmap);
	for (uint32_t mipSlice = 0; mipSlice < numMipmap; ++mipSlice)
	{
		ComPtr<ID3D11DepthStencilView> rtv = RHI::getInst().getDevice()->createDepthStencilViewTex2d(_texture, rtvFormat, mipSlice);
		_depthStencilRenderTargets.push_back(rtv);
	}
}

DX11DepthStencilRenderTarget::~DX11DepthStencilRenderTarget()
{
}

ComPtr<ID3D11ShaderResourceView> DX11DepthStencilRenderTarget::getTextureSRV() const
{
	return _textureSRV;
}

ComPtr<ID3D11DepthStencilView> DX11DepthStencilRenderTarget::getRenderTarget(uint32_t mipmap) const
{
	return _depthStencilRenderTargets[mipmap];
}


uint32_t DX11DepthStencilRenderTarget::getNumOfMipmaps() const
{
	return static_cast<uint32_t>(_depthStencilRenderTargets.size());
}